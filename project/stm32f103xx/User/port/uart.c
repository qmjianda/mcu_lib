#include "uart.h"
#include "lib_rng.h"

#define UART_BUF_SIZE (64)

char    uartRxBuff[UART_BUF_SIZE]; /*���ڻ���*/
T_Ring  uartRxRng = {0}; 
RING_ID uartRxRngID = {0};


void uartInit(uint32_t baudrate)
{
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef GPIO_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;
    
    /* ����ʱ�� */
    RCC_APB2PeriphClockCmd(UART1_RX_CLK|UART1_TX_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(UARTxClk, ENABLE);
    
    /*����*/
    uartRxRngID = xrngInit(&uartRxRng, uartRxBuff, UART_BUF_SIZE);

    GPIO_InitStruct.GPIO_Mode   = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed  = GPIO_Speed_50MHz;
    
    /* ����UART */
    USART_InitStruct.USART_BaudRate = baudrate;
    USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
    USART_InitStruct.USART_Parity = USART_Parity_No;
    USART_InitStruct.USART_StopBits = USART_StopBits_1;
    USART_InitStruct.USART_WordLength = USART_WordLength_8b; 
    
    /* ����RX */
    GPIO_InitStruct.GPIO_Pin = UART1_RX_PIN;
    GPIO_Init(UART1_RX_PORT, &GPIO_InitStruct);
    /* ����TX */
    GPIO_InitStruct.GPIO_Pin = UART1_TX_PIN;
    GPIO_Init(UART1_TX_PORT, &GPIO_InitStruct);    
    /* ����UART��NVIC*/
    NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    
    USART_DeInit(UARTx);
    USART_Init(UARTx, &USART_InitStruct);
    USART_ITConfig(UARTx, USART_IT_RXNE, ENABLE);

    NVIC_Init(&NVIC_InitStruct);
    USART_Cmd(UARTx, ENABLE); 
}

void USART1_IRQHandler(void)
{
    /*����*/
    if(USART_GetFlagStatus(UARTx, USART_FLAG_RXNE) != RESET)
    {
        xrngPutForce(uartRxRngID, (char)(UARTx->DR & 0xFF));
        USART_ClearFlag(UARTx, USART_FLAG_RXNE);
    }
}

int uartWrChar(uint8_t data)
{
	USART_SendData(UARTx,data);
	while (USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET){}
    return 1;
}

int uartRdChar(uint8_t *cha)
{
    if(xrngGet(uartRxRngID, (char*)cha))
    {
        return 1;
    }
    return 0;
}

void uartClean(void)
{
    xrngClear(uartRxRngID);
}

/*�ض���*/
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

int _ttywrch(int ch)
{
    return ch = ch;
}

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
int _sys_exit(int x) 
{ 
	return x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
    uartWrChar(ch);
	return ch;
}
