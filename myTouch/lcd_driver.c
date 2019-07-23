#include "lcd_driver.h"
#include "font.h"

void lcd_driver_fsmc_gpio_init ( void )
{

    RCC_APB2PeriphClockCmd ( RCC_APB2Periph_GPIOB
	    | RCC_APB2Periph_GPIOD
	    | RCC_APB2Periph_GPIOE
	    | RCC_APB2Periph_GPIOG,
	    ENABLE
	    );

    // PB0 背光
    bsp_gpio_init ( GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP, GPIO_Speed_50MHz );

    //PBG
    bsp_gpio_init ( GPIOG, GPIO_Pin_0 | GPIO_Pin_12, GPIO_Mode_AF_PP, GPIO_Speed_50MHz );

    //PBD
    bsp_gpio_init ( GPIOD, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |GPIO_Pin_8 |GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_14|GPIO_Pin_15,
	    GPIO_Mode_AF_PP, GPIO_Speed_50MHz );

    //PBE
    bsp_gpio_init ( GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |GPIO_Pin_11 |GPIO_Pin_12 | GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15,
	     GPIO_Mode_AF_PP, GPIO_Speed_50MHz );
}

void lcd_driver_fsmc_init ( void )
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;

    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming;
    FSMC_NORSRAMTimingInitTypeDef  writeTiming;
    RCC_AHBPeriphClockCmd ( RCC_AHBPeriph_FSMC, ENABLE );	//使能FSMC时钟


    readWriteTiming.FSMC_AddressSetupTime = 0x01;	 //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（ADDHLD）模式A未用到
    readWriteTiming.FSMC_DataSetupTime = 0x0f;		 // 数据保存时间为16个HCLK,因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A


    writeTiming.FSMC_AddressSetupTime = 0x00;	 //地址建立时间（ADDSET）为1个HCLK
    writeTiming.FSMC_AddressHoldTime = 0x00;	 //地址保持时间（A
    writeTiming.FSMC_DataSetupTime = 0x03;		 ////数据保存时间为4个HCLK
    writeTiming.FSMC_BusTurnAroundDuration = 0x00;
    writeTiming.FSMC_CLKDivision = 0x00;
    writeTiming.FSMC_DataLatency = 0x00;
    writeTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //模式A

    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;//  这里我们使用NE4 ，也就对应BTCR[6],[7]。
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; // 不复用数据地址
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM; // FSMC_MemoryType_SRAM;  //SRAM
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//存储器数据宽度为16bit
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable; // FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//  存储器写使能
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable; // 读写使用不同的时序
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming; //读写时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &writeTiming;  //写时序

    FSMC_NORSRAMInit ( &FSMC_NORSRAMInitStructure ); //初始化FSMC配置

    FSMC_NORSRAMCmd ( FSMC_Bank1_NORSRAM4, ENABLE ); // 使能BANK1
}

void lcd_register_init(void)
{
    lcddev.id = LCD_ReadReg(0x0000);
    printf("lcd id:%x\n",lcddev.id);

    LCD_WriteReg ( 0xF000, 0x55 );
    LCD_WriteReg ( 0xF001, 0xAA );
    LCD_WriteReg ( 0xF002, 0x52 );
    LCD_WriteReg ( 0xF003, 0x08 );
    LCD_WriteReg ( 0xF004, 0x01 );
    //AVDD Set AVDD 5.2V
    LCD_WriteReg ( 0xB000, 0x0D );
    LCD_WriteReg ( 0xB001, 0x0D );
    LCD_WriteReg ( 0xB002, 0x0D );
    //AVDD ratio
    LCD_WriteReg ( 0xB600, 0x34 );
    LCD_WriteReg ( 0xB601, 0x34 );
    LCD_WriteReg ( 0xB602, 0x34 );
    //AVEE -5.2V
    LCD_WriteReg ( 0xB100, 0x0D );
    LCD_WriteReg ( 0xB101, 0x0D );
    LCD_WriteReg ( 0xB102, 0x0D );
    //AVEE ratio
    LCD_WriteReg ( 0xB700, 0x34 );
    LCD_WriteReg ( 0xB701, 0x34 );
    LCD_WriteReg ( 0xB702, 0x34 );
    //VCL -2.5V
    LCD_WriteReg ( 0xB200, 0x00 );
    LCD_WriteReg ( 0xB201, 0x00 );
    LCD_WriteReg ( 0xB202, 0x00 );
    //VCL ratio
    LCD_WriteReg ( 0xB800, 0x24 );
    LCD_WriteReg ( 0xB801, 0x24 );
    LCD_WriteReg ( 0xB802, 0x24 );
    //VGH 15V (Free pump)
    LCD_WriteReg ( 0xBF00, 0x01 );
    LCD_WriteReg ( 0xB300, 0x0F );
    LCD_WriteReg ( 0xB301, 0x0F );
    LCD_WriteReg ( 0xB302, 0x0F );
    //VGH ratio
    LCD_WriteReg ( 0xB900, 0x34 );
    LCD_WriteReg ( 0xB901, 0x34 );
    LCD_WriteReg ( 0xB902, 0x34 );
    //VGL_REG -10V
    LCD_WriteReg ( 0xB500, 0x08 );
    LCD_WriteReg ( 0xB501, 0x08 );
    LCD_WriteReg ( 0xB502, 0x08 );
    LCD_WriteReg ( 0xC200, 0x03 );
    //VGLX ratio
    LCD_WriteReg ( 0xBA00, 0x24 );
    LCD_WriteReg ( 0xBA01, 0x24 );
    LCD_WriteReg ( 0xBA02, 0x24 );
    //VGMP/VGSP 4.5V/0V
    LCD_WriteReg ( 0xBC00, 0x00 );
    LCD_WriteReg ( 0xBC01, 0x78 );
    LCD_WriteReg ( 0xBC02, 0x00 );
    //VGMN/VGSN -4.5V/0V
    LCD_WriteReg ( 0xBD00, 0x00 );
    LCD_WriteReg ( 0xBD01, 0x78 );
    LCD_WriteReg ( 0xBD02, 0x00 );
    //VCOM
    LCD_WriteReg ( 0xBE00, 0x00 );
    LCD_WriteReg ( 0xBE01, 0x64 );
    //Gamma Setting
    LCD_WriteReg ( 0xD100, 0x00 );
    LCD_WriteReg ( 0xD101, 0x33 );
    LCD_WriteReg ( 0xD102, 0x00 );
    LCD_WriteReg ( 0xD103, 0x34 );
    LCD_WriteReg ( 0xD104, 0x00 );
    LCD_WriteReg ( 0xD105, 0x3A );
    LCD_WriteReg ( 0xD106, 0x00 );
    LCD_WriteReg ( 0xD107, 0x4A );
    LCD_WriteReg ( 0xD108, 0x00 );
    LCD_WriteReg ( 0xD109, 0x5C );
    LCD_WriteReg ( 0xD10A, 0x00 );
    LCD_WriteReg ( 0xD10B, 0x81 );
    LCD_WriteReg ( 0xD10C, 0x00 );
    LCD_WriteReg ( 0xD10D, 0xA6 );
    LCD_WriteReg ( 0xD10E, 0x00 );
    LCD_WriteReg ( 0xD10F, 0xE5 );
    LCD_WriteReg ( 0xD110, 0x01 );
    LCD_WriteReg ( 0xD111, 0x13 );
    LCD_WriteReg ( 0xD112, 0x01 );
    LCD_WriteReg ( 0xD113, 0x54 );
    LCD_WriteReg ( 0xD114, 0x01 );
    LCD_WriteReg ( 0xD115, 0x82 );
    LCD_WriteReg ( 0xD116, 0x01 );
    LCD_WriteReg ( 0xD117, 0xCA );
    LCD_WriteReg ( 0xD118, 0x02 );
    LCD_WriteReg ( 0xD119, 0x00 );
    LCD_WriteReg ( 0xD11A, 0x02 );
    LCD_WriteReg ( 0xD11B, 0x01 );
    LCD_WriteReg ( 0xD11C, 0x02 );
    LCD_WriteReg ( 0xD11D, 0x34 );
    LCD_WriteReg ( 0xD11E, 0x02 );
    LCD_WriteReg ( 0xD11F, 0x67 );
    LCD_WriteReg ( 0xD120, 0x02 );
    LCD_WriteReg ( 0xD121, 0x84 );
    LCD_WriteReg ( 0xD122, 0x02 );
    LCD_WriteReg ( 0xD123, 0xA4 );
    LCD_WriteReg ( 0xD124, 0x02 );
    LCD_WriteReg ( 0xD125, 0xB7 );
    LCD_WriteReg ( 0xD126, 0x02 );
    LCD_WriteReg ( 0xD127, 0xCF );
    LCD_WriteReg ( 0xD128, 0x02 );
    LCD_WriteReg ( 0xD129, 0xDE );
    LCD_WriteReg ( 0xD12A, 0x02 );
    LCD_WriteReg ( 0xD12B, 0xF2 );
    LCD_WriteReg ( 0xD12C, 0x02 );
    LCD_WriteReg ( 0xD12D, 0xFE );
    LCD_WriteReg ( 0xD12E, 0x03 );
    LCD_WriteReg ( 0xD12F, 0x10 );
    LCD_WriteReg ( 0xD130, 0x03 );
    LCD_WriteReg ( 0xD131, 0x33 );
    LCD_WriteReg ( 0xD132, 0x03 );
    LCD_WriteReg ( 0xD133, 0x6D );
    LCD_WriteReg ( 0xD200, 0x00 );
    LCD_WriteReg ( 0xD201, 0x33 );
    LCD_WriteReg ( 0xD202, 0x00 );
    LCD_WriteReg ( 0xD203, 0x34 );
    LCD_WriteReg ( 0xD204, 0x00 );
    LCD_WriteReg ( 0xD205, 0x3A );
    LCD_WriteReg ( 0xD206, 0x00 );
    LCD_WriteReg ( 0xD207, 0x4A );
    LCD_WriteReg ( 0xD208, 0x00 );
    LCD_WriteReg ( 0xD209, 0x5C );
    LCD_WriteReg ( 0xD20A, 0x00 );

    LCD_WriteReg ( 0xD20B, 0x81 );
    LCD_WriteReg ( 0xD20C, 0x00 );
    LCD_WriteReg ( 0xD20D, 0xA6 );
    LCD_WriteReg ( 0xD20E, 0x00 );
    LCD_WriteReg ( 0xD20F, 0xE5 );
    LCD_WriteReg ( 0xD210, 0x01 );
    LCD_WriteReg ( 0xD211, 0x13 );
    LCD_WriteReg ( 0xD212, 0x01 );
    LCD_WriteReg ( 0xD213, 0x54 );
    LCD_WriteReg ( 0xD214, 0x01 );
    LCD_WriteReg ( 0xD215, 0x82 );
    LCD_WriteReg ( 0xD216, 0x01 );
    LCD_WriteReg ( 0xD217, 0xCA );
    LCD_WriteReg ( 0xD218, 0x02 );
    LCD_WriteReg ( 0xD219, 0x00 );
    LCD_WriteReg ( 0xD21A, 0x02 );
    LCD_WriteReg ( 0xD21B, 0x01 );
    LCD_WriteReg ( 0xD21C, 0x02 );
    LCD_WriteReg ( 0xD21D, 0x34 );
    LCD_WriteReg ( 0xD21E, 0x02 );
    LCD_WriteReg ( 0xD21F, 0x67 );
    LCD_WriteReg ( 0xD220, 0x02 );
    LCD_WriteReg ( 0xD221, 0x84 );
    LCD_WriteReg ( 0xD222, 0x02 );
    LCD_WriteReg ( 0xD223, 0xA4 );
    LCD_WriteReg ( 0xD224, 0x02 );
    LCD_WriteReg ( 0xD225, 0xB7 );
    LCD_WriteReg ( 0xD226, 0x02 );
    LCD_WriteReg ( 0xD227, 0xCF );
    LCD_WriteReg ( 0xD228, 0x02 );
    LCD_WriteReg ( 0xD229, 0xDE );
    LCD_WriteReg ( 0xD22A, 0x02 );
    LCD_WriteReg ( 0xD22B, 0xF2 );
    LCD_WriteReg ( 0xD22C, 0x02 );
    LCD_WriteReg ( 0xD22D, 0xFE );
    LCD_WriteReg ( 0xD22E, 0x03 );
    LCD_WriteReg ( 0xD22F, 0x10 );
    LCD_WriteReg ( 0xD230, 0x03 );
    LCD_WriteReg ( 0xD231, 0x33 );
    LCD_WriteReg ( 0xD232, 0x03 );
    LCD_WriteReg ( 0xD233, 0x6D );
    LCD_WriteReg ( 0xD300, 0x00 );
    LCD_WriteReg ( 0xD301, 0x33 );
    LCD_WriteReg ( 0xD302, 0x00 );
    LCD_WriteReg ( 0xD303, 0x34 );
    LCD_WriteReg ( 0xD304, 0x00 );
    LCD_WriteReg ( 0xD305, 0x3A );
    LCD_WriteReg ( 0xD306, 0x00 );
    LCD_WriteReg ( 0xD307, 0x4A );
    LCD_WriteReg ( 0xD308, 0x00 );
    LCD_WriteReg ( 0xD309, 0x5C );
    LCD_WriteReg ( 0xD30A, 0x00 );

    LCD_WriteReg ( 0xD30B, 0x81 );
    LCD_WriteReg ( 0xD30C, 0x00 );
    LCD_WriteReg ( 0xD30D, 0xA6 );
    LCD_WriteReg ( 0xD30E, 0x00 );
    LCD_WriteReg ( 0xD30F, 0xE5 );
    LCD_WriteReg ( 0xD310, 0x01 );
    LCD_WriteReg ( 0xD311, 0x13 );
    LCD_WriteReg ( 0xD312, 0x01 );
    LCD_WriteReg ( 0xD313, 0x54 );
    LCD_WriteReg ( 0xD314, 0x01 );
    LCD_WriteReg ( 0xD315, 0x82 );
    LCD_WriteReg ( 0xD316, 0x01 );
    LCD_WriteReg ( 0xD317, 0xCA );
    LCD_WriteReg ( 0xD318, 0x02 );
    LCD_WriteReg ( 0xD319, 0x00 );
    LCD_WriteReg ( 0xD31A, 0x02 );
    LCD_WriteReg ( 0xD31B, 0x01 );
    LCD_WriteReg ( 0xD31C, 0x02 );
    LCD_WriteReg ( 0xD31D, 0x34 );
    LCD_WriteReg ( 0xD31E, 0x02 );
    LCD_WriteReg ( 0xD31F, 0x67 );
    LCD_WriteReg ( 0xD320, 0x02 );
    LCD_WriteReg ( 0xD321, 0x84 );
    LCD_WriteReg ( 0xD322, 0x02 );
    LCD_WriteReg ( 0xD323, 0xA4 );
    LCD_WriteReg ( 0xD324, 0x02 );
    LCD_WriteReg ( 0xD325, 0xB7 );
    LCD_WriteReg ( 0xD326, 0x02 );
    LCD_WriteReg ( 0xD327, 0xCF );
    LCD_WriteReg ( 0xD328, 0x02 );
    LCD_WriteReg ( 0xD329, 0xDE );
    LCD_WriteReg ( 0xD32A, 0x02 );
    LCD_WriteReg ( 0xD32B, 0xF2 );
    LCD_WriteReg ( 0xD32C, 0x02 );
    LCD_WriteReg ( 0xD32D, 0xFE );
    LCD_WriteReg ( 0xD32E, 0x03 );
    LCD_WriteReg ( 0xD32F, 0x10 );
    LCD_WriteReg ( 0xD330, 0x03 );
    LCD_WriteReg ( 0xD331, 0x33 );
    LCD_WriteReg ( 0xD332, 0x03 );
    LCD_WriteReg ( 0xD333, 0x6D );
    LCD_WriteReg ( 0xD400, 0x00 );
    LCD_WriteReg ( 0xD401, 0x33 );
    LCD_WriteReg ( 0xD402, 0x00 );
    LCD_WriteReg ( 0xD403, 0x34 );
    LCD_WriteReg ( 0xD404, 0x00 );
    LCD_WriteReg ( 0xD405, 0x3A );
    LCD_WriteReg ( 0xD406, 0x00 );
    LCD_WriteReg ( 0xD407, 0x4A );
    LCD_WriteReg ( 0xD408, 0x00 );
    LCD_WriteReg ( 0xD409, 0x5C );
    LCD_WriteReg ( 0xD40A, 0x00 );
    LCD_WriteReg ( 0xD40B, 0x81 );

    LCD_WriteReg ( 0xD40C, 0x00 );
    LCD_WriteReg ( 0xD40D, 0xA6 );
    LCD_WriteReg ( 0xD40E, 0x00 );
    LCD_WriteReg ( 0xD40F, 0xE5 );
    LCD_WriteReg ( 0xD410, 0x01 );
    LCD_WriteReg ( 0xD411, 0x13 );
    LCD_WriteReg ( 0xD412, 0x01 );
    LCD_WriteReg ( 0xD413, 0x54 );
    LCD_WriteReg ( 0xD414, 0x01 );
    LCD_WriteReg ( 0xD415, 0x82 );
    LCD_WriteReg ( 0xD416, 0x01 );
    LCD_WriteReg ( 0xD417, 0xCA );
    LCD_WriteReg ( 0xD418, 0x02 );
    LCD_WriteReg ( 0xD419, 0x00 );
    LCD_WriteReg ( 0xD41A, 0x02 );
    LCD_WriteReg ( 0xD41B, 0x01 );
    LCD_WriteReg ( 0xD41C, 0x02 );
    LCD_WriteReg ( 0xD41D, 0x34 );
    LCD_WriteReg ( 0xD41E, 0x02 );
    LCD_WriteReg ( 0xD41F, 0x67 );
    LCD_WriteReg ( 0xD420, 0x02 );
    LCD_WriteReg ( 0xD421, 0x84 );
    LCD_WriteReg ( 0xD422, 0x02 );
    LCD_WriteReg ( 0xD423, 0xA4 );
    LCD_WriteReg ( 0xD424, 0x02 );
    LCD_WriteReg ( 0xD425, 0xB7 );
    LCD_WriteReg ( 0xD426, 0x02 );
    LCD_WriteReg ( 0xD427, 0xCF );
    LCD_WriteReg ( 0xD428, 0x02 );
    LCD_WriteReg ( 0xD429, 0xDE );
    LCD_WriteReg ( 0xD42A, 0x02 );
    LCD_WriteReg ( 0xD42B, 0xF2 );
    LCD_WriteReg ( 0xD42C, 0x02 );
    LCD_WriteReg ( 0xD42D, 0xFE );
    LCD_WriteReg ( 0xD42E, 0x03 );
    LCD_WriteReg ( 0xD42F, 0x10 );
    LCD_WriteReg ( 0xD430, 0x03 );
    LCD_WriteReg ( 0xD431, 0x33 );
    LCD_WriteReg ( 0xD432, 0x03 );
    LCD_WriteReg ( 0xD433, 0x6D );
    LCD_WriteReg ( 0xD500, 0x00 );
    LCD_WriteReg ( 0xD501, 0x33 );
    LCD_WriteReg ( 0xD502, 0x00 );
    LCD_WriteReg ( 0xD503, 0x34 );
    LCD_WriteReg ( 0xD504, 0x00 );
    LCD_WriteReg ( 0xD505, 0x3A );
    LCD_WriteReg ( 0xD506, 0x00 );
    LCD_WriteReg ( 0xD507, 0x4A );
    LCD_WriteReg ( 0xD508, 0x00 );
    LCD_WriteReg ( 0xD509, 0x5C );
    LCD_WriteReg ( 0xD50A, 0x00 );
    LCD_WriteReg ( 0xD50B, 0x81 );

    LCD_WriteReg ( 0xD50C, 0x00 );
    LCD_WriteReg ( 0xD50D, 0xA6 );
    LCD_WriteReg ( 0xD50E, 0x00 );
    LCD_WriteReg ( 0xD50F, 0xE5 );
    LCD_WriteReg ( 0xD510, 0x01 );
    LCD_WriteReg ( 0xD511, 0x13 );
    LCD_WriteReg ( 0xD512, 0x01 );
    LCD_WriteReg ( 0xD513, 0x54 );
    LCD_WriteReg ( 0xD514, 0x01 );
    LCD_WriteReg ( 0xD515, 0x82 );
    LCD_WriteReg ( 0xD516, 0x01 );
    LCD_WriteReg ( 0xD517, 0xCA );
    LCD_WriteReg ( 0xD518, 0x02 );
    LCD_WriteReg ( 0xD519, 0x00 );
    LCD_WriteReg ( 0xD51A, 0x02 );
    LCD_WriteReg ( 0xD51B, 0x01 );
    LCD_WriteReg ( 0xD51C, 0x02 );
    LCD_WriteReg ( 0xD51D, 0x34 );
    LCD_WriteReg ( 0xD51E, 0x02 );
    LCD_WriteReg ( 0xD51F, 0x67 );
    LCD_WriteReg ( 0xD520, 0x02 );
    LCD_WriteReg ( 0xD521, 0x84 );
    LCD_WriteReg ( 0xD522, 0x02 );
    LCD_WriteReg ( 0xD523, 0xA4 );
    LCD_WriteReg ( 0xD524, 0x02 );
    LCD_WriteReg ( 0xD525, 0xB7 );
    LCD_WriteReg ( 0xD526, 0x02 );
    LCD_WriteReg ( 0xD527, 0xCF );
    LCD_WriteReg ( 0xD528, 0x02 );
    LCD_WriteReg ( 0xD529, 0xDE );
    LCD_WriteReg ( 0xD52A, 0x02 );
    LCD_WriteReg ( 0xD52B, 0xF2 );
    LCD_WriteReg ( 0xD52C, 0x02 );
    LCD_WriteReg ( 0xD52D, 0xFE );
    LCD_WriteReg ( 0xD52E, 0x03 );
    LCD_WriteReg ( 0xD52F, 0x10 );
    LCD_WriteReg ( 0xD530, 0x03 );
    LCD_WriteReg ( 0xD531, 0x33 );
    LCD_WriteReg ( 0xD532, 0x03 );
    LCD_WriteReg ( 0xD533, 0x6D );
    LCD_WriteReg ( 0xD600, 0x00 );
    LCD_WriteReg ( 0xD601, 0x33 );
    LCD_WriteReg ( 0xD602, 0x00 );
    LCD_WriteReg ( 0xD603, 0x34 );
    LCD_WriteReg ( 0xD604, 0x00 );
    LCD_WriteReg ( 0xD605, 0x3A );
    LCD_WriteReg ( 0xD606, 0x00 );
    LCD_WriteReg ( 0xD607, 0x4A );
    LCD_WriteReg ( 0xD608, 0x00 );
    LCD_WriteReg ( 0xD609, 0x5C );
    LCD_WriteReg ( 0xD60A, 0x00 );
    LCD_WriteReg ( 0xD60B, 0x81 );

    LCD_WriteReg ( 0xD60C, 0x00 );
    LCD_WriteReg ( 0xD60D, 0xA6 );
    LCD_WriteReg ( 0xD60E, 0x00 );
    LCD_WriteReg ( 0xD60F, 0xE5 );
    LCD_WriteReg ( 0xD610, 0x01 );
    LCD_WriteReg ( 0xD611, 0x13 );
    LCD_WriteReg ( 0xD612, 0x01 );
    LCD_WriteReg ( 0xD613, 0x54 );
    LCD_WriteReg ( 0xD614, 0x01 );
    LCD_WriteReg ( 0xD615, 0x82 );
    LCD_WriteReg ( 0xD616, 0x01 );
    LCD_WriteReg ( 0xD617, 0xCA );
    LCD_WriteReg ( 0xD618, 0x02 );
    LCD_WriteReg ( 0xD619, 0x00 );
    LCD_WriteReg ( 0xD61A, 0x02 );
    LCD_WriteReg ( 0xD61B, 0x01 );
    LCD_WriteReg ( 0xD61C, 0x02 );
    LCD_WriteReg ( 0xD61D, 0x34 );
    LCD_WriteReg ( 0xD61E, 0x02 );
    LCD_WriteReg ( 0xD61F, 0x67 );
    LCD_WriteReg ( 0xD620, 0x02 );
    LCD_WriteReg ( 0xD621, 0x84 );
    LCD_WriteReg ( 0xD622, 0x02 );
    LCD_WriteReg ( 0xD623, 0xA4 );
    LCD_WriteReg ( 0xD624, 0x02 );
    LCD_WriteReg ( 0xD625, 0xB7 );
    LCD_WriteReg ( 0xD626, 0x02 );
    LCD_WriteReg ( 0xD627, 0xCF );
    LCD_WriteReg ( 0xD628, 0x02 );
    LCD_WriteReg ( 0xD629, 0xDE );
    LCD_WriteReg ( 0xD62A, 0x02 );
    LCD_WriteReg ( 0xD62B, 0xF2 );
    LCD_WriteReg ( 0xD62C, 0x02 );
    LCD_WriteReg ( 0xD62D, 0xFE );
    LCD_WriteReg ( 0xD62E, 0x03 );
    LCD_WriteReg ( 0xD62F, 0x10 );
    LCD_WriteReg ( 0xD630, 0x03 );
    LCD_WriteReg ( 0xD631, 0x33 );
    LCD_WriteReg ( 0xD632, 0x03 );
    LCD_WriteReg ( 0xD633, 0x6D );
    //LV2 Page 0 enable
    LCD_WriteReg ( 0xF000, 0x55 );
    LCD_WriteReg ( 0xF001, 0xAA );
    LCD_WriteReg ( 0xF002, 0x52 );
    LCD_WriteReg ( 0xF003, 0x08 );
    LCD_WriteReg ( 0xF004, 0x00 );
    //Display control
    LCD_WriteReg ( 0xB100, 0xCC );
    LCD_WriteReg ( 0xB101, 0x00 );
    //Source hold time
    LCD_WriteReg ( 0xB600, 0x05 );
    //Gate EQ control
    LCD_WriteReg ( 0xB700, 0x70 );
    LCD_WriteReg ( 0xB701, 0x70 );
    //Source EQ control (Mode 2)
    LCD_WriteReg ( 0xB800, 0x01 );
    LCD_WriteReg ( 0xB801, 0x03 );
    LCD_WriteReg ( 0xB802, 0x03 );
    LCD_WriteReg ( 0xB803, 0x03 );
    //Inversion mode (2-dot)
    LCD_WriteReg ( 0xBC00, 0x02 );
    LCD_WriteReg ( 0xBC01, 0x00 );
    LCD_WriteReg ( 0xBC02, 0x00 );
    //Timing control 4H w/ 4-delay
    LCD_WriteReg ( 0xC900, 0xD0 );
    LCD_WriteReg ( 0xC901, 0x02 );
    LCD_WriteReg ( 0xC902, 0x50 );
    LCD_WriteReg ( 0xC903, 0x50 );

    LCD_Display_Dir ( 0 );		//默认为竖屏
    LCD_LED_ON();
    LCD_Clear ( WHITE );
}


//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/5408/9341/5310/5510/1963等IC已经实际测试
void LCD_Scan_Dir ( u8 dir )
{
    u16 regval = 0;
    u16 dirreg = 0;
    u16 temp;
    if ( ( lcddev.dir == 1 && lcddev.id != 0X6804 && lcddev.id != 0X1963 ) || ( lcddev.dir == 0 && lcddev.id == 0X1963 ) ) //横屏时，对6804和1963不改变扫描方向！竖屏时1963改变方向
    {
	switch ( dir ) //方向转换
	{
	    case 0:
		dir = 6;
		break;
	    case 1:
		dir = 7;
		break;
	    case 2:
		dir = 4;
		break;
	    case 3:
		dir = 5;
		break;
	    case 4:
		dir = 1;
		break;
	    case 5:
		dir = 0;
		break;
	    case 6:
		dir = 3;
		break;
	    case 7:
		dir = 2;
		break;
	}
    }
    if ( lcddev.id == 0x9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310 || lcddev.id == 0X5510 || lcddev.id == 0X1963 ) //9341/6804/5310/5510/1963,特殊处理
    {
	switch ( dir )
	{
	    case L2R_U2D://从左到右,从上到下
		regval |= ( 0 << 7 ) | ( 0 << 6 ) | ( 0 << 5 );
		break;
	    case L2R_D2U://从左到右,从下到上
		regval |= ( 1 << 7 ) | ( 0 << 6 ) | ( 0 << 5 );
		break;
	    case R2L_U2D://从右到左,从上到下
		regval |= ( 0 << 7 ) | ( 1 << 6 ) | ( 0 << 5 );
		break;
	    case R2L_D2U://从右到左,从下到上
		regval |= ( 1 << 7 ) | ( 1 << 6 ) | ( 0 << 5 );
		break;
	    case U2D_L2R://从上到下,从左到右
		regval |= ( 0 << 7 ) | ( 0 << 6 ) | ( 1 << 5 );
		break;
	    case U2D_R2L://从上到下,从右到左
		regval |= ( 0 << 7 ) | ( 1 << 6 ) | ( 1 << 5 );
		break;
	    case D2U_L2R://从下到上,从左到右
		regval |= ( 1 << 7 ) | ( 0 << 6 ) | ( 1 << 5 );
		break;
	    case D2U_R2L://从下到上,从右到左
		regval |= ( 1 << 7 ) | ( 1 << 6 ) | ( 1 << 5 );
		break;
	}
	if ( lcddev.id == 0X5510 ) dirreg = 0X3600;
	else dirreg = 0X36;
	if ( ( lcddev.id != 0X5310 ) && ( lcddev.id != 0X5510 ) && ( lcddev.id != 0X1963 ) ) regval |= 0X08; //5310/5510/1963不需要BGR
	if ( lcddev.id == 0X6804 ) regval |= 0x02; //6804的BIT6和9341的反了
	LCD_WriteReg ( dirreg, regval );
	if ( lcddev.id != 0X1963 ) //1963不做坐标处理
	{
	    if ( regval & 0X20 )
	    {
		if ( lcddev.width < lcddev.height ) //交换X,Y
		{
		    temp = lcddev.width;
		    lcddev.width = lcddev.height;
		    lcddev.height = temp;
		}
	    }
	    else
	    {
		if ( lcddev.width > lcddev.height ) //交换X,Y
		{
		    temp = lcddev.width;
		    lcddev.width = lcddev.height;
		    lcddev.height = temp;
		}
	    }
	}
	if ( lcddev.id == 0X5510 )
	{
	    LCD_WR_REG ( lcddev.setxcmd );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_REG ( lcddev.setxcmd + 1 );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_REG ( lcddev.setxcmd + 2 );
	    LCD_WR_DATA ( ( lcddev.width - 1 ) >> 8 );
	    LCD_WR_REG ( lcddev.setxcmd + 3 );
	    LCD_WR_DATA ( ( lcddev.width - 1 ) & 0XFF );
	    LCD_WR_REG ( lcddev.setycmd );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_REG ( lcddev.setycmd + 1 );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_REG ( lcddev.setycmd + 2 );
	    LCD_WR_DATA ( ( lcddev.height - 1 ) >> 8 );
	    LCD_WR_REG ( lcddev.setycmd + 3 );
	    LCD_WR_DATA ( ( lcddev.height - 1 ) & 0XFF );
	}
	else
	{
	    LCD_WR_REG ( lcddev.setxcmd );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_DATA ( ( lcddev.width - 1 ) >> 8 );
	    LCD_WR_DATA ( ( lcddev.width - 1 ) & 0XFF );
	    LCD_WR_REG ( lcddev.setycmd );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_DATA ( ( lcddev.height - 1 ) >> 8 );
	    LCD_WR_DATA ( ( lcddev.height - 1 ) & 0XFF );
	}
    }
    else
    {
	switch ( dir )
	{
	    case L2R_U2D://从左到右,从上到下
		regval |= ( 1 << 5 ) | ( 1 << 4 ) | ( 0 << 3 );
		break;
	    case L2R_D2U://从左到右,从下到上
		regval |= ( 0 << 5 ) | ( 1 << 4 ) | ( 0 << 3 );
		break;
	    case R2L_U2D://从右到左,从上到下
		regval |= ( 1 << 5 ) | ( 0 << 4 ) | ( 0 << 3 );
		break;
	    case R2L_D2U://从右到左,从下到上
		regval |= ( 0 << 5 ) | ( 0 << 4 ) | ( 0 << 3 );
		break;
	    case U2D_L2R://从上到下,从左到右
		regval |= ( 1 << 5 ) | ( 1 << 4 ) | ( 1 << 3 );
		break;
	    case U2D_R2L://从上到下,从右到左
		regval |= ( 1 << 5 ) | ( 0 << 4 ) | ( 1 << 3 );
		break;
	    case D2U_L2R://从下到上,从左到右
		regval |= ( 0 << 5 ) | ( 1 << 4 ) | ( 1 << 3 );
		break;
	    case D2U_R2L://从下到上,从右到左
		regval |= ( 0 << 5 ) | ( 0 << 4 ) | ( 1 << 3 );
		break;
	}
	dirreg = 0X03;
	regval |= 1 << 12;
	LCD_WriteReg ( dirreg, regval );
    }
}
//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir ( u8 dir )
{
    if ( dir == 0 )			//竖屏
    {
	lcddev.dir = 0;	//竖屏
	lcddev.width = 240;
	lcddev.height = 320;
	if ( lcddev.id == 0X9341 || lcddev.id == 0X6804 || lcddev.id == 0X5310 )
	{
	    lcddev.wramcmd = 0X2C;
	    lcddev.setxcmd = 0X2A;
	    lcddev.setycmd = 0X2B;
	    if ( lcddev.id == 0X6804 || lcddev.id == 0X5310 )
	    {
		lcddev.width = 320;
		lcddev.height = 480;
	    }
	}
	else if ( lcddev.id == 0x5510 )
	{
	    lcddev.wramcmd = 0X2C00;
	    lcddev.setxcmd = 0X2A00;
	    lcddev.setycmd = 0X2B00;
	    lcddev.width = 480;
	    lcddev.height = 800;
	}
	else if ( lcddev.id == 0X1963 )
	{
	    lcddev.wramcmd = 0X2C;	//设置写入GRAM的指令
	    lcddev.setxcmd = 0X2B;	//设置写X坐标指令
	    lcddev.setycmd = 0X2A;	//设置写Y坐标指令
	    lcddev.width = 480;		//设置宽度480
	    lcddev.height = 800;		//设置高度800
	}
	else
	{
	    lcddev.wramcmd = 0X22;
	    lcddev.setxcmd = 0X20;
	    lcddev.setycmd = 0X21;
	}
    }
    else 				//横屏
    {
	lcddev.dir = 1;	//横屏
	lcddev.width = 320;
	lcddev.height = 240;
	if ( lcddev.id == 0X9341 || lcddev.id == 0X5310 )
	{
	    lcddev.wramcmd = 0X2C;
	    lcddev.setxcmd = 0X2A;
	    lcddev.setycmd = 0X2B;
	}
	else if ( lcddev.id == 0X6804 )
	{
	    lcddev.wramcmd = 0X2C;
	    lcddev.setxcmd = 0X2B;
	    lcddev.setycmd = 0X2A;
	}
	else if ( lcddev.id == 0x5510 )
	{
	    lcddev.wramcmd = 0X2C00;
	    lcddev.setxcmd = 0X2A00;
	    lcddev.setycmd = 0X2B00;
	    lcddev.width = 800;
	    lcddev.height = 480;
	}
	else if ( lcddev.id == 0X1963 )
	{
	    lcddev.wramcmd = 0X2C;	//设置写入GRAM的指令
	    lcddev.setxcmd = 0X2A;	//设置写X坐标指令
	    lcddev.setycmd = 0X2B;	//设置写Y坐标指令
	    lcddev.width = 800;		//设置宽度800
	    lcddev.height = 480;		//设置高度480
	}
	else
	{
	    lcddev.wramcmd = 0X22;
	    lcddev.setxcmd = 0X21;
	    lcddev.setycmd = 0X20;
	}
	if ( lcddev.id == 0X6804 || lcddev.id == 0X5310 )
	{
	    lcddev.width = 480;
	    lcddev.height = 320;
	}
    }
    LCD_Scan_Dir ( DFT_SCAN_DIR );	//默认扫描方向
}
//设置窗口,并自动设置画点坐标到窗口左上角(sx,sy).

//清屏函数
//color:要清屏的填充色
void LCD_Clear ( u16 color )
{
    u32 index = 0;
    u32 totalpoint = lcddev.width;
    totalpoint *= lcddev.height; 			//得到总点数
    if ( ( lcddev.id == 0X6804 ) && ( lcddev.dir == 1 ) ) //6804横屏的时候特殊处理
    {
	lcddev.dir = 0;
	lcddev.setxcmd = 0X2A;
	lcddev.setycmd = 0X2B;
	LCD_SetCursor ( 0x00, 0x0000 );		//设置光标位置
	lcddev.dir = 1;
	lcddev.setxcmd = 0X2B;
	lcddev.setycmd = 0X2A;
    }
    else LCD_SetCursor ( 0x00, 0x0000 );	//设置光标位置
    LCD_WriteRAM_Prepare();     		//开始写入GRAM
    for ( index = 0; index < totalpoint; index++ )
    {
	LCD->LCD_RAM = color;
    }
}

void lcd_init(void)
{
    lcd_driver_fsmc_gpio_init();
    lcd_driver_fsmc_init();
    delay_ms(50);
    lcd_register_init();
}

//LCD的画笔颜色和背景色
uint16_t POINT_COLOR = 0x0000;	//画笔颜色
uint16_t BACK_COLOR = 0xFFFF; //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

//写寄存器函数
//regval:寄存器值
void LCD_WR_REG ( uint16_t regval )
{
    LCD->LCD_REG = regval; //写入要写的寄存器序号
}
//写LCD数据
//data:要写入的值
void LCD_WR_DATA ( uint16_t data )
{
    LCD->LCD_RAM = data;
}
//读LCD数据
//返回值:读到的值
uint16_t LCD_RD_DATA ( void )
{
    vu16 ram;			//防止被优化
    ram = LCD->LCD_RAM;
    return ram;
}
//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
void LCD_WriteReg ( uint16_t LCD_Reg, uint16_t LCD_RegValue )
{
    LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号
    LCD->LCD_RAM = LCD_RegValue;//写入数据
}
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
uint16_t LCD_ReadReg ( uint16_t LCD_Reg )
{
    LCD_WR_REG ( LCD_Reg );		//写入要读的寄存器序号
    delay_us ( 5 );
    return LCD_RD_DATA();		//返回读到的值
}
//开始写GRAM
void LCD_WriteRAM_Prepare ( void )
{
    LCD->LCD_REG = lcddev.wramcmd;
}
//LCD写GRAM
//RGB_Code:颜色值
void LCD_WriteRAM ( uint16_t RGB_Code )
{
    LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
uint16_t LCD_BGR2RGB ( uint16_t c )
{
    uint16_t  r, g, b, rgb;
    b = ( c >> 0 ) & 0x1f;
    g = ( c >> 5 ) & 0x3f;
    r = ( c >> 11 ) & 0x1f;
    rgb = ( b << 11 ) + ( g << 5 ) + ( r << 0 );
    return ( rgb );
}

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay ( uint8_t i )
{
    while ( i-- );
}
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor ( u16 Xpos, u16 Ypos )
{
    if ( lcddev.id == 0X9341 || lcddev.id == 0X5310 )
    {
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( Xpos >> 8 );
	LCD_WR_DATA ( Xpos & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( Ypos >> 8 );
	LCD_WR_DATA ( Ypos & 0XFF );
    }
    else if ( lcddev.id == 0X6804 )
    {
	if ( lcddev.dir == 1 ) Xpos = lcddev.width - 1 - Xpos; //横屏时处理
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( Xpos >> 8 );
	LCD_WR_DATA ( Xpos & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( Ypos >> 8 );
	LCD_WR_DATA ( Ypos & 0XFF );
    }
    else if ( lcddev.id == 0X1963 )
    {
	if ( lcddev.dir == 0 ) //x坐标需要变换
	{
	    Xpos = lcddev.width - 1 - Xpos;
	    LCD_WR_REG ( lcddev.setxcmd );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_DATA ( 0 );
	    LCD_WR_DATA ( Xpos >> 8 );
	    LCD_WR_DATA ( Xpos & 0XFF );
	}
	else
	{
	    LCD_WR_REG ( lcddev.setxcmd );
	    LCD_WR_DATA ( Xpos >> 8 );
	    LCD_WR_DATA ( Xpos & 0XFF );
	    LCD_WR_DATA ( ( lcddev.width - 1 ) >> 8 );
	    LCD_WR_DATA ( ( lcddev.width - 1 ) & 0XFF );
	}
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( Ypos >> 8 );
	LCD_WR_DATA ( Ypos & 0XFF );
	LCD_WR_DATA ( ( lcddev.height - 1 ) >> 8 );
	LCD_WR_DATA ( ( lcddev.height - 1 ) & 0XFF );

    }
    else if ( lcddev.id == 0X5510 )
    {
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( Xpos >> 8 );
	LCD_WR_REG ( lcddev.setxcmd + 1 );
	LCD_WR_DATA ( Xpos & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( Ypos >> 8 );
	LCD_WR_REG ( lcddev.setycmd + 1 );
	LCD_WR_DATA ( Ypos & 0XFF );
    }
    else
    {
	if ( lcddev.dir == 1 ) Xpos = lcddev.width - 1 - Xpos; //横屏其实就是调转x,y坐标
	LCD_WriteReg ( lcddev.setxcmd, Xpos );
	LCD_WriteReg ( lcddev.setycmd, Ypos );
    }
}

//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint ( u16 x, u16 y, u16 color )
{
    if ( lcddev.id == 0X9341 || lcddev.id == 0X5310 )
    {
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( x >> 8 );
	LCD_WR_DATA ( x & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( y >> 8 );
	LCD_WR_DATA ( y & 0XFF );
    }
    else if ( lcddev.id == 0X5510 )
    {
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( x >> 8 );
	LCD_WR_REG ( lcddev.setxcmd + 1 );
	LCD_WR_DATA ( x & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( y >> 8 );
	LCD_WR_REG ( lcddev.setycmd + 1 );
	LCD_WR_DATA ( y & 0XFF );
    }
    else if ( lcddev.id == 0X1963 )
    {
	if ( lcddev.dir == 0 ) x = lcddev.width - 1 - x;
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( x >> 8 );
	LCD_WR_DATA ( x & 0XFF );
	LCD_WR_DATA ( x >> 8 );
	LCD_WR_DATA ( x & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( y >> 8 );
	LCD_WR_DATA ( y & 0XFF );
	LCD_WR_DATA ( y >> 8 );
	LCD_WR_DATA ( y & 0XFF );
    }
    else if ( lcddev.id == 0X6804 )
    {
	if ( lcddev.dir == 1 ) x = lcddev.width - 1 - x; //横屏时处理
	LCD_WR_REG ( lcddev.setxcmd );
	LCD_WR_DATA ( x >> 8 );
	LCD_WR_DATA ( x & 0XFF );
	LCD_WR_REG ( lcddev.setycmd );
	LCD_WR_DATA ( y >> 8 );
	LCD_WR_DATA ( y & 0XFF );
    }
    else
    {
	if ( lcddev.dir == 1 ) x = lcddev.width - 1 - x; //横屏其实就是调转x,y坐标
	LCD_WriteReg ( lcddev.setxcmd, x );
	LCD_WriteReg ( lcddev.setycmd, y );
    }
    LCD->LCD_REG = lcddev.wramcmd;
    LCD->LCD_RAM = color;
}
//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"
//size:字体大小 12/16/24
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar ( u16 x, u16 y, u8 num, u8 size, u8 mode )
{
    u8 temp, t1, t;
    u16 y0 = y;
    u8 csize = ( size / 8 + ( ( size % 8 ) ? 1 : 0 ) ) * ( size / 2 );		//得到字体一个字符对应点阵集所占的字节数
    num = num - ' '; //得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
    for ( t = 0; t < csize; t++ )
    {
	if ( size == 12 ) temp = asc2_1206[num][t]; 	 	//调用1206字体
	else if ( size == 16 ) temp = asc2_1608[num][t];	//调用1608字体
	else if ( size == 24 ) temp = asc2_2412[num][t];	//调用2412字体
	else return;								//没有的字库
	for ( t1 = 0; t1 < 8; t1++ )
	{
	    if ( temp & 0x80 ) LCD_Fast_DrawPoint ( x, y, POINT_COLOR );
	    else if ( mode == 0 ) LCD_Fast_DrawPoint ( x, y, BACK_COLOR );
	    temp <<= 1;
	    y++;
	    if ( y >= lcddev.height ) return;		//超区域了
	    if ( ( y - y0 ) == size )
	    {
		y = y0;
		x++;
		if ( x >= lcddev.width ) return;	//超区域了
		break;
	    }
	}
    }
}
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow ( u8 m, u8 n )
{
    u32 result = 1;
    while ( n-- ) result *= m;
    return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//color:颜色
//num:数值(0~4294967295);
void LCD_ShowNum ( u16 x, u16 y, u32 num, u8 len, u8 size )
{
    u8 t, temp;
    u8 enshow = 0;
    for ( t = 0; t < len; t++ )
    {
	temp = ( num / LCD_Pow ( 10, len - t - 1 ) ) % 10;
	if ( enshow == 0 && t < ( len - 1 ) )
	{
	    if ( temp == 0 )
	    {
		LCD_ShowChar ( x + ( size / 2 ) *t, y, ' ', size, 0 );
		continue;
	    }
	    else enshow = 1;

	}
	LCD_ShowChar ( x + ( size / 2 ) *t, y, temp + '0', size, 0 );
    }
}
//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum ( u16 x, u16 y, u32 num, u8 len, u8 size, u8 mode )
{
    u8 t, temp;
    u8 enshow = 0;
    for ( t = 0; t < len; t++ )
    {
	temp = ( num / LCD_Pow ( 10, len - t - 1 ) ) % 10;
	if ( enshow == 0 && t < ( len - 1 ) )
	{
	    if ( temp == 0 )
	    {
		if ( mode & 0X80 ) LCD_ShowChar ( x + ( size / 2 ) *t, y, '0', size, mode & 0X01 );
		else LCD_ShowChar ( x + ( size / 2 ) *t, y, ' ', size, mode & 0X01 );
		continue;
	    }
	    else enshow = 1;

	}
	LCD_ShowChar ( x + ( size / 2 ) *t, y, temp + '0', size, mode & 0X01 );
    }
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小
//size:字体大小
//*p:字符串起始地址
void LCD_ShowString ( u16 x, u16 y, u16 width, u16 height, u8 size, u8 *p )
{
    u8 x0 = x;
    width += x;
    height += y;
    while ( ( *p <= '~' ) && ( *p >= ' ' ) ) //判断是不是非法字符!
    {
	if ( x >= width )
	{
	    x = x0;
	    y += size;
	}
	if ( y >= height ) break; //退出
	LCD_ShowChar ( x, y, *p, size, 0 );
	x += size / 2;
	p++;
    }
}
