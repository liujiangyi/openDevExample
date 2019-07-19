#ifndef _HAL_H
#define _HAL_H

#include "stdint.h"
#pragma once

void HAL_Init(void);
void HAL_IncTick(void);
void HAL_SuspendTick(void);
void HAL_ResumeTick(void);
uint32_t HAL_GetTick(void);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
#define clrwdt()

typedef struct
{
	uint16_t start;
	uint32_t value;
}MsTicketSoftTimer;

uint32_t _get_msTicket_timer_val(uint32_t pre_timer_val);

#define	    def_sw_tm(tm)	MsTicketSoftTimer tm;
#define	    declare_sw_tm(tm)	extern MsTicketSoftTimer tm;
#define	    tm_val(timer)	     _get_msTicket_timer_val(timer.value)

#define  start_tm(timer)	 do  \
{					     \
    timer.start = 1;	     \
    timer.value = HAL_GetTick();     \
}while(0)

#define  init_sw_tm(timer)	 do  \
{					     \
    timer.start = 0;	     \
    timer.value = 0;	    \
}while(0)

#define  is_tm_on(timer)	 timer.start
#define  stop_tm(timer)	 timer.start = 0

#endif // _HAL_H

