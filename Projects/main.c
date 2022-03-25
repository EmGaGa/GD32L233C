/*!
    \file    main.c
    \brief   running led

    \version 2021-11-15, V1.0.0, demo for GD32L23x
*/

/*
    Copyright (c) 2021, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "header.h"

static void MainInit(void);


dev_handler_str dev_ctrl_s = {
    .page = 0,
    .ledCtrlColor = 0,
    .radio_freq = 0,
};

static void MainInit(void)
{
	/* led init */
	led_init();

    /* key init */
    keyInit();
	
    /* 旋转电位器初始化 */
	rotationSensorInit();

	WS2812C_Init();
}

static struct rt_thread led_thread;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t rt_led_thread_stack[512];
static void led_thread_entry(void * para);
static rt_uint8_t led_thread_priority = 10;

static struct rt_thread app_thread;
ALIGN(RT_ALIGN_SIZE)
static rt_uint8_t app_thread_stack[1024];
static void app_thread_entry(void * para);
static rt_uint8_t app_thread_priority = 5;

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
//    PRO_LOG(LOG_DEBUG, "Entry %s. \r\n", __func__);
    MainInit();

    /* 创建所有任务 */
	rt_thread_init(&app_thread,
					"app_thread",
					app_thread_entry,
					RT_NULL,
					&app_thread_stack,
					sizeof(app_thread_stack),
					app_thread_priority,
					1000);
	rt_thread_startup(&app_thread);

 	/* led thread */
	rt_thread_init(&led_thread,
					"led_thread",
					led_thread_entry,
					RT_NULL,
					&rt_led_thread_stack,
					sizeof(rt_led_thread_stack),
					led_thread_priority,
					1000);
	rt_thread_startup(&led_thread);
}

static void app_thread_entry(void * para)
{
    PRO_LOG(LOG_DEBUG, "Entry %s. \r\n", __func__);
    uint16_t newVal = 0;
    uint16_t oldVal = 0;
    uint32_t rgbval;

    while(1)
    {
        newVal = getRotaSensorVar();

        if(newVal != oldVal)
        {
            oldVal = newVal;
            dev_ctrl_s.rgbColor[dev_ctrl_s.ledCtrlColor] = newVal;
            rgbval = RGB888CONVERT(dev_ctrl_s.rgbColor[0], dev_ctrl_s.rgbColor[1], dev_ctrl_s.rgbColor[2]);

            ws2812c_All_Ctrl(rgbval);
        }
		rt_thread_delay(10);
    }
}

static void led_thread_entry(void * para)
{
    PRO_LOG(LOG_DEBUG, "Entry %s. \r\n", __func__);
    while(1)
    {
        LED1_TOGGLE();
        LED2_TOGGLE();
    }
}