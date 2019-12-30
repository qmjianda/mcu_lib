#include "cmd_core.h"

void cmd_help(int argc, char* argv[])
{
	extern T_Cmd *g_cmdArry[];
    uint8_t i;
    for(i = 0; g_cmdArry[i]; i++)
    {
        printf("%-10s: %s\r\n", g_cmdArry[i]->name, g_cmdArry[i]->help);
    }
}
