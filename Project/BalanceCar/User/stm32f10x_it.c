/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI
  
  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "./exti/bsp_exti.h" 
#include "./i2c/bsp_i2c.h"
#include "./MG310/MG310.h"
#include "./BalanceCar/BalanceCar.h"
#include "./MPU6050/MPU6050.h"
#include "./Timer/Timer.h"
#include "./MSG/MSG.h"
#include "./led/bsp_led.h"
  #include "./BlueTooth/BlueTooth.h"

unsigned int Task_Delay[NumOfTask]={0};
extern void TimingDelay_Decrement(void);
extern void TimeStamp_Increment(void);


/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{

  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    //  SysTick(系统定时器)每毫秒会执行一次改函数，用于计时(MPU6050模块用到)
	TimeStamp_Increment();	
}

/**
  * @brief  蓝牙模块的串口中断函数(用于接收蓝牙主机发送过来的数据包)
  * @param  None
  * @retval None
  */
void BLT_USART_IRQHandler(void)
{
    /*  一个数据包的格式如下： */
    /*  开头为字符@，中间为数据载荷(最多有BLT_BUF_SIZE个字节)，末尾为\r\n*/
    
    static uint8_t BLT_RxState = 0;	    //	状态位
	static uint8_t BLT_pRxPacket = 0;	//	载荷数据读取次数
	if(SET == USART_GetITStatus( BLT_USARTx, USART_IT_RXNE))
	{
		uint8_t RxData = USART_ReceiveData(BLT_USARTx); //  接收字节数据
		if(BLT_RxState == 0)	    //	接收包头的状态
		{
			if(RxData == '@' && BLT_USART_RxFlag == 0)	//	如果接收到了指定的包头(字符@)，并且数据包接收完毕标识为1
			{
				BLT_RxState = 1;    //  转为接收载荷的状态
				BLT_pRxPacket = 0;  //  清零载荷数据读取次数
			}
		}
		else if(BLT_RxState == 1)	//	接收载荷的状态
		{
			if(RxData == '\r')      //  如果收到了包尾的第一个字符(字符\r)
			{	
				BLT_RxState = 2;    //  转为接收包尾(的第二个字符)的状态
			}
			else                    //  如果收到了数据载荷
			{
				BLT_USART_RxPacket[BLT_pRxPacket] = RxData; //  将接收到的字节数据放入接收数据缓冲区
				++BLT_pRxPacket;                            //  增加载荷数据读取次数
			}
		}
		else if(BLT_RxState == 2)	//	接收包尾(的第二个字符)的状态
		{
			if(RxData == '\n')      //  如果接收到了包尾的第一个字符(字符\n)
			{
#ifdef  USE_MSG     //  如果使用了MSG模块
                MSG msg;                                        //  消息变量，用于存放一个消息
                msg.count = 1;                                  //  初始化消息的重复计数为1(此处不计数也行，因为MSG_SendMessage函数会处理)
                msg.message = CM_BLTCMD;                        //  设置消息为蓝牙指令消息
                msg.param = *(uint32_t*)(BLT_USART_RxPacket);   //  将接收数据缓冲区(字符串)的首地址强制转换位 uint32_t* 类型以获取32位的蓝牙指令
#endif             
				BLT_RxState = 0;
				BLT_USART_RxFlag = 1;                           //  将数据包接收完毕标识置为1
#ifdef  USE_MSG      //  如果使用了MSG模块          
                MSG_SendMessage(&msg);	//	发送蓝牙指令消息
                BLT_USART_RxFlag = 0;   //  因为发送了蓝牙指令消息，对接收到的蓝牙指令完成了处理，所以将数据包接收完毕标识置为0
#else
                //  如果你没有使用我写的MSG模块
                //  你可以在此处对接收到的蓝牙指令进行处理
#endif 
			}
		}
		USART_ClearITPendingBit( BLT_USARTx, USART_IT_RXNE);
	} 
}

/**
  * @brief  外部中断函数
  * @param  None
  * @retval None
  */
void EXTI_INT_FUNCTION (void)
{
	if(EXTI_GetITStatus(EXTI_LINE) != RESET) 
	{
        //  不进行任何处理( 因为实际上没有用到任何外部中断 )
		EXTI_ClearITPendingBit(EXTI_LINE);     //清除中断标志位
	}  
}

/**
  * @brief  Timer模块的定时器中断函数
  * @param  None
  * @retval None
  */
void Timer_TIM_IRQHandler(void)
{
    if(SET == TIM_GetITStatus( Timer_TIM, TIM_IT_Update))
	{
		uint8_t i = 0;          //  循环计数变量
        uint8_t timer_length;   //  临时变量，用于记录定时列表的长度
		MSG msg;                //  消息变量，用于存放一个消息
		Timer* p_timer;         //  临时指针变量，用于指向定时器结构体
		
		timer_length = Timer_GetLength();   //  获取定时列表的长度
		msg.message = CM_TIMEUP;	        //	设置发送定时器到时了的消息
		for(i=0;i<timer_length;++i)	        //	扫描定时列表
		{
            p_timer = Timer_GetStatus(i);   //  获取第 i 个定时器结构体
			
			if( (p_timer->isRun==1) && (p_timer->Function != NULL) )	//	如果列表中的第i个位置配置了函数且配置为定时
			{
				++(p_timer->Count);	                                    //	使其当前定时值加一
				if( p_timer->Count >= p_timer->Target)	                //	如果当前定时值大于等于目标定时值
				{
					p_timer->Count = 0;	                                //	清零当前定时值
					msg.param = (uint32_t)(p_timer->Function);	        //	使消息参数为该定时器结构体中存放的函数的指针
					MSG_SendMessage(&msg);	                            //	发送消息
				}
			}
		}
        
        TIM_ClearITPendingBit( Timer_TIM, TIM_IT_Update);
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
