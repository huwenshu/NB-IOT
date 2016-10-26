/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  rtc.c
** 版本：  	    V1.00   
** 工作环境: 	  IAR Embedded Workbench IDE 7.70.2
** 作者: 		    eric.xu
** 功能:		    RTC驱动						
** 日期: 	      2016-10-23
** 修改日志：	  
**
** 版权所有     (C)2016-2017 利尔达科技有限公司
*******************************************************************************/
#include "stm32l4xx_hal.h"
#include "rtc.h"
#include <time.h>

RTC_HandleTypeDef RtcHandle;

static	RTC_DateTypeDef sdatestructureget;
static  RTC_TimeTypeDef stimestructureget;
static 	time_t timestamp;
static	struct tm t_tm;

/*******************************************************************************
** 功能	      RTC配置           
** 参数 			无      
** 返回值	    无
** 注意				LSE 32768
** 修改日志
*******************************************************************************/
void RTC_Config(uint32_t wakeup_time)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;
  
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
	
	/* Configue LSI as RTC clock soucre */
	HAL_RCC_GetOscConfig(&RCC_OscInitStruct);
	
	RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
	}
	
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if(HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
	}
	
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follows:
      - Hour Format    = Format 24
      - Asynch Prediv  = Value according to source clock
      - Synch Prediv   = Value according to source clock
      - OutPut         = Output Disable
      - OutPutPolarity = High Polarity
      - OutPutType     = Open Drain */
  
  RtcHandle.Instance = RTC; 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV; /* 0x7F */
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV; /* (32 kHz / 128) - 1 = 0xF9 */
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	
	  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
  
  /*##-3- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0x0, 0);
  
  /* Enable the RTC global Interrupt */
  HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
  
  /* Check if data stored in BackUp register1: Wakeup timer enable */
	/* Read the Back Up Register 1 Data */
  if (HAL_RTCEx_BKUPRead(&RtcHandle, RTC_BKP_DR1) == WAKEUP_TIMER_ENABLE)
  {
    /* if the wakeup timer is enabled then deactivate it to disable the wakeup timer interrupt */
    if(HAL_RTCEx_DeactivateWakeUpTimer(&RtcHandle) != HAL_OK)
    { 
    }
  } 
	
	/* Configure the RTC Wakeup peripheral */
  /* Setting the Wakeup time to 20 s
       If RTC_WAKEUPCLOCK_CK_SPRE_16BITS is selected, the frequency is 1Hz, 
       this allows to get a wakeup time equal to 20 s if the counter is 0x0 */
  HAL_RTCEx_SetWakeUpTimer_IT(&RtcHandle, wakeup_time, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
  
  /*##-4- Write 'wakeup timer enabled' tag in RTC Backup data Register 1 #######*/
  HAL_RTCEx_BKUPWrite(&RtcHandle, RTC_BKP_DR1, WAKEUP_TIMER_ENABLE);	

	/* Configure the Date, BCD format
	** Set Date: Monday October 24th 2016	
	*/
	sdatestructure.Year = 0x16;
  sdatestructure.Month = RTC_MONTH_OCTOBER;
  sdatestructure.Date = 0x24;
  sdatestructure.WeekDay = RTC_WEEKDAY_MONDAY;
	
	if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
  }
	
	/* Configure the Time
	** Set Time: 09:20:00	
	*/
  stimestructure.Hours = 0x09;
  stimestructure.Minutes = 0x20;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE ;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;	
	
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
	{}	
}

uint32_t RTC_GetTimeStamp(void)
{ 
    /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);	
		
	t_tm.tm_year = sdatestructureget.Year + 2000 - 1900;
	t_tm.tm_mon = sdatestructureget.Month;
	t_tm.tm_mday = sdatestructureget.Date;
	t_tm.tm_hour = stimestructureget.Hours;
	t_tm.tm_min = stimestructureget.Minutes;
	t_tm.tm_sec = stimestructureget.Seconds;
	t_tm.tm_isdst = -1;
	
	timestamp = mktime(&t_tm);
	
	return timestamp;
}

//void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
//{ 
//	//BSP_LED_Toggle(LED2);   
//
//	/* Send one message to send-task, 
//	** Read the rtc date and time 
//	*/
//	RTC_TimeShow();
//}

RTC_DateTypeDef RTC_DataShow(void)
{
	HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
	
	return sdatestructureget;
}

RTC_TimeTypeDef RTC_TimeShow(void)
{
	HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
	
	return stimestructureget;
}

