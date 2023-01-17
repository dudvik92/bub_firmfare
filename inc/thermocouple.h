#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

//#include "main.h"
#include "ADC.h"

#define DISCONNECT_THERM_LVL 	  1.5 // уровень обрыва датчиков

//ToDo: Изменить каналы
#define CHANNEL_ADC_TEMP_OPER  			ADC_CH_ADC0 //канал АЦП для датчика температуры операторского отсека
#define CHANNEL_ADC_TEMP_AGGR  			ADC_CH_ADC1 //канал АЦП для датчика температуры агрегатного отсека
#define CHANNEL_ADC_TEMP_OUTSIDE  	ADC_CH_ADC5 //канал АЦП для датчика температуры улицы

typedef struct {
		float current_themperature; // Текущий уровень температуры
		uint8_t error_sensor;
		Sensor sensor; // структура датчика 
} Thermocouple;

void init_thermocouple_oper(Thermocouple *oper); // Процедура инициализации структуры для бака шасси
void init_thermocouple_aggr(Thermocouple *aggr); // Процедура инициализации структуры для расходного шасси
void init_thermocouple_outside(Thermocouple *outside);

void update_themperature_level(Thermocouple *thermocouple); // Процедура вычисления уровня температуры

#endif
