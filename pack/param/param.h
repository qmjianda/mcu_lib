#ifndef __PARAM_H_
#define __PARAM_H_

#include <stdio.h>
#include <stdint.h>

/*�����趨�����Ƿ��Ѿ����ڵı�־*/ 
#define USER_PARAM_FLASH_WRITE_FLAG (0xAA5766) 
/*���ڶ���Ĭ�ϲ���*/ 
#define PARAM_DEF(name, type, default)  {#name, type, ._data.type##_DATA=default}
/*���ò�����ֵ*/ 
#define PARAM_SET(id, val)				do{\
											if(id < PARAM_MAX)\
											{\
												switch(g_Param[id]._type)\
												{\
													case PARAM_U32:\
														g_Param[id]._data.PARAM_U32_DATA = val;\
													break;\
													case PARAM_V32:\
														g_Param[id]._data.PARAM_V32_DATA = val;\
													break;\
													case PARAM_F:\
														g_Param[id]._data.PARAM_F_DATA = val;\
													break;\
												}\
											}\
										}while(0);
/*ͨ���������ֻ�ȡ����IDֵ*/
#define PARAM_ID(name)				name##_ID
/*ͨ��idֵ��ȡ�������ַ�������*/ 
#define PARAM_NAME(id)				((uint32_t)id >= PARAM_MAX ? NULL : g_Param[id]._name)
/*ͨ��id�����ͻ�ȡ������ֵ*/ 
#define PARAM_VAL(id, type)			((uint32_t)(id) >= PARAM_MAX ? 0 : g_Param[(id)]._data.type##_DATA)
/*��ȡ�������ܸ���*/ 
#define PARAM_TOTAL()				(PARAM_MAX)

/*��ȡ����������*/
#define PARAM_TYPE(id)              ((uint32_t)(id) >= PARAM_MAX ? -1 : (g_Param[(id)]._type))
                                        
typedef enum
{
    PARAM_U32 = 0,
    PARAM_V32 = 1,
    PARAM_F   = 2,
}T_ParamType;

typedef union
{
    uint32_t PARAM_U32_DATA;
    int32_t  PARAM_V32_DATA;
    float    PARAM_F_DATA;
}T_ParamData;

typedef struct
{
    char	    _name[20];
    T_ParamType _type;
    T_ParamData _data;
}T_ParamNode;

typedef enum
{
	PARAM_ID(flag), 		/*���������һ�����Ŀ�������������Ƿ��Ѿ�����*/ 
	PARAM_ID(bspVersion),	
	PARAM_ID(floatTest),	
	PARAM_ID(intTest),		
	PARAM_ID(uintTest),	
	PARAM_MAX
}M_paramId;

extern T_ParamNode g_Param[PARAM_MAX];

void paramDefault(void);
void paramInit(void);
void paramStone(void);
void paramShow(void);
#endif /*__PARAM_H_*/
