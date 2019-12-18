#include "cmd_core.h"

/*
 * DECLARE_CMD(name	, fun , help );
 * name: cmd �����֣��ڴ���������Ӧ�����ֽ����ж�Ӧ�ĺ��� 
 * fun : cmd ��Ӧ�ĺ���
 * help:�����ַ�����ʹ��helpָ�����ʹ�á����� �� �� ���� --help�� ����ӡ���ַ���
 * help��ʽ������<����>
 * ���в�������Ϊ {ѡ��:��Сȡֵ~���ȡֵ}| {ѡ��:ȡֵ1|ȡֵ2|ȡֵ3|}...... 
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
