#ifndef _TIMER_H_
#define _TIMER_H_

#include "stm32l4xx_hal.h"

void Timer_Init(uint32_t second);
void Timer_Stop(void);
void Timer_Start(void);
TIM_HandleTypeDef *Timer_Get(void);

uint32_t Timer_GetRegion(void);
void Timer_SetRegion(uint8_t hour, uint32_t period);

#endif