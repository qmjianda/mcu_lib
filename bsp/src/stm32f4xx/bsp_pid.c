#include "bsp_pid.h"

PID_ID pidCreate(T_PID *pid, float kp, float ki, float kd, float limit, uint32_t sample_time)
{
    pid->kp = kp;
    
    #ifdef PID_TIME_ENABLE
//    pid->ki = ki*sample_time;
//    pid->kd = kd/sample_time;
    
    pid->ki = ki;
    pid->kd = kd;
    #else
    pid->ki = ki;
    pid->kd = kd;
    #endif
    
    pid->int_limit = limit;
    pid->err_int = 0.0;
    pid->err = 0.0;
    pid->err_old = 0.0;
    pid->old_ctr = 0.0;
    pid->sample_time = sample_time;
    pid->old_time = 0;

    return (int)pid;
}

float pidIncCalc(PID_ID pid, float setVal,float nowVal)
{
    T_PID *_pid = (T_PID *)pid;
    float err = setVal - nowVal;
    uint32_t now_ms = PID_GET_TIM_MS();
    if(PID_TIMEOUT(now_ms, _pid->old_time, _pid->sample_time))
    {
        _pid->old_ctr = 0;
        if(_pid->kp != 0)
        {
           _pid->old_ctr += _pid->kp*err;
        }
        
        if(_pid->ki != 0)
        {
            _pid->old_ctr -= _pid->ki*_pid->err;
        }
        
        if(_pid->kd != 0)
        {
           _pid->old_ctr += _pid->kd*_pid->err_old;
        }
        
        _pid->err_old = _pid->err;
        _pid->err = err;
        _pid->old_time = now_ms;
    }
    return _pid->old_ctr;
}

float pidLocCalc(PID_ID pid, float setVal,float nowVal)
{
    uint32_t now_ms;
    T_PID *_pid = (T_PID *)pid;
    _pid->err = setVal - nowVal;
    now_ms = PID_GET_TIM_MS();
    if(PID_TIMEOUT(now_ms, _pid->old_time, _pid->sample_time))
    {
        _pid->old_ctr = 0.0;
        if(_pid->err_int < _pid->int_limit)
        {
            _pid->err_int += _pid->ki*_pid->err;
        }
        
        if(_pid->kp != 0)
        {
           _pid->old_ctr = _pid->kp*_pid->err;
        }
        
        if(_pid->ki != 0)
        {
            _pid->old_ctr -= _pid->err_int;
        }
        
        if(_pid->kd != 0)
        {
           _pid->old_ctr += _pid->kd*(_pid->set_val - setVal); /*特殊的微分项*/
        }
        
        _pid->err_old = _pid->err;
        _pid->set_val = setVal;
        
        _pid->old_time = now_ms;
    }
    
    return _pid->old_ctr;
}

void pidSetParam(PID_ID pid, float kp, float ki, float kd, float limit, uint32_t sample_time)
{
    T_PID *_pid = (T_PID *)pid;
    #ifdef PID_TIME_ENABLE
    _pid->ki = ki*sample_time;
    _pid->kd = kd/sample_time;
    #else
    _pid->ki = ki;
    _pid->kd = kd;
    #endif
    _pid->int_limit = limit;  
    _pid->sample_time = sample_time;
}
