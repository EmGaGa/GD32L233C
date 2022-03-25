#ifndef __ROTARY_H_
#define __ROTARY_H_

#include "gd32l23x.h"

#define ROTATION_SENSOR_GPIO_RCU        (RCU_GPIOB)

#define ROTATION_SENSOR_GPIO_PORT       (GPIOB)
#define ROTATION_SENSOR_GPIO_PIN        (GPIO_PIN_0)

#define ROTATION_ADC_CH                 (ADC_CHANNEL_8)

void rotationSensorInit(void);
uint16_t getRotaSensorVar(void);

#endif
