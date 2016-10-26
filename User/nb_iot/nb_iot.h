#ifndef _NB_IOT_H_
#define _NB_IOT_H_

#include "stm32l4xx_hal.h"

#define HEART_BIT		0x1
#define TIMER_BIT		0x2
#define TEST_BIT		0x4

#define 	START_BIT			0xFFAA

#define 	CMD_HEART			0xA0A0
#define 	CMD_REPORT		0xD0D0
#define 	CMD_REACK			0xD1D1				
#define 	CMD_CONFIRM		0xD2D2

#define 	SUB_CMD_TRIGGER	0xC0
#define 	SUB_CMD_CONFIG	0xC1

#define 	CMD_VER				0x00
						
#define 	DEV_LEN				0x08

//
//IOM IP地址：172.16.0.36
//APN:parking.donas.nb-iot.com
//IMEI：121530006540210


#define         AT_CGDCONT      "AT+CGDCONT=1,\"IP\",\"parking.donas.nb-iot.com\"\r\n"
//#define         AT_NCDP         "AT+NCDP=172.16.0.36\r\n" 
#define         AT_NCDP         "AT+NCDP=\"221530006544230\",172.16.0.36\r\n"
//#define         AT_NTSETID      "AT+NTSETID=1,121530006540210\r\n"  

//#define         AT_CGDCONT      "AT+CGDCONT=1,\"IP\",\"PCCW\"\r\n" 
//#define         AT_NCDP         "AT+NCDP=185.4.11.89\r\n"
//#define         AT_NTSETID      "AT+NTSETID=1,111112222233333\r\n" 
 
#define         AT_NSOCR        "AT+NSCOR=17,8899\r\n"
#define         AT_NSOST        "AT+NSOST=1,\"192.158.5.1\",8080,19"
#define         AT_NSOCL        "AT+NSOCL=1"
#define         AT_NSORF        "AT+NSORF=1,30"
#define         AT_OK           "OK"
#define         AT_CFUN         "AT+CFUN=1\r\n"   
 
#define         AT_CGATT        "AT+CGATT=1\r\n"

#define         AT_CSCON2        "AT+CSCON?\r\n"
#define         AT_CSCON1        "AT+CSCON=1\r\n"
#define         AT_CSCON_RT      "+CSCON:1"
#define         AT_CSCON_RT2     "+CSCON:1,2"

   
#define         AT_CGSN         "AT+CGSN=20160924142401\r\n"

#define         AT_CGPADDR      "AT+CGPADDR=1\r\n"
#define         AT_CGP_RT       "+CGPADDR:1"
#define         AT_NNMI         "AT+NNMI=1\r\n"
#define         AT_NMGS         "AT+NMGS=45,A0A0000028041122334400000000000000000102030405060708FFB00014000000000100000025340000000000\r\n"
#define         AT_NMG_RT       "+NNMI:26,A1A1"
#define         AT_NMG_RLEN      strlen("+LOG[D]IOM->UE: GET request\r\n\r\n+LOG[I]IOM->UE: GET request direct upload len=45\r\n\r\nOK\r\n\r\n+LOG[D]IOM->UE: POST request\r\n+NNMI:26,A1A1000015033131310000000000000001C00000000001002000")
  
#define         AT_NRB          "AT+NRB\r\n"
#define         HWEI_connect    "OK" 


/**
***	systick packet, 0xA0A0
***	| start | cmd | ver | cmd_len | dev_len | dev_addr | timestamp | recount | crc |	
***	|   2   |  2  |  1  |    2    |    1    |   V     |     8     |    2    |  1  |
**/
#pragma pack(1)
typedef struct {
	uint16_t 	start;	
	uint16_t 	format;	
	uint8_t 	ver;
	uint16_t	msg_len;
	uint8_t		dev_len;
	uint8_t		dev_addr[DEV_LEN];
	uint32_t	stamp_high;
	uint32_t	stamp_low;
	uint16_t	rect;	//重传成功次数
	uint8_t 	crc;
} HEART_PACKET_T;

/**
***	payload packet
***	| type | temper | state |
***	|   1  |    4   |   1   |
**/

typedef struct {
	uint8_t		type;
	int32_t	temper;	
	uint8_t		state;
} PAYLOAD_PACKET_T;

/**
***	data report packet, 0xD0D0
***	| start | cmd | ver | cmd_len | dev_len | dev_addr| sn | s_count | timestamp | rspr | sinr | level| recount | crc |	
***	|   2   |  2  |  1  |    2    |    1    |   V     |  4 |    4    |     8     |   2  |   2  |   1  |    2    |  1  |
**/
typedef struct {
	uint16_t 	start;
	uint16_t 	format;
	uint8_t 	ver;
	uint16_t	msg_len;
	uint8_t		dev_len;
	uint8_t		dev_addr[DEV_LEN];
	uint32_t	msg_ct;
	uint32_t 	ack_ct;	
	uint32_t	stamp_high;
	uint32_t	stamp_low;
	int16_t		rspr;
	int16_t		sinr;
	uint8_t		level;
	uint16_t	rect;
	uint16_t	payload_len;
	PAYLOAD_PACKET_T	payload;
	uint8_t		crc;
} TIMER_PACKET_T;


/**
***	reply report packet, 0xD1D1
***	| start | cmd | ver | cmd_len | dev_len | dev_addr | timestmap | cmd | crc | 
***	|   2   |  2  |  1  |    2    |    1    |   v      |     8     |  v  |  1  | 
**/

typedef struct {
	uint16_t 	start;
	uint16_t 	format;
	uint8_t 	ver;	
	uint16_t	msg_len;
	uint8_t		dev_len;
	uint8_t		dev_addr[DEV_LEN];	
	uint32_t	stamp_high;
	uint32_t	stamp_low;
	uint8_t		crc;
} IOT_TO_DEV_PACKET_T;

/**
***	terminal confirm packet, 0xD2D2
***
***
**/

typedef struct {
	uint8_t		cmd;	
	uint32_t	num;
	uint8_t		flag;
	uint16_t	len;
} CONFIRM_CMD_PACKET_T;

typedef struct {
	uint16_t 	start;
	uint16_t	format;
	uint8_t 	ver;
	uint16_t	msg_len;	
	uint8_t		dev_len;
	uint8_t		dev_addr[DEV_LEN];
	uint8_t		arr[0];
} CONFIRM_PACKET_T;

typedef struct {
	uint8_t		cmd;	//0xC0
	uint8_t		ver;	
	uint32_t	num;
	uint16_t	len;
	uint8_t		flag;
} TRIGGER_CMD0_T;

typedef struct {
	uint8_t		cmd;	//0xC1
	uint8_t		ver;
	uint16_t	len;
	uint8_t		mode;
	uint8_t		t1;	
	uint32_t	t2;	//test period
	uint16_t	reserve;	
	uint8_t		recount;
} TRIGGER_CMD1_T;

typedef struct
{
  uint32_t 	socket;
 	uint32_t 	ip_addr;
  uint32_t 	port;
  uint32_t	len;
  uint8_t 	udp_data[100];
}UDP_RECV_PACKET;

#pragma pack()

/* function list */
uint8_t calc_crc(uint8_t *packet, uint32_t len);
uint8_t charsToHex(char *asc);
void  hexToAscs(uint8_t hex,uint8_t *ascs);
uint16_t HexsToAscs(uint8_t *hexs,char * ascs,uint16_t length, uint8_t litt);
uint16_t ascsToHexs(uint8_t *ascs,char * hexs,uint16_t length);
int at_get_words(char chop,char *srcStr, char **word, int size);
void convert_data(UDP_RECV_PACKET *udp_recv_data, char *uart_data);

void dispose_multipacket(UDP_RECV_PACKET *udp_recv_data);
void alloc_confirm_packet(void);
void fill_confirm_packet(CONFIRM_PACKET_T *packet, uint8_t *trigger_packet);
uint8_t fill_heart_packet(HEART_PACKET_T *heart_packet, uint32_t stamp, uint16_t rect);
uint8_t fill_timer_packet(TIMER_PACKET_T *packet, uint32_t stamp, uint32_t msg_ct, uint32_t ack_ct, uint16_t rect);

void bs_connect(void);
void uart_coap_thread(void const *param);
void uart_udp_thread(void const *param);
void send_message_thread(void const *param);

#endif