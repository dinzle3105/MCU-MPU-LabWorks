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
#define RED1_Pin GPIO_PIN_0
#define RED1_GPIO_Port GPIOA
#define YELLOW1_Pin GPIO_PIN_1
#define YELLOW1_GPIO_Port GPIOA
#define GREEN1_Pin GPIO_PIN_2
#define GREEN1_GPIO_Port GPIOA
#define RED2_Pin GPIO_PIN_3
#define RED2_GPIO_Port GPIOA
#define YELLOW2_Pin GPIO_PIN_4
#define YELLOW2_GPIO_Port GPIOA
#define GREEN2_Pin GPIO_PIN_5
#define GREEN2_GPIO_Port GPIOA
#define SEG_A1_Pin GPIO_PIN_0
#define SEG_A1_GPIO_Port GPIOB
#define SEG_B1_Pin GPIO_PIN_1
#define SEG_B1_GPIO_Port GPIOB
#define SEG_C1_Pin GPIO_PIN_2
#define SEG_C1_GPIO_Port GPIOB
#define SEG_D2_Pin GPIO_PIN_10
#define SEG_D2_GPIO_Port GPIOB
#define SEG_E2_Pin GPIO_PIN_11
#define SEG_E2_GPIO_Port GPIOB
#define SEG_F2_Pin GPIO_PIN_12
#define SEG_F2_GPIO_Port GPIOB
#define SEG_G2_Pin GPIO_PIN_13
#define SEG_G2_GPIO_Port GPIOB
#define SEG_D1_Pin GPIO_PIN_3
#define SEG_D1_GPIO_Port GPIOB
#define SEG_E1_Pin GPIO_PIN_4
#define SEG_E1_GPIO_Port GPIOB
#define SEG_F1_Pin GPIO_PIN_5
#define SEG_F1_GPIO_Port GPIOB
#define SEG_G1_Pin GPIO_PIN_6
#define SEG_G1_GPIO_Port GPIOB
#define SEG_A2_Pin GPIO_PIN_7
#define SEG_A2_GPIO_Port GPIOB
#define SEG_B2_Pin GPIO_PIN_8
#define SEG_B2_GPIO_Port GPIOB
#define SEG_C2_Pin GPIO_PIN_9
#define SEG_C2_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
