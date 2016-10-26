#ifndef _DS18B20_H_
#define _DS18B20_H_

#include "lsd_bsp.h"

#define  SkipROM    0xCC     //跳过ROM  
#define  SearchROM  0xF0  //搜索ROM  
#define  ReadROM    0x33  //读ROM  
#define  MatchROM   0x55  //匹配ROM  
#define  AlarmROM   0xEC  //告警ROM 

#define  StartConvert    0x44  //开始温度转换，在温度转换期间总线上输出0，转换结束后输出1  
#define  ReadScratchpad  0xBE  //读暂存器的9个字节  
#define  WriteScratchpad 0x4E  //写暂存器的温度告警TH和TL  
#define  CopyScratchpad  0x48  //将暂存器的温度告警复制到EEPROM，在复制期间总线上输出0，复制完后输出1  
#define  RecallEEPROM    0xB8    //将EEPROM的温度告警复制到暂存器中，复制期间输出0，复制完成后输出1  
#define  ReadPower       0xB4    //读电源的供电方式：0为寄生电源供电；1为外部电源供电 

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