#include "bsp_adc.h"
#include "bsp_peripheral_clk.h"


static T_Adc1Val sADC1_Val = {0};

void adcCHxGpioInit(GPIO_TypeDef* gpiox, uint16_t pinx)
{
    RCC_AHB1PeriphClockCmd(getGpioClk(gpiox), ENABLE);
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Pin = pinx;
    GPIO_Init(gpiox, &GPIO_InitStruct);
}

void adc1Init(void)
{
    ADC_InitTypeDef ADC_InitStruct;
    ADC_CommonInitTypeDef ADC_CommonInitStruct;
    DMA_InitTypeDef DMA_InitStruct;
    
    RCC_AHB1PeriphClockCmd(ADC1_DMA_CLK, ENABLE); /*开启DMA时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); /*开启ADC时钟*/
    
    DMA_InitStruct.DMA_Mode = DMA_Mode_Circular; /*循环传送模式*/
    /*DMA基础配置*/
    DMA_InitStruct.DMA_Priority = DMA_Priority_High;
    DMA_InitStruct.DMA_BufferSize = ADC1_CH_MAX_NUM; /*传输的数据个数*/
    DMA_InitStruct.DMA_Channel = ADC1_DMA_CH;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable; /*不使用FIFO*/
    //DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    
    /*内存端配置*/
    DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&sADC1_Val;
    DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;

    /*外设端配置*/
    DMA_InitStruct.DMA_PeripheralBaseAddr = ADC1_DMA_PH_BASE_ADDR;
    DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    
    DMA_Init(ADC1_DMA_STREAM, &DMA_InitStruct);
    DMA_Cmd(ADC1_DMA_STREAM, ENABLE);

    /*ADC GPIO初始化*/
    adcCHxGpioInit(ADC1_CH1_GPIO_PORT, ADC1_CH1_GPIO_PIN);
    adcCHxGpioInit(ADC1_CH2_GPIO_PORT, ADC1_CH2_GPIO_PIN);
    
    ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;
    ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
    ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
    ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
    ADC_CommonInit(&ADC_CommonInitStruct);
    
    ADC_InitStruct.ADC_ContinuousConvMode = ENABLE; /*连续采样模式*/
    ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right; /*数据右对齐*/
    ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; /*使用软件触发*/
    ADC_InitStruct.ADC_NbrOfConversion = ADC1_CH_MAX_NUM;
    ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
    ADC_InitStruct.ADC_ScanConvMode = ENABLE;
    ADC_Init(ADC1, &ADC_InitStruct);
   
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_480Cycles);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 2, ADC_SampleTime_480Cycles);
     
    // 使能 DMA 请求 after last transfer (Single-ADC mode)
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    // 使能 ADC DMA
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    
    
}

void adc1Start(void)
{
    ADC_SoftwareStartConv(ADC1);
}

void adc1GetRawVal(T_Adc1Val *val)
{
    memcpy(val, &sADC1_Val, sizeof(T_Adc1Val));
}

/*用于AS5045检测*/
void cmdAdcFun(int argc, char* argv[])
{
    T_Adc1Val val;
    EXIT_MESSAGE();
    while(1)
    {
        EXIT_CHECK();
        adc1GetRawVal(&val);
        
        /*I(ma) = adcV/4096*3.3/0.2*1000*/
        float v1 = val.chVal[0]*1.007;
        float v2 = val.chVal[1]*1.007;
        printf("ADC ch1=%d ADC ch2=%d\r\n", val.chVal[0], val.chVal[1]);
        printf("I ch1=%f  I ch2=%f\r\n", v1, v2);
        delay_ms(1000);
    }
}
T_Cmd cmdAdc = 
{
    .name = "adc",
    .help = "show adc raw val",
    .cmdFun = cmdAdcFun,
};
