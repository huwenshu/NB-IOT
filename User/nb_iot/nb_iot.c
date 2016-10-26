/*******************************************************************************
** ��Ȩ:		    ������Ƽ����޹�˾
** �ļ���: 		  lsd_protocol.c
** �汾��  	    V1.00   
** ��������: 	  IAR Embedded Workbench IDE 7.70.2
** ����: 		    eric.xu
** ����:		    NB-IOT����������Э��					
** ����: 	      2016-10-23
** �޸���־��	  
**
** ��Ȩ����     (C)2016-2017 ������Ƽ����޹�˾
*******************************************************************************/
#include "nb_iot.h"
#include "lsd_bsp.h"
#include "ds18b20.h"
#include "timer.h"
#include <string.h>
#include <stdlib.h>


typedef enum {
	TRIGGER_START = 0,
	TRIGGER_STOP,
	TRIGGER_RESTART
} TRIGGER_T;

//�Ƿ񿼻��ַ�������Ψһ���豸��Ϣ
const uint8_t	dev_addr[DEV_LEN] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
extern UART_HandleTypeDef Uart3Handle;		//for AT channel

//CONFIRM_PACKET_T *s_confirm_pack;
CONFIRM_PACKET_T	*confirm_packet_cmd0;
CONFIRM_PACKET_T	*confirm_packet_cmd1;

//static SYSTICK_PACKET_T sys_pack;

//uint8_t *alloc_systick_packet(SYSTICK_PACKET_T *packet)
//{
//	if(packet != NULL)	return;
//	
//	packet->start 	= 0xFFAA;
//	packet->format 	= 0xA0A0;
//	packet->ver			= 0x00;
//	
//}

//static void Error_Handler(void)
//{
//}

uint8_t charsToHex(char *asc)
{  
  uint8_t hex;
  if((asc[0]>='0')&&(asc[0]<='9')){
    hex=asc[0]-0x30;
  }
  else if((asc[0]>=0x41)&&(asc[0]<=46)){
    hex=asc[0]-0x41+10;
  }
  else if((asc[0]>=0x61)&&(asc[0]<=66)){
    hex=asc[0]-0x61+10;
  }
  
  if((asc[1]>='0')&&(asc[1]<='9')){
    hex+=(asc[1]-0x30)<<4;
  }
  else if((asc[1]>=0x41)&&(asc[1]<=46)){
    hex+=(asc[1]-0x41+10)<<4;
  }
  else if((asc[1]>=0x61)&&(asc[1]<=66)){
    hex+=(asc[1]-0x61+10)<<4;
  } 
  
  return hex;
}

void  hexToAscs(uint8_t hex,uint8_t *ascs)
{
  uint8_t h,l;
  h=(hex>>4)&0x0f;
  l=(hex&0x0f);
  if((h>=0)&&(h<=9))
    ascs[0]=h+0x30;
  else if((h>=10)&&(h<=15)){
    ascs[0]=h+0x41-10;
  }else{
    ascs[0]=0xff;
  }
  
  if((l>=0)&&(l<=9))
    ascs[1]=l+0x30;
  else if((l>=10)&&(l<=15)){
    ascs[1]=l+0x41-10;
  }else{
    ascs[1]=0xff;
  }  
   
}

uint16_t HexsToAscs(uint8_t *hexs,char * ascs,uint16_t length, uint8_t litt)
{
  uint16_t j=0;
  if(litt == 0) //�������
  {
    for(uint16_t i=0;i<length;i++){
      hexToAscs(hexs[i],ascs+j);
      j+=2;
    } 
  }
  else if(litt == 1) //�������
  {
    for(uint16_t i=0;i<length;i++){
      hexToAscs(hexs[length - i-1],ascs+j);
      j+=2;
  } 
  }  
  return j;
}

uint16_t  ascsToHexs(uint8_t *ascs,char * hexs,uint16_t length)
{
  uint16_t j=0;
  for(uint16_t i=0;i<length;i+=2){
    
    hexs[j++]=charsToHex(ascs+i);
    //hexs[i]=charToHex(ascs[i]);   
  } 
  return j;
}

/**
* ��ȡAT������Ӧ��ÿһ������ֵ
*
* @author qhq
*
* @param srcStr ָ��AT������Ӧ������
* @param word  ����ÿһ������ֵ
* @param size  word�ĸ���
*
* @return int  ����ʵ��word��
*/
int at_get_words(char chop,char *srcStr, char **word, int size)
{
  int index = 0;
  int i = 0;
  char *str = srcStr;
  
  while (*(str + i) != '\0')
  {
    if (*(str + i) == chop)	//','
    {
      //strncmp(word[index], str, i);
      word[index] = str;
      word[index++][i] = '\0';
      str = (str + i + 1);
      i = -1;
    }
    
    if (*(str + i) == '\r')
    {
      word[index] = str;
      word[index++][i] = '\0';
      str = (str + i);
      i = 0;
      
      break;
    }
    
    if (index >= size)
    {
      return index;
    }
    
    i++;
  }
  
  if (strlen(str) > 0)
  {
    word[index++] = str;
  }
  
  return index;
}

/*	eg:
***	AT+NSORF=1,10
***	1,192.168.5.1,1024,2,ABAB,0
***	OK
*/
//����ATָ��صĸ�ʽ���ӽ���������ȡ��udp����
void convert_data(UDP_RECV_PACKET *udp_recv_data, char *uart_data)
{
  char *words[4] = { NULL };
  int i;
  i = at_get_words(',', uart_data, words, 4);
  //  udp_recv_pack->udp_data = (SERVER_TO_IOT_PACKET )words[4];
  strcpy((char *)((udp_recv_data->udp_data)), words[3]);
	
//	udp_recv_pack->len = 
}

/* ���ݰ���ʼ��ַ�������ݰ���ȥCRC�ֶκ�ĳ��� */
uint8_t calc_crc(uint8_t *packet, uint32_t len)
{
	uint32_t i, ret = 0;
	uint32_t temp = (packet[5]<<8) | packet[6]; 
	
	if((packet == NULL) || (temp != len))
		return -1;

	//ȥ����ʼλλ
	for(i = 2; i < len; i++)
	{
		ret += packet[i];
	}	
	ret &= 0xff;
	
	return ret;
}

void dispose_multipacket(UDP_RECV_PACKET *udp_recv_data)
{
	uint8_t crc;
	uint32_t len = 	udp_recv_data->len - sizeof(IOT_TO_DEV_PACKET_T);	//��ȡ�����
	TRIGGER_CMD0_T	*cmd0 = (TRIGGER_CMD0_T *)(udp_recv_data + sizeof(IOT_TO_DEV_PACKET_T) - 1);
	TRIGGER_CMD1_T	*cmd1	=	(TRIGGER_CMD1_T *)(udp_recv_data + sizeof(IOT_TO_DEV_PACKET_T) - 1);
	
	crc = calc_crc((uint8_t *)udp_recv_data, udp_recv_data->len - 1);
	/*У���յ����ֶ�CRC�Ƿ�ƥ��*/
	if(crc != udp_recv_data->udp_data[udp_recv_data->len-1])	return;
	
	if(len == sizeof(TRIGGER_CMD0_T))
	{
		if(cmd0->cmd == SUB_CMD_TRIGGER)
		{
			switch(cmd0->flag)
			{
			case TRIGGER_START:		//���������ϱ�
					Timer_Start();
					break;
			case TRIGGER_STOP:		//����ֹͣ�ϱ�
					Timer_Stop();
					break;
			case TRIGGER_RESTART:	//�������¿�ʼ�ϱ�
					Timer_Stop();
					Timer_Start();
					break;
			}
			fill_confirm_packet(confirm_packet_cmd0, (uint8_t *)cmd0);
		}		
	}
	
	if(len == sizeof(TRIGGER_CMD1_T))
	{
		if(cmd1->cmd == SUB_CMD_CONFIG)
		{
			if(cmd1->mode == 0)	//�趨�̶��ϱ�����ģʽ
			{
				Timer_Stop();
				Timer_Init(cmd1->t2);
			}		
			if(cmd1->mode == 1)	//���ò���ʱ�����ϱ�����ģʽ
			{
				Timer_Stop();
				Timer_Init(cmd1->t2);
				Timer_SetRegion(cmd1->t1, cmd1->t2);
			}
			fill_confirm_packet(confirm_packet_cmd1, (uint8_t *)cmd1);
		}		
	}
	
}

/*
**	�豸ȷ�ϻظ�֡����Ҫ���ݲ�ͬ����������
*/
void fill_confirm_packet(CONFIRM_PACKET_T *packet, uint8_t *trigger_packet)
{
	static uint32_t confirm_packet_cnt = 0;
	uint32_t msg_len, temper;
	
	if(packet == NULL)	return;
	
	if(trigger_packet[0] == SUB_CMD_TRIGGER)
	{
		msg_len = sizeof(CONFIRM_PACKET_T)+sizeof(CONFIRM_CMD_PACKET_T)+sizeof(PAYLOAD_PACKET_T);	
//		packet 	= (CONFIRM_PACKET_T *)malloc(msg_len+1);		
	}
	if(trigger_packet[0] == SUB_CMD_CONFIG)
	{
		msg_len =	sizeof(CONFIRM_PACKET_T)+sizeof(CONFIRM_CMD_PACKET_T);
//		packet 	= (CONFIRM_PACKET_T *)malloc(msg_len+1);
	}
	
	packet->start 	= START_BIT;
	packet->format	= CMD_CONFIRM;
	packet->ver			= CMD_VER;
	
	packet->dev_len	=	DEV_LEN;
	memcpy(packet->dev_addr, dev_addr, DEV_LEN);
	
	packet->msg_len = msg_len+1;
	
	packet->arr[0] 	= trigger_packet[0];
											
	packet->arr[1]	=	(confirm_packet_cnt & 0xff000000) >> 24;
	packet->arr[2] 	= (confirm_packet_cnt & 0xff0000) >> 16;
	packet->arr[3]	=	(confirm_packet_cnt & 0xff00)	>> 8;
	packet->arr[4]	=	(confirm_packet_cnt &	0xff);
	packet->arr[5]	=	1;	//flag, default settting
	
	if(trigger_packet[0] == SUB_CMD_TRIGGER)
	{
		packet->arr[6]	= 0;
		packet->arr[7]	=	sizeof(PAYLOAD_PACKET_T);
		
		temper = DS18B20_Read();
		//payload data 
		packet->arr[8]	=	1;	//�����ϱ�
		packet->arr[9]	=	(temper & 0xff000000) >> 24;
		packet->arr[10]	=	(temper & 0xff0000) >> 16;
		packet->arr[11]	=	(temper & 0xff00) >> 8;
		packet->arr[12]	=	(temper & 0xff);
	
		packet->arr[13]	=	OCP_GetState();
	}
	if(trigger_packet[0] == SUB_CMD_CONFIG)
	{
		packet->arr[6]	=	0;
		packet->arr[7]	=	0;
	}

	packet->arr[msg_len-sizeof(CONFIRM_PACKET_T)-1]	=	calc_crc((uint8_t *)packet, msg_len);

	confirm_packet_cnt++;	
}

void alloc_confirm_packet(void)
{
	uint32_t msg_len;
	
	msg_len = sizeof(CONFIRM_PACKET_T)+sizeof(CONFIRM_CMD_PACKET_T)+sizeof(PAYLOAD_PACKET_T);
	confirm_packet_cmd0 	= (CONFIRM_PACKET_T *)malloc(msg_len+1);
	
	msg_len = sizeof(CONFIRM_PACKET_T)+sizeof(CONFIRM_CMD_PACKET_T);
	confirm_packet_cmd1 	= (CONFIRM_PACKET_T *)malloc(msg_len+1);
}

/* 0 success, -1 fail */
uint8_t fill_heart_packet(HEART_PACKET_T *packet, uint32_t stamp, uint16_t rect)
{	
	if(packet == NULL)
			return -1;

	packet->start 	= START_BIT;	//��ʼλ
	packet->format 	= CMD_HEART; 	//����ָ��
	packet->ver 		= CMD_VER;		//�汾
	packet->dev_len	=	DEV_LEN;		//�豸��ų���
	memcpy(packet->dev_addr, dev_addr, DEV_LEN);	//�豸���
	
	packet->stamp_high 	= 0;
	packet->stamp_low 	=	stamp;	//ʱ���
	packet->rect 			= rect;	//�ط��ɹ�����
	packet->msg_len		= sizeof(HEART_PACKET_T);
	
	packet->crc = calc_crc((uint8_t *)packet, sizeof(HEART_PACKET_T)-1);
	
	return 0;
}

/* 0 success, -1 fail */
uint8_t fill_timer_packet(TIMER_PACKET_T *packet, uint32_t stamp, uint32_t msg_ct, uint32_t ack_ct, uint16_t rect)
{	
	if(packet == NULL)
			return -1;
	
	memset(packet, 0, sizeof(TIMER_PACKET_T));
	
	packet->start 	= START_BIT;	//��ʼλ
	packet->format 	= CMD_REPORT; 	//�ϱ�ָ��
	packet->ver 		= CMD_VER;		//�汾
	packet->dev_len	=	DEV_LEN;		//�豸��ų���
	memcpy(packet->dev_addr, dev_addr, DEV_LEN);	//�豸���
	
	packet->stamp_high 	= 0;
	packet->stamp_low 	=	stamp;	//ʱ���
	packet->rect 				= rect;
	
	packet->payload_len = sizeof(PAYLOAD_PACKET_T);
	packet->payload.type	=	0;
	packet->payload.temper	= DS18B20_Read();		//��������
	packet->payload.state		= OCP_GetState();	//����״̬
	
	packet->msg_ct		= msg_ct;		//�豸�������ݰ�ͳ��
	packet->ack_ct		=	ack_ct;		//�豸�������ݰ��յ�ģ��Ӧ�����
	packet->rect			=	rect;			//�ط�����
			
	packet->crc = calc_crc((uint8_t *)packet, sizeof(TIMER_PACKET_T)-1);
	
	return 0;
}

#if 0
void bs_connect(void)
{
  unsigned char i;
  LED_On(LED1);
  HAL_Delay(1000);
  LED_Off(LED1);
  HAL_Delay(1000);
  LED_On(LED1);
  HAL_Delay(1000);
  LED_Off(LED1);
  HAL_Delay(1000);
  LED_On(LED1);
  HAL_Delay(1000);
  LED_Off(LED1);
  
//#if  SH_NBIOT
//  HAL_UART_Transmit(&UartHandle, (uint8_t*)AT_NTSETID, strlen(AT_NTSETID), 500); // ����EIMI����
  HAL_Delay(1000);
//#endif
  HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_CFUN, strlen(AT_CFUN), 500); // �����ֻ�ȫ����
  HAL_Delay(1000);
//  LED_On(LED_CONN_PIN);
  HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_CGDCONT, strlen(AT_CGDCONT), 500); // ����PDP������
  HAL_Delay(1000);
//  LED_Off(LED_CONN_PIN);
  HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_NCDP, strlen(AT_NCDP), 500);//����NCDP������
  HAL_Delay(1000);
//  LED_On(LED_CONN_PIN);
  HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_CGATT, strlen(AT_CGATT), 500); //GPRS����
  
  HAL_Delay(1000);
  HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_CSCON1, strlen(AT_CSCON1), 500); //�������ӻ�վ��
  HAL_Delay(1000);
  
  
  //�ȴ������ϻ�վ
  while(1)
  {
    HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_CSCON2, strlen(AT_CSCON2), 500);
    ////      i = strlen(aRxBuffer);
    memset(aRxBuffer, 0,RXBUFFERSIZE);
    //      HAL_Delay(5);
    //      if(HAL_UART_Receive(&UartHandle, (uint8_t *)aRxBuffer, RXBUFFERSIZE, 5000) != HAL_OK)
    // �������Ϊʲôֻ�ܽ���һ�Σ�
    // �������ݣ������ָ����ʱ��û�н��չ����ݣ�����������
    __HAL_UART_CLEAR_FLAG(&Uart3Handle, UART_CLEAR_OREF);
    if(HAL_UART_Receive(&Uart3Handle, (uint8_t *)aRxBuffer, (strlen(AT_CSCON_RT)+1), 5000) != HAL_OK)
    {
      Error_Handler(); 
      HAL_Delay(4000);
    }
    else if(strstr(aRxBuffer, AT_CSCON_RT) != NULL)
    {
      HAL_Delay(4000);
      break;
    }     
  }
  //�ȴ���ȡIP��ַ
  
  while(1)
  {
    HAL_Delay(20000);
    HAL_UART_Transmit(&Uart3Handle, (uint8_t*)AT_CGPADDR, strlen(AT_CGPADDR), 500);
    memset(aRxBuffer, 0, RXBUFFERSIZE);
    __HAL_UART_CLEAR_FLAG(&UartHandle, UART_CLEAR_OREF);
    
    if(HAL_UART_Receive(&Uart3Handle, (uint8_t *)aRxBuffer, (strlen(AT_CGP_RT)+2), 5000) != HAL_OK)
    {
      Error_Handler();  
    }
    else if(strstr(aRxBuffer, AT_CGP_RT) != NULL)
    {
      
      break;
    }
    
  }
  LED_On(LED1);
}
#endif

void uart_coap_thread(void const *param)
{
	
}

void uart_udp_thread(void const *param)
{
	bs_connect();
	
	while(1)
	{
		
	}
}

void send_message_thread(void const *param)
{

}