/**
 * 	Reference:
 *
 * 	1. MOOC
 * 	   https://www.st.com/content/st_com/en/support/learning/stm32-education/stm32-moocs/external_QSPI_loader.html
 * 	   Custom_Loader/Core/Src/Loader_Src.c
 *
 * 	2. STM32CubeProgrammer
 * 	   C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader
 * 	   IS61WV51216BLL_STM3210E-EVAL\
 * 	   M25P64_STM3210E-EVAL\
 * 	   M29W128GL_STM3210E-EVAL\
 * 	   N25Q256A_STM32L476G-EVAL\
 *
 * 	3. Github
 * 	   https://github.com/STMicroelectronics/stm32-external-loader
 */

/* Includes ------------------------------------------------------------------*/
#include "Loader_Src.h"

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  return HAL_OK;
}

uint32_t HAL_GetTick(void)
{
  return 1;
}

int itoa(int value,char *ptr)
{
	int count=0,temp;
	if(ptr==NULL)
		return 0;
	if(value==0)
	{
		*ptr='0';
		return 1;
	}

	if(value<0)
	{
		value*=(-1);
		*ptr++='-';
		count++;
	}
	for(temp=value;temp>0;temp/=10,ptr++);
	*ptr='\0';
	for(temp=value;temp>0;temp/=10)
	{
		*--ptr=temp%10+'0';
		count++;
	}
	return count;
}

/**
 * @brief  System initialization.
 * @param  None
 * @retval  LOADER_OK = 1	: Operation succeeded
 * @retval  LOADER_FAIL = 0	: Operation failed
 */
int Init(void)
{
  //extern uint8_t _sbss;
  //memset((void*)&_sbss, 0, 524);	//Zero Bss

  SystemInit();
  HAL_Init();
  SystemClock_Config();
  HAL_SuspendTick();

  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_SPI2_Init();

  DBGMSG("\r\nEntering %s\n\r", __func__);

  DBGMSG("\tBSP_W25Qx_Init\n\r");
  if (BSP_W25Qx_Init() != W25Qx_OK)
  {
	return LOADER_FAIL;
  }

  DBGMSG("\rEnd of %s\n\r", __func__);

  return LOADER_OK;
}

/**
 * @brief   Program memory.
 * @param   Address: page address
 * @param   Size   : size of data
 * @param   buffer : pointer to data buffer
 * @retval  LOADER_OK = 1		: Operation succeeded
 * @retval  LOADER_FAIL = 0	: Operation failed
 */
int Write(uint32_t Address, uint32_t Size, uint8_t* buffer)
{
  DBGMSG("\rEntering %s\n\r", __func__);
  DBGMSG("\tAddress = 0x%X\n\r", (unsigned int) Address);
  DBGMSG("\tSize = 0x%08X\n\r", (unsigned int) Size);

  uint32_t WriteAddress = Address & (0x0fffffff);
  DBGMSG("\tWriteAddress = 0x%X\n\r", (unsigned int) WriteAddress);

  if (BSP_W25Qx_Write((uint8_t*) buffer, WriteAddress ,Size) != W25Qx_OK)
  {
	return LOADER_FAIL;
  }
  DBGMSG("\rEnd of %s\n\r", __func__);
  return LOADER_OK;
}

/**
 * @brief   Sector erase.
 * @param   EraseStartAddress :  erase start address
 * @param   EraseEndAddress   :  erase end address
 * @retval  LOADER_OK = 1		: Operation succeeded
 * @retval  LOADER_FAIL = 0	: Operation failed
 */
int SectorErase(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
  DBGMSG("\rEntering %s\n\r", __func__);
  DBGMSG("\tEraseStartAddress = 0x%x\n\r", (unsigned int)EraseStartAddress );
  DBGMSG("\tEraseEndAddress = 0x%x\n\r", (unsigned int)EraseEndAddress );

  uint32_t SectorAddr;
  EraseStartAddress &= 0x0FFFFFFF;
  EraseEndAddress &= 0x0FFFFFFF;
  EraseStartAddress = EraseStartAddress -  EraseStartAddress % 0x1000;

  DBGMSG("\t************************\n\r");

  while (EraseEndAddress>=EraseStartAddress)
  {
	SectorAddr = EraseStartAddress;
	DBGMSG("\tErasing SectorAddr 0x%x: ", (unsigned int)SectorAddr);

	/*Erases the specified sector*/
	if (BSP_W25Qx_Erase_Block(SectorAddr) != W25Qx_OK)
	{
	  DBGMSG("Error after BSP_W25Qx_Erase_Block!\n\r");
	  return LOADER_FAIL;
	}
	DBGMSG("OK\n\r");
	EraseStartAddress+=0x1000;
  }

  DBGMSG("SectorErase OK\n\n\r");
  return LOADER_OK;
}

/**
 * Description :
 * Mass erase of external flash area
 * Optional command - delete in case usage of mass erase is not planed
 * Inputs    :
 *      none
 * outputs   :
 *     none
 * Note: Optional for all types of device
 */
int MassErase(void)
{
  DBGMSG("\rEntering %s\n\r", __func__);

  /*Erases the entire SPI memory*/
  BSP_W25Qx_Erase_Chip();

  return LOADER_OK;
}

#if 0
/**
 * Description :
 * Calculates checksum value of the memory zone
 * Inputs    :
 *      StartAddress  : Flash start address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Checksum value
 * Note: Optional for all types of device
 */
uint32_t CheckSum(uint32_t StartAddress, uint32_t Size, uint32_t InitVal) {
	uint8_t missalignementAddress = StartAddress % 4;
	uint8_t missalignementSize = Size;
	int cnt;
	uint32_t Val;

	StartAddress -= StartAddress % 4;
	Size += (Size % 4 == 0) ? 0 : 4 - (Size % 4);

	for (cnt = 0; cnt < Size; cnt += 4) {
		Val = *(uint32_t*) StartAddress;
		if (missalignementAddress) {
			switch (missalignementAddress) {
			case 1:
				InitVal += (uint8_t) (Val >> 8 & 0xff);
				InitVal += (uint8_t) (Val >> 16 & 0xff);
				InitVal += (uint8_t) (Val >> 24 & 0xff);
				missalignementAddress -= 1;
				break;
			case 2:
				InitVal += (uint8_t) (Val >> 16 & 0xff);
				InitVal += (uint8_t) (Val >> 24 & 0xff);
				missalignementAddress -= 2;
				break;
			case 3:
				InitVal += (uint8_t) (Val >> 24 & 0xff);
				missalignementAddress -= 3;
				break;
			}
		} else if ((Size - missalignementSize) % 4 && (Size - cnt) <= 4) {
			switch (Size - missalignementSize) {
			case 1:
				InitVal += (uint8_t) Val;
				InitVal += (uint8_t) (Val >> 8 & 0xff);
				InitVal += (uint8_t) (Val >> 16 & 0xff);
				missalignementSize -= 1;
				break;
			case 2:
				InitVal += (uint8_t) Val;
				InitVal += (uint8_t) (Val >> 8 & 0xff);
				missalignementSize -= 2;
				break;
			case 3:
				InitVal += (uint8_t) Val;
				missalignementSize -= 3;
				break;
			}
		} else {
			InitVal += (uint8_t) Val;
			InitVal += (uint8_t) (Val >> 8 & 0xff);
			InitVal += (uint8_t) (Val >> 16 & 0xff);
			InitVal += (uint8_t) (Val >> 24 & 0xff);
		}
		StartAddress += 4;
	}

	return (InitVal);
}

/**
 * Description :
 * Verify flash memory with RAM buffer and calculates checksum value of
 * the programmed memory
 * Inputs    :
 *      FlashAddr     : Flash address
 *      RAMBufferAddr : RAM buffer address
 *      Size          : Size (in WORD)
 *      InitVal       : Initial CRC value
 * outputs   :
 *     R0             : Operation failed (address of failure)
 *     R1             : Checksum value
 * Note: Optional for all types of device
 */
uint64_t Verify(uint32_t MemoryAddr, uint32_t RAMBufferAddr, uint32_t Size,uint32_t missalignement){

	HAL_ResumeTick();
	uint32_t VerifiedData = 0, InitVal = 0;
	uint64_t checksum;
	Size *= 4;

	if (CSP_QSPI_EnableMemoryMappedMode() != HAL_OK)
	{
		HAL_SuspendTick();
		return LOADER_FAIL;
	}

	checksum = CheckSum((uint32_t) MemoryAddr + (missalignement & 0xf),
			Size - ((missalignement >> 16) & 0xF), InitVal);
	while (Size > VerifiedData) {
		if (*(uint8_t*) MemoryAddr++
				!= *((uint8_t*) RAMBufferAddr + VerifiedData)){
			HAL_SuspendTick();
			return ((checksum << 32) + (MemoryAddr + VerifiedData));
		}
		VerifiedData++;
	}

	HAL_SuspendTick();
	return (checksum << 32);
}
#endif

/*******************************************************************************
 Description :
 Read data from the device
 Inputs :
 				Address 	: Read location
 				Size 		  : Length in bytes
 				buffer 		: Address where to get the data to write
 outputs :
 				"1" 		  : Operation succeeded
 				"0" 		  : Operation failure
 Note : Not Mandatory
********************************************************************************/
int Read (uint32_t Address, uint32_t Size, uint8_t* Buffer)
{
	DBGMSG("\r\nEntering %s\n\r", __func__);
	DBGMSG("\tAddress = 0x%x\n\r", (unsigned int) Address);
	DBGMSG("\tSize = 0x%x\n\r", (unsigned int) Size);

	uint32_t StartAddress = Address & 0x0FFFFFFF;
	DBGMSG("\tStartAddress = 0x%x\n\r", (unsigned int) StartAddress);

	BSP_W25Qx_Read(Buffer, StartAddress, Size);
	DBGMSG("\rEnd of %s\n\n\r", __func__);
	return 1;
}
