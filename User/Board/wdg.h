#ifndef _WDG_H_
#define _WDG_H_

#include "stm32l4xx_hal.h"

#define WWDG_WMAX		(0x7F)
#define WWDG_TMAX		WWDG_WMAX

//void WWDG_Init(uint8_t tr, uint8_t wr, uint32_t prer)
void WWDG_Init(void);
void WWDG_Refresh(void);
void IWDG_Init(uint16_t rl, uint8_t prer);
void IWDG_Refresh(void);

//wdg thread
void wdg_thread(void const *param);

#endif