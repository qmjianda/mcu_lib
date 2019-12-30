#ifndef __BSP_TIMER_ENC_H_
#define __BSP_TIMER_ENC_H_

#include "include.h"

/*定时器3 编码器模式输入引脚*/
#define TIM3_CHx_CLK            (RCC_AHB1Periph_GPIOB)
#define TIM3_CH1_PORT           (GPIOB)
#define TIM3_CH1_PIN            (GPIO_Pin_4)
#define TIM3_CH1_PINSOURCE      (GPIO_PinSource4)
#define TIM3_CH2_PORT           (GPIOB)
#define TIM3_CH2_PIN            (GPIO_Pin_5)
#define TIM3_CH2_PINSOURCE      (GPIO_PinSource5)

#define TIM4_CHx_CLK            (RCC_AHB1Periph_GPIOD)
#define TIM4_CH1_PORT           (GPIOD)
#define TIM4_CH1_PIN            (GPIO_Pin_12)
#define TIM4_CH1_PINSOURCE      (GPIO_PinSource12)
#define TIM4_CH2_PORT           (GPIOD)
#define TIM4_CH2_PIN            (GPIO_Pin_13)
#define TIM4_CH2_PINSOURCE      (GPIO_PinSource13)

#define TIM5_CHx_CLK            (RCC_AHB1Periph_GPIOB)
#define TIM5_CH1_PORT           (GPIOA)
#define TIM5_CH1_PIN            (GPIO_Pin_0)
#define TIM5_CH1_PINSOURCE      (GPIO_PinSource0)
#define TIM5_CH2_PORT           (GPIOA)
#define TIM5_CH2_PIN            (GPIO_Pin_1)
#define TIM5_CH2_PINSOURCE      (GPIO_PinSource1)

void TIMx_EncoderInit(TIM_TypeDef *TIMx);
uint16_t TIMx_getEncVal(TIM_TypeDef *TIMx);
void TIMx_setEncVal(TIM_TypeDef *TIMx, uint16_t val);
#endif /*__BSP_TIMER_ENC_H_*/
