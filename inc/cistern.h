#ifndef CISTERN_H
#define CISTERN_H

#include "main.h"
#include "ADC.h"

#define DISCONNECT_LVL   1.5 // уровень обрыва датчиков
#define MINIMAL_LVL      0.244 // 25% топлива 
#define MAX_LVL          0.8784 // 90% топлива 

//Подстройка под БУБ из состава Багаж-30
#define new_BUB_max    	 0.935
#define old_BUB_max      19.2

#define new_BUB_min      0.026
#define old_BUB_min      4.6

#define new_BUB_step     0.00909
#define old_BUB_step   	 0.146

#define CHANNEL_ADC_RB1   ADC_CH_ADC2 //канал АЦП для датчика в основном баке 
#define CHANNEL_ADC_RB2  	ADC_CH_ADC3 //канал АЦП для датчика в основном баке 
#define CHANNEL_ADC_BSH  	ADC_CH_ADC6 //канал АЦП для датчика в баке шасси 

typedef struct {
		float current_fuel_lvl; // Текущий уровень топлива в долях АЦП ToDo: перевести в проценты или литры
		float scale_value;
		uint8_t lvl_25;
		uint8_t lvl_90;
		uint8_t number_sensors; // Количество датчиков в баке
		uint8_t error_sensors;
		Sensor sensor[2]; // Массив структур датчиков 
} Cistern;

void init_cistern_bsh(Cistern *bsh); // Процедура инициализации структуры для бака шасси
void init_cistern_rb(Cistern *rb); // Процедура инициализации структуры для расходного шасси
void update_fuel_level(Cistern *cistern); // Процедура вычисления уровня топлива в баке

void init_scale(void); // Процедура инициализации шкалы уровня топлива (подстройка под значения БУБа из состава Багаж-30)
void find_scale_value(Cistern *cistern);

#endif
