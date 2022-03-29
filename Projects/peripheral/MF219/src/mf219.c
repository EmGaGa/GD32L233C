#include "mf219.h"

#define LCD_COM_PER                   UART3
#define LCD_COM_BAUD                  (115200UL)
#define LCD_USART_GPIO_PORT           GPIOC
#define LCD_USART_TX_PIN              GPIO_PIN_10
#define LCD_USART_RX_PIN              GPIO_PIN_11

#define LCD_COM_CLK                   RCU_UART3
#define LCD_COM_GPIO_CLK              RCU_GPIOC
#define LCD_COM_AF                    GPIO_AF_7




