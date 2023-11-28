#include "Motor.h"
#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  *@brief		直流电机初始化
  *@param		无
  *@retval		无
  **/
void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);	//	使能GPIOA时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//	选择推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;	//	选择pin4和pin5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
}

/**
  *@brief		控制直流电机速度
  *@param		Speed 速度值，大于0正转，小于0反转。范围：-100-100 (在CCR为99的情况下)
  *@retval		无
  **/
void Motor_SetSpeed(int8_t Speed)
{
	if(Speed > 100)
		Speed = 100;
	else if(Speed < -100)
		Speed = -100;
	
	if(Speed >= 0)
	{
		GPIO_SetBits( GPIOA, GPIO_Pin_4);	//	设置方向
		GPIO_ResetBits( GPIOA, GPIO_Pin_5);	//	设置方向
		PWM_SetCpmpare3(Speed);		//	配置PWM来控制速度
	}
	else
	{
		GPIO_ResetBits( GPIOA, GPIO_Pin_4);	//	设置方向
		GPIO_SetBits( GPIOA, GPIO_Pin_5);	//	设置方向
		PWM_SetCpmpare3(-Speed);	//	配置PWM来控制速度
	}
	
}
