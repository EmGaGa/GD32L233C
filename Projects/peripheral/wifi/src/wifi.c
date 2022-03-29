#include "wifi.h"
#include "log_cfg.h"

#define WIFI_COM_PER                    (USART1)
#define WIFI_COM_BAUD                   (115200UL)
#define WIFI_USART_GPIO_PORT            (GPIOA)
#define WIFI_USART_TX_PIN               (GPIO_PIN_2)
#define WIFI_USART_RX_PIN               (GPIO_PIN_3)

#define WIFI_COM_CLK                    (RCU_USART1)
#define WIFI_COM_GPIO_CLK               (RCU_GPIOA)
#define WIFI_COM_AF                     (GPIO_AF_7)

#define WIFI_RXDMACH                    (DMA_CH1)
#define WIFI_TXDMACH                    (DMA_CH2)

#define USART1_RDATA_ADDRESS            (&USART_RDATA(USART1))
#define USART1_TDATA_ADDRESS            (&USART_TDATA(USART1))
#define ARRAYNUM(arr_nanme)             (uint32_t)(sizeof(arr_nanme) / sizeof(*(arr_nanme)))

#define USART_FIFO_LEN                  (256)

uint8_t g_ucRecvBuf_FIFO1[USART_FIFO_LEN] = {0};
uint16_t g_ucRecvCnt_FIFO1 = 0;

static void DMAInit(void);


/**
 * @brief DMA INIT
 * 
 */
static void DMAInit(void)
{
    dma_parameter_struct dma_init_struct;
    /* enable DMA clock */
    rcu_periph_clock_enable(RCU_DMA);

    /* initialize DMA channel 1 */
    dma_deinit(WIFI_RXDMACH);
    dma_struct_para_init(&dma_init_struct);
    dma_init_struct.request      = DMA_REQUEST_USART1_RX;
    dma_init_struct.direction    = DMA_PERIPHERAL_TO_MEMORY;
    dma_init_struct.memory_addr  = (uint32_t)g_ucRecvBuf_FIFO1;
    dma_init_struct.memory_inc   = DMA_MEMORY_INCREASE_ENABLE;
    dma_init_struct.memory_width = DMA_MEMORY_WIDTH_8BIT;
    dma_init_struct.number       = ARRAYNUM(g_ucRecvBuf_FIFO1);
    dma_init_struct.periph_addr  = (uint32_t)USART1_RDATA_ADDRESS;
    dma_init_struct.periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    dma_init_struct.periph_width = DMA_PERIPHERAL_WIDTH_8BIT;
    dma_init_struct.priority     = DMA_PRIORITY_ULTRA_HIGH;
    dma_init(WIFI_RXDMACH, &dma_init_struct);

    /* configure DMA mode */
    dma_circulation_disable(WIFI_RXDMACH);
    dma_memory_to_memory_disable(WIFI_RXDMACH);
    /* disable the DMAMUX_MUXCH1 synchronization mode */
    dmamux_synchronization_disable(DMAMUX_MUXCH1);
    /* USART DMA enable for reception */
    usart_dma_receive_config(WIFI_COM_PER, USART_DENR_ENABLE);
    /* enable DMA channel 1 */
    dma_channel_enable(WIFI_RXDMACH);
}

/*!
    \brief      this function handles USART1 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART1_IRQHandler(void)
{
    uint8_t i;
    if(RESET != usart_interrupt_flag_get(USART1, USART_INT_FLAG_IDLE)) 
    {
        usart_interrupt_flag_clear(USART1, USART_INT_FLAG_IDLE);

        /* disable DMA channel */
        dma_channel_disable(WIFI_RXDMACH);
        
        /* read one byte from the receive data register */
        g_ucRecvCnt_FIFO1 = ARRAYNUM(g_ucRecvBuf_FIFO1) - dma_transfer_number_get(WIFI_RXDMACH);

        PRO_LOG(LOG_DEBUG, "g_ucRecvCnt_FIFO1: %d. \r\n", g_ucRecvCnt_FIFO1);

        for(i = 0; i < g_ucRecvCnt_FIFO1; i++)
        {
            PRO_LOG(LOG_DEBUG, "g_ucRecvBuf_FIFO1[%d]: 0x%x. \r\n", i, g_ucRecvBuf_FIFO1[i]);
        }
        /* set DMA memory base address */
        dma_memory_address_config(WIFI_RXDMACH, (uint32_t)g_ucRecvBuf_FIFO1);

        /* set the number of remaining data to be transferred by the DMA */
        dma_transfer_number_config(WIFI_RXDMACH, ARRAYNUM(g_ucRecvBuf_FIFO1));

        /* enable DMA channel */
        dma_channel_enable(WIFI_RXDMACH);
    }
}

/**
 * @brief Uart Init
 * 
 */
void USART1Init(void)
{
    /* enable COM GPIO clock */
    rcu_periph_clock_enable(WIFI_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(WIFI_COM_CLK);

    /* usart deinit */
    usart_deinit(WIFI_COM_PER);

    /* USART interrupt configuration */
    nvic_irq_enable(USART1_IRQn, 0);

    /* enable USART1 receive interrupt */
    usart_interrupt_enable(WIFI_COM_PER, USART_INT_IDLE);

    /* connect port to USARTx_Tx */
    gpio_af_set(WIFI_USART_GPIO_PORT, WIFI_COM_AF, WIFI_USART_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_af_set(WIFI_USART_GPIO_PORT, WIFI_COM_AF, WIFI_USART_RX_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(WIFI_USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, WIFI_USART_TX_PIN);
    gpio_output_options_set(WIFI_USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, WIFI_USART_TX_PIN);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(WIFI_USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, WIFI_USART_RX_PIN);
    gpio_output_options_set(WIFI_USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, WIFI_USART_RX_PIN);

    /* USART configure */
    usart_baudrate_set(WIFI_COM_PER, WIFI_COM_BAUD);
    usart_receive_config(WIFI_COM_PER, USART_RECEIVE_ENABLE);
    usart_transmit_config(WIFI_COM_PER, USART_TRANSMIT_ENABLE);
    usart_enable(WIFI_COM_PER);

    /* DMA init */
    DMAInit();
}
