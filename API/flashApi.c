#include "flashAPi.h"
#include "../config.h"


static unsigned char buf[512] = {0};

ST_LOG stLog;
/*********************************************************************************************************
** Function name:       saveFlash
** Descriptions:        保存数据到flash中
** input parameters:    page：页号；buf：需要保存的数据内容
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
unsigned char saveFlash(unsigned int page,unsigned char *buf,unsigned short len)
{
	if(!buf)
		return 0;
	return I2C0WrNByte(0xa0,2,page,buf,len);
}


/*********************************************************************************************************
** Function name:       readFlash
** Descriptions:        获取指定Flash地址上的数据
** input parameters:    addr：地址一次固定读取256字节
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
unsigned char readFlash(unsigned int page,unsigned char *buf,unsigned short len)
{
	if(!buf)
		return 0;
	return I2C0RdNByte(0xa0,2,page,buf,len);
}




unsigned char FM_readFromFlash(void)
{
	unsigned short index = 0;
	unsigned char i,j;
	ST_CHANGE_RATO *ratio;
	
	memset(buf,0,sizeof(buf));
	readFlash(0x00,buf,512);
	if(buf[index++] != 0xE5){
		return 0;
	}
	stMdb.bill_type = buf[index++];
	stMdb.coin_type = buf[index++];
	stMdb.highEnable = buf[index++];
	for(i = 0;i < 16;i++){
		stPcoin.ch[i] = INTEG32(buf[index + 0],buf[index + 1],
									buf[index + 2],buf[index + 3]);
		index += 4;
	}
	
	for(i = 0;i < HP_SUM;i++){
		stHopper[i].ch = INTEG32(buf[index + 0],buf[index + 1],
									buf[index + 2],buf[index + 3]);
		index += 4;
	}
	
	stMdb.pointValue = buf[index++];
	for(i = 0;i < 8;i++){
		ratio = &stMdb.billRato[i];
		ratio->amount = INTEG32(buf[index + 0],buf[index + 1],
									buf[index + 2],buf[index + 3]);
		index += 4;
		for(j = 0;j < 8;j++){
			ratio->num[j] = buf[index++];
		}
	}
	
	for(i = 0;i < 8;i++){
		ratio = &stMdb.coinRato[i];
		ratio->amount = INTEG32(buf[index + 0],buf[index + 1],
									buf[index + 2],buf[index + 3]);
		index += 4;
		for(j = 0;j < 8;j++){
			ratio->num[j] = buf[index++];
		}
	}
	
	Trace("ReadFlash:size=%d\r\n",index);
	return 1;
}



unsigned char FM_writeToFlash(void)
{
	unsigned short index = 0;
	unsigned char i,j;
	ST_CHANGE_RATO *ratio;
	buf[index++] = 0xE5; // 校验码
	buf[index++] = stMdb.bill_type;
	buf[index++] = stMdb.coin_type;
	buf[index++] = stMdb.highEnable;
	for(i = 0;i < 16;i++){
		buf[index++] = H0UINT32(stPcoin.ch[i]);
		buf[index++] = H1UINT32(stPcoin.ch[i]);
		buf[index++] = L0UINT32(stPcoin.ch[i]);
		buf[index++] = L1UINT32(stPcoin.ch[i]);
	}
	
	for(i = 0;i < HP_SUM;i++){
		buf[index++] = H0UINT32(stHopper[i].ch);
		buf[index++] = H1UINT32(stHopper[i].ch);
		buf[index++] = L0UINT32(stHopper[i].ch);
		buf[index++] = L1UINT32(stHopper[i].ch);
	}
	
	buf[index++] = stMdb.pointValue;
	
	for(i = 0;i < 8;i++){
		ratio = &stMdb.billRato[i];
		buf[index++] = H0UINT32(ratio->amount);
		buf[index++] = H1UINT32(ratio->amount);
		buf[index++] = L0UINT32(ratio->amount);
		buf[index++] = L1UINT32(ratio->amount);
		for(j = 0;j < 8;j++){
			buf[index++] = ratio->num[j];
		}
	}
	
	for(i = 0;i < 8;i++){
		ratio = &stMdb.coinRato[i];
		buf[index++] = H0UINT32(ratio->amount);
		buf[index++] = H1UINT32(ratio->amount);
		buf[index++] = L0UINT32(ratio->amount);
		buf[index++] = L1UINT32(ratio->amount);
		for(j = 0;j < 8;j++){
			buf[index++] = ratio->num[j];
		}
	}
	
	

	saveFlash(0x00,buf,index);
	return 1;
}


unsigned char FM_readLogFromFlash(void)
{
	unsigned short in = 0,i;

	memset(buf,0,sizeof(buf));
	readFlash(512,buf,512);
	if(buf[in++] != 0xE5){
		Trace("buf[in++] != 0xE5\r\n");
		return 0;
	}
	stLog.billRecv = INTEG32(buf[in + 0],buf[in + 1],buf[in + 2],buf[in + 3]);
	in += 4;
	stLog.coinRecv = INTEG32(buf[in + 0],buf[in + 1],buf[in + 2],buf[in + 3]);
	in += 4;
	stLog.coinChanged = INTEG32(buf[in + 0],buf[in + 1],buf[in + 2],buf[in + 3]);
	in += 4;
	stLog.iou = INTEG32(buf[in + 0],buf[in + 1],buf[in + 2],buf[in + 3]);
	in += 4;
	
	Trace("FLASH:bRecv=%d,cRecv=%d,iou=%d,changed=%d\r\n",
		stLog.billRecv,stLog.coinRecv,stLog.iou,stLog.coinChanged);
	
	
	for(i = 0;i < HP_SUM;i++){
		stLog.hpChanged[i] = INTEG32(buf[in + 0],buf[in + 1],buf[in + 2],buf[in + 3]);
		in += 4;
		Trace("Flash-hpChanged[%d]= %d\r\n",i,stLog.hpChanged[i]);
	}
	
	Trace("readLog:size=%d\r\n",in);
	return 1;
}


unsigned char FM_writeLogToFlash(void)
{
	unsigned short in = 0,i;
	
	buf[in++] = 0xE5; // 校验码
	buf[in++] = H0UINT32(stLog.billRecv);
	buf[in++] = H1UINT32(stLog.billRecv);
	buf[in++] = L0UINT32(stLog.billRecv);
	buf[in++] = L1UINT32(stLog.billRecv);
	
	buf[in++] = H0UINT32(stLog.coinRecv);
	buf[in++] = H1UINT32(stLog.coinRecv);
	buf[in++] = L0UINT32(stLog.coinRecv);
	buf[in++] = L1UINT32(stLog.coinRecv);
	
	buf[in++] = H0UINT32(stLog.coinChanged);
	buf[in++] = H1UINT32(stLog.coinChanged);
	buf[in++] = L0UINT32(stLog.coinChanged);
	buf[in++] = L1UINT32(stLog.coinChanged);
	
	
	buf[in++] = H0UINT32(stLog.iou);
	buf[in++] = H1UINT32(stLog.iou);
	buf[in++] = L0UINT32(stLog.iou);
	buf[in++] = L1UINT32(stLog.iou);
	
	for(i = 0;i < HP_SUM;i++){
		buf[in++] = H0UINT32(stLog.hpChanged[i]);
		buf[in++] = H1UINT32(stLog.hpChanged[i]);
		buf[in++] = L0UINT32(stLog.hpChanged[i]);
		buf[in++] = L1UINT32(stLog.hpChanged[i]);
	}
	
	
	saveFlash(512,buf,in);
	return 1;
}


unsigned char FM_clearLog(void)
{
	memset(&stLog,0,sizeof(stLog));
	FM_writeLogToFlash();
	return 1;
}



