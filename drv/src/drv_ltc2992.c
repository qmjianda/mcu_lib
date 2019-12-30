#include "drv_ltc2992.h"
#include "bsp.h"

/*
 * 说明：向ltc2992写一个byte
 * 参数：ch=0 input ch=1output
 *返回0写失败， 返回1写成功
 */
uint8_t ltc2992WrByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t data)
{
    return i2cWrByte(ch, addr, regAddr, data);
}

/*返回实际写入的数据量*/
uint8_t ltc2992WrNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* data, uint8_t Len)
{
    return i2cWrNByte(ch, addr, regAddr, data, Len);
}

/*返回0失败， 返回1成功*/
uint8_t ltc2992RdByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata)
{
    return i2cRdByte(ch, addr, regAddr, Rdata);
}


uint8_t ltc2992RdNByte(uint8_t ch, uint8_t addr, uint8_t regAddr, uint8_t* Rdata, uint8_t Len)
{
    return i2cRdNByte(ch, addr, regAddr, Rdata, Len);
}

void ltc2992Init(void)
{
    uint8_t ok = 1;
    /*设备检测*/
    if(i2c_CheckDevice(0, LTC2992_1_DEVID_7BIT))
    {
        printf("ltc2992(1) does not exist!\r\n");
        setErrno(DEV_UNCONNECT);
        ok = 0;
    }

    if(i2c_CheckDevice(1, LTC2992_2_DEVID_7BIT))
    {
        printf("ltc2992(2) does not exist!\r\n");
        setErrno(DEV_UNCONNECT);
        ok = 0;
    }
    if(ok == 0)
    {
        //return;
    }
     
    /*配置GPIO3位高阻态*/
    ltc2992WrByte(1, LTC2992_2_DEVID_7BIT, GPIO_CONTROL, 0x02);
    /*单次采集模式*/
//    ltc2992WrByte(0, LTC2992_1_DEVID_7BIT, CTRLA, 0x40);
//    ltc2992WrByte(1, LTC2992_2_DEVID_7BIT, CTRLA, 0x40);
    /*设置ADC位数为8位*/
    ltc2992WrByte(0, LTC2992_1_DEVID_7BIT, NADC, 0x80);
    ltc2992WrByte(1, LTC2992_2_DEVID_7BIT, NADC, 0x80);
    delay_ms(100);
}

/*
 *从寄存器中获取到原始的12位AD值
 *regist 可以填入 S1 S2 I1 I2 G1 G2 G3 G4 
*/
uint16_t ltc2992GetRaw(uint8_t ch, uint8_t regist)
{
    uint8_t temp[2];
    uint16_t ret = 0;
    if(ch == 0)
    {
        ltc2992RdNByte(0, LTC2992_1_DEVID_7BIT, regist, temp, 2);
    }
    else if(ch == 1)
    {
        ltc2992RdNByte(1, LTC2992_2_DEVID_7BIT, regist, temp, 2);
    }
    ret = temp[1] | (temp[0]<<8);
    ret = ret>>8;
    return ret;
}

void ltc2992SysPowerVoltage(float *sys24V, float *sys12V, float *sys5V, float *sys3V3) /*单位V*/
{
    uint16_t val;
    val = ltc2992GetRaw(1, G1);
    //*sys24V = ((float)val/256.f*2.048f)*(270.f/20.f);
    *sys24V = (float)val/9.259259f;

    val = ltc2992GetRaw(1, G3);
    //*sys12V = ((float)val/512.f*2.048f)*(270.f/20.f);
    *sys12V = (float)val/9.259259f;
    
    val = ltc2992GetRaw(1, G4);
    //*sys5V = ((float)val/256.f*2.048f)*(270.f/121.f);
    *sys5V = (float)val/56.018518f;

    val = ltc2992GetRaw(1, G2);
    //*sys3V3 = ((float)val/256.f*2.048f)*(270.f/121.f);
    *sys3V3 = (float)val/56.018518f;
}

void ltc2992MotorI(float *motor_I) /*单位ma*/
{
    uint16_t val;
    val = ltc2992GetRaw(1, I1);
    //*motor_I = ((float)val/256.f * 51.2f)*40;   
    *motor_I = (float)val*8.f;
}

void ltc2992ServoI(float *servo_I) /*单位ma*/
{
    uint16_t val;

    val = ltc2992GetRaw(1, I2);
    //*motor_I = ((float)val/256.f * 51.2)*40;   
    *servo_I = (float)val*8.f;
}

void ltc2992LinearDrvI(uint8_t ch, float *linearDrv_I) /*单位ma*/
{

    uint16_t val;

    if(ch == 0)
    {
        val = ltc2992GetRaw(0, I1);
    }
    else
    {
        val = ltc2992GetRaw(0, I2);
    }
    //*motor_I = ((float)val/256.f * 51.2)*40;   
    *linearDrv_I = (float)val*8;
}


