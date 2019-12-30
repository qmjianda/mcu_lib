#include "cmd_core.h"
#include "stm32f10x.h"

void cmd_reboot(int argc, char* argv[])
{
	NVIC_SystemReset();
}
