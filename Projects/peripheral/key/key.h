#ifndef __KEY_H__
#define __KEY_H__

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "gd32l23x.h"

/* C binding of definitions if building with C++ compiler */
#ifdef __cplusplus
extern "C"
{
#endif
/*******************************************************************************
 * Global type definitions ('typedef')
 ******************************************************************************/

/*****************************************************************************/
/* Global pre-processor symbols/macros ('define')                            */
/*****************************************************************************/
/* Port/Pin definition */
#define WAKEUP_KEY_RCU              RCU_GPIOA

#define WAKEUP_KEY_GPIO_PORT        GPIOA

#define WAKEUP_KEY_GPIO_PIN         GPIO_PIN_0
#define WAKEUP_KEY_STA()            gpio_input_bit_get(WAKEUP_KEY_GPIO_PORT, GPIO_PIN_0)

#define WAKEUP_KEY_EXTI_LINE               EXTI_0
#define WAKEUP_KEY_EXTI_PORT_SOURCE        EXTI_SOURCE_GPIOA
#define WAKEUP_KEY_EXTI_PIN_SOURCE         EXTI_SOURCE_PIN0
#define WAKEUP_KEY_EXTI_IRQn               EXTI0_IRQn

void keyInit(void);

#ifdef __cplusplus
}
#endif

#endif /* __KEY_H__ */