#include "header.h"

extern dev_handler_str dev_ctrl_s;

void keyInit(void)
{
    /* enable the KEY GPIO clock */
    rcu_periph_clock_enable(WAKEUP_KEY_RCU);
    /* configure KEY GPIO pin */
    gpio_mode_set(WAKEUP_KEY_GPIO_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, WAKEUP_KEY_GPIO_PIN);
    /* enable and set key EXTI interrupt priority */
    nvic_irq_enable(EXTI0_IRQn, 2U);
    /* connect key EXTI line to key GPIO pin */
    syscfg_exti_line_config(WAKEUP_KEY_EXTI_PORT_SOURCE, WAKEUP_KEY_EXTI_PIN_SOURCE);
    /* configure key EXTI line */
    exti_init(WAKEUP_KEY_EXTI_LINE, EXTI_INTERRUPT, EXTI_TRIG_RISING);
    exti_interrupt_flag_clear(WAKEUP_KEY_EXTI_LINE);
}

/*!
    \brief      this function handles external lines 0 interrupt request
    \param[in]  none
    \param[out] none
    \retval     none
*/
void EXTI0_IRQHandler(void)
{
    if(RESET != exti_interrupt_flag_get(EXTI_0)) {
        dev_ctrl_s.ledCtrlColor++;
        dev_ctrl_s.ledCtrlColor = dev_ctrl_s.ledCtrlColor%3;
        exti_interrupt_flag_clear(EXTI_0);
    }
}