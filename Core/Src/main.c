/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "W25Qx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi2;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
#ifndef IS_EXTLOADER
#define STARTINGBLOCK	0x00
#define ARRAYSIZE 0x300
uint8_t wData[ARRAYSIZE];
uint8_t rData[ARRAYSIZE];
uint8_t ID[4];
uint32_t i;
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
#ifndef IS_EXTLOADER
  uint8_t SR1;
#endif
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
#ifndef IS_EXTLOADER
  printf("W25Q Pre-Test\n\r");
  printf("=====================================================================\n\r");
  printf("Perform the following sequence:\n\r");
  printf("\tRead W25Q from Sector %d with length 0x%x\n\r", STARTINGBLOCK, ARRAYSIZE);
  printf("\tRead device ID\n\r");
  printf("\tErase Sector 0x%x\n\r", STARTINGBLOCK);
  printf("\tWrite W25Q from Sector %d with length 0x%x\n\r", STARTINGBLOCK, ARRAYSIZE);
  printf("\tRead W25Q again from Sector %d with length 0x%x\n\r", STARTINGBLOCK, ARRAYSIZE);
  printf("\tCompare\n\r");
  printf("=====================================================================\n\r");

  printf("\r\nStandard-SPI-W25Qxxx Example \r\n");
  printf("W25Q Init and Reset\n\r");
  BSP_W25Qx_Init();
//  BSP_W25Qx_Read_ID(ID);
//  printf("\tManufacture: 0x%X\tDevice ID: 0x%x\n\r", ID[0], ID[1]);
//
  BSP_W25Qx_Read_StatusRegister1(&SR1);
  printf("\tStatus Register 1 = 0x%x\n\r\n\r", SR1);

//	/*##-0  Mass Erase #################################*/
//	printf("Perform Chip Erase...\n\r");
//	BSP_W25Qx_Erase_Chip();
//	printf("\tDone!");

	/*##-3- Read the flash     ########################*/
	if(BSP_W25Qx_Read(rData, STARTINGBLOCK * W25Q128FV_SUBSECTOR_SIZE, ARRAYSIZE)== W25Qx_OK)
		//printf("Standard SPI Read ok\r\n\r\n");
		printf("Standard SPI Read Data from Sector %d with length 0x%x: OK!\r\n", STARTINGBLOCK, ARRAYSIZE);
	else
		Error_Handler();

	//printf("Standard SPI Read Data from Sector %d with length 0x%x:\r\n", STARTINGBLOCK, ARRAYSIZE);
	printf("\n\rDisplay Read Data : \r\n");
	for(i =0;i<ARRAYSIZE;i++)
		printf("0x%02X  ",rData[i]);
	printf("\r\n\r\n");

	/*##-1- Read the device ID  ########################*/
	BSP_W25Qx_Init();
	BSP_W25Qx_Read_ID(ID);
	printf("W25Qxxx ID is : ");
	for(i=0;i<2;i++)
	{
		printf("0x%02X ",ID[i]);
	}
	printf("\r\n\r\n");

	/*##-2- Erase Block ##################################*/
//	uint32_t EraseBlockNumber = (ARRAYSIZE / W25Q128FV_SUBSECTOR_SIZE) + 1;
//    for (i=0;i<EraseBlockNumber;i++)
//    {
//    	if(BSP_W25Qx_Erase_Block(i) == W25Qx_OK)
//    		printf(" Standard SPI Erase Block %d ok\r\n", (int)i);
//    	else
//    		Error_Handler();
//    }
	if(BSP_W25Qx_Erase_Block(STARTINGBLOCK) == W25Qx_OK)
		printf("Standard SPI Erase Block %d: OK!\r\n", STARTINGBLOCK);
	else
		Error_Handler();

	/*##-2- Written to the flash ########################*/
	/* Prepare buffer */
	uint32_t loop = ARRAYSIZE / W25Q128FV_PAGE_SIZE;
	for (uint8_t j=0;j<loop;j++)
	{
		for(i =0;i<0x100;i ++)
		{
			  wData[j*0x100+i] = i;
			  rData[j*0x100+i] = 0;
		}

	}

	/* Perform write to flash */
	if(BSP_W25Qx_Write(wData, STARTINGBLOCK * W25Q128FV_SUBSECTOR_SIZE, ARRAYSIZE)== W25Qx_OK)
		//printf(" Standard SPI Write ok\r\n");
		printf("Standard SPI Write Data to Sector %d with length 0x%x: OK!\r\n",STARTINGBLOCK, ARRAYSIZE);
	else
		Error_Handler();

	/*##-3- Read the flash     ########################*/
	if(BSP_W25Qx_Read(rData, STARTINGBLOCK * W25Q128FV_SUBSECTOR_SIZE, ARRAYSIZE)== W25Qx_OK)
		//printf(" Standard SPI Read ok\r\n\r\n");
		printf("Standard SPI Read Data from Sector %d with length 0x%x: OK!\r\n",STARTINGBLOCK, ARRAYSIZE);
	else
		Error_Handler();

	printf("\n\rDisplay Read Data : \r\n");
	for(i =0;i<ARRAYSIZE;i++)
		printf("0x%02X  ",rData[i]);
	printf("\r\n\r\n");

	/*##-4- check date          ########################*/
	if(memcmp(wData,rData,ARRAYSIZE) == 0 )
		printf("W25Q128FV Standard SPI Test OK\r\n");
	else
		printf("W25Q128FV Standard SPI Test False\r\n");
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI2 Initialization Function
  * @param None
  * @retval None
  */
void MX_SPI2_Init(void)
{

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi2.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_GREEN_Pin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GREEN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
int __io_putchar(int ch)
{
	HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 0xffff);
	return ch;
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
