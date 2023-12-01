#include "MyDMA.h"
#include "stm32f10x.h"                  // Device header

static uint16_t MyDMA_Size = 0;

void MyDMA_Init( uint32_t AddrA, uint32_t AddrB, uint16_t Size)
{
	MyDMA_Size = Size;

	DMA_InitTypeDef DMA_InitStructure;
	
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE);	//	使能 DMA1 外设

	DMA_InitStructure.DMA_PeripheralBaseAddr = AddrA;	//	外设站点起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;	//	外设站点数据宽度(这里为byte)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;	//	外设站点地址是否自增(这里为自增)
	DMA_InitStructure.DMA_MemoryBaseAddr = AddrB;	//	存储器站点起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	//	存储器站点数据宽度(这里为byte)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//	存储器站点地址是否自增(这里为自增)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//	传输方向(设置外设站点为数据源)
	DMA_InitStructure.DMA_BufferSize = Size;	//	缓冲区大小(传输计时器的值)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;	//	传输模式(是否自动重装，由于这里是存储器到存储器运输，所以不重装)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;	//	触发方式(硬件或者软件，这里使用软件)
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//	通道优先级，这里为中等优先级
	DMA_Init( DMA1_Channel1, &DMA_InitStructure);	//	初始化DMA1的通道1
	
	DMA_Cmd( DMA1_Channel1, DISABLE);	//	失能DMA1的通道1
}

void MyDMA_Transfer(void)
{
	DMA_Cmd( DMA1_Channel1, DISABLE);	//	失能DMA1的通道1
	DMA_SetCurrDataCounter( DMA1_Channel1, MyDMA_Size);//	配置传输计时器的值
	DMA_Cmd( DMA1_Channel1, ENABLE);	//	使能DMA1的通道1
	while(RESET == DMA_GetFlagStatus(DMA1_FLAG_TC1));	//	根据标志位，等待转运完成
	DMA_ClearFlag(DMA1_FLAG_TC1);//	清除标志位
}
