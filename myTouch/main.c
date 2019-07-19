#include "stm32f10x.h"
#include  "led.h"
#include "hal.h"
#include "bsp_gpio_common.h"
#include "usart.h"
#include "gt9147.h"

void led_process(void)
{
   static MsTicketSoftTimer tm_intv; 
   static uint8_t i = 0;
   if ( !is_tm_on(tm_intv) || tm_val(tm_intv) >= 500)
   {
        if ( i == 0)
	{
	    BSP_GPIO_RESET_BIT(GPIOB,GPIO_Pin_5);
	    i = 1;
	}
	else
	{
	    BSP_GPIO_SET_BIT(GPIOB,GPIO_Pin_5);
	    i = 0;
	}
	start_tm(tm_intv);
   }
}

int main ( void )
{
    HAL_Init();
    uart_init(2000000);
    LED_Init();

    delay_ms(100);
    printf("power on\n");
    GT9147_Init();

    while ( 1 )
    {
	led_process();
	gt9147_test_scan();
    }
}
