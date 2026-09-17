/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
// Map clock indices 0-11 to GPIO pins (Index 0 = 12 o'clock, 1 = 1 o'clock, etc.)
uint16_t clockPins[12] = {
    CLK_12_Pin, CLK_1_Pin, CLK_2_Pin, CLK_3_Pin,
    CLK_4_Pin,  CLK_5_Pin, CLK_6_Pin, CLK_7_Pin,
    CLK_8_Pin,  CLK_9_Pin, CLK_10_Pin, CLK_11_Pin
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void clearAllClock(void);
void setNumberOnClock(int num);
void clearNumberOnClock(int num);
void displayClock(int hour, int minute, int second);
void ex6(void);
/* USER CODE END PFP */

/* Private user code --------------------------------------------------------- */
/* USER CODE BEGIN 0 */
void clearAllClock(void) {
    for (int i = 0; i < 12; i++) {
        HAL_GPIO_WritePin(GPIOA, clockPins[i], GPIO_PIN_SET);
    }
}

void setNumberOnClock(int num) {
    if (num >= 0 && num < 12) {
        HAL_GPIO_WritePin(GPIOA, clockPins[num], GPIO_PIN_RESET);
    }
}

void clearNumberOnClock(int num) {
    if (num >= 0 && num < 12) {
        HAL_GPIO_WritePin(GPIOA, clockPins[num], GPIO_PIN_SET);
    }
}

void displayClock(int hour, int minute, int second) {
    clearAllClock();

    int second_led = (second / 5) % 12;
    int minute_led = (minute / 5) % 12;
    int hour_led = hour % 12;

    setNumberOnClock(hour_led);
    setNumberOnClock(minute_led);
    setNumberOnClock(second_led);
}

void ex6(void) {
    for (int i = 0; i < 12; i++) {
        HAL_GPIO_WritePin(GPIOA, clockPins[i], GPIO_PIN_RESET);
        HAL_Delay(500);
        HAL_GPIO_WritePin(GPIOA, clockPins[i], GPIO_PIN_SET);
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* MCU Configuration--------------------------------------------------------*/
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();

  /* USER CODE BEGIN 2 */
  clearAllClock();
  int h = 10;
  int m = 30;
  int s = 45;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    displayClock(h, m, s);
    HAL_Delay(1000);

    s++;
    if (s >= 60) {
        s = 0;
        m++;
        if (m >= 60) {
            m = 0;
            h = (h + 1) % 12;
        }
    }
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

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CLK_12_Pin|CLK_1_Pin|CLK_2_Pin|CLK_3_Pin
                          |CLK_4_Pin|CLK_5_Pin|CLK_6_Pin|CLK_7_Pin
                          |CLK_8_Pin|CLK_9_Pin|CLK_10_Pin|CLK_11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins */
  GPIO_InitStruct.Pin = CLK_12_Pin|CLK_1_Pin|CLK_2_Pin|CLK_3_Pin
                          |CLK_4_Pin|CLK_5_Pin|CLK_6_Pin|CLK_7_Pin
                          |CLK_8_Pin|CLK_9_Pin|CLK_10_Pin|CLK_11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}
