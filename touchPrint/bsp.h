#ifndef _BSP_H_
#define _BSP_H_

#define G9147_INT_EXIT_PORT_SRC	    GPIO_PortSourceGPIOF
#define G9147_INT_EXIT_PIN_SRC	    GPIO_PinSource10
#define G9147_INT_EXTI_LINE         EXTI_Line10
#define G9147_INT_EXTI_TRIGGER      EXTI_Trigger_Falling
#define G9147_INT_EXTI_IRQ          EXTI15_10_IRQn

void bsp_enable_gt9147_exit_isr(void);
void bsp_disable_gt9147_exit_isr(void);
#endif
