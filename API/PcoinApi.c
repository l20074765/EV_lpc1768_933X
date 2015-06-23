#include "..\config.h"

#define	PARALLCOIN_CTL		(1ul<<29)				//P1.20��������Ӳ����ʹ����,�͵�ƽʹ�ܡ��ߵ�ƽ����
#define SERIALCOIN_CTL 		(1ul<<30)				//P1.21��������Ӳ����ʹ����,�͵�ƽʹ�ܡ��ߵ�ƽ����

#define PARALLCHL_IN		0x0000000000070103		//����Ӳ����ͨ��
#define GETPPCOIN_CHL1()	((FIO1PIN >> 10)& 0x01)	//ͨ��1
#define GETPPCOIN_CHL2()	((FIO1PIN >> 9) & 0x01)	//ͨ��2
#define GETPPCOIN_CHL3()	((FIO1PIN >> 8) & 0x01)	//ͨ��3
#define GETPPCOIN_CHL4()	((FIO1PIN >> 4) & 0x01)	//ͨ��4
#define GETPPCOIN_CHL5()	((FIO1PIN >> 1) & 0x01)	//ͨ��5
#define GETPPCOIN_CHL6()	((FIO1PIN >> 0) & 0x01)	//ͨ��6

#define SERIALCOIN_IN		(1ul<<11)				//����Ӳ����ͨ��P2.11
#define GETSERIALCON_CHL()	((FIO2PIN >>11) & 0x01)	//����ͨ��



static uint8 highEnabled = 0;
static volatile uint32 recvAmount = 0;

ST_PCOIN stPcoin;

uint8 PCOIN_setCh(uint8 no,uint32 value)
{
	if(no > 16 || no == 0){
		return 0;
	}
	stPcoin.ch[no - 1] = value;
	return 1;
	
}



uint8 PCOIN_initParallelPluse(uint8 high)
{
	FIO1DIR &= (~PARALLCHL_IN);				//����ͨ��Ϊ����ģʽ
	InitTimer(1,480000);					//20msɨ��һ�β���Ӳ����	
	highEnabled = high;
	PCOIN_disableParallelPluse();
	return 1;
}



/*********************************************************************************************************
** Function name:       EnableParallelPluseCoinAcceptor
** Descriptions:        ʹ�ܲ���Ӳ����
** input parameters:    ����ȫ�ֱ���PARALLELPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 PCOIN_enableParallelPluse(void)
{
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	if(highEnabled == 1){ 
		FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	else{
		FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	stPcoin.isEnable = 1;
	return 1;
	
	
}
/*********************************************************************************************************
** Function name:       DisableParallelPluseCoinAcceptor
** Descriptions:        ���ܲ���Ӳ����
** input parameters:    ��
** output parameters:   ����ȫ�ֱ���PARALLELPULSECOINACCEPTORDEVICESTATUS
** Returned value:      ��
*********************************************************************************************************/
uint8 PCOIN_disableParallelPluse(void)
{
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	if(highEnabled == 1){
		FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	else{
		FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	stPcoin.isEnable = 0;
	return 1;
}



/*********************************************************************************************************
** Function name:       PCOIN_scanParallelPluse
** Descriptions:        ɨ�貢��Ӳ����ͨ����ע��ֻ����Time1��ʱ�ж���ɨ��
** input parameters:    ��
** output parameters:   
** Returned value:      ��
*********************************************************************************************************/
void PCOIN_scanParallelPluse(void)
{
	uint32 Rchannel;
	uint8 CurChannel,Temp;
	static uint8 PreChannel;
	static uint8 PreStatus ;
	Rchannel  = GETPPCOIN_CHL1()<<0;
	Rchannel |= GETPPCOIN_CHL2()<<1;
	Rchannel |= GETPPCOIN_CHL3()<<2;
	Rchannel |= GETPPCOIN_CHL4()<<3;
	Rchannel |= GETPPCOIN_CHL5()<<4;
	Rchannel |= GETPPCOIN_CHL6()<<5;
	Temp = (uint8)(~Rchannel);
	Temp &= 0x3F;
	switch(Temp)
	{
		case 0x00 : if(PreStatus == 0x01){
						PreStatus = 0x00;
						Trace("PreChannel=%d recvAmount=%d\r\n",
								PreChannel,recvAmount);
						recvAmount += stPcoin.ch[PreChannel - 1];
					}
					PreChannel = 0x00;
					CurChannel = 0x00;
					break;
		case 0x01 : CurChannel = 0x06;
					break;
		case 0x02 : CurChannel = 0x05;
					break;
		case 0x04 : CurChannel = 0x04;
					break;
		case 0x08 : CurChannel = 0x03;
					break;
		case 0x10 : CurChannel = 0x02;
					break;
		case 0x20 : CurChannel = 0x01;
					break;
		default  :  CurChannel = 0x00;
					break;					
	}
	if(PreStatus == 0x00){
		if((PreChannel > 0x00)&(PreChannel == CurChannel))
			PreStatus = 0x01;
		else
			PreChannel = CurChannel;
	}
}


/*****************************************����Ӳ����API**************************************************/

/*********************************************************************************************************
** Function name:       PCOIN_initSerialPluse
** Descriptions:        ��ʼ��������Ӳ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 PCOIN_initSerialPluse(uint8 high)
{
	FIO2DIR &= (~SERIALCOIN_IN);			//����ͨ��Ϊ����ģʽ
	InitTimer(1,480000);					//20msɨ��һ�β���Ӳ����
	highEnabled = high;
	PCOIN_disableSerialPluse();
	return 1;
}
/*********************************************************************************************************
** Function name:       PCOIN_enableSerialPluse
** Descriptions:        ʹ�ܴ���Ӳ����
** input parameters:    ����ȫ�ֱ���SERIALPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 PCOIN_enableSerialPluse(void)
{
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	if(highEnabled == 1){
		FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	else{
		FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	stPcoin.isEnable = 1;
	return 1;
}


/*********************************************************************************************************
** Function name:       PCOIN_disableSerialPluse
** Descriptions:        ʹ�ܴ���Ӳ����
** input parameters:    ����ȫ�ֱ���SERIALPULSECOINACCEPTORDEVICESTATUS
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 PCOIN_disableSerialPluse(void)
{
	FIO0DIR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);//P0.30 PLS_MASK
	if(highEnabled == 1){
		FIO0CLR |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	else{
		FIO0SET |= (SERIALCOIN_CTL|PARALLCOIN_CTL);
	}
	stPcoin.isEnable = 0;
	return 1;
}



/*********************************************************************************************************
** Function name:       PCOIN_scanSerialPluse
** Descriptions:        ɨ�贮��Ӳ����ͨ����ע��ֻ����Time1��ʱ�ж���ɨ��
** input parameters:    ��
** output parameters:   ��ɨ�赽��ֵ���봮������Ӳ����ר����Ϣ����
** Returned value:      ��
*********************************************************************************************************/
void PCOIN_scanSerialPluse(void)
{
	uint8 CurStatus;
	static uint8 PreStatus ;

	FIO2DIR &= (~SERIALCOIN_IN);
	CurStatus  = (uint8)GETSERIALCON_CHL();
	switch(CurStatus)
	{					
		case 0x00 :	if(PreStatus == 0x01){
						recvAmount += stPcoin.ch[0];
					}
					PreStatus = CurStatus;
					break;
		case 0x01 : PreStatus = CurStatus;
					break;	
		default   : break;
	}
}



uint32 PCOIN_recvAmount(void)
{
	return recvAmount;
}

uint32 PCOIN_costAmount(uint32 amount)
{
	uint32 remainAmount;
	if(amount <= recvAmount){
		recvAmount -= amount;
		remainAmount = 0;
	}
	else{
		remainAmount = amount - recvAmount;
		recvAmount = 0;
	}
	return  remainAmount;

}




uint8 PCOIN_isEnable(void)
{
	return stPcoin.isEnable;
}




/**************************************End Of File*******************************************************/


