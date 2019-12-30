#include "bsp_peripheral_clk.h"
#include "debug.h"

uint32_t getGpioClk(GPIO_TypeDef* gpiox)
{
    uint32_t clk = RCC_APB2Periph_GPIOA;
    if(gpiox == GPIOA)
    {
        clk = RCC_APB2Periph_GPIOA;
    }
    else if(gpiox == GPIOB)
    {
        clk = RCC_APB2Periph_GPIOB;
    }
    else if(gpiox == GPIOC)
    {
        clk = RCC_APB2Periph_GPIOC;
    }
    else if(gpiox == GPIOD)
    {
        clk = RCC_APB2Periph_GPIOD;
    }
    else if(gpiox == GPIOE)
    {
        clk = RCC_APB2Periph_GPIOE;
    }
    else if(gpiox == GPIOF)
    {
        clk = RCC_APB2Periph_GPIOF;
    }
    else if(gpiox == GPIOG)
    {
        clk = RCC_APB2Periph_GPIOG;
    }
    else
    {
        PERROR();
    }
    return clk;
}

uint32_t getUsartClk(USART_TypeDef* usartx)
{
    uint32_t clk = RCC_APB2Periph_USART1;
    if(usartx == USART1)
    {
        clk = RCC_APB2Periph_USART1;
    }
    else if(usartx == USART2)
    {
        clk = RCC_APB1Periph_USART2;
    }
    else if(usartx == USART3)
    {
        clk = RCC_APB1Periph_USART3;
    }
    else if(usartx == UART4)
    {
        clk = RCC_APB1Periph_UART4;
    }    
    else if(usartx == UART5)
    {
        clk = RCC_APB1Periph_UART5;
    }  
    return clk;
}

