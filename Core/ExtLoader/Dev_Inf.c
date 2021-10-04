/**
 * 	Reference:
 *
 * 	1. C:\Program Files\STMicroelectronics\STM32Cube\STM32CubeProgrammer\bin\ExternalLoader
 * 		IS61WV51216BLL_STM3210E-EVAL\
 * 		M25P64_STM3210E-EVAL\
 * 		M29W128GL_STM3210E-EVAL\
 * 		N25Q256A_STM32L476G-EVAL\
 *
 * 	2. https://github.com/STMicroelectronics/stm32-external-loader
 */

#include "Dev_Inf.h"

#include "W25Qx.h"

// MEMORY_FLASH_SIZE:	Value is shown in STM32CubeProgrammer::Externaloders::Memory Size
// MEMORY_BLOCK_SIZE:	Not used in STM32CubeProgrammer
// MEMORY_SECTOR_SIZE:	Minimum unit of erasing. Value is shown in STM32CubeProgrammer::Erasing-Programming::EraseExternalMemory::Size
// MEMORY_PAGE_SIZE:	Minimum unit of programming. Value is shown in STM32CubeProgrammer::Externaloders::Page Size
#define MEMORY_FLASH_SIZE	W25Q128FV_FLASH_SIZE
#define MEMORY_BLOCK_SIZE	W25Q128FV_SECTOR_SIZE
#define MEMORY_SECTOR_SIZE	W25Q128FV_SUBSECTOR_SIZE
#define MEMORY_PAGE_SIZE	W25Q128FV_PAGE_SIZE

/* This structure contains information used by ST-LINK Utility/STM32CubeProgrammer to program and erase the device */
#if defined (__ICCARM__)
__root struct StorageInfo const StorageInfo  =  {
#else
//struct StorageInfo const StorageInfo = {
struct StorageInfo const StorageInfo __attribute__((section(".storageinfo"))) __attribute__((aligned(4))) =  {
#endif

#ifdef _DBG
		"W25Q16-SPI_NUCLEOG071-DBG",		// Device Name + version number
#else
		"W25Q128-SPI_NUCLEOG071",			// Device Name + version number
#endif

		NOR_FLASH,                  		 // Device Type
		0x90000000,                			 // Device Start Address
		MEMORY_FLASH_SIZE,                 	 // Device Size in Bytes
		MEMORY_PAGE_SIZE,                    // Programming Page Size
		0xFF,                                // Initial Content of Erased Memory

		// Specify Size and Address of Sectors (view example below)
		{
				{ (MEMORY_FLASH_SIZE / MEMORY_SECTOR_SIZE),  	// Sector Numbers,
				  (uint32_t) MEMORY_SECTOR_SIZE },       		// Sector Size
				{ 0x00000000, 0x00000000 }
		}

};
