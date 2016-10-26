/*******************************************************************************
** 版权:		    利尔达科技有限公司
** 文件名: 		  uart.c
** 版本：  	    V1.00   
** 工作环境: 	  IAR Embedded Workbench IDE 7.70.2
** 作者: 		    eric.xu
** 功能:		    UART驱动						
** 日期: 	      2016-10-23
** 修改日志：	  
**
** 版权所有     (C)2016-2017 利尔达科技有限公司
*******************************************************************************/
#include "uart.h"
#include "rcc.h"
#include <stdarg.h>

UART_HandleTypeDef Uart1Handle;
UART_HandleTypeDef Uart3Handle;
UART_HandleTypeDef Uart4Handle;

/* 暂定义软件版本信息 */
const char Version[] = {"*** NB-IOT Demo Software Version: "__DATE__" "__TIME__" ***"};
const char Copyright[] = {"*** Copyright(C) Lierda hangzhou Corporation. ***"};

#if 0
/*******************************************************************************
以下内容为printf函数重定向至串口的代码，重定向至USART2,可修改
*******************************************************************************/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  while((USART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);//等待发送寄存器为空
  UART2_SendByte(ch);
  return ch;
}
/*******************************************************************************
以上内容为printf函数重定向至串口的代码
*******************************************************************************/
#endif

/*******************************************************************************
** 功能	      USART1初始化
** 参数       无
** 返回值	    无
** 注意       若使用奇偶校验，则数据长度包含校验位
** 修改日志
*******************************************************************************/
void UART1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;//USART1时钟源配置选择为系统时钟 
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);  
	
	/* Enable GPIO RX/TX clock */
	USART1_RX_GPIO_CLK_ENABLE();
	USART1_TX_GPIO_CLK_ENABLE();
	
	/* Enable USART1 clock	*/
	USART1_CLK_ENABLE();
	
	// Disable USART1
	__HAL_UART_DISABLE(&Uart1Handle);
	
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USART1_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USART1_TX_AF;
  
  HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART1_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = USART1_RX_AF; 
  
  HAL_GPIO_Init(USART1_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */	
	
	Uart1Handle.Instance 			 = USART1; 
	Uart1Handle.Init.BaudRate   = 9600;
  Uart1Handle.Init.WordLength = UART_WORDLENGTH_8B;
  Uart1Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart1Handle.Init.Parity     = UART_PARITY_NONE;
  Uart1Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  Uart1Handle.Init.Mode       = UART_MODE_TX_RX;
 
  if(HAL_UART_Init(&Uart1Handle) != HAL_OK);

	__HAL_UART_DISABLE(&Uart1Handle);	//失能USART
  USART1->RQR = 1<<3;	//clears the RXNE flag
  USART1->CR1 |= 1<<5;	//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);	//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(USART1_IRQn);	//使能USART中断
  __HAL_UART_ENABLE(&Uart1Handle);	//使能USART
}


/*******************************************************************************
** 功能	      USART1发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART1_SendByte(uint8_t data)
{
  while((USART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  USART1->TDR = data;
}

/*******************************************************************************
** 功能	      USART1发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART1_SendString(char *str)
{
  while((*str)!=0)
  {
    while((USART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
    USART1->TDR = *str++;       
  }
}


/*******************************************************************************
** 功能	      USART3初始化
** 参数       无
** 返回值	    无
** 注意       若使用奇偶校验，则数据长度包含校验位
** 修改日志
*******************************************************************************/
void UART3_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;//USART1时钟源配置选择为系统时钟 
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);  
	
	/* Enable GPIO RX/TX clock */
	USART3_RX_GPIO_CLK_ENABLE();
	USART3_TX_GPIO_CLK_ENABLE();
	
	/* Enable USART1 clock	*/
	USART3_CLK_ENABLE();
	
	// Disable USART3
	__HAL_UART_DISABLE(&Uart3Handle);
	
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = USART3_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = USART3_TX_AF;
  
  HAL_GPIO_Init(USART1_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = USART3_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = USART3_RX_AF; 
  
  HAL_GPIO_Init(USART3_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */	
	
	Uart3Handle.Instance 			 	= USART3; 
	Uart3Handle.Init.BaudRate   = 9600;
  Uart3Handle.Init.WordLength = UART_WORDLENGTH_8B;
  Uart3Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart3Handle.Init.Parity     = UART_PARITY_NONE;
  Uart3Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  Uart3Handle.Init.Mode       = UART_MODE_TX_RX;
 
  if(HAL_UART_Init(&Uart3Handle) != HAL_OK);

	__HAL_UART_DISABLE(&Uart3Handle);	//失能USART
  USART3->RQR = 1<<3;	//clears the RXNE flag
  USART3->CR1 |= 1<<5;	//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART3_IRQn, 3, 0);	//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(USART3_IRQn);	//使能USART中断
  __HAL_UART_ENABLE(&Uart3Handle);	//使能USART
}


/*******************************************************************************
** 功能	      USART3发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART3_SendByte(uint8_t data)
{
  while((USART3->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  USART3->TDR = data;
}

/*******************************************************************************
** 功能	      USART3发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART3_SendString(char * str)
{
  while((*str)!=0)
  {
    while((USART3->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
    USART3->TDR = *str++;       
  }
}


/*******************************************************************************
** 功能	      USART4初始化
** 参数       无
** 返回值	    无
** 注意       若使用奇偶校验，则数据长度包含校验位
** 修改日志
*******************************************************************************/
void UART4_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* 选择USART时钟源 */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART4;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_UART4CLKSOURCE_SYSCLK;//USART1时钟源配置选择为系统时钟 
  HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct);  
	
	/* Enable GPIO RX/TX clock */
	UART4_RX_GPIO_CLK_ENABLE();
	UART4_TX_GPIO_CLK_ENABLE();
	
	/* Enable USART1 clock	*/
	UART4_CLK_ENABLE();
	
	// Disable USART3
	__HAL_UART_DISABLE(&Uart4Handle);
	
	/* UART TX GPIO pin configuration  */
	GPIO_InitStruct.Pin       = UART4_TX_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = UART4_TX_AF;
  
  HAL_GPIO_Init(UART4_TX_GPIO_PORT, &GPIO_InitStruct);
  
  /* UART RX GPIO pin configuration  */
  GPIO_InitStruct.Pin       = UART4_RX_PIN;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  GPIO_InitStruct.Alternate = UART4_RX_AF; 
  
  HAL_GPIO_Init(UART4_RX_GPIO_PORT, &GPIO_InitStruct);
  
  /* Put the USART peripheral in the Asynchronous mode (UART Mode) */
  /* UART configured as follows:
      - Word Length = 8 Bits
      - Stop Bit = One Stop bit
      - Parity = None
      - BaudRate = 9600 baud
      - Hardware flow control disabled (RTS and CTS signals) */	
	
	Uart4Handle.Instance 			 	= UART4; 
	Uart4Handle.Init.BaudRate   = 9600;
  Uart4Handle.Init.WordLength = UART_WORDLENGTH_8B;
  Uart4Handle.Init.StopBits   = UART_STOPBITS_1;
  Uart4Handle.Init.Parity     = UART_PARITY_NONE;
  Uart4Handle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
  Uart4Handle.Init.Mode       = UART_MODE_TX_RX;
 
  if(HAL_UART_Init(&Uart4Handle) != HAL_OK);

	__HAL_UART_DISABLE(&Uart4Handle);	//失能USART
  UART4->RQR = 1<<3;	//clears the RXNE flag
  UART4->CR1 |= 1<<5;	//enable the RXNE interrupt
  HAL_NVIC_SetPriority(UART4_IRQn, 3, 0);	//设置USART中断优先级:抢占优先级3；子优先级0（M0+内核无子优先级）
  HAL_NVIC_EnableIRQ(UART4_IRQn);	//使能USART中断
  __HAL_UART_ENABLE(&Uart4Handle);	//使能USART
}


/*******************************************************************************
** 功能	      UART4发送一字节数据
** 参数       data：将要发送的字节
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART4_SendByte(uint8_t data)
{
  while((UART4->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  UART4->TDR = data;
}

/*******************************************************************************
** 功能	      UART4发送字符串
** 参数       str：将要发送的字符串
** 返回值	    无
** 注意       
** 修改日志
*******************************************************************************/
void UART4_SendString(char * str)
{
  while((*str)!=0)
  {
    while((UART4->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
    UART4->TDR = *str++;       
  }
}

void Debug_Init(void)
{
	UART1_Init();	
	
	Debug_Printf("%s\r\n", Version);
	Debug_Printf("%s\r\n", Copyright);
}

/*******************************************************************************
** 功能	      串口打印调试
** 参数       可变参数
** 返回值	    无
** 注意       可根据使用的串口进行修改
** 修改日志
*******************************************************************************/
void Debug_Printf(const char *fmt, ...)
{
	char buf[512];
	va_list va_args;
	
	// Start the varargs processing.
	va_start(va_args, fmt);
	
	vsnprintf((char *)buf, sizeof(buf), fmt, va_args);
	
	//End the varargs processing.
	va_end(va_args);
	
	//the real printf, depends on the arch
	UART1_SendString((char *)buf);	
}