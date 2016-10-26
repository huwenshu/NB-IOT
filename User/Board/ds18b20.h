#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "lsd_bsp.h"

#define  SkipROM    0xCC     //����ROM  
#define  SearchROM  0xF0  //����ROM  
#define  ReadROM    0x33  //��ROM  
#define  MatchROM   0x55  //ƥ��ROM  
#define  AlarmROM   0xEC  //�澯ROM 

#define  StartConvert    0x44  //��ʼ�¶�ת�������¶�ת���ڼ����������0��ת�����������1  
#define  ReadScratchpad  0xBE  //���ݴ�����9���ֽ�  
#define  WriteScratchpad 0x4E  //д�ݴ������¶ȸ澯TH��TL  
#define  CopyScratchpad  0x48  //���ݴ������¶ȸ澯���Ƶ�EEPROM���ڸ����ڼ����������0������������1  
#define  RecallEEPROM    0xB8    //��EEPROM���¶ȸ澯���Ƶ��ݴ����У������ڼ����0��������ɺ����1  
#define  ReadPower       0xB4    //����Դ�Ĺ��緽ʽ��0Ϊ������Դ���磻1Ϊ�ⲿ��Դ���� 

#define HIGH		GPIO_PIN_RESET
#define LOW			GPIO_PIN_SET

#define SetDQ(STATE)								HAL_GPIO_WritePin(DS_DQ_GPIO_PORT, DS_DQ_PIN, STATE)
#define GetDQ()											HAL_GPIO_ReadPin(DS_DQ_GPIO_PORT, DS_DQ_PIN)

//#define Delay_ms()	HAL_Delay()		
//#define Delay_us()

uint8_t DS18B20_Read_Byte(void);
void DS18B20_Write_Byte(uint8_t data);
void ReadRom(unsigned char *Read_Addr);
void DS18B20_Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL);
void DS18B20_StartConvert(void);
void DS18B20_Start(void);
uint32_t DS18B20_Read(void);


#endif