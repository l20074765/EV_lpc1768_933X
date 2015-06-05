/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           lcd.h
** Last modified Date:  2012-07-22
** Last Version:         
** Descriptions:        LCD����������غ�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2011-07-22
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/
#include "..\config.h"

volatile unsigned int  EVBCONVERSATIONWAITACKTIMEOUT;	//EVBͨѶ����ACK��ʱʱ��
volatile unsigned int  UART3RECVACKMSGTIMEOUT;			//����ACK���ݰ���ʱʱ��
volatile unsigned char EvbAckFromDevice[8];				//EVB ACK���ݻ���
/*********************************************************************************************************
** Function name:     	Uart3RecvEvbAckFromDevice
** Descriptions:	    EVBͨѶ������ACK��
** input parameters:    ��
** output parameters:   ��
** Returned value:      1�����ճɹ���0������ʧ��
*********************************************************************************************************/
unsigned char Uart3RecvEvbAckFromDevice(void)
{
	unsigned char RecvData,AckIndex;
	if(Uart3BuffIsNotEmpty() == 1)
	{
		RecvData = Uart3GetCh();
		//Trace("GetCh====%x\r\n",RecvData);
		if((RecvData == 0xFD)||(RecvData == 0xEF))
		{
			AckIndex = 0;
			EvbAckFromDevice[AckIndex++] = RecvData;	
			UART3RECVACKMSGTIMEOUT = 20; 				//����ʣ���ֽڳ�ʱʱ�䣬100ms
			while(UART3RECVACKMSGTIMEOUT)
			{
				if(Uart3BuffIsNotEmpty()==1)
				{
					EvbAckFromDevice[AckIndex++] = Uart3GetCh();
					if((AckIndex == 2)&&(EvbAckFromDevice[0]==0xfd))
					{
						if(EvbAckFromDevice[1] > 0x08)
							break;
					}
					//if((AckIndex == 0x08)||((EvbAckFromDevice[0]==0xfd)&&(AckIndex == 0x06)))
					if((AckIndex == EvbAckFromDevice[1])||((EvbAckFromDevice[0]==0xef)&&(AckIndex==0x08)))	//����6/8�ֽڵ�ACK
					{
						if(EvbAckFromDevice[0]==0xfd)
						{
							if(EvbAckFromDevice[AckIndex-1] == XorCheck((unsigned char *)EvbAckFromDevice,(EvbAckFromDevice[1]-1)))
								return 1;
							else
								break;
						}
						else
						{
							if(EvbAckFromDevice[AckIndex-1] == XorCheck((unsigned char *)EvbAckFromDevice,7))
							{
								return 1;
							}
							else
							{
								break;
							}	
						}
					}
				}
			}
		}
		return 0;
	}
	return 0;
}


/*********************************************************************************************************
** Function name:     	EvbConversation
** Descriptions:	    EVBͨѶ
** input parameters:    Chl:ѡ��ͨ����Head:��ͷ��Sn�����кţ�Type:��Ϣ���Addr:�豸��ַ;Data�����ݣ�
** output parameters:   *ACK��Ӧ���
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
uint8_t EvbConversation(uint8_t Chl,uint8_t Head,uint8_t Sn,uint8_t Type,uint8_t Addr,uint16_t Data,uint8_t *Ack)
{
	unsigned char i,EvbSendBuff[8];

	i = Chl;//Ŀǰδ�ã�������
	EvbSendBuff[0] = Head;
	EvbSendBuff[1] = 0x08;
	if((Chl==1)&&(Type==0x71))
		Sn = 0x00;
	EvbSendBuff[2] = Sn;
	EvbSendBuff[3] = Type;
	EvbSendBuff[4] = Addr;
	if(Chl == 1)
	{
		EvbSendBuff[5] = (unsigned char)(Data>>8);//0x07;
		EvbSendBuff[6] = (unsigned char)Data;//0x00;
	}
	else
	{
		EvbSendBuff[5] = (unsigned char)Data;//0x08;
		EvbSendBuff[6] = (unsigned char)(Data>>8);//0x00;
	}	
	EvbSendBuff[7] = XorCheck(EvbSendBuff,7);

	//�������ݽ�ֹ�л�����
	OSSchedLock();
	Uart3PutStr(EvbSendBuff,8);
	OSSchedUnlock();
	
	Trace("Send=%02x %02x %02x %02x %02x %02x %02x %02x\r\n",EvbSendBuff[0],EvbSendBuff[1],EvbSendBuff[2],EvbSendBuff[3],EvbSendBuff[4],EvbSendBuff[5],EvbSendBuff[6],EvbSendBuff[7]);
	if(Type>=0x70)
		EVBCONVERSATIONWAITACKTIMEOUT = 500;
	else
		EVBCONVERSATIONWAITACKTIMEOUT = 100;
	while(EVBCONVERSATIONWAITACKTIMEOUT)			//1000ms���յ�ACK,����ʱ
	{
		//Trace("TimeOut...%d\r\n",EVBCONVERSATIONWAITACKTIMEOUT);
		if(Uart3RecvEvbAckFromDevice() == 1)
		{
			
			Trace("Recv1=%02x %02x %02x %02x %02x %02x %02x %02x\r\n",EvbAckFromDevice[0],EvbAckFromDevice[1],EvbAckFromDevice[2],EvbAckFromDevice[3],EvbAckFromDevice[4],EvbAckFromDevice[5],EvbAckFromDevice[6],EvbAckFromDevice[7]);
			
			for(i=0;i<8;i++)
			{
				*Ack++ = EvbAckFromDevice[i];
			}
			
			//SetUart2MdbMode();
			return 1;
		}

		OSTimeDly(2);
	}
	//Trace("Fail..\r\n");
	//SetUart2MdbMode();
	return 0;
}
/**************************************End Of File*******************************************************/
