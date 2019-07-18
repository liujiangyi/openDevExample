#include "bsp.h"

void bsp_gpio_init ( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode , GPIOSpeed_TypeDef GPIO_Speed )
{
    GPIO_InitTypeDef io_init;

    io_init.GPIO_Pin = GPIO_Pin;
    io_init.GPIO_Speed = GPIO_Speed;
    io_init.GPIO_Mode = GPIO_Mode;

    GPIO_Init ( GPIOx, &io_init );
}
