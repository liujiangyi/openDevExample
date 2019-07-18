#ifndef _GT9147_IIC_H_
#define _GT9147_IIC_H_
#include <stdint.h>

/*
//IO方向设置
#define CT_SDA_IN()  {GPIOF->CRH&=0XFFFFFF0F;GPIOF->CRH|=8<<4;}
#define CT_SDA_OUT() {GPIOF->CRH&=0XFFFFFF0F;GPIOF->CRH|=3<<4;}

//IO操作函数	 
#define CT_IIC_SCL    PBout(1) 			//SCL     
#define CT_IIC_SDA    PFout(9) 			//SDA	 
#define CT_READ_SDA   PFin(9)  			//输入SDA 
*/

#define CT_IIC_SCK_PORT	    GPIOB
#define CT_IIC_SCK_PIN	    GPIO_Pin_1

#define CT_IIC_SDA_PORT	    GPIOF
#define CT_IIC_SDA_PIN	    GPIO_Pin_9

#define CT_IIC_SET_SDA()     BSP_GPIO_SET_BIT(CT_IIC_SDA_PORT,CT_IIC_SDA_PIN)
#define CT_IIC_RESET_SDA()   BSP_GPIO_RESET_BIT(CT_IIC_SDA_PORT,CT_IIC_SDA_PIN)

#define CT_IIC_SET_SCL()     BSP_GPIO_SET_BIT(CT_IIC_SCK_PORT,CT_IIC_SCK_PIN)
#define CT_IIC_RESET_SCL()   BSP_GPIO_RESET_BIT(CT_IIC_SCK_PORT,CT_IIC_SCK_PIN)

#define CT_IIC_SDA_OUT()     bsp_gpio_init ( CT_IIC_SDA_PORT, CT_IIC_SDA_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz )
#define CT_IIC_SDA_IN()      bsp_gpio_init ( CT_IIC_SDA_PORT, CT_IIC_SDA_PIN, GPIO_Mode_IPU, GPIO_Speed_50MHz )

#define CT_READ_SDA()	    BSP_GPIO_RD(CT_IIC_SDA_PORT,CT_IIC_SDA_PIN)

#define CT_Delay()	delay_us(5)

//IIC所有操作函数
void CT_IIC_Init(void);                	//初始化IIC的IO口				 
void CT_IIC_Start(void);				//发送IIC开始信号
void CT_IIC_Stop(void);	  				//发送IIC停止信号
void CT_IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t CT_IIC_Read_Byte(unsigned char ack);	//IIC读取一个字节
uint8_t CT_IIC_Wait_Ack(void); 				//IIC等待ACK信号
void CT_IIC_Ack(void);					//IIC发送ACK信号
void CT_IIC_NAck(void);					//IIC不发送ACK信号


#endif
