#include "header.h"

void led_init(void)
{
    /* enable the LED GPIO clock */
    rcu_periph_clock_enable(LED12_RCU);
    rcu_periph_clock_enable(LED34_RCU);
    /* configure LED GPIO pin */
    gpio_mode_set(LED12_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED1_GPIO_PIN | LED2_GPIO_PIN);
    gpio_output_options_set(LED12_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED1_GPIO_PIN | LED2_GPIO_PIN);
    gpio_mode_set(LED34_GPIO_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED3_GPIO_PIN | LED4_GPIO_PIN);
    gpio_output_options_set(LED34_GPIO_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED3_GPIO_PIN | LED4_GPIO_PIN);
    /* reset LED GPIO pin */
    gpio_bit_reset(LED12_GPIO_PORT, LED1_GPIO_PIN | LED2_GPIO_PIN);
    gpio_bit_reset(LED34_GPIO_PORT, LED3_GPIO_PIN | LED4_GPIO_PIN);
}