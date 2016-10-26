#ifndef _RTC_H_
#define _RTC_H_

#include "main.h"

/* LSE 32768 */
#define RTC_ASYNCH_PREDIV		0x7F
#define RTC_SYNCH_PREDIV		0xFF

#define WAKEUP_TIMER_ENABLE 0x32F2

void RTC_Config(uint32_t wakeup_time);
uint32_t RTC_GetTimeStamp(void);
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc);
RTC_DateTypeDef RTC_DataShow(void);
RTC_TimeTypeDef RTC_TimeShow(void);

#endif