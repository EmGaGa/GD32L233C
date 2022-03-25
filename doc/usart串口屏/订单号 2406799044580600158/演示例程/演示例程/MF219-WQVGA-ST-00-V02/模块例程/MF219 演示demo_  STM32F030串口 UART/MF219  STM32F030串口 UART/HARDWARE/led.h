#ifndef __LED_H
#define __LED_H

//===================================================================================================
/* 函数: 
   描述: 命令调用
   注意事项：
   申明：深圳尚视界科技有限公司   （2008-2020 版权所有，盗版必究）
	 公司网站： www.sun-lcm.com
	 淘宝网站： https://shop151604432.taobao.com/index.htm?spm=a1z10.5-c.w5002-14603162597.2.4c2619d6w9oYgh
	 技术支持:QQ:3085638545
 */ 
//====================================================================================================




#include "stm32f0xx.h"


#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOA 
#define LED_PORT   	   GPIOA
#define LED_PIN        GPIO_Pin_4

void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);

#endif
