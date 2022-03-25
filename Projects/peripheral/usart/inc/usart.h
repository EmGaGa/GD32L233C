#ifndef __USART_H_
#define __USART_H_

#include "gd32l23x.h"

#define DEBUG_COM_PER                   USART0
#define DEBUG_COM_BAUD                  (115200UL)
#define DEBUG_USART_GPIO_PORT           GPIOA
#define DEBUG_USART_TX_PIN              GPIO_PIN_9
#define DEBUG_USART_RX_PIN              GPIO_PIN_10

#define DEBUG_COM_CLK                   RCU_USART0
#define DEBUG_COM_GPIO_CLK              RCU_GPIOA
#define DEBUG_COM_AF                    GPIO_AF_7

void usartInit(uint32_t usart_periph, uint32_t baud);

#endif
