#include "bsp_timer_pwm.h"

void TIM1_PWM_OutInit(uint32_t freq)
{
    RCC_ClocksTypeDef SYS_RCC_Clocks;
    uint32_t timerClk;
    uint16_t psc;
    uint32_t arr;
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
    RCC_GetClocksFreq(&SYS_RCC_Clocks); /*��ȡϵͳʱ��*/
    timerClk = SYS_RCC_Clocks.PCLK2_Frequency*2; /*��APB2��Ƶ����Ϊ1 ʱTIMxʱ��ΪAPB2����*/
    psc = timerClk/1000000; 
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   	//TIM1ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(TIM1_CHx_CLK, ENABLE); 	        //ʹ��PORTEʱ��	
	
	GPIO_PinAFConfig(TIM1_CH1_PORT,TIM1_CH1_PIN_SOURCS,GPIO_AF_TIM1);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����      
    
    GPIO_InitStructure.GPIO_Pin = TIM1_CH1_PIN;
	GPIO_Init(TIM1_CH1_PORT,&GPIO_InitStructure);

    
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
    
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ,
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//��ʼ����ʱ��
	
    
	//��ʼ��TIM1 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  
    TIM_OCInitStructure.TIM_Pulse = 0;
    
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM1 4OC1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //ʹ��TIM1��CCR1�ϵ�Ԥװ�ؼĴ���
     
    TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPEʹ�� 
	    
	TIM_Cmd(TIM1, ENABLE);  //ʹ��TIM1
    
    TIM_CtrlPWMOutputs(TIM1, ENABLE);

}

void TIM2_PWM_OutInit(uint32_t freq)
{
    RCC_ClocksTypeDef SYS_RCC_Clocks;
    uint32_t timerClk;
    uint16_t psc;
    uint32_t arr;
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
    RCC_GetClocksFreq(&SYS_RCC_Clocks); /*��ȡϵͳʱ��*/
    timerClk = SYS_RCC_Clocks.PCLK1_Frequency*2; /*��APB2��Ƶ����Ϊ1 ʱTIMxʱ��ΪAPB2����*/
    psc = timerClk/1000000;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);   	//TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(TIM2_CHx_CLK, ENABLE); 	        //ʹ��PORTEʱ��	
	
    GPIO_PinAFConfig(TIM2_CH3_PORT,TIM2_CH3_PIN_SOURCS,GPIO_AF_TIM2);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    
    GPIO_InitStructure.GPIO_Pin = TIM2_CH3_PIN;
	GPIO_Init(TIM2_CH3_PORT,&GPIO_InitStructure);
	  
    if(freq<10000) /*1k~10K*/
    {
        psc = psc-1;  /*clock = 1MHz*/
        arr = 1000000/freq;
    }
    else
    {
        psc = 0; /*clock = 84Mhz*/
        arr =  timerClk/freq;
    }
      
	TIM_TimeBaseStructure.TIM_Prescaler= psc;  //��ʱ����Ƶ,
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period= arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	//��ʼ��TIM2 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_Pulse = 0;
    
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM2 4OC3
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);  //ʹ��TIM2��CCR1�ϵ�Ԥװ�ؼĴ���
    
    TIM_ARRPreloadConfig(TIM2,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2
}

void TIM8_PWM_OutInit(uint32_t freq)
{
    RCC_ClocksTypeDef SYS_RCC_Clocks;
    uint32_t timerClk;
    uint16_t psc;
    uint32_t arr;
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

    RCC_GetClocksFreq(&SYS_RCC_Clocks); /*��ȡϵͳʱ��*/
    timerClk = SYS_RCC_Clocks.PCLK2_Frequency*2; /*��APB2��Ƶ����Ϊ1 ʱTIMxʱ��ΪAPB2����*/
    psc = timerClk/1000000;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);   	//TIM8ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(TIM8_CHx_CLK, ENABLE); 	        //ʹ��PORTEʱ��	
	
    GPIO_PinAFConfig(TIM8_CH1_PORT,TIM8_CH1_PIN_SOURCS,GPIO_AF_TIM8);
    GPIO_PinAFConfig(TIM8_CH2_PORT,TIM8_CH2_PIN_SOURCS,GPIO_AF_TIM8);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    
    GPIO_InitStructure.GPIO_Pin = TIM8_CH1_PIN;
	GPIO_Init(TIM8_CH1_PORT,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = TIM8_CH2_PIN;
	GPIO_Init(TIM8_CH2_PORT,&GPIO_InitStructure);
	  
    if(freq<10000) /*1k~10K*/
    {
        psc = psc-1;  /*clock = 1MHz*/
        arr = 1000000/freq;
    }
    else
    {
        psc = 0; /*clock = 42Mhz*/
        arr =  timerClk/freq;
    }
      
	TIM_TimeBaseStructure.TIM_Prescaler= psc;  //��ʱ����Ƶ,
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period= arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	//��ʼ��TIM8 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //�Ƚ����ʹ��
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;  
    TIM_OCInitStructure.TIM_Pulse = 0;
    
    TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC1
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR1�ϵ�Ԥװ�ؼĴ���
    
    TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM8 OC1
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIM8��CCR1�ϵ�Ԥװ�ؼĴ���
 
    TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPEʹ�� 
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM8
    TIM_CtrlPWMOutputs(TIM8, ENABLE);
}

void TIM12_PWM_OutInit(uint32_t freq)
{
    RCC_ClocksTypeDef SYS_RCC_Clocks;
    uint32_t timerClk;
    uint16_t psc;
    uint32_t arr;
    GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
    RCC_GetClocksFreq(&SYS_RCC_Clocks); /*��ȡϵͳʱ��*/
    timerClk = SYS_RCC_Clocks.PCLK1_Frequency*2; /*��APB2��Ƶ����Ϊ1 ʱTIMxʱ��ΪAPB2����*/
    psc = timerClk/1000000;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12,ENABLE);   	//TIM2ʱ��ʹ��    
	RCC_AHB1PeriphClockCmd(TIM12_CHx_CLK, ENABLE); 	        //ʹ��PORTEʱ��	
	
    GPIO_PinAFConfig(TIM12_CH1_PORT,TIM12_CH1_PIN_SOURCS,GPIO_AF_TIM12);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //����
    
    GPIO_InitStructure.GPIO_Pin = TIM12_CH1_PIN;
	GPIO_Init(TIM12_CH1_PORT,&GPIO_InitStructure);
	  
    if(freq<10000) /*1k~10K*/
    {
        psc = psc-1;  /*clock = 1MHz*/
        arr = 1000000/freq;
    }
    else
    {
        psc = 0; /*clock = 42Mhz*/
        arr = timerClk/freq;
    }
      
	TIM_TimeBaseStructure.TIM_Prescaler= psc;  //��ʱ����Ƶ,
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period= arr;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM12,&TIM_TimeBaseStructure);//��ʼ����ʱ��2
	
	//��ʼ��TIM12 Channel1 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
    TIM_OCInitStructure.TIM_Pulse = 0;
    
    TIM_OC1Init(TIM12, &TIM_OCInitStructure);  //����Tָ���Ĳ�����ʼ������TIM12 4OC3
	TIM_OC1PreloadConfig(TIM12, TIM_OCPreload_Enable);  //ʹ��TIM12��CCR1�ϵ�Ԥװ�ؼĴ���

    TIM_ARRPreloadConfig(TIM12,ENABLE);//ARPEʹ�� 
	
	TIM_Cmd(TIM12, ENABLE);  //ʹ��TIM12
}

/**
  * @brief  ���ö�ʱ��PWMͨ����ռ�ձ�
  * @note   ֻ������ΪPWM���ģʽ�Ķ�ʱ������ʹ��
  * @param  TIMx����ʱ����ch��ͨ���ţ�dutyCycle��ռ�ձȣ����֮һ��5000Ϊ%50ռ�ձ�
  * @retval ��
  */
void TIMx_PWM_SetDutyCycle(TIM_TypeDef *TIMx, uint8_t ch,uint32_t dutyCycle)
{
    uint32_t arr = TIMx->ARR;
    if(dutyCycle > 10000)
    {
        dutyCycle = 10000;
    }
    if(IS_PWM_OUT_CH(ch))
    {
        switch(ch)
        {
            case 0:
                TIM_SetCompare1(TIMx, (arr*dutyCycle)/10000);
                break;
            case 1:
                TIM_SetCompare2(TIMx, (arr*dutyCycle)/10000);
                break;
            case 2:
                TIM_SetCompare3(TIMx, (arr*dutyCycle)/10000);
                break;
            case 3:
                TIM_SetCompare4(TIMx, (arr*dutyCycle)/10000);
                break;
            default:
                break;
        }
    }
    else
    {
        setErrno(NON_EXIST);
    }
}

/**
  * @brief  ���ö�ʱ������
  * @note   ֻ������ΪPWM���ģʽ�Ķ�ʱ������ʹ��
  * @param  TIMx����ʱ����ch��ͨ���ţ�pluse����
  * @retval ��
  */
void TIMx_PWM_SetPluse(TIM_TypeDef *TIMx, uint8_t ch,uint32_t pluse)
{
    if(IS_PWM_OUT_CH(ch))
    {
        switch(ch)
        {
            case 0:
                TIM_SetCompare1(TIMx, pluse);
                break;
            case 1:
                TIM_SetCompare2(TIMx, pluse);
                break;
            case 2:
                TIM_SetCompare3(TIMx, pluse);
                break;
            case 3:
                TIM_SetCompare4(TIMx, pluse);
                break;
            default:
                break;
        }
    }
    else
    {
        setErrno(NON_EXIST);
    }
}

void TIMx_PWM_Init(TIM_TypeDef *TIMx, uint32_t freq)
{
    if(TIMx == TIM1)
    {
        TIM1_PWM_OutInit(freq);
    }
    else if(TIMx == TIM2)
    {
        TIM2_PWM_OutInit(freq);
    }
    else if(TIMx == TIM8)
    {
        TIM8_PWM_OutInit(freq);
    }
    else if(TIMx == TIM12)
    {
        TIM12_PWM_OutInit(freq);
    }
    else
    {
        setErrno(NON_EXIST);
    }
}

