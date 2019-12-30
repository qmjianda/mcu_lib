#include "bsp_spi_bus.h"
#include "lib.h"
#include "bsp.h"


static SPI_TypeDef*  const  sSPI_CH[SPI_CH_MAX_NUM]              = {SPI1, SPI2, SPI3}; /*代表SPI通道数据*/
static uint32_t             sSPI_TIEOUT_MS[SPI_CH_MAX_NUM]       = {500,500, 500};    /*SPI超时时间*/


static SPI_InitTypeDef  SPI_InitStruct[SPI_CH_MAX_NUM];

int spiSetMode(uint8_t ch, uint16_t mode)
{
    if(IS_SPI_CH(ch))
    {
        SPI_InitStruct[ch].SPI_CPHA = mode & 0x01;
        SPI_InitStruct[ch].SPI_CPOL = mode & 0x02;
//        SPI_InitStruct[ch].SPI_DataSize = mode & 0x0800;
        SPI_InitStruct[ch].SPI_FirstBit = mode & 0x0080;
        SPI_Init(sSPI_CH[ch],&SPI_InitStruct[ch]);
        return 1;
    }
    else
    {
        return -1;
    }
}

int spiSetBaudRatePrescaler(uint8_t ch, uint16_t scaler)
{
    if(IS_SPI_CH(ch))
    {
        SPI_InitStruct[ch].SPI_BaudRatePrescaler = scaler;
        SPI_Init(sSPI_CH[ch],&SPI_InitStruct[ch]);
        return 1;
    }
    else
    {
        return -1;
    }
}

int spiSetTiemOutTime(uint8_t ch, uint32_t time)
{
    if(IS_SPI_CH(ch))
    {
        sSPI_TIEOUT_MS[ch] = time;
        return 1;
    }
    else
    {
        return -1;
    }
}

/**
  * @brief  SPI初始化函数，用来打开SPI接口
  * @note   如果要修改SPI的配置，在函数SPI结构体配置中选择合适的配置选项
  * @note   阻塞模式指的是SPI_Read时在没有收到数据前函数会阻塞在read中
  * @param  ch：通道号，可选0或1
  *         block：是否为阻塞模式
  * @retval 1：参数正确配置成功，0：参数错误配置失败
  */
uint8_t spiInit(uint8_t ch)
{
    
    /*参数有效性判断*/
    if(!(IS_SPI_CH(ch)))
    {
        return 0;
    }
    
   
    
    if(ch==0)
    {
        /*使能时钟*/
        gpioClkEnable(SPI1_CLK_PORT, true);
        gpioClkEnable(SPI1_MISO_PORT, true);
        gpioClkEnable(SPI1_MOSI_PORT, true);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);  
        
        /*GPIO CLK 初始化*/
        gpioConfig(SPI1_CLK_PORT, SPI1_CLK_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*GPIO MISO初始化*/
        gpioConfig(SPI1_MISO_PORT, SPI1_CLK_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*GPIO MOSI初始化*/
        gpioConfig(SPI1_MOSI_PORT, SPI1_MOSI_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);

        
        /*SPI 初始化*/
        SPI_InitStruct[ch].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        SPI_InitStruct[ch].SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStruct[ch].SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStruct[ch].SPI_CRCPolynomial = 7;
        SPI_InitStruct[ch].SPI_DataSize = SPI_DataSize_8b;
        SPI_InitStruct[ch].SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStruct[ch].SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStruct[ch].SPI_Mode = SPI_Mode_Master;
        SPI_InitStruct[ch].SPI_NSS = SPI_NSS_Soft;
    }
    else if(ch==1)
    {
        /*使能时钟*/
        gpioClkEnable(SPI2_CLK_PORT, true);
        gpioClkEnable(SPI2_MISO_PORT, true);
        gpioClkEnable(SPI2_MOSI_PORT, true);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);  
        
        /*GPIO CLK 初始化*/
        gpioConfig(SPI2_CLK_PORT, SPI2_CLK_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*GPIO MISO初始化*/
        gpioConfig(SPI2_MISO_PORT, SPI2_CLK_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*GPIO MOSI初始化*/
        gpioConfig(SPI2_MOSI_PORT, SPI2_MOSI_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*SPI 初始化*/
        SPI_InitStruct[ch].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        SPI_InitStruct[ch].SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStruct[ch].SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStruct[ch].SPI_CRCPolynomial = 7;
        SPI_InitStruct[ch].SPI_DataSize = SPI_DataSize_8b;
        SPI_InitStruct[ch].SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStruct[ch].SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStruct[ch].SPI_Mode = SPI_Mode_Master;
        SPI_InitStruct[ch].SPI_NSS = SPI_NSS_Soft;
    }
    else if(ch==2)
    {
        /*使能时钟*/
        gpioClkEnable(SPI3_CLK_PORT, true);
        gpioClkEnable(SPI3_MISO_PORT, true);
        gpioClkEnable(SPI3_MOSI_PORT, true);
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);  
        
        /*GPIO CLK 初始化*/
        gpioConfig(SPI3_CLK_PORT, SPI3_CLK_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*GPIO MISO初始化*/
        gpioConfig(SPI3_MISO_PORT, SPI3_CLK_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*GPIO MOSI初始化*/
        gpioConfig(SPI3_MOSI_PORT, SPI3_MOSI_PIN, GPIO_AF_PUSHPULL, GPIO_MODE_OUT50MHZ);
        
        /*SPI 初始化*/
        SPI_InitStruct[ch].SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        SPI_InitStruct[ch].SPI_CPHA = SPI_CPHA_1Edge;
        SPI_InitStruct[ch].SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStruct[ch].SPI_CRCPolynomial = 7;
        SPI_InitStruct[ch].SPI_DataSize = SPI_DataSize_8b;
        SPI_InitStruct[ch].SPI_Direction = SPI_Direction_2Lines_FullDuplex;
        SPI_InitStruct[ch].SPI_FirstBit = SPI_FirstBit_MSB;
        SPI_InitStruct[ch].SPI_Mode = SPI_Mode_Master;
        SPI_InitStruct[ch].SPI_NSS = SPI_NSS_Soft;
    }   
    SPI_Init(sSPI_CH[ch],&SPI_InitStruct[ch]);
    SPI_Cmd(sSPI_CH[ch], ENABLE);
    return 1;
}

uint16_t spiRdWrData(uint8_t ch, uint16_t data)
{
    uint32_t retry = 0;
    if(IS_SPI_CH(ch))
    {
        while ((sSPI_CH[ch]->SR & SPI_I2S_FLAG_TXE) == (uint16_t)RESET)
        {
            if((++retry)>sSPI_TIEOUT_MS[ch])
            {
                return 0;
            }
        }
        SPI_I2S_SendData(sSPI_CH[ch],data);
        
        while ((sSPI_CH[ch]->SR & SPI_I2S_FLAG_RXNE) == (uint16_t)RESET)
        {
            if((++retry)>sSPI_TIEOUT_MS[ch])
            {
                return 0;
            }
        }
        return SPI_I2S_ReceiveData(sSPI_CH[ch]);
    }
    else
    {
        return 0;
    }
}

void spiWrNdata(uint8_t ch, uint8_t *data, uint16_t dataLen)
{
    uint16_t i=0;
    while(dataLen--)
    {
        spiRdWrData(ch, data[i++]);
    }
}

void spiRdNdata(uint8_t ch, uint16_t *recData, uint16_t recLen, uint16_t dumpData)
{
    uint16_t i=0;
    while(recLen--)
    {
        recData[i] = spiRdWrData(ch, dumpData);
    }
}


