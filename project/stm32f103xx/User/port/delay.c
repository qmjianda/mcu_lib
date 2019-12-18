#include "delay.h"


static u8  fac_us=0;							//us��ʱ������			   
static volatile uint32_t timerTick = 0;
static volatile uint64_t sysTimeUs = 0;
static void(*tickIrqCallBackFun)(void) = 0;
 
//systick�жϷ�����
void SysTick_Handler(void)
{	
    timerTick++;
    sysTimeUs += 1000;
    if(tickIrqCallBackFun)
    {
        tickIrqCallBackFun();
    }
}
			   
//��ʼ���ӳٺ���
void delay_init(u8 SYSCLK)
{
 	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//SYSTICKʹ���ⲿʱ��Դ	 
	fac_us=SYSCLK;						//�����Ƿ�ʹ��OS,fac_us����Ҫʹ��
    SysTick_Config(SystemCoreClock / 1000);
}								    

void delay_us(uint32_t nus)
{		
	volatile uint32_t ticks;
	volatile uint32_t told,tnow,tcnt=0;
	volatile uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*fac_us; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
}

//��ʱnms 
void delay_ms(uint32_t nms)
{	 	 
    volatile uint32_t nextTime;
    nextTime = timerTick+nms;
    if(nextTime < timerTick) /*�����*/
    {
        while(nextTime <= timerTick);
    }
    else /*δ���*/
    {
        while(nextTime >= timerTick);
    }
}

uint64_t now_us(void)
{
    return (sysTimeUs + SysTick->VAL);
}
uint32_t now_ms(void)
{
    return (timerTick);
}

void regSysTickCallBackFun(void(*callBackFun)(void))
{
    tickIrqCallBackFun = callBackFun;
}

