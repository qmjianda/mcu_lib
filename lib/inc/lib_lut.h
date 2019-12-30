#ifndef __BSP_LUT_H_
#define __BSP_LUT_H_
#include "include.h"
/*���ļ�����ʵ�ֲ��, ע����RAWֵ��Ҫ��С��������*/

typedef struct
{
    int      raw; /*ԭʼֵ*/
    float    val; /*��Ӧֵ*/
    float    k;   /*б��, ��ʼ����ʱ����Զ�����*/
}T_LutVal; /*look up table*/

typedef struct
{
    T_LutVal *valTabP; /*������*/
    int maxRaw;
    int minRaw;
    float maxVal;
    float minVal;
    int length;        /*��Ĵ�С*/ 
}T_LutTable; /*look up table*/

void lutInit(T_LutTable* table, T_LutVal *vals, int length);
float lutRawToVal(T_LutTable* table, int raw);
int lutValToRaw(T_LutTable* table, float val);

#endif /*__BSP_LUT_H_*/

