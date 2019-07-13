#include "bsp.h"
#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"


void bsp_enable_gt9147_exit_isr(void)
{
    EXTI_InitTypeDef exti_init;
    NVIC_InitTypeDef   NVIC_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(G9147_INT_EXIT_PORT_SRC, G9147_INT_EXIT_PIN_SRC);

    /*
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
    */

    exti_init.EXTI_Line = G9147_INT_EXTI_LINE;
    exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init.EXTI_Trigger = G9147_INT_EXTI_TRIGGER;
    exti_init.EXTI_LineCmd = ENABLE;
    EXTI_Init ( &exti_init );

    NVIC_InitStructure.NVIC_IRQChannel = G9147_INT_EXTI_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void bsp_disable_gt9147_exit_isr(void)
{
    EXTI_InitTypeDef exti_init;
    GPIO_EXTILineConfig(G9147_INT_EXIT_PORT_SRC, G9147_INT_EXIT_PIN_SRC);

    exti_init.EXTI_Line = G9147_INT_EXTI_LINE;
    exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
    exti_init.EXTI_Trigger = G9147_INT_EXTI_TRIGGER;
    exti_init.EXTI_LineCmd = DISABLE;

    EXTI_Init ( &exti_init );
}
