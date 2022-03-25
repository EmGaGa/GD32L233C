#ifndef __LED_H_
#define __LED_H_

#include "gd32l23x.h"

#define LED12_RCU           RCU_GPIOA
#define LED34_RCU           RCU_GPIOC

#define LED12_GPIO_PORT     GPIOA
#define LED34_GPIO_PORT     GPIOC

#define LED1_GPIO_PIN       GPIO_PIN_7
#define LED1_GPIO_SET()     gpio_bit_set(LED12_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_GPIO_CLR()     gpio_bit_reset(LED12_GPIO_PORT, LED1_GPIO_PIN)
#define LED1_TOGGLE()       gpio_bit_toggle(LED12_GPIO_PORT, LED1_GPIO_PIN)

#define LED2_GPIO_PIN       GPIO_PIN_8
#define LED2_GPIO_SET()     gpio_bit_set(LED12_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_GPIO_CLR()     gpio_bit_reset(LED12_GPIO_PORT, LED2_GPIO_PIN)
#define LED2_TOGGLE()       gpio_bit_toggle(LED12_GPIO_PORT, LED2_GPIO_PIN)

#define LED3_GPIO_PIN       GPIO_PIN_6
#define LED3_GPIO_SET()     gpio_bit_set(LED34_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_GPIO_CLR()     gpio_bit_reset(LED34_GPIO_PORT, LED3_GPIO_PIN)
#define LED3_TOGGLE()       gpio_bit_toggle(LED34_GPIO_PORT, LED3_GPIO_PIN)

#define LED4_GPIO_PIN       GPIO_PIN_7
#define LED4_GPIO_SET()     gpio_bit_set(LED34_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_GPIO_CLR()     gpio_bit_reset(LED34_GPIO_PORT, LED4_GPIO_PIN)
#define LED4_TOGGLE()       gpio_bit_toggle(LED34_GPIO_PORT, LED4_GPIO_PIN)

void led_init(void);

#endif
