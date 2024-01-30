#ifndef __BLT_H
#define	__BLT_H


#include "stm32f10x.h"
/*  一个数据包的格式如下： */
/*  开头为字符@，中间为数据载荷(最多有BLT_BUF_SIZE个字节)，末尾为\r\n*/
    




/*变量声明*********************/

extern char BLT_USART_RxPacket[];   //  声明接收数据缓冲区
extern uint8_t BLT_USART_RxFlag;    //  声明数据包接收完毕标识

/*********************变量声明*/





/*宏参数定义*********************/

#define  BLT_BUF_SIZE                 100                       //  接收数据缓冲区大小(单位字节)

#define  BLT_USARTx                   USART1                    //  蓝牙模块用到的USART外设
#define  BLT_USART_CLK                RCC_APB2Periph_USART1     //  蓝牙模块用到的USART外设的时钟
#define  BLT_USART_APBxClkCmd         RCC_APB2PeriphClockCmd    //  蓝牙模块用到的USART外设的时钟使能函数
#define  BLT_USART_BAUDRATE           9600                      //  蓝牙模块用到的USART外设所要配置的波特率

#define  BLT_USART_GPIO_CLK           (RCC_APB2Periph_GPIOA)    //  蓝牙模块用到的USART外设所要用到的GPIO口的时钟
#define  BLT_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd    //  蓝牙模块用到的USART外设所要用到的GPIO口的时钟使能函数
    
#define  BLT_USART_TX_GPIO_PORT       GPIOA                     //  蓝牙模块用到的USART外设TX引脚所要用到的GPIO口
#define  BLT_USART_TX_GPIO_PIN        GPIO_Pin_9                //  蓝牙模块用到的USART外设TX引脚所要用到的GPIO口引脚
#define  BLT_USART_RX_GPIO_PORT       GPIOA                     //  蓝牙模块用到的USART外设RX引脚所要用到的GPIO口
#define  BLT_USART_RX_GPIO_PIN        GPIO_Pin_10               //  蓝牙模块用到的USART外设RX引脚所要用到的GPIO口引脚

#define  BLT_USART_IRQ                USART1_IRQn               //  蓝牙模块用到的USART外设的中断号
#define  BLT_USART_IRQHandler         USART1_IRQHandler         //  蓝牙模块用到的USART外设的中断函数

/*宏参数定义*********************/





/*函数声明*********************/
void BLT_Init(void);
void BLT_SendByte( uint8_t ch);
void BLT_SendArray( uint8_t *array, uint16_t num);
void BLT_SendString( char *str);
void BLT_SendHalfWord( uint16_t ch);
void BLT_SendCommand( uint32_t Command);
/*函数声明*********************/

#endif /* __USART_H */
