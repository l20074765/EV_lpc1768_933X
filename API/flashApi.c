#include "flashAPi.h"
#include "../config.h"


static unsigned char buf[512];


/*********************************************************************************************************
** Function name:       saveFlash
** Descriptions:        �������ݵ�flash��
** input parameters:    page��ҳ�ţ�buf����Ҫ�������������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char saveFlash(unsigned int page,unsigned char *buf,unsigned short len)
{
	if(!buf)
		return 0;
	return I2C0WrNByte(0xa0,2,page,buf,len);
}


/*********************************************************************************************************
** Function name:       readFlash
** Descriptions:        ��ȡָ��Flash��ַ�ϵ�����
** input parameters:    addr����ַһ�ι̶���ȡ256�ֽ�
** output parameters:   ��
** Returned value:      ��
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
	readFlash(0x00,buf,256);
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
	return 1;
}



unsigned char FM_writeToFlash(void)
{
	unsigned short index = 0;
	unsigned char i,j;
	ST_CHANGE_RATO *ratio;
	buf[index++] = 0xE5; // У����
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
	

	saveFlash(0x00,buf,index);
	return 1;
}







