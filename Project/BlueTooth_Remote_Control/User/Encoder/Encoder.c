/***************************************************************************************
  * 本源文件含有旋转编码器相关函数
  * GPIO口，发送的消息均可在对应的地方自定义
  ***************************************************************************************
  */

#include "Encoder.h"
#include "stm32f10x.h"                  // Device header
#include "./MSG/MSG.h"
#include "./Timer/Timer.h"

/**
  * 函    数：读取定时器3的计数器来获取编码器接口信息
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：无
  */
static int16_t Encoder_GetCounter(void)
{
	int16_t Temp = (int16_t)(TIM_GetCounter( TIM3));
	TIM_SetCounter( TIM3, 0);
	return Temp;
}

/**
  * 函    数：旋转编码器循环函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：建议每20ms执行一次获取旋转编码器状态，据此发送消息
  *           发生的消息也可以在Message.h里自定义
  */
void Encoder_Loop(void)
{
	static uint8_t Left = 0;	//	记录向左的波形数
	static uint8_t Right = 0;	//	记录向右的波形数
	
	MSG msg;
	int16_t EncoderCount = Encoder_GetCounter();	//	获取旋转编码器信息
	
	if(EncoderCount > 0)	//	增加向右的波形数
	{
		Right += EncoderCount;
		
	}
	if(EncoderCount < 0)	//	增加向左的波形数
	{
		Left += (-EncoderCount);
	}
	
	if(Right/4)	//	EC11旋转编码器转一格四个波形
	{
		Left = 0;	//	清空计数
		Right = 0;
		msg.message = CM_ROATATING;
		msg.param = EK_RIGHT;
		MSG_SendMessage(&msg);	//	发送旋转编码器向右旋转信息
	}
	
	if(Left/4)	//	EC11旋转编码器转一格四个波形
	{
		Left = 0;	//	清空计数
		Right = 0;
		msg.message = CM_ROATATING;
		msg.param = EK_LEFT;
		MSG_SendMessage(&msg);	//	发送旋转编码器向左旋转信息
	}
}

/**
  * 函    数：旋转编码器初始化
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：打开对应的GPIOA口和定时器，可自定义
  */
void Encoder_Init(void)
{
	/*初始化编码器接口*/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	//	选择向上计数模式
												//(但是没用，因为计数方向现在由编码器接口托管)
	TIM_TimeBaseInitStructure.TIM_Period = 65536-1;	//	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1-1;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;	//	Repetiti            onCounter重复计数器的值(高级定时器才有)
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStructure);
	//	配置通道1
	TIM_ICStructInit( &TIM_ICInitStructure);	//	初始化IC结构体
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	//	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	//	配置输入捕获的滤波器
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，
															//编码器上升沿和下降沿均有效
															//此处Rising表示高低电平极性不反转
															//和后续编码器接口配置函数重复配置了同一个寄存器，所以可以省略
	//	配置通道2
	TIM_ICInit( TIM3, &TIM_ICInitStructure);
	
	TIM_ICStructInit( &TIM_ICInitStructure);	//	初始化IC结构体
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;	//	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	//	配置输入捕获的滤波器
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，
															//编码器上升沿和下降沿均有效
															//此处Rising表示高低电平极性不反转
															//和后续编码器接口配置函数重复配置了同一个寄存器，所以可以省略
	TIM_ICInit( TIM3, &TIM_ICInitStructure);
	
	TIM_EncoderInterfaceConfig( TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	//	配置编码器接口，在TI1和TI2上计数，且极性均不反向
	
	TIM_Cmd( TIM3, ENABLE);
	/*初始化编码器接口*/
	
	/*将Encoder_Loop函数添加到定时列表每20ms执行一次*/
	Timer_AddFunction( Encoder_Loop, 20);
	Timer_Start(Encoder_Loop);
	/*将Encoder_Loop函数添加到定时列表每20ms执行一次*/
}
