#ifndef PUMP_H
#define PUMP_H

#include "main.h"
#include "cistern.h"
//#include "connection.h"

#define PUMP_WORK 1
#define PUMP_OFF 0

typedef struct
{
  uint8_t error;
	uint8_t status;
	float fuel_lvl_for_check;
	unsigned char vtimer;
	void(*turn_on)(void);
	void(*turn_off)(void);
}Pump;

//typedef struct
//{
//  uint8_t current_fuel_lvl;
//  uint8_t lvl_25;
//  uint8_t lvl_90;
//  uint8_t error_sensor;
//}Cistern;

void init_pump_rtr(Pump *pump);
void init_pump_bsh(Pump *pump);

void turn_on_pump_rtr(void);
void turn_off_pump_rtr(void);

void turn_on_pump_bsh(void);
void turn_off_pump_bsh(void);

int pump_on (Pump *pump, float current_fuel_lvl);
void pump_off (Pump *pump);

int check_pump(Pump *pump, float current_fuel_lvl);


#endif
