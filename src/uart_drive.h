#ifndef UART_DRIVE
#define UART_DRIVE

#include "stm32f1xx.h"
#include "gpio_drive.h"

void UART_init(unsigned short usart, unsigned long BR);
unsigned long USART_BRR_Custom(unsigned short usart, unsigned long BR);
void UART_ISR(USART_TypeDef *uart, unsigned short bridge, unsigned short *signal, unsigned short *counter, char str[]);

void transmitChar(char ch, USART_TypeDef *usart);
void transmitString(char str[], USART_TypeDef *usart);
char receiveChar(USART_TypeDef *usart);
void receiveString(char out[], USART_TypeDef *usart);

#endif