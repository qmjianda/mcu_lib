#ifndef _BSP_I2C_BUS_H
#define _BSP_I2C_BUS_H

#include "include.h"

#define USE_HARD_I2C    //使用硬件IIC

#define MAX_I2C_NUM     2

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

#define IS_IIC_CH(ch)       (ch<MAX_I2C_NUM)

/* 定义I2C总线连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */

#ifdef USE_HARD_I2C

/* STM32 I2C 频率 */
#define I2C_Speed               50000

#define I2C_RCC                 RCC_APB1Periph_I2C1|RCC_APB1Periph_I2C2 /*I2C时钟*/

#define GPIO_I2C_RCC            RCC_AHB1Periph_GPIOB    /*所有IO的时钟*/

#define GPIO_PORT_SCL_1	        GPIOB			        /* GPIO端口 */
#define I2C_SCL_PIN_1		    GPIO_Pin_6			    /* 连接到SCL时钟线的GPIO */
#define I2C_SCL_PIN_SOURCE_1    GPIO_PinSource6
#define I2C_SCL_AF_MODE_1       GPIO_AF_I2C1

#define GPIO_PORT_SDA_1	        GPIOB			        /* GPIO端口 */
#define I2C_SDA_PIN_1		    GPIO_Pin_7			    /* 连接到SDA数据线的GPIO */
#define I2C_SDA_PIN_SOURCE_1    GPIO_PinSource7
#define I2C_SDA_AF_MODE_1       GPIO_AF_I2C1


#define GPIO_PORT_SCL_2	        GPIOB			        /* GPIO端口 */
#define I2C_SCL_PIN_2		    GPIO_Pin_10			    /* 连接到SCL时钟线的GPIO */
#define I2C_SCL_PIN_SOURCE_2    GPIO_PinSource10
#define I2C_SCL_AF_MODE_2       GPIO_AF_I2C2

#define GPIO_PORT_SDA_2	        GPIOB			        /* GPIO端口 */
#define I2C_SDA_PIN_2		    GPIO_Pin_11			    /* 连接到SDA数据线的GPIO */
#define I2C_SDA_PIN_SOURCE_2    GPIO_PinSource11
#define I2C_SDA_AF_MODE_2       GPIO_AF_I2C2

#else /*软件IIC*/
#define GPIO_I2C_RCC        RCC_AHB1Periph_GPIOB    /*所有IO的时钟*/

#define GPIO_PORT_SCL_1	    GPIOB			        /* GPIO端口 */
#define GPIO_PORT_SDA_1	    GPIOB			        /* GPIO端口 */
#define I2C_SCL_PIN_1		GPIO_Pin_6			    /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN_1		GPIO_Pin_7			    /* 连接到SDA数据线的GPIO */

#define GPIO_PORT_SCL_2	    GPIOB			        /* GPIO端口 */
#define GPIO_PORT_SDA_2	    GPIOB			        /* GPIO端口 */
#define I2C_SCL_PIN_2		GPIO_Pin_10			    /* 连接到SCL时钟线的GPIO */
#define I2C_SDA_PIN_2		GPIO_Pin_11			    /* 连接到SDA数据线的GPIO */
#endif

uint8_t i2cRdByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata);
uint8_t i2cRdNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata, uint8_t Len);
uint8_t i2cWrByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t data);
uint8_t i2cWrNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* data, uint8_t Len);
uint8_t i2c_CheckDevice(uint8_t ch, uint8_t _Address);


#endif

