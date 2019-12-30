#ifndef __LIB_RNG_H_
#define __LIB_RNG_H_

#include "include.h"

typedef struct
{
	char *pBuf;		/*ָ��ring��������ָ��*/
	int  capaticy;  /*ring�Ĵ�С*/
	int  fromBuf;    /*ָ���ȡ����*/
	int  toBuf;	 	/*ָ��ɴ�����*/
}T_Ring;

typedef int RING_ID;

RING_ID xrngInit(T_Ring *ring ,char *buffer, int bufLen);
bool xrngGet(RING_ID ring, char* rcv);
bool xrngPut(RING_ID ring, char val);
void xrngPutForce(RING_ID ring, char val);
int xrngBufGet(RING_ID ring, char*buf, int len);
int xrngBufPut(RING_ID ring, char*buf, int len);
bool xrngCpy(RING_ID ring, int where, char* rcv);
int xrngBufCpy(RING_ID ring , char* buf, int bufLen);
int xrngLen(RING_ID ring);
bool xrngIsFull(RING_ID ring);
bool xrngIsEmpty(RING_ID ring);
void xrngClear(RING_ID ring); 

#endif /*__LIB_RNG_H_*/
