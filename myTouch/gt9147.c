#include "gt9147.h"
#include "hal.h"
#include "gt9147_iic.h"
#include "bsp_gpio_common.h"

//GT9147配置参数表
//第一个字节为版本号(0x60),必须保证新的版本号大于等于GT9147内部
//flash原有版本号,才会更新配置.
const uint8_t GT9147_CFG_TBL[] =
{
    0x60, 0xE0, 0x01, 0x20, 0x03, 0x05, 0x35, 0x00, 0x02, 0x08,
    0x1E, 0x08, 0x50, 0x3C, 0x0F, 0x05, 0x00, 0x00, 0xFF, 0x67,
    0x50, 0x00, 0x00, 0x18, 0x1A, 0x1E, 0x14, 0x89, 0x28, 0x0A,
    0x30, 0x2E, 0xBB, 0x0A, 0x03, 0x00, 0x00, 0x02, 0x33, 0x1D,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00,
    0x2A, 0x1C, 0x5A, 0x94, 0xC5, 0x02, 0x07, 0x00, 0x00, 0x00,
    0xB5, 0x1F, 0x00, 0x90, 0x28, 0x00, 0x77, 0x32, 0x00, 0x62,
    0x3F, 0x00, 0x52, 0x50, 0x00, 0x52, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F,
    0x0F, 0x03, 0x06, 0x10, 0x42, 0xF8, 0x0F, 0x14, 0x00, 0x00,
    0x00, 0x00, 0x1A, 0x18, 0x16, 0x14, 0x12, 0x10, 0x0E, 0x0C,
    0x0A, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x29, 0x28, 0x24, 0x22, 0x20, 0x1F, 0x1E, 0x1D,
    0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x05, 0x04, 0x02, 0x00, 0xFF,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF,
};

const uint16_t GT9147_TPX_TBL[5] =
{
    GT_TP1_REG,
    GT_TP2_REG,
    GT_TP3_REG,
    GT_TP4_REG,
    GT_TP5_REG
};

//发送GT9147配置参数
//mode:0,参数不保存到flash
//     1,参数保存到flash
uint8_t GT9147_Send_Cfg ( uint8_t mode )
{
    uint8_t buf[2];
    uint8_t i = 0;
    buf[0] = 0;
    buf[1] = mode;	//是否写入到GT9147 FLASH?  即是否掉电保存

    for ( i = 0; i < sizeof ( GT9147_CFG_TBL ); i++ )
	buf[0] += GT9147_CFG_TBL[i]; //计算校验和

    buf[0] = ( ~buf[0] ) + 1;

    GT9147_WR_Reg ( GT_CFGS_REG, ( uint8_t* ) GT9147_CFG_TBL, sizeof ( GT9147_CFG_TBL ) ); //发送寄存器配置
    GT9147_WR_Reg ( GT_CHECK_REG, buf, 2 ); //写入校验和,和配置更新标记
    return 0;
}
//向GT9147写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
uint8_t GT9147_WR_Reg ( uint16_t reg, uint8_t *buf, uint8_t len )
{
    uint8_t i;
    uint8_t ret = 0;
    CT_IIC_Start();
    CT_IIC_Send_Byte ( GT_CMD_WR );   	//发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte ( reg >> 8 );   	//发送高8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte ( reg & 0xFF );   	//发送低8位地址
    CT_IIC_Wait_Ack();
    for ( i = 0; i < len; i++ )
    {
	CT_IIC_Send_Byte ( buf[i] );  	//发数据
	ret = CT_IIC_Wait_Ack();
	if ( ret ) break;
    }
    CT_IIC_Stop();					//产生一个停止条件
    return ret;
}
//从GT9147读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度
void GT9147_RD_Reg ( uint16_t reg, uint8_t *buf, uint8_t len )
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte ( GT_CMD_WR ); //发送写命令
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte ( reg >> 8 );   	//发送高8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte ( reg & 0xFF );   	//发送低8位地址
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte ( GT_CMD_RD ); //发送读命令
    CT_IIC_Wait_Ack();
    for ( i = 0; i < len; i++ )
    {
	buf[i] = CT_IIC_Read_Byte ( i == ( len - 1 ) ? 0 : 1 ); //发数据
    }
    CT_IIC_Stop();//产生一个停止条件
}

//初始化GT9147触摸屏
//返回值:0,初始化成功;1,初始化失败
uint8_t GT9147_Init ( void )
{
    uint8_t temp[5];
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOF, ENABLE );	 //使能PF端口时钟

    bsp_gpio_init ( GT_RESET_GPIO_PORT, GT_RESET_GPIO_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz );
    BSP_GPIO_SET_BIT ( GT_RESET_GPIO_PORT, GT_RESET_GPIO_PIN );


    bsp_gpio_init ( GT_INT_GPIO_PORT, GT_INT_GPIO_PIN, GPIO_Mode_IPU, GPIO_Speed_50MHz );
    BSP_GPIO_SET_BIT ( GT_INT_GPIO_PORT, GT_INT_GPIO_PIN );


    CT_IIC_Init();      	//初始化电容屏的I2C总线
    BSP_GPIO_RESET_BIT ( GT_RESET_GPIO_PORT, GT_RESET_GPIO_PIN );
    delay_ms ( 10 );
    BSP_GPIO_SET_BIT ( GT_RESET_GPIO_PORT, GT_RESET_GPIO_PIN );
    delay_ms ( 10 );

    bsp_gpio_init ( GT_INT_GPIO_PORT, GT_INT_GPIO_PIN, GPIO_Mode_IPD, GPIO_Speed_50MHz );
    BSP_GPIO_RESET_BIT ( GT_INT_GPIO_PORT, GT_INT_GPIO_PIN );


    delay_ms ( 100 );
    GT9147_RD_Reg ( GT_PID_REG, temp, 4 );	//读取产品ID
    temp[4] = 0;
    printf ( "CTP ID:%s\r\n", temp );		//打印ID
    if ( strcmp ( ( char* ) temp, "9147" ) == 0 )	//ID==9147
    {
	temp[0] = 0x02;
	GT9147_WR_Reg ( GT_CTRL_REG, temp, 1 ); //软复位GT9147
	GT9147_RD_Reg ( GT_CFGS_REG, temp, 1 ); //读取GT_CFGS_REG寄存器

	if ( temp[0] < 0x60 ) //默认版本比较低,需要更新flash配置
	{
	    printf ( "Default Ver:%d\r\n", temp[0] );
	    GT9147_Send_Cfg ( 1 ); //更新并保存配置
	}
	delay_ms ( 10 );
	temp[0] = 0x00;
	GT9147_WR_Reg ( GT_CTRL_REG, temp, 1 );	//结束复位
	return 0;
    }

    bsp_gpio_init ( GT_INT_GPIO_PORT, GT_INT_GPIO_PIN, GPIO_Mode_IPU, GPIO_Speed_50MHz );
    return 1;
}
//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸

#if GT9147_SCAN_ON
uint8_t GT9147_Scan ( uint8_t mode )
{
    uint8_t buf[4];
    uint8_t i = 0;
    uint8_t res = 0;
    uint8_t temp;
    uint8_t tempsta;
    static uint8_t t = 0; //控制查询间隔,从而降低CPU占用率
    t++;
    if ( PEN == 0 ) //有按键按下
    {
	GT9147_RD_Reg ( GT_GSTID_REG, &mode, 1 );	//读取触摸点的状态
	if ( mode & 0x80 && ( ( mode & 0xF ) < 6 ) )
	{
	    temp = 0;
	    GT9147_WR_Reg ( GT_GSTID_REG, &temp, 1 ); //清标志
	}
	if ( ( mode & 0xF ) && ( ( mode & 0xF ) < 6 ) )
	{
	    temp = 0xFF << ( mode & 0xF );		//将点的个数转换为1的位数,匹配tp_dev.sta定义
	    tempsta = tp_dev.sta;			//保存当前的tp_dev.sta值
	    tp_dev.sta = ( ~temp ) | TP_PRES_DOWN | TP_CATH_PRES;
	    tp_dev.x[4] = tp_dev.x[0];	//保存触点0的数据
	    tp_dev.y[4] = tp_dev.y[0];
	    for ( i = 0; i < 5; i++ )
	    {
		if ( tp_dev.sta & ( 1 << i ) )	//触摸有效?
		{
		    GT9147_RD_Reg ( GT9147_TPX_TBL[i], buf, 4 );	//读取XY坐标值
		    if ( tp_dev.touchtype & 0x01 ) //横屏
		    {
			tp_dev.y[i] = ( ( uint16_t ) buf[1] << 8 ) + buf[0];
			tp_dev.x[i] = 800 - ( ( ( uint16_t ) buf[3] << 8 ) + buf[2] );
		    }
		    else
		    {
			tp_dev.x[i] = ( ( uint16_t ) buf[1] << 8 ) + buf[0];
			tp_dev.y[i] = ( ( uint16_t ) buf[3] << 8 ) + buf[2];
		    }
		    //printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
		}
	    }
	    res = 1;
	    if ( tp_dev.x[0] > lcddev.width || tp_dev.y[0] > lcddev.height ) //非法数据(坐标超出了)
	    {
		if ( ( mode & 0xF ) > 1 )		//有其他点有数据,则复第二个触点的数据到第一个触点.
		{
		    tp_dev.x[0] = tp_dev.x[1];
		    tp_dev.y[0] = tp_dev.y[1];
		    t = 0;				//触发一次,则会最少连续监测10次,从而提高命中率
		}
		else					//非法数据,则忽略此次数据(还原原来的)
		{
		    tp_dev.x[0] = tp_dev.x[4];
		    tp_dev.y[0] = tp_dev.y[4];
		    mode = 0x80;
		    tp_dev.sta = tempsta;	//恢复tp_dev.sta
		}
	    }
	    else t = 0;					//触发一次,则会最少连续监测10次,从而提高命中率
	}
    }
    if ( ( mode & 0x8F ) == 0x80 ) //无触摸点按下
    {
	if ( tp_dev.sta & TP_PRES_DOWN )	//之前是被按下的
	{
	    tp_dev.sta &= ~ ( 1 << 7 );	//标记按键松开
	}
	else						//之前就没有被按下
	{
	    tp_dev.x[0] = 0xffff;
	    tp_dev.y[0] = 0xffff;
	    tp_dev.sta &= 0xE0;	//清除点有效标记
	}
    }
    if ( t > 240 ) t = 10; //重新从10开始计数
    return res;
}
#endif


void gt9147_test_scan ( void )
{
    uint8_t mode = 0x00;
    uint8_t temp = 0;
    uint8_t point_num = 0;
    uint8_t buf[4] = 0x00;
    uint16_t curr_x = 0;
    uint16_t curr_y = 0;

    uint8_t  i = 0;

    if ( CT_INT_ON() )
    {
	GT9147_RD_Reg ( GT_GSTID_REG, &mode, 1 );	//读取触摸点的状态
	point_num = mode & 0x0F;

	if ( ( mode & 0x80 ) && ( point_num < 6 ) )
	{
	    temp = 0;
	    GT9147_WR_Reg ( GT_GSTID_REG, &temp, 1 ); //清标志
	}

	if ( point_num > 0 && point_num < 6 )
	{
	    /* printf("mode :%x\n",mode); */
	    for ( i = 0 ; i < point_num ; i++ )
	    {
		GT9147_RD_Reg ( GT9147_TPX_TBL[i], buf, 4 );	//读取XY坐标值
		curr_x = BUILD_UINT16 ( buf[0], buf[1] );
		curr_y = BUILD_UINT16 ( buf[2], buf[3] );

		printf ( "%d,x=%03d,y=%03d,num=%d\n", i,curr_x, curr_y, point_num );
	    }
	}
    }
}



