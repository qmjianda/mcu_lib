#include "cmd_core.h"

uint8_t cmdParamCheck(int need, int argc, char*argv[])
{
    uint8_t i,ret = 0;
    if(need<(argc))
    {
        ret = 1;
    }
    else
    {
        printf("err need %d but input %d:\r\n", need, argc-1);
    	printf("input= ");
	    for(i=0; i<argc; i++)
	    {
	        if(argv[i] != NULL)
	        {
	            printf("%s ", argv[i]);
	        }
	    }
	    printf("\r\n");
        ret = 0;
    }
    return ret;
}

int cmdGetParam(char* msg, char*delim, char* get[], int maxNum)
{
	int i,ret;
	char *ptr = NULL;
	ptr = strtok(msg, delim);
	for(i=0; ptr!=NULL &&i<maxNum; i++)
	{
		get[i] = ptr;
		ptr = strtok(NULL, delim);
	}
	ret = i;
	return ret;
}

T_Cmd *cmdFindCmd(T_Cmd* cmdArry[], int argc, char*argv[])
{
	int i;
	for(i = 0; cmdArry[i]; i++)
	{
		if(strcmp(cmdArry[i]->name, argv[0]) == 0)
		{
			return cmdArry[i];
		}
	}
	return NULL;
}
