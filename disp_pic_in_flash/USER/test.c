#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "led.h"
#include "lcd.h"
#include "image2lcd.h"
#include "stdlib.h"
//ALIENTEK 战舰板 STM32开发板扩展实验
//内部flash图片显示实验
//技术支持：www.openedv.co
//广州市星翼电子科技有限公司

extern const u8 gImage_image1[];//图片数据(包含信息头),存储在image1.c里面.
int main ( void )
{
    u8 i = 0;
    HEADCOLOR *imginfo;
    u16 x = 0, y = 0;
    u16 x0, y0;
    imginfo = ( HEADCOLOR* ) gImage_image1;	//得到文件信息
    Stm32_Clock_Init ( 9 ); //系统时钟设置
    delay_init ( 72 );		//延时初始化
    uart_init ( 72, 115200 ); //串口1初始化
    LED_Init();			//LED初始化
    LCD_Init(); 	    //LCD初始化
    srand ( imginfo->h * imginfo->w );

    POINT_COLOR = RED;
    LCD_ShowString ( 30, 50, 200, 16, 16, "WarShip STM32" );
    LCD_ShowString ( 30, 70, 200, 16, 16, "IN Flash Picture Display" );
    LCD_ShowString ( 30, 90, 200, 16, 16, "ATOM@ALIENTEK" );
    LCD_ShowString ( 30, 110, 200, 16, 16, "2016/4/12" );
    while ( 1 )
    {

	if ( i == 0 )
	{
	    LCD_Clear ( 0X0000 ); //黑屏
	    if ( imginfo->w >= lcddev.width || imginfo->h >= lcddev.height )
	    {
		POINT_COLOR = RED;
		delay_ms ( 200 );
		LCD_ShowString ( 10, 70, 200, 16, 16, "The Picture is too large" );
		delay_ms ( 200 );
		continue;
	    }
	    x0 = x;
	    y0 = y;
	    while ( ( x + imginfo->w ) > lcddev.width || x == x0 ) //超过屏幕尺寸了
	    {
		x = rand(); //获得随机的x值
	    }
	    while ( ( y + imginfo->h ) > lcddev.height || y == y0 ) //超过屏幕尺寸了
	    {
		y = rand(); //获得随机的y值
	    }
	    image_display ( x, y, ( u8* ) gImage_image1 ); //在指定地址显示图片
	}
	i++;
	if ( i > 10 ) i = 0;
	LED0 = !LED0;
	delay_ms ( 200 );
    }
}






