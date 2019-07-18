#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收


#define DEBUG_USART		USART1
#define DEBUG_USART_PORT	GPIOA

#define DEBUG_USART_TX_PORT		DEBUG_USART_PORT
#define DEBUG_USART_TX_PIN		GPIO_Pin_9

#define DEBUG_USART_RX_PORT		DEBUG_USART_PORT
#define DEBUG_USART_RX_PIN		GPIO_Pin_10

#define DEBUG_USART_PORT_CLK_ENABLE()	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOA , ENABLE )
#define	DEBUG_USART_CLK_ENABLE()	RCC_APB2PeriphClockCmd ( RCC_APB2Periph_USART1 , ENABLE )

void uart_init ( u32 bound );

#endif


