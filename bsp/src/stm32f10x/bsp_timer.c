#include "bsp_timer.h"

pvFunType g_TIM_isrFun = NULL;

void TIM7_PWM_OutInit(uint32_t freq, pvFunType isrFun)
{
    RCC_ClocksTypeDef SYS_RCC_Clocks;
    uint32_t timerClk;
    uint16_t psc;
    uint32_t arr;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
    g_TIM_isrFun = isrFun;
    
    RCC_GetClocksFreq(&SYS_RCC_Clocks); /*获取系统时钟*/
    timerClk = SYS_RCC_Clocks.PCLK2_Frequency*2; /*当APB2分频数不为1 时TIMx时钟为APB2两倍*/
    psc = timerClk/1000000; 
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   	//TIM1时钟使能    
	 
    if(freq<10000) /*10k~100K*/
    {
        psc = psc-1;  /*clock = 1MHz*/
        arr = 1000000/freq;
    }
    else
    {
        psc = 0; /*clock = 84Mhz*/
        arr =  timerClk/freq;
    }
    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频,
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器
	
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
    
	TIM_Cmd(TIM7, ENABLE);  //使能TIM1
}

void TIM7_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        if(g_TIM_isrFun)
        {
            g_TIM_isrFun();
        }
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
}
