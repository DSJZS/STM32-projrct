/***************************************************************************************
  * 本源文件含有蓝牙相关函数
  * 使用那个USART外设，发送的消息均可在对应的地方自定义
  ***************************************************************************************
  */
  
#include "BlueTooth.h"
#include "./usart/bsp_usart.h"

/*全局变量*********************/

char BLT_USART_RxPacket[BLT_BUF_SIZE] = {0};	//	接收数据缓冲区
uint8_t BLT_USART_RxFlag = 0;	                //	接收标志位

/*********************全局变量*/





/*功能函数*********************/

 /**
  * @brief  对蓝牙模块用到的GPIO口进行配置
  * @param  无
  * @retval 无
  */
static void BLT_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 打开串口GPIO的时钟
	BLT_USART_GPIO_APBxClkCmd(BLT_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	BLT_USART_APBxClkCmd(BLT_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = BLT_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BLT_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = BLT_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(BLT_USART_RX_GPIO_PORT, &GPIO_InitStructure);
}
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void BLT_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
	/* 提示 NVIC_PriorityGroupConfig() 在整个工程只需要调用一次来配置优先级分组*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = BLT_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  对蓝牙模块用到的USART的工作参数进行配置
  * @param  无
  * @retval 无
  */
static void BLT_USART_Config(void)
{
	
	USART_InitTypeDef USART_InitStructure;
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = BLT_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(BLT_USARTx, &USART_InitStructure);
	
	// 使能串口接收中断
	USART_ITConfig(BLT_USARTx, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(BLT_USARTx, ENABLE);	    
}

/**
  * 函    数：初始化蓝牙模块
  * 参    数：无
  * 返 回 值：无
  * 说    明：初始化其用到的GPIO和USART外设
  */
void BLT_Init(void)
{
    BLT_GPIO_Config();      //  初始化GPIO口
    BLT_NVIC_Config();      //  初始化NVIC
    BLT_USART_Config();     //  初始化USART
}

/**
  * 函    数：蓝牙模块发送一个字节
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用USART外设发送一个字节
  */
void BLT_SendByte( uint8_t ch)
{
    Usart_SendByte( BLT_USARTx, ch);
}

/**
  * 函    数：蓝牙模块发送数组(元素大小为一字节)
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用USART外设发送数组(元素大小为一字节)
  */
void BLT_SendArray( uint8_t *array, uint16_t num)
{
    Usart_SendArray( BLT_USARTx, array, num);
}

/**
  * 函    数：蓝牙模块发送字符串
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用USART外设发送字符串
  */
void BLT_SendString( char *str)
{
    Usart_SendString( BLT_USARTx, str);
}

/**
  * 函    数：蓝牙模块发送半字(两个字节大小的数据)
  * 参    数：无
  * 返 回 值：无
  * 说    明：利用USART外设发送半字(两个字节大小的数据)
  */
void BLT_SendHalfWord( uint16_t ch)
{
	Usart_SendHalfWord( BLT_USARTx, ch);
}

/**
  * 函    数：发送指令
  * 参    数：Command      蓝牙指令，为一个32位的变量
  * 返 回 值：无
  * 说    明：32位指令是先发低位字节，再发高位字节
  */
void BLT_SendCommand( uint32_t Command)
{
/*  一个数据包的格式如下： */
/*  包头为字符@，中间为数据载荷(最多有BLT_BUF_SIZE个字节)，包尾为\r\n*/
    BLT_SendByte('@');  //  发送包头
    
    BLT_SendByte( (uint8_t)( ( Command & 0x000000FF ) >>  0 ) );    //  发送数据载荷
    BLT_SendByte( (uint8_t)( ( Command & 0x0000FF00 ) >>  8 ) );
    BLT_SendByte( (uint8_t)( ( Command & 0x00FF0000 ) >> 16 ) );
    BLT_SendByte( (uint8_t)( ( Command & 0xFF000000 ) >> 24 ) );
    
    BLT_SendByte('\r'); //  发送包尾
    BLT_SendByte('\n');
}

/*********************功能函数*/
