#include "bsp_uart.h"
#include "lib_rng.h"
#include "bsp_gpio.h"
#include "debug.h"
#include "bsp_peripheral_clk.h"

#define UART_CH_MAX_NUM (5)
#define IS_UART_CH(s)        (s<UART_CH_MAX_NUM) /*用来判断输入的参数有效性*/
/*UART1*/
#define UART1_RX_PORT       (GPIOA)
#define UART1_RX_PIN        (10)
#define UART1_TX_PORT       (GPIOA)
#define UART1_TX_PIN        (9)

/*UART2*/
#define UART2_RX_PORT       (GPIOA)
#define UART2_RX_PIN        (3)
#define UART2_TX_PORT       (GPIOA)
#define UART2_TX_PIN        (2)

/*UART3*/
#define UART3_RX_PORT       (GPIOB)
#define UART3_RX_PIN        (11)
#define UART3_TX_PORT       (GPIOB)
#define UART3_TX_PIN        (10)

/*UART4*/
#define UART4_RX_PORT       (GPIOC)
#define UART4_RX_PIN        (11) 
#define UART4_TX_PORT       (GPIOC)
#define UART4_TX_PIN        (10)

/*UART5*/
#define UART5_RX_PORT       (GPIOD)
#define UART5_RX_PIN        (2)   
#define UART5_TX_PORT       (GPIOC)
#define UART5_TX_PIN        (12)


T_GpioPin uartRxPin[UART_CH_MAX_NUM] = 
{
    {UART1_RX_PORT, UART1_RX_PIN},
    {UART2_RX_PORT, UART2_RX_PIN},
    {UART3_RX_PORT, UART3_RX_PIN},
    {UART4_RX_PORT, UART4_RX_PIN},
    {UART5_RX_PORT, UART5_RX_PIN}    
};

T_GpioPin uartTxPin[UART_CH_MAX_NUM] = 
{
    {UART1_TX_PORT, UART1_TX_PIN},
    {UART2_TX_PORT, UART2_TX_PIN},
    {UART3_TX_PORT, UART3_TX_PIN},
    {UART4_TX_PORT, UART4_TX_PIN},
    {UART5_TX_PORT, UART5_TX_PIN}
};

AFIO_REMAP uartPinAf[UART_CH_MAX_NUM] = 
{
    AFIO_USART1_NO_REMAP,
    AFIO_USART2_NO_REMAP,
    AFIO_USART3_NO_REMAP,
    AFIO_UNAVAILABLE_REMAP,
    AFIO_UNAVAILABLE_REMAP
};

/*回调函数*/
typedef void(*T_uartCallBackFun)(void);
T_uartCallBackFun uartSendBefor[UART_CH_MAX_NUM] = {0};
T_uartCallBackFun uartSendafter[UART_CH_MAX_NUM] = {0};


static USART_TypeDef* const usartCH[UART_CH_MAX_NUM] = {USART1, USART2, USART3, UART4, UART5};

static char    uartRxBuff[UART_CH_MAX_NUM][UART_BUF_SIZE]; /*串口缓存*/
static char    uartTxBuff[UART_CH_MAX_NUM][UART_BUF_SIZE]; /*串口缓存*/

static T_Ring  uartRxRng[UART_CH_MAX_NUM] = {0}; 
static T_Ring  uartTxRng[UART_CH_MAX_NUM] = {0}; 

static RING_ID uartRxRngID[UART_CH_MAX_NUM] = {0};
static RING_ID uartTxRngID[UART_CH_MAX_NUM] = {0};

static USART_InitTypeDef USART_InitStruct[UART_CH_MAX_NUM];

static int uartConfigMode(uint8_t ch, uint16_t config)
{
    if(IS_UART_CH(ch))
    {
        USART_InitStruct[ch].USART_Mode = config & 0x0F;;
        USART_InitStruct[ch].USART_Parity = config & 0x0F00;
        USART_InitStruct[ch].USART_StopBits = config & 0xF000;
        USART_InitStruct[ch].USART_WordLength = USART_WordLength_8b; 
        USART_Init(usartCH[ch], &USART_InitStruct[ch]);
        return 1;
    }
    else
    {
        PERROR();
    }
    return -1;
}

static int uartConfigBaudRate(uint8_t ch, uint32_t baudRate)
{
    if(IS_UART_CH(ch))
    {
        USART_InitStruct[ch].USART_BaudRate = baudRate;
        USART_Init(usartCH[ch], &USART_InitStruct[ch]);
        return 1;
    }
    else
    {
        PERROR();
    }
    return -1;
}

static int uartConfigCallBackFunSendBefor(uint8_t ch, void(*before)(void))
{
    if(IS_UART_CH(ch))
    {
        uartSendBefor[ch] = before;
        return 1;
    }
    else
    {
        PERROR();
        return -1;
    }
}

static int uartConfigCallBackFunSendafter(uint8_t ch, void(*after)(void))
{
    if(IS_UART_CH(ch))
    {
        uartSendafter[ch] = after;
        return 1;
    }
    else
    {
        PERROR();
        return -1;
    }
}

static void uartISR(uint8_t ch)
{
    char cha;

    /*接收*/
    if(USART_GetFlagStatus(usartCH[ch], USART_FLAG_RXNE) != RESET)
    {
        xrngPutForce(uartRxRngID[ch], (char)(usartCH[ch]->DR & 0xFF));
        USART_ClearFlag(usartCH[ch], USART_FLAG_RXNE);
    }
    
    /*发送*/
    if((usartCH[ch]->SR & USART_FLAG_TXE) != (uint16_t)RESET)
    {
        if((usartCH[ch]->CR1 & (1<<6)) == 0) /*判断是否未开启发送完成中断*/
        {
            if(xrngGet(uartTxRngID[ch], &cha))
            {
                usartCH[ch]->DR = cha & (uint16_t)0x01FF;
                usartCH[ch]->CR1 |= (1<<6); /*开启发送完成中断*/
            }
            else
            {
                if(uartSendafter[ch])
                {
                    uartSendafter[ch]();
                }
            }
        }
        usartCH[ch]->CR1 &= ~(1<<7); /*关闭发送缓冲区空中断*/
        usartCH[ch]->SR = (uint16_t)~USART_FLAG_TXE;
    }
    
    if((usartCH[ch]->SR & USART_FLAG_TC) != (uint16_t)RESET)
    {
        if(xrngGet(uartTxRngID[ch], &cha))
        {
            usartCH[ch]->DR = cha & (uint16_t)0x01FF;
        }
        else
        {
            usartCH[ch]->CR1 &= ~(1<<6); /*关闭发送完成中断*/
            if(uartSendafter[ch])
            {
                uartSendafter[ch]();
            }
        }
        usartCH[ch]->SR = (uint16_t)~USART_FLAG_TC;
    }
}

void USART1_IRQHandler(void)
{
    uartISR(0);
}

void USART2_IRQHandler(void)
{
    uartISR(1);
}

void USART3_IRQHandler(void)
{
    uartISR(2);
}

void UART4_IRQHandler(void)
{
    uartISR(3);
}

void UART5_IRQHandler(void)
{
    uartISR(4);
}

void USART6_IRQHandler(void)
{
    uartISR(5);
}

void uartInit(uint8_t ch)
{
    NVIC_InitTypeDef NVIC_InitStruct;
    
    if(!IS_UART_CH(ch))
    {
        PERROR();
        return;
    }
    
    /*接收*/
    uartRxRngID[ch] = xrngInit(&uartRxRng[ch], uartRxBuff[ch], UART_BUF_SIZE);
    
    /*发送*/
    uartTxRngID[ch] = xrngInit(&uartTxRng[ch], uartTxBuff[ch], UART_BUF_SIZE);
    
    
    /* 开启时钟 */
    gpioClkEnable(uartRxPin[ch].GPIOx, true);
    gpioClkEnable(uartTxPin[ch].GPIOx, true);
    if(ch == 0)
    {
        RCC_APB2PeriphClockCmd(getUsartClk(USART1), ENABLE);
    }
    else
    {
        RCC_APB1PeriphClockCmd(getUsartClk(usartCH[ch]), ENABLE);
    }
    
    /* 配置RX */
    gpioConfig(uartRxPin[ch].GPIOx, uartRxPin[ch].num, GPIO_IN_FLOATING, GPIO_MODE_INPUT);
    /* 配置TX */
    gpioConfig(uartTxPin[ch].GPIOx, uartTxPin[ch].num, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
    
    /* 配置AF */
    gpioPinAF(uartPinAf[ch]);
    
    /* 配置UART的NVIC*/
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2; 
    
    
    /* 配置UART */
    USART_InitStruct[ch].USART_BaudRate = 115200;
    USART_InitStruct[ch].USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct[ch].USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct[ch].USART_Parity = USART_Parity_No;
    USART_InitStruct[ch].USART_StopBits = USART_StopBits_1;
    USART_InitStruct[ch].USART_WordLength = USART_WordLength_8b; 
        
    USART_Init(usartCH[ch], &USART_InitStruct[ch]);
    USART_ITConfig(usartCH[ch], USART_IT_RXNE, ENABLE);
    USART_ITConfig(usartCH[ch], USART_IT_TXE, DISABLE);

    NVIC_Init(&NVIC_InitStruct);
    USART_Cmd(usartCH[ch], ENABLE); 
}

void uartEnableSendIT(uint8_t ch)
{
    if((usartCH[ch]->CR1 & (1<<7)) == 0)
    {
        usartCH[ch]->CR1 |= (1<<7); /*使能发送中断*/
    }
}

int uartWrChar(uint8_t ch, uint16_t data)
{
    if(IS_UART_CH(ch))
    {
        if(uartSendBefor[ch])
        {
            uartSendBefor[ch]();
        }
        while(xrngIsFull(uartTxRngID[ch]))
        {
        }
        if(xrngPut(uartTxRngID[ch],data) == true)
        {
            uartEnableSendIT(ch);
            return 0;
        }
    }
    else
    {
        PERROR();
    }
    return -1;
}

int uartWrCharBlock(uint8_t ch, uint16_t data)
{
    uartWrChar(ch, data);
    while(xrngIsEmpty(uartTxRngID[ch]) == false )
    {
    }
    return 1;
}

int uartWrNCharBlock(int ch, uint8_t* buffer, uint16_t bufferLen)
{
    int ret = uartWrNChar(ch, buffer, bufferLen);
    while(xrngIsEmpty(uartTxRngID[ch]) == false )
    {
    }
    return ret;
}

int uartRdChar(uint8_t ch)
{
    uint8_t  temp;
    if(IS_UART_CH(ch))
    {
        if(xrngGet(uartRxRngID[ch], (char*)&temp))
        {
            return (uint8_t)temp;
        }
    }
    else
    {
        PERROR();
    }
    return -1;
}

int uartWrNChar(int ch, uint8_t* buffer, uint16_t bufferLen)
{
    uint16_t i=0;
    if(IS_UART_CH(ch))
    {
        if(uartSendBefor[ch])
        {
            uartSendBefor[ch]();
        }
        
        while(i<bufferLen)
        {
            if(xrngPut(uartTxRngID[ch], buffer[i]) != true)
            {
                uartEnableSendIT(ch); /*使能发送中断*/
                while(xrngIsFull(uartTxRngID[ch]))
                {
                }
            }
            else
            {
                i++;
            }
        }
        uartEnableSendIT(ch); /*使能发送中断*/
        return i;
    }
    else
    {
        PERROR();
    }
    return -1;
}

int uartRdNChar(int ch, uint8_t* buffer, uint16_t bufferLen)
{
    /*设备数据读取操作*/
    if(IS_UART_CH(ch))
    {
        return xrngBufGet(uartRxRngID[ch], (char*)buffer, bufferLen);
    }
    else
    {
        PERROR();
    }
    return -1;  
}

int uartCtrl(int ch, uint8_t flag, uint32_t param)
{
    int ret = -1;
    switch(flag)
    {
        case UART_MODE:
        {
            ret = uartConfigMode(ch,param);
        }
        break;
        case UART_BAUDRATE:
        {
            ret = uartConfigBaudRate(ch, param);
        }
        break;
        case UART_REG_BEFOR_FUN:
        {
            ret = uartConfigCallBackFunSendBefor(ch, (T_uartCallBackFun)param);
        }
        break;
        case UART_REG_AFTER_FUN:
        {
            ret = uartConfigCallBackFunSendafter(ch, (T_uartCallBackFun)param);
        }
        break;
        default:
            PERROR();
            break;
    }
    return ret;
}

