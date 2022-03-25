#include "header.h"

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
#define SPI1_MOSI_RCU            		(RCU_GPIOC)

#define WS2812_RCU_PERIPH				(RCU_SPI1)
#define WS2812_PERIPH					(SPI1)

#define WS2812_MOSI_PORT          		(GPIOC)
#define WS2812_MOSI_PIN           		(GPIO_PIN_12)

#define WS2812_LOW						(0xC0)
#define WS2812_HIGH						(0xF0)

static uint8_t u8TxBuffer[24] = {0};
static uint16_t u16BufferCnt = 0;

static uint32_t color_group[WS2812C_LED_NUM] = {LED_COLOR_BLUE, LED_COLOR_RED, LED_COLOR_GREEN, LED_COLOR_YELLOW ,\
                                                LED_COLOR_CYAN, LED_COLOR_PURPLE, LED_COLOR_WHITE, LED_COLOR_OFF};

static void RGB_Set_Down(void);
static void RGB_Set_Up(void);

static void RGB_Set_Up(void)
{
	u8TxBuffer[u16BufferCnt] = WS2812_HIGH;
    u16BufferCnt++;
}

static void RGB_Set_Down(void)
{
	u8TxBuffer[u16BufferCnt] = WS2812_LOW;
    u16BufferCnt++;
}

/**
 *******************************************************************************
 ** \brief Configure SPI DMA function
 **
 ** \param [in] None
 **
 ** \retval None
 **
 ******************************************************************************/
static void Spi_DmaConfig(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

    /* initialize DMA channel 0 */
    dma_deinit(DMA_CH0);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.request      = DMA_REQUEST_SPI1_TX;
    dma_init_struct.direction    = DMA_MEMORY_TO_PERIPHERAL;
    dma_init_struct.memory_addr  = (uint32_t)u8TxBuffer;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number       = 24;
    dma_init_struct.periph_addr  = (uint32_t)&SPI_DATA(WS2812_PERIPH);
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(DMA_CH0, &dma_init_struct);
}

void WS2812C_Init(void)
{
    spi_parameter_struct spi_init_struct;

    rcu_periph_clock_enable(SPI1_MOSI_RCU);
    rcu_periph_clock_enable(WS2812_RCU_PERIPH);

    /* SPI1_MOSI(PC12) GPIO pin configuration */
    gpio_af_set(WS2812_MOSI_PORT, GPIO_AF_5, WS2812_MOSI_PIN);
    gpio_mode_set(WS2812_MOSI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, WS2812_MOSI_PIN);
    gpio_output_options_set(WS2812_MOSI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, WS2812_MOSI_PIN);

    spi_i2s_deinit(WS2812_PERIPH);

    /* SPI1 parameter config */
    spi_init_struct.trans_mode           = SPI_TRANSMODE_FULLDUPLEX;
    spi_init_struct.device_mode          = SPI_MASTER;
    spi_init_struct.frame_size           = SPI_FRAMESIZE_8BIT;
    spi_init_struct.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;
    spi_init_struct.nss                  = SPI_NSS_SOFT;		//CS脚由软件托管
    spi_init_struct.prescale             = SPI_PSC_4;
    spi_init_struct.endian               = SPI_ENDIAN_MSB;
    spi_init(WS2812_PERIPH, &spi_init_struct);

    /* configure SPI1 byte access to FIFO */
    spi_fifo_access_size_config(WS2812_PERIPH, SPI_BYTE_ACCESS);

    spi_dma_enable(WS2812_PERIPH, SPI_DMA_TRANSMIT);

    /* enable SPI1 */
    spi_enable(WS2812_PERIPH);

    Spi_DmaConfig();
}


void WS2812C_SetRGB(uint32_t RGB888)
{
	int8_t i = 0;
	uint8_t byte = 0;
	
    u16BufferCnt = 0;

	for(i = 23; i >= 0; i--)
	{
		byte = ((RGB888>>i)&0x01);
		if(byte == 1)
		{
			RGB_Set_Up();
		}
		else
		{
			RGB_Set_Down();
		}
	} 

    
    dma_transfer_number_config(DMA_CH0, 24);
    dma_channel_enable(DMA_CH0);

	while (RESET == dma_flag_get(DMA_CH0, DMA_FLAG_FTF))
	{
	}
	dma_flag_clear(DMA_CH0, DMA_FLAG_FTF);
    dma_channel_disable(DMA_CH0);
}

void WS2812C_Reset(void)
{
	RGB_Set_Down();
	delay_1ms(500);
}

void Led_Cycle_Run(void)
{
	uint8_t i = 0;
	for(i = 0; i < WS2812C_LED_NUM; i++)
	{
		WS2812C_SetRGB(color_group[i]);
	}
}

void ws2812c_All_Ctrl(uint32_t RGB888)
{
	uint8_t i = 0;
	for(i = 0; i < WS2812C_LED_NUM; i++)
	{
		WS2812C_SetRGB(RGB888);
	}
}

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
