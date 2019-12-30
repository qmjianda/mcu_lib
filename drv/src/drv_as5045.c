/*********************************************************************************************************
*
*    ģ������ : AS5045ģ��
*    �ļ����� : bsp_as5045.c
*    ��    �� : V1.0
*    ��    �� : hqm
*    ��    �� : 2018-09-12
*    ˵    �� : ����������AS5045ģ�飬ͨѶ��ʽ����ģ��SSIЭ�飬��λ�궨��������궨
*
*********************************************************************************************************/

#define DEBUG_LOG_EN 1

#include "drv_as5045.h"
#include "debug.h"
#include "param.h"

typedef struct
{
    GPIO_TypeDef* GPIOx;
    uint16_t      Pinx;
}AS5045GpioType;

static const AS5045GpioType CSN[AS5045_CH_MAX_NUM]   = 
    {
        {A_AS5045_CSN_PORT,A_AS5045_CSN_PIN},
        {B_AS5045_CSN_PORT,B_AS5045_CSN_PIN},
        {C_AS5045_CSN_PORT,C_AS5045_CSN_PIN},
    };
static uint8_t AS5045_SPI_CH_Remap[AS5045_CH_MAX_NUM] = {0, 2, 3};


#define CSN_H(ch)       GPIO_SetBits(CSN[ch].GPIOx,CSN[ch].Pinx) 
#define CSN_L(ch)       GPIO_ResetBits(CSN[ch].GPIOx,CSN[ch].Pinx)

#define LIMIT_VAL        (55) /*�޷��˲�����*/
#define LIMIT_MAX_COUNNT (3)  /*�޷��˲��ӳٴ���*/

/**
*   �� �� ��: AS5045_GpioConfig
*   ����˵��: SPI��GPIO������      
*   ��    ��: ch/ͨ���ţ���ѡ0��1
*   �� �� ֵ: ��
**/
static void AS5045_GpioConfig(uint8_t ch)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
    RCC_AHB1PeriphClockCmd(getGpioClk(CSN[0].GPIOx), ENABLE);
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Fast_Speed;
    
    GPIO_InitStruct.GPIO_Pin = CSN[ch].Pinx;
    GPIO_Init(CSN[ch].GPIOx, &GPIO_InitStruct);
}


void ssiRead(uint8_t ch, uint8_t *Buffer, uint8_t bufLen)
{
    uint16_t i;
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }

    
    CSN_L(ch);
    for(i = 0; i<bufLen; i++)
    {
        Buffer[i] = spiRdWrData(AS5045_SPI_CH_Remap[ch], Dummy_Byte);
    }
    CSN_H(ch);
}


uint32_t AS5045GetRaw18bit_SSI(uint8_t ch)
{
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    uint32_t ret = 0;
    uint8_t buffer[3];
    CSN_L(ch);
    bsp_DelayUS(1);
    ssiRead(ch,buffer,3);
    CSN_H(ch);
    bsp_DelayUS(1);
    ret = buffer[2]>>5 | (uint16_t)buffer[1]<<3 | (uint32_t)buffer[0] << 11;
    ret &= ~(1<<18);
    return ret;
}



/*
*����1 ����������1
*����0 ����ż����1
*/
uint8_t AS5045ParityCheck(uint32_t val)
{
    uint8_t parity = 0;  //初始判断标记
    while (val)
    {
      parity = !parity;
      val = val & (val - 1);
    }
    return parity;
}

int AS5045GetRaw12BitNoOffSet(uint8_t ch)
{
    int ret = -1;
    T_As5045Msg *valP = (T_As5045Msg *)&ret;
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    ret = AS5045GetRaw18bit_SSI(ch);

    if(AS5045ParityCheck(ret) == 0) /*偶校�?*/
    {
        /*У��״̬λ D11-D0����Ϊ OCF(ƫ���1��Ч) COF(cordic��� 0����) LIN(���Զȱ���0����) MagInc(����ƫ������0�ޱ仯) MagDec(����ƫ�Ƽ�С0�ޱ仯)��MagInc��MagDec����ͬʱΪ1��*/
//      if( (valP->ocf==0) || (valP->cof==1) || (valP->lin==1) ||((valP->magDec && valP->magInc) == 1))*/
        if( (valP->ocf==0) || (valP->cof==1) || ((valP->magDec || valP->magInc) == 1)) 
        {
            ret = -1;
        }
        else
        {
            ret = ret>>6;
        }
    }
    else
    {
        ret = -1;
    }
    
    if(ret>-1)
    {
        if(ret<0)
        {
            ret = 4095+ret;
        }
    }
    return ret;
}


int AS5045GetRaw12Bit(uint8_t ch)
{
    int ret = AS5045GetRaw12BitNoOffSet(ch);
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    if(ret>-1)
    {
        ret = ret - g_Param.zeroPos[ch].data.u32Data;
        if(ret<0)
        {
            ret = 4095+ret;
        }
    }
    return ret;
}

/*
 *�Ľ����޷��˲�����
 */
int limitFilter(uint8_t ch ,int val)
{   
    static int oldVal[AS5045_CH_MAX_NUM] = {0};
    static uint16_t count[AS5045_CH_MAX_NUM] = {0};
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    if( ((oldVal[ch] - val)>=LIMIT_VAL) || ((val - oldVal[ch])>=LIMIT_VAL) ) /*����*/
    {
        if(count[ch]++ >= LIMIT_MAX_COUNNT)
        {
            count[ch] = 0;
            oldVal[ch] = val;
        }
    }
    else
    {
        count[ch] = 0;
        oldVal[ch] = val;
    }
    return oldVal[ch];
}

int averageFiter(int val)
{
    static int maxVal = 0, minVal = 0;
    static int ret = 0;
    static uint8_t count = 0;
    static int valArry[10] = {0};
    
    if(count==0)
    {
        valArry[count++] = val;
        maxVal = val;
        minVal = val;
    }
    else if(count<10)
    {
        valArry[count++] = val;
        if(val >= maxVal)
        {
            maxVal = val;
        }
        
        if(val<=minVal)
        {
            minVal = val;
        }
    }
    else
    {
        uint8_t i;
        count = 0;
        ret = 0;
        for(i=0; i<10; i++)
        {
            ret += valArry[i];
        }
        ret = ret-maxVal-minVal;
        ret = ret>>3;
    }
    return ret;
}

int AS5045GetRaw12BitWhithFiter(uint8_t ch)
{
    static int val[AS5045_CH_MAX_NUM] = {0};
    int temp = AS5045GetRaw12Bit(ch);
    int ret = 0;
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    if(temp != -1)
    {
        val[ch] = temp;
        ret = limitFilter(ch,val[ch]);
    }
    else
    {
        ret = -1;
    }
    return ret;
}

float AS5045_Raw12BitToAngle180(int val)
{
    float angle;
    angle = (float)val*360.f/4096.f;
    if(angle>180.f)
    {
        angle = angle-360.f;
    }
    return angle;
}

float AS5045_Raw12BitToAngle360(int val)
{
    float angle;
    angle = (float)val*360.f/4096.f;
    return angle;
}

uint16_t AS5045_AngleToRaw(float angle)
{
    if(angle<0.f)
    {
        angle += 360;
    }
    return (uint16_t)(angle/360.f*4096.f);
}

uint8_t AS5045_StatuCheck(uint8_t ch)
{
    uint32_t rawData = AS5045GetRaw18bit_SSI(ch);
    uint8_t ret = 0;
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return 0;
    }
    
    ret = rawData>>1;
    ret = ret & 0x03;
    
    T_As5045Msg *valP = (T_As5045Msg *)&rawData;
    
    if(ch==0)
    {
        PINFO("A_AS5045");
    }
    else if(ch == 1)
    {
        PINFO("B_AS5045");
    }
    else if(2)
    {
        PINFO("C_AS5045");
    }
    PINFO("RAW=%-10d VAL=%-8d OCF=%d COF=%d LIN=%d MAG_Inc:%d MAG_Dec:%d\r\n",rawData,rawData>>6,valP->ocf, valP->cof, valP->lin, valP->magInc, valP->magDec);
    return ret;
}

int AS5045ZeroProgSelf(uint8_t ch)
{
    int ret;
    
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return -1;
    }
    
    ret = AS5045GetRaw12BitNoOffSet(ch);
    if(ret>-1)
    {
        g_Param.zeroPos[ch].data.u32Data = ret;
    }
    return ret;
}

void AS5045Init(uint8_t ch)
{
    if(!IS_AS5045_CH(ch))
    {
        setErrno(NON_EXIST);
        return;
    }
    AS5045_GpioConfig(ch);
    CSN_H(ch);
    spiInit(AS5045_SPI_CH_Remap[ch]);
    bsp_DelayMS(100); /*����ģʽ�ϵ���Ҫ20ms������ģʽ�ϵ���Ҫ80ms*/
}


