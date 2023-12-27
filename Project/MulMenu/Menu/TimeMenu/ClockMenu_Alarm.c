/***************************************************************************************
  * 本菜单为时钟菜单的二级子菜单，用于设置闹钟提醒
  ***************************************************************************************
  */

#include "ClockMenu_Alarm.h"
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

/*	闹钟菜单图标	——	为一个铃铛	*/
const uint8_t ClockMenu_Alarm_Icon[] = {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x80,0x80,0xC0,0xC0,0xC0,0xDC,0xE2,0xE2,0xDC,0xC0,0xC0,0xC0,0x80,0x80,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF0,0xF8,0xFE,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFE,0xF8,0xF0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xF8,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xF8,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x80,0xC0,0xE0,0xF0,0xF0,0xF8,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC,0xF8,0xF0,0xF0,0xE0,0xC0,0x80,0x00,0x00,0x00,
	0x00,0x00,0x07,0x0F,0x1F,0x1F,0x1F,0x3F,0x3F,0x3F,0x7F,0x7F,0x7F,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0x7F,0x7F,0x7F,0x3F,0x3F,0x3F,0x1F,0x1F,0x1F,0x0F,0x07,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x01,0x01,0x01,0x01,0x1D,0x23,0x43,0x43,0x23,0x1D,0x01,0x01,0x01,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

/*********************图像数据*/





/*全局变量*********************/

/*	闹钟菜单结构体	*/
Menu ClockMenu_Alarm;	

/*	闹钟菜单名字	*/
const char* ClockMenu_Alarm_Name = "闹钟";

/*	闹钟菜单结构体	*/
static struct {
	uint16_t Time[6];	//	定义全局的时间数组，数组内容分别为年、月、日、时、分、秒
	int8_t Index;		//	索引，用于表面当前调整的是年、月、日、时、分、秒中的哪一个
}Alarm;

/*	菜单状态标志	*/
static uint8_t Status;	

/*********************全局变量*/





/*枚举*********************/

/*	菜单状态枚举	*/
enum {
	MAIN,		//	主菜单
	SETTING,	//	设置
	SWITCH,		//	开关
};

/*	时间数组下标标识	*/
enum {
	YEAR,	//	年
	MON,	//	月
	MDAY,	//	日
	HOUR,	//	时
	MIN,	//	分
	SEC,	//	秒
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
	OLED_ShowChinese( 34, 48, "开关");
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
	
	/*显示设置的时间*/
	OLED_ShowChinese(0,0,"目标时间:");
	OLED_Printf(0,16,OLED_8X16,"%4d- %2d - %2d ",Alarm.Time[YEAR],Alarm.Time[MON],Alarm.Time[MDAY]);
	OLED_Printf(0,32,OLED_8X16," %2d : %2d : %2d ",Alarm.Time[HOUR],Alarm.Time[MIN],Alarm.Time[SEC]);
	/*显示设置的时间*/
	
	/*显示按键提示*/
	OLED_ShowChinese( 0,48,"左移右移确认退出");
	/*显示按键提示*/
	
	/*显示选中的时间部分*/
	if(Alarm.Index >= 0 && Alarm.Index <3)
		OLED_DrawRectangle(5*Alarm.Index*8,16,32,16,OLED_UNFILLED);
	else
		OLED_DrawRectangle(5*(Alarm.Index-3)*8,32,32,16,OLED_UNFILLED);
	/*显示选中的时间部分*/
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：闹钟开关菜单视图显示
  * 参    数：无
  * 返 回 值：无
  */
static void ClockMenu_Alarm_View_SWITCH(void)
{
	OLED_Clear();	//	清屏
	
	OLED_ShowImage( 10,0,20,48,Button);
	OLED_ShowImage(40,0,20,48,Button);
	OLED_ShowImage(70,0,20,48,Button);
	OLED_ShowImage(100,0,20,48,Button);

	OLED_ShowChinese( 3, 48,  "打开");
	OLED_ShowChinese( 34, 48, "关闭");
	OLED_ShowChinese( 95, 48, "退出");
	
	OLED_Update();	//	刷新
}

/**
  * 函    数：闹钟菜单的功能函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于闹钟菜单的功能函数
  */
static void ClockMenu_Alarm_Funsion(void)
{
	MSG msg;	//	消息结构体
	
	Status = MAIN;	//	初始化状态为 MAIN
	
	ClockMenu_Alarm_View_MAIN();	//	闹钟主菜单视图显示
	
	while(1)
	{
		if(MSG_GetMessage(&msg))	//	获取消息
		{						
			if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = SETTING;	//	初始化状态为 SETTING
					ClockMenu_Alarm_View_SETTING();	//	闹钟设置菜单视图显示
				}
				else if(Status == SETTING)	/*	设置界面	*/
				{
					if(Alarm.Index-1<0)	//	防止索引越界
					{
						Alarm.Index = 5;	//	索引指向最后一个部分(秒)
					}
					else
					{
						Alarm.Index--;	//	索引指向上一个
					}
					ClockMenu_Alarm_View_SETTING();	//	闹钟设置菜单视图显示
				}
				else if(Status == SWITCH)	/*	开关界面	*/
				{
					MyRTC_TurnOnAlarm();	//	打开闹钟中断
					
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	闹钟主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键2松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					Status = SWITCH;	//	初始化状态为 SWITCH
					ClockMenu_Alarm_View_SWITCH();	//	闹钟设置菜单视图显示
				}
				else if(Status == SETTING)	/*	设置界面	*/
				{
					if(Alarm.Index+1>5)	//	防止索引越界
					{
						Alarm.Index = 0;	//	索引指向第一个部分(年)
					}
					else
					{
						Alarm.Index++;	//	索引指向下一个
					}
					ClockMenu_Alarm_View_SETTING();	//	闹钟设置菜单视图显示
				}
				else if(Status == SWITCH)	/*	开关界面	*/
				{
					MyRTC_TurnOffAlarm();	//	关闭闹钟中断
					
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	闹钟主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键3松开
			{
				if(Status == SETTING)	/*	设置界面	*/
				{
					time_t time_cnt;		//定义秒计数器数据类型
					struct tm time_date;	//定义日期时间数据类型
					
					time_date.tm_year = Alarm.Time[YEAR] - 1900;	//将数组的时间赋值给日期时间结构体
					time_date.tm_mon = Alarm.Time[MON] - 1;
					time_date.tm_mday = Alarm.Time[MDAY];
					time_date.tm_hour = Alarm.Time[HOUR];
					time_date.tm_min = Alarm.Time[MIN];
					time_date.tm_sec = Alarm.Time[SEC];
					
					time_cnt = mktime(&time_date);	//调用mktime函数，将日期时间转换为秒计数器格式
					MyRTC_SetAlarm(time_cnt);	//	设置RTC闹钟值并且打开闹钟中断
					
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	闹钟主菜单视图显示
				}
			}
			else if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键4松开
			{
				if(Status == MAIN)	/*	初始界面	*/
				{
					return;	//	退出
				}
				if(Status == SETTING)	/*	设置界面	*/
				{
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	闹钟主菜单视图显示
				}
				if(Status == SWITCH)	/*	开关界面	*/
				{
					Status = MAIN;	//	初始化状态为 MAIN
					ClockMenu_Alarm_View_MAIN();	//	闹钟主菜单视图显示
				}
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
			{
				if(Status == SETTING)	/*	设置界面	*/
				{
					switch(Alarm.Index)	//	根据索引判断调整的是年、月、日、时、分、秒中的哪一个
					{
					case YEAR:
						if(Alarm.Time[YEAR] <= 11111)	//	年分最大值这里是随机选的，合理即可
						{
							++Alarm.Time[YEAR];
							if(!( (Alarm.Time[YEAR] % 400 == 0) || ( (Alarm.Time[YEAR] % 4 == 0) && (Alarm.Time[YEAR] % 100 != 0) ) ))
							{	
								if(Alarm.Time[MON] == 2)
									if(Alarm.Time[MDAY] > 28)
										Alarm.Time[MDAY] = 28;
								//	如果当前选中的月份是二月，且日期为29日
								//	调整后的年份不是闰年，那么调整日期到28日
							}
						}
						break;
					case MON:
						if(Alarm.Time[MON] + 1 <= 12)	//	月份不大于12
						{
							++Alarm.Time[MON];
							switch(Alarm.Time[MON])
							{
								case 4:		//	非2月的小月限定日期不能大于30
								case 6:		//	如果调整月份后不满足，则调整日期为30
								case 9:
								case 11:
								{
									if(Alarm.Time[MDAY] > 30)
										Alarm.Time[MDAY] = 30;
									break;
								}
								case 2:		//	2月的日期不能大于28或者29
								{			//	如果调整月份后不满足，则调整日期为28或29
									if( (Alarm.Time[YEAR] % 400 == 0) || ( (Alarm.Time[YEAR] % 4 == 0) && (Alarm.Time[YEAR] % 100 != 0) ) )
									{	
										if(Alarm.Time[MDAY] > 29)
											Alarm.Time[MDAY] = 29;
									}
									else
									{
										if(Alarm.Time[MDAY] > 28)
											Alarm.Time[MDAY] = 28;
									}
									break;
								}
							}
						}
						break;
					case MDAY:
						switch(Alarm.Time[MON])
						{
							case 1:	//	大月日期不大于31
							case 3:
							case 5:
							case 7:
							case 8:
							case 10:
							case 12:
							{
								if(Alarm.Time[MDAY] + 1 <= 31)
									++Alarm.Time[MDAY];
								break;
							}
							case 4:	//	非2月的小月日期不大于30
							case 6:
							case 9:
							case 11:
							{
								if(Alarm.Time[MDAY] + 1 <= 30)
									++Alarm.Time[MDAY];
								break;
							}
							case 2:	//	2月日期大大于28或者29
							{
								if( (Alarm.Time[YEAR] % 400 == 0) || ( (Alarm.Time[YEAR] % 4 == 0) && (Alarm.Time[YEAR] % 100 != 0) ) )
								{	
									if(Alarm.Time[MDAY] + 1 <= 29)
										++Alarm.Time[MDAY];
								}
								else
								{
									if(Alarm.Time[MDAY] + 1 <= 28)
										++Alarm.Time[MDAY];
								}
								break;
							}
						}
							
						break;
					case HOUR:	//	小时最大为23
						if(Alarm.Time[HOUR] + 1 < 24)
						{
							++Alarm.Time[HOUR];
							if(!( (Alarm.Time[YEAR] % 400 == 0) || ( (Alarm.Time[YEAR] % 4 == 0) && (Alarm.Time[YEAR] % 100 != 0) ) ))
							{	
								if(Alarm.Time[MDAY] > 28)
									Alarm.Time[MDAY] = 28;
							}
						}
						break;
					case MIN:	//	分钟最大为59
						if(Alarm.Time[MIN] + 1 < 60)
							++Alarm.Time[MIN];
						break;
					case SEC:	//	秒数最大为59
						if(Alarm.Time[SEC] + 1 < 60)
							++Alarm.Time[SEC];
						break;
					}
					ClockMenu_Alarm_View_SETTING();	//	闹钟设置菜单视图显示
				}
				
			}
			
			else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
			{
				if(Status == SETTING)	/*	设置界面	*/
				{
					switch(Alarm.Index)
					{
					case YEAR:	//	这里年份最小值可自己设置
						if(Alarm.Time[YEAR] - 1 >= 1900)
						{
							--Alarm.Time[YEAR];
							if(!( (Alarm.Time[YEAR] % 400 == 0) || ( (Alarm.Time[YEAR] % 4 == 0) && (Alarm.Time[YEAR] % 100 != 0) ) ))
							{	
								if(Alarm.Time[MON] == 2)
									if(Alarm.Time[MDAY] > 28)
										Alarm.Time[MDAY] = 28;
								//	如果当前选中的月份是二月，且日期为29日
								//	调整后的年份不是闰年，那么调整日期到28日
							}
						}
						break;
					case MON:	//	月份大于0
						if(Alarm.Time[MON] - 1 > 0)	
						{
							--Alarm.Time[MON];
							switch(Alarm.Time[MON])
							{
								case 4:
								case 6:
								case 9:
								case 11:
								{
									if(Alarm.Time[MDAY] > 30)
										Alarm.Time[MDAY] = 30;
									break;
								}
								case 2:
								{
									if( (Alarm.Time[YEAR] % 400 == 0) || ( (Alarm.Time[YEAR] % 4 == 0) && (Alarm.Time[YEAR] % 100 != 0) ) )
									{	
										if(Alarm.Time[MDAY] > 29)
											Alarm.Time[MDAY] = 29;
									}
									else
									{
										if(Alarm.Time[MDAY] > 28)
											Alarm.Time[MDAY] = 28;
									}
									break;
								}
							}
						}
						break;
					case MDAY:	//	日期大于0
						if(Alarm.Time[MDAY] - 1 > 0)
							--Alarm.Time[MDAY];
						break;
					case HOUR:	//	小时大于0
						if(Alarm.Time[HOUR] - 1 >= 0)
							--Alarm.Time[HOUR];
						break;
					case MIN:	//	分钟大于0
						if(Alarm.Time[MIN] - 1 >= 0)
							--Alarm.Time[MIN];
						break;
					case SEC:	//	秒数大于0
						if(Alarm.Time[SEC] - 1 >= 0)
							--Alarm.Time[SEC];
						break;
					}
					ClockMenu_Alarm_View_SETTING();	//	闹钟设置菜单视图显示
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
  * 函    数：初始化闹钟菜单
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：仅配置了闹钟菜单的功能函数、图标和名字，同级前后关系和父子菜单关系由外部决定
  */
void ClockMenu_Alarm_Init(void)
{
	/*初始化闹钟菜单*/
	Menu_Init(&ClockMenu_Alarm);
	ClockMenu_Alarm.Funsion = ClockMenu_Alarm_Funsion;
	ClockMenu_Alarm.Icon = ClockMenu_Alarm_Icon;
	ClockMenu_Alarm.Name = ClockMenu_Alarm_Name;
	/*初始化闹钟菜单*/
	
	Key_Init();		//	初始化Key模块
	Encoder_Init();	//	初始化Encoder模块
	MyRTC_Init();	//	初始化MyRTC模块
	
	MyRTC_ReadTime();	//	从RTC读取当前的时间
	Alarm.Time[YEAR] = MyRTC_Time[YEAR];
	Alarm.Time[MON] = MyRTC_Time[MON];
	Alarm.Time[MDAY] = MyRTC_Time[MDAY];
	Alarm.Time[HOUR] = MyRTC_Time[HOUR];
	Alarm.Time[MIN] = MyRTC_Time[MIN];
	Alarm.Time[SEC] = MyRTC_Time[SEC];	
}

/**
  * 函    数：RTC中断函数
  * 参    数：无
  * 参    数：无
  * 返 回 值：无
  * 说    明：用于处理闹钟中断
  */
void RTC_IRQHandler(void)
{
	if( SET == RTC_GetITStatus(RTC_IT_ALR) )	//	如果是闹钟中断
	{
		MSG msg;		
		uint8_t OLED_TmpBuf[8][128];	//	暂时存放OLED显示屏的显存
		
		RTC_ClearITPendingBit(RTC_IT_ALR);	//	消除中断标志位，避免不断进行中断
		
		Timer_StopAll();	//	暂停定时列表中的所有函数
		while(MSG_GetMessage(&msg))	//	清空消息
		{
			IWDG_ReloadCounter();	//	喂狗，防止清空消息的时候复位
		}
		Timer_Start(IWDG_ReloadCounter);//	开始为喂狗函数定时
		Timer_Start(Key_Loop);			//	开始为按键循环函数定时
		Timer_Start(Encoder_Loop);		//	开始为旋转编码器循环函数定时
		
		OLED_CopyDisplayBuf(OLED_TmpBuf);	//	将显存复制到暂存中
		OLED_Clear();	//	清屏
		OLED_ShowImage( 38, 0, 48, 48, Warning);	//	显示警告图标
		OLED_ShowChinese( 32, 48, "闹钟到时");	//	提示闹钟到时
		OLED_Update();	//	刷新
		
		while(1)	//	一直循环，直到获取按键或者编码器消息为止
		{
			if(MSG_GetMessage(&msg))	//	获取消息
			{						
				if(msg.message == CM_KEYUP && msg.param == VK_KEY1 )	//	处理按键1松开
					break;
				if(msg.message == CM_KEYUP && msg.param == VK_KEY2 )	//	处理按键1松开
					break;
				if(msg.message == CM_KEYUP && msg.param == VK_KEY3 )	//	处理按键1松开
					break;
				if(msg.message == CM_KEYUP && msg.param == VK_KEY4 )	//	处理按键1松开
					break;
				else if(msg.message == CM_ROATATING && msg.param == EK_LEFT )	//	处理编码器左转
					break;
				else if(msg.message == CM_ROATATING && msg.param == EK_RIGHT )	//	处理编码器右转
					break;
				else
					MSG_DefaultProc(&msg);	//	消息默认处理
			}
		}
		Timer_StartAll();	//	开启定时器列表中的所有函数
		
		OLED_ShowImage(0,0,128,64,(uint8_t*)OLED_TmpBuf);	//	显示暂存中的数据
		OLED_Update();	//	刷新
	}
}

/*********************功能函数*/
