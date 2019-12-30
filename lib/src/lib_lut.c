#include "lib_lut.h"


void lutInit(T_LutTable* table, T_LutVal *vals, int length)
{
    int i;
    if(table && vals && length)
    {
        table->valTabP = vals;
        table->length = length;
        table->maxRaw = vals[0].raw;
        table->minRaw = vals[0].raw;
        table->maxVal = vals[0].val;
        table->minVal = vals[0].val;
        
        
        for(i=0; i<length-1; i++)
        {
            if(vals[i+1].raw == vals[i].raw)
            {
                vals[i].k = 1;
            }
            else
            {
                vals[i].k = (float)(vals[i+1].val - vals[i].val)/(float)(vals[i+1].raw - vals[i].raw);
            }
            
            if(table->maxRaw < vals[i].raw)
            {
                table->maxRaw = vals[i].raw;
            }

            if(table->minRaw > vals[i].raw)
            {
                table->minRaw = vals[i].raw;
            }            

            if(table->maxVal < vals[i].val)
            {
                table->maxVal = vals[i].val;
            }

            if(table->minVal > vals[i].val)
            {
                table->minVal = vals[i].val;
            }                
            
        }
        vals[i].k = vals[i-1].k;
    }
}

float lutRawToVal(T_LutTable* table, int raw)
{
    int i;
    int len = table->length-1;
    float ret = 0.f;

    /*查表*/
    if(table && (table->length>1))
    {
        for(i=0; i<len; i++)
        {
            if( ((raw < table->valTabP[i].raw)&&(raw < table->valTabP[i+1].raw)) || /*小于两点*/
                ((raw > table->valTabP[i].raw)&&(raw > table->valTabP[i+1].raw)) )  /*大于两点*/
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    /*计算*/
    ret = (float)(raw - table->valTabP[i].raw) * table->valTabP[i].k + table->valTabP[i].val;
    
    return ret;
}

int lutValToRaw(T_LutTable* table, float val)
{
    int i;
    int len = table->length-1;
    int ret = 0;

    /*查表*/
    if(table && (table->length>1))
    {
        for(i=0; i<len; i++)
        {
            if( ((val < table->valTabP[i].val)&&(val < table->valTabP[i+1].val)) || /*小于两点*/
                ((val > table->valTabP[i].val)&&(val > table->valTabP[i+1].val)) )  /*大于两点*/
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    /*计算*/
    ret = (float)(val - table->valTabP[i].val) / table->valTabP[i].k + table->valTabP[i].raw;
    
    return ret;
}
