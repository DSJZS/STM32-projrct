#include "Store.h"
#include "stm32f10x.h"                  // Device header
#include "MyFLASH.h"

#define STORE_START_ADDRESS 0x0800FC00	//	表示Store模块管理的那一页的地址
#define STORE_COUNT			512			//	表示Store模块管理的半字数量(一半字为2字节)

uint16_t Store_Data[STORE_COUNT];	//	暂存FLASH某一页的数据(一共 1KB 数据)，第一个半字数据为标志位，不要写入

/**
  *@brief		初始化Store模块和读取FLASH中某一页数据并存放到SRAM数组中
  *@param		无
  *@retval		无
  **/
void Store_Init(void)
{
	uint16_t i = 0;	//	循环计数变量
	if(MyFLASH_ReadHalfWord(STORE_START_ADDRESS) != 0xA5A5)	//	根据FLASH某一页的第一个半字判断是否第一次使用Store模块
	{
		MyFLASH_ErasePage(STORE_START_ADDRESS);	//	擦除FLASH某一页
		MyFLASH_ProgramHalfWord( STORE_START_ADDRESS, 0xA5A5);	//	写入标志，表示已经使用过一次Store模块
		for(i = 1;i < STORE_COUNT;++i)
		{
			MyFLASH_ProgramHalfWord( STORE_START_ADDRESS+i*2, 0x0000);	//	除了标志，剩余的半字全部初始化为0x0000
		}
	}
	
	for(i = 0;i < STORE_COUNT;++i)
	{
		Store_Data[i] = MyFLASH_ReadHalfWord(STORE_START_ADDRESS+i*2);	//	将FLASH中某一页数据存放到SRAM数组中
	}
}

/**
  *@brief		将SRAM数组数据存放到FLASH某一页
  *@param		无
  *@retval		无
  **/
void Store_Save(void)
{
	uint16_t i = 0;	//	循环计数变量
	MyFLASH_ErasePage(STORE_START_ADDRESS);	//	擦除FLASH某一页
	for(i = 0;i < STORE_COUNT;++i)
	{
		MyFLASH_ProgramHalfWord( STORE_START_ADDRESS+i*2, Store_Data[i]);	//	将SRAM数组数据存放到FLASH某一页
	}
}

/**
  *@brief		清除SRAM数组和FLASH某一页
  *@param		无
  *@retval		无
  **/
void Store_Clear(void)
{
	uint16_t i = 0;	//	循环计数变量
	for(i = 1;i < STORE_COUNT;++i)
	{
		Store_Data[i] = 0x0000;	//	清除SRAM数组
	}
	Store_Save();	//	将数据保存到FLASH某一页
}
