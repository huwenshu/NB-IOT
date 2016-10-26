#ifndef _LSD_BSP_H
#define _LSD_BSP_H

#include "stm32l4xx_hal.h"

/* config the board pripheral */
/*
** USART1 for Debug
*/
/* Definition for USART1 clock resources */
#define USART1_CLK_ENABLE()								__HAL_RCC_USART1_CLK_ENABLE()
#define USART1_RX_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_TX_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()
#define USART1_RX_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOA_CLK_DISABLE()
#define USART1_TX_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOA_CLK_DISABLE()

#define USART1_FORCE_RESET()             	__HAL_RCC_USART1_FORCE_RESET()
#define USART1_RELEASE_RESET()          	__HAL_RCC_USART1_RELEASE_RESET()

/* Definition for USART1 Pins, PA9-TX, PA10-RX */
#define USART1_TX_PIN                    	GPIO_PIN_9
#define USART1_TX_GPIO_PORT              	GPIOA
#define USART1_TX_AF                     	GPIO_AF7_USART1
#define USART1_RX_PIN                    	GPIO_PIN_10
#define USART1_RX_GPIO_PORT              	GPIOA
#define USART1_RX_AF                     	GPIO_AF7_USART1

/*
** UART3/4, PC2, PB11 for BC95 NB-IOT module
*/
/* Definition for USART3 clock resources */
#define USART3_CLK_ENABLE()								__HAL_RCC_USART3_CLK_ENABLE()
#define USART3_RX_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOC_CLK_ENABLE()
#define USART3_TX_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOC_CLK_ENABLE()

#define USART3_FORCE_RESET()             	__HAL_RCC_USART3_FORCE_RESET()
#define USART3_RELEASE_RESET()           	__HAL_RCC_USART3_RELEASE_RESET()

/* Definition for USART3 Pins, PC4-TX, PC5-RX */
#define USART3_TX_PIN                    	GPIO_PIN_4
#define USART3_TX_GPIO_PORT              	GPIOC
#define USART3_TX_AF                     	GPIO_AF7_USART3
#define USART3_RX_PIN                    	GPIO_PIN_5
#define USART3_RX_GPIO_PORT              	GPIOC
#define USART3_RX_AF                     	GPIO_AF7_USART3

/* Definition for USART4 clock resources */
#define UART4_CLK_ENABLE()								__HAL_RCC_UART4_CLK_ENABLE()
#define UART4_RX_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()
#define UART4_TX_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOA_CLK_ENABLE()

#define UART4_FORCE_RESET()             	__HAL_RCC_UART4_FORCE_RESET()
#define UART4_RELEASE_RESET()           	__HAL_RCC_UART4_RELEASE_RESET()

/* Definition for USART4 Pins, PA0, PA1 */
#define UART4_TX_PIN                   		GPIO_PIN_0
#define UART4_TX_GPIO_PORT              	GPIOA
#define UART4_TX_AF                     	GPIO_AF8_UART4
#define UART4_RX_PIN                    	GPIO_PIN_1
#define UART4_RX_GPIO_PORT              	GPIOA
#define UART4_RX_AF                     	GPIO_AF8_UART4


/* Definition for BC95 RST PIN, PC2*/
#define IOT_RST_PIN												GPIO_PIN_2
#define IOT_RST_GPIO_PORT									GPIOC			
#define IOT_RST_GPIO_CLK_ENABLE()					__HAL_RCC_GPIOC_CLK_ENABLE()  
#define IOT_RST_GPIO_CLK_DISABLE()				__HAL_RCC_GPIOC_CLK_DISABLE()

#define IOT_RST_HIGH()										HAL_GPIO_WritePin(IOT_RST_GPIO_PORT, IOT_RST_PIN, GPIO_PIN_SET)
#define IOT_RST_LOW()											HAL_GPIO_WritePin(IOT_RST_GPIO_PORT, IOT_RST_PIN, GPIO_PIN_RESET)

/* Definition for BC95 RI PIN, PB11*/
#define IOT_RI_PIN												GPIO_PIN_11
#define IOT_RI_GPIO_PORT									GPIOB
#define IOT_RI_GPIO_CLK_ENABLE()					__HAL_RCC_GPIOB_CLK_ENABLE()  
#define IOT_RI_GPIO_CLK_DISABLE()					__HAL_RCC_GPIOB_CLK_DISABLE()


/*
** gpio for Optocouplers switch circuit, PB0，PB1，PB2，PB10
*/
#define OCP1_STAT_PIN											GPIO_PIN_0
#define OCP1_STAT_GPIO_PORT								GPIOB
#define OCP1_STAT_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()  
#define OCP1_STAT_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()  

#define OCP2_STAT_PIN											GPIO_PIN_1
#define OCP2_STAT_GPIO_PORT								GPIOB
#define OCP2_STAT_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()  
#define OCP2_STAT_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()

#define OCP3_STAT_PIN											GPIO_PIN_2
#define OCP3_STAT_GPIO_PORT								GPIOB
#define OCP3_STAT_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()  
#define OCP3_STAT_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()

#define OCP4_STAT_PIN											GPIO_PIN_10
#define OCP4_STAT_GPIO_PORT								GPIOB
#define OCP4_STAT_GPIO_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE()  
#define OCP4_STAT_GPIO_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()

#define OCP_STAT_GPIO_CLK_ENABLE()				OCP1_STAT_GPIO_CLK_ENABLE()
/*
**	ds18b20, PB6
*/
#define DS_DQ_PIN													GPIO_PIN_6
#define DS_DQ_GPIO_PORT										GPIOB	
#define DS_DQ_GPIO_CLK_ENABLE()						__HAL_RCC_GPIOB_CLK_ENABLE()  
#define DS_DQ_GPIO_CLK_DISABLE()					__HAL_RCC_GPIOB_CLK_DISABLE()

/*
**	LED，LED1-PB12，LED2-PB13，LED3-PB14
*/
#define LED1_PIN													GPIO_PIN_12
#define LED1_GPIO_PORT										GPIOB
#define LED1_GPIO_PORT_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE() 
#define LED1_GPIO_PORT_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()

#define LED2_PIN													GPIO_PIN_13
#define LED2_GPIO_PORT										GPIOB
#define LED2_GPIO_PORT_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE() 
#define LED2_GPIO_PORT_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()

#define LED3_PIN													GPIO_PIN_14
#define LED3_GPIO_PORT										GPIOB
#define LED3_GPIO_PORT_CLK_ENABLE()				__HAL_RCC_GPIOB_CLK_ENABLE() 
#define LED3_GPIO_PORT_CLK_DISABLE()			__HAL_RCC_GPIOB_CLK_DISABLE()

#define LED_GPIO_PORT_CLK_ENABLE()				LED1_GPIO_PORT_CLK_ENABLE()
/*
**	预留温度传感，I2C1_SCL-PB8，I2C1_SDA-PB9
*/
#define TSENSOR_SCL_PIN												GPIO_PIN_8
#define TSENSOR_SCL_PORT											GPIOB
#define TSENSOR_SCL_PORT_CLK_ENABLE()					__HAL_RCC_GPIOB_CLK_ENABLE()
#define TSENSOR_SCL_PORT_CLK_DISABLE()				__HAL_RCC_GPIOB_CLK_DISABLE()
#define TSENSOR_SCL_AF												GPIO_AF4_I2C1

#define TSENSOR_SDA_PIN												GPIO_PIN_9
#define TSENSOR_SDA_PORT											GPIOB
#define TSENSOR_SDA_PORT_CLK_ENABLE()					__HAL_RCC_GPIOB_CLK_ENABLE()
#define TSENSOR_SDA_PORT_CLK_DISABLE()				__HAL_RCC_GPIOB_CLK_DISABLE()
#define TSENSOR_SDA_AF												GPIO_AF4_I2C1

/*
**	预留外置flash，SPI1_NSS-PA4，SPI1_SCK-PA5，SPI1_MISO-PA6，SPI1_MOSI-PA7；
*/
#define FLASH_NSS_PIN													GPIO_PIN_4
#define FLASH_NSS_PORT												GPIOA
#define FLASH_NSS_PORT_CLK_ENABLE()						__HAL_RCC_GPIOA_CLK_ENABLE()
#define FLASH_NSS_PORT_CLK_DISABLE()					__HAL_RCC_GPIOA_CLK_DISABLE()
#define FLASH_NSS_AF													GPIO_AF5_SPI1

#define FLASH_SCK_PIN													GPIO_PIN_5
#define FLASH_SCK_PORT												GPIOA
#define FLASH_SCK_PORT_CLK_ENABLE()						__HAL_RCC_GPIOA_CLK_ENABLE()
#define FLASH_SCK_PORT_CLK_DISABLE()					__HAL_RCC_GPIOA_CLK_DISABLE()
#define FLASH_SCK_AF													GPIO_AF5_SPI1

#define FLASH_MISO_PIN												GPIO_PIN_6	
#define FLASH_MISO_PORT												GPIOA
#define FLASH_MISO_PORT_CLK_ENABLE()					__HAL_RCC_GPIOA_CLK_ENABLE()
#define FLASH_MISO_PORT_CLK_DISABLE()					__HAL_RCC_GPIOA_CLK_DISABLE()
#define FLASH_MISO_AF													GPIO_AF5_SPI1

#define FLASH_MOSI_PIN												GPIO_PIN_7
#define FLASH_MOSI_PORT												GPIOA
#define FLASH_MOSI_PORT_CLK_ENABLE()					__HAL_RCC_GPIOA_CLK_ENABLE()
#define FLASH_MOSI_PORT_CLK_DISABLE()					__HAL_RCC_GPIOA_CLK_DISABLE()
#define FLASH_MOSI_AF													GPIO_AF5_SPI1

#define FLASH_GPIO_PORT_CLK_ENABLE()					FLASH_NSS_PORT_CLK_ENABLE()
/*
***	charge circuit
***	PC3浮空输入使能充电，置低禁止充电，PC1 - ADC123_IN2量测电池电量，
***	ISET2-PC0/ADC123_IN1，控制充电的最大电流，PG-PC13控制供电方式
*/
#define CHARGE_ISET_PIN												GPIO_PIN_0
#define	CHARGE_ISET_PORT											GPIOC
#define CHARGE_ISET_PORT_CLK_ENABLE()					__HAL_RCC_GPIOC_CLK_ENABLE()
#define CHARGE_ISET_PORT_CLK_DISABLE()				__HAL_RCC_GPIOC_CLK_DISABLE()

#define CHARGE_ADC_PIN												GPIO_PIN_1
#define CHARGE_ADC_PORT												GPIOC
#define CHARGE_ADC_PORT_CLK_ENABLE()					__HAL_RCC_GPIOC_CLK_ENABLE()
#define CHARGE_ADC_PORT_CLK_DISABLE()					__HAL_RCC_GPIOC_CLK_DISABLE()

#define CHARGE_TS_PIN													GPIO_PIN_3
#define CHARGE_TS_PORT												GPIOC
#define CHARGE_TS_PORT_CLK_ENABLE()						__HAL_RCC_GPIOC_CLK_ENABLE()
#define CHARGE_TS_PORT_CLK_DISABLE()					__HAL_RCC_GPIOC_CLK_DISABLE()

#define CHARGE_PG_PIN													GPIO_PIN_13
#define CHARGE_PG_PORT												GPIOC
#define CHARGE_PG_PORT_CLK_ENABLE()						__HAL_RCC_GPIOC_CLK_ENABLE()
#define CHARGE_PG_PORT_CLK_DISABLE()					__HAL_RCC_GPIOC_CLK_DISABLE()

#define CHARGE_GPIO_PORT_CLK_ENABLE()					CHARGE_ISET_PORT_CLK_ENABLE()


typedef enum {
	LED1 = 0,
	LED2,
	LED3,
	LEDn,
}Led_TypeDef;

/* function list */
//static void IOT_GPIO_Init(void);
//static void IOT_Action_Init(void);
void IOT_Module_Init(void);

void OCP_GPIO_Init(void);
uint8_t OCP_GetState(void);

void LED_Init(void);
void LED_On(Led_TypeDef Led);
void LED_Off(Led_TypeDef Led);
void LED_Toggle(Led_TypeDef Led);

void DQ_GPIO_Init(void);
void TSENSEOR_GPIO_Init(void);
void FLASH_GPIO_Init(void);
void CHARGE_GPIO_Init(void);

#endif
