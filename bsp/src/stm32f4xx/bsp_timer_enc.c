#include "bsp_timer_enc.h"
#include "bsp_uart.h"

uint32_t encOverFlowVal[3] = {0};



void TIMx_EncGpioInit(TIM_TypeDef *TIMx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    if(TIMx == TIM3)
	{
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE); //TIM3时钟使能    
        RCC_AHB1PeriphClockCmd(TIM3_CHx_CLK, ENABLE); 	    //使能PORTE时钟	
        
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉(根据实际编码器修改)
        GPIO_PinAFConfig(TIM3_CH1_PORT,TIM3_CH1_PINSOURCE,GPIO_AF_TIM3);
        GPIO_PinAFConfig(TIM3_CH2_PORT,TIM3_CH2_PINSOURCE,GPIO_AF_TIM3);
        
        GPIO_InitStructure.GPIO_Pin = TIM3_CH1_PIN;
        GPIO_Init(TIM3_CH1_PORT,&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = TIM3_CH2_PIN;
        GPIO_Init(TIM3_CH2_PORT,&GPIO_InitStructure);
    }
    else if(TIMx == TIM4)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE); //TIM4时钟使能    
        RCC_AHB1PeriphClockCmd(TIM4_CHx_CLK, ENABLE); 	    //使能PORTE时钟	
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉(根据实际编码器修改)
        GPIO_PinAFConfig(TIM4_CH1_PORT,TIM4_CH1_PINSOURCE,GPIO_AF_TIM4);
        GPIO_PinAFConfig(TIM4_CH2_PORT,TIM4_CH2_PINSOURCE,GPIO_AF_TIM4);
        
        GPIO_InitStructure.GPIO_Pin = TIM4_CH1_PIN;
        GPIO_Init(TIM4_CH1_PORT,&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = TIM4_CH2_PIN;
        GPIO_Init(TIM4_CH2_PORT,&GPIO_InitStructure);
    }
    else if(TIMx == TIM5)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE); //TIM5时钟使能    
        RCC_AHB1PeriphClockCmd(TIM5_CHx_CLK, ENABLE); 	    //使能PORTE时钟	
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉(根据实际编码器修改)
        
        GPIO_PinAFConfig(TIM5_CH1_PORT,TIM5_CH1_PINSOURCE,GPIO_AF_TIM5);
        GPIO_PinAFConfig(TIM5_CH2_PORT,TIM5_CH2_PINSOURCE,GPIO_AF_TIM5);
        
        GPIO_InitStructure.GPIO_Pin = TIM5_CH1_PIN;
        GPIO_Init(TIM5_CH1_PORT,&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = TIM5_CH2_PIN;
        GPIO_Init(TIM5_CH2_PORT,&GPIO_InitStructure); 
    }
    else
    {
        setErrno(NON_EXIST);
    }
}

void TIMx_EncoderNVICInit(TIM_TypeDef *TIMx)
{
    NVIC_InitTypeDef   NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    
    if(TIMx == TIM3)
    {
        NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
    }

    if(TIMx == TIM4)
    {
        NVIC_InitStruct.NVIC_IRQChannel = TIM4_IRQn;
    }    

    if(TIMx == TIM5)
    {
        NVIC_InitStruct.NVIC_IRQChannel = TIM5_IRQn;
    }     
    NVIC_Init(&NVIC_InitStruct);
}

void TIMx_EncoderInit(TIM_TypeDef *TIMx)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStruct;
    
    TIMx_EncoderNVICInit(TIMx);
    
    
    TIMx_EncGpioInit(TIMx);
    
	TIM_TimeBaseStructure.TIM_Prescaler=4;                      //定时器分频,
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;   //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=0xFFFF;                    //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;       //定时器时钟分频
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);              //初始化定时器

    /*配置为编码器模式*/
    TIM_EncoderInterfaceConfig(TIMx, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStruct);
    TIM_ICInitStruct.TIM_ICFilter = 6;
    TIM_ICInit(TIMx, &TIM_ICInitStruct);
 
	TIM_Cmd(TIMx, ENABLE);  //使能TIM    
    //TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
    //TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
}

uint16_t TIMx_getEncVal(TIM_TypeDef *TIMx)
{
    uint16_t ret = 0;
    uint8_t ch = 0;
    if(TIMx == TIM3)
    {
        ch = 0;
    }
    else if(TIMx == TIM4)
    {
        ch = 1;
    }
    else if(TIMx == TIM5)
    {
        ch = 2;
    }

    ret = (encOverFlowVal[ch]+ TIMx->CNT);
    return ret;
}

void TIMx_setEncVal(TIM_TypeDef *TIMx, uint16_t val)
{    
    //encOverFlowVal[ch] = val & 0xFFFF0000;
    
    TIMx->CNT = val&0xFFFF;
}


void TIM3_IRQHandler(void)
{
    uint8_t ch = 0;
    uint16_t cnt = TIM3->CNT;
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if( cnt >0x1fff ) /*向下增加溢出*/
        {
            encOverFlowVal[ch] -= 0xFFFF;
        }
        else if(cnt <0x00FF) /*向上增加溢出*/
        {
            encOverFlowVal[ch] += 0xFFFF;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

void TIM4_IRQHandler(void)
{
    uint8_t ch = 0;
    uint16_t cnt = TIM3->CNT;
    if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        if( cnt >0x1fff ) /*向下增加溢出*/
        {
            encOverFlowVal[ch] -= 0xFFFF;
        }
        else if(cnt <0x00FF) /*向上增加溢出*/
        {
            encOverFlowVal[ch] += 0xFFFF;
        }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}
