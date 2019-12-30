#ifndef __BSP_LUT_H_
#define __BSP_LUT_H_
#include "include.h"
/*本文件用于实现查表法, 注意表格RAW值需要从小到大排列*/

typedef struct
{
    int      raw; /*原始值*/
    float    val; /*对应值*/
    float    k;   /*斜率, 初始化的时候会自动生成*/
}T_LutVal; /*look up table*/

typedef struct
{
    T_LutVal *valTabP; /*表内容*/
    int maxRaw;
    int minRaw;
    float maxVal;
    float minVal;
    int length;        /*表的大小*/ 
}T_LutTable; /*look up table*/

void lutInit(T_LutTable* table, T_LutVal *vals, int length);
float lutRawToVal(T_LutTable* table, int raw);
int lutValToRaw(T_LutTable* table, float val);

#endif /*__BSP_LUT_H_*/

