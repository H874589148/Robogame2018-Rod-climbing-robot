#ifndef _STEPPERMOTOR_H
#define _STEPPERMOTOR_H

#include "stm32f10x.h"

void stepper_Init(u16 divNum, double round);
void stepper_direction(u8 direction);
void stepper_enable(u8 status);


#endif