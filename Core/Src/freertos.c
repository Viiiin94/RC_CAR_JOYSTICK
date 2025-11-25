/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : freertos.c
 * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "stdio.h"
#include "handle_rc.h"
#include "delay_us.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

enum {
	RIGHT_ULTRASONIC = 0,
	FORWARD_ULTRASONIC,
	LEFT_ULTRASONIC
};

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

int _write(int file, unsigned char* p, int len)
{
	HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, p, len, 100);
	return (status == HAL_OK ? len : 0);
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

uint16_t IC_Value1[3] = {0};
uint16_t IC_Value2[3] = {0};
uint16_t echoTime[3] = {0};
uint8_t captureFlag[3] = {0};
uint8_t distance[3] = {0}; // 실제 거리

/* USER CODE END Variables */
/* Definitions for ultra */
osThreadId_t ultraHandle;
const osThreadAttr_t ultra_attributes = {
  .name = "ultra",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for carmode */
osThreadId_t carmodeHandle;
const osThreadAttr_t carmode_attributes = {
  .name = "carmode",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for auto_mobility */
osThreadId_t auto_mobilityHandle;
const osThreadAttr_t auto_mobility_attributes = {
  .name = "auto_mobility",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for uart */
osThreadId_t uartHandle;
const osThreadAttr_t uart_attributes = {
  .name = "uart",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
}

void triggerRightUltrasonic(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, SET);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, RESET);
	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC1);
}

void triggerForwardUltrasonic(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, SET);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, RESET);

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC2);
}

void triggerLeftUltrasonic(){
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, SET);
	delay_us(10);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, RESET);

	__HAL_TIM_ENABLE_IT(&htim1, TIM_IT_CC3);
}

void processUltrasonicCapture(uint8_t idx, uint32_t channel){
	uint32_t tim_it_channel;

	switch(channel)
	{
	case TIM_CHANNEL_1:
		tim_it_channel = TIM_IT_CC1;
		break;
	case TIM_CHANNEL_2:
		tim_it_channel = TIM_IT_CC2;
		break;
	case TIM_CHANNEL_3:
		tim_it_channel = TIM_IT_CC3;
		break;
	default: return; // 잘못된 채널 보호
	}

	if(captureFlag[idx] == 0)
	{
		IC_Value1[idx] = HAL_TIM_ReadCapturedValue(&htim1, channel);
		captureFlag[idx] = 1;
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, channel, TIM_INPUTCHANNELPOLARITY_FALLING);
	}
	else
	{
		IC_Value2[idx] = HAL_TIM_ReadCapturedValue(&htim1, channel);
		__HAL_TIM_SET_CAPTUREPOLARITY(&htim1, channel, TIM_INPUTCHANNELPOLARITY_RISING);

		if(IC_Value2[idx] >= IC_Value1[idx])
			echoTime[idx] = IC_Value2[idx] - IC_Value1[idx];
		else
			echoTime[idx] = (0xFFFF - IC_Value1[idx]) + IC_Value2[idx];

		distance[idx] = echoTime[idx] / 58;

		captureFlag[idx] = 0;
		__HAL_TIM_DISABLE_IT(&htim1, tim_it_channel);
	}
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1){
		processUltrasonicCapture(RIGHT_ULTRASONIC, TIM_CHANNEL_1);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2){
		processUltrasonicCapture(FORWARD_ULTRASONIC, TIM_CHANNEL_2);
	}

	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3){
		processUltrasonicCapture(LEFT_ULTRASONIC, TIM_CHANNEL_3);
	}
}

/* USER CODE END FunctionPrototypes */

void ultrasonic(void *argument);
void changemode(void *argument);
void automoblilty(void *argument);
void usart_slave(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

	HAL_TIM_Base_Start(&htim10);

	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_3);

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of ultra */
  ultraHandle = osThreadNew(ultrasonic, NULL, &ultra_attributes);

  /* creation of carmode */
  carmodeHandle = osThreadNew(changemode, NULL, &carmode_attributes);

  /* creation of auto_mobility */
  auto_mobilityHandle = osThreadNew(automoblilty, NULL, &auto_mobility_attributes);

  /* creation of uart */
  uartHandle = osThreadNew(usart_slave, NULL, &uart_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_ultrasonic */
/**
  * @brief  Function implementing the ultra thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_ultrasonic */
void ultrasonic(void *argument)
{
  /* USER CODE BEGIN ultrasonic */
	uint32_t currentTime = HAL_GetTick();
	uint32_t triggerTime = 0;


  /* Infinite loop */
  for(;;)
  {
	  if(currentTime - triggerTime >= 60){
		  triggerRightUltrasonic();
		  triggerForwardUltrasonic();
		  triggerLeftUltrasonic();
		  triggerTime = currentTime;
	  }

	  printf("Right : %d cm \n", distance[0]);
	  printf("Forward : %d cm \n", distance[1]);
	  printf("Left : %d cm \n", distance[2]);
    osDelay(1);
  }
  /* USER CODE END ultrasonic */
}

/* USER CODE BEGIN Header_changemode */
/**
* @brief Function implementing the carmode thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_changemode */
void changemode(void *argument)
{
  /* USER CODE BEGIN changemode */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END changemode */
}

/* USER CODE BEGIN Header_automoblilty */
/**
* @brief Function implementing the auto_mobility thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_automoblilty */
void automoblilty(void *argument)
{
  /* USER CODE BEGIN automoblilty */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END automoblilty */
}

/* USER CODE BEGIN Header_usart_slave */
/**
 * @brief Function implementing the uart thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_usart_slave */
void usart_slave(void *argument)
{
  /* USER CODE BEGIN usart_slave */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
  /* USER CODE END usart_slave */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

