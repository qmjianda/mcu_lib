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


// addr:��ַ  count:������  
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
*	�� �� ��: bsp_WriteCpuFlash
*	����˵��: д���ݵ�CPU �ڲ�Flash��
*	��    ��: _ulFlashAddr : Flash��ַ
*			 _ucpSrc : ���ݻ�����
*			 _ulSize : ���ݴ�С����λ���ֽڣ�
*	�� �� ֵ: 0 �ɹ���1 ���ݳ��Ȼ��ַ�����2 дFlash����(����Flash������)
**********************************************************************************************************/
uint8_t flashWrite(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize)
{
	uint32_t i;
	uint32_t usTemp;
	FLASH_Status status = FLASH_COMPLETE;

	/* ���ƫ�Ƶ�ַ����оƬ�������򲻸�д��������� */
	if (_ulFlashAddr + _ulSize > FLASH_BASE_ADDR + FLASH_SIZE)
	{
		return 1;
	}

	/* ����Ϊ0 ʱ����������  */
	if (_ulSize == 0)
	{
		return 0;
	}

    /*���ֽڶ���*/
    _ulSize = ((_ulSize+3)>>2);


	/* FLASH ���� */
	FLASH_Unlock();

  	/* Clear pending flags (if any) */
	FLASH_ClearFlag( FLASH_FLAG_BSY | FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);	

	/* ���ֽ�ģʽ��̣�Ϊ���Ч�ʣ����԰��ֱ�̣�һ��д��4�ֽڣ� */
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

  	/* Flash ��������ֹдFlash���ƼĴ��� */
  	FLASH_Lock();


	if (status == FLASH_COMPLETE)
	{
		return 0;
	}
	return 2;
}

