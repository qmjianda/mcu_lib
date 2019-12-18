#ifndef __SOFT_TIMER_H_
#define __SOFT_TIMER_H_
#include <stdint.h>

/*控制softTimerShow是否使能*/
#define SOFT_TIMER_DEBUG

#define DEF_SOFT_TIMER(_name, _fun, _mode)  \
                                            T_SoftTimer _name = \
                                            {\
                                                .parent = NULL, \
                                                .timing = 0, \
                                                .nextTime = 0, \
                                                .name=#_name, \
                                                .fun=_fun,\
                                                .mode = _mode\
                                            }

enum softMode
{ 
	SOFT_TIMER_ONE_SHOT,
	SOFT_TIMER_LOOP
};

typedef void(*T_TimerFun)(void);

typedef struct __soft_timer
{
	struct __soft_timer *next;
	struct __soft_timer *pre;
	void *parent;
	T_TimerFun fun;
	uint32_t timing;
	uint32_t nextTime;
	enum softMode  mode;
#ifdef SOFT_TIMER_DEBUG
	char *name;
#endif
}T_SoftTimer;

typedef struct
{
	T_SoftTimer *head;
	uint32_t len;
}T_SoftTimerList;

void softTimerMainLoop(void);
void softTimerInit(T_SoftTimer* timer, char* name, T_TimerFun fun, enum softMode mode);
void softTimerStart(T_SoftTimer* timer, uint32_t timing);
void softTimerStop(T_SoftTimer* timer);
void softTimerShow(void);


#endif /*__SOFT_TIMER_H_*/

