/***************************************************************************************
  * 本源文件含有按键相关函数
  * GPIO口，发送的消息均可在对应的地方自定义
  ***************************************************************************************
  */

#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "MSG.h"
#include "Timer.h"

/**
  * 函    数：获取当前按键状态
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：可自定义GPIO口
  */
static uint8_t Key_GetState(void)
{
	unsigned char KeyNum=VK_UNPRESS;
	
	if( 0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11))	{	KeyNum=VK_KEY1;	}
	if( 0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10))	{	KeyNum=VK_KEY2;	}
	if( 0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1) )	{	KeyNum=VK_KEY3;	}
	if( 0 == GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0) )	{	KeyNum=VK_KEY4;	}
	
	return KeyNum;
}

/**
  * 函    数：按键循环函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：建议每20ms执行一次扫描各个按键状态，据此发送消息
  *           可根据多次按下来识别连按，或者接连判断按下来识别双击等
  *           发生的消息也可以在Message.h里自定义
  */
void Key_Loop(void)
{
	static unsigned char NowState,LastState;	//	存放上次扫描的按键状态和本次扫描的按键状态
	MSG msg;
	
	LastState = NowState;	//	记录上次扫描的按键状态
	
	NowState = Key_GetState();	//	获取本次扫描的按键状态
	
	if((LastState==VK_KEY1)&&(NowState==VK_UNPRESS))//	上次按键1按下而本次没按键按下，表示按键1松开
	{	
		msg.message = CM_KEYUP;
		msg.param = VK_KEY1;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键1松开消息
	}
	if((LastState==VK_KEY2)&&(NowState==VK_UNPRESS))//	上次按键2按下而本次没按键按下，表示按键2松开
	{	
		msg.message = CM_KEYUP;
		msg.param = VK_KEY2;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键2松开消息
	}
	if((LastState==VK_KEY3)&&(NowState==VK_UNPRESS))//	上次按键3按下而本次没按键按下，表示按键3松开
	{	
		msg.message = CM_KEYUP;
		msg.param = VK_KEY3;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键3松开消息
	}
	if((LastState==VK_KEY4)&&(NowState==VK_UNPRESS))//	上次按键4按下而本次没按键按下，表示按键4松开
	{	
		msg.message = CM_KEYUP;
		msg.param = VK_KEY4;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键4松开消息
	}
	
	
	if((LastState==VK_UNPRESS)&&(NowState==VK_KEY1))//	上次没按键按下而本次按键1按下，表示按键1按下
	{	
		msg.message = CM_KEYDOWN;
		msg.param = VK_KEY1;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键1按下消息
	}
	if((LastState==VK_UNPRESS)&&(NowState==VK_KEY2))//	上次没按键按下而本次按键1按下，表示按键2按下
	{	
		msg.message = CM_KEYDOWN;
		msg.param = VK_KEY2;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键2按下消息
	}
	if((LastState==VK_UNPRESS)&&(NowState==VK_KEY3))//	上次没按键按下而本次按键1按下，表示按键3按下
	{	
		msg.message = CM_KEYDOWN;
		msg.param = VK_KEY3;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键3按下消息
	}
	if((LastState==VK_UNPRESS)&&(NowState==VK_KEY4))//	上次没按键按下而本次按键1按下，表示按键4按下
	{	
		msg.message = CM_KEYDOWN;
		msg.param = VK_KEY4;
		msg.count = 1;
		MSG_SendMessage(&msg);	//	发送按键4按下消息
	}
}

/**
  * 函    数：按键初始化
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：打开对应的GPIOA口，可自定义
  */
void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
	
	/*初始化GPIO口*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	/*初始化GPIO口*/
	
	/*将Key_Loop函数添加到定时列表每20ms执行一次*/
	Timer_AddFunction( Key_Loop, 20);
	Timer_Start(Key_Loop);
	/*将Key_Loop函数添加到定时列表每20ms执行一次*/
}
