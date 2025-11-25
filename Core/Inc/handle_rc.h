#ifndef INC_HANDLE_RC_H_
#define INC_HANDLE_RC_H_

#include "stm32f4xx_hal.h"

void setSpeed(uint16_t speed);
void moveForward();
void moveBackward();
void turnLeft();
void turnRight();

#endif /* INC_HANDLE_RC_H_ */
