/*******************************************************************************
** ��Ȩ:		    ������Ƽ����޹�˾
** �ļ���: 		  wdg.c
** �汾��  	    V1.00   
** ��������: 	  IAR Embedded Workbench IDE 7.70.2
** ����: 		    eric.xu
** ����:		    ���Ź�����						
** ����: 	      2016-10-23
** �޸���־��	  
**
** ��Ȩ����     (C)2016-2017 ������Ƽ����޹�˾
*******************************************************************************/
#include "wdg.h"

static WWDG_HandleTypeDef   WwdgHandle;
static IWDG_HandleTypeDef		IwdgHandle;

/*******************************************************************************
** ����	      ���ڿ��Ź���ʼ��           
** ���� 			��Ƶϵ�����������ã����ֵ����      
** ����ֵ	    ��
** ע��				����EWI�������жϴ����ڵ��ﴰ��ǰι��
** �޸���־
*******************************************************************************/
#if 0
void WWDG_Init(uint8_t tr, uint8_t wr, uint32_t prer)
{	
	//ʹ��ʱ��
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
	//ʹ��ʱ��
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
** ����	      ���ڿ��Ź�ι������           
** ���� 			��    
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void WWDG_Refresh(void)
{
	HAL_WWDG_Refresh(&WwdgHandle);
}

/*******************************************************************************
** ����	     	�������Ź���ʼ��           
** ���� 			reload, prer  
** ����ֵ	    ��
** ע��				LsiFreq���̶�
** �޸���־
*******************************************************************************/
void IWDG_Init(uint16_t rl, uint8_t prer)
{
		//����ʱ�ӣ���ͬ��WWDG
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
	//������Ҫ��wdg����������		
	HAL_IWDG_Refresh(&IwdgHandle);
	//
}

void wdg_thread(void const *param)
{
	IWDG_Init(10000, IWDG_PRESCALER_32);	// timeout = 32 * reload /lsi, Լ10s
	
	while(1)
	{
		IWDG_Refresh();
	}
}
