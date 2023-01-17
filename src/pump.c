#include "..\inc\pump.h"
#include "..\inc\VTimers.h"
#define CHECK_TIMEOUT 200000 // 5min ToDo: need to correct TIMEOUT

void init_pump_rtr(Pump *pump) {
		pump->turn_on = turn_on_pump_rtr;
		pump->turn_off = turn_off_pump_rtr;
		pump->vtimer = VTIMER7; 
		pump->status = PUMP_OFF;  
}

void init_pump_bsh(Pump *pump) {
		pump->turn_on = turn_on_pump_bsh;
		pump->turn_off = turn_off_pump_bsh;
		pump->vtimer = VTIMER6; // ToDo: need to correct VTIMER
		pump->status = PUMP_OFF;  
}

void turn_on_pump_rtr(void) {
		PORT_SetBits (MDR_PORTC, PORT_Pin_1);
}

void turn_off_pump_rtr(void) {
		PORT_ResetBits (MDR_PORTC, PORT_Pin_1);
}

void turn_on_pump_bsh(void) {
		PORT_SetBits (MDR_PORTC, PORT_Pin_0); 
}

void turn_off_pump_bsh(void) {
		PORT_ResetBits (MDR_PORTC, PORT_Pin_0); 
}

int pump_on (Pump *pump, float current_fuel_lvl) {
		if (pump->error) { // TODO pump->error && !(bub_status == manual_refueling)
				return 1;
		}
		pump->turn_on();
		pump->status = PUMP_WORK;
		StartVTimer(pump->vtimer);
		pump->fuel_lvl_for_check = current_fuel_lvl;  
		return 0;
}

void pump_off (Pump *pump) {
		//int count = 1000; // ??
		pump->turn_off();
		pump->status = PUMP_OFF;
		StopVTimer(pump->vtimer);
		pump->fuel_lvl_for_check = 0; //test
		
//		while (count > 4) { // ??
//				count --;
//		}
}
  
int check_pump(Pump *pump, float current_fuel_lvl) {
    if (current_fuel_lvl > pump->fuel_lvl_for_check) {
				StopVTimer(pump->vtimer);
				StartVTimer(pump->vtimer);
				pump->fuel_lvl_for_check = current_fuel_lvl;
				return 0;
		} else {
				if (GetVTimer(pump->vtimer) > CHECK_TIMEOUT) {
						pump_off (pump);
						pump->error = 1;
						StopVTimer(pump->vtimer);
						return 1;
				} else {
						return 0;
				}
		} 		
}
