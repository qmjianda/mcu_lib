#ifndef __BSP_SPI_BUS_H_
#define __BSP_SPI_BUS_H_
#include "include.h"

#define SPI_CH_MAX_NUM      (3)
#define IS_SPI_CH(s)        (s<SPI_CH_MAX_NUM) /*用来判断输入的参数有效性*/

/*需要自行管理CSN引脚*/
#define SPI1_CLK_PORT           GPIOA
#define SPI1_CLK_PIN            5
#define SPI1_MISO_PORT          GPIOA
#define SPI1_MISO_PIN           6
#define SPI1_MOSI_PORT          GPIOA
#define SPI1_MOSI_PIN           7

#define SPI2_CLK_PORT           GPIOB
#define SPI2_CLK_PIN            13
#define SPI2_MISO_PORT          GPIOB
#define SPI2_MISO_PIN           14
#define SPI2_MOSI_PORT          GPIOB
#define SPI2_MOSI_PIN           15

#define SPI3_CLK_PORT           GPIOB
#define SPI3_CLK_PIN            3
#define SPI3_MISO_PORT          GPIOB
#define SPI3_MISO_PIN           4
#define SPI3_MOSI_PORT          GPIOB
#define SPI3_MOSI_PIN           5


/*模式选择*/
/*{*/
    #define SPI_CPHA_1_EDGE         ((uint16_t)0x0000)
    #define SPI_CPHA_2_EDGE         ((uint16_t)0x0001)

    #define SPI_CPOL_LOW            ((uint16_t)0x0000)
    #define SPI_CPOL_HIGH           ((uint16_t)0x0002)

    #define SPI_FIRST_BIT_MSB       ((uint16_t)0x0000)
    #define SPI_FIRST_BIT_LSB       ((uint16_t)0x0080)
/*}*/

/*时钟分频选择*/
/*{*/
    #define SPI_BAUDRATE_PRESCALER_2    ((uint16_t)0x0000)
    #define SPI_BAUDRATE_PRESCALER_4    ((uint16_t)0x0008)
    #define SPI_BAUDRATE_PRESCALER_8    ((uint16_t)0x0010)
    #define SPI_BAUDRATE_PRESCALER_16   ((uint16_t)0x0018)
    #define SPI_BAUDRATE_PRESCALER_32   ((uint16_t)0x0020)
    #define SPI_BAUDRATE_PRESCALER_64   ((uint16_t)0x0028)
    #define SPI_BAUDRATE_PRESCALER_128  ((uint16_t)0x0030)
    #define SPI_BAUDRATE_PRESCALER_256  ((uint16_t)0x0038)
/*}*/


/*设备默认状态
CPHA              = SPI_CPHA_1_EDGE
CPOL              = SPI_CPOL_LOW
DATA_SAIZE        = SPI_DATA_SIZE_8
BaudRatePrescaler = SPI_BAUDRATE_PRESCALER_16
FirstBit          = SPI_FIRST_BIT_MSB
TIME_OUT          = 500
*/

uint8_t spiInit(uint8_t ch);
int spiSetTiemOutTime(uint8_t ch, uint32_t time);
int spiSetBaudRatePrescaler(uint8_t ch, uint16_t scaler);
int spiSetMode(uint8_t ch, uint16_t mode);
uint16_t spiRdWrData(uint8_t ch, uint16_t data);
void spiWrNdata(uint8_t ch, uint8_t *data, uint16_t dataLen);
void spiRdNdata(uint8_t ch, uint16_t *recData, uint16_t recLen, uint16_t dumpData);
#endif /*__BSP_SPI_BUS_H_*/
