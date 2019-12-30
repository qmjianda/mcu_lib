#include "bsp_iwdg.h"

/*********************************************************************************************************
*
*    ģ������ : ���Ź�ģ��
*    �ļ����� : bsp_iwdg.c
*    ��    �� : V1.0
*    ��    �� : zhangkeyan
*    ��    �� : 2017-07-05
*    ˵    �� : ���ö������Ź����ܣ��������Ź�ʹ��LSIʹ��32KHz
*
*********************************************************************************************************/

/*
 *  ����˵��: �������Ź�ģ���ʼ��
 *  ��    ��: prer �� IWDG��Ƶֵ ����ɲ鿴�ֲ����
 *            rlr  �� ����װ�ؼ���ֵ
 *  �� �� ֵ: None
 **/
void idwgInit(u8 pre,u16 rlr) 
{
    /*
       ��Ƶֵ  ֵ        ��Сʱ��(ms)    ���ʱ��(ms) 
        /4     0            0.125           512
        /8     1            0.25            1024
        /16    2            0.5             2048
        /32    3            1               4096
        /64    4            2               8192
        /128   5            4               16384
        /256   6            8               32768
    
    /64    125*1.6 = 200ms
    */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  /*ʹ�ܶԼĴ���IWDG_PR��IWDG_RLR��д����*/
    IWDG_SetPrescaler(pre);          /*����IWDGԤ��Ƶֵ:����IWDGԤ��ƵֵΪ  64   IWDG_Prescaler_64*/
    IWDG_SetReload(rlr);              /*����IWDG��װ��ֵ*/
    IWDG_ReloadCounter();             /*����IWDG��װ�ؼĴ�����ֵ��װ��IWDG������*/
    IWDG_Enable();                    /*ʹ��IWDG*/
}


/*
 *  ����˵��: �������Ź�ι������
 *  ��    ��: None
 *  �� �� ֵ: None
 **/
void iwdgFeed(void)
{   
    IWDG_ReloadCounter();
}

uint8_t isIwdgReset(void)
{
    if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET )
    {
        return 1;
    }
    return 0;
}

void clearIwdgResetFlag(void)
{
    RCC_ClearFlag();
}
/************************************************** DIHEELEC (END OF FILE) *******************************************************/
