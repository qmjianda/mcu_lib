#ifndef __CMD_H_
#define __CMD_H_

#include "cmd_core.h"
#include "uart.h"

#define GET_CHAR(c)					uartRdChar(c)
#define EXIT_MESSAGE()     			printf("press \"q\" or \"Q\" to exit!\r\n")
#define EXIT_CHECK()       			do(toupper(GET_CHAR()) == 'Q')\
									{\
										uint8_t c;\
										if(GET_CHAR(&c))\
										{\
											switch(c)\
												case 'q':\
												case 'Q':\
												case 0x1B:\
													return;\
										}\
									}while(0);

void cmdGet(uint8_t byte);
void cmdTask(void);

#endif /*__CMD_SYS_H_*/



