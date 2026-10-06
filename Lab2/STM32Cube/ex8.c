/* USER CODE BEGIN Header */
/**
  * @file           : main.c
  * @brief          : Exercise 8 - interrupt only runs software timers,
  *                   all processing and LED scanning is done in main.
  *                   Self-contained: no seg7.c / matrix.c needed.
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM2_Init(void);

/* USER CODE BEGIN 0 */

/* ---------------------------------------------------------------------------
 * 7-segment driver (reimplemented here, all names are static and prefixed
 * so they cannot clash with seg7.c or other exercise files).
 * Common-anode displays: a segment lights when its pin is LOW (RESET),
 * a digit is enabled through its PNP transistor when its EN pin is LOW.
 * If your Proteus wiring is the other way round, swap these two lines.
 * ------------------------------------------------------------------------- */
#define SEG_ON    GPIO_PIN_RESET
#define SEG_OFF   GPIO_PIN_SET
#define EN_ON     GPIO_PIN_RESET
#define EN_OFF    GPIO_PIN_SET

#define EX8_MAX_LED 4

/* SEG0..SEG6 = segments a..g */
static const uint16_t seg_pins[7] = {
    SEG0_Pin, SEG1_Pin, SEG2_Pin, SEG3_Pin, SEG4_Pin, SEG5_Pin, SEG6_Pin
};

/* Bit i set = segment i (a..g) is lit. Digits 0 to 9. */
static const uint8_t digit_pattern[10] = {
    0x3F, /* 0: a b c d e f   */
    0x06, /* 1: b c           */
    0x5B, /* 2: a b d e g     */
    0x4F, /* 3: a b c d g     */
    0x66, /* 4: b c f g       */
    0x6D, /* 5: a c d f g     */
    0x7D, /* 6: a c d e f g   */
    0x07, /* 7: a b c         */
    0x7F, /* 8: all           */
    0x6F  /* 9: a b c d f g   */
};

static void ex8_display7SEG(int num) {
    uint8_t pattern = 0x00;                 /* invalid number: all segments off */
    if (num >= 0 && num <= 9) {
        pattern = digit_pattern[num];
    }
    for (int i = 0; i < 7; i++) {
        HAL_GPIO_WritePin(GPIOB, seg_pins[i],
                          ((pattern >> i) & 0x01) ? SEG_ON : SEG_OFF);
    }
}

static void ex8_turnOffAll7SEG(void) {
    HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, EN_OFF);
    HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, EN_OFF);
    HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, EN_OFF);
    HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, EN_OFF);
}

static int led_buffer[EX8_MAX_LED] = {1, 5, 0, 8};
static int index_led = 0;

static void ex8_update7SEG(int index) {
    ex8_turnOffAll7SEG();                   /* avoid ghosting */

    switch (index) {
        case 0:
            ex8_display7SEG(led_buffer[0]);
            HAL_GPIO_WritePin(EN0_GPIO_Port, EN0_Pin, EN_ON);
            break;
        case 1:
            ex8_display7SEG(led_buffer[1]);
            HAL_GPIO_WritePin(EN1_GPIO_Port, EN1_Pin, EN_ON);
            break;
        case 2:
            ex8_display7SEG(led_buffer[2]);
            HAL_GPIO_WritePin(EN2_GPIO_Port, EN2_Pin, EN_ON);
            break;
        case 3:
            ex8_display7SEG(led_buffer[3]);
            HAL_GPIO_WritePin(EN3_GPIO_Port, EN3_Pin, EN_ON);
            break;
        default:
            break;
    }
}

/* ---------------------------------------------------------------------------
 * Clock and software timers
 * ------------------------------------------------------------------------- */
#define TIMER_CYCLE 10          /* TIM2 interrupt period in ms */

static int hour = 15;
static int minute = 8;
static int second = 50;

static volatile int timer0_counter = 0;   /* clock, 1 s */
static volatile int timer0_flag = 0;

static volatile int timer1_counter = 0;   /* DOT blink, 1 s */
static volatile int timer1_flag = 0;

static volatile int timer2_counter = 0;   /* 7-segment scanning, 250 ms */
static volatile int timer2_flag = 0;

static void setTimer0(int duration) {
    timer0_counter = duration / TIMER_CYCLE;
    timer0_flag = 0;
}

static void setTimer1(int duration) {
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

static void setTimer2(int duration) {
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

static void timer_run(void) {
    if (timer0_counter > 0) {
        timer0_counter--;
        if (timer0_counter == 0) timer0_flag = 1;
    }
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter == 0) timer1_flag = 1;
    }
    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter == 0) timer2_flag = 1;
    }
}

static void updateClockBuffer(void) {
    led_buffer[0] = hour / 10;
    led_buffer[1] = hour % 10;
    led_buffer[2] = minute / 10;
    led_buffer[3] = minute % 10;
}
/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_TIM2_Init();

  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim2);

  ex8_turnOffAll7SEG();
  updateClockBuffer();     /* show 15:08 right away */

  setTimer0(1000);         /* timer 0: clock */
  setTimer1(1000);         /* timer 1: DOT LED */
  setTimer2(250);          /* timer 2: 7-segment scanning */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* 1. Update the clock every second */
    if (timer0_flag == 1) {
        setTimer0(1000);
        second++;
        if (second >= 60) {
            second = 0;
            minute++;
        }
        if (minute >= 60) {
            minute = 0;
            hour++;
        }
        if (hour >= 24) {
            hour = 0;
        }
        updateClockBuffer();
    }

    /* 2. Blink the DOT every second */
    if (timer1_flag == 1) {
        setTimer1(1000);
        HAL_GPIO_TogglePin(DOT_GPIO_Port, DOT_Pin);
    }

    /* 3. Scan the 7-segment LEDs entirely in the main loop */
    if (timer2_flag == 1) {
        setTimer2(250);
        ex8_update7SEG(index_led);
        index_led++;
        if (index_led >= EX8_MAX_LED) {
            index_led = 0;
        }
    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/* USER CODE BEGIN 4 */
/* The interrupt only runs the software timers */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM2)
  {
    timer_run();
  }
}
/* USER CODE END 4 */

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
  * @brief GPIO Initialization Function (same pin setup as the old main.c)
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