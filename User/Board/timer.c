/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  timer.c
** 版本：  	    V1.00   
** 工作环境: 	  IAR Embedded Workbench IDE 7.70.2
** 作者: 		    eric.xu
** 功能:		    Timer驱动						
** 日期: 	      2016-10-25
** 修改日志：	  
**
** 版权所有     (C)2016-2017 利尔达科技有限公司
*******************************************************************************/
#include "timer.h"

static TIM_HandleTypeDef    TimHandle;
static uint32_t 	timer_region_cnt = 0;

/* 
**	TIM2&TIM5 32-bit up, down, up/down auto-reload counter 
**	programmable prescaler 16-bit
**	Default one second, the max value must less than four days
*/
void Timer_Init(uint32_t second)
{
	uint32_t rl;
	uint32_t uwPrescalerValue = (uint32_t)(SystemCoreClock / 10000) - 1;
	
	/* TIM2 Peripheral clock enable */
	__HAL_RCC_TIM2_CLK_ENABLE();
	
	TimHandle.Instance = TIM2;
	
  /* Initialize TIMx peripheral as follows:
       + Period = 10000 - 1
       + Prescaler = (SystemCoreClock/10000) - 1
       + ClockDivision = 0
       + Counter direction = Up
  */
  rl = second*10000;
  
  TimHandle.Init.Period            = rl - 1;
  TimHandle.Init.Prescaler         = uwPrescalerValue;
  TimHandle.Init.ClockDivision     = 0;
  TimHandle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  TimHandle.Init.RepetitionCounter = 0;
  
  if (HAL_TIM_Base_Init(&TimHandle) != HAL_OK)
  {
  }
  
  /* Set the TIM2 priority */
  HAL_NVIC_SetPriority(TIM2_IRQn, 3, 0);
  
  /* Enable the TIMx global Interrupt */
  HAL_NVIC_EnableIRQ(TIM2_IRQn);
  
  /* Start the TIM Base generation in interrupt mode */
  if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {}
  
}

void Timer_Stop(void)
{
	if (HAL_TIM_Base_Stop_IT(&TimHandle) != HAL_OK)
	{
	}	
}

void Timer_Start(void)
{
	if (HAL_TIM_Base_Start_IT(&TimHandle) != HAL_OK)
  {}
}

void Timer_SetRegion(uint8_t hour, uint32_t period)
{
	if(hour >= period*3600)	
		timer_region_cnt = hour*3600/period;
}

uint32_t Timer_GetRegion(void)
{
	return timer_region_cnt;
}

TIM_HandleTypeDef *Timer_Get(void)
{
	return &TimHandle;
}
