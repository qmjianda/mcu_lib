#ifndef __YMODEM_PORT_H_
#define __YMODEM_PORT_H_

#include <stdio.h>
#include <stdint.h>


void    ymodePutByte(uint8_t byte);
uint8_t ymodeGetByte(uint8_t *byte);

/*�������*/
void    callFileInfoCb(char* filename, uint32_t filesize);
void    callGetDataCb(uint8_t *data, uint32_t len);
void    callSendEndCb(void);

/*�������*/
void retFileInifCb( char* filename, uint32_t *filesize);
uint32_t retFileDataCb( uint8_t *data, uint32_t *size); 

void ymodemInit(void);

#endif /*__YMODEM_PORT_H_*/
