#include "stm32f10x.h"   // Device header

#include "Delay.h"

#include "OLED.h"
#include "Key.h"
#include "Encoder.h"
#include "Timer.h"

#include "GUI.h"
#include "MSG.h"

void IWDG_Init(void);

int main(void)
{
	MSG msg;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//	配置NVIC中断分组
	
	OLED_Init();	//	初始化OLED模块
	Timer_Init();	//	初始化Timer模块
	
	IWDG_Init();	//	开启独立看门狗
	
	
	if(SET == RCC_GetFlagStatus(RCC_FLAG_IWDGRST))	//	判断是不是独立看门狗复位
	{
		OLED_Clear();
		OLED_ShowString( 0, 0, "IWDGRST", OLED_8X16);
		OLED_Update();
		Delay_ms(2000);
		OLED_Clear();
		OLED_Update();
		RCC_ClearFlag();	//	清除标志位
	}
	
	MSG_Init();	//	初始化消息模块
	
	Key_Init();	//	初始化按键模块
	Encoder_Init();	//	初始化旋转编码器模块
	
	GUI_Init();	//	初始化GUI模块
	GUI_Display();	//	调用GUI显示函数
	
	while(1)
	{
		if( MSG_GetMessage(&msg) )	//	接收消息
		{
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				GUI_ReceiveCommand(GUI_COMMAND_UP);	//	对GUI发送向上指令
			}
			
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				GUI_ReceiveCommand(GUI_COMMAND_DOWN);	//	对GUI发送向下指令
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				GUI_ReceiveCommand(GUI_COMMAND_RIGHT);	//	对GUI发送向左指令
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				GUI_ReceiveCommand(GUI_COMMAND_LEFT);	//	对GUI发送向右指令
			}
			
			else
			{
				MSG_DefaultProc(&msg);	//	默认处理消息
			}
			GUI_Display();	//	调用GUI显示函数
		}
	}
}


void IWDG_Init(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//	解除写保护
	IWDG_SetPrescaler(IWDG_Prescaler_128);	//	写(设置)预分频器，这里设置为128分频
	IWDG_SetReload(3124);	// 写(设置)重装值
	//	超时时间：TIWDG = 0.025ms × 128 × (3124 + 1) = 1000ms = 10s
	
	IWDG_ReloadCounter();	// 重装载计数器(喂狗)，确保第一个周期为10s
	IWDG_Enable();	//	启动独立看门狗(独立看门狗启动后无法关闭)
	
	Timer_AddFunction(IWDG_ReloadCounter,5000);	//	将喂狗操作函数加入定时列表，每隔5s执行一次
	Timer_Start(IWDG_ReloadCounter);	//	开始为喂狗操作函数定时
}
