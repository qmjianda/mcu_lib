#ifndef __BSP_PERIPHERAL_CLK_H_
#define __BSP_PERIPHERAL_CLK_H_
#include "include.h"

uint32_t getGpioClk(GPIO_TypeDef* gpiox);
uint32_t getUsartClk(USART_TypeDef* usartx);
#endif /*__BSP_PERIPHERAL_CLK_H_*/
