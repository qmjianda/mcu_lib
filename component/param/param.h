#ifndef __PARAM_H_
#define __PARAM_H_

#include <stdio.h>
#include <stdint.h>

/*用于设定参数是否已经村在的标志*/ 
#define USER_PARAM_FLASH_WRITE_FLAG (0xAA5766) 
/*用于定义默认参数*/ 
#define PARAM_DEF(name, type, default)  {#name, type, ._data.type##_DATA=default}
/*设置参数的值*/ 
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
/*通过参数名字获取参数ID值*/
#define PARAM_ID(name)				name##_ID
/*通过id值获取参数的字符串名字*/ 
#define PARAM_NAME(id)				((uint32_t)id >= PARAM_MAX ? NULL : g_Param[id]._name)
/*通过id和类型获取参数的值*/ 
#define PARAM_VAL(id, type)			((uint32_t)(id) >= PARAM_MAX ? 0 : g_Param[(id)]._data.type##_DATA)
/*获取参数的总个数*/ 
#define PARAM_TOTAL()				(PARAM_MAX)

/*获取参数的类型*/
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
	PARAM_ID(flag), 		/*必须存在这一项，此项目是用来检测参数是否已经存在*/ 
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
