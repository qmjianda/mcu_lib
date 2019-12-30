#ifndef __DRV_AS5145_H_
#define __DRV_AS5145_H_

#include "include.h"
#include "bsp.h"

#define Dummy_Byte                (0x00)
#define AS5045_CH_MAX_NUM         (3)
#define IS_AS5045_CH(ch)          (ch<AS5045_CH_MAX_NUM)

#define CCW_VAL                   (1) /*0�ǶȰ�˳ʱ�뷽�����ӣ�1�ǶȰ���ʱ�뷽������*/

/*��һ��оƬ*/
#define A_AS5045_GPIO_CLK       (RCC_AHB1Periph_GPIOA)
#define A_AS5045_CSN_PORT       (GPIOA)
#define A_AS5045_CSN_PIN        (GPIO_Pin_4)

/*�ڶ���оƬ*/
#define B_AS5045_GPIO_CLK       (RCC_AHB1Periph_GPIOA)
#define B_AS5045_CSN_PORT       (GPIOA)                
#define B_AS5045_CSN_PIN        (GPIO_Pin_15)           

/*������оƬ*/
#define C_AS5045_GPIO_CLK       (RCC_AHB1Periph_GPIOE)
#define C_AS5045_CSN_PORT       (GPIOE)
#define C_AS5045_CSN_PIN        (GPIO_Pin_4)

typedef struct
{
	uint32_t  even   :1;  /*żУ��λ*/
	uint32_t  magDec :1;  /*�����������*/
	uint32_t  magInc :1;  /*�����������*/
	uint32_t  lin    :1;  /*���Զȱ���*/
	uint32_t  cof    :1;  /*cordic���*/
	uint32_t  ocf    :1;  /*ƫ������*/
	uint32_t  pos    :12; /*�Ƕ�*/
}T_As5045Msg;

void    AS5045Init(uint8_t ch);
int     AS5045GetRaw12BitWhithFiter(uint8_t ch);
float   AS5045_Raw12BitToAngle(int val);
float   AS5045_Raw12BitToAngle180(int val);
float   AS5045_Raw12BitToAngle360(int val);
uint16_t AS5045_AngleToRaw(float angle);
int     AS5045ZeroProgSelf(uint8_t ch);
uint8_t AS5045_StatuCheck(uint8_t ch);
#endif /*__BSP_AS5145_H_*/

