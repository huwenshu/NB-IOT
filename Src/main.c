/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "string.h"
#include "trcUser.h"
#include "rcc.h"
#include "rtc.h"
#include "uart.h"
#include "wdg.h"
#include "timer.h"
#include "nb_iot.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
#define ULONG_MAX			0xffffffffUL
#define	WAIT_MAX_MS		30000

/* Private variables ---------------------------------------------------------*/
osThreadId Wdg_Thread_Handle, LEDThread2Handle, AT_Thread_Handle, Message_Thread_Handle;
__IO ITStatus UartReady = RESET;
/* Buffer used for transmission */
uint8_t aTxBuffer[50] = "AT+NRB\r\n";

/* message successful transfer count */
static uint16_t resend_cnt = 0;
static uint32_t timer_packet_cnt = 0;
static uint32_t msg_ack_cnt = 0;

/* Private function prototypes -----------------------------------------------*/
static uint16_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);
void SystemClock_Config(void);

/* Private functions ---------------------------------------------------------*/

/**
* @brief  Main program
* @param  None
* @retval None
*/
extern UART_HandleTypeDef Uart3Handle;
UART_HandleTypeDef	UartHandle;
// 1 是否需要连接服务器的动作？目前没看到，socket直接创建一个吧，问题是
// 2 先移植串口吧，以后多用结构体，免得数据乱。

static void Error_Handler(void)
{
  /* Turn LED3 on */
	LED_On(LED3);
  while(1)
  {
    /* Error if LED3 is slowly blinking (1 sec. period) */
		LED_Toggle(LED3); 
    HAL_Delay(1000); 
  }
}

#if 0
//Create Socket, +NSOCR=<type>,<protocol>,<listen port>[,<receive control>]
#define         AT_NSOCR        "AT+NSOCR=DGRAM,17,8899\r\n"	
//Send to Command(UDP), +NSOST= socket, remote_addr, remote_port,local_port,length, data
#define         AT_NSOST        "AT+NSOST=1,\"192.158.5.1\",8080,19,\r\n"  //send cmd
//Receive Command(UDP),	+NSORF=<socket>, <req_length>
#define         AT_NSORF        "AT+NSORF=1,30\r\n"  //udp receive cmd
//Close Socket, +NSOCL= socket
#define         AT_NSOCL        "AT+NSOCL=1\r\n"   //close socket
//Socket message arrived indicator(response only), eg, +NSONMI:1,10
#define 				AT_NSONMI				"+NSONMI:"				//
//Test IP network connectivity to a remote host, +NPING=<remote_address>
#define 				AT_NPING				"AT+NPING=\"192.168.1.1\"\r\n"

#define         AT_OK           "OK"
#define         AT_CFUN         "AT+CFUN=1\r\n"    //Set phone functionality  
#define         AT_CGDCONT      "AT+CGDCONT=1,\"IP\",\"PCCW\"\r\n"  //Define PDP Context
#define         AT_CGATT        "AT+CGATT=1\r\n"        //PS attach or detach 

#define         AT_CSCON        "AT+CSCON?\r\n"         //Signalling connection status
#define         AT_CSCON1       "AT+CSCON=1\r\n"

#define         AT_CGSN         "AT+CGSN=20160924142401\r\n"  //Request Product Serial Number
#define         AT_NRB          "AT+NRB\r\n"                   //Neul Reboot 

#endif

/**
 * 获取AT命令响应后每一个参数值
 *
 * @author qhq
 *
 * @param srcStr 指向AT命令响应的内容
 * @param word  保存每一个参数值
 * @param size  word的个数
 *
 * @return int  返回实际word数
 */


int main(void)
{
  /* STM32L4xx HAL library initialization:
  - Configure the Flash prefetch
  - Systick timer is configured by default as source of time base, but user 
  can eventually implement his proper time base source (a general purpose 
  timer for example or other time source), keeping in mind that Time base 
  duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
  handled in milliseconds basis.
  - Set NVIC Group Priority to 4
  - Low Level Initialization
  */
	// freertos trace init
	vTraceInitTraceData();	
		
  HAL_Init();
  
  /* Configure the System clock to 80 MHz */
//  SystemClock_Config();
	MySystemClock_Config();
		
	/* set rtc wakeup time to 20s */
	RTC_Config(20);
	
	/* 打印当前版本信息，编译时间 */
	Debug_Init();

	LED_Init(); 
	
	osThreadDef(THREAD_1, wdg_thread, osPriorityLow, 0, (configMINIMAL_STACK_SIZE));
	
	Wdg_Thread_Handle = osThreadCreate(osThread(THREAD_1), NULL);
	
//  osThreadDef(THREAD_3, uart_at_thread, osPriorityNormal, 0, (configMINIMAL_STACK_SIZE));
//
//  AT_Thread_Handle = osThreadCreate(osThread(THREAD_3), NULL);
  
	osThreadDef(THREAD_4, send_message_thread, osPriorityNormal, 0, (configMINIMAL_STACK_SIZE));
	
	Message_Thread_Handle = osThreadCreate(osThread(THREAD_4), NULL);
	
	//start recording trace here.
	uiTraceStart();
	
  /* Start scheduler */
  osKernelStart();
  
  /* We should never get here as control is now taken by the scheduler */
  for (;;);
  
}



/**
* @brief  System Clock Configuration
*         The system Clock is configured as follows :
*            System Clock source            = PLL (MSI)
*            SYSCLK(Hz)                     = 80000000
*            HCLK(Hz)                       = 80000000
*            AHB Prescaler                  = 1
*            APB1 Prescaler                 = 1
*            APB2 Prescaler                 = 1
*            MSI Frequency(Hz)              = 4000000
*            PLL_M                          = 1
*            PLL_N                          = 40
*            PLL_R                          = 2
*            PLL_P                          = 7
*            PLL_Q                          = 4
*            Flash Latency(WS)              = 4
* @param  None
* @retval None
*/
void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  
  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
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
    /* Initialization Error */
    while(1);
  }
}

#ifdef  USE_FULL_ASSERT

/**
* @brief  Reports the name of the source file and the source line number
*   where the assert_param error has occurred.
* @param  file: pointer to the source file name
* @param  line: assert_param error line source number
* @retval None
*/
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  
  /* Infinite loop */
  while (1)
  {}
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
/**
  * @brief  Input Capture callback in non blocking mode 
  * @param  htim : TIM IC handle
  * @retval None
*/
void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{ 
	BaseType_t xHigherPriorityTaskWoken;
	
	/* RTC定时唤醒消息发送任务，发送通知事件 */
	if(Message_Thread_Handle != NULL)
	{
		/* Toggle LED2 */
  	LED_Toggle(LED2);
			
		xHigherPriorityTaskWoken = pdFALSE;		
		
		xTaskNotifyFromISR( Message_Thread_Handle, HEART_BIT, eSetBits, &xHigherPriorityTaskWoken );
		
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	uint32_t count;
	
	/* TIM6 for base tick */
	if(htim->Instance == TIM6)
	{
		HAL_IncTick();
	}
	
	/* TIM2 for send message event */
	if(htim->Instance == TIM2)
	{
		BaseType_t xHigherPriorityTaskWoken;
		
		/* TIM2定时唤醒消息发送任务，发送通知事件 */
		if(Message_Thread_Handle != NULL)
		{
			/* Toggle LED1 */
			LED_Toggle(LED1);
				
			xHigherPriorityTaskWoken = pdFALSE;		
				
			xTaskNotifyFromISR( Message_Thread_Handle, TIMER_BIT, eSetBits, &xHigherPriorityTaskWoken );
		
			/* 检测上报测试的固定事件 */
			count = Timer_GetRegion();
			if(count > 0)
			{
				count--;
			} else {
				//上报测试的固定时间到了, 关闭定时器
				Timer_Stop();
			}
			
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		}
	}
}
