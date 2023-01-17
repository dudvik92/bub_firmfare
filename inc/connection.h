#ifndef CONNECTION_H
#define CONNECTION_H

#include "stdlib.h"

#include "pump.h"
#include "cistern.h"
#include "thermocouple.h"

#define UART1 						MDR_UART1
#define ID_BUB        		0x10 // id БУБ
#define MODBUS_CODE_03  	0x3  // код функции modbus

#define FUEL_REG					0x0B00
#define STATUS_REG				0x0100
#define TEMP_OPER_REG			0x0102
#define TEMP_AGGR_REG			0x0104
#define TEMP_OUTSIDE_REG	0x0106

//void ReInit_Connection(void);
unsigned char* floatToHex(float val);
void Answer_Process(float scale_cistern_rb, float scale_cistern_bsh);

/*
void answer_process(Cistern* const cistern_bsh, Cistern* const cistern_rb, 
										Pump* const pump_bsh, Pump* const pump_rtr, 
										Thermocouple* const term_oper, Thermocouple* const term_aggr,
										Thermocouple* const term_outside, enum Mode bub_mode,
										enum Status bub_status);
*/

#endif
