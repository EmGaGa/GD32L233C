#include "header.h"


int fputc(int ch, FILE *f)
{
    //等待先前数据传输到移位寄存器
    while(!usart_flag_get(DEBUG_COM_PER, USART_FLAG_TBE));
    //发送字符
    usart_data_transmit(DEBUG_COM_PER, ch);
    return ch;
}

void usartInit(uint32_t usart_periph, uint32_t baud)
{
    uint32_t com_id = 0U;

    /* enable COM GPIO clock */
    rcu_periph_clock_enable(DEBUG_COM_GPIO_CLK);

    /* enable USART clock */
    rcu_periph_clock_enable(DEBUG_COM_CLK);

    /* connect port to USARTx_Tx */
    gpio_af_set(DEBUG_USART_GPIO_PORT, DEBUG_COM_AF, DEBUG_USART_TX_PIN);

    /* connect port to USARTx_Rx */
    gpio_af_set(DEBUG_USART_GPIO_PORT, DEBUG_COM_AF, DEBUG_USART_RX_PIN);

    /* configure USART Tx as alternate function push-pull */
    gpio_mode_set(DEBUG_USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, DEBUG_USART_TX_PIN);
    gpio_output_options_set(DEBUG_USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, DEBUG_USART_TX_PIN);

    /* configure USART Rx as alternate function push-pull */
    gpio_mode_set(DEBUG_USART_GPIO_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLUP, DEBUG_USART_RX_PIN);
    gpio_output_options_set(DEBUG_USART_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_10MHZ, DEBUG_USART_RX_PIN);

    /* USART configure */
    usart_deinit(DEBUG_COM_PER);
    usart_baudrate_set(DEBUG_COM_PER, DEBUG_COM_BAUD);
    usart_receive_config(DEBUG_COM_PER, USART_RECEIVE_ENABLE);
    usart_transmit_config(DEBUG_COM_PER, USART_TRANSMIT_ENABLE);
    usart_enable(DEBUG_COM_PER);
}

