#include "ymodem.h"
#include "ymodem_port.h"

T_Ymodem ymodem = {0};

uint16_t ymodeCRC16(uint8_t *data, uint32_t len)
{
	uint16_t crc = 0;  
	int i;  
	
	while(len--) 
	{  
		crc = crc ^ *data++ << 8;  
		
		for (i=0; i<8; i++) 
		{  
			if (crc & 0x8000) 
			{  
				crc = crc << 1 ^ 0x1021;  
			} 
			else
			{  
				crc = crc << 1;  
			}
		}  
	}  
	return crc;  
}

uint16_t  ymodeCheckSum(uint8_t *data, uint32_t len)
{
	uint16_t ret = 0;
	uint32_t i;
	for(i=0; i<len; i++)
	{
		ret += data[i];
	}
	return ret;
}

uint32_t getFileName(uint8_t *data, char* filename)
{
	uint32_t len, filesize;
	strcpy(filename, data);
	len = strlen(filename);
	data +=  len+1;
	
	if( data[0] == '0')
	{
		if(data[1] == 'x' )
		{
			sscanf(data, "0x%x", &filesize);
		}
		else if(data[1] == 'X')
		{
			sscanf(data, "0x%X", &filesize);
		}
	}
	else
	{
		sscanf(data, "%d", &filesize);
	}
	return filesize;
}

/*
 * len: 0收到EOF包
 * 0：未收到包数据
 * 1：收到包数据
 * 2：结束包
 * -1: 发送方结束传输
 */
int8_t ymodeGetPack(uint8_t *buf, uint32_t *len)
{
	#define GET_HEAD      (0)
	#define GET_PACK_128  (1)
	#define GET_PACK_1024 (2)
	
	uint8_t c;
	static uint8_t caseStu = 0;
	static uint32_t count = 0;
	while(ymodeGetByte(&c))
	{
		switch(caseStu)
		{
			case GET_HEAD:
				count = 0;
				switch(c)
				{
					case SOH: /*128字节包*/
						caseStu = GET_PACK_128;
						buf[count++] = c;
					break;
					
					case STX: /*1024字节包*/
						caseStu = GET_PACK_1024;
						buf[count++] = c;
					break;
					
					case EOT: /*结束包*/
						*len = 0;
						return YMODEM_GET_PACK_END;
					break;
					
					case CAN: /*发送方结束*/
						return YMODEM_GET_PACK_STOP;
					break;
					
					case CNC:
						return YMODEM_GET_PACK_CNC;
					
					default:  /*未知*/
						return YMODEM_GET_PACK_STOP;
					break;
				}
			break;
			case GET_PACK_128:
				buf[count++] = c;
				if( (YMODEM_PACK_128+5) <= count) /*收到一包*/
				{
					caseStu = 0;
					*len = count;
					return YMODEM_GET_PACK_DATA;
				}
			break;
			case GET_PACK_1024:
				buf[count++] = c;
				if( (YMODEM_PACK_1024+5) <= count) /*收到一包*/
				{
					caseStu = 0;
					*len = count;
					return YMODEM_GET_PACK_DATA;
				}
			break;
		}
	}
	return YMODEM_GET_PACK_NONE;
}

uint8_t ymodePackCheck(uint8_t *pack, uint32_t len)
{
	uint8_t ret = 0;
	uint16_t crc = pack[len-2]<<8 | pack[len-1];
	if(pack[1] == ymodem.nextNum)
	{
		if(pack[1] == ((~pack[2]) & 0xFF))
		{
			if(crc == ymodeCRC16(pack+3, len-5))
			{
				ymodem.nextNum++;
				ret = 1;
			}
		}
	}
	return ret;
}

uint32_t ymodePackPackHead(char* filename, uint32_t filelen)
{
	uint32_t i;
	uint16_t crc;
	uint16_t strLen = 0;
	
	memset(ymodem.packBuf, 0, YMODEM_PACK_1024+5);
	
	ymodem.packBuf[0] = SOH;
	ymodem.packBuf[1] = 0;
	ymodem.packBuf[2] = 0xFF;
	
	if( 0 == filelen )
	{
		memset(&ymodem.packBuf[3], 0, 128);
	}
	else
	{
		strcpy( &ymodem.packBuf[3], filename );
		strLen = strlen(filename);
		sprintf(&ymodem.packBuf[4+strLen], "%d", filelen);
	}
	crc = ymodeCRC16(&ymodem.packBuf[3], 128);
	ymodem.packBuf[YMODEM_PACK_128+3] = crc>>8 & 0xFF;
	ymodem.packBuf[YMODEM_PACK_128+4] = crc & 0xFF; 
	return YMODEM_PACK_128+5;
}

uint32_t ymodePackPack(uint8_t index,  uint32_t len)
{
	uint32_t i;
	uint16_t crc;
	ymodem.packBuf[1] = index;
	ymodem.packBuf[2] = ~index & 0xFF;
	
	if(len <= 128)
	{
		ymodem.packBuf[0] = SOH;
		for(i=len; i<128; i++)
		{
			ymodem.packBuf[i+3] = 0x1A;
		}
		crc = ymodeCRC16(&ymodem.packBuf[3], 128);
	}
	else
	{
		ymodem.packBuf[0] = STX;
		for(i=len; i<1024; i++)
		{
			ymodem.packBuf[i+3] = 0x1A;
		}
		crc = ymodeCRC16(&ymodem.packBuf[3], 1024);
	}
	
	ymodem.packBuf[i+3] = crc>>8 & 0xFF;
	ymodem.packBuf[i+4] = crc & 0xFF;
	
	return i+5;
}

void ymodeSendPack(uint8_t *data, uint32_t len)
{
	uint32_t i;
	for(i = 0; i<len; i++)
	{
		ymodePutByte(data[i]);
	}
}

int8_t  ymodemTxProcess(void)
{
	#define CASE_SEND_FILE_INFO			(0)
	#define CASE_SEND_WAITE_ACK 		(1)
	#define CASE_SEND_WAITE_CNC 		(2)
	#define CASE_SEND_SENDPACK_NEXT 	(4)
	#define CASE_SEND_SENDPACK_CURRENT 	(5)
	#define CASE_SEND_EOT			 	(6)
	#define CASE_SEND_OK	 			(0xFD)
	#define CASE_SEND_ERR	 			(0xFE)	
	
	#define WAITE_ACK(fail, success, timems) do{caseStu = CASE_SEND_WAITE_ACK; errStu = fail; succStu = success; waite = timems; curStu = caseStu;}while(0);
	
	static uint8_t caseStu = CASE_SEND_FILE_INFO, succStu = CASE_SEND_FILE_INFO, errStu = CASE_SEND_FILE_INFO, curStu = CASE_SEND_FILE_INFO;
	static uint8_t index = 0;
	uint8_t  c;
	uint32_t len;
	static uint32_t waite = 0;
	static uint16_t errcount = 10;
	
	switch(caseStu)
	{
		case CASE_SEND_FILE_INFO:
			index = 0;
			retFileInifCb(ymodem.filename, &ymodem.filelen);
			len = ymodePackPackHead(ymodem.filename, ymodem.filelen);
			ymodeSendPack(ymodem.packBuf, len);
			if(ymodem.filelen == 0)
			{
				WAITE_ACK(CASE_SEND_OK, CASE_SEND_OK, 1000);			
			}
			else
			{
				WAITE_ACK(CASE_SEND_ERR, CASE_SEND_WAITE_CNC, 1000);
			}
		break;
				
		case CASE_SEND_WAITE_CNC:
			printf("CASE_SEND_WAITE_CNC\r\n");
			WAITE_ACK(CASE_SEND_ERR, CASE_SEND_SENDPACK_NEXT, 1000);
		break;

		case CASE_SEND_SENDPACK_NEXT:
			printf("CASE_SEND_SENDPACK_NEXT\r\n");
			retFileDataCb(&ymodem.packBuf[3], &len);
			if(0 == len) /*文件发送结束*/
			{
				caseStu = CASE_SEND_EOT;
			}
			else 
			{
				len = ymodePackPack(++index,  len);
				ymodeSendPack(ymodem.packBuf, len);
				WAITE_ACK(CASE_SEND_SENDPACK_CURRENT, CASE_SEND_SENDPACK_NEXT, 1000);
			}

		break;
		
		case CASE_SEND_SENDPACK_CURRENT:
			printf("CASE_SEND_SENDPACK_CURRENT\r\n");
			ymodeSendPack(ymodem.packBuf, len);
			WAITE_ACK(CASE_SEND_SENDPACK_CURRENT, CASE_SEND_SENDPACK_NEXT, 1000);
		break;
		
		case CASE_SEND_EOT:
			printf("CASE_SEND_EOT\r\n");
			ymodePutByte(EOT);
			WAITE_ACK(CASE_SEND_EOT, CASE_SEND_OK, 1000);
		break;
		
		case CASE_SEND_WAITE_ACK:
			if(0 == waite--)
			{
				if( 0 == errcount-- )
				{
					caseStu = CASE_SEND_ERR;
				}
				else
				{
					caseStu = curStu;
				}
			}
			else if(ymodeGetByte(&c))
			{
				if( (c == ACK) || (c == CNC) )
				{
					caseStu = succStu;
					errcount = 10;
				}
				else if(c == NAK)
				{
					if(0 == errcount--)
					{
						caseStu = CASE_SEND_ERR;
					}
					else
					{
						caseStu = errStu;
					}
				}
				else
				{
					return -1;
				}
			}
		break;
		
		case CASE_SEND_ERR:
			printf("CASE_SEND_ERR\r\n");
			caseStu = CASE_SEND_FILE_INFO;
			return -1;
		break;
		
		case CASE_SEND_OK:
			printf("CASE_SEND_OK\r\n");
			caseStu = CASE_SEND_FILE_INFO;
			return 1;
		break;
		/*发送数据内容*/ 
		/*发送结束符*/ 	
	}
	return 0;
}

void ymodemProcess(void)
{
	#define CASE_WAITING		 	(0xFF)
	#define CASE_START           	(0)
	#define CASE_CAN			 	(1)
	#define CASE_HEAD			 	(2)
	#define CASE_HEAD_TRY		 	(3)
	#define CASE_DATA			 	(4)
	#define CASE_DATA_TRY_NEXT   	(5)	
	#define CASE_DATA_TRY_CURRENT   (6)	
	#define CASE_EOT		     	(7)
	#define CASE_EOT_TRY		 	(8)	
	#define CASE_EOT_CONFIRM     	(9)
	#define CASE_TX			     	(10)
	#define WAITING(fail, success, timems) do{caseStu = CASE_WAITING; failStu = fail; succStu=success; waiteTime = timems;}while(0);

	static uint8_t caseStu = CASE_START;
	static uint8_t failStu = CASE_START;
	static uint8_t succStu = CASE_START;
	static uint8_t tryCount = YMODEM_TRY_NUM;
	static uint32_t waiteTime = 0;
	
	switch(caseStu)
	{
		
		case CASE_START: /*开始传输*/
		{
			ymodePutByte(CNC);
			WAITING(CASE_START, CASE_HEAD, YMODEM_TIMEOUT);
		}
		break;
		
		case CASE_HEAD:
		{
			if( 0 == ymodem.packBuf[3] ) /*传输结束*/
			{
				ymodePutByte(ACK);
				caseStu = CASE_START;
			}
			else
			{
				ymodem.nextNum = 0;
				if( (ymodem.packBuf[1] == 0) && ymodePackCheck(ymodem.packBuf, ymodem.packlen) )
				{
					ymodem.filelen = getFileName(ymodem.packBuf+3, ymodem.filename); /*获取*/
					callFileInfoCb(ymodem.filename, ymodem.filelen); /*调用获取到文件名回调函数*/
					tryCount = YMODEM_TRY_NUM;
					caseStu = CASE_HEAD_TRY;
				}
				else
				{
					ymodePutByte(CAN);
					caseStu = CASE_START;
				}
				
			}
		}
		break;
		
		case CASE_HEAD_TRY:
		{
			if(tryCount--)
			{
				ymodePutByte(ACK);
				ymodePutByte(CNC);
				WAITING(CASE_HEAD_TRY, CASE_DATA, YMODEM_TIMEOUT);
			}
			else
			{
				caseStu = CASE_START;
				ymodePutByte(CAN);
			}
		}
		break;

		case CASE_DATA:
		{
			if(ymodePackCheck(ymodem.packBuf, ymodem.packlen))
			{
				callGetDataCb(ymodem.packBuf+3, ymodem.packlen-5); /*调用获取到数据的回调函数*/
				caseStu = CASE_DATA_TRY_NEXT;
				tryCount = YMODEM_TRY_NUM;
			}
			else
			{
				caseStu = CASE_DATA_TRY_CURRENT;
				tryCount = YMODEM_TRY_NUM;
			}
		}
		break;

		case CASE_DATA_TRY_NEXT:
		{
			
			if(tryCount--)
			{
				ymodePutByte(ACK);
				WAITING(CASE_DATA_TRY_NEXT, CASE_DATA, YMODEM_TIMEOUT);
			}
			else
			{
				caseStu = CASE_START;
				ymodePutByte(CAN);
			}
		}
		break;
		
		case CASE_DATA_TRY_CURRENT:
		{
			
			if(tryCount--)
			{
				ymodePutByte(NAK);
				WAITING(CASE_DATA_TRY_CURRENT, CASE_DATA, YMODEM_TIMEOUT);
			}
			else
			{
				caseStu = CASE_START;
				ymodePutByte(CAN);
			}
		}
		break;

		case CASE_EOT:
			tryCount = YMODEM_TRY_NUM;
			caseStu = CASE_EOT_TRY;
		break;
		
		case CASE_EOT_TRY: /*结束包*/
		{
			if( tryCount-- )
			{
				ymodePutByte(NAK);
				WAITING(CASE_EOT_TRY, CASE_EOT_CONFIRM, YMODEM_TIMEOUT);
			}
			else /*失败*/
			{
				caseStu = CASE_START;
				ymodePutByte(CAN);
			}
		}
		break;
		
		case CASE_EOT_CONFIRM: /*结束包确认*/
		{
			ymodePutByte(ACK);
			caseStu = CASE_START;
			callSendEndCb();
		}
		break;

		case CASE_CAN: /*主动结束*/
		{
			ymodePutByte(ACK);
			caseStu = CASE_START;
		}
		break;

		case CASE_TX:
		{
			int8_t ret = ymodemTxProcess();
			if(ret != 0)
			{
				caseStu = CASE_START;
				printf("ret = %d\r\n", ret);
			}
		}
		break;

		case CASE_WAITING: /*延时等待*/
		{
			int8_t resault = YMODEM_GET_PACK_NONE;
			if(0 == waiteTime--)
			{
				caseStu = failStu;
			}
			else
			{
				resault = ymodeGetPack(ymodem.packBuf, &ymodem.packlen);
				if(YMODEM_GET_PACK_DATA == resault)
				{
					caseStu = succStu;
				}
				else if(YMODEM_GET_PACK_STOP == resault)
				{
					caseStu = CASE_CAN;
				}
				else if(YMODEM_GET_PACK_CNC == resault)
				{
					caseStu = CASE_TX;
				}
				else if(YMODEM_GET_PACK_END == resault)
				{
					if(succStu != CASE_EOT_CONFIRM)
					{
						caseStu = CASE_EOT;
					}
					else
					{
						caseStu = CASE_EOT_CONFIRM;
					} 
				}
				
			}
		}
		break;
	}
}
