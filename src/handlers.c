#include "..\inc\handlers.h"

unsigned char ReceiveByte;   //байт для приема

unsigned char rx_buff[8] = {0};	//буфер для приема запроса
unsigned char rx_i;			//индекс принимаемых байтов

unsigned char UART_counter = 0;

int reg_addr = 0; // Адрес регистра. Необходимо добавить сравнение с define REG для определения формата ответа

extern Cistern cistern_rb, cistern_bsh;
extern Thermocouple temp_oper, temp_aggr, temp_outside;
extern Pump pump_rtr, pump_bsh;

extern enum Mode bub_mode;
extern enum Status bub_status;


void UART1_IRQHandler(void)
{
	if(UART_GetITStatusMasked(UART1, UART_IT_RX) == SET)//проверка установки флага прерывания по окончании приема данных
	{
		//unsigned int CRC;
		
		UART_ClearITPendingBit(UART1, UART_IT_RX);//очистка флага прерывания
					
		ReceiveByte = UART_ReceiveData(UART1); //считываем принятый байт
		
		switch(UART_counter) {
				case 0:
						if(ReceiveByte == ID_BUB) {
								rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
								rx_i++; //инкремент индекса
								UART_counter = 1;
								reg_addr = 0;
						}
						break;
					
				case 1:
						if(ReceiveByte == MODBUS_CODE_03) {
								rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
								rx_i++; //инкремент индекса
								UART_counter = 2;
						} else {
								UART_counter = 0;
								rx_i = 0;
						}
						break;
					
				case 2:
						if(ReceiveByte == 0x0b || ReceiveByte == 0x01) {// Уровень топлива в баках
								reg_addr = reg_addr | (ReceiveByte << 8);
								rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
								rx_i++; //инкремент индекса
								UART_counter = 3;
//								status_request++;
						} else {
								UART_counter = 0;
								rx_i = 0;
						}
						break;
					
				case 3:
						if(ReceiveByte == 0x00 || ReceiveByte == 0x02 || ReceiveByte == 0x04 
							|| ReceiveByte == 0x06) {
								reg_addr = reg_addr | ReceiveByte;
								rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
								rx_i++; //инкремент индекса
								UART_counter = 4;
						} else {
								UART_counter = 0;
								rx_i = 0;
								reg_addr = 0;
						}
						break;
					
				case 4:
						if(ReceiveByte == 0x00) {
								rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
								rx_i++; //инкремент индекса
								UART_counter = 5;
						} else {
								UART_counter = 0;
								rx_i = 0;
						}
						break;
						
				case 5: // Количество считываемых регистров
						if(ReceiveByte == 0x04 || ReceiveByte == 0x02) {
								rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
								rx_i++; //инкремент индекса
								UART_counter = 6;
//								status_request++;
						} else {
								UART_counter = 0;
								rx_i = 0;
						}
						break;
					
				case 6:
						rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
						rx_i++; //инкремент индекса
						UART_counter = 7;
						break;
				
				case 7:
						rx_buff[rx_i] = ReceiveByte; //запись принятого байта в приемный буфер
						unsigned int CRC = CRC16(rx_buff, 6);
						if((rx_buff[6] == (CRC & 0xff)) && (rx_buff[7] == (CRC >> 8))) {
								TIMER_Cmd(MDR_TIMER3, ENABLE);//запуск таймера3
								rx_i = 0; 
								UART_counter = 8;
						} else {
								UART_counter = 0;
								rx_i = 0;
						}
						break;
					
				case 8:
						TIMER_Cmd(MDR_TIMER3, DISABLE);//отключение таймера3
						UART_counter = 0;
						break;
			}
	}	else {
			UART_ClearITPendingBit(UART1, UART_IT_RX);//очистка флага прерывания
	}
}

void Timer1_IRQHandler() 
{
		//Проверка что причина прерывания – обновление таймера 
		if(TIMER_GetITStatus(MDR_TIMER1, TIMER_STATUS_CNT_ARR))
		{
				TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ARR);//Очистка флага прерывания
				InterruptVTimerHandler();
		}
		else
		{
				TIMER_ClearITPendingBit(MDR_TIMER1, TIMER_STATUS_CNT_ARR);//Очистка флага прерывания
		}
}

void Timer2_IRQHandler() 
{
		//Проверка что причина прерывания – обновление таймера 
		if(TIMER_GetITStatus(MDR_TIMER2, TIMER_STATUS_CNT_ARR))
		{
			TIMER_ClearITPendingBit(MDR_TIMER2, TIMER_STATUS_CNT_ARR);//Очистка флага прерывания
			RxON();
			PORT_ResetBits(MDR_PORTA, PORT_Pin_0);//перевод ин10 в режим приемника
			TIMER_Cmd(MDR_TIMER2, DISABLE);//отключение таймера2
				
			UART_counter = 0;
			ResetVTimer(VTIMER3); //сброс значения таймера
		}
		else
		{
				TIMER_ClearITPendingBit(MDR_TIMER2, TIMER_STATUS_CNT_ARR);//Очистка флага прерывания
		}
}

void Timer3_IRQHandler()
{
	//Проверка что причина прерывания – обновление таймера 
		if(TIMER_GetITStatus(MDR_TIMER3, TIMER_STATUS_CNT_ARR))
		{
			TIMER_ClearITPendingBit(MDR_TIMER3, TIMER_STATUS_CNT_ARR);//Очистка флага прерывания
			TIMER_Cmd(MDR_TIMER3, DISABLE);//отключение таймера3
			
			PORT_SetBits(MDR_PORTA, PORT_Pin_0); //перевод ин10 в режим передатчика
			RxOFF();
			//Answer_Process(aggr.current_themperature, oper.current_themperature); //запуск процедуры передачи ответа 
						//на запрос Answer_Process(cistern_rb.scale_value, cistern_bsh.scale_value); - для топлива
			//Answer_Process(cistern_rb.scale_value, cistern_bsh.scale_value);
		
			answer_process(reg_addr, &cistern_bsh, &cistern_rb, &pump_bsh, &pump_rtr, 
										&temp_oper, &temp_aggr, &temp_outside, bub_mode,
										bub_status);
		}
		else
		{
			TIMER_ClearITPendingBit(MDR_TIMER3, TIMER_STATUS_CNT_ARR);//Очистка флага прерывания
		}
}
//End file handlers.c
