#ifndef __HRT_
#define __HRT_
#include "stm32f10x.h"

/*��Ҫʹ��Ӳ����ʱ��*/

void hrtTimerInit(void);

uint64_t hrtAbsoluteTimeUs(void);
uint32_t hrtAbsoluteTimeMs(void);
uint32_t hrtAbsoluteTimeS(void);

#endif /*__HRT_*/
