#ifndef MAIN_H
#define MAIN_H

#include "init.h"
#include "stdio.h"
#include "stdlib.h"
#include "VTimers.h"

#include "pump.h"
#include "cistern.h"
#include "thermocouple.h"

#include "CRC16.h"
#include "ADC.h"

#include "connection.h"

#define RS485_RxOFF PORT_ResetBits(MDR_PORTA, PORT_Pin_0);//ин10 режим приемника вылючен

//светодиоды 
#define HL_RB_MINIMAL_OFF PORT_ResetBits(MDR_PORTA, PORT_Pin_1)//светодиод РБ<25% выключен
#define HL_RB_MINIMAL_ON PORT_SetBits(MDR_PORTA, PORT_Pin_1)//светоодиод РБ<25% вклчен
#define HL_BSH_MINIMAL_OFF PORT_ResetBits(MDR_PORTA, PORT_Pin_2)//светодиод БШ<25% выключен  
#define HL_BSH_MINIMAL_ON PORT_SetBits(MDR_PORTA, PORT_Pin_2)//светодид БШ<25% включен
//#define HL_RS485_OFF digitalWrite(PORTA, pin3, 0)//светодиод RS-485 выключен
//#define HL_RS485_ON digitalWrite(PORTA, pin3, 1)//светодиод RS-485 включен
#define HL_RTR_FAULT_OFF PORT_ResetBits(MDR_PORTF, PORT_Pin_3)//светодиод "Авария РТР" выключен 
#define HL_RTR_FAULT_ON PORT_SetBits(MDR_PORTF, PORT_Pin_3)//светодиод "Авария РТР" включен 
#define HL_BSH_FAULT_OFF PORT_ResetBits(MDR_PORTF, PORT_Pin_2)//светодиод "Авария БШ" выключен 
#define HL_BSH_FAULT_ON PORT_SetBits(MDR_PORTF, PORT_Pin_2)//светодиод "Авария БШ" включен 
//#define HL_REFUELING_OFF digitalWrite(PORTF, pin1, 0)//светодиод "дозаправка" выключен 
//#define HL_REFUELING_ON digitalWrite(PORTF, pin1, 1)//светодиод "дозаправка" включен 

//переключатель 
#define SWITCH_RTR PORT_ReadInputDataBit(MDR_PORTA,PORT_Pin_4)//режим 1 (РТР)
#define SWITCH_BSH PORT_ReadInputDataBit(MDR_PORTA,PORT_Pin_5)//режим 2 (БШ)
#define FORCED_INCLUSION PORT_ReadInputDataBit(MDR_PORTA,PORT_Pin_6)//кнопка на принудительное включение насоса БШ/РТР

#undef USE_JTAG_B

enum Status
{
  READY,
  REFUELING_RTR,
  REFUELING_BSH,
  MANUAL_REFUELING,
  UNKWONW
}; 

enum Mode
{
  AUTO_OFF_MODE,
  RTR_MODE,
  BSH_MODE
};


void Level_Control(void);
void Disconnect_Blink(void);
void Connection_Indication_RS485(void);

#endif
