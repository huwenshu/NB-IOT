/*******************************************************************************
** ��Ȩ:		    ������Ƽ����޹�˾
** �ļ���: 		  uart.c
** �汾��  	    V1.00   
** ��������: 	  IAR Embedded Workbench IDE 7.70.2
** ����: 		    eric.xu
** ����:		    UART����						
** ����: 	      2016-10-23
** �޸���־��	  
**
** ��Ȩ����     (C)2016-2017 ������Ƽ����޹�˾
*******************************************************************************/
#include "uart.h"
#include "rcc.h"
#include <stdarg.h>

UART_HandleTypeDef Uart1Handle;
UART_HandleTypeDef Uart3Handle;
UART_HandleTypeDef Uart4Handle;

/* �ݶ�������汾��Ϣ */
const char Version[] = {"*** NB-IOT Demo Software Version: "__DATE__" "__TIME__" ***"};
const char Copyright[] = {"*** Copyright(C) Lierda hangzhou Corporation. ***"};

#if 0
/*******************************************************************************
��������Ϊprintf�����ض��������ڵĴ��룬�ض�����USART2,���޸�
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
  while((USART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);//�ȴ����ͼĴ���Ϊ��
  UART2_SendByte(ch);
  return ch;
}
/*******************************************************************************
��������Ϊprintf�����ض��������ڵĴ���
*******************************************************************************/
#endif

/*******************************************************************************
** ����	      USART1��ʼ��
** ����       ��
** ����ֵ	    ��
** ע��       ��ʹ����żУ�飬�����ݳ��Ȱ���У��λ
** �޸���־
*******************************************************************************/
void UART1_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* ѡ��USARTʱ��Դ */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART1CLKSOURCE_SYSCLK;//USART1ʱ��Դ����ѡ��Ϊϵͳʱ�� 
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

	__HAL_UART_DISABLE(&Uart1Handle);	//ʧ��USART
  USART1->RQR = 1<<3;	//clears the RXNE flag
  USART1->CR1 |= 1<<5;	//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);	//����USART�ж����ȼ�:��ռ���ȼ�3�������ȼ�0��M0+�ں��������ȼ���
  HAL_NVIC_EnableIRQ(USART1_IRQn);	//ʹ��USART�ж�
  __HAL_UART_ENABLE(&Uart1Handle);	//ʹ��USART
}


/*******************************************************************************
** ����	      USART1����һ�ֽ�����
** ����       data����Ҫ���͵��ֽ�
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void UART1_SendByte(uint8_t data)
{
  while((USART1->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  USART1->TDR = data;
}

/*******************************************************************************
** ����	      USART1�����ַ���
** ����       str����Ҫ���͵��ַ���
** ����ֵ	    ��
** ע��       
** �޸���־
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
** ����	      USART3��ʼ��
** ����       ��
** ����ֵ	    ��
** ע��       ��ʹ����żУ�飬�����ݳ��Ȱ���У��λ
** �޸���־
*******************************************************************************/
void UART3_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* ѡ��USARTʱ��Դ */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_USART3;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_USART3CLKSOURCE_SYSCLK;//USART1ʱ��Դ����ѡ��Ϊϵͳʱ�� 
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

	__HAL_UART_DISABLE(&Uart3Handle);	//ʧ��USART
  USART3->RQR = 1<<3;	//clears the RXNE flag
  USART3->CR1 |= 1<<5;	//enable the RXNE interrupt
  HAL_NVIC_SetPriority(USART3_IRQn, 3, 0);	//����USART�ж����ȼ�:��ռ���ȼ�3�������ȼ�0��M0+�ں��������ȼ���
  HAL_NVIC_EnableIRQ(USART3_IRQn);	//ʹ��USART�ж�
  __HAL_UART_ENABLE(&Uart3Handle);	//ʹ��USART
}


/*******************************************************************************
** ����	      USART3����һ�ֽ�����
** ����       data����Ҫ���͵��ֽ�
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void UART3_SendByte(uint8_t data)
{
  while((USART3->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  USART3->TDR = data;
}

/*******************************************************************************
** ����	      USART3�����ַ���
** ����       str����Ҫ���͵��ַ���
** ����ֵ	    ��
** ע��       
** �޸���־
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
** ����	      USART4��ʼ��
** ����       ��
** ����ֵ	    ��
** ע��       ��ʹ����żУ�飬�����ݳ��Ȱ���У��λ
** �޸���־
*******************************************************************************/
void UART4_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;

  /* ѡ��USARTʱ��Դ */  
  RCC_PeriphCLKInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART4;
  RCC_PeriphCLKInitStruct.Usart1ClockSelection = RCC_UART4CLKSOURCE_SYSCLK;//USART1ʱ��Դ����ѡ��Ϊϵͳʱ�� 
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

	__HAL_UART_DISABLE(&Uart4Handle);	//ʧ��USART
  UART4->RQR = 1<<3;	//clears the RXNE flag
  UART4->CR1 |= 1<<5;	//enable the RXNE interrupt
  HAL_NVIC_SetPriority(UART4_IRQn, 3, 0);	//����USART�ж����ȼ�:��ռ���ȼ�3�������ȼ�0��M0+�ں��������ȼ���
  HAL_NVIC_EnableIRQ(UART4_IRQn);	//ʹ��USART�ж�
  __HAL_UART_ENABLE(&Uart4Handle);	//ʹ��USART
}


/*******************************************************************************
** ����	      UART4����һ�ֽ�����
** ����       data����Ҫ���͵��ֽ�
** ����ֵ	    ��
** ע��       
** �޸���־
*******************************************************************************/
void UART4_SendByte(uint8_t data)
{
  while((UART4->ISR & UART_FLAG_TXE) != UART_FLAG_TXE);
  UART4->TDR = data;
}

/*******************************************************************************
** ����	      UART4�����ַ���
** ����       str����Ҫ���͵��ַ���
** ����ֵ	    ��
** ע��       
** �޸���־
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
** ����	      ���ڴ�ӡ����
** ����       �ɱ����
** ����ֵ	    ��
** ע��       �ɸ���ʹ�õĴ��ڽ����޸�
** �޸���־
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