#include "soft_timer.h"
#include <stdio.h>

static T_SoftTimerList timerList[2] = {0};
static T_SoftTimerList *currentList = &timerList[0];
static T_SoftTimerList *overflowList = &timerList[1];
static uint32_t        systemTime;

/**
  * @brief  �������ʱ�����뵽��ʱ�������У�����ʱ���С��������
  * @param  list��ʱ���б�
  * @param  timer��ʱ��
  * @retval None
  */
static void listInsertMax(T_SoftTimerList *list, T_SoftTimer* timer)
{
	uint32_t i;
	T_SoftTimer* node = list->head;
    
	if(list->len == 0) /*�����*/
	{
		list->head = timer;
		list->head->next = list->head;
		list->head->pre = list->head;
		list->len++;
	}
	else
	{
        /*�ҵ�����λ��*/
		for(i=1; i<list->len; i++)
		{
			if(timer->nextTime < node->nextTime)
			{
				break;
			}
			node = node->next;
		}
        
		if( i== list->len) /*β��*/
		{
			timer->next = node->next;
			node->next->pre = timer;
			
			timer->pre = node;
			node->next = timer;
		}
		else
		{
			if(i == 0) /*���������ͷ�ڵ㣬��Ҫ������ͷ����ָ���µ���Ԫ��*/
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
  * @brief  �������ʱ����������ȥ��
  * @param  timer��ʱ��
  * @retval None
  */
static void listRemove(T_SoftTimer* timer) 
{
	T_SoftTimerList *list = NULL;
    
	if(timer->parent) /*ȷ�϶�ʱ���Ƿ��Ѿ�����ĳ������*/
	{
		list = (T_SoftTimerList *)timer->parent;
		if(list->len == 1) /*����ֻ��һ��Ԫ��ʱ������ֱ�ӽ�����ͷ�ڵ㸳ΪNULL*/
		{
			list->head = NULL; 
		}
		else
		{
			if(timer == list->head) /*ȥ��ͷ�ڵ�ʱ����Ҫ���½������ͷָ����һ��Ԫ��*/
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
  * @brief  ����ʱ�������е�ͷ���ȥ����
  * @param  list��ʱ������
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
  * @brief  ��ʼ�������ʱ������ʱ��start֮ǰ����Ҫ�ȳ�ʼ��
  * @param  timer��ʱ��
  * @param  name��ʱ������
  * @param  fun��ʱ���ص�����
  * @param  mode��ʱ��ģʽ
  *     @arg SOFT_TIMER_ONE_SHOT����ģʽ
  *     @arg SOFT_TIMER_LOOPѭ��ģʽ
  * @retval None
  */
void softTimerInit(T_SoftTimer* timer, char* name, T_TimerFun fun, enum softMode mode)
{
	if(timer)
	{
		/*��ʼ������Ƕ�ʱ�����Լ��뵽��ʱ������*/
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
  * @brief  ��ʾ��ǰ���еĶ�ʱ��
  *         ��pre<-name->next��ʽ��ʾ
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
  * @brief  ������ʱ��
  * @param  timer��ʱ��
  * @param  timing��ʱʱ�䣬msΪ��λ
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
        
        if(newTime >= systemTime) /*���*/
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
  * @brief  ֹͣ��ʱ��
  * @param  timer��ʱ��
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
  * @brief  �����ʱ����ѭ������Ҫ��֤�˺���ÿ1ms����һ�Σ�ͨ����1ms��Ӳ����ʱ���жϺ����е�����
  * @retval None
  */
void softTimerMainLoop(void)
{
    T_SoftTimer* timer;
	systemTime++;
	if(systemTime == 0) /*���*/
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
