#include "cmd.h"

#define CMD_BUFFER_MAX_LEN  (50) /*命令最长的字符长度*/
#define CMD_PARAM_MAX_NUM   (6)  /*命令最多支持的参数*/
char   cmdBuff[CMD_BUFFER_MAX_LEN] = {0}; /*命令缓存区*/
int    paramNum = 0;
char   *cmdParamBuff[CMD_PARAM_MAX_NUM] = {NULL};
extern T_Cmd *g_cmdArry[];

uint8_t cmdGetLine(uint8_t byte, char* buff, uint16_t buffLen)
{
    uint8_t ret = 0;
    static uint16_t count = 0;
    buff[count] = byte;
    switch(byte)
    {
        case '\r':
            break;
        case '\n':
            buff[count] = '\0';
            count = 0;
            printf("%s", "\r\n");
            ret = 1;
        case 0x08: /*退格*/
        case 0x7F: /*删除键*/
            if(count>0)
            {
                printf("%c%c%c",  0x08, ' ',0x08);
                count--;
            }
            break;
        default:
        {
            count++;
            if(count>=buffLen)
            {
                buff[count-1] = '\0';
                count = 0;
                ret = 1;
            }
            printf("%c", byte);
        }
        break;
    }
    return ret;
}

void cmdRunCmd(T_Cmd* cmds[], int argc, char *argv[])
{
	T_Cmd *pCmd = NULL;
	pCmd = cmdFindCmd(cmds, argc, argv);
    if(pCmd == NULL)
    {
        printf("cmd \"%s\" does not exist\r\n", cmdParamBuff[0]);
    }
	else
	{
		if(argc == 2)
		{
			if( !(strcmp(cmdParamBuff[1],"?") && strcmp(cmdParamBuff[1],"--help")) )
			{
				printf("%s\r\n", pCmd->help);
			}
			else
			{
				pCmd->cmdFun(argc, cmdParamBuff);
			}
		}
		else
		{
			pCmd->cmdFun(argc, cmdParamBuff);
		}
	}
    printf("\r\n");
}

void cmdGet(uint8_t byte)
{
    if(cmdGetLine(byte, cmdBuff, CMD_BUFFER_MAX_LEN))
    {
        paramNum = cmdGetParam(cmdBuff, "., ", cmdParamBuff, CMD_PARAM_MAX_NUM);
        if(paramNum >0)
        {
			cmdRunCmd(g_cmdArry, paramNum, cmdParamBuff);
        }
        printf("->");
    }
}

void cmdTask(void)
{
	uint8_t c;
	if(GET_CHAR(&c))
	{
		cmdGet(c);
	}
}

/*******************************************************************************************/





