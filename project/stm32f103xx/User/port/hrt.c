#include "hrt.h"

static uint64_t hrtUs = 0;
static uint32_t hrtMs = 0;
static uint32_t hrtS = 0;

void hrtTimerInit(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    
    TIM_DeInit(TIM1);
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_Period = 1000-1;
    TIM_TimeBaseInitStruct.TIM_Prescaler = 72-1;
    TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0x00;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);
    
    NVIC_InitStruct.NVIC_IRQChannel = TIM1_UP_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStruct);
    
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    
    TIM_Cmd(TIM1, ENABLE);
}

uint64_t hrtAbsoluteTimeUs(void)
{
    return (hrtUs + TIM1->CNT);
}
uint32_t hrtAbsoluteTimeMs(void)
{
    return (hrtMs);
}

uint32_t hrtAbsoluteTimeS(void)
{
    return (hrtS);
}

void TIM1_UP_IRQHandler(void)
{
    static uint32_t count=0;
    if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        hrtUs+=1000;
        hrtMs++;
        if(++count == 1000)
        {
            hrtS++;
            count = 0;
        }
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }
    
}
