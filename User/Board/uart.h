#ifndef _UART_H_
#define _UART_H_

#include "lsd_bsp.h"

#define RXBUFFERSIZE  256

/* Function definition */
void UART1_Init(void);
void UART1_SendByte(uint8_t data);
void UART1_SendString(char *str);

void UART3_Init(void);
void UART3_SendByte(uint8_t data);
void UART3_SendString(char *str);

void UART4_Init(void);
void UART4_SendByte(uint8_t data);
void UART4_SendString(char *str);

void Debug_Init(void);
void Debug_Printf(const char *fmt, ...);
#endif