/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           Uart2.h
** Last modified Date:  2013-01-08
** Last Version:         
** Descriptions:        Uart2����������غ�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-08
** Version:             V0.1
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

volatile unsigned char  Uart2_RdBuf[UART2_BUFLEN];
volatile unsigned char  Uart2_RIndex;
volatile unsigned char  Uart2_FIndex;
volatile unsigned char  Uart2_Mode;
volatile unsigned char  MdbStatus = MDB_RCV_START;

/*********************************************************************************************************
** Function name:     	Uart2_Init
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitUart2(void)
{	
	PCONP = PCONP | (1<<24);
    U2LCR  = 0x80;                         
    U2DLM  = 0;
    U2DLL  = 125;
	U2FDR  = 0x41; 
    U2LCR  = 0x03;                      
    U2FCR  = 0x07;                      
    U2IER  = 0x05;
	ClrUart2Buff();
	memset((void *)Uart2_RdBuf,0x00,UART2_BUFLEN);                       
    zyIsrSet(NVIC_UART2,(unsigned long)Uart2IsrHandler,PRIO_FIVE);                                 
}
/*********************************************************************************************************
** Function name:     	Uart2_ClrBuf
** Descriptions:	    �������2���ջ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void ClrUart2Buff(void) 
{
	Uart2_FIndex = 0;
	Uart2_RIndex = 0;
	SetMdbStartStatus();//���ÿ�ʼ״̬
}
/*********************************************************************************************************
** Function name:     	SetUart2ParityMode
** Descriptions:	    ���ô�����żУ��λ
** input parameters:    mode:��żУ������,������MDBЭ��ı�־λ
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SetUart2ParityMode(unsigned char mode) 
{
	switch(mode) 
	{		
		case PARITY_ODD	: 	U2LCR = UART_LCR_PARITY_ODD|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
						 	break;
		case PARITY_EVEN:	U2LCR = UART_LCR_PARITY_EVEN|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
						 	break;
		case PARITY_F_1	:	U2LCR = UART_LCR_PARITY_F_1|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
						 	break;
		case PARITY_F_0	:	U2LCR = UART_LCR_PARITY_F_0|UART_LCR_PARITY_EN|UART_LCR_WLEN8;
						 	break;
		default:		 	U2LCR = UART_LCR_WLEN8;
						 	break;
	}
}
/*********************************************************************************************************
** Function name:     	MdbPutChr
** Descriptions:	    ��MDB�������ͳ�һ���ֽڵ�����
** input parameters:    dat--Ҫ�͵����ݣ�mode--���ͷ�ʽ��0Ϊ��ַ�ֽ�/1Ϊ�����ֽ�;��MDBЭ�鴦��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void MdbPutChr(unsigned char dat,unsigned char mode)
{
	switch(mode) 
	{
		case MDB_DAT :	SetUart2ParityMode(PARITY_F_0);
						Uart2PutCh(dat);
						break;
		case MDB_ADD:	SetUart2ParityMode(PARITY_F_1);
						Uart2PutCh(dat);
						break;
		default	:		
						SetUart2ParityMode(PARITY_DIS);
						Uart2PutCh(dat);
						break;
	}
	while(!(U2LSR & UART_LSR_TEMT));
}

void RS232PutChr(unsigned char dat,unsigned char mode)
{
	unsigned char check = 0,i;

	for(i=0;i<8;i++)
	{
		if(dat&(1<<i))
			check++;
	}
	if(check%2)
		dat |= 0x80;
	switch(mode)
	{
		case RS232_GBA:
			SetUart2ParityMode(PARITY_DIS);		
			Uart2PutCh(dat);
			break;
	}
}


/*********************************************************************************************************
** Function name:     	Uart2PutCh
** Descriptions:	    �򴮿�2����һ���ֽ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart2PutCh(unsigned char dat) 
{
   while(!(U2LSR & UART_LSR_THRE));
   U2THR = dat; 
}


/*********************************************************************************************************
** Function name:     	Uart2_PutStr
** Descriptions:	    �򴮿�2����һ���ַ���
** input parameters:    *buf�ַ�����Lenth�ַ�������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart2PutStr(unsigned char *buf,unsigned char lenth) 
{
	unsigned int i;
	for(i=0;i<lenth;i++)
	{
		Uart2PutCh(*buf++);
	}
}


/*********************************************************************************************************
** Function name:     	Uart2GetStr
** Descriptions:	    �Ӵ���2���ջ�������ȡһ���ַ���
** input parameters:    *buf�ַ�����Lenth�ַ�������
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
unsigned char Uart2GetStr(unsigned char *buf, unsigned char len) 
{
	unsigned char count,i;
	count = Uart2_RIndex - Uart2_FIndex;	
	if((len == 0) || (count == 0)) 
	    return 0;
	else if(len > count) 
	    len = count;
	for(i = 0; i < len; i++)	
	{
	    *buf++ = Uart2_RdBuf[Uart2_FIndex++];
	}
	if(Uart2_FIndex == Uart2_RIndex) 
	{
		Uart2_FIndex = 0;
		Uart2_RIndex = 0;
	}
	return (len-1);
}


/*********************************************************************************************************
** Function name:     	GetMdbStatus
** Descriptions:	    ��ȡ��ǰMDB����״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ǰMDB����״̬
*********************************************************************************************************/
unsigned char GetMdbStatus(void) 
{
	unsigned char status;
	status = MdbStatus;
	return status;
}
/*********************************************************************************************************
** Function name:     	SetMdbStartStatus
** Descriptions:	    ����MDB״̬
** input parameters:    ��
** output parameters:   MdbStatus
** Returned value:      ��
*********************************************************************************************************/
void SetMdbStartStatus(void)
{
	MdbStatus = MDB_RCV_START;
}
/*********************************************************************************************************
** Function name:     	Uart2_SetMdbMode
** Descriptions:	    ���ô���ΪMDB����״̬
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SetUart2MdbMode(void) 
{
	//Ӳ������
	FIO1DIR |= UART2MODESELECT0;
	FIO1CLR |= UART2MODESELECT0;
	FIO1DIR |= UART2MODESELECT1;
	FIO1CLR |= UART2MODESELECT1;
	//�������	
	Uart2_Mode = UART2_MDB_MODE;
}
/*********************************************************************************************************
** Function name:     	Uart2_SetEvbMode
** Descriptions:	    ���ô���ΪEVB1ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SetUart2Evb1Mode(void) 
{
	//Ӳ������
	FIO1DIR |= UART2MODESELECT0;
	FIO1SET |= UART2MODESELECT0;
	FIO1DIR |= UART2MODESELECT1;
	FIO1SET |= UART2MODESELECT1;
	//�������
	Uart2_Mode = UART2_EVB_MODE;
}
/*********************************************************************************************************
** Function name:     	Uart2_SetEvbMode
** Descriptions:	    ���ô���ΪEVB2ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SetUart2Evb2Mode(void) 
{
	//Ӳ������
	FIO1DIR |= UART2MODESELECT0;
	FIO1SET |= UART2MODESELECT0;
	FIO1DIR |= UART2MODESELECT1;
	FIO1SET |= UART2MODESELECT1;
	//�������
	Uart2_Mode = UART2_EVB_MODE;
}
/*********************************************************************************************************
** Function name:     	Uart2_SetEvbMode
** Descriptions:	    ���ô���ΪEVB3ģʽ
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SetUart2Evb3Mode(void) 
{
	//Ӳ������
	FIO1DIR |= UART2MODESELECT0;
	FIO1SET |= UART2MODESELECT0;
	FIO1DIR |= UART2MODESELECT1;
	FIO1SET |= UART2MODESELECT1;
	//�������
	Uart2_Mode = UART2_EVB_MODE;
}
/*********************************************************************************************************
** Function name:     	Uart2IsrHandler
** Descriptions:	    ����2�жϴ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart2IsrHandler(void) 
{
	volatile unsigned char dummy;
	unsigned int intsrc, tmp, tmp1;
	unsigned char udata;
	static unsigned char checksum;	
	OSIntEnter();	
	intsrc = U2IIR;									//Determine the interrupt source 
	tmp = intsrc & UART_IIR_INTID_MASK;				//UART_IIR_INTID_MASK = 0x0000000E,U2IIR[3:1]Ϊ�жϱ�ʶ
	//U2IIR[3:1] = 011;RLS������״̬���������ж�Ϊ�ĸ�����֮һ(OE,PE,FE,BI);��Ҫͨ���鿴LSR�Ĵ����õ�����ԭ��
	if(tmp == UART_IIR_INTID_RLS) 		
	{
		tmp1 = U2LSR;
		tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE | UART_LSR_BI | UART_LSR_RXFE);
		if(tmp1 & UART_LSR_PE)//ÿ��VMC������MDB���ݺ�Uart2���յھ�λ���Ǳ�ǿ��ΪУ��0ģʽ������˵���յ�MDB���ݽ�����־ 
		{
			udata = U2RBR & UART_RBR_MASKBIT;			//���ݿ���			
			if(Uart2_Mode == UART2_MDB_MODE) 
			{				
				if(MdbStatus == MDB_RCV_START) 			//��ǰ״̬Ϊ��ʼ����״̬
				{
					Uart2_RdBuf[Uart2_RIndex++] = udata;
					MdbStatus = MDB_RCV_OK;				//״̬ת��������
					MdbPutChr(MDB_ACK,MDB_DAT);			//�ͳ�ACK
				} 
				else if(MdbStatus == MDB_RCV_DATA) 		//��ǰ״̬Ϊ��������״̬
				{
					if(Uart2_RIndex < UART2_BUFLEN)		//������δ��
					{
						Uart2_RdBuf[Uart2_RIndex++] = udata;
						if(udata == checksum) 			//�ж�У��ֵ
						{
							MdbStatus = MDB_RCV_OK;
							MdbPutChr(MDB_ACK,MDB_DAT);
						} 
						else 
						{
							MdbStatus = MDB_CHKSUM_ERR;
							MdbPutChr(MDB_NAK,MDB_DAT);
						}
					} 
					else 
					{
						MdbStatus = MDB_OUT_RANGE;
						MdbPutChr(MDB_NAK,MDB_DAT);
					}
				}	
			}
		}
		else if(tmp1)							//��������������ж�����Ե� 
		{		
		    dummy = U2RBR & UART_RBR_MASKBIT;
		}
	}
	//U2IIR[3:1]=010�������ݿ��� || U2IIR[3:1]=110�ַ���ʱָʾ	  
	if((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI))  
	{	    
		if(Uart2_RIndex < UART2_BUFLEN)	
		{		    
			udata = U2RBR & UART_RBR_MASKBIT;
			Uart2_RdBuf[Uart2_RIndex++] = udata;
			if(Uart2_Mode == UART2_MDB_MODE)	//����MDBģʽ 
			{
				if(MdbStatus == MDB_RCV_START) 
				{
					checksum = 0;
				}
				MdbStatus = MDB_RCV_DATA;
				checksum += udata;
			}
		} 
		else									//��������
		{
		    dummy = U2RBR & UART_RBR_MASKBIT;
			if(Uart2_Mode == UART2_MDB_MODE)	//����MDBģʽ���ͳ�NAK
			{
				MdbStatus = MDB_OUT_RANGE;
				MdbPutChr(MDB_NAK,MDB_DAT);
			}
		}
	}
    OSIntExit();
}
/**************************************End Of File*******************************************************/
