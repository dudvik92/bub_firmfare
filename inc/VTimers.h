/////////////////////////////////////////////////////
//ЗАГОЛОВОЧНЫЙ ФАЙЛ БИБЛИОТЕКИ ВИРТУАЛЬНЫХ ТАЙМЕРОВ//
//Автор: Ефремов А.А.                              //
// - Подключить #include "VTimers.h                //
// - Корректировать макросы единиц времени         //
// - Корректировать макрос MAX_TIMERS              //
// - В handler реального таймера:                  //
//           InterruptVTimerHandler();             //
//           MillisHandler();                      //
//           MicrosHandler();                      //
/////////////////////////////////////////////////////

#ifndef VTIMER_h
#define VTIMER_h

//Определение единиц времени
//частота прерывания по переполнению таймера-счётчика 1953,125 Гц
#define msec 1      // 1мс = 1 такт    
#define sec 1000    // 1с =  1000 тактов 
#define min 60000   // MAX 64 sec
#define hour 60*min 
#define day 24*hour

#define MAX_TIMERS 10 //Кол-во виртуальных таймеров
//Timers ID:
#define VTIMER1   0  
#define VTIMER2   1  
#define VTIMER3   2  
#define VTIMER4   3  
#define VTIMER5   4  
#define VTIMER6   5  
#define VTIMER7   6  
#define VTIMER8   7  
#define VTIMER9   8  
#define VTIMER10  9  
#define VTIMER11  10
#define VTIMER12  11
#define VTIMER13  12
#define VTIMER14  13
#define VTIMER15  14
#define VTIMER16  15

//Функции работы с таймерами:
void InitVTimers(void);                       //Инициализация таймеров
void StartVTimer(unsigned char Timer);        //Запуск виртуального таймера
void StopVTimer(unsigned char Timer);         //Остановить+сбросить
void ResetVTimer(unsigned char Timer);        //Сброс значения таймера
void PauseVTimer(unsigned char Timer);        //Пауза
void ReleaseVTimer(unsigned char Timer);      //Продолжить
unsigned int GetVTimer(unsigned char Timer);  //Вернуть текущее значение таймера
unsigned long millis(void);   
unsigned long micros(void);   

// Обработчик прерывания таймера:
//    - помещать в обработчик прерывания реального тоймера/счётчика
//    - каждое прерывание реального таймера = 1 дискрета виртуального таймера
void InterruptVTimerHandler(void);   
void MillisHandler(void);          //необходимо настроить таймер на 1 тик = 1 мс
void MicrosHandler(void);          //необходимо настроить таймер на 1 тик = 1 мкс

#endif

