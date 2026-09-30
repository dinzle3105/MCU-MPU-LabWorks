/* USER CODE BEGIN Header */
/**
  * @file           : main.c
  * @brief          : Main program body with Direct Exercise Multiplexer
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */


#define ACTIVE_EXERCISE 4

extern void ex1_init(void);  extern void ex1_loop(void);  extern void ex1_timer_isr(void);
extern void ex2_init(void);  extern void ex2_loop(void);  extern void ex2_timer_isr(void);
extern void ex3_init(void);  extern void ex3_loop(void);  extern void ex3_timer_isr(void);
extern void ex4_init(void);  extern void ex4_loop(void);  extern void ex4_timer_isr(void);
extern void ex5_init(void);  extern void ex5_loop(void);  extern void ex5_timer_isr(void);
extern void ex8_init(void);  extern void ex8_loop(void);  extern void ex8_timer_isr(void);
extern void ex7_init(void);  extern void ex7_loop(void);  extern void ex3_timer_isr(void);
extern void ex9_init(void);  extern void ex9_loop(void);  extern void ex9_timer_isr(void);
extern void ex10_init(void); extern void ex10_loop(void); extern void ex10_timer_isr(void);

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

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
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  /* MULTIPLEXER: Initialize the active exercise */
#if (ACTIVE_EXERCISE == 1)
  ex1_init();
#elif (ACTIVE_EXERCISE == 2)
  ex2_init();
#elif (ACTIVE_EXERCISE == 3)
  ex3_init();
#elif (ACTIVE_EXERCISE == 4)
  ex4_init();
#elif (ACTIVE_EXERCISE == 5)
  ex5_init();
#elif (ACTIVE_EXERCISE == 7)
  ex7_init();
#elif (ACTIVE_EXERCISE == 8)
  ex8_init();
#elif (ACTIVE_EXERCISE == 9)
  ex9_init();
#elif (ACTIVE_EXERCISE == 10)
  ex10_init();
#endif
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* MULTIPLEXER: Run the active exercise loop */
#if (ACTIVE_EXERCISE == 1)
    ex1_loop();
#elif (ACTIVE_EXERCISE == 2)
    ex2_loop();
#elif (ACTIVE_EXERCISE == 3)
    ex3_loop();
#elif (ACTIVE_EXERCISE == 4)
  ex4_loop();
#elif (ACTIVE_EXERCISE == 5)
  ex5_loop();
#elif (ACTIVE_EXERCISE == 7)
  ex7_loop();
#elif (ACTIVE_EXERCISE == 8)
  ex8_loop();
#elif (ACTIVE_EXERCISE == 9)
    ex9_loop();
#elif (ACTIVE_EXERCISE == 10)
    ex10_loop();
#endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief  Period elapsed callback in non-blocking mode
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */
  if (htim->Instance == TIM2) {

    /* MULTIPLEXER: Run the active exercise timer interrupt */
#if (ACTIVE_EXERCISE == 1)
    ex1_timer_isr();
#elif (ACTIVE_EXERCISE == 2)
    ex2_timer_isr();
#elif (ACTIVE_EXERCISE == 3)
    ex3_timer_isr();
#elif (ACTIVE_EXERCISE == 4)
  ex4_timer_isr();
#elif (ACTIVE_EXERCISE == 5)
  ex5_timer_isr();
#elif (ACTIVE_EXERCISE == 7)
  ex7_timer_isr();
#elif (ACTIVE_EXERCISE == 8)
  ex8_timer_isr();
#elif (ACTIVE_EXERCISE == 9)
    ex9_timer_isr();
#elif (ACTIVE_EXERCISE == 10)
    ex10_timer_isr();
#endif

  }
  /* USER CODE END Callback 0 */
}

/**
  * @brief System Clock Configuration
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
  * @brief TIM2 Initialization Function
  */
static void MX_TIM2_Init(void)
{
  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 7999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 9;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOA, ENM0_Pin|ENM1_Pin|DOT_Pin|Led_Red_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = ENM0_Pin|ENM1_Pin|DOT_Pin|Led_Red_Pin
                          |EN0_Pin|EN1_Pin|EN2_Pin|EN3_Pin
                          |ENM2_Pin|ENM3_Pin|ENM4_Pin|ENM5_Pin
                          |ENM6_Pin|ENM7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = SEG0_Pin|SEG1_Pin|SEG2_Pin|ROW2_Pin
                          |ROW3_Pin|ROW4_Pin|ROW5_Pin|ROW6_Pin
                          |ROW7_Pin|SEG3_Pin|SEG4_Pin|SEG5_Pin
                          |SEG6_Pin|ROW0_Pin|ROW1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif /* USE_FULL_ASSERT */
