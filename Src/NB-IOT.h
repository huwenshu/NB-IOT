#ifndef __NB_IOT_H__
#define __NB_IOT_H__

//iot���������Ķ�ʱ����
typedef struct 
{
  unsigned int cmd; //0XA0A0
  unsigned char version;
  unsigned int  cmd_len;
  unsigned char device_sn_len; //
  unsigned char device_sn[6]; // �豸����ǿɱ䳤��
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

//������������
typedef struct 
{
  unsigned char cmd;
  unsigned char version;
  unsigned char len;
  unsigned long cmd_sn;
  unsigned char data[100];
}srever_cmd;

//��������IOT��Ӧ��
typedef struct
{
  unsigned int cmd; //0XA1A1
  unsigned char version;
  unsigned int  cmd_len;
  unsigned char device_sn_len; //
  unsigned char device_sn[6]; // �豸����ǿɱ䳤��
  unsigned long unix_time_h;
  unsigned long unix_time_l;
  unsigned char cmd_num;
  srever_cmd server_command;
} SERVER_TO_IOT_PACKET;

//IOTģ��Է�����������Ӧ�� //0XA2A2
typedef struct
{
  unsigned int cmd;
  unsigned char version;
  unsigned int  cmd_len;
  unsigned char device_sn_len; //
  unsigned char device_sn[6]; // �豸����ǿɱ䳤��

  
  unsigned char cmd_num;
  srever_cmd server_command;
} IOT_ACK_SERVER_T;

#endif
