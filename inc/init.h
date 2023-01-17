#ifndef INIT_H
#define INIT_H

#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_timer.h"
#include "MDR32F9Qx_adc.h"

void Init_RST_CLK(void);
void Init_PORT(void);
void Init_TIMER(void);
void Init_ADC(void);
void Init_UART1(void);
void RxOFF(void);
void RxON(void);

#endif
