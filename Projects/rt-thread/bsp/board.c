/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2009-01-05     Bernard      first implementation
 */
#include <stdint.h>
#include <rthw.h>
#include <rtthread.h>
#include "gd32l23x.h"
#include "usart.h"

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler */
    /* User can add his own implementation to report the HAL error return state */
    while (1)
    {
    }
    /* USER CODE END Error_Handler */
}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if(SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND)) {
        /* capture error */
        while(1) {
        }
    }
    /* configure the systick handler priority */
    NVIC_SetPriority(SysTick_IRQn, 0x00U);
    
    #if 0
    SysTick_Config(SystemCoreClock / RT_TICK_PER_SECOND);
    NVIC_SetPriority(SysTick_IRQn, 0);
    #endif
}

/**
 * This is the timer interrupt service routine.
 *
 */
void SysTick_Handler(void)
{
    /* enter interrupt */
    rt_interrupt_enter();

    rt_tick_increase();

    /* leave interrupt */
    rt_interrupt_leave();

    delay_decrement();
}

/**
 * This function will initial GD32 board.
 */
void rt_hw_board_init(void)
{
    #if 0       //TODO:中断向量表，后续新增bootloader功能，在这里更改
    /* NVIC Configuration */
#define NVIC_VTOR_MASK              0x3FFFFF80
#ifdef  VECT_TAB_RAM
    /* Set the Vector Table base location at 0x10000000 */
    SCB->VTOR  = (0x10000000 & NVIC_VTOR_MASK);
#else  /* VECT_TAB_FLASH  */
    /* Set the Vector Table base location at 0x08000000 */
    SCB->VTOR  = (0x08000000 & NVIC_VTOR_MASK);
#endif
    #endif

    SystemClock_Config();

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif

#if defined(RT_USING_USER_MAIN) && defined(RT_USING_HEAP)
    rt_system_heap_init((void*)HEAP_BEGIN, (void*)HEAP_END);
#endif

    usartInit(DEBUG_COM_PER, DEBUG_COM_BAUD);
}

void rt_hw_console_output(const char *str)
{
    rt_size_t i = 0, size = 0;
    char a = '\r';


    size = rt_strlen(str);

    for (i = 0; i < size; i++)
    {
        if (*(str + i) == '\n')
        {
            usart_data_transmit(DEBUG_COM_PER, (uint32_t )a);
            while((usart_flag_get(DEBUG_COM_PER, USART_FLAG_TC) == RESET));
        }
        usart_data_transmit(DEBUG_COM_PER, (uint32_t)*(str + i));
        while((usart_flag_get(DEBUG_COM_PER, USART_FLAG_TC) == RESET));
    }
}

char rt_hw_console_getchar(void)
{
    /* the initial value of ch must < 0 */
    int ch = -1;

    if (usart_flag_get(DEBUG_COM_PER, USART_FLAG_RBNE) != RESET)
    {
        ch = usart_data_receive(DEBUG_COM_PER);
    }
    else
    {
        rt_thread_mdelay(10);
    }
    return ch;
}



