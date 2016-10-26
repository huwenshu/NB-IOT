/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  wdg.c
** 版本：  	    V1.00   
** 工作环境: 	  IAR Embedded Workbench IDE 7.70.2
** 作者: 		    eric.xu
** 功能:		    看门狗驱动						
** 日期: 	      2016-10-23
** 修改日志：	  
**
** 版权所有     (C)2016-2017 利尔达科技有限公司
*******************************************************************************/
#include "wdg.h"

static WWDG_HandleTypeDef   WwdgHandle;
static IWDG_HandleTypeDef		IwdgHandle;

/*******************************************************************************
** 功能	      窗口看门狗初始化           
** 参数 			分频系数，窗口设置，最大值设置      
** 返回值	    无
** 注意				设置EWI需配置中断处理，在到达窗口前喂狗
** 修改日志
*******************************************************************************/
#if 0
void WWDG_Init(uint8_t tr, uint8_t wr, uint32_t prer)
{	
	//使能时钟
	__HAL_RCC_WWDG_CLK_ENABLE();
	
	WwdgHandle.Instance = WWDG;
	WwdgHandle.Init.Prescaler = prer;
	WwdgHandle.Init.Window		= wr;
	WwdgHandle.Init.Counter 	= tr;
	WwdgHandle.Init.EWIMode		=	WWDG_EWI_DISABLE;
	
	HAL_WWDG_Init(&WwdgHandle);
}
#else
void WWDG_Init(void)
{
	//使能时钟
	__HAL_RCC_WWDG_CLK_ENABLE();
	
	WwdgHandle.Instance = WWDG;
	WwdgHandle.Init.Prescaler = WWDG_PRESCALER_8;
	WwdgHandle.Init.Window		= 0x40;
	WwdgHandle.Init.Counter 	= 0x7F;
	WwdgHandle.Init.EWIMode		=	WWDG_EWI_DISABLE;		//early interrupt
	
	HAL_WWDG_Init(&WwdgHandle);	
}
#endif

/*******************************************************************************
** 功能	      窗口看门狗喂狗操作           
** 参数 			无    
** 返回值	    无
** 注意
** 修改日志
*******************************************************************************/
void WWDG_Refresh(void)
{
	HAL_WWDG_Refresh(&WwdgHandle);
}

/*******************************************************************************
** 功能	     	独立看门狗初始化           
** 参数 			reload, prer  
** 返回值	    无
** 注意				LsiFreq不固定
** 修改日志
*******************************************************************************/
void IWDG_Init(uint16_t rl, uint8_t prer)
{
		//独立时钟，不同于WWDG
//	_HAL_RCC_IWDG_CLK_ENABLE();
	
 /* Set counter reload value to obtain 1 sec. IWDG TimeOut.
     IWDG counter clock Frequency = uwLsiFreq
     Set Prescaler to 32 (IWDG_PRESCALER_32)
     Timeout Period = (Reload Counter Value * 32) / LsiFreq
     So Set Reload Counter Value = (1 * LsiFreq) / 32 */	
	IwdgHandle.Instance = IWDG;
  IwdgHandle.Init.Prescaler = prer;	//IWDG_PRESCALER_32;
  IwdgHandle.Init.Reload = 	rl;		//(LsiFreq / 32), about 32KHz, 1000;  	
  IwdgHandle.Init.Window = IWDG_WINDOW_DISABLE;

	HAL_IWDG_Init(&IwdgHandle);
}

void IWDG_Refresh(void)
{
	//可能需要对wdg进行锁操作		
	HAL_IWDG_Refresh(&IwdgHandle);
	//
}

void wdg_thread(void const *param)
{
	IWDG_Init(10000, IWDG_PRESCALER_32);	// timeout = 32 * reload /lsi, 约10s
	
	while(1)
	{
		IWDG_Refresh();
	}
}
