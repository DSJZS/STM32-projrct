#include "MyFLASH.h"
#include "stm32f10x.h"                  // Device header

/**
  *@brief		读取指定地址处的字数据(32位数据)
  *@param		Address	指定地址
  *@retval		指定地址的字数据(32位数据)
  **/
uint32_t MyFLASH_ReadWord(uint32_t Address)
{
	return *((__IO uint32_t *)(Address));
}

/**
  *@brief		读取指定地址处的半字数据(16位数据)
  *@param		Address	指定地址
  *@retval		指定地址的半字数据(16位数据)
  **/
uint16_t MyFLASH_ReadHalfWord(uint32_t Address)
{
	return *((__IO uint16_t *)(Address));
}

/**
  *@brief		读取指定地址处的字节数据(8位数据)
  *@param		Address	指定地址
  *@retval		指定地址的字节数据(8位数据)
  **/
uint8_t MyFLASH_ReadByte(uint32_t Address)
{
	return *((__IO uint8_t *)(Address));
}

/**
  *@brief		擦除FLASH所有页
  *@param		无
  *@retval		无
  **/
void MyFLASH_EraseAllPages(void)
{
	FLASH_Unlock();	//	解锁FLASH
	FLASH_EraseAllPages();	//	擦除FLASH所有页
	FLASH_Lock();	//	加锁FLASH
}

/**
  *@brief		擦除FLASH指定地址处的页
  *@param		PageAddress	指定地址
  *@retval		无
  **/
void MyFLASH_ErasePage(uint32_t PageAddress)
{
	FLASH_Unlock();	//	解锁FLASH
	FLASH_ErasePage(PageAddress);	//	擦除FLASH指定地址处的页
	FLASH_Lock();	//	加锁FLASH
}

/**
  *@brief		在FLASH指定地址处进行字编程(写入32位数据)
  *@param		Address	指定地址
  *@param		Data	字数据(32位数据)
  *@retval		无
  **/
void MyFLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
	FLASH_Unlock();	//	解锁FLASH
	FLASH_ProgramWord( Address, Data);	//	在FLASH指定地址处进行字编程(写入32位数据)
	FLASH_Lock();	//	加锁FLASH
}

/**
  *@brief		在FLASH指定地址处进行半字编程(写入16位数据)
  *@param		Address	指定地址
  *@param		Data	半字数据(16位数据)
  *@retval		无
  **/
void MyFLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
	FLASH_Unlock();	//	解锁FLASH
	FLASH_ProgramHalfWord( Address, Data);	//	在FLASH指定地址处进行半字编程(写入16位数据)
	FLASH_Lock();	//	加锁FLASH
}
