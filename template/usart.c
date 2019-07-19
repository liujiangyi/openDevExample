#include "sys.h"
#include "usart.h"

#if 1
#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;

};

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
_sys_exit ( int x )
{
    x = x;
}

//�ض���fputc����
int fputc ( int ch, FILE *f )
{
    while ( ( DEBUG_USART->SR & 0X40 ) == 0 )
	; //ѭ������,ֱ���������
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


    //DEBUG_USART NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //��ռ���ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init ( &NVIC_InitStructure );	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

    //USART ��ʼ������

    USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

    USART_Init ( DEBUG_USART, &USART_InitStructure ); //��ʼ������1
    USART_ITConfig ( DEBUG_USART, USART_IT_RXNE, ENABLE ); //�������ڽ����ж�
    USART_Cmd ( DEBUG_USART, ENABLE );                 //ʹ�ܴ���1

}

void USART1_IRQHandler ( void )                	//����1�жϷ������
{
    uint8_t Res;
    if ( USART_GetITStatus ( DEBUG_USART, USART_IT_RXNE ) != RESET ) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
	Res = USART_ReceiveData ( DEBUG_USART );	//��ȡ���յ�������

	/*
	   if ( ( USART_RX_STA & 0x8000 ) == 0 ) //����δ���
	   {
	   if ( USART_RX_STA & 0x4000 ) //���յ���0x0d
	   {
	   if ( Res != 0x0a ) USART_RX_STA = 0; //���մ���,���¿�ʼ
	   else USART_RX_STA |= 0x8000;	//���������
	   }
	   else //��û�յ�0X0D
	   {
	   if ( Res == 0x0d ) USART_RX_STA |= 0x4000;
	   else
	   {
	   USART_RX_BUF[USART_RX_STA & 0X3FFF] = Res ;
	   USART_RX_STA++;
	   if ( USART_RX_STA > ( USART_REC_LEN - 1 ) ) USART_RX_STA = 0; //�������ݴ���,���¿�ʼ����
	   }
	   }
	   }
	   */
    }
}

