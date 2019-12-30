#include "cmd_core.h"
#include "param.h"
#include <string.h>

void cmd_param(int argc, char* argv[])
{
    if(cmdParamCheck(1, argc, argv))
    {
        if(strcmp(argv[1], "show") == 0)
        {
            paramShow();
        }
        else if(strcmp(argv[1], "config") == 0)
        {
            if(cmdParamCheck(3, argc, argv))
            {
                uint32_t val_u32;
                int32_t  val_v32;
                float    val_f;
                uint16_t no;
                
                no = atoi(argv[2]);
                if(PARAM_TYPE(no) == PARAM_U32)
                {
                    val_u32 = atoll(argv[3]);
                    PARAM_SET(no, val_u32);
                }
                else if(PARAM_TYPE(no) == PARAM_V32)
                {
                    val_v32 = atoi(argv[3]);
                    PARAM_SET(no, val_v32);
                }
                else if(PARAM_TYPE(no) == PARAM_F)
                {
                    val_f = atof(argv[3]);
                    PARAM_SET(no, val_f);
                }     
            }
        }
        else if(strcmp(argv[1], "save") == 0)
        {
            paramStone();
        }
        else if(strcmp(argv[1], "default") == 0)
        {
            paramDefault();
        }
        else
        {
            printf("please enter param.save or param.show or param.config param.default\r\n");
        }
    }
}
