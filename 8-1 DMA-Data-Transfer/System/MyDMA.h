#ifndef __MYDMA_H__
#define __MYDMA_H__

#include "stm32f10x.h"                  // Device header

void MyDMA_Init( uint32_t AddrA, uint32_t AddrB, uint16_t Size);
void MyDMA_Transfer(void);

#endif
