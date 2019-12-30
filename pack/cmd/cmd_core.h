#ifndef __CMD_CORE_H_
#define __CMD_CORE_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define DECLARE_CMD(name, fun, help)    extern void fun(int argv, char* argc[]);\
							 	   	    T_Cmd cmd_struct_##name = { #name, help, fun };
							 	   
#define REGIST_CMD(name) 			&cmd_struct_##name

typedef void(*T_CmdFun)(int argv, char* argc[]);

typedef struct
{
    char* name;
    char* help;
    T_CmdFun cmdFun;
}T_Cmd;

uint8_t cmdParamCheck(int need, int argc, char*argv[]);
int cmdGetParam(char* msg, char*delim, char* get[], int maxNum);
T_Cmd *cmdFindCmd(T_Cmd* cmdArry[], int argc, char*argv[]);

#endif /*__CMD_CORE_H_*/
