#include "param.h"
#include "flash.h"
#include "string.h"

#define PARAM_ADDR  (0x0800FC00)
/********************************************/ 

void paramPortWriteData(uint8_t* data, uint16_t len)
{
    flashErase2(PARAM_ADDR, len);
    flashWrite(PARAM_ADDR, data, len);
}

uint16_t paramPortReadData(uint8_t* data, uint16_t len)
{
	memcpy(data, (void*)(PARAM_ADDR), len);
    return len;
}

void paramPortInit(void)
{
	
}
/************************************************/ 
