/***************************************************************************************
  * 本菜单为时钟菜单的二级子菜单，用于设置格林威治时间(伦敦时间)
  ***************************************************************************************
  */

#include "ClockMenu_Setting.h"
#include "stm32f10x.h"                  // Device header

#include "Key.h"
#include "OLED.h"
#include "Menu.h"
#include <time.h>
#include "Delay.h"
#include "MyRTC.h"
#include "Store.h"
#include "Timer.h"
#include "Encoder.h"

#include "MSG.h"


/*图像数据*********************/

/*	时钟设置菜单图标	——	为一个发条带动的齿轮	*/
const uint8_t ClockMenu_Setting_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
	0x40,0x80,0x00,0x40,0xE0,0xF0,0xF8,0xFC,0x40,0x40,0x80,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,0x40,0x20,0x20,0x00,0x00,0x80,0x80,0x40,
	0x40,0xA0,0xA0,0x50,0x50,0x28,0x28,0xD4,0x14,0xEA,0x0A,0x05,0x05,0x02,0x02,0x01,
	0x01,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x00,0x00,0x00,0x00,0x01,0xC1,0x32,0x0C,
	0x00,0x60,0x98,0x06,0x01,0x00,0x00,0x00,0x00,0x00,0xC0,0x80,0x01,0x02,0x02,0x01,
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x04,0x04,0x02,0x02,0x01,0x00,0x00,0x00,
	0x00,0x00,0x00,0x01,0x01,0x02,0x02,0x04,0x04,0x04,0x3F,0x1F,0x1F,0x0E,0x04,0x40,
	0x80,0x00,0x90,0x60,0x40,0x28,0x30,0xFF,0x00,0xFF,0x20,0x38,0x40,0xC0,0xA0,0x10,
	0x80,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x11,0x92,0x7C,
	0x96,0x11,0x10,0x11,0x12,0x94,0x58,0x3F,0xF0,0x3F,0x54,0x92,0x11,0x10,0x10,0xD3,
	0x3C,0x52,0x91,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x08,
	0x05,0x22,0x14,0x0A,0x09,0x78,0x08,0x08,0x0F,0x78,0x08,0x14,0x25,0x02,0x05,0x08,
	0x01,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	时钟设置菜单结构体	*/
Menu ClockMenu_Setting;	

/*	时钟设置菜单名字	*/
const char* ClockMenu_Setting_Name = "设置";

/*	闹钟菜单结构体	*/
static struct {
	uint16_t Time[6];
	int8_t Index;
}Clock;

/*	菜单状态标志	*/
static uint8_t Status;	

/*********************全局变量*/





/*枚举*********************/

/*	菜单状态枚举	*/
enum {
	MAIN,		//	主菜单
	SETTING,	//	设置
};

/*	时间数组下标标识	*/
enum {
	YEAR,
	MON,
	MDAY,
	HOUR,
	MIN,
	SEC,
};

/*********************枚举*/





/*功能函数*********************/

/**
  * 函    数：闹钟主菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void ClockMenu_Alarm_View_MAIN(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 10,0,20,48,Button);
	OLED_ShowImage(40,0,20,48,Button);
	OLED_ShowImage(70,0,20,48,Button);
	OLED_ShowImage(100,0,20,48,Button);

	OLED_ShowChinese( 3, 48,  "设置");
	OLED_ShowChinese( 95, 48, "退出");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：闹钟设置菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void ClockMenu_Alarm_View_SETTING(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowChinese(0,0,"目标时间:");
	OLED_Printf(0,16,OLED_8X16,"%4d- %2d - %2d ",Clock.Time[YEAR],Clock.Time[MON],Clock.Time[MDAY]);
	OLED_Printf(0,32,OLED_8X16," %2d : %2d : %2d ",Clock.Time[HOUR],Clock.Time[MIN],Clock.Time[SEC]);
	OLED_ShowChinese( 0,48,"左移右移确认退出");
	
	if(Clock.Index >= 0 && Clock.Index <3)
		OLED_DrawRectangle(5*Clock.Index*8,16,32,16,OLED_UNFILLED);
	else
		OLED_DrawRectangle(5*(Clock.Index-3)*8,32,32,16,OLED_UNFILLED);
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：时钟设置菜单的功能函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于时钟设置菜单的功能函数
  */
void ClockMenu_Setting_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	Status = MAIN;	//	初始化状态为 MAIN
	

	ClockMenu_Alarm_View_MAIN();	//	游戏主菜单视图显示
	//Timer_AddFunction( Tetris_Loop, TIME_INTERVAL);	//	将Tetris_Loop函数添加到定时器定时列表，配置为每1s到时一次
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == MAIN)
				{
					Status = SETTING;
					ClockMenu_Alarm_View_SETTING();
				}
				else if(Status == SETTING)
				{
					if(Clock.Index-1<0)
					{
						Clock.Index = 5;
					}
					else
					{
						Clock.Index--;
					}
					ClockMenu_Alarm_View_SETTING();
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == SETTING)
				{
					if(Clock.Index+1>5)
					{
						Clock.Index = 0;
					}
					else
					{
						Clock.Index++;
					}
					ClockMenu_Alarm_View_SETTING();
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == SETTING)
				{
					time_t time_cnt;		//定义秒计数器数据类型
					struct tm time_date;	//定义日期时间数据类型
					
					time_date.tm_year = Clock.Time[YEAR] - 1900;		//将数组的时间赋值给日期时间结构体
					time_date.tm_mon = Clock.Time[MON] - 1;
					time_date.tm_mday = Clock.Time[MDAY];
					time_date.tm_hour = Clock.Time[HOUR];
					time_date.tm_min = Clock.Time[MIN];
					time_date.tm_sec = Clock.Time[SEC];
					
					time_cnt = mktime(&time_date);	//调用mktime函数，将日期时间转换为秒计数器格式
					RTC_SetCounter(time_cnt);
					
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	游戏主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				if(Status == MAIN)
				{
					return;
				}
				if(Status == SETTING)
				{
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	游戏主菜单视图显示
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				if(Status == SETTING)
				{
					switch(Clock.Index)
					{
					case YEAR:
						if(Clock.Time[YEAR] <= 11111)
						{
							++Clock.Time[YEAR];
							if(!( (Clock.Time[YEAR] % 400 == 0) || ( (Clock.Time[YEAR] % 4 == 0) && (Clock.Time[YEAR] % 100 != 0) ) ))
							{	
								if(Clock.Time[MDAY] > 28)
									Clock.Time[MDAY] = 28;
							}
						}
						break;
					case MON:
						if(Clock.Time[MON] + 1 <= 12)
						{
							++Clock.Time[MON];
							switch(Clock.Time[MON])
							{
								case 4:
								case 6:
								case 9:
								case 11:
								{
									if(Clock.Time[MDAY] > 30)
										Clock.Time[MDAY] = 30;
									break;
								}
								case 2:
								{
									if( (Clock.Time[YEAR] % 400 == 0) || ( (Clock.Time[YEAR] % 4 == 0) && (Clock.Time[YEAR] % 100 != 0) ) )
									{	
										if(Clock.Time[MDAY] > 29)
											Clock.Time[MDAY] = 29;
									}
									else
									{
										if(Clock.Time[MDAY] > 28)
											Clock.Time[MDAY] = 28;
									}
									break;
								}
							}
						}
						break;
					case MDAY:
						switch(Clock.Time[MON])
						{
							case 1:
							case 3:
							case 5:
							case 7:
							case 8:
							case 10:
							case 12:
							{
								if(Clock.Time[MDAY] + 1 <= 31)
									++Clock.Time[MDAY];
								break;
							}
							case 4:
							case 6:
							case 9:
							case 11:
							{
								if(Clock.Time[MDAY] + 1 <= 30)
									++Clock.Time[MDAY];
								break;
							}
							case 2:
							{
								if( (Clock.Time[YEAR] % 400 == 0) || ( (Clock.Time[YEAR] % 4 == 0) && (Clock.Time[YEAR] % 100 != 0) ) )
								{	
									if(Clock.Time[MDAY] + 1 <= 29)
										++Clock.Time[MDAY];
								}
								else
								{
									if(Clock.Time[MDAY] + 1 <= 28)
										++Clock.Time[MDAY];
								}
								break;
							}
						}
							
						break;
					case HOUR:
						if(Clock.Time[HOUR] + 1 < 24)
						{
							++Clock.Time[HOUR];
							if(!( (Clock.Time[YEAR] % 400 == 0) || ( (Clock.Time[YEAR] % 4 == 0) && (Clock.Time[YEAR] % 100 != 0) ) ))
							{	
								if(Clock.Time[MDAY] > 28)
									Clock.Time[MDAY] = 28;
							}
						}
						break;
					case MIN:
						if(Clock.Time[MIN] + 1 < 60)
							++Clock.Time[MIN];
						break;
					case SEC:
						if(Clock.Time[SEC] + 1 < 60)
							++Clock.Time[SEC];
						break;
					}
					ClockMenu_Alarm_View_SETTING();
				}
				
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				if(Status == SETTING)
				{
					switch(Clock.Index)
					{
					case YEAR:
						if(Clock.Time[YEAR] - 1 >= 1900)
							--Clock.Time[YEAR];
						break;
					case MON:
						if(Clock.Time[MON] - 1 > 0)
						{
							--Clock.Time[MON];
							switch(Clock.Time[MON])
							{
								case 4:
								case 6:
								case 9:
								case 11:
								{
									if(Clock.Time[MDAY] > 30)
										Clock.Time[MDAY] = 30;
									break;
								}
								case 2:
								{
									if( (Clock.Time[YEAR] % 400 == 0) || ( (Clock.Time[YEAR] % 4 == 0) && (Clock.Time[YEAR] % 100 != 0) ) )
									{	
										if(Clock.Time[MDAY] > 29)
											Clock.Time[MDAY] = 29;
									}
									else
									{
										if(Clock.Time[MDAY] > 28)
											Clock.Time[MDAY] = 28;
									}
									break;
								}
							}
						}
						break;
					case MDAY:
						if(Clock.Time[MDAY] - 1 > 0)
							--Clock.Time[MDAY];
						break;
					case HOUR:
						if(Clock.Time[HOUR] - 1 >= 0)
							--Clock.Time[HOUR];
						break;
					case MIN:
						if(Clock.Time[MIN] - 1 >= 0)
							--Clock.Time[MIN];
						break;
					case SEC:
						if(Clock.Time[SEC] - 1 >= 0)
							--Clock.Time[SEC];
						break;
					}
					ClockMenu_Alarm_View_SETTING();
				}
				
			}
			else
			{
				MSG_DefaultProc(&msg);	//	消息默认处理
			}
		}
		
	}	
}

/**
  * 函    数：初始化时钟设置菜单
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了时钟设置菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void ClockMenu_Setting_Init(void)
{
	Menu_Init(&ClockMenu_Setting);
	ClockMenu_Setting.Funsion = ClockMenu_Setting_Funsion;
	ClockMenu_Setting.Icon = ClockMenu_Setting_Icon;
	ClockMenu_Setting.Name = ClockMenu_Setting_Name;
	
	Key_Init();
	Encoder_Init();
	MyRTC_Init();
	Store_Init();
	
	MyRTC_ReadTime();
	Clock.Time[YEAR] = MyRTC_Time[YEAR];
	Clock.Time[MON] = MyRTC_Time[MON];
	Clock.Time[MDAY] = MyRTC_Time[MDAY];
	Clock.Time[HOUR] = MyRTC_Time[HOUR];
	Clock.Time[MIN] = MyRTC_Time[MIN];
	Clock.Time[SEC] = MyRTC_Time[SEC];
}

/*********************功能函数*/
