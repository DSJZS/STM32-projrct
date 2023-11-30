#include "AD.h"
#include "stm32f10x.h"                  // Device header

void AD_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC1, ENABLE);	//	开始ADC1的时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	开始GPIOA的时钟
	
	RCC_ADCCLKConfig( RCC_PCLK2_Div6);	//	配置ADC分频器为6分频，72MHz/6=12MHz
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;	//	GPIO口选择模拟输入模式，防止GPIO输入输出造成影响
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	ADC_RegularChannelConfig( ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//	选择规则组输入通道
							//	在ADC1的规则组序列第一个位置选择通道0，采样时间为55.5个ADCCLK周期
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//	指定数据右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//	配置外部触发源
										//	此处选择不外部触发，纯由软件触发
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//	配置ADC工作模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//	是否连续转换，此处不连续
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//	是否开启扫码模式，此处不扫描
	ADC_InitStructure.ADC_NbrOfChannel = 1;		//	扫描模式下的通道数目，此处由于不是扫描模式，参数无效
	ADC_Init( ADC1, &ADC_InitStructure);
	
	ADC_Cmd( ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1);	// 复位校准
	while(SET == ADC_GetResetCalibrationStatus(ADC1));// 获取复位校准状态，等待复位完成
	ADC_StartCalibration(ADC1);	// 开始校准
	while(SET == ADC_GetCalibrationStatus(ADC1));	// 获取开始校准状态，等待校准完成
}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd( ADC1, ENABLE);	//	软件开启ADC转换
	while(RESET == ADC_GetFlagStatus( ADC1, ADC_FLAG_EOC));// 获取EOC标志位状态，等待转换完成
	return ADC_GetConversionValue(ADC1);	//	返回获取得转换值(会自动清除EOF标志位)
}
