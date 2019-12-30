#ifndef __YMODEM_H_
#define __YMODEM_H_

#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "stdlib.h"

/* ASCII control codes: */  
#define SOH (0x01)      /* start of 128-byte data packet */  
#define STX (0x02)      /* start of 1024-byte data packet */  
#define EOT (0x04)      /* end of transmission */  
#define ACK (0x06)      /* receive OK */  
#define NAK (0x15)      /* receiver error; retry */  
#define CAN (0x18)      /* two of these in succession aborts transfer */  
#define CNC (0x43)      /* character 'C' */  

#define YMODEM_PACK_128	 (128)
#define YMODEM_PACK_1024 (1024)

#define YMODEM_GET_PACK_NONE  (0)
#define YMODEM_GET_PACK_DATA  (1)
#define YMODEM_GET_PACK_END   (2)
#define YMODEM_GET_PACK_CNC   (3)
#define YMODEM_GET_PACK_STOP  (-1)

#define YMODEM_TRY_NUM 		  (10)
#define YMODEM_TIMEOUT 		  (100)

typedef struct
{
	uint8_t nextNum;
	uint8_t packBuf[YMODEM_PACK_1024+5];
	uint32_t packlen;
	char filename[32];
	uint32_t filelen;
	
}T_Ymodem;

void ymodemProcess(void);

#endif /*__YMODEM_H_*/
