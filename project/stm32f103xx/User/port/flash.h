#ifndef __FLASH_H_
#define __FLASH_H_

#include "stm32f10x.h"
#include "stdbool.h"


/* ����F103��  512K FLASH , ÿ��PAGE = 2K �ֽڣ��ܹ� 256�� PAGE  */
/* ����F100C8��64K FLASH  , ÿ��PAGE = 1K �ֽڣ��ܹ� 64 �� PAGE  */

#define SECTOR_MASK			    0x0800FC00

#define FLASH_SECTOR_NUM    64  // ��������  
#define FLASH_SECTOR_SIZE   1024 // ������С1KB
#define FLASH_BASE_ADDR	    0x08000000 /* Flash����ַ */

#define FLASH_START_ADDR    ((uint32_t)FLASH_BASE_ADDR)  
#define FLASH_END_ADDR      ((uint32_t)(FLASH_BASE_ADDR + FLASH_SECTOR_NUM * FLASH_SECTOR_SIZE))  
#define	FLASH_SIZE		    (FLASH_SECTOR_NUM*FLASH_SECTOR_SIZE) /* Flash ���� */


#define FLASH_IS_EQU		0   /* Flash���ݺʹ�д���������ȣ�����Ҫ������д���� */
#define FLASH_REQ_WRITE		1	/* Flash����Ҫ������ֱ��д */
#define FLASH_REQ_ERASE		2	/* Flash��Ҫ�Ȳ���,��д */
#define FLASH_PARAM_ERR		3	/* ������������ */

bool flashErase2(uint32_t addr, uint32_t len);
bool flashErase(uint32_t addr, uint8_t count);
uint8_t flashWrite(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize);

#endif /*__FLASH_H_*/
