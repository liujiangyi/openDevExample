#ifndef _BSP_GPIO_COMMON_H_
#define _BSP_GPIO_COMMON_H_

#include "stm32f10x.h"

#define BSP_GPIO_RD(GPIOx, GPIO_Pin)	    GPIO_ReadInputDataBit(GPIOx,GPIO_Pin)
#define BSP_GPIO_SET_BIT(GPIOx,GPIO_Pin)    GPIO_SetBits(GPIOx,GPIO_Pin)
#define BSP_GPIO_RESET_BIT(GPIOx,GPIO_Pin)    GPIO_ResetBits(GPIOx,GPIO_Pin)
void bsp_gpio_init ( GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIOMode_TypeDef GPIO_Mode , GPIOSpeed_TypeDef GPIO_Speed );

#endif
