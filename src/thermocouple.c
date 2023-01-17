#include "..\inc\main.h"
#include "..\inc\thermocouple.h"

// Только для тестов от стенда
float scal[2][102] = {0}; // Массив для шкал значений АЦП текущего БУБа и БУБа из состава Багаж-30


void init_scal(void) { // Функция инициализации шкалы значений АЦП
		unsigned char i;
		for(i = 0; i < 102; i++) {
				//scal[0][i] = 0.026 + 0.00909 * i;
				scal[0][i] = i;
				scal[1][i] = 4.6 + 0.146 * i;
		}
}
// Только для тестов от стенда

void init_thermocouple_oper(Thermocouple *oper){ // Процедура инициализации структуры для бака шасси
		oper->current_themperature = 0; // Текущий уровень температуры
		oper->error_sensor = 0;
		oper->sensor.error_sensor = 0;
		oper->sensor.adc_channel = CHANNEL_ADC_TEMP_OPER; //
} 
void init_thermocouple_aggr(Thermocouple *aggr){ // Процедура инициализации структуры для расходного шасси
		aggr->current_themperature = 0; // Текущий уровень температуры
		aggr->error_sensor = 0;
		aggr->sensor.error_sensor = 0;
		aggr->sensor.adc_channel = CHANNEL_ADC_TEMP_AGGR; // 
}

void init_thermocouple_outside(Thermocouple *outside){
		outside->current_themperature = 0; // Текущий уровень температуры
		outside->error_sensor = 0;
		outside->sensor.error_sensor = 0;
		outside->sensor.adc_channel = CHANNEL_ADC_TEMP_OUTSIDE; // 
}

void update_themperature_level(Thermocouple *thermocouple){ // Процедура вычисления уровня топлива в баке
		init_scal();
		float resistance_thermocouple = 0;
		
		resistance_thermocouple = scan_sensor(&thermocouple->sensor);
		if (thermocouple->sensor.value > DISCONNECT_LVL) {
				thermocouple->sensor.error_sensor = 1;
		} else {
				thermocouple->sensor.error_sensor = 0;
		}
		
		resistance_thermocouple = ((U_OP * 240)/(U_OP - thermocouple->sensor.value)) - 240;
		
		//thermocouple->current_themperature = resistance_thermocouple / 0.146 + 4.6;
		
		thermocouple->current_themperature = (resistance_thermocouple - 101)/0.4;
		
		
		// Для приведения температуры к процентам стенда
		unsigned char i;
		
		for(i = 0; i < (102 - 1); i++) {
				if((scal[0][i] <= thermocouple->current_themperature ) && (thermocouple->current_themperature  <= scal[0][i+1])) {
						if(scal[0][i+1] == thermocouple->current_themperature ) {
								thermocouple->current_themperature  = scal[1][i+1];
						} else {
								thermocouple->current_themperature = scal[1][i];
						}
				}	
		}
		
}
