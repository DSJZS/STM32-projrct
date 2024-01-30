/**
  ******************************************************************************
  * @file    bsp_SysTick.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   SysTick 系统滴答时钟10us中断函数库,中断时间可自由配置，
  *          常用的有 1us 10us 1ms 中断。     
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火 指南者 STM32 开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */
#include "./systick/bsp_SysTick.h"
 
 
 
 
 
/*全局变量*********************/

volatile uint32_t g_ul_ms_ticks=0;              //  毫秒计数变量，固定每毫秒加一
//  static volatile uint32_t TimingDelay=0;     //  延时计数变量

/*********************全局变量*/
 
 
 
 
 
/**
  * @brief  启动系统滴答定时器 SysTick
  * @param  无
  * @retval 无
  */
void SysTick_Init(void) 
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0库版本
	if (SysTick_Config(SystemCoreClock/1000))	// ST3.5.0库版本SystemCoreClock/10不能超过16777216
	{ 
		/* Capture error */ 
		while (1);
	}
		// 关闭滴答定时器  
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}



//void mdelay(unsigned long nTime)
//{
//	TimingDelay = nTime;
//	while(TimingDelay != 0);
//}

/**
  * 函    数：获取毫秒计数变量
  * 参    数：count        指向要存放毫秒计数变量数据的变量的指针
  * 返 回 值：固定为0
  * 说    明：无
  */
int get_tick_count(unsigned long *count)
{
    count[0] = g_ul_ms_ticks;
	return 0;
}

//void TimingDelay_Decrement(void)
//{
//	if (TimingDelay != 0x00)
//		TimingDelay--;
//}

/**
  * 函    数：毫秒计数变量加一
  * 参    数：无
  * 返 回 值：无
  * 说    明：需要固定得每毫秒执行一次
  */
void TimeStamp_Increment(void)
{
	g_ul_ms_ticks++;    //  使毫秒计数变量加一
}

// couter 减1的时间 等于 1/systick_clk
// 当counter 从 reload 的值减小到0的时候，为一个循环，如果开启了中断则执行中断服务程序，
// 同时 CTRL 的 countflag 位会置1
// 这一个循环的时间为 reload * (1/systick_clk)

/**
  * 函    数：微秒延时
  * 参    数：us       要延时的微秒数
  * 返 回 值：无
  * 说    明：可能会打乱毫秒计数变量计数
  */
void SysTick_Delay_Us( __IO uint32_t us)
{
	uint32_t i;
    uint32_t SysTick_RELOAD;
    
    SysTick_RELOAD = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;   //  记录延时前 SysTick 的RELOAD值
	SysTick_Config(SystemCoreClock/1000000);                    //  设置 SysTick 为微秒计时
	
	for(i=0;i<us;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1	
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	
	SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;                   // 关闭SysTick定时器
    
    SysTick_Config( SysTick_RELOAD+1 );                         //  设置 SysTick 的RELOAD值为延时前的状态(+1是因为SysTick_Config函数中会-1)
}

/**
  * 函    数：毫秒延时
  * 参    数：ms       要延时的毫秒数
  * 返 回 值：无
  * 说    明：可能会打乱毫秒计数变量计数
  */
void SysTick_Delay_Ms( __IO uint32_t ms)
{
	uint32_t i;	
    uint32_t SysTick_RELOAD;
    
    SysTick_RELOAD = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;   //  记录延时前 SysTick 的RELOAD值
	SysTick_Config(SystemCoreClock/1000);                       //  设置 SysTick 为毫秒计时
	
	for(i=0;i<ms;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) ){;}
        
	}
    
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;                  // 关闭SysTick定时器
    
    SysTick_Config( SysTick_RELOAD+1 );                         //  设置 SysTick 的RELOAD值为延时前的状态(+1是因为SysTick_Config函数中会-1)
}

/**
  * 函    数：秒延时
  * 参    数：s       要延时的秒数
  * 返 回 值：无
  * 说    明：可能会打乱毫秒计数变量计数
  */
void SysTick_Delay_S( __IO uint32_t s)
{
	uint32_t i;	
    uint32_t SysTick_RELOAD;
    
    SysTick_RELOAD = SysTick->LOAD & SysTick_LOAD_RELOAD_Msk;   //  记录延时前 SysTick 的RELOAD值
	SysTick_Config(SystemCoreClock);                            //  设置 SysTick 为秒计时
	
	for(i=0;i<s;i++)
	{
		// 当计数器的值减小到0的时候，CRTL寄存器的位16会置1
		// 当置1时，读取该位会清0
		while( !((SysTick->CTRL)&(1<<16)) );
	}
	
	SysTick->CTRL &=~ SysTick_CTRL_ENABLE_Msk;                  // 关闭SysTick定时器
    
    SysTick_Config( SysTick_RELOAD+1 );                         //  设置 SysTick 的RELOAD值为延时前的状态(+1是因为SysTick_Config函数中会-1)
}

/**
  * 函    数：使能/失能SysTick(系统定时器)
  * 参    数：NewState          SysTick(系统定时器)的新状态，能为 ENABLE 或者 DISABLE
                           //   为 ENABLE 时使能SysTick(系统定时器)，为 DISABLE 时失能SysTick(系统定时器)
  * 返 回 值：无
  * 说    明：无
  */
void SysTick_Cmd(FunctionalState NewState)
{
    assert_param(IS_FUNCTIONAL_STATE(NewState));                            //  断言NewState变量是否正确(即是否为ENABLE或DISABLE)
    
    if (NewState != DISABLE)                                                //  如果NewState不为DISABLE
    {
        SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;                           //  使能SysTick(系统定时器)
    }
    else                                                                    //  如果NewState为DISABLE
    {
        SysTick->CTRL &= (uint32_t)(~(uint32_t)(SysTick_CTRL_ENABLE_Msk));  //  失能SysTick(系统定时器)
    }
}

/*********************************************END OF FILE**********************/
