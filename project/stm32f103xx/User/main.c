#include <string.h>
#include "stm32f10x.h"
#include "delay.h"
#include "uart.h"
#include "cmd.h"
#include "param.h"
#include "hrt.h"
#include "soft_timer.h"

int main(void)
{
    SCB->VTOR = FLASH_BASE | 0x2000; /*中断向量表偏移量设置*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    hrtTimerInit();
    regSysTickCallBackFun(softTimerMainLoop);
    delay_init(72);
    uartInit(115200);
    paramInit();
    
    printf("hello world\r\n");
    
    while(1)
    {
        cmdTask();
        delay_ms(1);
    }
}
