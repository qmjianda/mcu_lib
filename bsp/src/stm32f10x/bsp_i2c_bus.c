/*
	应用说明：
	在访问I2C设备前，请先调用 i2c_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO

*/
#include "bsp_i2c_bus.h"

struct
{
    
    GPIO_TypeDef *port;
    uint16_t pin;
    #ifdef USE_HARD_I2C
    uint16_t pinSource;
    uint8_t  AFMode;
    #endif
}I2C_SCL_GPIO[MAX_I2C_NUM] = 
{
    {
        .port       = GPIO_PORT_SCL_1, 
        .pin        = I2C_SCL_PIN_1,
        #ifdef USE_HARD_I2C
        .pinSource  = I2C_SCL_PIN_SOURCE_1, 
        .AFMode     = I2C_SCL_AF_MODE_1,
        #endif
    },
    {
        .port       = GPIO_PORT_SCL_2, 
        .pin        = I2C_SCL_PIN_2,
        #ifdef USE_HARD_I2C
        .pinSource  = I2C_SCL_PIN_SOURCE_2, 
        .AFMode     = I2C_SCL_AF_MODE_2,
        #endif
    }
},

 I2C_SDA_GPIO[MAX_I2C_NUM] = 
{
    {
        .port       = GPIO_PORT_SDA_1, 
        .pin        = I2C_SDA_PIN_1,
        #ifdef USE_HARD_I2C
        .pinSource  = I2C_SDA_PIN_SOURCE_1, 
        .AFMode     = I2C_SDA_AF_MODE_1,
        #endif
    },
    {
        .port       = GPIO_PORT_SDA_2, 
        .pin        = I2C_SDA_PIN_2,
        #ifdef USE_HARD_I2C
        .pinSource  = I2C_SDA_PIN_SOURCE_2, 
        .AFMode     = I2C_SDA_AF_MODE_2,
        #endif
    }
};

#ifdef USE_HARD_I2C
void i2cModeInit(uint8_t ch);
 /*等待超时时间*/
#define I2CT_FLAG_TIMEOUT   ((uint32_t)0x10000)
#define I2CT_LONG_TIMEOUT   ((uint32_t)(10 * I2CT_FLAG_TIMEOUT))
 
I2C_TypeDef * I2C_CH[2]             = {I2C1, I2C2};
uint16_t      I2C_OWN_Addr[2]       = {0x0A,0x0C}; /*保证自身的地址与I2C器件的地址不同*/
static __IO uint32_t  I2CTimeout    = I2CT_LONG_TIMEOUT;   

/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回1，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* Block communication and all processes */
    printf("I2C outTime!errorCode = %d\r\n",errorCode);
    i2cModeInit(0);
    i2cModeInit(1);
    setErrno(TIMEOUT);
    return 1;
}

/**
  * @brief   写一个字节到I2C 器件中
  * @param   
  *	@arg pBuffer:缓冲区指针
  *	@arg WriteAddr:写地址 
  * @retval 0写成功，1写失败
  */
uint8_t i2cWrByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t data)
{
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 1;
    }
    
    I2C_AcknowledgeConfig(I2C_CH[ch], ENABLE);
    
    /*开始*/
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);
    
    /*等待事件5*/
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0) 
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            return I2C_TIMEOUT_UserCallback(0);
        }
    }    

    /*写设备地址*/
    I2C_Send7bitAddress(I2C_CH[ch], addr<<1, I2C_Direction_Transmitter);
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    /*等待事件6*/
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0) 
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            return I2C_TIMEOUT_UserCallback(1);
        }
    }    
      
    /*写寄存器地址*/
    I2C_SendData(I2C_CH[ch], regAddr);

    I2CTimeout = I2CT_FLAG_TIMEOUT;
    /*等待事件8*/
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_TRANSMITTED))  
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            return I2C_TIMEOUT_UserCallback(2);
        }
    } 
    /*发送数据*/
    I2C_SendData(I2C_CH[ch], data); 

    I2CTimeout = I2CT_FLAG_TIMEOUT;
    /*等待事件8*/
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            return I2C_TIMEOUT_UserCallback(3);
        }
    } 
    /*停止*/
    I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
    return 1;
}

/**
  * @brief   在EEPROM的一个写循环中可以写多个字节，但一次写入的字节数
  *          不能超过EEPROM页的大小，AT24C02每页有8个字节
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址
  *     @arg NumByteToWrite:写的字节数
  * @retval  返回实际写入的数据个数
  */
uint8_t i2cWrNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* data, uint8_t Len)
{
    uint8_t *pBuffer = data;
    uint8_t i=0;
    I2CTimeout = I2CT_LONG_TIMEOUT;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    I2C_AcknowledgeConfig(I2C_CH[ch], ENABLE);
    
    while(I2C_GetFlagStatus(I2C_CH[ch], I2C_FLAG_BUSY))  
    {
        if((I2CTimeout--) == 0) 
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(4);
            return 0;
        }
    }        

    /* Send START condition */
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);


    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(4);
            return 0;
        }
    } 

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_CH[ch], addr<<1, I2C_Direction_Transmitter);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(4);
            return 0;
        }
    } 
    /* Send the EEPROM's internal address to write to */    
    I2C_SendData(I2C_CH[ch], regAddr);  

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV8 and clear it */
    while(! I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_TRANSMITTED)) 
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(4);
            return 0;
        }
    } 
    /* While there is data to be written */
    while(i<Len)  
    {
        /* Send the current byte */
        I2C_SendData(I2C_CH[ch], *pBuffer); 

        /* Point to the next byte to be written */
        pBuffer++; 

        I2CTimeout = I2CT_FLAG_TIMEOUT;

        /* Test on EV8 and clear it */
        while (!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_TRANSMITTED))
        {
            if((I2CTimeout--) == 0)
            {
                I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
                I2C_TIMEOUT_UserCallback(4);
                return i;
            }
        }
        i++;
    }
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
    return i;
}

/**
  * @brief   通过I2C读取一个Byte
  * @param   
  *	@arg ch     可用的I2C通道
  *	@arg addr   器件的地址
  * @arg regAddr目标寄存器的地址
  * @arg Rdata   存储读取到的数据
  * @retval  
  */
uint8_t i2cRdByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata)
{
    uint8_t *pBuffer = Rdata;    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    I2C_AcknowledgeConfig(I2C_CH[ch], ENABLE);
    
    while(I2C_GetFlagStatus(I2C_CH[ch], I2C_FLAG_BUSY))   
    {
        if((I2CTimeout--) == 0)
        {
            I2C_TIMEOUT_UserCallback(9);
            return 0;
        }
    }
    /* Send START condition */
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(10);
            return 0;
        }
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_CH[ch], addr<<1, I2C_Direction_Transmitter);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(11);
            return 0;
        }
    }
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C_CH[ch], ENABLE);
    
    /* send  read regist*/
    I2C_SendData(I2C_CH[ch], regAddr);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(12);
            return 0;
        }
    }
    
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(13);
            return 0;
        }
    }
    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C_CH[ch], addr<<1, I2C_Direction_Receiver);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(14);
            return 0;
        }
    }
    /* Disable Acknowledgement */
    I2C_AcknowledgeConfig(I2C_CH[ch], DISABLE);
    I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    while(I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(3);
            return 0;
        }
    } 	
    /* Read a byte from the device */
    *pBuffer = I2C_ReceiveData(I2C_CH[ch]); 			
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C_CH[ch], ENABLE);
    return 1;
}

/**
  * @brief   通过I2C读取一组数据
  * @param   
  *	@arg ch     可用的I2C通道
  *	@arg addr   器件的地址
  * @arg regAddr目标寄存器的地址
  * @arg data   存储读取到的数据
  * @arg Len    存储缓冲区的大小
  * @retval  无
  */
uint8_t i2cRdNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* data, uint8_t Len)
{ 
    uint8_t i = 0;
    uint8_t *pBuffer = data;    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    I2C_AcknowledgeConfig(I2C_CH[ch], ENABLE);
    while(I2C_GetFlagStatus(I2C_CH[ch], I2C_FLAG_BUSY))   
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(9);
            return 0;
        }
    }
    /* Send START condition */
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(10);
            return 0;
        }
    }

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_CH[ch], addr<<1, I2C_Direction_Transmitter);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(11);
            return 0;
        }
    }
    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C_CH[ch], ENABLE);
    
    /* send  read regist*/
    I2C_SendData(I2C_CH[ch], regAddr);
    
    I2CTimeout = I2CT_FLAG_TIMEOUT;
    
    /* Test on EV8 and clear it */
    while (!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(12);
            return 0;
        }
    }
    
    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(13);
            return 0;
        }
    }
    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C_CH[ch], addr<<1, I2C_Direction_Receiver);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0)
        {
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
            I2C_TIMEOUT_UserCallback(14);
            return 0;
        }
    }
    /* While there is data to be read */
    while(i<Len)  
    {
        if(i == Len-1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C_CH[ch], DISABLE);

            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
        }


        I2CTimeout = I2CT_LONG_TIMEOUT;
        while(I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
        {
            if((I2CTimeout--) == 0)
            {
                I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
                I2C_TIMEOUT_UserCallback(3);
                return i;
            }
        } 	
        {
            /* Read a byte from the device */
            *pBuffer = I2C_ReceiveData(I2C_CH[ch]);

            /* Point to the next location where the byte read will be saved */
            pBuffer++; 

            /* Decrement the read bytes counter */
            i++;
        }   			
    }
    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C_CH[ch], ENABLE);
    return i;
}

#else
/* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */
#if 1	/* 条件编译： 1 选择GPIO的库函数实现IO读写 */
	#define I2C_SCL_H(ch)  GPIO_SetBits(I2C_SCL_GPIO[ch].port, I2C_SCL_GPIO[ch].pin)		/* SCL = 1 */
	#define I2C_SCL_L(ch)  GPIO_ResetBits(I2C_SCL_GPIO[ch].port, I2C_SCL_GPIO[ch].pin)		/* SCL = 0 */
	
	#define I2C_SDA_H(ch)  GPIO_SetBits(I2C_SDA_GPIO[ch].port, I2C_SDA_GPIO[ch].pin)		/* SDA = 1 */
	#define I2C_SDA_L(ch)  GPIO_ResetBits(I2C_SDA_GPIO[ch].port, I2C_SDA_GPIO[ch].pin)		/* SDA = 0 */
	
	#define I2C_SDA_READ(ch)  (GPIO_ReadInputDataBit(I2C_SDA_GPIO[ch].port, I2C_SDA_GPIO[ch].pin))	/* 读SDA口线状态 */
#else	/* 这个分支选择直接寄存器操作实现IO读写 */
    /*　注意：如下写法，在IAR最高级别优化时，会被编译器错误优化 */
	#define I2C_SCL_H(ch)  I2C_SCL_GPIO[ch].port->BSRR |= I2C_SCL_GPIO[ch].pin;   /* SCL = 1 */
	#define I2C_SCL_L(ch)  I2C_SCL_GPIO[ch].port->BSRR |= (I2C_SCL_GPIO[ch].pin<<16)   /* SCL = 0 */
	
	#define I2C_SDA_H(ch)  I2C_SDA_GPIO[ch].port->BSRR |= I2C_SDA_GPIO[ch].pin				/* SDA = 1 */
	#define I2C_SDA_L(ch)  I2C_SDA_GPIO[ch].port->BSRR |= (I2C_SDA_GPIO[ch].pin<<16)				/* SDA = 0 */
	
	#define I2C_SDA_READ(ch)  ((I2C_SDA_GPIO[ch].port->IDR & I2C_SDA_GPIO[ch].pin) != 0)	/* 读SDA口线状态 */
#endif

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint16_t i=0;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为20时，SCL频率 = 205KHz 
		循环次数为14时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为10时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i <25; i++)
    {
        __nop();
    }
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线启动信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Start(uint8_t ch)
{
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
    /* upg_mark 后续需要在发送起始信号时检测是否有总线使用权*/
	I2C_SDA_H(ch);
	I2C_SCL_H(ch);
	i2c_Delay();
	I2C_SDA_L(ch);
	i2c_Delay();
	I2C_SCL_L(ch);
	i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Start
*	功能说明: CPU发起I2C总线停止信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Stop(uint8_t ch)
{
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_L(ch);
	I2C_SCL_H(ch);
	i2c_Delay();
	I2C_SDA_H(ch);
    i2c_Delay();
}

/*
*********************************************************************************************************
*	函 数 名: i2c_SendByte
*	功能说明: CPU向I2C总线设备发送8bit数据
*	形    参：_ucByte ： 等待发送的字节
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_SendByte(uint8_t ch,uint8_t _ucByte)
{
	uint8_t i;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
	/* 先发送字节的高位bit7 */
	for (i = 0; i < 8; i++)
	{		
		if (_ucByte & 0x80)
		{
			I2C_SDA_H(ch);
		}
		else
		{
			I2C_SDA_L(ch);
		}
		i2c_Delay();
		I2C_SCL_H(ch);
		i2c_Delay();	
		I2C_SCL_L(ch);
		if (i == 7)
		{
            i2c_Delay();
			I2C_SDA_H(ch); // 释放总线
		}
		_ucByte <<= 1;	/* 左移一个bit */
		i2c_Delay();
	}
}

/*
*********************************************************************************************************
*	函 数 名: i2c_ReadByte
*	功能说明: CPU从I2C总线设备读取8bit数据
*	形    参：无
*	返 回 值: 读到的数据
*********************************************************************************************************
*/
uint8_t i2c_ReadByte(uint8_t ch)
{
	uint8_t i;
	uint8_t value;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_H(ch);
		i2c_Delay();
		if (I2C_SDA_READ(ch))
		{
			value++;
		}
		I2C_SCL_L(ch);
		i2c_Delay();
	}
	return value;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_WaitAck
*	功能说明: CPU产生一个时钟，并读取器件的ACK应答信号
*	形    参：无
*	返 回 值: 返回0表示正确应答，1表示无器件响应
*********************************************************************************************************
*/
uint8_t i2c_WaitAck(uint8_t ch)
{
	uint8_t re;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 1;
    }
	I2C_SDA_H(ch);	/* CPU释放SDA总线 */
	i2c_Delay();
	I2C_SCL_H(ch);	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
	if (I2C_SDA_READ(ch))	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
	I2C_SCL_L(ch);
	i2c_Delay();
	return re;
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Ack
*	功能说明: CPU产生一个ACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_Ack(uint8_t ch)
{
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
	I2C_SDA_L(ch);	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_H(ch);	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_L(ch);
	i2c_Delay();
	I2C_SDA_H(ch);	/* CPU释放SDA总线 */
}

/*
*********************************************************************************************************
*	函 数 名: i2c_NAck
*	功能说明: CPU产生1个NACK信号
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_NAck(uint8_t ch)
{
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
	I2C_SDA_H(ch);	/* CPU驱动SDA = 1 */
	i2c_Delay();
	I2C_SCL_H(ch);	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_L(ch);
	i2c_Delay();	
}


/*
 * 说明：向ltc2992写一个byte
 * 参数：ch=0 input ch=1output
 *返回0写失败， 返回1写成功
 */
uint8_t i2cWrByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t data)
{
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    i2c_Start(ch);
    i2c_SendByte(ch, addr|I2C_WR);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    i2c_SendByte(ch, regAddr);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    i2c_SendByte(ch, data);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    i2c_Stop(ch);
    return 1;
}

/*返回实际写入的数据量*/
uint8_t i2cWrNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* data, uint8_t Len)
{
    uint8_t i;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    i2c_Start(ch);
    i2c_SendByte(ch, (addr<<1)|I2C_WR);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    
    i2c_SendByte(ch, regAddr);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    
    for(i=0; i<Len; i++)
    {
        i2c_SendByte(ch, data[i]);
        if(i2c_WaitAck(ch))
        {
            break;
        }
    }
    i2c_Stop(ch);
    return i;
}

/*返回0失败， 返回1成功*/
uint8_t i2cRdByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata)
{
    i2c_Start(ch);
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    i2c_SendByte(ch, (addr<<1)|I2C_WR);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    i2c_SendByte(ch, regAddr);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    i2c_Start(ch);
    i2c_SendByte(ch, (addr<<1)|I2C_RD);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    
    *Rdata = i2c_ReadByte(ch);
    i2c_NAck(ch);
    i2c_Stop(ch);
    return 1;
}


uint8_t i2cRdNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata, uint8_t Len)
{
    uint8_t i;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    i2c_Start(ch);
    i2c_SendByte(ch, (addr<<1)|I2C_WR);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    i2c_SendByte(ch, regAddr);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    
    i2c_Start(ch);
    i2c_SendByte(ch, (addr<<1)|I2C_RD);
    if(i2c_WaitAck(ch))
    {
        i2c_Stop(ch);
        return 0;
    }
    
    for(i=0; i<Len-1; i++)
    {
        Rdata[i] = i2c_ReadByte(ch);
        i2c_Ack(ch);
				
    }
    Rdata[i] = i2c_ReadByte(ch);
    i2c_NAck(ch);
    i2c_Stop(ch);
    return 1;
}
#endif


/*
*********************************************************************************************************
*	函 数 名: i2c_CfgGpio
*	功能说明: 配置I2C总线的GPIO，采用模拟IO的方式实现
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void i2c_CfgGpio(uint8_t ch)
{
    
    #ifdef USE_HARD_I2C
    GPIO_InitTypeDef GPIO_InitStructure;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
	/* 打开GPIO时钟 */
    RCC_AHB1PeriphClockCmd(GPIO_I2C_RCC, ENABLE);	
    GPIO_PinAFConfig(I2C_SCL_GPIO[ch].port, I2C_SCL_GPIO[ch].pinSource, I2C_SCL_GPIO[ch].AFMode);
    GPIO_PinAFConfig(I2C_SDA_GPIO[ch].port, I2C_SDA_GPIO[ch].pinSource, I2C_SDA_GPIO[ch].AFMode);
    
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; /* 开漏输出 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = I2C_SCL_GPIO[ch].pin;
    GPIO_Init(I2C_SCL_GPIO[ch].port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_GPIO[ch].pin;
    GPIO_Init(I2C_SDA_GPIO[ch].port, &GPIO_InitStructure);
    
    #else
	GPIO_InitTypeDef GPIO_InitStructure;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }

	RCC_AHB1PeriphClockCmd(GPIO_I2C_RCC, ENABLE);	/* 打开GPIO时钟 */
	GPIO_InitStructure.GPIO_Pin = I2C_SCL_GPIO[ch].pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; /* 开漏输出 */
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  	
	GPIO_Init(I2C_SCL_GPIO[ch].port, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = I2C_SDA_GPIO[ch].pin;
    GPIO_Init(I2C_SDA_GPIO[ch].port, &GPIO_InitStructure);
	/* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
	i2c_Stop(ch);
    #endif
}

#ifdef USE_HARD_I2C
void i2cModeInit(uint8_t ch)
{
    
    I2C_InitTypeDef I2C_InitStruct;
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
    
    if(ch == 0)
    {
        /* 打开硬件I2C时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
    }
    else
    {
        /* 打开硬件I2C时钟 */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    }
    
    I2C_DeInit(I2C_CH[ch]);
    I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStruct.I2C_ClockSpeed = I2C_Speed;
    I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStruct.I2C_OwnAddress1 =  I2C_OWN_Addr[ch];
    I2C_Init(I2C_CH[ch], &I2C_InitStruct);
    I2C_Cmd(I2C_CH[ch], ENABLE);
}
#endif


/*
*********************************************************************************************************
*	函 数 名: i2c_CheckDevice
*	功能说明: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
*	形    参：_Address：设备的I2C总线地址
*	返 回 值: 返回值 0 表示正确， 返回1表示未探测到
*********************************************************************************************************
*/
uint8_t i2c_CheckDevice(uint8_t ch, uint8_t _Address)
{
	uint8_t ucAck = 0;
    
    if(!IS_IIC_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
	i2c_CfgGpio(ch);		/* 配置GPIO */
    
#ifdef USE_HARD_I2C
    i2cModeInit(ch);
    /* Send STRAT condition */
    I2C_GenerateSTART(I2C_CH[ch], ENABLE);

    I2CTimeout = I2CT_FLAG_TIMEOUT;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_MODE_SELECT))
    {
        if((I2CTimeout--) == 0) 
        {
            ucAck = 1;
            break;
        }
    }    

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C_CH[ch], _Address<<1, I2C_Direction_Transmitter);


    I2CTimeout = I2CT_FLAG_TIMEOUT;
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C_CH[ch], I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if((I2CTimeout--) == 0) 
        {
            ucAck = 1;
            break;
        }
    }
    
    if(ucAck == 1)
    {
        setErrno(DEV_UNCONNECT);
    }
    
    I2C_GenerateSTOP(I2C_CH[ch], ENABLE);
    return ucAck;
    
#else
	i2c_Start(ch);		/* 发送启动信号 */

	/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	i2c_SendByte(ch,(_Address<<1) | I2C_WR);
	ucAck = i2c_WaitAck(ch);	/* 检测设备的ACK应答 */

	i2c_Stop(ch);			/* 发送停止信号 */

	return ucAck; 
#endif	
}

