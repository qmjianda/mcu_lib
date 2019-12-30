#ifndef __BSP_TIMER_H_
#define __BSP_TIMER_H_
#include "include.h"

void TIM7_PWM_OutInit(uint32_t freq, void(*isrFun)(void));

#endif /*__BSP_TIMER_H_*/
