/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  lsd_bsp.c
** 版本：  	    V1.00   
** 工作环境: 	  IAR Embedded Workbench IDE 7.70.2
** 作者: 		    eric.xu
** 功能:		    bsp驱动						
** 日期: 	      2016-10-24
** 修改日志：	  
**
** 版权所有     (C)2016-2017 利尔达科技有限公司
*******************************************************************************/
#include "lsd_bsp.h"
#include "uart.h"

GPIO_TypeDef* LED_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT};
const uint16_t LED_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN};

static void IOT_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	IOT_RST_GPIO_CLK_ENABLE();
	IOT_RI_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       = IOT_RST_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(IOT_RST_GPIO_PORT, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin       = IOT_RI_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_INPUT;
  HAL_GPIO_Init(IOT_RI_GPIO_PORT, &GPIO_InitStruct);
}

static void IOT_Action_Init(void)
{
	IOT_GPIO_Init();
	
	UART3_Init();
	UART4_Init();
}

void IOT_Module_Init(void)
{
	IOT_Action_Init();
	
	IOT_RST_HIGH();
	HAL_Delay(10);
	IOT_RST_LOW();	
}

void OCP_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	OCP_STAT_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       	= OCP1_STAT_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull      	= GPIO_PULLUP;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(OCP1_STAT_GPIO_PORT, &GPIO_InitStruct);
		
	GPIO_InitStruct.Pin       	= OCP2_STAT_PIN;
	HAL_GPIO_Init(OCP2_STAT_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin       	= OCP3_STAT_PIN;
	HAL_GPIO_Init(OCP3_STAT_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin       	= OCP4_STAT_PIN;
	HAL_GPIO_Init(OCP4_STAT_GPIO_PORT, &GPIO_InitStruct);	
}

uint8_t OCP_GetState(void)
{
	uint8_t stat = 0;
	
	stat = HAL_GPIO_ReadPin(OCP1_STAT_GPIO_PORT, OCP1_STAT_PIN) |\
	 HAL_GPIO_ReadPin(OCP2_STAT_GPIO_PORT, OCP2_STAT_PIN) << 1 |\
	 HAL_GPIO_ReadPin(OCP3_STAT_GPIO_PORT, OCP3_STAT_PIN) << 2 |\
	 HAL_GPIO_ReadPin(OCP4_STAT_GPIO_PORT, OCP4_STAT_PIN) << 3;

	return stat;
}

void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	LED_GPIO_PORT_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       	= LED1_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull      	= GPIO_PULLUP;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(LED1_GPIO_PORT, &GPIO_InitStruct);
  
 	GPIO_InitStruct.Pin       	= LED2_PIN;
 	HAL_GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);	

 	GPIO_InitStruct.Pin       	= LED3_PIN;
 	HAL_GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStruct);	
}

void LED_On(Led_TypeDef Led)
{
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_RESET);	
}

void LED_Off(Led_TypeDef Led)
{
	HAL_GPIO_WritePin(LED_PORT[Led], LED_PIN[Led], GPIO_PIN_SET);	
}

void LED_Toggle(Led_TypeDef Led)
{
  HAL_GPIO_TogglePin(LED_PORT[Led], LED_PIN[Led]);
}

void DQ_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	DS_DQ_GPIO_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       	= DS_DQ_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull      	= GPIO_PULLUP;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(DS_DQ_GPIO_PORT, &GPIO_InitStruct); 
}

void TSENSEOR_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	TSENSOR_SCL_PORT_CLK_ENABLE();
	TSENSOR_SDA_PORT_CLK_ENABLE();
	
	GPIO_InitStruct.Pin       = TSENSOR_SCL_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = TSENSOR_SCL_AF; 
  HAL_GPIO_Init(TSENSOR_SCL_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin       = TSENSOR_SDA_PIN;  
  GPIO_InitStruct.Alternate = TSENSOR_SDA_AF;
  HAL_GPIO_Init(TSENSOR_SDA_PORT, &GPIO_InitStruct);
}

void FLASH_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;

	FLASH_GPIO_PORT_CLK_ENABLE();

	GPIO_InitStruct.Pin       = FLASH_NSS_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = FLASH_NSS_AF; 
  HAL_GPIO_Init(FLASH_NSS_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin       = FLASH_SCK_PIN;  
  GPIO_InitStruct.Alternate = FLASH_SCK_AF;
  HAL_GPIO_Init(FLASH_SCK_PORT, &GPIO_InitStruct);	

	GPIO_InitStruct.Pin       = FLASH_MOSI_PIN;  
  GPIO_InitStruct.Alternate = FLASH_MOSI_AF;
  HAL_GPIO_Init(FLASH_MOSI_PORT, &GPIO_InitStruct);  
  
  GPIO_InitStruct.Pin       = FLASH_MISO_PIN;
  GPIO_InitStruct.Mode			= GPIO_MODE_INPUT;  
  GPIO_InitStruct.Alternate = FLASH_MISO_AF;
  HAL_GPIO_Init(FLASH_MISO_PORT, &GPIO_InitStruct);	
}

void CHARGE_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	CHARGE_GPIO_PORT_CLK_ENABLE();
	
	/*
	* ISET2, High = 500mAmax, Low = ISET, FLOAT = 100mA max
	*/
	GPIO_InitStruct.Pin       	= CHARGE_ISET_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;	//floating
  GPIO_InitStruct.Pull      	= GPIO_NOPULL;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(CHARGE_ISET_PORT, &GPIO_InitStruct); 
 
 	GPIO_InitStruct.Pin       	= CHARGE_ADC_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull      	= GPIO_PULLDOWN;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(CHARGE_ADC_PORT, &GPIO_InitStruct); 
  
  /* TS, floating enable charge, low disable charge */
	GPIO_InitStruct.Pin       	= CHARGE_TS_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull      	= GPIO_NOPULL;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(CHARGE_TS_PORT, &GPIO_InitStruct); 
  
  /* PG, low to select extend power supply, high to select battery */
	GPIO_InitStruct.Pin       	= CHARGE_PG_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;	//auto select
  GPIO_InitStruct.Pull      	= GPIO_NOPULL;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(CHARGE_PG_PORT, &GPIO_InitStruct);   
}