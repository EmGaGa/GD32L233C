#ifndef __LED_H
#define __LED_H

//===================================================================================================
/* ����: 
   ����: �������
   ע�����
   �������������ӽ�Ƽ����޹�˾   ��2008-2020 ��Ȩ���У�����ؾ���
	 ��˾��վ�� www.sun-lcm.com
	 �Ա���վ�� https://shop151604432.taobao.com/index.htm?spm=a1z10.5-c.w5002-14603162597.2.4c2619d6w9oYgh
	 ����֧��:�QQQ:3085638545
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
