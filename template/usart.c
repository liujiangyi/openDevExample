#include "sys.h"
#include "usart.h"

#if 1
#pragma import(__use_no_semihosting)
//标准库需要的支持函数
struct __FILE
{
    int handle;

};

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式
_sys_exit ( int x )
{
    x = x;
}

//重定义fputc函数
int fputc ( int ch, FILE *f )
{
    while ( ( DEBUG_USART->SR & 0X40 ) == 0 )
	; //循环发送,直到发送完毕
    DEBUG_USART->DR = ( uint8_t ) ch;
    return ch;
}
#endif

void uart_init ( uint32_t bound )
{
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    DEBUG_USART_CLK_ENABLE();
    DEBUG_USART_PORT_CLK_ENABLE();

    bsp_gpio_init ( DEBUG_USART_TX_PORT, DEBUG_USART_TX_PIN, GPIO_Mode_AF_PP, GPIO_Speed_50MHz );

    bsp_gpio_init ( DEBUG_USART_RX_PORT, DEBUG_USART_RX_PIN, GPIO_Mode_IN_FLOATING, GPIO_Speed_50MHz );


    //DEBUG_USART NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init ( &NVIC_InitStructure );	//根据指定的参数初始化VIC寄存器

    //USART 初始化设置

    USART_InitStructure.USART_BaudRate = bound;//串口波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

    USART_Init ( DEBUG_USART, &USART_InitStructure ); //初始化串口1
    USART_ITConfig ( DEBUG_USART, USART_IT_RXNE, ENABLE ); //开启串口接受中断
    USART_Cmd ( DEBUG_USART, ENABLE );                 //使能串口1

}

void USART1_IRQHandler ( void )                	//串口1中断服务程序
{
    uint8_t Res;
    if ( USART_GetITStatus ( DEBUG_USART, USART_IT_RXNE ) != RESET ) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
	Res = USART_ReceiveData ( DEBUG_USART );	//读取接收到的数据

	/*
	   if ( ( USART_RX_STA & 0x8000 ) == 0 ) //接收未完成
	   {
	   if ( USART_RX_STA & 0x4000 ) //接收到了0x0d
	   {
	   if ( Res != 0x0a ) USART_RX_STA = 0; //接收错误,重新开始
	   else USART_RX_STA |= 0x8000;	//接收完成了
	   }
	   else //还没收到0X0D
	   {
	   if ( Res == 0x0d ) USART_RX_STA |= 0x4000;
	   else
	   {
	   USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
	   USART_RX_STA++;
	   if ( USART_RX_STA > ( USART_REC_LEN - 1 ) ) USART_RX_STA = 0; //接收数据错误,重新开始接收
	   }
	   }
	   }
	   */
    }
}

