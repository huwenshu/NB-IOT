/*******************************************************************************
** ��Ȩ:		    ������Ƽ����޹�˾
** �ļ���: 		  rcc.c
** �汾��  	    V1.00   
** ��������: 	  IAR Embedded Workbench IDE 7.70.2
** ����: 		    eric.xu
** ����:		    ϵͳʱ������						
** ����: 	      2016-10-23
** �޸���־��	  
**
** ��Ȩ����     (C)2016-2017 ������Ƽ����޹�˾
*******************************************************************************/
#include "rcc.h"
#include "FreeRTOSConfig.h"

//HSE 8MHz

/*******************************************************************************
** ����	      ����ϵͳʱ��
              System Clock source            = PLL (HSE)
              SYSCLK(Hz)                     = 80000000
              HCLK(Hz)                       = 80000000
              AHB Prescaler                  = 1
              APB1 Prescaler                 = 1
              APB2 Prescaler                 = 1
              HSE Frequency(Hz)              = 8000000
              PLL_MUL                        = 20
              PLL_DIV                        = 2
              Flash Latency(WS)              = 1
              Main regulator output voltage  = Scale1 mode                       
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void MySystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI; 
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
  }
}

void SystemClock_ConfigExitStop(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* ʹ��PWR����ʱ�� */
  __PWR_CLK_ENABLE();

  /* ���õ�ѹ��������ѹ��Χ  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//����Ϊ��Χ1

  /* ͨ��RCCֵ��ȡ��ǰ���������� */
  HAL_RCC_GetOscConfig(&RCC_OscInitStruct);

  /* After wake-up from STOP reconfigure the system clock: Enable HSI and PLL */ 
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI; 
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
  }
}

/*******************************************************************************
** ����	      ʹ���ⲿ���پ���LSE              
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void LSE_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

/*******************************************************************************
** ����	      ʹ���ڲ����پ���LSI              
** ����       ��
** ����ֵ	    ��
** ע��
** �޸���־
*******************************************************************************/
void LSI_ClockEnable(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct;
  
  /* Enable LSE clock */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.LSEState = RCC_LSI_ON;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
}

/*******************************************************************************
** ����	     	���΢����ʱ����             
** ����       ��
** ����ֵ	    ��
** ע��				nus <= 2^24/8
** �޸���־
*******************************************************************************/
void delay_us(uint32_t nus)
{
	uint32_t ticks;
	uint32_t told, tnow, tcnt;
	uint32_t reload = SysTick->LOAD;
	
	tcnt = 0;
	ticks = nus*(configCPU_CLOCK_HZ/8/10000000);	//�ӳ�ʱ����Ҫ����ʱ�ӽ���
	/* ��ֹos���� */
	
	told = SysTick->VAL;
	while(1)
	{
		tnow = SysTick->VAL;
		if(tnow != told)
		{
			if(tnow < told) 
				tcnt += told - tnow;
			else
				tcnt += reload - tnow + told;
				
			told = tnow;
			if(tcnt >= ticks)	break;				
		}
	}	
	
	/* �ָ�os���� */
}