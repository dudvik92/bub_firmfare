/////////////////////////////////////////////////////
//БИБЛИОТЕКА ВИРТУАЛЬНЫХ ТАЙМЕРОВ                  //
//Автор: Ефремов А.А.                              //
/////////////////////////////////////////////////////
#include "..\inc\VTimers.h"

//Timers states
#define TIMER_STOPPED 0 
#define TIMER_RUNNING 1
#define TIMER_PAUSED  2

struct VTimerType
 {
  unsigned int Counter;
  unsigned char State;
 } VTimer[MAX_TIMERS]; 
 
unsigned long milliseconds = 0;
unsigned long microseconds = 0;
  
void InitVTimers(void)
{
 unsigned char i = 0;
	for (i = 0; i < MAX_TIMERS; i++)
	{
		VTimer[i].State = TIMER_STOPPED;
	}
}

void StartVTimer(unsigned char Timer)        //Запуск виртуального таймера
{
 if (VTimer[Timer].State==TIMER_STOPPED)
  {
   VTimer[Timer].Counter=0;
   VTimer[Timer].State=TIMER_RUNNING;
  }
}

void StopVTimer(unsigned char Timer)         //Остановить+сбросить
{
 VTimer[Timer].State=TIMER_STOPPED;
 VTimer[Timer].Counter=0;
}

void ResetVTimer(unsigned char Timer)        //Сброс значения таймера
{
 VTimer[Timer].Counter=0;
}

void PauseVTimer(unsigned char Timer)        //Пауза
{
 if (VTimer[Timer].State==TIMER_RUNNING)
  VTimer[Timer].State=TIMER_PAUSED;
}       

void ReleaseVTimer(unsigned char Timer)      //Продолжить
{
 if (VTimer[Timer].State==TIMER_PAUSED)
  VTimer[Timer].State=TIMER_RUNNING;
}

unsigned int GetVTimer(unsigned char Timer)  //Вернуть текущее значение таймера
{
 return VTimer[Timer].Counter;
}

void InterruptVTimerHandler(void)             //Обработчик прерывания таймера
{//переполнение пока неучитывается!!!!!!!!
	 unsigned char i;
	 for (i = 0; i < MAX_TIMERS; i++) 
	 {
			if (VTimer[i].State==TIMER_RUNNING) 
			{
					VTimer[i].Counter++;
			}
	 }
   /*if (VTimer[i].Counter==0xFF) {VTimer[i].Counter=0;} 
     else {VTimer[i].Counter++;} } }*/
}

void MillisHandler(void)
{
  milliseconds++; 
}

void MicrosHandler(void)
{
	microseconds++;
}

unsigned long millis(void)
{
	return(milliseconds);
}

unsigned long micros(void)
{
	return(microseconds);
}
