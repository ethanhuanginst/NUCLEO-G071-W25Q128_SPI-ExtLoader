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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#include <string.h>

#include "W25Qx.h"
#include "Dev_Inf.h"

/* Private typedef -----------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/

#define LOADER_OK	0x1
#define LOADER_FAIL	0x0

//extern QSPI_HandleTypeDef hqspi;
extern SPI_HandleTypeDef hspi2;
extern UART_HandleTypeDef huart2;

void SystemClock_Config(void);

char itoa_string[10];
int itoa(int value,char *ptr);

int Write (uint32_t Address, uint32_t Size, uint8_t* buffer);
int SectorErase (uint32_t EraseStartAddress ,uint32_t EraseEndAddress);
int MassErase(void);
int Init (void);
int Read (uint32_t Address, uint32_t Size, uint8_t* Buffer);
