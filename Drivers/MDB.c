/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           MDB.h
** Last modified Date:  2013-01-08
** Last Version:         
** Descriptions:        MDBͨѶЭ��                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-08
** Version:             V0.1
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"


//#define  DEBUG_MDB
#ifdef 	 DEBUG_MDB
#define  print_mdb(...)       Trace(__VA_ARGS__)
#else
#define  print_mdb(...) 
#endif




/*********************************************************************************************************
** Function name:     	MdbBusHardwareReset
** Descriptions:	    MDB���߸�λ����MDB����TXD������200ms;ע��ú���ΪӲ����λ�������������豸����λ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
/*void MdbBusHardwareReset(void)
{
	PINSEL0 &=(~(0x03<<20));	//P0.10��ΪGPIO
	FIO0DIR |= (1ul<<10);		//����Ϊ���
	FIO0CLR |= (1ul<<10);		//���0
	OSTimeDly(40);				//��ʱ200ms
	PINSEL0 |= (0x03<<20);		//����ΪTXD����
	InitUart2();
	return;
}*/


/*********************************************************************************************************
** Function name:     	MdbBusHardwareReset
** Descriptions:	    MDB���߸�λ����MDB����TXD������200ms;ע��ú���ΪӲ����λ�������������豸����λ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void MdbBusHardwareReset(void)
{
	SetUart2MdbMode();
	PINSEL0 &= (~(0x03<<20));	//P0.10��ΪGPIOģʽ
	FIO0DIR |= (1ul<<10);		//����Ϊ���
	FIO0CLR |= (1ul<<10);		//���0
	OSTimeDly(40);				//��ʱ200ms
	PINSEL0 |= (0x01<<20);		//����ΪTXD����
	InitUart2();
	return;
}



/*********************************************************************************************************
** Function name:     	MdbConversation
** Descriptions:	    MDB�豸ͨѶ
** input parameters:    Dev:�豸��(��5λ��ַ������λ����)��
**						*SeDat:Ҫ���͵����ݣ�
**						SenDatLen��Ҫ�������ݵĳ���
** output parameters:   *RevDat:���յ������ݣ�RevLen:���յ����ݵĳ��ȣ�
** Returned value:      0��ͨѶ��ʱ 1��ͨѶ�ɹ��� 2 ����Խ�� 3У�����
*********************************************************************************************************/
uint8_t MDB_conversation(uint8_t dev,uint8_t *sdata,uint8_t slen,uint8_t *rdata,uint8_t *rlen)
{
	unsigned char i,check = 0,err = 0,temp;	

	check = dev;
	ClrUart2Buff();
	OSIntEnter();
	MdbPutChr(dev,MDB_ADD);					//�ͳ�MDB��ַ������
	for(i=0;i< slen;i++)				//�ͳ�����
	{
		MdbPutChr(sdata[i],MDB_DAT);
		check += sdata[i];	
	}
	MdbPutChr(check,MDB_DAT);				//�ͳ�У���
	OSIntExit();
#ifdef DEBUG_MDB
	print_mdb("MDB-SEND[%d]:",slen + 1);
	print_mdb("%02x ",dev);
	for(i = 0;i < slen;i++){
		print_mdb("%02x ",sdata[i]);
	}
	print_mdb("\r\n");
#endif
	Timer.mdb_timeout = 200;	//����ACK�ȴ�����ʱ1000ms
	while(Timer.mdb_timeout)
	{
		temp = GetMdbStatus();
		if(temp > MDB_RCV_DATA)
		{
			switch(temp)
			{
				case MDB_RCV_OK		: 
					*rlen = Uart2GetStr(rdata,MDB_MAX_BLOCK_SIZE);
					err = 1;
				  #ifdef DEBUG_MDB
				  print_mdb("MDB-RECV[%d]:",*rlen);
				  for(i = 0;i < *rlen;i++){
					print_mdb("%02x ",rdata[i]);		
				  }
				  print_mdb("\r\n");
				  #endif
									  break;	
				case MDB_OUT_RANGE	: err = 2;
									  break;
				case MDB_CHKSUM_ERR	: err = 3;
									  break;
				default:break;
			}
			break;	
		}
		OSTimeDly(2);//10ms ��ʱ
			
	}
	return err;	
}



/*********************************************************************************************************
** Function name:     	MdbConversation
** Descriptions:	    MDB�豸ͨѶ
** input parameters:    Dev:�豸��(��5λ��ַ������λ����)��
**						*SeDat:Ҫ���͵����ݣ�
**						SenDatLen��Ҫ�������ݵĳ���
** output parameters:   *RevDat:���յ������ݣ�RevLen:���յ����ݵĳ��ȣ�
** Returned value:      1��ͨѶ�ɹ���0��ͨѶʧ�ܣ�
*********************************************************************************************************/
uint8_t MdbConversation(uint8_t Dev,uint8_t *SeDat,uint8_t SenDatLen,uint8_t *RevDat,uint8_t *RevLen)
{
	unsigned char i,check = 0,err = 0;	
	unsigned char Temp;
	check = Dev;
	ClrUart2Buff();
	OSSchedLock();
	MdbPutChr(Dev,MDB_ADD);					//�ͳ�MDB��ַ������
	for(i=0;i<SenDatLen;i++)				//�ͳ�����
	{
		MdbPutChr(SeDat[i],MDB_DAT);
		check += SeDat[i];	
	}
	MdbPutChr(check,MDB_DAT);				//�ͳ�У���
	OSSchedUnlock();
	OSTimeDly(3);
	Timer.mdb_timeout = 100;	//����ACK�ȴ�����ʱ1000ms
	while(Timer.mdb_timeout)
	{
		Temp = GetMdbStatus();
		if(Temp > MDB_RCV_DATA)
		{
			switch(Temp)
			{
				case MDB_RCV_OK		: *RevLen = Uart2GetStr(RevDat,MDB_MAX_BLOCK_SIZE);
									  err = 1;
									  break;	
				case MDB_OUT_RANGE	: err = 2;
									  break;
				case MDB_CHKSUM_ERR	: err = 3;
									  break;
				default:break;
			}
			break;	
		}
		OSTimeDly(2);
			
	}
	return err;	
}




/**************************************End Of File*******************************************************/
