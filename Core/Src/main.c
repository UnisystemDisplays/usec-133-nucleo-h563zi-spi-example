/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "icache.h"
#include "memorymap.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "usec_dev.h"
#include "img_1_1bpp.h"
#include "img_2_1bpp.h"
#include "img_8bpp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

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

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  usec_ctx *ctx;
  uint8_t usec_temp;
  uint8_t status;
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  MX_ICACHE_Init();
  /* USER CODE BEGIN 2 */

  printf ("[status] app init...\n\r");

  /* initialize controller */
  ctx = usec_init();
  if (ctx == NULL)
    {
	  printf ("[error] cannot initialize e-ink controller\n\r");
      while(1);
    }

  printf ("[status] screen width - %ld [px]\n\r", ctx->dev_width[0]);
  printf ("[status] screen height - %ld [px]\n\r", ctx->dev_height[0]);

  /* read temperature value */
  status = usec_get_temp (ctx, &usec_temp);
  if (status != USEC_DEV_OK)
    {
	  printf ("[error] cannot read temperature value\n\r");

      usec_deinit (ctx);
      while(1);
    }
  printf ("[status] screen temperature: %d [degC]\n\r", usec_temp);

  /*
   * DEMO MAINLOOP
   */
  while(1)
    {
	  /* cleanup display - UPDATE_MODE_INIT */
	  status = usec_img_update (ctx, 0, 0, 1600, 1200, UPDATE_MODE_INIT, 1);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot cleanup display\n\r");
		  while(1);
		}

	  /* upload 8BPP image into internal buffer */
	  status = usec_img_upload (ctx, (uint8_t*)img_8bpp, sizeof (img_8bpp),
								IMG_8BPP, 400, 300, 800, 600);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot upload 'img_8bpp' data\n\r");
		  while(1);
		}

	  /* update display - UPDATE_MODE_GC16 */
	  status = usec_img_update (ctx, 400, 300, 800, 600, UPDATE_MODE_GC16, 1);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot update display\n\r");
		  while(1);
		}

	  /* delay only for demo purposes */
	  HAL_Delay (2000);

	  /* switch to 1BPP mode */
	  status = usec_1bpp_mode (ctx, ENABLE_1BPP);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot switch display to 1BPP mode\n\r");
		  while(1);
		}

	  /* upload 1BPP image to internal buffer */
	  status = usec_img_upload (ctx, (uint8_t*)img_1_1bpp, sizeof(img_1_1bpp),
								IMG_1BPP, 0, 0, 1600, 1200);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot upload 'img_1_1bpp' data\n\r");
		  while(1);
		}

	  /* update display */
	  status = usec_img_update (ctx, 0, 0, 1600, 1200, UPDATE_MODE_GC16, 1);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot update display\n\r");
		  while(1);
		}

	  /* delay only for demo purposes */
	  HAL_Delay (2000);

	  /* upload 1BPP image to internal buffer */
	  status = usec_img_upload (ctx, (uint8_t*)img_2_1bpp,
								sizeof(img_2_1bpp), IMG_1BPP, 0, 0, 1600, 1200);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot upload 'img_2_1bpp' data\n\r");
		  while(1);
		}

	  /* update display */
	  status = usec_img_update (ctx, 0, 0, 1600, 1200, UPDATE_MODE_GC16, 1);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot update display\n\r");
		  while(1);
		}

	  /* disable 1BPP mode if we want to display 8BPP image */
	  status = usec_1bpp_mode (ctx, DISABLE_1BPP);
	  if (status != USEC_DEV_OK)
		{
		  printf ("[error] cannot disable 1BPP mode\n\r");
		  while(1);
		}
    }
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

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLL1_SOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 250;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1_VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1_VCORANGE_WIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_2);
}

/* USER CODE BEGIN 4 */
PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit (&huart3, (uint8_t*)&ch, 1, 0xFFFF);

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
