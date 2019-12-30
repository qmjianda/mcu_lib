#include "soft_timer.h"
#include <stdio.h>

static T_SoftTimerList timerList[2] = {0};
static T_SoftTimerList *currentList = &timerList[0];
static T_SoftTimerList *overflowList = &timerList[1];
static uint32_t        systemTime;

/**
  * @brief  将软件定时器插入到定时器链表中，按照时间从小到大排列
  * @param  list定时器列表
  * @param  timer定时器
  * @retval None
  */
static void listInsertMax(T_SoftTimerList *list, T_SoftTimer* timer)
{
	uint32_t i;
	T_SoftTimer* node = list->head;
    
	if(list->len == 0) /*链表空*/
	{
		list->head = timer;
		list->head->next = list->head;
		list->head->pre = list->head;
		list->len++;
	}
	else
	{
        /*找到插入位置*/
		for(i=1; i<list->len; i++)
		{
			if(timer->nextTime < node->nextTime)
			{
				break;
			}
			node = node->next;
		}
        
		if( i== list->len) /*尾插*/
		{
			timer->next = node->next;
			node->next->pre = timer;
			
			timer->pre = node;
			node->next = timer;
		}
		else
		{
			if(i == 0) /*如果插入在头节点，需要将链表头重新指向新的首元素*/
			{
				list->head = timer;
			}
			
			timer->pre = node->pre;
			node->pre->next = timer;
			
			timer->next = node;
			node->pre = timer;
		}
		list->len++;
	}
	timer->parent = list;
}

/**
  * @brief  将软件定时器从链表中去除
  * @param  timer定时器
  * @retval None
  */
static void listRemove(T_SoftTimer* timer) 
{
	T_SoftTimerList *list = NULL;
    
	if(timer->parent) /*确认定时器是否已经属于某个链表*/
	{
		list = (T_SoftTimerList *)timer->parent;
		if(list->len == 1) /*链表只有一个元素时，可以直接将链表头节点赋为NULL*/
		{
			list->head = NULL; 
		}
		else
		{
			if(timer == list->head) /*去除头节点时，需要重新将链表表头指向下一个元素*/
			{
				list->head = timer->next;
			}
			timer->pre->next = timer->next;
			timer->next->pre = timer->pre;
		}
		timer->parent = NULL;
		list->len--;
	}
}

/**
  * @brief  将定时器链表中的头结点去除掉
  * @param  list定时器链表
  * @retval None
  */
static void listPop( T_SoftTimerList *list )
{
	T_SoftTimer *timer = list->head;
	if(list->len == 1)
	{
		list->head = NULL;
	}
	else if(list->len > 1)
	{
		list->head = list->head->next;
		timer->pre->next = timer->next;
		timer->next->pre = timer->pre;
	}
    
	list->len--;
	timer->parent = NULL;
}

/**
  * @brief  初始化软件定时器，定时器start之前必须要先初始化
  * @param  timer定时器
  * @param  name定时器名字
  * @param  fun定时器回调函数
  * @param  mode定时器模式
  *     @arg SOFT_TIMER_ONE_SHOT单次模式
  *     @arg SOFT_TIMER_LOOP循环模式
  * @retval None
  */
void softTimerInit(T_SoftTimer* timer, char* name, T_TimerFun fun, enum softMode mode)
{
	if(timer)
	{
		/*初始化，标记定时器可以加入到定时器队列*/
		timer->parent = NULL;
		
		timer->timing = 0;
		timer->nextTime = 0;
		
		#ifdef SOFT_TIMER_DEBUG
		timer->name = name;
		#endif
		timer->fun = fun;
		timer->mode = mode;
	}
}

/**
  * @brief  显示当前所有的定时器
  *         以pre<-name->next格式显示
  * @retval None
  */
void softTimerShow(void)
{
    #ifdef SOFT_TIMER_DEBUG
	T_SoftTimer* node = currentList->head;
	uint32_t i;
    printf("current timer list:\r\n");
	for(i=0; i<currentList->len; i++)
	{
		printf("%s<-%s->%s\r\n", node->pre->name, node->name, node->next->name);
		node = node->next;
	}
    
    printf("overflow timer list:\r\n");
    node = overflowList->head;
	for(i=0; i<overflowList->len; i++)
	{
		printf("%s<-%s->%s\r\n", node->pre->name, node->name, node->next->name);
		node = node->next;
	}
    
    printf("\r\n");
    #endif
}

/**
  * @brief  启动定时器
  * @param  timer定时器
  * @param  timing定时时间，ms为单位
  * @retval None
  */
void softTimerStart(T_SoftTimer* timer, uint32_t timing)
{
	if( timer )
	{
		uint32_t newTime = systemTime + timing;
        
        listRemove(timer);
        
		timer->nextTime = newTime;
		timer->timing = timing;
        
        if(newTime >= systemTime) /*溢出*/
        {
            listInsertMax(currentList, timer);
        }
        else
        {
            listInsertMax(overflowList, timer);
        }
	}
}

/**
  * @brief  停止定时器
  * @param  timer定时器
  * @retval None
  */
void softTimerStop(T_SoftTimer* timer)
{
	if( timer )
	{
		listRemove(timer);
	}
}

/**
  * @brief  软件定时器主循环，需要保证此函数每1ms运行一次，通常在1ms的硬件定时器中断函数中调用它
  * @retval None
  */
void softTimerMainLoop(void)
{
    T_SoftTimer* timer;
	systemTime++;
	if(systemTime == 0) /*溢出*/
	{
		T_SoftTimerList *temp;
		temp = currentList;
		currentList = overflowList;
		overflowList = temp;
	}
	

	do{
		timer = currentList->head;
		if(timer)
		{
			if(systemTime >= timer->nextTime)
			{
				if(timer->mode == SOFT_TIMER_LOOP)
				{
					softTimerStart(timer, timer->timing);
				}
				else
				{
					listPop(currentList);
				}
				if(timer->fun)
				{
					timer->fun();
				}
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}while(1);
}
