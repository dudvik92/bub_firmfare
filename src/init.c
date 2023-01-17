#include "..\inc\init.h"

void Init_RST_CLK(void)
{
	RST_CLK_DeInit();
    RST_CLK_HSEconfig(RST_CLK_HSE_ON);                      // External oscillator on
    while(!(RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY))){;} // Waiting if (RST_CLK_HSEstatus() != SUCCESS){  Trap  while (1) {}} //
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10); // configures the CPU_PLL clock source and multiplication factor - 80 MHz
    RST_CLK_CPU_PLLcmd(ENABLE);                             // PLL on
    while(!(RST_CLK_GetFlagStatus(RST_CLK_FLAG_HSERDY))){;} // Waiting  if (RST_CLK_CPU_PLLstatus() != SUCCESS) {  Trap  while (1) {}}
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);            // Configures the CPU_C3_SEL division factor 
    RST_CLK_CPU_PLLuse(ENABLE);                             // Select the CPU_PLL output as input for CPU_C2_SEL
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);          // Select the HCLK clock source
		
	//// Enable peripheral clocks:    	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_RST_CLK, ENABLE);  
}
//-----------------------------------------//
void Init_PORT(void)
{	
	PORT_InitTypeDef PortInit;
	
	//// Enable the RTCHSE clock on PORTs:
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTA, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTC, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTD, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTE, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTF, ENABLE);
	
	//Конфигурция ножек для управления клапанами
	PORT_StructInit (&PortInit);
	PortInit.PORT_Pin = PORT_Pin_0 | PORT_Pin_1;
	PortInit.PORT_MODE = PORT_MODE_DIGITAL;
	PortInit.PORT_FUNC = PORT_FUNC_PORT;
	PortInit.PORT_OE = PORT_OE_OUT;
	PortInit.PORT_PD = PORT_PD_DRIVER;
	PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PortInit.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init (MDR_PORTC, &PortInit);
	
	//Конфигурация ножек для управления режимами ИН10 и светодиодами
	PortInit.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3;
	PortInit.PORT_OE = PORT_OE_OUT; // Конфигурация группы выводов как выход
	PortInit.PORT_FUNC = PORT_FUNC_PORT; // Работа а режиме порта ввода-вывода
	PortInit.PORT_MODE = PORT_MODE_DIGITAL; // Цифровой режим
	PortInit.PORT_SPEED = PORT_SPEED_SLOW; // Низкая частота тактования порта
	PORT_Init(MDR_PORTA, &PortInit); //Инициализация порта объявленной структурой
	
		
	//Конфигурация ножек для управления обогревом в баках РЕ6,РЕ7 - 210,420 соответственно
	PortInit.PORT_Pin = PORT_Pin_6 | PORT_Pin_7;
	PortInit.PORT_OE = PORT_OE_OUT; // Конфигурация группы выводов как выход
	PortInit.PORT_FUNC = PORT_FUNC_PORT; // Работа а режиме порта ввода-вывода
	PortInit.PORT_MODE = PORT_MODE_DIGITAL; // Цифровой режим
	PortInit.PORT_SPEED = PORT_SPEED_SLOW; // Низкая частота тактования порта
	PORT_Init(MDR_PORTE, &PortInit); //Инициализация порта объявленной структурой
	
	//Конфигурация ножек для обеспечения работы UART1
	PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PortInit.PORT_PD_SHM = PORT_PD_SHM_OFF;
	PortInit.PORT_PD = PORT_PD_DRIVER;
	PortInit.PORT_GFEN = PORT_GFEN_OFF;
	PortInit.PORT_FUNC = PORT_FUNC_ALTER;
	PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
	PortInit.PORT_MODE = PORT_MODE_DIGITAL;
	// Конфигурация 5 ножки порта PORTB как выхода (UART1_TX) 
	PortInit.PORT_OE = PORT_OE_OUT;
	PortInit.PORT_Pin = PORT_Pin_5;
	PORT_Init(MDR_PORTB, &PortInit);
	// Конфигурация 6 ножки порта PORTB как входа (UART1_RX)
	PortInit.PORT_OE = PORT_OE_IN;
	PortInit.PORT_Pin = PORT_Pin_6;
	PORT_Init(MDR_PORTB, &PortInit);
		
	//Конфигурация ножек для АЦП
	PORT_StructInit(&PortInit);
	PortInit.PORT_Pin = PORT_Pin_0 | PORT_Pin_1 | PORT_Pin_2 | PORT_Pin_3 | PORT_Pin_5 | PORT_Pin_6;
	PortInit.PORT_MODE = PORT_MODE_ANALOG;
	PORT_Init (MDR_PORTD, &PortInit);
	
	//Конфигурация ножек для считывания положения галетного переключателя
	PORT_StructInit (&PortInit);
	PortInit.PORT_Pin = PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6;
	PortInit.PORT_MODE = PORT_MODE_DIGITAL;
	PortInit.PORT_FUNC = PORT_FUNC_PORT;
	PortInit.PORT_OE = PORT_OE_IN;
	PortInit.PORT_PD = PORT_PD_DRIVER;
	PortInit.PORT_PULL_UP = PORT_PULL_UP_OFF;
	PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
	PortInit.PORT_SPEED = PORT_SPEED_SLOW;
	PORT_Init (MDR_PORTA, &PortInit);
	
	//Конфигурация ножек для светодиодов Авария РТР, БШ
	PortInit.PORT_Pin = PORT_Pin_2 | PORT_Pin_3;
	PortInit.PORT_OE = PORT_OE_OUT; // Конфигурация группы выводов как выход
	PortInit.PORT_FUNC = PORT_FUNC_PORT; // Работа а режиме порта ввода-вывода
	PortInit.PORT_MODE = PORT_MODE_DIGITAL; // Цифровой режим
	PortInit.PORT_SPEED = PORT_SPEED_SLOW; // Низкая частота тактования порта
	PORT_Init(MDR_PORTF, &PortInit); //Инициализация порта объявленной структурой
}
//-----------------------------------------//
void Init_TIMER(void)
{   
	TIMER_CntInitTypeDef Timer_struct;
	
	//// Enable TIMERs clk:
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER1, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER2, ENABLE);
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER3, ENABLE);
	
	//Настройка TIMER1
	TIMER_DeInit(MDR_TIMER1); // Reset all TIMER1 settings
	TIMER_BRGInit(MDR_TIMER1,TIMER_HCLKdiv1);
	Timer_struct.TIMER_IniCounter = 0;          // Начальное значение CNT
	Timer_struct.TIMER_Prescaler = 8000 - 1;     // 1 tic = 0.1 msec
	Timer_struct.TIMER_Period = 10;           //1 мс (для виртуальных таймеров)
	Timer_struct.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;     // The Timer clock changes the TIMERx_CNT value. The count direction is not changed.
  Timer_struct.TIMER_CounterDirection = TIMER_CntDir_Up;               // Таймер нарастающий
  Timer_struct.TIMER_EventSource = TIMER_EvSrc_None;               
  Timer_struct.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
  Timer_struct.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately;
  Timer_struct.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
  Timer_struct.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None;
  Timer_struct.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted;
  Timer_struct.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted;
	TIMER_CntInit(MDR_TIMER1, &Timer_struct);
	TIMER_ITConfig(MDR_TIMER1, TIMER_STATUS_CNT_ARR, ENABLE);
	NVIC_EnableIRQ(Timer1_IRQn); //Timer1 Interrupt enable
	NVIC_SetPriority(Timer1_IRQn, 3); 
	TIMER_Cmd(MDR_TIMER1, ENABLE);//запуск таймера1
	
	//Настройка TIMER2
	TIMER_DeInit(MDR_TIMER2); // Reset all TIMER2 settings
	TIMER_BRGInit(MDR_TIMER2,TIMER_HCLKdiv1);
	Timer_struct.TIMER_IniCounter = 0;          // Начальное значение CNT
	Timer_struct.TIMER_Prescaler = 8000 - 1;     // 1 tic = 0.1 msec
	Timer_struct.TIMER_Period = 11;           //1,1 мс (время для полного выползания сообщения на линию)
	Timer_struct.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;     // The Timer clock changes the TIMERx_CNT value. The count direction is not changed.
    Timer_struct.TIMER_CounterDirection = TIMER_CntDir_Up;               // Таймер нарастающий
    Timer_struct.TIMER_EventSource = TIMER_EvSrc_None; //TIMER_EvSrc_CH1;(было)               // Источник событий - внешнее событие, канал 1
    Timer_struct.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
    Timer_struct.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately;
    Timer_struct.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
    Timer_struct.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None;
    Timer_struct.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted;
    Timer_struct.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted;
	TIMER_CntInit(MDR_TIMER2, &Timer_struct);
	TIMER_ITConfig(MDR_TIMER2, TIMER_STATUS_CNT_ARR, ENABLE);
	NVIC_SetPriority(Timer2_IRQn, 2); 
	NVIC_EnableIRQ(Timer2_IRQn); //Timer2 Interrupt enable
	
	//Настройка TIMER3
	TIMER_DeInit(MDR_TIMER3); // Reset all TIMER3 settings
	TIMER_BRGInit(MDR_TIMER3,TIMER_HCLKdiv1);
	Timer_struct.TIMER_IniCounter = 0;          // Начальное значение CNT
	Timer_struct.TIMER_Prescaler = 8000 - 1;     // 1 tic = 0.1 msec
	Timer_struct.TIMER_Period = 46;           //4600 мкс (ожидание конца сообщения)
	Timer_struct.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;     // The Timer clock changes the TIMERx_CNT value. The count direction is not changed.
    Timer_struct.TIMER_CounterDirection = TIMER_CntDir_Up;               // Таймер нарастающий
    Timer_struct.TIMER_EventSource = TIMER_EvSrc_None; //TIMER_EvSrc_CH1;(было)               // Источник событий - внешнее событие, канал 1
    Timer_struct.TIMER_FilterSampling = TIMER_FDTS_TIMER_CLK_div_1;
    Timer_struct.TIMER_ARR_UpdateMode = TIMER_ARR_Update_Immediately;
    Timer_struct.TIMER_ETR_FilterConf = TIMER_Filter_1FF_at_TIMER_CLK;
    Timer_struct.TIMER_ETR_Prescaler = TIMER_ETR_Prescaler_None;
    Timer_struct.TIMER_ETR_Polarity = TIMER_ETRPolarity_NonInverted;
    Timer_struct.TIMER_BRK_Polarity = TIMER_BRKPolarity_NonInverted;
	TIMER_CntInit(MDR_TIMER3, &Timer_struct);
	TIMER_ITConfig(MDR_TIMER3, TIMER_STATUS_CNT_ARR, ENABLE);
	NVIC_SetPriority(Timer3_IRQn, 1); 
	NVIC_EnableIRQ(Timer3_IRQn); //Timer3 Interrupt enable
}

void Init_ADC(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADCx_InitTypeDef ADCx_InitStructure;
	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_ADC, ENABLE);
	
	//----------------------Общая настройка двух АЦП--------------------------//
	ADC_InitStructure.ADC_SynchronousMode = ADC_SyncMode_Independent;//независимый друг от друга запуск АЦП1 и АЦП2(АЦП2 не используем)
	ADC_InitStructure.ADC_StartDelay = 0;//задержка между запусками двух АЦП(0 т.к. используем только АЦП1)
		
	//-----Настройки встроенного датчика температуры и датчика опорного напряжения (все отключено т.к. не нужно(по крайней мере пока что))-----//
	ADC_InitStructure.ADC_TempSensor = ADC_TEMP_SENSOR_Disable;
	ADC_InitStructure.ADC_TempSensorAmplifier = ADC_TEMP_SENSOR_AMPLIFIER_Disable;
	ADC_InitStructure.ADC_TempSensorConversion = ADC_TEMP_SENSOR_CONVERSION_Disable;
	ADC_InitStructure.ADC_IntVRefConversion = ADC_VREF_CONVERSION_Disable;
	ADC_InitStructure.ADC_IntVRefTrimming = 0;
	ADC_Init (&ADC_InitStructure);
		
	//---------------Настройки конкретно АЦП1---------------------------//
	ADCx_StructInit (&ADCx_InitStructure);
	ADCx_InitStructure.ADC_ClockSource = ADC_CLOCK_SOURCE_CPU;//тактирование с той же частотой, что и ядро МК
	ADCx_InitStructure.ADC_Prescaler = ADC_CLK_div_512;// делим частоту тактирования АЦП 80 000 000 / 512 = 156,25 КГц (5580 преобразований в секунду)
	ADCx_InitStructure.ADC_SamplingMode = ADC_SAMPLING_MODE_SINGLE_CONV;// вид преобразования  - одиночное
	ADCx_InitStructure.ADC_ChannelSwitching = ADC_CH_SWITCHING_Disable;//перебор каналов для преобразования - выключен
	ADCx_InitStructure.ADC_ChannelNumber = 0;
	ADCx_InitStructure.ADC_Channels = 0;
	ADCx_InitStructure.ADC_DelayGo = 7;//задержка между преобразованиями
	ADCx_InitStructure.ADC_LevelControl = ADC_LEVEL_CONTROL_Disable;//контроль уровней напряжения - отключен
	ADCx_InitStructure.ADC_LowLevel = 0;
	ADCx_InitStructure.ADC_HighLevel = 0;
	ADCx_InitStructure.ADC_VRefSource = ADC_VREF_SOURCE_INTERNAL;// источник опорного напряжения - внутренний
	ADCx_InitStructure.ADC_IntVRefSource = ADC_INT_VREF_SOURCE_INEXACT;// источник опорного напряжения для датчика опорного напряжения - источник без температурной компенсации
	ADC1_Init (&ADCx_InitStructure);//инициализация АЦП1 структурой
	ADC1_Cmd(ENABLE);//разрешение работы АЦП1
}
//-----------------------------------------//
void Init_UART1(void)
{
	UART_InitTypeDef UART_InitStructure;
	
	RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
	
	//Инициализация делителя тактовой частоты для UART1
	UART_BRGInit(MDR_UART1, UART_HCLKdiv1);
	// Разрешение прерывания для UART1
	NVIC_EnableIRQ(UART1_IRQn);
	//установка приоритета прерывания
	NVIC_SetPriority (UART1_IRQn, 0);
	// Заполнение полей для переменной UART_InitStructure
	UART_InitStructure.UART_BaudRate = 9600; //тактовая частота передачи данных
	UART_InitStructure.UART_WordLength = UART_WordLength8b; //длина символов 8 бит
	UART_InitStructure.UART_StopBits = UART_StopBits1; //1 стоп бит
	UART_InitStructure.UART_Parity = UART_Parity_No; // нет контроля четности
	UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF; // выключение FIFO буфера
	/* Аппаратный контроль за передачей и приемом */
	UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE; 
	UART_Init (MDR_UART1, &UART_InitStructure); //Инициализация UART1
	UART_ITConfig (MDR_UART1, UART_IT_RX, ENABLE);//Разрешение прерывания по приему
	NVIC_SetPriority(UART1_IRQn, 0);
	UART_Cmd(MDR_UART1, ENABLE); //Разрешение работы UART1                                 
}

void RxOFF(void)
{
	PORT_InitTypeDef PortInit;
	//Функции RxOFF() и RxON() - костыли для корректной работы с К2011ВВ24.  
	//Во время передачи байта запроса К2011ВВ24 иногда отпускает уровень на линии Rx UARTa,
	//(должна всегда держать), поэтому нужно отключить ножку Rx на время передачи, чтобы не словить нулевой пакет
    //PB6 - UART1_RxD
	//PORT_InitStructure.PORT_FUNC      = PORT_FUNC_ALTER;
  	PortInit.PORT_FUNC = PORT_FUNC_PORT;
    PortInit.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PortInit);
}

void RxON(void)
{
	PORT_InitTypeDef PortInit;
	
    // PB6 - UART1_RxD
    PortInit.PORT_PULL_DOWN = PORT_PULL_DOWN_OFF;
    PortInit.PORT_PD_SHM    = PORT_PD_SHM_OFF;
    PortInit.PORT_PD        = PORT_PD_DRIVER;
    PortInit.PORT_GFEN      = PORT_GFEN_OFF;
    PortInit.PORT_FUNC      = PORT_FUNC_ALTER;

    PortInit.PORT_SPEED     = PORT_SPEED_MAXFAST;
    PortInit.PORT_MODE      = PORT_MODE_DIGITAL;
    //Configure PORTB pins 6 (UART1_RX) as input
		PortInit.PORT_PULL_UP   = PORT_PULL_UP_OFF;
    PortInit.PORT_OE  = PORT_OE_IN;
    PortInit.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PortInit);
}
//End file init.c
