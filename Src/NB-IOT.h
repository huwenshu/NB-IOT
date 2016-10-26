#ifndef __NB_IOT_H__
#define __NB_IOT_H__

//iot到服务器的定时数据
typedef struct 
{
  unsigned int cmd; //0XA0A0
  unsigned char version;
  unsigned int  cmd_len;
  unsigned char device_sn_len; //
  unsigned char device_sn[6]; // 设备编号是可变长度
  unsigned long mesg_send_num;
  unsigned long mesg_receive_num;
  unsigned long unix_time_h;
  unsigned long unix_time_l;
  unsigned int RSPR;
  unsigned int SINR;
  char cover_level;
  unsigned long repat_num;
  unsigned long loop_time;
  unsigned long reserve[20];
  
}IOT_TO_SERVER_PAKCET;

//服务器的命令
typedef struct 
{
  unsigned char cmd;
  unsigned char version;
  unsigned char len;
  unsigned long cmd_sn;
  unsigned char data[100];
}srever_cmd;

//服务器到IOT的应答
typedef struct
{
  unsigned int cmd; //0XA1A1
  unsigned char version;
  unsigned int  cmd_len;
  unsigned char device_sn_len; //
  unsigned char device_sn[6]; // 设备编号是可变长度
  unsigned long unix_time_h;
  unsigned long unix_time_l;
  unsigned char cmd_num;
  srever_cmd server_command;
} SERVER_TO_IOT_PACKET;

//IOT模块对服务器的命令应答 //0XA2A2
typedef struct
{
  unsigned int cmd;
  unsigned char version;
  unsigned int  cmd_len;
  unsigned char device_sn_len; //
  unsigned char device_sn[6]; // 设备编号是可变长度

  
  unsigned char cmd_num;
  srever_cmd server_command;
} IOT_ACK_SERVER_T;

#endif
