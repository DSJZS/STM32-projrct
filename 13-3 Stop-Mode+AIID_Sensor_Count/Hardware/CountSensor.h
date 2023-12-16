#ifndef __COUNT_SENSOR_H__
#define __COUNT_SENSOR_H__

#include "stm32f10x.h"                  // Device header

void CountSensor_Init(void);
uint16_t CountSensor_Get(void);

#endif
