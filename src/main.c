/*
Блок управления баками для электростанции Э-351ВМ2

Версия:

Программное обеспечение разработано под 
плату управления баками ЖДИТ.687254.031
печатная плата ЖДИТ.758726.062и2

В текущей версии на стенде проверено:
	- измерение и передача по ModBus текущего уровня топлива в расходном баке и баке шасси
	- управление топлиаоперекачивающими насосами
требует уточнения и проверки:
	- таймаут на ошибку для сухого хода насосов
	- измерение температуры воздуха
	- значения АЦП для идентификации неисправностей датчиков температуры

*/

#include "..\inc\main.h"


#define BTN_ITER_COUNT 5

int btn_refueling = 0;

enum Mode bub_mode;
enum Status bub_status = READY;

Cistern cistern_rb, cistern_bsh;
Pump pump_rtr, pump_bsh;
Thermocouple temp_oper, temp_aggr, temp_outside;

int check_btn(void) {
		static uint16_t count = 0;
		static uint8_t step = 0;
		btn_refueling = 0;
		switch (step) {
				case 0:
						if (!FORCED_INCLUSION) count++; 
						else count = 0;
						if (count >= 10) {
								step++;
								count = 0;
								btn_refueling = 1;
								return 1; // кнопка была нажата
						}
						btn_refueling = 0;
						return 0;
						break;
				
				case 1:			
						if (FORCED_INCLUSION) {
							count++; 
							if (count >= 15) {
										step = 0;
										count = 0;
										btn_refueling = 2;
										return 2; // кнопка была нажата
								}
							btn_refueling = 1;
							return 1;
						} else {
							btn_refueling = 1;
							return 1;
						}	
						break;

		}
		//return 0;
}

void check_switch_control(void) {
		
		if (SWITCH_RTR) {
				bub_mode = RTR_MODE;
		} else if (SWITCH_BSH) {
				bub_mode = BSH_MODE; // BSH_MODE
		} else {
				bub_mode = AUTO_OFF_MODE; // AUTO_OFF_MODE
		}
		check_btn();
}

 
 
void check_status (void) {
		static enum Mode current_bub_mode = AUTO_OFF_MODE;
		if (bub_mode != current_bub_mode) {
				pump_off (&pump_rtr);	
				pump_off (&pump_bsh);	
				current_bub_mode = bub_mode;
				bub_status = READY;
		} 
		switch(bub_status) {
				case READY:
						if (((btn_refueling == 2) || (cistern_rb.lvl_25 == 1)) && (bub_mode != AUTO_OFF_MODE) && (!cistern_rb.sensor->error_sensor)) { // TODO не обрабатывается ручная дозаправка  )
								if (bub_mode == RTR_MODE) {
										if (!pump_on (&pump_rtr, cistern_rb.current_fuel_lvl)) {
												bub_status = REFUELING_RTR;
												break;
										}
								}	else {	
										if (!cistern_bsh.lvl_25) {
												if (!pump_on (&pump_bsh, cistern_rb.current_fuel_lvl)) {
														bub_status = REFUELING_BSH;
												}	
										}
								}
						}	else {
								if ((btn_refueling) && cistern_rb.sensor->error_sensor) {
										bub_status = MANUAL_REFUELING;
										switch (bub_mode) {
												case RTR_MODE:
														pump_on (&pump_rtr, cistern_rb.current_fuel_lvl); 
														break;
												case BSH_MODE:
														pump_on (&pump_bsh, cistern_rb.current_fuel_lvl); 
														break;
										}            
								}
						}
						
						break;
				 
				case REFUELING_RTR:
						if (btn_refueling == 2 || cistern_rb.lvl_90) { 
							pump_off (&pump_rtr);	
							bub_status = READY;
						} else {
								if (check_pump(&pump_rtr, cistern_rb.current_fuel_lvl)) {
										bub_status = READY;
								} else {
										break;
								}
						}
						break;
				case REFUELING_BSH:
						if ((btn_refueling == 2) || cistern_rb.lvl_90 || cistern_bsh.lvl_25) { 
								pump_off (&pump_bsh);
								bub_status = READY;
						} else {
								
								if (check_pump(&pump_bsh, cistern_rb.current_fuel_lvl)) {
										bub_status = READY;
								} else {
										break;
								}
						}
						break;
				case MANUAL_REFUELING:
						if (btn_refueling == 1) {
								break;
						} else {
								switch (bub_mode)	{
										case RTR_MODE:
												pump_off (&pump_rtr); 
												break;
										case BSH_MODE:
												pump_off (&pump_bsh); 
												break;            
								}            
								bub_status = READY;
					 }
						break;
		}
}

void led_bub (void) { // TODO светодиод остаток меньше 25, error pump
//		static unsigned char pump_rtr_flag = 0;
//		static unsigned char pump_bsh_flag = 0;
		static unsigned char cistern_rb_flag = 0;
		static unsigned char cistern_bsh_flag = 0;
					
		if(cistern_bsh.sensor->error_sensor) {//бак шасси
				StartVTimer(VTIMER2);			
				if(GetVTimer(VTIMER2) >= sec/4) {
						if(cistern_bsh_flag) {
								HL_BSH_MINIMAL_ON;//светодиод БШ<25% включен
								cistern_bsh_flag = 0;
						} else {
								HL_BSH_MINIMAL_OFF;//светодиод БШ<25% выключен
								cistern_bsh_flag = 1;
						}				
						StopVTimer(VTIMER2);
				}
		} else {
				StopVTimer(VTIMER2);
				if(cistern_bsh.lvl_25) {
						HL_BSH_MINIMAL_ON;
				} else {
						HL_BSH_MINIMAL_OFF;
				}
		}
		
		if(cistern_rb.error_sensors) {//расходый бак
				StartVTimer(VTIMER1);				
				if(GetVTimer(VTIMER1) >= sec/4)	{
						if(cistern_rb_flag) {
								HL_RB_MINIMAL_ON;//светодид РБ<25% включен
								cistern_rb_flag = 0;
						} else {
								HL_RB_MINIMAL_OFF;//светодиод РБ<25% выключен
								cistern_rb_flag = 1;
						}								
						StopVTimer(VTIMER1);
				}
		}	else {
				StopVTimer(VTIMER1);
				if(cistern_rb.lvl_25) {
						HL_RB_MINIMAL_ON;
				} else {
						HL_RB_MINIMAL_OFF;
				}
		}
//		
		if(pump_bsh.error) {
//				StartVTimer(VTIMER2);	//ToDo: TIMER ?????			
//				if(GetVTimer(VTIMER2) >= sec/4)	{
//						if(pump_bsh_flag) {
								HL_BSH_FAULT_ON;//
//								pump_bsh_flag = 0;
//						} else {
//								HL_BSH_FAULT_OFF;//
//								pump_bsh_flag = 1;
//						}								
//						StopVTimer(VTIMER2);
//				}
		}	else {
//				StopVTimer(VTIMER2);
				HL_BSH_FAULT_OFF;
		}
		
		if(pump_rtr.error) {
//				StartVTimer(VTIMER1);		//ToDo: TIMER ?????				
//				if(GetVTimer(VTIMER1) >= sec/4)	{
//						if(pump_rtr_flag) {
								HL_RTR_FAULT_ON;//
//								pump_rtr_flag = 0;
//						} else {
//								HL_RTR_FAULT_OFF;//
//								pump_rtr_flag = 1;
//						}								
//						StopVTimer(VTIMER1);
//				}
		}	else {
//				StopVTimer(VTIMER1);
				HL_RTR_FAULT_OFF;
	}
//		
}
int main(void)
{
		Init_RST_CLK();//включение тактирования периферии
		Init_PORT();//инициализация портов ввода/вывода
		Init_UART1();//инициализация и включение UART1
		Init_ADC();//инициализация АЦП
		Init_TIMER();//инициализация таймера
		InitVTimers();
		init_scale();
			
		init_pump_rtr(&pump_rtr);
		init_pump_bsh(&pump_bsh);
		
		init_cistern_bsh(&cistern_bsh);
		init_cistern_rb(&cistern_rb);		
		init_scale();
	
		init_thermocouple_oper(&temp_oper);
		init_thermocouple_oper(&temp_aggr);
		init_thermocouple_oper(&temp_outside);
	
		PORT_ResetBits(MDR_PORTC, PORT_Pin_0);//210л нормально-закрыт без питания
		PORT_ResetBits(MDR_PORTC, PORT_Pin_1);//420л нормально-открыт без питания
		
		PORT_ResetBits(MDR_PORTA, PORT_Pin_0);//перевод ин10 в режим приемника
		
		PORT_ResetBits(MDR_PORTA, PORT_Pin_1);//потушить светодиод "420 минимальный уровень"
		PORT_ResetBits(MDR_PORTA, PORT_Pin_2);//потушить светодиод "210 минимальный уровень"

		int cnt = 0;
		while(1) 
		{
				check_switch_control();
			
				if (cnt > 50) { // т.к. вычисление АЦП происходит так себе)) - значение не успевает обновиться 
											//и в первый момент включения устройства всегда устанавливается флаг минимального уровня топлива
						check_status();
				} else {
						cnt ++;
				}
				
				update_fuel_level(&cistern_rb);
				update_fuel_level(&cistern_bsh);
								
				update_themperature_level(&temp_oper);
				update_themperature_level(&temp_aggr);
				update_themperature_level(&temp_outside);
				
				led_bub();
				
		}//END OF WHILE
} // END OF MAIN
