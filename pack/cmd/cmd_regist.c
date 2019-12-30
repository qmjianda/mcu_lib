#include "cmd_core.h"

/*
 * DECLARE_CMD(name	, fun , help );
 * name: cmd 的名字，在串口输入相应的名字将运行对应的函数 
 * fun : cmd 对应的函数
 * help:帮助字符串，使用help指令或者使用“命令 ？ 或 命令 --help” 将打印本字符串
 * help格式：描述<参数>
 * 其中参数描述为 {选项:最小取值~最大取值}| {选项:取值1|取值2|取值3|}...... 
 */

DECLARE_CMD(clean, cmd_clean, "clear screen" );
DECLARE_CMD(help, cmd_help, "show all cmds");
DECLARE_CMD(example, cmd_example, "a example {str:any}|{int:any int num}|{float:any float num}");
DECLARE_CMD(param, cmd_param, "param ctrl {show}|{save}|{config:no&val}|{default}");
DECLARE_CMD(reboot, cmd_reboot, "reboot system");
DECLARE_CMD(softtimer, cmd_soft_timer, "soft timer test {start|stop}");
T_Cmd *g_cmdArry[] = 
{
    REGIST_CMD(clean),
    REGIST_CMD(help),
    REGIST_CMD(example),
    REGIST_CMD(param),
    REGIST_CMD(reboot),
    REGIST_CMD(softtimer),
    NULL,
};
