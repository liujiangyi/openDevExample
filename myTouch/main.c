#include "stm32f10x.h"
#include  "led.h"
#include "hal.h"
#include "bsp.h"
#include "usart.h"

int main ( void )
{
    HAL_Init();
    uart_init(2000000);
    LED_Init();
    bsp_gpio_init(GPIOA,GPIO_Pin_7,GPIO_Mode_Out_PP,GPIO_Speed_50MHz);

    delay_ms(100);
    printf("power on\n");
    while ( 1 )
    {
        GPIO_SetBits ( GPIOB, GPIO_Pin_5 );
        GPIO_SetBits ( GPIOE, GPIO_Pin_5 );
	BSP_GPIO_SET_BIT(GPIOA,GPIO_Pin_7);

        delay_ms ( 300 );


	BSP_GPIO_RESET_BIT(GPIOA,GPIO_Pin_7);
        GPIO_ResetBits ( GPIOB, GPIO_Pin_5 );
        GPIO_ResetBits ( GPIOE, GPIO_Pin_5 );
        delay_ms ( 300 );
    }
}
