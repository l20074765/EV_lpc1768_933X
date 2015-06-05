#include "..\config.h"

volatile uint16_t RS232WAITRECVPACKTIME;

struct STGbaMission
{
	//���ݰ�����
	uint8_t MsgLen;
	//�˴�ͨ�ŵ��豸״̬
	uint8_t State;
	//�豸ID��
	uint8_t Id;
	//������Ϣ�Ĵ洢buf
	uint8_t CmdMsg[32];
};

struct STGbaMission stGbaMission;

volatile uint8_t RecvBillType;
uint8_t BillColumnSet=0;


/*********************************************************************************************************
** Function name:       RS232BillDevMission
** Descriptions:        RS232ͨ�ŷ�ʽͨ�Ŵ�����
** input parameters:    buf���������ݣ�len�������
** output parameters:   ��
** Returned value:      1����ʾ�յ���ȷͨ�Ű���0����ʾͨ��ʧ��
*********************************************************************************************************/
unsigned char RS232BillDevMission(unsigned char *buf,unsigned char len)
{
	uint8_t check=0,i,res=0;
	static uint8_t lstate = 0x10;
	uint8_t j;
	unsigned char nRecvBuf[36]={0};

	RS232PutChr(0x02,RS232_GBA);
	RS232PutChr(0x08,RS232_GBA);
	check ^= 0x08;
	if(lstate != 0x10)
		lstate = 0x11;
	lstate ^= 0x01;
	RS232PutChr(lstate,RS232_GBA);
	check ^= lstate;
	for(i=0;i<len;i++)
	{
		RS232PutChr(buf[i],RS232_GBA);
		check ^= buf[i];
	}
	RS232PutChr(0x03,RS232_GBA);
	RS232PutChr(check,RS232_GBA);
	RS232WAITRECVPACKTIME = 200;
	while(RS232WAITRECVPACKTIME)
	{
		res = Uart2GetStr(nRecvBuf,36);
		//Trace("res=%d\r\n",res);
		if(res > 0)
		{
			res +=1;
			for(i=0;i<res;i++)
			{
				nRecvBuf[i] &= ~(1<<7);
				Trace(" %02x",nRecvBuf[i]);
			}
			Trace("\r\n");
			for(i=0;i<res;i++)
			{
				if(nRecvBuf[i]!=0x02)
					continue;
				if(nRecvBuf[i+1]!=0x0b)
					continue;
				if(nRecvBuf[i+9]!=0x03)
					continue;
				if((i+0x0b)>res)
					break;
				check = 0;
				for(j=i+1;j<i+9;j++)
				{
					check ^= nRecvBuf[j];
				}
				if(check != nRecvBuf[i+10])
					continue;
				stGbaMission.MsgLen = 0x0b - 5;
				stGbaMission.State = nRecvBuf[i+3];
				stGbaMission.Id = nRecvBuf[i+4];
				for(j=0;j<stGbaMission.MsgLen;j++)
				{
					stGbaMission.CmdMsg[j] = nRecvBuf[i+j+3];
				}
				return 1;
			}
			break;
		}
	}
	return 0;
}


/*********************************************************************************************************
** Function name:       GbaControl_BillAccept
** Descriptions:        RS232ͨ�ŷ�ʽֽ������ѯ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1����ʾ��ֽ�����룻0����ʾ��ֽ�ҽ��룻2��δ�յ����ݰ�
*********************************************************************************************************/
uint8_t GbaControl_BillAccept()
{
	uint8_t buf[3],res;

	buf[0]=BillColumnSet;
	buf[1]=0x14;
	buf[2]=0x00;
	
	res = RS232BillDevMission(buf,3);
	if(res == 1)
	{
		RecvBillType = stGbaMission.CmdMsg[2];
		if(stGbaMission.State == 0x04)
		{
			if(RecvBillType==0)
				return 0;
			else
				return 1;
		} 
		return 0;
	}
	return 2;
}

/*********************************************************************************************************
** Function name:       GbaControl_BillReject
** Descriptions:        RS232ͨ�ŷ�ʽֽ�����˱Һ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      1����ʾ��ֽ�����룻0����ʾ��ֽ�ҽ��룻2��δ�յ����ݰ�
*********************************************************************************************************/
uint8_t GbaControl_BillReject()
{
	uint8_t buf[3],res;

	buf[0]=0x00;
	buf[1]=0x54;
	buf[2]=0x00;
   	res = RS232BillDevMission(buf,3);
	if(res == 1)
	{
		RecvBillType = stGbaMission.CmdMsg[2];
		if(stGbaMission.State == 0x04)
		{
			if(RecvBillType==0)
				return 0;
			else
				return 1;
		} 
	}
	return 2;
}


/*********************************************************************************************************
** Function name:       GbaControl_BillStack
** Descriptions:        RS232ͨ�ŷ�ʽֽ����ѹ�ֺ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      1����ʾ��ֽ�����룻0����ʾ��ֽ�ҽ��룻2��δ�յ����ݰ�
*********************************************************************************************************/
uint8_t GbaControl_BillStack()
{
	uint8_t buf[3],res;

	buf[0]=BillColumnSet;
	buf[1]=0x34;
	buf[2]=0x00;
   	res = RS232BillDevMission(buf,3);
	if(res == 1)
	{
		RecvBillType = stGbaMission.CmdMsg[2];
		if(stGbaMission.State == 0x04)
		{
			if(RecvBillType==0)
				return 0;
			else
				return 1;
		} 
	}
	return 2;
}


/*********************************************************************************************************
** Function name:       GbaControl_BillStacked
** Descriptions:        RS232ͨ�ŷ�ʽֽ�����Ƿ�ѹ�ֳɹ���ѯ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1����ʾ��ֽ�����룻0����ʾ��ֽ�ҽ��룻2��δ�յ����ݰ�
*********************************************************************************************************/
uint8_t GbaControl_BillStacked()
{
	uint8_t buf[3],res;

	buf[0]=BillColumnSet;
	buf[1]=0x14;
	buf[2]=0x00;
   	res = RS232BillDevMission(buf,3);
	if(res == 1)
	{
		RecvBillType = stGbaMission.CmdMsg[2];
		if((stGbaMission.State == 0x11)||(stGbaMission.State == 0x30))
		{
			if(RecvBillType==0)
				return 0;
			else
				return 1;
		}
		return 0;
		
	}
	return 2;
}

/*********************************************************************************************************
** Function name:       GbaControl_BillGetState
** Descriptions:        RS232ͨ�ŷ�ʽֽ������ȡ״̬����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1����ʾ��ֽ�����룻0����ʾ��ֽ�ҽ��룻2��δ�յ����ݰ�
*********************************************************************************************************/
uint8_t GbaControl_BillGetState()
{
	uint8_t buf[3],res;

	buf[0]=BillColumnSet;
	buf[1]=0x14;
	buf[2]=0x00;
   	res = RS232BillDevMission(buf,3);
	if(res == 1)
	{
		if((stGbaMission.State == 0x06))
		{
			return 1;
		}
		else
			return 0;
		
	}
	return 0;
}

/*********************************************************************************************************
** Function name:       GbaControl_BillDisable
** Descriptions:        RS232ͨ�ŷ�ʽֽ�������ܺ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      1����ʾ��ֽ�����룻0����ʾ��ֽ�ҽ��룻2��δ�յ����ݰ�
*********************************************************************************************************/
uint8_t GbaControl_BillDisable()
{
	uint8_t buf[3],res;

	buf[0]=0x00;
	buf[1]=0x14;
	buf[2]=0x00;
   	res = RS232BillDevMission(buf,3);
	if(res == 1)
	{
		if((stGbaMission.State == 0x06))
		{
			return 1;
		}
		else
			return 0;
	
	}
	return 0;
}




