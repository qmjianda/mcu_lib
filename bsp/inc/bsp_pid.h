#ifndef __BSP_PID_H_
#define __BSP_PID_H_

#include "include.h"

#define PID_TIME_ENABLE

#ifdef  PID_TIME_ENABLE
#define PID_GET_TIM_MS()                            millins()
#define PID_TIMEOUT(now_ms, old_ms, sample_time)    ((now_ms-old_ms)>sample_time)
#else
#define PID_GET_TIM_MS()                            (1)
#define PID_TIMEOUT(now_ms, old_ms, sample_time)    (1)
#endif

typedef struct
{
    float kp;
    float ki;
    float kd;
    
    float err;
    float err_old;
    float err_int;      /*积分项*/
    float int_limit;    /*积分限幅*/
    float set_val;      /*设置量*/
    float old_ctr;
    uint32_t sample_time; /*计算频率*/
    uint32_t old_time;  /*上次计算时间*/
}T_PID;

typedef int PID_ID;

PID_ID pidCreate(T_PID *pid, float kp, float ki, float kd, float limit, uint32_t sample_time);
void pidSetParam(PID_ID pid, float kp, float ki, float kd, float limit, uint32_t sample_time);
float pidIncCalc(PID_ID pid, float setVal,float nowVal);
float pidLocCalc(PID_ID pid, float setVal,float nowVal);

#endif /*__BSP_PID_H_*/
