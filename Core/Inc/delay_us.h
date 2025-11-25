#ifndef INC_DELAY_US_H_
#define INC_DELAY_US_H_

// 실질적으로 필요한 4시리즈 hal 함수 헤더
#include "stm32f4xx_hal.h"
#include "tim.h"

void delay_us(uint16_t us);

#endif /* INC_DELAY_US_H_ */
