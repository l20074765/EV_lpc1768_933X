/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           HopperAccepter.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        Hopper�������ܽӿ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          liya
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"

//����������ַHopper�Ľṹ��
struct EvbHopper stEvbHp[3];



/*********************************************************************************************************
** Function name:     	HopperAccepter_Handle
** Descriptions:	    EVB�豸�������ƺ���
** input parameters:    DeviceCmdType:��Ҫ�������Ƶ��豸����
						Address:�豸��Ӳ�������ַ
						nbuf:�������ݣ���Hopper���Ҳ����ĳ�������
** output parameters:   ��
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
unsigned char HopperAccepter_Handle(unsigned char DeviceCmdType,unsigned char Address,unsigned char *nbuf)
{
	uint8_t EvbAckMsg[36],i,j;
	uint8_t MsgHead,MsgType,MsgAdd;
	uint16_t MsgDate;
	uint8_t res,DevNum=0;
	static uint8_t sn=0;
	
	//Hopper��ѯ����ָ��	
	if(DeviceCmdType == EVBHANDLE_HPQUERY)
	{
		Trace("HpAdd=%d\r\n",Address);
		MsgHead = 0xED;
		MsgType = EVBHANDLE_HPQUERY;
		MsgAdd = Address;
		MsgDate = 0;	
	}
	else
	//Hopper���Ҳ���ָ��
	if(DeviceCmdType == EVBHANDLE_HPOUTPUT)
	{
		MsgHead = 0xED;
		MsgType = EVBHANDLE_HPOUTPUT;
		MsgAdd = Address;
		MsgDate = (nbuf[0]<<8)|(nbuf[1]&0x00ff);	
	}
	for(j=0;j<5;j++)
	{
		//���Ͳ����������ж�Ӧ���
		res = EvbConversation(1,MsgHead,sn,MsgType,MsgAdd,MsgDate,EvbAckMsg);
		if(res == 1)
		{
			if((DeviceCmdType == EVBHANDLE_HPQUERY)||(DeviceCmdType == EVBHANDLE_HPOUTPUT))
			{
				for(i=0;i<3;i++)
				{
					if(stEvbHp[i].Add == EvbAckMsg[4])
					{
						DevNum = i;
						break;
					}
				}
				if(i>=3)
					return 0;
				sn++;
				Trace("1_EvbAckMsg=%x state=%d\r\n",EvbAckMsg[3],stEvbHp[DevNum].State);
				if(EvbAckMsg[3]==0x00)
					stEvbHp[DevNum].State = 0x00;
				else
				if((EvbAckMsg[3]==0x20)||(EvbAckMsg[3]==0x29))
					stEvbHp[DevNum].State = 0x02;
				else
				if((EvbAckMsg[3]==0x11)||(EvbAckMsg[3]==0x39)||(EvbAckMsg[3]==0x28)||(EvbAckMsg[3]==0x41))
					stEvbHp[DevNum].State = 0x03;	
				stEvbHp[DevNum].OutPutCount = (EvbAckMsg[5]<<8)+(EvbAckMsg[5]&0x00ff);
				Trace("2_EvbAckMsg=%x state=%d\r\n",EvbAckMsg[3],stEvbHp[DevNum].State);	
				return 1;
			}	
		}
		else
		{
			for(i=0;i<3;i++)
			{
				if(stEvbHp[i].Add == MsgAdd)
				{
					stEvbHp[i].State = 0x03;
					break;
				}
			}		
		}
	}
	return 0;
}



/*********************************************************************************************************
** Function name:     	HopperAccepter_CheckHPOk
** Descriptions:	    ������ӵ������ϵ�Hopper�豸�Ƿ����,ֻ����ʱ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void HopperAccepter_CheckHPOk(void)
{
	uint8_t j,i,res;

	//2.��⵱ǰ����Hopper����,�������򽫸�Hp�Ĺ���λ��־λ1
	for(j=0;j<3;j++)
	{
		for(i=0;i<3;i++)
		{
			res = HopperAccepter_Handle(EVBHANDLE_HPQUERY,stEvbHp[j].Add,0x00);	
			if(res == 1)
			{
				stEvbHp[j].DevBadFlag = 0;
				break;	
			}
			else
			{
				stEvbHp[j].DevBadFlag += 1;	
			}
		}
	}
}


/*********************************************************************************************************
** Function name:     	HopperAccepter_HpOutHandle
** Descriptions:	    Hopper���Ҳ�������
** input parameters:    Count����������
						HpNum:Hp��ţ�1,00��ַHp;2,01��ַHp;3,10��ַHp
** output parameters:   ��
** Returned value:      1���ɹ����ң�0������ʧ��
*********************************************************************************************************/
uint8_t HopperAccepter_HpOutHandle(uint8_t Count,uint8_t HpNum,uint8_t *HpOutNum)
{
	uint8_t res;
	uint8_t buf[2]={0};

	if(Count<=0)
		return 0;
	buf[0] = Count/256;
	buf[1] = Count%256;
	//���ͳ���ָ��
	res = HopperAccepter_Handle(EVBHANDLE_HPOUTPUT,stEvbHp[HpNum-1].Add,buf);
	if(res==1)
	{
		//�ȴ�������ɷ���Ӧ���
		Timer.PayoutTimer = 40+Count/2;
		while(Timer.PayoutTimer)
		{
			//��ѯ�Ƿ�������
			res = HopperAccepter_Handle(EVBHANDLE_HPQUERY,stEvbHp[HpNum-1].Add,0);
			if(res==1)
			{
				*HpOutNum = stEvbHp[HpNum-1].OutPutCount;
				return 1;
			}
		}
		return 0;	
	}
	return 0;
}

/*********************************************************************************************************
** Function name:     	HopperAccepter_GetState
** Descriptions:	    Hopper�豸�����豸״̬����,����ʱ���������ʵʱ���
** input parameters:    DevType:��Ҫ�������Ƶ��豸����
						--	EVBDEV_HP1:�����ַΪ00��Hopper
							EVBDEV_HP2:�����ַΪ01��Hopper
							EVBDEV_HP3:�����ַΪ10��Hopper
** output parameters:   ��
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
void HopperAccepter_GetState(uint8_t DevType)
{
	if(DevType == EVBDEV_HP1)
	{
		Trace("Hp1State=%d\r\n",stEvbHp[0].State);
		if((stEvbHp[0].Price !=0)&&(stEvbHp[0].State != 0x03))
			HopperAccepter_Handle(EVBHANDLE_HPQUERY,0x00,0x00);	
	}
	if(DevType == EVBDEV_HP2)
	{
		Trace("Hp2State=%d\r\n",stEvbHp[1].State);
		if((stEvbHp[1].Price !=0)&&(stEvbHp[1].State != 0x03))
			HopperAccepter_Handle(EVBHANDLE_HPQUERY,0x01,0x00);	
	}
	if(DevType == EVBDEV_HP3)
	{
		Trace("Hp3State=%d\r\n",stEvbHp[2].State);
		if((stEvbHp[2].Price !=0)&&(stEvbHp[2].State != 0x03))
			HopperAccepter_Handle(EVBHANDLE_HPQUERY,0x02,0x00);	
	}	
}






