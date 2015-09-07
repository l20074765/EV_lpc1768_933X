#ifndef _FLASH_API_H
#define _FLASH_API_H

#include "..\Drivers\board.h"
#include "..\API\hopperApi.h"
//=======================flashҳ��==============================

//flashҳ����󲻳��� 0x1FFF = 8191 ���� flash�ᷭת��ַΪ0x0000
//ϵͳ����д��flash  add by yoc 2014.2.19
#define ADD_SYSTEM_PARA     (256)  //Ԥ��256bytes�ռ�洢


//=======================�������ʹ洢==============================
#define TYPE_HUODAO_ADDR			0x0001
#define TYPE_HUODAO_PRICE			0x0002
#define TYPE_HUODAO_STATE		    0x0004
#define TYPE_HUODAO_OPEN_FLAG		0x0008
#define TYPE_HUODAO_REMAIN			0x0010
#define TYPE_HUODAO_TRADE_COUNT		0x0020
#define TYPE_HUODAO_MAX_COUNT		0x0040
#define TYPE_HUODAO_PHYSIC_NO		0x0080
#define TYPE_HUODAO_LEVEL_OPEN      0x0100
#define TYPE_HUODAO_LOGIC      		0x0200

#define TYPE_HUODAO_ALL				0xFFFF

//=====================================================================


typedef struct {
	uint32 billRecv;
	uint32 coinRecv;
	uint32 cardRecv;
	uint32 billChanged;
	uint32 coinChanged;
	uint32 iou;
	uint32 lastIou;
	uint32 hpChanged[HP_SUM];
}ST_LOG;
extern ST_LOG stLog;





unsigned char FM_readLogFromFlash(void);
unsigned char FM_writeLogToFlash(void);
unsigned char saveFlash(unsigned int addr,unsigned char *buf,unsigned short len);
unsigned char readFlash(unsigned int addr,unsigned char *buf,unsigned short len);
unsigned char FM_writeToFlash(void);
unsigned char FM_readFromFlash(void);
unsigned char FM_clearLog(void);
#endif
