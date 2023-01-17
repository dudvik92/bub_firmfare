#include "..\inc\ADC.h"

int read_adc(uint32_t Channel) {
		ADC1_SetChannel(Channel); //установка канала, на котором будет проводиться измерение
		ADC1_Start(); //запуск преобразование АЦП
		while ((ADC1_GetStatus() & ADC_STATUS_FLG_REG_EOCIF) == 0); //ожидание конца преобразования
		return ADC1_GetResult() & 0x0fff; //очистка результата преобразования от лишних данных и его возвращение 
}

float scan_sensor(Sensor *sensor) { // Функция обработки выбранного датчика
		sensor->sum -= sensor->value_arr[sensor->arr_index];               
		sensor->value_arr[sensor->arr_index] = read_adc(sensor->adc_channel);  
		sensor->sum += sensor->value_arr[sensor->arr_index];              
		sensor->value = ((float)(sensor->sum/SIZE_ARR)) * COEFFICIENT_ADC;  

		if(sensor->arr_index < (SIZE_ARR-1)) {
				sensor->arr_index++;
			}	else {
				sensor->arr_index = 0;
		} 
	
		return sensor->value;
}

//End file ADC.c


