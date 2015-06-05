#ifndef _BOARD_H_
#define _BOARD_H_

#define xdata 


typedef unsigned char 		uint8;
typedef unsigned short 		uint16;
typedef unsigned int 		uint32;
typedef char 				int8;
typedef short 				int16;
typedef int 				int32;



//=============================================================================
//�꺯�������ʽ
//=============================================================================

#define HUINT16(n)				(uint8)(((n) >> 8) & 0xFF)//16λ��ֵ��8λ
#define LUINT16(n)				(uint8)((n) & 0xFF)//16λ��ֵ��8λ	
#define H0UINT32(n)				(uint8)(((n) >> 24) & 0xFF)//32λ��ֵ��һ����8λ
#define H1UINT32(n)				(uint8)(((n) >> 16) & 0xFF)//32λ��ֵ�ڶ�����8λ
#define L0UINT32(n)				(uint8)(((n) >> 8) & 0xFF)//32λ��ֵ��һ����8λ
#define L1UINT32(n)				(uint8)((n) & 0xff)//32λ��ֵ�ڶ�����8λ

#define INTEG16(h,l)			(uint16)(((uint16)(h) << 8) | ((l) & 0xFF))
//�ĸ�unsigned char �������ϳ�unsigned int������
#define INTEG32(h0,l0,h1,l1)	(uint32)(((uint32)(h0) << 24) | ((uint32)(l0) << 16) | \
										 ((uint32)(h1) << 8) | ((l1) & 0xFF))






//======================================================================================
unsigned char XorCheck(unsigned char *pstr,unsigned short len);
unsigned short CrcCheck(unsigned char *msg, unsigned short len);
void msleep(unsigned int msec);


#endif


/**************************************End Of File*******************************************************/
