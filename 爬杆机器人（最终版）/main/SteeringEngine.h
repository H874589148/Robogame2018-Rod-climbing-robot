#ifndef _STEERINGENGINE_H
#define _STEERINGENGINE_H

#include "stm32f10x.h"

void TIM_PWM_Init();
void setAngle(u16 ID, u16 angle);
void nvic();
void setWheel(u16 num, u16 dire,u16 v);
#endif