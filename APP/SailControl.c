#include "../API/Public.h"
#include "SailControl.h"
#include "..\config.h"
#include "Global.h"
#include "Menu.h"

unsigned char Uart0RecvCmdBuff[8]={0};
volatile uint16_t RecvCmdTimer = 0;
unsigned char lsSn = 0;
unsigned char LsSn = 0;
extern uint8_t HpValueFlag;
uint8_t LstCmd = 0;
uint8_t LsMoneyAckbuf[8]={0};
uint8_t LsStateAckbuf[8]={0};
uint8_t LsOutCoinAckbuf[8]={0};



unsigned char IsCmdRecv(unsigned char *CMDType,unsigned char *SubCmd)
{
	uint8_t Recvdata = 0;
	uint8_t CmdIndex=0;

	Recvdata = Uart0GetCh();
	if(Recvdata == 0xEA)
	{
		Uart0RecvCmdBuff[CmdIndex++] = Recvdata;
		RecvCmdTimer = 20;
		while(RecvCmdTimer)
		{
			if(Uart0BuffIsNotEmpty()==1)
			{
				Uart0RecvCmdBuff[CmdIndex++] = Uart0GetCh();
				if(Uart0RecvCmdBuff[2]>8)
					break;
				if(CmdIndex == Uart0RecvCmdBuff[2])
				{
					if(Uart0RecvCmdBuff[CmdIndex-1] == XorCheck((unsigned char *)Uart0RecvCmdBuff,(Uart0RecvCmdBuff[2]-1)))
					{
						lsSn = Uart0RecvCmdBuff[1];
						*CMDType = Uart0RecvCmdBuff[3];
						LstCmd = Uart0RecvCmdBuff[5];
						SubCmd[0] = Uart0RecvCmdBuff[4];
						SubCmd[1] = Uart0RecvCmdBuff[5];
						SubCmd[2] = Uart0RecvCmdBuff[6];
						//Trace("OK...%02x %02x %02x\r\n",Uart0RecvCmdBuff[1],Uart0RecvCmdBuff[7],XorCheck((unsigned char *)Uart0RecvCmdBuff,(Uart0RecvCmdBuff[2]-1)));
						return 1;
					}
					else
						break;	
				}
			}	
		}		
	}
	return 0;
}




unsigned char ReturnAckPack(unsigned char CMDType,unsigned char databuf1,unsigned char databuf2,unsigned char databuf3)
{
	uint8_t Ackbuf[8]={0};

	if(lsSn != LsSn)
	{
		LsSn = lsSn;
		Ackbuf[0] = 0xEB;
		Ackbuf[1] = lsSn;
		Ackbuf[2] = 0x08;
		Ackbuf[3] = CMDType;
		Ackbuf[4] = databuf1;
		Ackbuf[5] = databuf2;
		Ackbuf[6] = databuf3;
		Ackbuf[7] = XorCheck((unsigned char *)Ackbuf,(Ackbuf[2]-1));
		Uart0PutStr(Ackbuf,Ackbuf[2]);
		if((CMDType == 0xa1)&&(LstCmd != 0xc2))
			memcpy(LsMoneyAckbuf,Ackbuf,8);
		else
		if((CMDType == 0xa1)&&(LstCmd == 0xc2))
			memcpy(LsStateAckbuf,Ackbuf,8);
		else
		if(CMDType == 0xa4)
			memcpy(LsOutCoinAckbuf,Ackbuf,8);
	}
	else
	{
		if((CMDType == 0xa1)&&(LstCmd != 0xc2))
			Uart0PutStr(LsMoneyAckbuf,8);	
		else
		if((CMDType == 0xa1)&&(LstCmd == 0xc2))
			Uart0PutStr(LsStateAckbuf,8);
		else
		if(CMDType == 0xa4)
			Uart0PutStr(LsOutCoinAckbuf,8);
	}
	return 0;
}

/*********************************************************************************************************
** Function name:       checkHopperState
** Descriptions:        ºÏ≤Èhopper◊¥Ã¨
** input parameters:    0 ’˝≥£ 1 »±±“ 2 π ’œ
** output parameters:   Œﬁ
** Returned value:      Œﬁ
*********************************************************************************************************/
uint8_t checkDeviceState(void)
{
	//≤È—Øhopper	
		hopperErrNo = HP_ERR_NO_NORMAL;
		HardWareErr = 0;	
		//hopper 1»±±“
		if(stHopper[0].state == 0x01 || stHopper[0].specialFault >= HOPPER_FAULT_NUM)
		{
			hopperErrNo |=	HP_ERR_NO_HOPPER1_EMPTY;
		}
		//hopper1π ’œ
		else if(stHopper[0].state != 0 )
		{
			hopperErrNo |=	HP_ERR_NO_HOPPER1_FAULT;
		}
	
		//hopper 2»±±“
		if(stHopper[1].state == 0x01 || stHopper[1].specialFault >= HOPPER_FAULT_NUM)
		{
			hopperErrNo |=	HP_ERR_NO_HOPPER2_EMPTY;
		}
		//hopper2π ’œ
		else if(stHopper[1].state != 0 )
		{
			hopperErrNo |=	HP_ERR_NO_HOPPER2_FAULT;
		}
		
#if (VMC_TYPE == VMC_TYPE_CHANGER)
		//≤È—Øhopper
		if((hopperErrNo & HP_ERR_NO_HOPPER1_FAULT) || (hopperErrNo & HP_ERR_NO_HOPPER2_FAULT))
		{
			HardWareErr |= SYS_ERR_NO_HOPPER;
		}
		else
		{
			if(stHopper[0].channelValue < stHopper[1].channelValue)
			{
				if(hopperErrNo & HP_ERR_NO_HOPPER1_EMPTY)
					HardWareErr |= SYS_ERR_NO_HOPPER;
			}
			else if(stHopper[0].channelValue > stHopper[1].channelValue)
			{
				if(hopperErrNo & HP_ERR_NO_HOPPER2_EMPTY)
					HardWareErr |= SYS_ERR_NO_HOPPER;
			}
			else
			{
				if((hopperErrNo & HP_ERR_NO_HOPPER2_EMPTY) && (hopperErrNo & HP_ERR_NO_HOPPER1_EMPTY))
				{
					HardWareErr |= SYS_ERR_NO_HOPPER;
				}
			}
			
		}
			
			
#else 
		//≤È—Øhopper
		if((hopperErrNo & HP_ERR_NO_HOPPER1_FAULT) || (hopperErrNo & HP_ERR_NO_HOPPER2_FAULT))
		{
			HardWareErr |= SYS_ERR_NO_HOPPER;
		}
		else
			HardWareErr &= ~(SYS_ERR_NO_HOPPER);
#endif
		
		if(SYSPara.BillType)
		{	
			GetBillDevState();
			if(stDevState.billState==1)
				HardWareErr |= SYS_ERR_NO_BILL;
			else
				HardWareErr &= ~(SYS_ERR_NO_BILL);
		}
	
		//≤È—Ø”≤±“∆˜◊¥Ã¨


		
		Trace("hopperErrNo %d HardWareErr %d...\r\n",hopperErrNo,HardWareErr);	
		if((hopperErrNo & HP_ERR_NO_HOPPER1_EMPTY) && (hopperErrNo & HP_ERR_NO_HOPPER2_EMPTY))
			return 1;
		else if(HardWareErr)
		{
			//disp_err_page();
			return 2;
		}
			

	return 0;
}


/*********************************************************************************************************
** Function name:       hopperIsEmpty
** Descriptions:        hopper»±±“ 
** input parameters:    
** output parameters:   Œﬁ
** Returned value:      1 »±±“  0 ’˝≥£
*********************************************************************************************************/
uint8_t hopperIsEmpty(void)
{
	return ((hopperErrNo & HP_ERR_NO_HOPPER1_EMPTY) && (hopperErrNo & HP_ERR_NO_HOPPER2_EMPTY));

}



unsigned char HopperDispence(unsigned int DispenceMoney,unsigned int *iouMoney)
{
	uint8_t res = 0;
	uint32_t SurplusMoney=0;
	uint8_t CHp1Out=0,CHp2Out=0,CHp3Out=0;
	*iouMoney = DispenceMoney;
	if(SYSPara.DispenceType == VMC_CHANGER_HOPPER)
	{
		res = evb_device_hp_dispence(DispenceMoney,&SurplusMoney,&CHp1Out,&CHp2Out,&CHp3Out);
		Trace("Dispence=%d Sur=%d cHp1=%d cHp2=%d cHp3=%d\r\n",
			DispenceMoney,SurplusMoney,CHp1Out,CHp2Out,CHp3Out);
		if(res==1) //∂“¡„≥…π¶
		{	
			stTotalTrade.DispenceSum += DispenceMoney;	
			stDetailTradeInfo.tradePayout = DispenceMoney;
		}
		else//∂“¡„ ß∞‹
		{
			stTotalTrade.DispenceSum += (DispenceMoney-SurplusMoney);
			stDetailTradeInfo.tradePayout = (DispenceMoney-SurplusMoney);
			*iouMoney = SurplusMoney;
		}
		
		if(CHp1Out)
		{
			stTotalTrade.Hopper1DispSum += CHp1Out;
		}
		if(CHp2Out)
		{	
			stTotalTrade.Hopper2DispSum += CHp2Out;
		}
		if(CHp3Out)
		{
			stTotalTrade.Hopper3DispSum += CHp3Out;	
		}
	}

	
	return res;
}




