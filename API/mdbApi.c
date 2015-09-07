#include "..\config.h"


ST_MDB xdata stMdb;



/*********************************************************************************************************
* Function Name:        MDB_setBillAcceptor
* Description:          设置MDB纸币器接收器类型
* Input:                None
* Output:               None
* Return:               无
*********************************************************************************************************/
void MDB_setBillAcceptor(uint8 type)
{
	stMdb.bill_type &= ~0x0F;
	stMdb.bill_type |= (type & 0x0F);
}

/*********************************************************************************************************
* Function Name:        MDB_setBillDispenser
* Description:          设置MDB纸币器找零器类型
* Input:                None
* Output:               None
* Return:               无
*********************************************************************************************************/
void MDB_setBillDispenser(uint8 type)
{
	stMdb.bill_type &= ~(0x0F << 4);
	stMdb.bill_type |= ((type & 0x0F) << 4);
}



/*********************************************************************************************************
* Function Name:        MDB_setCoinAcceptor
* Description:          设置MDB硬币器接收器类型
* Input:                None
* Output:               None
* Return:               无
*********************************************************************************************************/
void MDB_setCoinAcceptor(uint8 type)
{
	stMdb.coin_type &= ~0x0F;
	stMdb.coin_type |= (type & 0x0F);
}


/*********************************************************************************************************
* Function Name:        MDB_setCoinDispenser
* Description:          设置MDB硬币器找零器类型
* Input:                None
* Output:               None
* Return:               无
*********************************************************************************************************/
void MDB_setCoinDispenser(uint8 type)
{
	stMdb.coin_type &= ~(0x0F << 4);
	stMdb.coin_type |= ((type & 0x0F) << 4);
}

	

/*********************************************************************************************************
* Function Name:        MDB_getBillAcceptor
* Description:          获取MDB纸币器接收器类型
* Input:                None
* Output:               None
* Return:               类型 0关闭 2 MDB
*********************************************************************************************************/
uint8 MDB_getBillAcceptor(void)
{
	return (stMdb.bill_type & 0x0F);
}


/*********************************************************************************************************
* Function Name:        MDB_getBillDispenser
* Description:          获取MDB纸币器找零器类型
* Input:                None
* Output:               None
* Return:               类型 0关闭 2 MDB
*********************************************************************************************************/
uint8 MDB_getBillDispenser(void)
{
	return ((stMdb.bill_type >> 4) & 0x0F);
}

uint8 MDB_getCoinAcceptor(void)
{
	return (stMdb.coin_type & 0x0F);
}


uint8 MDB_getCoinDispenser(void)
{
	return ((stMdb.coin_type >> 4) & 0x0F);
}


uint8 MDB_getCardType(void)
{
	return stMdb.card_type;
}

void MDB_setCardType(const uint8 type)
{
	stMdb.card_type = type;
}



uint8 MDB_billEnable(uint8 en)
{
	if(MDB_getBillAcceptor() == BILL_ACCEPTOR_MDB){
		return bill_enable(en);
	}
	
	return 1;
}


uint8 MDB_cardEnable(uint8 en)
{
	if(MDB_getCardType() == CARD_MDB){
		//card_enable(en);
		stCard.setEnable = en;
	}
	return 1;
}

uint8 MDB_coinEnable(uint8 en)
{
	uint8 res;
	if(MDB_getCoinAcceptor() == COIN_ACCEPTOR_MDB){
		res = coin_enable(en);
	}
	else if(MDB_getCoinAcceptor() == COIN_ACCEPTOR_PPLUSE){
		if(en == 1){
			res = PCOIN_enableParallelPluse();
		}
		else{
			res = PCOIN_disableParallelPluse();
		}
	}
	else if(MDB_getCoinAcceptor() == COIN_ACCEPTOR_SPLUSE){
		if(en == 1){
			res = PCOIN_enableSerialPluse();
		}
		else{
			res = PCOIN_disableSerialPluse();
		}
	}
	
	return res;
	
}


uint32 MDB_billCost(uint32 amount)
{
	uint8 temp = MDB_getBillAcceptor();
	uint32 remainAmount = 0;
	if(temp == BILL_ACCEPTOR_MDB){
		if(amount <= stBill.amount.recv_amount){
			stBill.amount.recv_amount -= amount;
			remainAmount = 0;
		}	
		else{
			remainAmount = amount - stBill.amount.recv_amount;
			stBill.amount.recv_amount = 0;
		}
			
	}
	else{
		remainAmount = amount;
	}
	
	return remainAmount;
}

uint32 MDB_coinCost(uint32 amount)
{
	uint8 temp = MDB_getCoinAcceptor();
	uint32 reaminAmount = 0;
	if(temp == COIN_ACCEPTOR_MDB){
		if(amount <= stCoin.amount.recv_amount){
			stCoin.amount.recv_amount -= amount;
			reaminAmount = 0;
		}
		else{
			reaminAmount = amount - stCoin.amount.recv_amount;
			stCoin.amount.recv_amount = 0;
		}
	}
	else if(temp == COIN_ACCEPTOR_PPLUSE){
		reaminAmount = PCOIN_costAmount(amount);
	}
	else if(temp == COIN_ACCEPTOR_SPLUSE){
		reaminAmount = PCOIN_costAmount(amount);
	}
	else{
		reaminAmount = amount;
	}
	
	return reaminAmount;
}




uint32 MDB_getBillRecvAmount(void)
{
	uint8 temp = MDB_getBillAcceptor();
	if(temp == BILL_ACCEPTOR_MDB){
		return stBill.amount.recv_amount;
	}
	else{
		return 0;
	}
}



uint32 MDB_getCoinRecvAmount(void)
{
	uint8 temp = MDB_getCoinAcceptor();
	if(temp == COIN_ACCEPTOR_MDB){
		return stCoin.amount.recv_amount;
	}
	else if(temp == COIN_ACCEPTOR_PPLUSE){
		return PCOIN_recvAmount();
	}
	else if(temp == COIN_ACCEPTOR_SPLUSE){
		return PCOIN_recvAmount();
	}
	else{
		return 0;
	}
}




uint8 MDB_billInit(void)
{
	uint8 type,res = 0;
	type = MDB_getBillAcceptor();
	if(type == BILL_ACCEPTOR_MDB){
		stBill.s.status |= BILL_BIT_FAULT;
		stBill.s.errNo |= BILL_ERR_COM;
		res = billInit();
		if(res == 1){
			stBill.s.status &= ~BILL_BIT_FAULT;
			stBill.s.errNo = 0;
		}
	}
	else{
		res = 0;
	}
	return res;	
}

uint8 MDB_coinInit(void)
{
	uint8 type,res = 0;
	type = MDB_getCoinAcceptor();
	if(type == COIN_ACCEPTOR_MDB){
		stCoin.state.s |= COIN_BIT_FAULT;
		stCoin.state.err |= COIN_ERR_COM;
		res = coinInit();
		if(res == 1){
			stCoin.state.s &= ~COIN_BIT_FAULT;
			stCoin.state.err = 0;
		}
	}
	else if(type == COIN_ACCEPTOR_PPLUSE){
		PCOIN_initParallelPluse(stMdb.highEnable);
		res = 1;
	}
	else if(type == COIN_ACCEPTOR_SPLUSE){
		PCOIN_initSerialPluse(stMdb.highEnable);
		res = 1;
	}
	
	type = MDB_getCoinDispenser();
	if(type == COIN_DISPENSER_HOPPER){
		HP_init();
		res = 1;
	}
	
	
	
	return res;
}



uint8 MDB_getBillStatus(void)
{
	uint8 s;
	uint8 temp = MDB_getBillAcceptor();
	if(temp == BILL_ACCEPTOR_MDB){
		s = stBill.s.status;
		stBill.s.status &= ~BILL_BIT_PAYBACK;//上报后清除 退币指令
		return s;
	}
	else{
		return 0;
	}
}

uint16 MDB_getBillErrNo(void)
{
	uint8 temp = MDB_getBillAcceptor();
	if(temp == BILL_ACCEPTOR_MDB){
		return stBill.s.errNo;
	}
	else{
		return 0;
	}
}


uint8 MDB_getCoinStatus(void)
{
	uint8 s = 0;
	uint8 temp = MDB_getCoinAcceptor();
	if(temp == COIN_ACCEPTOR_MDB){
		s = stCoin.state.s;
		stCoin.state.s &= ~COIN_BIT_PAYBACK;//上报后清除 退币指令
	}
	else{
		temp = MDB_getCoinAcceptor();
		if(temp == COIN_ACCEPTOR_PPLUSE || temp == COIN_ACCEPTOR_SPLUSE){
			if(PCOIN_isEnable() == 1){
				s &= ~COIN_BIT_DISABLE;
			}
			else{
				s |= COIN_BIT_DISABLE;
			}
		}
		//temp = MDB_getCoinDispenser();
		//if(temp == COIN_DISPENSER_HOPPER){
		//}
	}
	return s;
}

uint16 MDB_getCoinErrNo(void)
{
	uint8 temp = MDB_getCoinAcceptor();
	uint8 i;
	uint16 errNo = 0;
	if(temp == COIN_ACCEPTOR_MDB){
		errNo = stCoin.state.err;	 
	}
	else{
		temp = MDB_getCoinDispenser();
		if(temp == COIN_DISPENSER_HOPPER){
			errNo = 0;
			for(i = 0;i < HP_SUM;i++){
				errNo |= (stHopper[i].state << i*2);
			}
		}
		else{
			errNo = 0;
		}
	}
	return errNo;
}


uint32 MDB_bill_payout(uint32 payAmount)
{
	uint32 xdata changedAmount,payout_amount;
	uint8 i,b_enable = 0,c_enable = 0;
	uint16 j,count;
	uint32 ch;
	if(payAmount == 0){
		return 0;
	}
	
	if(stBill.setup.isRecycler == 0){
		return 0;
	}
	
	//校正找零金额
	payout_amount = 0;
	for(i = 0;i < 16;i++){
		ch = stBill.recycler.ch[i];
		count = stBill.recycler.ch_count[i];
		if(ch > 0 && count > 0){
			//该通道可以找零
			for(j = 0;j < count;j++){
				payout_amount += ch;
				if(payout_amount > payAmount){ //已经超出找零金额					
					break;
				}
			}
			if(payout_amount > payAmount){
				payout_amount -= ch;
				break;
			}
		}
	}
	
	if(payout_amount == 0){
		return 0;
	}
	
	if(stBill.s.status & BILL_BIT_DISABLE){
		b_enable = 0;
	}
	else{
		b_enable = 1;
		MDB_billEnable(0);
	}
	
	if(stCoin.state.s & COIN_BIT_DISABLE){
		c_enable = 0;
	}
	else{
		c_enable = 1;
		MDB_coinEnable(0);
	}
	
	msleep(100);
	changedAmount = bill_recycler_payout_by_value(payout_amount);
	msleep(100);
	if(c_enable == 1){
		MDB_coinEnable(1);
	}
	
	if(b_enable == 1){
		MDB_billEnable(1);
	}
	
	
	return changedAmount;
	
	
}



//硬币找零
uint32 MDB_coin_payout(uint32 payAmount)
{
	uint8 type,c_enable,b_enable;
	uint32 xdata changedAmount;
	uint16 hp[HP_SUM]= {0};
	if(payAmount == 0){
		return 0;
	}

	changedAmount = 0;
	
	if(stBill.s.status & BILL_BIT_DISABLE){
		b_enable = 0;
	}
	else{
		b_enable = 1;
		MDB_billEnable(0);
	}
	
	if(stCoin.state.s & COIN_BIT_DISABLE){
		c_enable = 0;
	}
	else{
		c_enable = 1;
		MDB_coinEnable(0);
	}
	
	msleep(100);
	
	type = MDB_getCoinDispenser();
	if(type == COIN_DISPENSER_MDB){
		changedAmount = coinPayout(payAmount);
	}
	else if(type == COIN_DISPENSER_HOPPER){
		changedAmount = HP_payout(payAmount,hp);
	}
	else{
		changedAmount = 0;
	}
	
	msleep(100);
	
	if(c_enable == 1){
		MDB_coinEnable(1);
	}
	
	if(b_enable == 1){
		MDB_billEnable(1);
	}

	return changedAmount;
}



/*********************************************************************************************************
** Function name:       MDB_valueFromPoint
** Descriptions:        分转换值
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint32 MDB_valueFromCents(uint32 value)
{
	uint32 v = 0;
	switch(stMdb.pointValue){
		case 0:v = value / 100; break;
		case 1:v = value / 10; 	break;
		case 2:v = value;		break;
		case 3:v = value * 10; break;
		default:v = value;		break;
	}
	return v;
}




/*********************************************************************************************************
** Function name:       MDB_valueToPoint
** Descriptions:        根据小数点转换分
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint32 MDB_valueToCents(uint32 value)
{
	uint32 v = 0;
	switch(stMdb.pointValue){
		case 0:v = value * 100; break;
		case 1:v = value * 10; 	break;
		case 2:v = value;		break;
		case 3:v = value / 10; break;
		default:v = value;		break;
	}
	return v;
	
}


void MDB_clearRecvAmount(void)
{
	uint32 amount;
	amount = MDB_getBillRecvAmount();//接收纸币
	MDB_billCost(amount);
	amount = MDB_getCoinRecvAmount();//接收硬币
	MDB_coinCost(amount);
}


