#include "handle_rc.h"

static uint16_t current_speed = 0;

void setSpeed(uint16_t speed){
	current_speed = speed;
}

void moveForward()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0);

	TIM3->CCR1 = current_speed;
	TIM3->CCR2 = current_speed;
}

void moveBackward()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);

	TIM3->CCR1 = current_speed;
	TIM3->CCR2 = current_speed;
}

void turnLeft()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 1);

	TIM3->CCR1 = current_speed;
	TIM3->CCR2 = current_speed;
}

void turnRight()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, 0);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, 1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, 0);

	TIM3->CCR1 = current_speed;
	TIM3->CCR2 = current_speed;
}

