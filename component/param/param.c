#include "param.h"
#include <string.h>

/**********************需要在param_port.c中实现下面这个三个函数*******************************/
void paramPortWriteData(uint8_t* data, uint16_t len); /*将数据写入到存储设备中*/ 
uint16_t paramPortReadData(uint8_t* data, uint16_t len); /*从存储设备中读取数据*/ 
void paramPortInit(void); /*存储设备的一些初始化工作*/
/**********************************************************************************************/

/*默认参数列表*/ 
T_ParamNode g_DefaultParam[PARAM_MAX] = 
{
	PARAM_DEF(flag      , PARAM_U32, USER_PARAM_FLASH_WRITE_FLAG),
	PARAM_DEF(bspVersion, PARAM_U32, 1),
	PARAM_DEF(floatTest , PARAM_F  , 0.123456),
	PARAM_DEF(intTest   , PARAM_V32, -1 ),
	PARAM_DEF(uintTest  , PARAM_U32, 1234),
};


/*程序运行时当前正在使用的参数列表*/ 
T_ParamNode g_Param[PARAM_MAX] = {0}; 


/*
 * 恢复参数默认值 
 */ 
void paramDefault(void)
{
	memcpy(g_Param, g_DefaultParam, sizeof(g_Param));
}

/*
 * 存储参数到存储设备中 
 */ 
void paramStone(void)
{
    paramPortWriteData( (uint8_t*)g_Param, sizeof(g_Param) );
}

/*
 * 格式化显示所有的参数 
 */
void paramShow(void)
{
    uint16_t i;
	for(i=0; i<PARAM_MAX; i++)
	{
		printf("[%-2d] %-20s=", i,PARAM_NAME(i));
		switch(g_Param[i]._type)
		{
			case PARAM_U32:
				printf("%-10d [0x%-10X](uint32_t)\r\n", PARAM_VAL(i, PARAM_U32), PARAM_VAL(i, PARAM_U32));
				break;
			case PARAM_V32:
				printf("%-10d [0x%-10X](int32_t)\r\n", PARAM_VAL(i, PARAM_U32), PARAM_VAL(i, PARAM_V32));
				break;
			case PARAM_F:
				printf("%-10f [0x%-10X](float)\r\n", PARAM_VAL(i, PARAM_F), 0);
				break;
		}
	}
}

/*
 * 初始化工作：1.初始化存储设备接口 2.从存储设备读取参数 3.判断是否需要加载默认参数 
 */ 
void paramInit(void)
{
	paramPortInit();
    paramDefault();
	paramPortReadData( (uint8_t*)&g_Param, sizeof(g_Param) );
    
    if(PARAM_VAL(0, PARAM_U32) != USER_PARAM_FLASH_WRITE_FLAG)
    {
        printf("param not exit ,old flag:0x%X new:0x%X!!\r\n", PARAM_VAL(0, PARAM_U32), USER_PARAM_FLASH_WRITE_FLAG);
        paramDefault();
        paramStone();
    }
}
