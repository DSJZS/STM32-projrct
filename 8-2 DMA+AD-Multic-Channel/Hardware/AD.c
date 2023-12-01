#include "AD.h"
#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4] = {0};

/**
  *@brief		初始化ADC
  *@param		无
  *@retval		无
  **/
void AD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);	//	开始ADC1的时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	开始GPIOA的时钟
	RCC_AHBPeriphClockCmd( RCC_AHBPeriph_DMA1, ENABLE);	//	使能 DMA1 外设
	
	RCC_ADCCLKConfig( RCC_PCLK2_Div6);	//	配置ADC分频器为6分频，72MHz/6=12MHz
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//	GPIO口选择模拟输入模式，防止GPIO输入输出造成影响
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//开启GPIOA0-3
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig( ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//	选择规则组输入通道
	ADC_RegularChannelConfig( ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);	//	选择规则组输入通道
	ADC_RegularChannelConfig( ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);	//	选择规则组输入通道
	ADC_RegularChannelConfig( ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);	//	选择规则组输入通道
							//	在ADC1的规则组选择通道ADC_Channel0-3，采样时间为55.5个ADCCLK周期
	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//	指定数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//	配置外部触发源
										//	此处选择不外部触发，纯由软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//	配置ADC工作模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//	是否连续转换，此处连续
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//	是否开启扫码模式，此处扫描
	ADC_InitStructure.ADC_NbrOfChannel = 4;		//	扫描模式下的通道数目
	ADC_Init( ADC1, &ADC_InitStructure);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC1->DR);	//	外设站点起始地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//	外设站点数据宽度(这里为byte)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//	外设站点地址是否自增(这里为不自增)
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;	//	存储器站点起始地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//	存储器站点数据宽度(这里为byte)
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//	存储器站点地址是否自增(这里为自增)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//	传输方向(设置外设站点为数据源)
	DMA_InitStructure.DMA_BufferSize = 4;	//	缓冲区大小(传输计时器的值)
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//	传输模式(是否自动重装，这里重装，也就是循环模式)
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	//	触发方式(硬件或者软件，这里不使用软件)
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//	通道优先级，这里为中等优先级
	DMA_Init( DMA1_Channel1, &DMA_InitStructure);	//	初始化DMA1的通道1
	
	DMA_Cmd( DMA1_Channel1, ENABLE);	//	使能DMA1的通道1
	
	ADC_DMACmd( ADC1, ENABLE);	//	使ADC1发送DMA请求信号
	
	ADC_Cmd( ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	// 复位校准
	while(SET == ADC_GetResetCalibrationStatus(ADC1));// 获取复位校准状态，等待复位完成
	ADC_StartCalibration(ADC1);	// 开始校准
	while(SET == ADC_GetCalibrationStatus(ADC1));	// 获取开始校准状态，等待校准完成
	
	ADC_SoftwareStartConvCmd( ADC1, ENABLE);	//	软件开启ADC转换
}
