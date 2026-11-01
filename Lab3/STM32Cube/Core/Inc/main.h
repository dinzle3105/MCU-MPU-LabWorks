/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define A_RED_Pin GPIO_PIN_0
#define A_RED_GPIO_Port GPIOA
#define A_AMBER_Pin GPIO_PIN_1
#define A_AMBER_GPIO_Port GPIOA
#define A_GREEN_Pin GPIO_PIN_2
#define A_GREEN_GPIO_Port GPIOA
#define B_RED_Pin GPIO_PIN_3
#define B_RED_GPIO_Port GPIOA
#define B_AMBER_Pin GPIO_PIN_4
#define B_AMBER_GPIO_Port GPIOA
#define B_GREEN_Pin GPIO_PIN_5
#define B_GREEN_GPIO_Port GPIOA
#define BUTTON_1_Pin GPIO_PIN_6
#define BUTTON_1_GPIO_Port GPIOA
#define BUTTON_2_Pin GPIO_PIN_7
#define BUTTON_2_GPIO_Port GPIOA
#define Aa_Pin GPIO_PIN_0
#define Aa_GPIO_Port GPIOB
#define Ab_Pin GPIO_PIN_1
#define Ab_GPIO_Port GPIOB
#define Ac_Pin GPIO_PIN_2
#define Ac_GPIO_Port GPIOB
#define Bd_Pin GPIO_PIN_10
#define Bd_GPIO_Port GPIOB
#define Be_Pin GPIO_PIN_11
#define Be_GPIO_Port GPIOB
#define Bf_Pin GPIO_PIN_12
#define Bf_GPIO_Port GPIOB
#define Bg_Pin GPIO_PIN_13
#define Bg_GPIO_Port GPIOB
#define BUTTON_3_Pin GPIO_PIN_8
#define BUTTON_3_GPIO_Port GPIOA
#define SEG_A1_Pin GPIO_PIN_9
#define SEG_A1_GPIO_Port GPIOA
#define SEG_A2_Pin GPIO_PIN_10
#define SEG_A2_GPIO_Port GPIOA
#define SEG_B1_Pin GPIO_PIN_11
#define SEG_B1_GPIO_Port GPIOA
#define SEG_B2_Pin GPIO_PIN_12
#define SEG_B2_GPIO_Port GPIOA
#define Ad_Pin GPIO_PIN_3
#define Ad_GPIO_Port GPIOB
#define Ae_Pin GPIO_PIN_4
#define Ae_GPIO_Port GPIOB
#define Af_Pin GPIO_PIN_5
#define Af_GPIO_Port GPIOB
#define Ag_Pin GPIO_PIN_6
#define Ag_GPIO_Port GPIOB
#define Ba_Pin GPIO_PIN_7
#define Ba_GPIO_Port GPIOB
#define Bb_Pin GPIO_PIN_8
#define Bb_GPIO_Port GPIOB
#define Bc_Pin GPIO_PIN_9
#define Bc_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define TIMER_PERIOD_MS  10
#define LED_BLINK_HZ     2
#define BLINK_HALF_PERIOD_MS  250U
#define LED_TOGGLE_MS (1000 / (2 * LED_BLINK_HZ))
#define BUTTON_SCAN_MS        10U
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
