#include "delay_us.h"

// 블로킹 코드
void delay_us(uint16_t us)
{
  // 내가 사용할 TIMER의 레지스터를 리셋
  __HAL_TIM_SET_COUNTER(&htim10, 0);
  while((__HAL_TIM_GET_COUNTER(&htim10)) < us);
}
