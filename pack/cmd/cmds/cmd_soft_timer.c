#include "cmd_core.h"
#include "stm32f10x.h"
#include "soft_timer.h"
#include "hrt.h"



static void softTimerTest(void)
{
    printf("soft timer:%ums\r\n", hrtAbsoluteTimeMs());
}

static DEF_SOFT_TIMER(timer, softTimerTest, SOFT_TIMER_LOOP);

void cmd_soft_timer(int argc, char* argv[])
{
    if(cmdParamCheck(1, argc, argv))
    {
        if(strcmp(argv[1], "start") == 0)
        {
            softTimerStart(&timer, 1000);
        }
        else if(strcmp(argv[1], "stop") == 0)
        {
            softTimerStop(&timer);
        }
    }
}
