#include "lcd_init.h"
#include "rtthread.h"

#if (DRV_MODE == DRV_SPI)

void LCD_SPI_Write(uint8_t Data)
{
    /* Send data */
	spi_i2s_data_transmit(SPI_PERIPH, Data);
    
	/* Wait tx buffer empty */
	while(RESET == spi_i2s_flag_get(SPI_PERIPH, SPI_FLAG_TBE));
}

void LCD_SPI_Init(void)
{
	spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(SPI_SCK_RCU);
    rcu_periph_clock_enable(SPI_MOSI_RCU);
    rcu_periph_clock_enable(SPI_RCU_PERIPH);

    /* SPI0_CLK(PA5) GPIO pin configuration */
    gpio_af_set(SPI_SCK_PORT, GPIO_AF_5, SPI_SCK_PIN);
    gpio_mode_set(SPI_SCK_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_SCK_PIN);
    gpio_output_options_set(SPI_SCK_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_SCK_PIN);

    /* SPI0_MOSI(PB5) GPIO pin configuration */
    gpio_af_set(SPI_MOSI_PORT, GPIO_AF_5, SPI_MOSI_PIN);
    gpio_mode_set(SPI_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_MOSI_PIN);
    gpio_output_options_set(SPI_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_MOSI_PIN);

    /* SPI1 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_HIGH_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;
    spi_init_struct.prescale             = SPI_PSC_2;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(SPI_PERIPH, &spi_init_struct);

    /* configure SPI0 byte access to FIFO */
    spi_fifo_access_size_config(SPI_PERIPH, SPI_BYTE_ACCESS);

    /* set crc polynomial */
    spi_crc_polynomial_set(SPI_PERIPH, 7);

    /* quad wire SPI_IO2 and SPI_IO3 pin output enable */
    qspi_io23_output_enable(SPI_PERIPH);

    /* enable SPI0 */
    spi_enable(SPI_PERIPH);
}
#endif

void LCD_GPIO_Init(void)
{
	#if (DRV_MODE == DRV_GPIO)
    /* enable the LED GPIO clock */
    rcu_periph_clock_enable(SPI_SCK_RCU);
    rcu_periph_clock_enable(SPI_MOSI_RCU);
    /* configure LED GPIO pin */
    gpio_mode_set(SPI_GPIO_PORT1, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_SCK_PIN | RES_PIN | DC_PIN | BLK_PIN);
    gpio_output_options_set(SPI_GPIO_PORT1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_SCK_PIN | RES_PIN | DC_PIN | BLK_PIN);
    gpio_mode_set(SPI_GPIO_PORT2, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SPI_MOSI_PIN);
    gpio_output_options_set(SPI_GPIO_PORT2, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, SPI_MOSI_PIN);
	/* reset LED GPIO pin */
    gpio_bit_reset(SPI_GPIO_PORT1, SPI_SCK_PIN | RES_PIN | DC_PIN | BLK_PIN);
    gpio_bit_reset(SPI_GPIO_PORT2, SPI_MOSI_PIN);
	#elif (DRV_MODE == DRV_SPI)
	/* enable the LED GPIO clock */
    rcu_periph_clock_enable(SPI_SCK_RCU);
	/* configure LED GPIO pin */
    gpio_mode_set(SPI_GPIO_PORT1, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, RES_PIN | DC_PIN | BLK_PIN);
    gpio_output_options_set(SPI_GPIO_PORT1, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, RES_PIN | DC_PIN | BLK_PIN);
	/* reset LED GPIO pin */
    gpio_bit_reset(SPI_GPIO_PORT1, RES_PIN | DC_PIN | BLK_PIN);
	#endif
}

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(uint8_t dat) 
{	
#if (DRV_MODE == DRV_GPIO)
	uint8_t counter;
	for(counter=0; counter<8; counter++)
	{ 
		LCD_SCLK_Clr();		  
		if((dat&0x80)==0)
		{
			LCD_MOSI_Clr();
		}
		else
		{ 
			LCD_MOSI_Set();
		}
		dat=dat<<1;	
		LCD_SCLK_Set();		
	}		
	LCD_SCLK_Clr();
#elif (DRV_MODE == DRV_SPI)
	LCD_SPI_Write(dat);
#endif
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(uint8_t dat)
{
	LCD_DC_Set();
#if (DRV_MODE == DRV_GPIO)
	LCD_Writ_Bus(dat);
#elif (DRV_MODE == DRV_SPI)
	LCD_SPI_Write(dat);
#endif
}


/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(uint16_t dat)
{
#if (DRV_MODE == DRV_GPIO)	
	LCD_WR_DATA8(dat>>8);
	LCD_WR_DATA8(dat);
#elif (DRV_MODE == DRV_SPI)
	LCD_DC_Set();
	LCD_SPI_Write(dat>>8);
	LCD_SPI_Write(dat);
#endif
}


/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(uint8_t dat)
{
	LCD_DC_Clr();
#if (DRV_MODE == DRV_GPIO)	
	LCD_Writ_Bus(dat);
#elif (DRV_MODE == DRV_SPI)
	LCD_SPI_Write(dat);
#endif
}


/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{
	LCD_WR_REG(0x2a);     //Column address set
	LCD_WR_DATA(x1);    //start column
	LCD_WR_DATA(x2);    //end column

	LCD_WR_REG(0x2b);     //Row address set
	LCD_WR_DATA(y1);    //start row
	LCD_WR_DATA(y2);    //end row
	LCD_WR_REG(0x2C);     //Memory write
}

void LCD_Init(void)
{
	LCD_GPIO_Init();//初始化GPIO
#if (DRV_MODE == DRV_SPI)
	LCD_SPI_Init();
#endif

	LCD_RES_Clr();
	delay_1ms(120);
	LCD_RES_Set();
	delay_1ms(120);
#if (DRV_MODE == DRV_GPIO)
	LCD_SCLK_Set();			//特别注意！！
#endif
    LCD_WR_REG(0x11); 			//Sleep Out
	delay_1ms(120);               //DELAY120ms 
	//-----------------------ST7789V Frame rate setting-----------------//
	//************************************************
	LCD_WR_REG(0x3A);        //65k mode
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xC5); 		//VCOM
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0x36);                 // 屏幕显示方向设置
	LCD_WR_DATA8(0x00);
	//-------------ST7789V Frame rate setting-----------//
	LCD_WR_REG(0xb2);		//Porch Setting
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xb7);			//Gate Control
	LCD_WR_DATA8(0x05);			//12.2v   -10.43v
	//--------------ST7789V Power setting---------------//
	LCD_WR_REG(0xBB);//VCOM
	LCD_WR_DATA8(0x3F);

	LCD_WR_REG(0xC0); //Power control
	LCD_WR_DATA8(0x2c);

	LCD_WR_REG(0xC2);		//VDV and VRH Command Enable
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);			//VRH Set
	LCD_WR_DATA8(0x0F);		//4.3+( vcom+vcom offset+vdv)

	LCD_WR_REG(0xC4);			//VDV Set
	LCD_WR_DATA8(0x20);				//0v

	LCD_WR_REG(0xC6);				//Frame Rate Control in Normal Mode
	LCD_WR_DATA8(0X01);			//111Hz

	LCD_WR_REG(0xd0);				//Power Control 1
	LCD_WR_DATA8(0xa4);
	LCD_WR_DATA8(0xa1);

	LCD_WR_REG(0xE8);				//Power Control 1
	LCD_WR_DATA8(0x03);

	LCD_WR_REG(0xE9);				//Equalize time control
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	//---------------ST7789V gamma setting-------------//
	LCD_WR_REG(0xE0); //Set Gamma
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x3F);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x30);
		
	LCD_WR_REG(0XE1); //Set Gamma
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x24);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x32);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x13);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x2F);

	LCD_WR_REG(0x21); 		//反显
	
	LCD_WR_REG(0x29);         //开启显示 
} 
