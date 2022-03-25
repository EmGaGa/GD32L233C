#include "timer2.h"

void timer2Init(void)
{
    /* TIMER2 configuration: generate PWM signals with different duty cycles:
       TIMER2CLK = SystemCoreClock / 64 = 1MHz */
    timer_oc_parameter_struct timer_ocintpara;
    timer_parameter_struct timer_initpara;

    rcu_periph_clock_enable(RCU_TIMER2);
    timer_deinit(TIMER2);

    /* TIMER2 configuration */
    timer_struct_para_init(&timer_initpara);
    timer_initpara.prescaler         = 63;
    timer_initpara.alignedmode       = TIMER_COUNTER_EDGE;
    timer_initpara.counterdirection  = TIMER_COUNTER_UP;
    timer_initpara.period            = 1;
    timer_initpara.clockdivision     = TIMER_CKDIV_DIV1;
    timer_init(TIMER2, &timer_initpara);

    /* auto-reload preload enable */
    timer_auto_reload_shadow_enable(TIMER2);

    /* TIMER disable */
    timer_disable(TIMER2);
}

void delay_us(uint32_t us_cnt)
{
    timer_enable(TIMER2);

    TIMER_CTL0(TIMER2) |= TIMER_CTL0_CEN;
    while(us_cnt--){
        while(timer_flag_get(TIMER2, TIMER_FLAG_UP) == RESET);
        timer_flag_clear(TIMER2, TIMER_FLAG_UP);
    }
	TIMER_CTL0(TIMER2) &= ~TIMER_CTL0_CEN;

    timer_disable(TIMER2);
}
