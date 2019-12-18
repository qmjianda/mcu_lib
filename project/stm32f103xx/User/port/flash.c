#include "flash.h"
#include <string.h>

bool flashErase2(uint32_t addr, uint32_t len)
{
    uint8_t count;
    uint32_t _addr = (addr/FLASH_SECTOR_SIZE)*FLASH_SECTOR_SIZE;
    len = addr - _addr + len;
    count = (len+FLASH_SECTOR_SIZE-1)/FLASH_SECTOR_SIZE;
    return flashErase(_addr, count);
}


// addr:地址  count:块数量  
bool flashErase(uint32_t addr, uint8_t count)  
{  
    uint8_t i;  
    uint32_t pageAddr = 0;
    addr &= SECTOR_MASK;
    pageAddr = addr;
    FLASH_Unlock();
    for(i = 0; i < count; ++i)  
    {  
        if(FLASH_ErasePage(pageAddr) != FLASH_COMPLETE)  
        {  
            FLASH_Lock();
            return false;  
        }  
        pageAddr += FLASH_SECTOR_SIZE;
    }
    FLASH_Lock();
    return true;  
}  

/**********************************************************************************************************
*	函 数 名: bsp_WriteCpuFlash
*	功能说明: 写数据到CPU 内部Flash。
*	形    参: _ulFlashAddr : Flash地址
*			 _ucpSrc : 数据缓冲区
*			 _ulSize : 数据大小（单位是字节）
*	返 回 值: 0 成功，1 数据长度或地址溢出，2 写Flash出错(估计Flash寿命到)
**********************************************************************************************************/
uint8_t flashWrite(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize)
{
	uint32_t i;
	uint32_t usTemp;
	FLASH_Status status = FLASH_COMPLETE;

	/* 如果偏移地址超过芯片容量，则不改写输出缓冲区 */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* 长度为0 时不继续操作  */
	if (_ulSize == 0)
	{
		return 0;
	}

    /*四字节对齐*/
    _ulSize = ((_ulSize+3)>>2);


	/* FLASH 解锁 */
	FLASH_Unlock();

  	/* Clear pending flags (if any) */
	FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	/* 按字节模式编程（为提高效率，可以按字编程，一次写入4字节） */
	for (i = 0; i < _ulSize; i++)
	{	
        memcpy(&usTemp, _ucpSrc,4);
		status = FLASH_ProgramWord(_ulFlashAddr, usTemp);
		if (status != FLASH_COMPLETE)
		{
			break;
		}
		_ulFlashAddr += 4;
        _ucpSrc += 4;
	}

  	/* Flash 加锁，禁止写Flash控制寄存器 */
  	FLASH_Lock();


	if (status == FLASH_COMPLETE)
	{
		return 0;
	}
	return 2;
}

