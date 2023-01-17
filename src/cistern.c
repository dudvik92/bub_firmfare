
#include "..\inc\ADC.h"
#include "..\inc\cistern.h"

#define SIZE_SCALE_ARR  102
float scale[2][SIZE_SCALE_ARR] = {0}; // Массив для шкал значений АЦП текущего БУБа и БУБа из состава Багаж-30

void init_scale(void) { // Функция инициализации шкалы значений АЦП
		unsigned char i;
		for(i = 0; i < 102; i++) {
				scale[0][i] = new_BUB_min + new_BUB_step * i;
				scale[1][i] = old_BUB_min + old_BUB_step * i;
		}
}


void init_cistern_bsh(Cistern *bsh) { // 
		bsh->number_sensors = 1; // Сделать define!
		bsh->current_fuel_lvl = DISCONNECT_LVL;
		bsh->lvl_25 = 0;
		bsh->lvl_90 = 0;
		bsh->sensor->error_sensor = 0;
		bsh->sensor->adc_channel = CHANNEL_ADC_BSH;
}

void init_cistern_rb(Cistern *rb) {
		rb->number_sensors = 2; // Сделать define!
		rb->current_fuel_lvl = DISCONNECT_LVL;
		rb->lvl_25 = 0;
		rb->lvl_90 = 0;
		rb->sensor[0].error_sensor = 0;
		rb->sensor[0].adc_channel = CHANNEL_ADC_RB1;
		rb->sensor[1].error_sensor = 0;
		rb->sensor[1].adc_channel = CHANNEL_ADC_RB2; 
	
}

void update_fuel_level(Cistern *cistern) {
		unsigned char i = 0;
		float total_fuel_level = 0;
		unsigned char count_working_sens = 0;
		while (i < (cistern->number_sensors)) { //(cistern->number_sensors)
				scan_sensor(&cistern->sensor[i]);
				if (cistern->sensor[i].value > DISCONNECT_LVL) {
						cistern->sensor[i].error_sensor = 1;
				} else {
						cistern->sensor[i].error_sensor = 0;
						count_working_sens += 1;
						total_fuel_level += cistern->sensor[i].value;
				}
				i += 1;
		} 
		
		if (count_working_sens) {
				cistern->current_fuel_lvl = total_fuel_level / count_working_sens;
				cistern->error_sensors = 0; // ToDo: Define value 0/1 ON/ERR!!
		} else {
				cistern->error_sensors = 1;
				cistern->current_fuel_lvl = -1; // ToDo: определить правильное значение уровня топлива для аварийных ситуаций
		}
			
		
    if (cistern->current_fuel_lvl <= MINIMAL_LVL) {
				cistern->lvl_25 = 1;
		} else {
				cistern->lvl_25 = 0;
		}
		
		if (cistern->current_fuel_lvl >= MAX_LVL) {
				cistern->lvl_90 = 1;
		} else {
				cistern->lvl_90 = 0;
		}
		find_scale_value(cistern);
}


// ToDo: Процедуры не переписаны! 
// 				Необходимо: - изменить функции отправки данных по RS-485 с учетом того, что переменные находятся в структурах
//										
//				NB: Изменить логику работы! отказаться от масиива шкалы! Реализовать алгоритмически преобразование в старые значения


void find_scale_value(Cistern *cistern) {
		unsigned char i;
		
		for(i = 0; i < (SIZE_SCALE_ARR - 1); i++) {
				if((scale[0][i] <= cistern->current_fuel_lvl) && (cistern->current_fuel_lvl <= scale[0][i+1])) {
						if(scale[0][i+1] == cistern->current_fuel_lvl) {
								cistern->scale_value = scale[1][i+1];
						} else {
								cistern->scale_value = scale[1][i];
						}
				}	
		}
}

