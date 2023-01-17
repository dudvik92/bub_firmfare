#include "..\inc\connection.h"
#include "..\inc\CRC16.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_uart.h"


#define LEN_MESSAGE_STATUS				9
#define LEN_MESSAGE_TEMPERATURE		9
#define LEN_MESSAGE_FUEL					13


unsigned char tx_buff[13] = {ID_BUB, MODBUS_CODE_03};	//буфер для передачи ответа

//unsigned char tx_buff_status[] = {0x10, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0xFB, 0x32};	//буфер для передачи ответа

unsigned char* hex_value;						//указатель для размещения результата преобразования float в hex
unsigned char* a;							//указатель для размещения результата преобразования float в hex для бака 420л
unsigned char* b;						    //указатель для размещения результата преобразования float в hex для бака 210л

extern enum Status bub_status;

//extern unsigned char rx_buff[8];								//буфер для приема запроса
//extern int reg_addr; // Адрес регистра. Необходимо добавить сравнение с define REG для определения формата ответа
//extern int number_reg; // Количество запрашиваемых регистров. Для определения длины массива для ответа


unsigned char* float_to_hex(float val)
{
		hex_value = malloc(sizeof(float)); //динамическое выделение памяти под входной параметр
		
		//расфасовка частей(байтов) входного параметра по областям выделенной памяти
		hex_value[0] = ((unsigned char*)&val)[0];  
		hex_value[1] = ((unsigned char*)&val)[1];
		hex_value[2] = ((unsigned char*)&val)[2];
		hex_value[3] = ((unsigned char*)&val)[3];
		
		return hex_value; //возвращение адреса выделенной памяти
}

void send_temp_value (Thermocouple* const term) {
		unsigned char i;
		unsigned int CRC;
	
		a = float_to_hex(term->current_themperature); //размещение результата преобразования float в hex
						
	//формирование пакета для передачи (по хорошему было в формате Mid-Little Endian(CDAB), 
	//но теперь пересобираем пакет в  Big Endian (ABCD) чтобы подстроиться под показания старого БУБа)
	
//		tx_buff[1] = rx_buff[1]; 	// ToDo: сделать отдельную переменную для функционального кода, 
//															//чтоб не тащить в этот файл весь массив
		
		//tx_buff[0] = 0x10;
		//tx_buff[1] = 0x03;
		
		tx_buff[2] = 0x04; // Длина ответа
		tx_buff[3] = a[1];
		tx_buff[4] = a[0];
		tx_buff[5] = a[3];
		tx_buff[6] = a[2];
		CRC = CRC16(tx_buff, 7);
		tx_buff[7] = CRC & 0xff;
		tx_buff[8] = CRC >> 8;
		
	//отправка пакета
		for(i = 0; i < LEN_MESSAGE_TEMPERATURE; i++)	{
				UART_SendData(UART1, tx_buff[i]); //запуск передачи байта
				while (UART_GetFlagStatus(UART1, UART_FLAG_TXFF)); //ожидание успешной передачи байта
		}
		
		free(a);
		//ToDo: Сделать очистку массива tx_buff
}

void send_bub_status (Cistern* const cistern_bsh, Cistern* const cistern_rb, 
										Pump* const pump_bsh, Pump* const pump_rtr, 
										enum Mode bub_mode, enum Status bub_status){
		
		unsigned char i;
		unsigned int CRC;

		tx_buff[2] = 0x04; // Длина ответа
		tx_buff[3] = 0x00;
		tx_buff[4] = 0x00;
		tx_buff[5] = 0x00;
											
		tx_buff[6] = 0x00; // Status READY
		if (bub_status == REFUELING_RTR) {
			tx_buff[6] = tx_buff[6] | (1 << 0); // REFUELING_RTR
		} else if (bub_status == REFUELING_BSH) {
			tx_buff[6] = tx_buff[6] | (1 << 1); // REFUELING_BSH
		} 
		if (bub_mode == RTR_MODE)	{
			tx_buff[6] = tx_buff[6] | (1 << 2); // RTR_MODE
		} else if (bub_mode == BSH_MODE) {
			tx_buff[6] = tx_buff[6] | (1 << 3); // BSH_MODE
		}
		tx_buff[6] = tx_buff[6] | (!cistern_rb->error_sensors << 4); // error sensor rb
		tx_buff[6] = tx_buff[6] | (!cistern_bsh->error_sensors << 5); // error sensor bsh
		tx_buff[6] = tx_buff[6] | (!pump_rtr->error << 6); // error pump rtr
		tx_buff[6] = tx_buff[6] | (!pump_bsh->error << 7); // error pump bsh
		
		CRC = CRC16(tx_buff, 7);
		tx_buff[7] = CRC & 0xff;
		tx_buff[8] = CRC >> 8;		
					
		//отправка пакета
		for(i = 0; i < LEN_MESSAGE_STATUS; i++) {
				UART_SendData(UART1, tx_buff[i]); //запуск передачи байта
				while (UART_GetFlagStatus(UART1, UART_FLAG_TXFF)); //ожидание успешной передачи байта
		}
}

void send_fuel_level (Cistern* const cistern_bsh, Cistern* const cistern_rb){
		unsigned char i;
		unsigned int CRC;
	
		a = float_to_hex(cistern_rb->scale_value); //размещение результата преобразования float в hex
		b = float_to_hex(cistern_bsh->scale_value); //размещение результата преобразования float в hex
						
	//формирование пакета для передачи (по хорошему было в формате Mid-Little Endian(CDAB), 
	//но теперь пересобираем пакет в  Big Endian (ABCD) чтобы подстроиться под показания старого БУБа)
	
//		tx_buff[1] = rx_buff[1];
		
	
		tx_buff[2] = 0x08;
		tx_buff[3] = a[1];
		tx_buff[4] = a[0];
		tx_buff[5] = a[3];
		tx_buff[6] = a[2];
		tx_buff[7] = b[1];
		tx_buff[8] = b[0];
		tx_buff[9] = b[3];
		tx_buff[10]= b[2];
		CRC = CRC16(tx_buff, 11);
		tx_buff[11] = CRC & 0xff;
		tx_buff[12] = CRC >> 8;
						
		//отправка пакета
		for(i = 0; i < LEN_MESSAGE_FUEL; i++)	{
				UART_SendData(UART1, tx_buff[i]); //запуск передачи байта
				while (UART_GetFlagStatus(UART1, UART_FLAG_TXFF)); //ожидание успешной передачи байта
		}
							
		free(a); free(b); //очистка динамически выделенной памяти 
}

void answer_process(int reg_addr, Cistern* const cistern_bsh, Cistern* const cistern_rb, 
										Pump* const pump_bsh, Pump* const pump_rtr, 
										Thermocouple* const temper_oper, Thermocouple* const temper_aggr,
										Thermocouple* const temper_outside, enum Mode bub_mode,
										enum Status bub_status) {
		
		if (reg_addr == STATUS_REG) { // ToDo: Сделать четкое понимание какой запрос 
			
					send_bub_status (cistern_bsh, cistern_rb, pump_bsh, pump_rtr, 
													bub_mode, bub_status);		
			
		}	else if (reg_addr == FUEL_REG) {
		
				send_fuel_level (cistern_bsh, cistern_rb);
		
		} else if (reg_addr == TEMP_OPER_REG) {
		
				send_temp_value(temper_oper);
		
		} else if (reg_addr == TEMP_AGGR_REG) {
		
				send_temp_value(temper_aggr);
		
		} else if (reg_addr == TEMP_OUTSIDE_REG) {
		
				send_temp_value(temper_outside);
		
		}
		
		TIMER_Cmd(MDR_TIMER2, ENABLE);//запуск таймера2											
}


//End file connection.c
