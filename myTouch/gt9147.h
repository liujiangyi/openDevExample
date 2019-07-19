#ifndef _GT9147_H_
#define _GT9147_H_

#include <stdint.h>

 
//I2C读写命令	
#define GT_CMD_WR 		0x28     	//写命令
#define GT_CMD_RD 		0x29		//读命令
  
//GT9147 部分寄存器定义 
#define GT_CTRL_REG 	0x8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0x8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0x80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0x8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG		0x814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0x8150  	//第一个触摸点数据地址
#define GT_TP2_REG 		0x8158		//第二个触摸点数据地址
#define GT_TP3_REG 		0x8160		//第三个触摸点数据地址
#define GT_TP4_REG 		0x8168		//第四个触摸点数据地址
#define GT_TP5_REG 		0x8170		//第五个触摸点数据地址  
 
#define GT_RESET_GPIO_PORT	GPIOF
#define GT_RESET_GPIO_PIN	GPIO_Pin_11

#define GT_INT_GPIO_PORT	GPIOF
#define GT_INT_GPIO_PIN		GPIO_Pin_10
#define CT_INT_ON()	BSP_GPIO_RD( GT_INT_GPIO_PORT, GT_INT_GPIO_PIN)


#define TP_PRES_DOWN 0x80  		//触屏被按下	  
#define TP_CATH_PRES 0x40  		//有按键按下了 
#define CT_MAX_TOUCH  5    		//电容屏支持的点数,固定为5点
 
uint8_t GT9147_Send_Cfg(uint8_t mode);
uint8_t GT9147_WR_Reg(uint16_t reg,uint8_t *buf,uint8_t len);
void GT9147_RD_Reg(uint16_t reg,uint8_t *buf,uint8_t len); 
uint8_t GT9147_Init(void);
uint8_t GT9147_Scan(uint8_t mode); 
void gt9147_test_scan(void);

#endif
