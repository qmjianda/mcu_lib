#include "lib_rng.h"


/*
 *ʵ��ʹ����ʹ��xrngBufPut��xrngPutֻ�ܴ洢buflen-1������
 *ʹ��xrngPutForce���Դ洢bufLen������ 
 */
RING_ID xrngInit(T_Ring *ring ,char *buffer, int bufLen)
{

	ring->pBuf  = buffer;
	ring->capaticy = bufLen;
	ring->fromBuf   = 0;
	ring->toBuf	 = 0;
	return (int)ring;
}

/*
 * ��ring�ж�ȡ����
 * ���� 1����ȡ�ɹ�
 * ���� 0����ȡʧ�ܣ�ring�Ѿ�����
 */
bool xrngGet(RING_ID ring, char* rcv)
{
	bool ret = false;
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return ret;
    }
    
	if(_ring->fromBuf != _ring->toBuf) /*�ж��Ƿ��Ѿ�����*/
	{
		*rcv = _ring->pBuf[_ring->fromBuf++];
		
		/*fromBuf = fromBuf%capaticy*/
		if(_ring->fromBuf>=_ring->capaticy)
		{
			_ring->fromBuf = 0;
		}
		ret = true;
	}
	return ret;
}

/*
 * ��ring�п������ݣ��������ݲ�������ring�е����ݼ���
 * ���� 1����ȡ�ɹ�
 * ���� 0����ȡʧ�ܣ�ring�Ѿ�����
 */
bool xrngCpy(RING_ID ring, int where, char* rcv)
{
    int fromBuf , i=0;
    bool ret = false;
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return ret;
    }
    fromBuf = _ring->fromBuf;
    while(fromBuf  != _ring->toBuf)
    {
        if( i == where)
        {
            *rcv = _ring->pBuf[_ring->fromBuf];
            ret = true;
            break;
        }
        i++;
        fromBuf++;
        if(fromBuf >= _ring->capaticy)
        {
            fromBuf = 0;
        }
    }
    return ret;
}

/*
 * ��ring�п���len���ݴ洢��buf�У�ʵ�ʶ�ȡ�ĳ��ȿ���С��len�������������������ring�е�����
 * ���أ�ʵ�ʶ�ȡ�����ݳ���
 */
int xrngBufCpy(RING_ID ring , char* buf, int bufLen)
{
    int fromBuf = 0, i=0;
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return 0;
    }
    
    fromBuf = _ring->fromBuf;
    while(fromBuf != _ring->toBuf)
    {
        if(i<bufLen)
            buf[i++] = _ring->pBuf[_ring->fromBuf++];
        else
            break;
        if(fromBuf >= _ring->capaticy)
        {
            fromBuf = 0;
        }
    }
    return i;
}

/*
 * ������д�뵽ring�У���д����������ֻ��д�� capaticy-1������ 
 * ���� 1��д��ɹ�
 * ���� 0��д��ʧ�ܣ�ring�Ѿ�����
 */
bool xrngPut(RING_ID ring, char val)
{
	bool ret = false;
    int temp = 0;
    
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return ret;
    }
    
	temp = _ring->toBuf; /*���ڼ���toBuf+1�Ƿ���fromBuf���*/
	/*temp = temp%capaticy*/
	if(++temp>= _ring->capaticy)
	{
		temp = 0;
	}
	if(temp != _ring->fromBuf) /*�ж�ring�Ƿ�����*/
	{
		_ring->pBuf[_ring->toBuf++] = val;
		if(_ring->toBuf >= _ring->capaticy)
		{
			_ring->toBuf = 0;
		}
		ret = true;
	}
	return ret;
}

/*
 *ǿ�ƽ�����д�뵽ring�У�����Ѿ�д���򸲸ǵ�֮ǰ������
 */
void xrngPutForce(RING_ID ring, char val)
{
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return;
    }
    
	_ring->pBuf[_ring->toBuf++] = val; /*��д������*/
	if(_ring->toBuf >= _ring->capaticy)
	{
		_ring->toBuf = 0;
	}
	if(_ring->toBuf == _ring->fromBuf) /*�ж��Ƿ�д����*/
	{
		
        _ring->fromBuf++; /*�ƶ�fromBuf��toBuf��һ���ռ�*/
		/*fromBuf = fromBuf%capaticy*/
		if(_ring->fromBuf>=_ring->capaticy)
		{
			_ring->fromBuf = 0;
		}
	}
}

/*
 * ��ring�ж�ȡlen���ݴ洢��buf�У�ʵ�ʶ�ȡ�ĳ��ȿ���С��len
 * ���أ�ʵ�ʶ�ȡ�����ݳ���
 */
int xrngBufGet(RING_ID ring, char*buf, int len)
{
	int i=0;
	int min;
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return 0;
    }
    
	/*����ʵ�ʿ��Զ�ȡ������*/
	min = xrngLen(ring);
	if(min > len) 
	{
		min = len;
	}
	
    for(i=0; i< min; i++)
    {
        buf[i] = _ring->pBuf[_ring->fromBuf++];
        if( _ring->fromBuf>=_ring->capaticy )
        {
            _ring->fromBuf = 0;
        }
    }
	return i;
}

/*
 *��ring��д��һ�����ݣ�д��ĳ��ȿ���С��len�����ֻ��д�� capaticy-1������  
 *���أ�ʵ��д������ݵĳ���
 */
int xrngBufPut(RING_ID ring, char*buf, int len)
{
	int i=0;
    int min;
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return 0;
    }
    
	/*����ʵ�ʿ���д���������*/
	min = (_ring->capaticy - xrngLen(ring))-1;
	if(min > len)
	{
		min = len;
	}
    for(i=0; i<min; i++)
    {
        _ring->pBuf[_ring->toBuf++] = buf[i];
        if(_ring->toBuf >= _ring->capaticy)
        {
            _ring->toBuf = 0;
        }
    }
	return i;
}

/*
 *��ȡring���Ѿ��洢�����ݵĸ��� 
 */
int xrngLen(RING_ID ring)
{
	int ret = 0;
    
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return ret;
    }
    
	if(_ring->toBuf >= _ring->fromBuf)
	{
		ret = _ring->toBuf - _ring->fromBuf;
	}
	else
	{
		ret = _ring->capaticy - _ring->fromBuf;
		ret += _ring->toBuf;
	}
	return ret;
}

bool xrngIsFull(RING_ID ring)
{
	int toBuf;
    
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return false;
    }
    
    toBuf = _ring->toBuf;
    
	if(++toBuf >= _ring->capaticy)
	{
		toBuf = 0;
	}
	return (bool)(toBuf == _ring->fromBuf);
}

bool xrngIsEmpty(RING_ID ring)
{
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return false;
    }
	return (bool)(_ring->toBuf == _ring->fromBuf);
}

void xrngClear(RING_ID ring)
{
    T_Ring* _ring = (T_Ring*)ring;
    if(_ring == NULL)
    {
        return;
    }
	_ring->toBuf = _ring->fromBuf = 0;
}
