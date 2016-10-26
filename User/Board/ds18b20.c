/*******************************************************************************
** ��Ȩ:		    ������Ƽ����޹�˾
** �ļ���: 		  ds18b20.c
** �汾��  	    V1.00   
** ��������: 	  IAR Embedded Workbench IDE 7.70.2
** ����: 		    eric.xu
** ����:		    ds18b20����						
** ����: 	      2016-10-24
** �޸���־��	  
**
** ��Ȩ����     (C)2016-2017 ������Ƽ����޹�˾
*******************************************************************************/
#include "ds18b20.h"
#include "rcc.h"

#define EnableINT()    
#define DisableINT() 

#define DS_PRECISION 0x7f   //�������üĴ��� 1f=9λ; 3f=10λ; 5f=11λ; 7f=12λ;  
#define DS_AlarmTH  0x64  
#define DS_AlarmTL  0x8a  
#define DS_CONVERT_TICK 1000

static unsigned char TempX_TAB[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};

static void DQ_InputModeConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	DS_DQ_GPIO_CLK_ENABLE();	
	
	GPIO_InitStruct.Pin       	= DS_DQ_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull      	= GPIO_NOPULL;
  HAL_GPIO_Init(DS_DQ_GPIO_PORT, &GPIO_InitStruct); 		
}

static void DQ_OutputModeConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	DS_DQ_GPIO_CLK_ENABLE();	
	
	GPIO_InitStruct.Pin       	= DS_DQ_PIN;
  GPIO_InitStruct.Mode      	= GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull      	= GPIO_NOPULL;
  GPIO_InitStruct.Speed    	 	= GPIO_SPEED_FREQ_VERY_HIGH; 
  HAL_GPIO_Init(DS_DQ_GPIO_PORT, &GPIO_InitStruct); 	
}

static void DS18B20_Reset(void)
{
	/* ��������DQ��� */	
	DQ_OutputModeConfig();
	
	/* ����DQ����480us, ֪ͨ�豸��λ */
	SetDQ(LOW);
	delay_us(480);
	SetDQ(HIGH);
	
	/* �ȴ�60us���豸����DQ */
	delay_us(60);
	
	DQ_InputModeConfig();
	while(GetDQ());
	delay_us(500);
	
	DQ_OutputModeConfig();
	SetDQ(HIGH);
}

/*
**	return value: 1 for failed, 0 for success
*/
//static uint8_t DS18B20_Active(void)
//{
//	uint32_t pluse_cnt = 0;
//	
//	/* ��������DQ���� */
//	DQ_InputModeConfig();
//	
//	while(GetDQ() && (pluse_cnt < 480))
//	{
//		pluse_cnt++;
//	}
//	
//	if(pluse_cnt > 480)
//			return 1;
//	else
//			pluse_time = 0;
//	
//	while(!GetDQ() && (pluse_cnt < 2000))
//	{
//		pluse_cnt++;
//	}
//	
//	if(pluse_cnt > 2000)
//		return 1;
//	else
//		return 0;
//}

static uint8_t DS18B20_Read_Bit(void)
{
	uint8_t data;
	
	DQ_OutputModeConfig();
	SetDQ(LOW);
	delay_us(10);
	
	DQ_InputModeConfig();
	if(GetDQ() == HIGH)
	{
		data = 1;
	} else {
		data = 0;	
	}
	
	delay_us(45);
	
	return data;
}

uint8_t DS18B20_Read_Byte(void)
{
	uint8_t i, data = 0;
	
	for(i = 0; i < 8; i++)
	{
		data |= (DS18B20_Read_Bit() << i); 	
	}
	
	return data;
}

void DS18B20_Write_Byte(uint8_t data)
{
	uint8_t i;
	
	DQ_OutputModeConfig();
	for(i = 0; i < 8; i++)
	{
		SetDQ(LOW);
		delay_us(5);
		
		if(data & 0x1)
			SetDQ(HIGH);	
		else 
			SetDQ(LOW);
		
		delay_us(65);
		SetDQ(HIGH);
		delay_us(2);
				
		data >> 1;	
	}
}

void ReadRom(unsigned char *Read_Addr)  
{  
	unsigned char i;  
  
	DS18B20_Write_Byte(ReadROM);  
	  
	for(i=8;i>0;i--)  
	{  
		*Read_Addr = DS18B20_Read_Byte();  
		Read_Addr++;  
	}  
} 

void DS18B20_Init(unsigned char Precision,unsigned char AlarmTH,unsigned char AlarmTL)  
{  
	DisableINT();  
	DS18B20_Reset();  
	DS18B20_Write_Byte(SkipROM);   //CC
	DS18B20_Write_Byte(WriteScratchpad);	//4E  
	DS18B20_Write_Byte(AlarmTL);  //64
	DS18B20_Write_Byte(AlarmTH);  //8A
	DS18B20_Write_Byte(Precision);  //7F
	
	DS18B20_Reset();  
	DS18B20_Write_Byte(SkipROM);  //CC 
	DS18B20_Write_Byte(CopyScratchpad);	//48  
	EnableINT();  
	
	while(!GetDQ());  //�ȴ�������� ///////////  
}  

void DS18B20_StartConvert(void)  
{  
	DisableINT();  
	DS18B20_Reset();  
	DS18B20_Write_Byte(SkipROM);   
	DS18B20_Write_Byte(StartConvert);   //44
	EnableINT();  
}  

void DS18B20_Start(void)  
{  
	DQ_GPIO_Init();  
	DS18B20_Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);  
	DS18B20_StartConvert();  
}  

uint32_t DS18B20_Read(void)  
{  
	uint8_t TemperatureL,TemperatureH;  
	uint32_t  Temperature;  
	
	DisableINT();  
	DS18B20_Reset();  
	DS18B20_Write_Byte(SkipROM);   
	DS18B20_Write_Byte(ReadScratchpad);  
	TemperatureL = DS18B20_Read_Byte();  
	TemperatureH = DS18B20_Read_Byte();   
	DS18B20_Reset();  
	EnableINT();  
	
	if(TemperatureH & 0x80)  
	{  
		TemperatureH = (~TemperatureH) | 0x08;  
		TemperatureL = ~TemperatureL+1;  
		if(TemperatureL == 0)  
			TemperatureH += 1;  
	}  
	
	TemperatureH = (TemperatureH<<4) + ((TemperatureL&0xf0)>>4);  
	TemperatureL = TempX_TAB[TemperatureL&0x0f];  
	
	//bit0-bit7ΪС��λ��bit8-bit14Ϊ����λ��bit15Ϊ����λ  
	Temperature = TemperatureH;  
	Temperature = (Temperature<<8) | TemperatureL;   
	
	DS18B20_StartConvert();  
	
	return  Temperature;  
}  