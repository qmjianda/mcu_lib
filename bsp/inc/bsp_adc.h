#ifndef __BSP_ADC_H_
#define __BSP_ADC_H_
#include "include.h"



#define ADC1_ENABLE_DMA (1)
#define ADC1_CH_MAX_NUM (2)

typedef struct
{
    uint16_t chVal[ADC1_CH_MAX_NUM];
}T_Adc1Val;

#define ADC1_CH1_GPIO_PORT  GPIOC
#define ADC1_CH1_GPIO_PIN   GPIO_Pin_0
#define ADC1_CH2_GPIO_PORT  GPIOC
#define ADC1_CH2_GPIO_PIN   GPIO_Pin_2

#if ADC1_ENABLE_DMA
#define ADC1_DMA_CLK                RCC_AHB1Periph_DMA2
#define ADC1_DMA_CH                 DMA_Channel_0
#define ADC1_DMA_STREAM             DMA2_Stream0
#define ADC1_DMA_PH_BASE_ADDR       ((uint32_t)ADC1+0x4c)
#endif

void adc1Init(void);
void adc1Start(void);
void adc1GetRawVal(T_Adc1Val *val);

#endif /*__BSP_ADC_H_*/
