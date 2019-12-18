#include "param.h"
#include <string.h>

/**********************��Ҫ��param_port.c��ʵ�����������������*******************************/
void paramPortWriteData(uint8_t* data, uint16_t len); /*������д�뵽�洢�豸��*/ 
uint16_t paramPortReadData(uint8_t* data, uint16_t len); /*�Ӵ洢�豸�ж�ȡ����*/ 
void paramPortInit(void); /*�洢�豸��һЩ��ʼ������*/
/**********************************************************************************************/

/*Ĭ�ϲ����б�*/ 
T_ParamNode g_DefaultParam[PARAM_MAX] = 
{
	PARAM_DEF(flag      , PARAM_U32, USER_PARAM_FLASH_WRITE_FLAG),
	PARAM_DEF(bspVersion, PARAM_U32, 1),
	PARAM_DEF(floatTest , PARAM_F  , 0.123456),
	PARAM_DEF(intTest   , PARAM_V32, -1 ),
	PARAM_DEF(uintTest  , PARAM_U32, 1234),
};


/*��������ʱ��ǰ����ʹ�õĲ����б�*/ 
T_ParamNode g_Param[PARAM_MAX] = {0}; 


/*
 * �ָ�����Ĭ��ֵ 
 */ 
void paramDefault(void)
{
	memcpy(g_Param, g_DefaultParam, sizeof(g_Param));
}

/*
 * �洢�������洢�豸�� 
 */ 
void paramStone(void)
{
    paramPortWriteData( (uint8_t*)g_Param, sizeof(g_Param) );
}

/*
 * ��ʽ����ʾ���еĲ��� 
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
 * ��ʼ��������1.��ʼ���洢�豸�ӿ� 2.�Ӵ洢�豸��ȡ���� 3.�ж��Ƿ���Ҫ����Ĭ�ϲ��� 
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
