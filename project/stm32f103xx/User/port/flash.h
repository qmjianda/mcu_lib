#ifndef __FLASH_H_
#define __FLASH_H_

#include "stm32f10x.h"
#include "stdbool.h"


/* 对于F103，  512K FLASH , 每个PAGE = 2K 字节，总共 256个 PAGE  */
/* 对于F100C8，64K FLASH  , 每个PAGE = 1K 字节，总共 64 个 PAGE  */

#define SECTOR_MASK			    0x0800FC00

#define FLASH_SECTOR_NUM    64  // 扇区数量  
#define FLASH_SECTOR_SIZE   1024 // 扇区大小1KB
#define FLASH_BASE_ADDR	    0x08000000 /* Flash基地址 */

#define FLASH_START_ADDR    ((uint32_t)FLASH_BASE_ADDR)  
#define FLASH_END_ADDR      ((uint32_t)(FLASH_BASE_ADDR + FLASH_SECTOR_NUM * FLASH_SECTOR_SIZE))  
#define	FLASH_SIZE		    (FLASH_SECTOR_NUM*FLASH_SECTOR_SIZE) /* Flash 容量 */


#define FLASH_IS_EQU		0   /* Flash内容和待写入的数据相等，不需要擦除和写操作 */
#define FLASH_REQ_WRITE		1	/* Flash不需要擦除，直接写 */
#define FLASH_REQ_ERASE		2	/* Flash需要先擦除,再写 */
#define FLASH_PARAM_ERR		3	/* 函数参数错误 */

bool flashErase2(uint32_t addr, uint32_t len);
bool flashErase(uint32_t addr, uint8_t count);
uint8_t flashWrite(uint32_t _ulFlashAddr, uint8_t *_ucpSrc, uint32_t _ulSize);

#endif /*__FLASH_H_*/
