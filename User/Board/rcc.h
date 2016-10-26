#ifndef _RCC_H_
#define _RCC_H_

#include "stm32l4xx_hal.h"

void MySystemClock_Config(void);
void SystemClock_ConfigExitStop(void);
void LSE_ClockEnable(void);
void LSI_ClockEnable(void);

void delay_us(uint32_t nus);
#endif