#include "gt9147_iic.h"
#include "hal.h"

//电容触摸芯片IIC接口初始化
void CT_IIC_Init ( void )
{
    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOF, ENABLE );	 //使能PB,PF端口时钟

    //sck fb1
    bsp_gpio_init ( CT_IIC_SCK_PORT, CT_IIC_SCK_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz );
    BSP_GPIO_SET_BIT ( CT_IIC_SCK_PORT, CT_IIC_SCK_PIN );

    //sda pf9
    bsp_gpio_init ( CT_IIC_SDA_PORT, CT_IIC_SDA_PIN, GPIO_Mode_Out_PP, GPIO_Speed_50MHz );
    BSP_GPIO_SET_BIT ( CT_IIC_SDA_PORT, CT_IIC_SDA_PIN );
}

//产生IIC起始信号
void CT_IIC_Start ( void )
{
    CT_IIC_SDA_OUT();     //sda线输出

    CT_IIC_SET_SDA();
    CT_IIC_SET_SCL();

    delay_us ( 30 );

    //START:when CLK is high,DATA change form high to low
    CT_IIC_RESET_SDA();
    CT_Delay();

    //钳住I2C总线，准备发送或接收数据
    CT_IIC_RESET_SCL();
}

//产生IIC停止信号
void CT_IIC_Stop ( void )
{
    CT_IIC_SDA_OUT();//sda线输出
    CT_IIC_SET_SCL();

    delay_us ( 30 );

    //STOP:when CLK is high DATA change form low to high
    CT_IIC_RESET_SDA();
    CT_Delay();
    //发送I2C总线结束信号
    CT_IIC_SET_SDA();
}

//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t CT_IIC_Wait_Ack ( void )
{
    uint8_t ucErrTime = 0;

    CT_IIC_SDA_IN();      //SDA设置为输入
    CT_IIC_SET_SDA();
    CT_IIC_SET_SCL();

    CT_Delay();
    while ( CT_READ_SDA() )
    {
	ucErrTime++;
	if ( ucErrTime > 250 )
	{
	    CT_IIC_Stop();
	    return 1;
	}
	CT_Delay();
    }

    //时钟输出0
    CT_IIC_RESET_SCL();
    return 0;
}
//产生ACK应答
void CT_IIC_Ack ( void )
{
    CT_IIC_RESET_SCL();
    CT_IIC_SDA_OUT();
    CT_Delay();
    CT_IIC_RESET_SDA();
    CT_Delay();
    CT_IIC_SET_SCL();
    CT_Delay();
    CT_IIC_RESET_SCL();
}

//不产生ACK应答
void CT_IIC_NAck ( void )
{
    CT_IIC_RESET_SCL();
    CT_IIC_SDA_OUT();
    CT_Delay();
    CT_IIC_SET_SDA();
    CT_Delay();
    CT_IIC_SET_SCL();
    CT_Delay();
    CT_IIC_RESET_SCL();
}

//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void CT_IIC_Send_Byte ( uint8_t txd )
{
    uint8_t t;
    CT_IIC_SDA_OUT();
    //拉低时钟开始数据传输
    CT_IIC_RESET_SCL();
    CT_Delay();
    for ( t = 0; t < 8; t++ )
    {
	if ( ( txd & 0x80 ) >> 7)
	    CT_IIC_SET_SDA();
	else
	    CT_IIC_RESET_SDA();
	txd <<= 1;
	CT_IIC_SET_SCL();
	CT_Delay();
	CT_IIC_RESET_SCL();
	CT_Delay();
    }
}

//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
uint8_t CT_IIC_Read_Byte ( unsigned char ack )
{
    uint8_t i, receive = 0;
    CT_IIC_SDA_IN();//SDA设置为输入
    delay_us ( 30 );
    for ( i = 0; i < 8; i++ )
    {
	CT_IIC_RESET_SCL();
	CT_Delay();
	CT_IIC_SET_SCL();
	receive <<= 1;
	if ( CT_READ_SDA() ) receive++;
    }
    if ( !ack) 
	CT_IIC_NAck(); //发送nACK
    else
	CT_IIC_Ack(); // send ack
    return receive;
}
