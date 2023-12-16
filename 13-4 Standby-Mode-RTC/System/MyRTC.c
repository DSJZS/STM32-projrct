#include "MyRTC.h"
#include "stm32f10x.h"                  // Device header
#include <time.h>

//	分别表示年月日时分秒
uint16_t MyRTC_Time[] = { 2023, 1, 1, 23, 59, 55};

void MyRTC_Init(void)
{
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_PWR, ENABLE);	//	使能PWR的时钟
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_BKP, ENABLE);	//	使能BKP的时钟

	PWR_BackupAccessCmd(ENABLE);	//	设置PWR_CR的DBP，使能对BKP和RTC的访问
	
	if(BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)	//	用BKP判断备用电源是否掉电过，掉电则初始化
	{
		RCC_LSEConfig(RCC_LSE_ON);	//	开启LSE时钟，用LSE外部晶振作时钟源
		while(SET != RCC_GetFlagStatus(RCC_FLAG_LSERDY));	//	等待LSE开启完成
		
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//	选择LSE为RTCCLK时钟源
		RCC_RTCCLKCmd(ENABLE);	//	使能RTCCLK时钟
		
		RTC_WaitForSynchro();	//	等待同步
		RTC_WaitForLastTask();	//	等待上次写操作完成
		
		RTC_SetPrescaler(32768-1);//	配置预分频器，LSE的频率为32.768KHz，所以这里进行32768分频
		RTC_WaitForLastTask();	//	等待写操作完成
		
	//	RTC_SetCounter(1672588795);	//	配置时间为2023年1月1日15:59:55(伦敦时间)
	//	RTC_WaitForLastTask();	//	等待写操作完成
		MyRTC_SetTime();
		
		BKP_WriteBackupRegister( BKP_DR1, 0xA5A5);	//	BKP写入掉电标志
	}
	else
	{
		RTC_WaitForSynchro();	//	等待同步
		RTC_WaitForLastTask();	//	等待上次写操作完成
	}
}

void MyRTC_SetTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	//	将时间填充到tm结构体
	time_date.tm_year = MyRTC_Time[0]-1900;
	time_date.tm_mon = MyRTC_Time[1]-1;
	time_date.tm_mday = MyRTC_Time[2];
	time_date.tm_hour = MyRTC_Time[3];
	time_date.tm_min = MyRTC_Time[4];
	time_date.tm_sec = MyRTC_Time[5];
	
	time_cnt = mktime(&time_date) - 8*60*60;	//	获取对应时间的时间戳(秒计数器值)，这里将北京时间进行转换
	
	RTC_SetCounter(time_cnt);	//	写入时间
	RTC_WaitForLastTask();	//	等待写操作完成
}

void MyRTC_ReadTime(void)
{
	time_t time_cnt;
	struct tm time_date;
	
	time_cnt = RTC_GetCounter() + 8*60*60;	//	将伦敦时间改为北京时间
	
	time_date = *localtime(&time_cnt);
	
	MyRTC_Time[0] = time_date.tm_year+1900;
	MyRTC_Time[1] = time_date.tm_mon+1;
	MyRTC_Time[2] = time_date.tm_mday;
	MyRTC_Time[3] = time_date.tm_hour;
	MyRTC_Time[4] = time_date.tm_min;
	MyRTC_Time[5] = time_date.tm_sec;
}
