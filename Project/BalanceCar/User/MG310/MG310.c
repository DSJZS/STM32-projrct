/***************************************************************************************
  * 本源文件含有MG310电机、编码器相关函数
  * GPIO口，TIM定时器外设均可在对应的地方自定义
  ***************************************************************************************
  */
  
#include "MG310.h"
#include "stm32f10x.h"





/*功能函数*********************/

/**
  * 函    数：对编码器利用的GPIO口进行初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：对连接编码器AB相的GPIO口进行初始化
  */
static void MG310_Encoder_GPIO_Config(void) 
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    /*  使能这些GPIO口对应的时钟，并且将这些GPIO口配置为上拉输入 */
    
    MG310_ENCODER1_A_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER1_A_TIM_CH_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_ENCODER1_A_TIM_CH_PIN;
    GPIO_Init( MG310_ENCODER1_A_TIM_CH_PORT, &GPIO_InitStructure);
    
    MG310_ENCODER1_B_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER1_B_TIM_CH_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_ENCODER1_B_TIM_CH_PIN;
    GPIO_Init( MG310_ENCODER1_B_TIM_CH_PORT, &GPIO_InitStructure);
    
    MG310_ENCODER2_A_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER2_A_TIM_CH_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_ENCODER2_A_TIM_CH_PIN;
    GPIO_Init( MG310_ENCODER2_A_TIM_CH_PORT, &GPIO_InitStructure);
    
    MG310_ENCODER2_B_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER2_B_TIM_CH_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_ENCODER2_B_TIM_CH_PIN;
    GPIO_Init( MG310_ENCODER2_B_TIM_CH_PORT, &GPIO_InitStructure);
}

/**
  * 函    数：对编码器利用的定时器编码器接口进行初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：对处理编码器发出信号的定时器编码器接口进行初始化
  */
static void MG310_Encoder_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
    
    /*使能定时器时钟*/
    MG310_ENCODER1_TIM_APBxClock_FUN( MG310_ENCODER1_TIM_CLK, ENABLE);
    MG310_ENCODER2_TIM_APBxClock_FUN( MG310_ENCODER2_TIM_CLK, ENABLE);
    /*使能定时器时钟*/
    
    /*配置定时器时钟时基单元*/
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;	            //  配置外部时钟的采样频率由内部时钟几分频而来
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;	        //	选择向上计数模式
                                                                                //(但是没用，因为计数方向现在由编码器接口托管)
	TIM_TimeBaseInitStructure.TIM_Period = MG310_ENCODER1_TIM_Period;	    //	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = MG310_ENCODER1_TIM_Prescaler;	//	PSC预分频器的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 1-1;	                //	RepetitionCounter重复计数器的值(高级定时器才有)    
	TIM_TimeBaseInit( MG310_ENCODER1_TIM, &TIM_TimeBaseInitStructure);      //  初始化编码器1定时器的时基单元
    
    TIM_TimeBaseInitStructure.TIM_Period = MG310_ENCODER2_TIM_Period;	    //	ARR自动重装器的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = MG310_ENCODER2_TIM_Prescaler;	//	PSC预分频器的值
    TIM_TimeBaseInit( MG310_ENCODER2_TIM, &TIM_TimeBaseInitStructure);      //  初始化编码器2定时器的时基单元
    /*配置定时器时钟时基单元*/
    
    /*配置定时器输入捕获*/
	//	配置通道1
	TIM_ICStructInit( &TIM_ICInitStructure);	                //	初始化IC结构体
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;	        //	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	                    //	配置输入捕获的滤波器
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，
                                                                    //编码器上升沿和下降沿均有效
                                                                    //此处Rising表示高低电平极性不反转
                                                                    //和后续编码器接口配置函数重复配置了同一个寄存器，所以可以省略
	
	TIM_ICInit( MG310_ENCODER1_TIM, &TIM_ICInitStructure);
    TIM_ICInit( MG310_ENCODER2_TIM, &TIM_ICInitStructure);
    
	//	配置通道2
	TIM_ICStructInit( &TIM_ICInitStructure);	                //	初始化IC结构体
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;            //	选择配置的通道
	TIM_ICInitStructure.TIM_ICFilter = 0xF;	                    //	配置输入捕获的滤波器
//	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//	配置输入捕获的极性，
                                                                    //编码器上升沿和下降沿均有效
                                                                    //此处Rising表示高低电平极性不反转
                                                                    //和后续编码器接口配置函数重复配置了同一个寄存器，所以可以省略
	TIM_ICInit( MG310_ENCODER1_TIM, &TIM_ICInitStructure);
    TIM_ICInit( MG310_ENCODER2_TIM, &TIM_ICInitStructure);
	/*配置定时器输入捕获*/
    
    /*配置定时器编码器接口*/
	TIM_EncoderInterfaceConfig( MG310_ENCODER1_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_EncoderInterfaceConfig( MG310_ENCODER2_TIM, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    /*配置定时器编码器接口*/
    
    /*使能定时器*/
    TIM_Cmd( MG310_ENCODER1_TIM, ENABLE);
    TIM_Cmd( MG310_ENCODER2_TIM, ENABLE);
    /*使能定时器*/
}

/**
  * 函    数：关闭编码器
  * 参    数：无
  * 返 回 值：无
  * 说    明：会关闭编码器相关时钟，失能相关外设
  */
static void MG310_Encoder_Close(void)
{
    TIM_Cmd( MG310_ENCODER1_TIM, DISABLE);
    TIM_Cmd( MG310_ENCODER2_TIM, DISABLE);   
    MG310_ENCODER1_A_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER1_A_TIM_CH_GPIO_CLK, DISABLE); 
    MG310_ENCODER1_B_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER1_B_TIM_CH_GPIO_CLK, DISABLE);
    MG310_ENCODER2_A_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER2_A_TIM_CH_GPIO_CLK, DISABLE);
    MG310_ENCODER2_B_TIM_CH_GPIO_APBxClock_FUN( MG310_ENCODER2_B_TIM_CH_GPIO_CLK, DISABLE);
    MG310_ENCODER1_TIM_APBxClock_FUN( MG310_ENCODER1_TIM_CLK, DISABLE);
    MG310_ENCODER2_TIM_APBxClock_FUN( MG310_ENCODER2_TIM_CLK, DISABLE);
}

/**
  * 函    数：获取编码器1计数
  * 参    数：无
  * 返 回 值：无
  * 说    明：返回CNT计数值
  */
int16_t MG310_Encoder1_GetCount(void)
{
    int16_t Temp = (int16_t)(TIM_GetCounter( MG310_ENCODER1_TIM));
	TIM_SetCounter( MG310_ENCODER1_TIM, 0);
	return Temp;
}

/**
  * 函    数：获取编码器1计数
  * 参    数：无
  * 返 回 值：无
  * 说    明：返回CNT计数值
  */
int16_t MG310_Encoder2_GetCount(void)
{
    int16_t Temp = (int16_t)(TIM_GetCounter( MG310_ENCODER2_TIM));
	TIM_SetCounter( MG310_ENCODER2_TIM, 0);
	return Temp;
}

/**
  * 函    数：对电机利用的GPIO口进行初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：对连接TB6612电机驱动芯片的AIN1、AIN2、BIN1、BIN2、PWMA、PWMB引脚的GPIO口进行初始化
  */
static void MG310_Motor_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /*  使能连接TB6612电机驱动芯片的AIN1、AIN2、BIN1、BIN2、PWMA、PWMB引脚的GPIO口的对应时钟，并将这些GPIo口配置为推挽输出模式*/
    MG310_TB6612_AIN1_GPIO_APBxClock_FUN( MG310_TB6612_AIN1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_TB6612_AIN1_PIN;
    GPIO_Init( MG310_TB6612_AIN1_PORT, &GPIO_InitStructure);
    
    MG310_TB6612_AIN2_GPIO_APBxClock_FUN( MG310_TB6612_AIN2_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_TB6612_AIN2_PIN;
    GPIO_Init( MG310_TB6612_AIN2_PORT, &GPIO_InitStructure);
    
    MG310_TB6612_BIN1_GPIO_APBxClock_FUN( MG310_TB6612_BIN1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_TB6612_BIN1_PIN;
    GPIO_Init( MG310_TB6612_BIN1_PORT, &GPIO_InitStructure);
    GPIO_ResetBits( MG310_TB6612_AIN1_PORT, MG310_TB6612_AIN1_PIN);
    
    MG310_TB6612_BIN2_GPIO_APBxClock_FUN( MG310_TB6612_BIN2_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_TB6612_BIN2_PIN;
    GPIO_Init( MG310_TB6612_BIN2_PORT, &GPIO_InitStructure);
    GPIO_ResetBits( MG310_TB6612_AIN2_PORT, MG310_TB6612_AIN2_PIN);
    
    MG310_TB6612_PWMA_TIM_CH_GPIO_APBxClock_FUN( MG310_TB6612_PWMA_TIM_CH_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_TB6612_PWMA_TIM_CH_PIN;
    GPIO_Init( MG310_TB6612_PWMA_TIM_CH_PORT, &GPIO_InitStructure);
    
    MG310_TB6612_PWMB_TIM_CH_GPIO_APBxClock_FUN( MG310_TB6612_PWMB_TIM_CH_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = MG310_TB6612_PWMB_TIM_CH_PIN;
    GPIO_Init( MG310_TB6612_PWMB_TIM_CH_PORT, &GPIO_InitStructure);
}

/**
  * 函    数：对电机利用的定时器进行初始化
  * 参    数：无
  * 返 回 值：无
  * 说    明：将这些定时器配置为PWM1模式，并且初始化相关比较输出通道
  */
static void MG310_Motor_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    /*初始化定时器时基单元*/
    MG310_TB6612_PWMA_TIM_APBxClock_FUN( MG310_TB6612_PWMA_TIM_CLK, ENABLE); 
    TIM_InternalClockConfig( MG310_TB6612_PWMA_TIM );                               //  定时器选用内部时钟
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;                     //  配置外部时钟的采样频率由内部时钟几分频而来  
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;                 //	选择向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period = MG310_TB6612_PWMA_TIM_Period;            //	ARR自动重装器的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = MG310_TB6612_PWMA_TIM_Prescaler;      //	PSC预分频器的值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 1-1;                          //	RepetitionCounter重复计数器的值(高级定时器才有) 
    TIM_TimeBaseInit( MG310_TB6612_PWMA_TIM, &TIM_TimeBaseInitStructure);           //  初始化定时器时基单元
    
    if(  MG310_TB6612_PWMA_TIM != MG310_TB6612_PWMB_TIM )                           //  如果PWMA利用的定时器和PWMB利用的定时器不一致
    {
        MG310_TB6612_PWMB_TIM_APBxClock_FUN( MG310_TB6612_PWMB_TIM_CLK, ENABLE);
        TIM_InternalClockConfig( MG310_TB6612_PWMB_TIM );                           //  定时器选用内部时钟
        TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;                 //  配置外部时钟的采样频率由内部时钟几分频而来  
        TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;             //	选择向上计数模式
        TIM_TimeBaseInitStructure.TIM_Period = MG310_TB6612_PWMB_TIM_Period;        //	ARR自动重装器的值
        TIM_TimeBaseInitStructure.TIM_Prescaler = MG310_TB6612_PWMB_TIM_Prescaler;  //	PSC预分频器的值
        TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 1-1;                      //	RepetitionCounter重复计数器的值(高级定时器才有) 
        TIM_TimeBaseInit( MG310_TB6612_PWMB_TIM, &TIM_TimeBaseInitStructure);       //  初始化定时器时基单元
    }
    /*初始化定时器时基单元*/
    
    /*初始化定时器比较输出通道*/
    TIM_OCStructInit(&TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                                   //  比较输出通道选择PWM1模式
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                       //  使能比较输出通道
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;                           //  配置比较输出通道的极性为高电平
    TIM_OCInitStructure.TIM_Pulse = 0;                                                  //  CCR捕获/比较寄存器的值
    
    MG310_TB6612_PWMA_TIM_OCxInit( MG310_TB6612_PWMA_TIM, &TIM_OCInitStructure);        //  初始化定时器时基单元
    MG310_TB6612_PWMA_TIM_OCxPreloadConfig( MG310_TB6612_PWMA_TIM, TIM_OCPreload_Enable);// 比较输出通道开启预装载功能  
    
    MG310_TB6612_PWMB_TIM_OCxInit( MG310_TB6612_PWMB_TIM, &TIM_OCInitStructure);        //  初始化定时器时基单元
    MG310_TB6612_PWMB_TIM_OCxPreloadConfig( MG310_TB6612_PWMB_TIM, TIM_OCPreload_Enable);// 比较输出通道开启预装载功能  
    
    if( (MG310_TB6612_PWMA_TIM == TIM1) || ( MG310_TB6612_PWMA_TIM == TIM8 ) )          //  如果使用的定时器是高级定时器
        TIM_CtrlPWMOutputs(MG310_TB6612_PWMA_TIM,ENABLE);                               //  需要使用TIM_CtrlPWMOutputs函数使能比较输出
    
    if( (MG310_TB6612_PWMB_TIM == TIM1) || ( MG310_TB6612_PWMB_TIM == TIM8 ) )          //  如果使用的定时器是高级定时器
        TIM_CtrlPWMOutputs(MG310_TB6612_PWMB_TIM,ENABLE);                               //  需要使用TIM_CtrlPWMOutputs函数使能比较输出
    /*初始化定时器比较输出通道*/
    
    /*使能定时器*/
    TIM_Cmd( MG310_TB6612_PWMA_TIM, ENABLE);
    TIM_Cmd( MG310_TB6612_PWMB_TIM, ENABLE);
    /*使能定时器*/
}

/**
  * 函    数：关闭电机
  * 参    数：无
  * 返 回 值：无
  * 说    明：会关闭电机相关时钟，失能相关外设
  */
static void MG310_Motor_Close(void)
{
    TIM_Cmd( MG310_TB6612_PWMA_TIM, DISABLE);
    TIM_Cmd( MG310_TB6612_PWMB_TIM, DISABLE); 
    MG310_TB6612_AIN1_GPIO_APBxClock_FUN( MG310_TB6612_AIN1_GPIO_CLK, DISABLE);
    MG310_TB6612_AIN2_GPIO_APBxClock_FUN( MG310_TB6612_AIN2_GPIO_CLK, DISABLE);
    MG310_TB6612_BIN1_GPIO_APBxClock_FUN( MG310_TB6612_BIN1_GPIO_CLK, DISABLE);
    MG310_TB6612_BIN2_GPIO_APBxClock_FUN( MG310_TB6612_BIN2_GPIO_CLK, DISABLE);
    MG310_TB6612_PWMA_TIM_CH_GPIO_APBxClock_FUN( MG310_TB6612_PWMA_TIM_CH_GPIO_CLK, DISABLE);
    MG310_TB6612_PWMB_TIM_CH_GPIO_APBxClock_FUN( MG310_TB6612_PWMB_TIM_CH_GPIO_CLK, DISABLE);
    MG310_TB6612_PWMA_TIM_APBxClock_FUN( MG310_TB6612_PWMA_TIM_CLK, DISABLE);  
    MG310_TB6612_PWMB_TIM_APBxClock_FUN( MG310_TB6612_PWMB_TIM_CLK, DISABLE);
}

/**
  * 函    数：控制左电机转动方向，使其让小车前进
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动方向
  */
static void MG310_Motor_LeftAdvance(void)
{
    GPIO_SetBits( MG310_TB6612_AIN1_PORT, MG310_TB6612_AIN1_PIN);
    GPIO_ResetBits( MG310_TB6612_AIN2_PORT, MG310_TB6612_AIN2_PIN);
}

/**
  * 函    数：控制右电机转动方向，使其让小车前进
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动方向
  */
static void MG310_Motor_RightAdvance(void)
{
    GPIO_SetBits( MG310_TB6612_BIN1_PORT, MG310_TB6612_BIN1_PIN);
    GPIO_ResetBits( MG310_TB6612_BIN2_PORT, MG310_TB6612_BIN2_PIN);
}

/**
  * 函    数：控制两个电机转动方向，使其让小车前进
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动方向
  */
static void MG310_Motor_Advance(void)
{
    MG310_Motor_LeftAdvance();
    MG310_Motor_RightAdvance();
}

/**
  * 函    数：控制左电机转动方向，使其让小车后退
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动方向
  */
static void MG310_Motor_LeftBack(void)
{
    GPIO_ResetBits( MG310_TB6612_AIN1_PORT, MG310_TB6612_AIN1_PIN);
    GPIO_SetBits( MG310_TB6612_AIN2_PORT, MG310_TB6612_AIN2_PIN);
}

/**
  * 函    数：控制右电机转动方向，使其让小车后退
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动方向
  */
static void MG310_Motor_RightBack(void)
{
    GPIO_ResetBits( MG310_TB6612_BIN1_PORT, MG310_TB6612_BIN1_PIN);
    GPIO_SetBits( MG310_TB6612_BIN2_PORT, MG310_TB6612_BIN2_PIN);
}

/**
  * 函    数：控制两个电机转动方向，使其让小车后退
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动方向
  */
static void MG310_Motor_Back(void)
{
    MG310_Motor_LeftBack();
    MG310_Motor_RightBack();
}

/**
  * 函    数：控制左电机转动，使其让小车制动
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动
  */
static void MG310_Motor_LeftBrake(void)
{
    GPIO_SetBits( MG310_TB6612_AIN1_PORT, MG310_TB6612_AIN1_PIN);
    GPIO_SetBits( MG310_TB6612_AIN2_PORT, MG310_TB6612_AIN2_PIN);
}

/**
  * 函    数：控制右电机转动，使其让小车制动
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动
  */
static void MG310_Motor_RightBrake(void)
{
    GPIO_SetBits( MG310_TB6612_AIN1_PORT, MG310_TB6612_BIN1_PIN);
    GPIO_SetBits( MG310_TB6612_AIN2_PORT, MG310_TB6612_BIN2_PIN);
}

/**
  * 函    数：控制两个电机转动，使其让小车制动
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动
  */
static void MG310_Motor_Brake(void)
{
    MG310_Motor_LeftBrake();
    MG310_Motor_RightBrake();
}

/**
  * 函    数：控制电机左转动，使其让小车停止
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动
  */
static void MG310_Motor_LeftStop(void)
{
    GPIO_ResetBits( MG310_TB6612_AIN1_PORT, MG310_TB6612_AIN1_PIN);
    GPIO_ResetBits( MG310_TB6612_AIN2_PORT, MG310_TB6612_AIN2_PIN);
}

/**
  * 函    数：控制右电机转动，使其让小车停止
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动
  */
static void MG310_Motor_RightStop(void)
{
    GPIO_ResetBits( MG310_TB6612_BIN1_PORT, MG310_TB6612_BIN1_PIN);
    GPIO_ResetBits( MG310_TB6612_BIN2_PORT, MG310_TB6612_BIN2_PIN);
}

/**
  * 函    数：控制电机转动，使其让小车停止
  * 参    数：无
  * 返 回 值：无
  * 说    明：改变连接TB6612电机驱动芯片AIN1、AIN2、BIN1、BIN2引脚的GPIO口的电平，使TB6612控制电机转动
  */
static void MG310_Motor_Stop(void)
{
    MG310_Motor_LeftStop();
    MG310_Motor_RightStop();
}

/**
  * 函    数：设置左电机的转速
  * 参    数：Speed        电机转动速度，其小于等于 MOTOR_OUT_MAX
  * 返 回 值：
  * 说    明：输出给电机的PWM信号的占空比Duty = Speed / ARR
  */
void MG310_Motor_SetLeftVelocity(int16_t Speed)
{
    MG310_Motor_LeftStop();         //  使左电机停止转动
    if(Speed > 0)                   //  Speed大于0表示前进
    {
        MG310_Motor_LeftAdvance();  //  控制左电机转动方向，使小车前进
    }
    else if (Speed < 0)             //  Speed小于0表示前进
    {
        MG310_Motor_LeftBack();     //  控制左电机转动方向，使小车后退
        Speed = -Speed;             //  使Speed等于其绝对值
    }
    else                            //  Speed等于0表示停止
    {
        MG310_Motor_LeftBrake();    //  控制左电机转动，使小车停止
    }
    MG310_TB6612_PWMA_TIM_SetComparex( MG310_TB6612_PWMA_TIM, Speed);   //  配置输出给电机的PWM信号的占空比
}

/**
  * 函    数：设置右电机的转速
  * 参    数：Speed        电机转动速度，其小于等于 MOTOR_OUT_MAX
  * 返 回 值：
  * 说    明：输出给电机的PWM信号的占空比Duty = Speed / ARR
  */
void MG310_Motor_SetRightVelocity(int16_t Speed)
{
    MG310_Motor_RightStop();        //  使左电机停止转动
    if(Speed > 0)                   //  Speed大于0表示前进
    {
        MG310_Motor_RightAdvance(); //  控制右电机转动方向，使小车前进
    }
    else if (Speed < 0)             //  Speed小于0表示前进
    {
        MG310_Motor_RightBack();    //  控制右电机转动方向，使小车后退
        Speed = -Speed;             //  使Speed等于其绝对值
    }
    else                            //  Speed等于0表示停止
    {
        MG310_Motor_RightBrake();   //  控制右电机转动，使小车停止
    }
    MG310_TB6612_PWMB_TIM_SetComparex( MG310_TB6612_PWMB_TIM, Speed);   //  配置输出给电机的PWM信号的占空比
}

/**
  * 函    数：设置两个电机的转速
  * 参    数：Speed        电机转动速度，其小于等于 MOTOR_OUT_MAX
  * 返 回 值：
  * 说    明：输出给电机的PWM信号的占空比Duty = Speed / ARR
  */
void MG310_Motor_SetVelocity(int16_t Speed)
{
    MG310_Motor_Stop(); //  使电机停止转动
    
    /*对Speed的大小进行限制*/
    if(Speed > MOTOR_OUT_MAX)   
        Speed = MOTOR_OUT_MAX;
    if(Speed < -MOTOR_OUT_MAX)
        Speed = -MOTOR_OUT_MAX;
    
    
    if(Speed > 0)               //  Speed大于0表示前进
    {
        MG310_Motor_Advance();  //  控制电机转动方向，使小车前进
    }
    else if (Speed < 0)         //  Speed小于0表示前进
    {
        MG310_Motor_Back();     //  控制电机转动方向，使小车后退
        Speed = -Speed;         //  使Speed等于其绝对值
    }
    else                        //  Speed等于0表示停止
    {
        MG310_Motor_Brake();    //  控制电机转动，使小车停止
    }
    MG310_TB6612_PWMA_TIM_SetComparex( MG310_TB6612_PWMA_TIM, Speed);   //  配置输出给电机的PWM信号的占空比
    MG310_TB6612_PWMB_TIM_SetComparex( MG310_TB6612_PWMB_TIM, Speed);   //  配置输出给电机的PWM信号的占空比
}

/**
  * 函    数：初始化MG310电机和编码器
  * 参    数：无
  * 返 回 值：无
  * 说    明：初始化了利用到的定时器和GPIO口
  */
void MG310_Init(void)
{
    MG310_Encoder_GPIO_Config();    //  初始化编码器利用的GPIO口
    MG310_Motor_GPIO_Config();      //  初始化电机利用的GPIO口

    MG310_Encoder_TIM_Mode_Config();//  初始化编码器利用的定时器功能
    MG310_Motor_TIM_Mode_Config();  //  初始化电机利用的定时器功能
}

/**
  * 函    数：关闭MG310电机和编码器
  * 参    数：无
  * 返 回 值：无
  * 说    明：关闭了相关外设的时钟，并失能了相关外设
  */
void MG310_Close(void)
{
    MG310_Encoder_Close();  //  关闭编码器
    MG310_Motor_Close();    //  关闭电机
}

/*********************功能函数*/
