#ifndef __BSP_TIMER_PWM_H_
#define __BSP_TIMER_PWM_H_

#include "include.h"


/*********************PWM OUT********************************/
/*定时器1*/
#define TIM1_CHx_CLK            (RCC_AHB1Periph_GPIOE)
#define TIM1_CH1_PORT           (GPIOE)
#define TIM1_CH1_PIN            (GPIO_Pin_9)
#define TIM1_CH1_PIN_SOURCS     (GPIO_PinSource9)


/*定时器2*/
#define TIM2_CHx_CLK            (RCC_AHB1Periph_GPIOA)
#define TIM2_CH3_PORT           (GPIOA)
#define TIM2_CH3_PIN            (GPIO_Pin_2)
#define TIM2_CH3_PIN_SOURCS     (GPIO_PinSource2)


/*定时器8*/
#define TIM8_CHx_CLK            (RCC_AHB1Periph_GPIOC)
#define TIM8_CH1_PORT           (GPIOC)
#define TIM8_CH1_PIN            (GPIO_Pin_6)
#define TIM8_CH1_PIN_SOURCS     (GPIO_PinSource6)
#define TIM8_CH2_PORT           (GPIOC)
#define TIM8_CH2_PIN            (GPIO_Pin_7)
#define TIM8_CH2_PIN_SOURCS     (GPIO_PinSource7)

/*定时器12*/
#define TIM12_CHx_CLK            (RCC_AHB1Periph_GPIOB)
#define TIM12_CH1_PORT           (GPIOB)
#define TIM12_CH1_PIN            (GPIO_Pin_14)
#define TIM12_CH1_PIN_SOURCS     (GPIO_PinSource14)
/*********************PWM OUT END********************************/

#define IS_PWM_OUT_CH(ch)       (ch<4)
#define IS_PWM_IN_CH(ch)        (ch<4)

void TIMx_PWM_Init(TIM_TypeDef *TIMx, uint32_t freq);
void TIMx_PWM_SetDutyCycle(TIM_TypeDef *TIMx, uint8_t ch,uint32_t dutyCycle);
void TIMx_PWM_SetPluse(TIM_TypeDef *TIMx, uint8_t ch,uint32_t pluse);

#endif /*__BSP_TIMER_PWM_H_*/
