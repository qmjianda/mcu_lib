#include "bsp_iwdg.h"

/*********************************************************************************************************
*
*    模块名称 : 看门狗模块
*    文件名称 : bsp_iwdg.c
*    版    本 : V1.0
*    作    者 : zhangkeyan
*    日    期 : 2017-07-05
*    说    明 : 配置独立看门狗功能，独立看门狗使用LSI使用32KHz
*
*********************************************************************************************************/

/*
 *  功能说明: 独立看门狗模块初始化
 *  形    参: prer ： IWDG分频值 具体可查看手册解释
 *            rlr  ： 重新装载计数值
 *  返 回 值: None
 **/
void idwgInit(u8 pre,u16 rlr) 
{
    /*
       分频值  值        最小时间(ms)    最大时间(ms) 
        /4     0            0.125           512
        /8     1            0.25            1024
        /16    2            0.5             2048
        /32    3            1               4096
        /64    4            2               8192
        /128   5            4               16384
        /256   6            8               32768
    
    /64    125*1.6 = 200ms
    */
    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);  /*使能对寄存器IWDG_PR和IWDG_RLR的写操作*/
    IWDG_SetPrescaler(pre);          /*设置IWDG预分频值:设置IWDG预分频值为  64   IWDG_Prescaler_64*/
    IWDG_SetReload(rlr);              /*设置IWDG重装载值*/
    IWDG_ReloadCounter();             /*按照IWDG重装载寄存器的值重装载IWDG计数器*/
    IWDG_Enable();                    /*使能IWDG*/
}


/*
 *  功能说明: 独立看门狗喂狗操作
 *  形    参: None
 *  返 回 值: None
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
