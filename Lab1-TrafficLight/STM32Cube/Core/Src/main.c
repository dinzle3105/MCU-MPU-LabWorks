/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2026 STMicroelectronics.
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* Helper function to set Traffic Light Set 1 (e.g., North-South) */
void setLight1(GPIO_PinState r, GPIO_PinState y, GPIO_PinState g) {
    HAL_GPIO_WritePin(GPIOA, RED1_Pin, (GPIO_PinState)!r);
    HAL_GPIO_WritePin(GPIOA, YELLOW1_Pin, (GPIO_PinState)!y);
    HAL_GPIO_WritePin(GPIOA, GREEN1_Pin, (GPIO_PinState)!g);
}

/* Helper function to set Traffic Light Set 2 (e.g., East-West) */
void setLight2(GPIO_PinState r, GPIO_PinState y, GPIO_PinState g) {
    HAL_GPIO_WritePin(GPIOA, RED2_Pin, (GPIO_PinState)!r);
    HAL_GPIO_WritePin(GPIOA, YELLOW2_Pin, (GPIO_PinState)!y);
    HAL_GPIO_WritePin(GPIOA, GREEN2_Pin, (GPIO_PinState)!g);
}

/* Segment drive map array (Common Anode: 1 = ON / active-LOW output) */
const uint8_t segOn[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
};

/* Display digit on 7-Segment #1 (Direction 1) */
void display7SEG_1(int num) {
    const uint16_t segPins1[7] = {
        SEG_A1_Pin, SEG_B1_Pin, SEG_C1_Pin, SEG_D1_Pin, SEG_E1_Pin, SEG_F1_Pin, SEG_G1_Pin
    };

    if (num < 0 || num > 9) return;

    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, segPins1[i], segOn[num][i] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

/* Display digit on 7-Segment #2 (Direction 2) */
void display7SEG_2(int num) {
    const uint16_t segPins2[7] = {
        SEG_A2_Pin, SEG_B2_Pin, SEG_C2_Pin, SEG_D2_Pin, SEG_E2_Pin, SEG_F2_Pin, SEG_G2_Pin
    };

    if (num < 0 || num > 9) return;

    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, segPins2[i], segOn[num][i] ? GPIO_PIN_RESET : GPIO_PIN_SET);
    }
}

void ex3(void) {
    /* Phase 1: Direction 1 GREEN, Direction 2 RED (3s) */
    setLight1(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET);
    setLight2(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
    HAL_Delay(3000);

    /* Phase 2: Direction 1 YELLOW, Direction 2 RED (2s) */
    setLight1(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
    HAL_Delay(2000);

    /* Phase 3: Direction 1 RED, Direction 2 GREEN (3s) */
    setLight1(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
    setLight2(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET);
    HAL_Delay(3000);

    /* Phase 4: Direction 1 RED, Direction 2 YELLOW (2s) */
    setLight2(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
    HAL_Delay(2000);
}

void ex4(void) {
    int counter = 0;
    while (1) {
        if (counter >= 10) counter = 0;
        display7SEG_1(counter++);
        HAL_Delay(1000);
    }
}

void ex5(void) {
    /* Timing durations (in seconds):
       GREEN  = 3s
       YELLOW = 2s
       RED    = 5s (GREEN + YELLOW)
    */
    int count1 = 3; // Direction 1 starts GREEN (3s)
    int count2 = 5; // Direction 2 starts RED (5s)

    /* Phase 1: Direction 1 GREEN (3s), Direction 2 RED (3s remaining) */
    setLight1(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET);
    setLight2(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
    while (count1 > 0) {
        display7SEG_1(count1--);
        display7SEG_2(count2--);
        HAL_Delay(1000);
    }

    /* Phase 2: Direction 1 YELLOW (2s), Direction 2 RED (2s remaining) */
    count1 = 2;
    setLight1(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
    while (count1 > 0) {
        display7SEG_1(count1--);
        display7SEG_2(count2--);
        HAL_Delay(1000);
    }

    /* Phase 3: Direction 1 RED (5s), Direction 2 GREEN (3s) */
    count1 = 5;
    count2 = 3;
    setLight1(GPIO_PIN_SET, GPIO_PIN_RESET, GPIO_PIN_RESET);
    setLight2(GPIO_PIN_RESET, GPIO_PIN_RESET, GPIO_PIN_SET);
    while (count2 > 0) {
        display7SEG_1(count1--);
        display7SEG_2(count2--);
        HAL_Delay(1000);
    }

    /* Phase 4: Direction 1 RED (2s remaining), Direction 2 YELLOW (2s) */
    count2 = 2;
    setLight2(GPIO_PIN_RESET, GPIO_PIN_SET, GPIO_PIN_RESET);
    while (count2 > 0) {
        display7SEG_1(count1--);
        display7SEG_2(count2--);
        HAL_Delay(1000);
    }
}
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

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      ex5();
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, RED1_Pin|YELLOW1_Pin|GREEN1_Pin|RED2_Pin
                          |YELLOW2_Pin|GREEN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SEG_A1_Pin|SEG_B1_Pin|SEG_C1_Pin|SEG_D2_Pin
                          |SEG_E2_Pin|SEG_F2_Pin|SEG_G2_Pin|SEG_D1_Pin
                          |SEG_E1_Pin|SEG_F1_Pin|SEG_G1_Pin|SEG_A2_Pin
                          |SEG_B2_Pin|SEG_C2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : RED1_Pin YELLOW1_Pin GREEN1_Pin RED2_Pin
                           YELLOW2_Pin GREEN2_Pin */
  GPIO_InitStruct.Pin = RED1_Pin|YELLOW1_Pin|GREEN1_Pin|RED2_Pin
                          |YELLOW2_Pin|GREEN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SEG_A1_Pin SEG_B1_Pin SEG_C1_Pin SEG_D2_Pin
                           SEG_E2_Pin SEG_F2_Pin SEG_G2_Pin SEG_D1_Pin
                           SEG_E1_Pin SEG_F1_Pin SEG_G1_Pin SEG_A2_Pin
                           SEG_B2_Pin SEG_C2_Pin */
  GPIO_InitStruct.Pin = SEG_A1_Pin|SEG_B1_Pin|SEG_C1_Pin|SEG_D2_Pin
                          |SEG_E2_Pin|SEG_F2_Pin|SEG_G2_Pin|SEG_D1_Pin
                          |SEG_E1_Pin|SEG_F1_Pin|SEG_G1_Pin|SEG_A2_Pin
                          |SEG_B2_Pin|SEG_C2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

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
