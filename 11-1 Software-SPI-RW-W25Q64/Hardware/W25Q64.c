#include "W25Q64.h"
#include "stm32f10x.h"                  // Device header
#include "MySPI.h"
#include "W25Q64_Ins.h"

void W25Q64_Init(void)
{
	MySPI_Init();	//	初始化软件SPI底层
}

/**
  *@brief		读取芯片ID
  *@param		MID	存放厂商ID变量的指针
  *@param		DID	存放设备ID变量的指针
  *@retval		无
  **/
void W25Q64_ReadID(uint8_t *MID,uint16_t *DID)
{
	MySPI_Start();	//	开始
	MySPI_SwapByte(W25Q64_JEDEC_ID);	//	发送读取ID的指令
	*MID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//	接收厂商ID
	*DID = MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//	接收设备ID高8位，表示存储器类型
	*DID <<= 8;	//	将读取的高8位数据放到高位，顺便初始化低8位数据(全0)
	*DID |= MySPI_SwapByte(W25Q64_DUMMY_BYTE);	//	接收设备ID低8位，表示容量
	MySPI_Stop();	//	结束
}

/**
  *@brief		发送写使能指令
  *@param		无
  *@retval		无
  **/
void W25Q64_WriteEnable(void)
{
	MySPI_Start();	//	开始
	MySPI_SwapByte(W25Q64_WRITE_ENABLE);	//	发送写使能的指令
	MySPI_Stop();	//	结束
}

/**
  *@brief		等待BUSY忙状态，知道不处于忙状态
  *@param		无
  *@retval		无
  **/
void W25Q64_WaitBusy(void)
{
	uint32_t Timeout = 0;
	MySPI_Start();	//	开始
	MySPI_SwapByte(W25Q64_READ_STATUS_REGISTER_1);	//	发送读取状态寄存器1的指令
	Timeout = 100000;
	while(0x01 == (MySPI_SwapByte(W25Q64_DUMMY_BYTE) & 0x01))//	用掩码取出最低位BUSY，如果为一，表示忙，需要等待 
	{
		--Timeout;
		if(Timeout == 0)
			break;	//	防止意外导致程序卡死
	}
	MySPI_Stop();	//	结束
}

/**
  *@brief		页编程(写入)
  *@param		Address		数据写入位置的地址
  *@param		DataArray	数据数组(存放写入数据)
  *@param		Count		要写入几字节数据
  *@retval		无
  **/
void W25Q64_PageProgram(uint32_t Address,uint8_t *DataArray,uint16_t Count)
{
	uint16_t i = 0;	//	循环计数变量
	W25Q64_WriteEnable();	//	写使能
	MySPI_Start();	//	开始
	MySPI_SwapByte(W25Q64_PAGE_PROGRAM);	//	发送页编程的指令
	MySPI_SwapByte(Address>>16);	//	发送地址高8位
	MySPI_SwapByte(Address>>8);		//	发送地址中8位(小端模式，会自动舍弃高位)
	MySPI_SwapByte(Address);		//	发送地址低8位(小端模式，会自动舍弃高位)
	for(i=0;i<Count;++i)
	{
		MySPI_SwapByte(DataArray[i]);
	}
	MySPI_Stop();	//	结束
	W25Q64_WaitBusy();	//	等待忙状态结束
}

/**
  *@brief		扇区擦除
  *@param		Address		待擦除扇区内的地址
  *@retval		
  **/
void W25Q64_SectorErase(uint32_t Address)
{
	W25Q64_WriteEnable();	//	写使能
	MySPI_Start();	//	开始
	MySPI_SwapByte(W25Q64_SECTOR_ERASE_4KB);	//	发送扇区擦除的指令
	MySPI_SwapByte(Address>>16);	//	发送地址高8位
	MySPI_SwapByte(Address>>8);		//	发送地址中8位(小端模式，会自动舍弃高位)
	MySPI_SwapByte(Address);		//	发送地址低8位(小端模式，会自动舍弃高位)
	MySPI_Stop();	//	结束
	W25Q64_WaitBusy();	//	等待忙状态结束
}

/**
  *@brief		读取数据
  *@param		Address		数据读取位置的地址
  *@param		DataArray	数据数组(存放读取数据)
  *@param		Count		要读取几字节数据
  *@retval		无
  **/
void W25Q64_ReadData(uint32_t Address,uint8_t *DataArray,uint32_t Count)
{
	uint32_t i = 0;	//	循环计数变量
	MySPI_Start();	//	开始
	MySPI_SwapByte(W25Q64_READ_DATA);	//	发送页编程的指令
	MySPI_SwapByte(Address>>16);	//	发送地址高8位
	MySPI_SwapByte(Address>>8);		//	发送地址中8位(小端模式，会自动舍弃高位)
	MySPI_SwapByte(Address);		//	发送地址低8位(小端模式，会自动舍弃高位)
	for(i=0;i<Count;++i)
	{
		DataArray[i] = MySPI_SwapByte(W25Q64_DUMMY_BYTE);
	}
	MySPI_Stop();	//	结束
}
