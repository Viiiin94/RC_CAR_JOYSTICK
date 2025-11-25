/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f4xx_hal.h"

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
#define IN1_Pin GPIO_PIN_0
#define IN1_GPIO_Port GPIOC
#define IN2_Pin GPIO_PIN_1
#define IN2_GPIO_Port GPIOC
#define IN3_Pin GPIO_PIN_2
#define IN3_GPIO_Port GPIOC
#define IN4_Pin GPIO_PIN_3
#define IN4_GPIO_Port GPIOC
#define ultra_Output0_Pin GPIO_PIN_5
#define ultra_Output0_GPIO_Port GPIOA
#define TIM3_CH1_ENA_Pin GPIO_PIN_6
#define TIM3_CH1_ENA_GPIO_Port GPIOA
#define TIM3_CH2_ENB_Pin GPIO_PIN_7
#define TIM3_CH2_ENB_GPIO_Port GPIOA
#define ultra_Output1_Pin GPIO_PIN_2
#define ultra_Output1_GPIO_Port GPIOB
#define ultra_Output2_Pin GPIO_PIN_12
#define ultra_Output2_GPIO_Port GPIOB
#define TIM1_CH1_RIGHT_Pin GPIO_PIN_8
#define TIM1_CH1_RIGHT_GPIO_Port GPIOA
#define TIM1_CH2_FORWARD_Pin GPIO_PIN_9
#define TIM1_CH2_FORWARD_GPIO_Port GPIOA
#define TIM1_CH3_LEFT_Pin GPIO_PIN_10
#define TIM1_CH3_LEFT_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
