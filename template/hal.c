#include "stm32f10x.h"
#include "hal.h"
#define  TICK_INT_PRIORITY            ((uint32_t)0)    /*!< tick interrupt priority (lowest by default)  */

__IO uint32_t g_uwTick;
static uint32_t g_us_multiplier;

void HAL_Init ( void )
{
    //uint32_t priority = 0x00;
    g_uwTick = 0;
    g_us_multiplier = SystemCoreClock / 1000000;

    SysTick_Config ( SystemCoreClock / 1000 );

    /* SysTick_IRQn interrupt configuration */
    //priority = NVIC_EncodePriority(4, 0, 0);
    //NVIC_SetPriority(SysTick_IRQn, priority);
}

void HAL_IncTick ( void )
{
    g_uwTick++;
}

void HAL_SuspendTick ( void )
{
    /* Disable SysTick Interrupt */
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

void HAL_ResumeTick ( void )
{
    /* Enable SysTick Interrupt */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

uint32_t HAL_GetTick ( void )
{
    return g_uwTick;
}

void delay_ms ( uint32_t ms )
{
    delay_us ( ms * 1000 - 1 );
}

void delay_us ( uint32_t us )
{
    register uint32_t r0 __asm ( "r0" );
    uint32_t micros = 0;

    if ( us < 2 ) return;

    micros = ( us - 1 ) * g_us_multiplier / 12; // 4 cycles per loop

    while ( micros-- > 0 );
}

uint32_t _get_msTicket_timer_val ( uint32_t pre_timer_val )
{
    uint32_t curr_timer_val = HAL_GetTick();
    uint32_t ret_timer_val = 0;

    if ( curr_timer_val >= pre_timer_val )
    {
	ret_timer_val  = curr_timer_val - pre_timer_val;
    }
    else
    {
	ret_timer_val = 0xFFFFFFFF - pre_timer_val + curr_timer_val;
    }

    return ret_timer_val;
}
