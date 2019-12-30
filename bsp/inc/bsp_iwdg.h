#ifndef __BSP_IWDG_H_
#define __BSP_IWDG_H_

#include "include.h"

/*
pre=
IWDG_Prescaler_4  
IWDG_Prescaler_8  
IWDG_Prescaler_16 
IWDG_Prescaler_32 
IWDG_Prescaler_64 
IWDG_Prescaler_128
IWDG_Prescaler_256
*/
void idwgInit(u8 pre,u16 rlr);
void iwdgFeed(void);
uint8_t isIwdgReset(void);
void clearIwdgResetFlag(void);
#endif /*__BSP_IWDG_H_*/
