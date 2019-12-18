#include "ymodem_port.h"
#include "..\devpp\serial.h"
#include "..\devpp\serial.h"

T_Serial serial;

FILE* file;
uint32_t g_Filesize = 0;

void ymodePutByte(uint8_t byte)
{
	serialWrite(&serial, (char*)&byte, 1);
}

uint8_t ymodeGetByte(uint8_t *byte)
{
	return serialRead(&serial, byte, 1);
}

void callFileInfoCb(char* filename, uint32_t filesize)
{
	char buf[50];
	sprintf(buf, "file\\%s", filename);
	printf("%s %dByte\r\n", filename, filesize);
	g_Filesize = filesize;
	file = fopen(buf, "wb+");
}

void callGetDataCb(uint8_t *data, uint32_t len)
{
	if(g_Filesize > len)
	{
		fwrite(data, 1, len, file);
		g_Filesize -= len; 
	}
	else
	{
		fwrite(data, 1, g_Filesize, file);
		g_Filesize = 0;
	}
}
void callSendEndCb(void)
{
	fclose(file);
	printf("ymode end\r\n");
}

void ymodemInit(void)
{
	serial.baudrate = 921600;
	serial.bytesize = 8;
	serial.parity = 'N';
	serial.stopbit = 1;
	if(-1 == serialOpen(&serial, "com4"))
	{
		perror("serial");
	}
}

void retFileInifCb(char * filename, uint32_t *filesize)
{
	static uint8_t flag = 1;
	if(flag)
	{
		fclose(file);
		file = fopen("main.c", "rb+");

		if(file == NULL)
		{
			perror("open");
			*filesize = 0;
		}
		else
		{
			strcpy(filename, "xservo.exe");
			fseek(file, 0L, SEEK_END);
			*filesize=ftell(file);
			fseek(file, 0L, SEEK_SET);
		}
		printf("statr send %d\r\n", *filesize);
		flag = 0;
	}
	else
	{
		*filesize = 0;
		flag = 1;
	}
	
}
uint32_t retFileDataCb( uint8_t *data, uint32_t *size)
{
	*size = fread(data, 1, 1024, file);
	if( *size == 0 )
	{
		fclose(file);
	}
	return *size;
}

