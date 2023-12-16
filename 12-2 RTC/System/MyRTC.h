#ifndef __MYRTC_H__
#define __MYRTC_H__

#include "stm32f10x.h"                  // Device header

extern uint16_t MyRTC_Time[];

void MyRTC_Init(void);
void MyRTC_SetTime(void);
void MyRTC_ReadTime(void);

#endif
