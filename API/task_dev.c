#include "..\config.h"


#define DEBUG_DEV

#ifdef 	DEBUG_DEV
#define print_dev(...)	Trace(__VA_ARGS__)
#else
#define print_dev(...)
#endif




#define DEV_PAY_IDLE		0
#define DEV_PAY_READY		1
#define DEV_PAY_FINISH		2



#define DEV_QUEUE_SIZE 		20

void *dev_s[DEV_QUEUE_SIZE];
OS_EVENT *dev_Q_s;

void *dev_r[DEV_QUEUE_SIZE];
OS_EVENT *dev_Q_r;


Q_MSG dev_msg_s[DEV_QUEUE_SIZE];
Q_MSG dev_msg_r[DEV_QUEUE_SIZE];
static uint8 dev_in_s = 0;
static uint8 dev_in_r = 0;

static void DEV_registerOSQ(void)
{
	dev_Q_s = OSQCreate(&dev_s[0],DEV_QUEUE_SIZE);
	dev_Q_r = OSQCreate(&dev_r[0],DEV_QUEUE_SIZE);
}


uint32 g_hpMinCh = 0;



uint8 DEV_msg_req(uint8 type)
{
	uint8 err;
	dev_msg_s[dev_in_s].type = type;
	err = OSQPost(dev_Q_s,&dev_msg_s[dev_in_s]); 
	dev_in_s = (dev_in_s + 1) % DEV_QUEUE_SIZE;
	return (err == OS_NO_ERR);
	
}


uint8 DEV_msg_rpt(uint8 type)
{
	uint8 err;
	dev_msg_r[dev_in_r].type = type;
	err = OSQPost(dev_Q_r,&dev_msg_r[dev_in_r]); 
	dev_in_r = (dev_in_r + 1) % DEV_QUEUE_SIZE;
	return (err == OS_NO_ERR);
}


/*********************************************************************************************************			
** Function name:       DEV_billEnableReq
** Descriptions:        发送使能命令
** output parameters:       obj:1纸币器  2硬币器  opt:1使能 0禁能
** Returned value:          none
*********************************************************************************************************/
uint8 DEV_enableReq(uint8 obj,uint8 opt)
{
	dev_msg_s[dev_in_s].obj = obj;
	dev_msg_s[dev_in_s].opt = opt;
	
	return DEV_msg_req(DEV_ENABLE);
	
}


/*********************************************************************************************************			
** Function name:       DEV_billEnableRpt
** Descriptions:        发送纸币器使能命令回应结果
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
uint8 DEV_enableRpt(Q_MSG *msg)
{
	uint8 err = 0;
	if(msg == NULL){return 0;}
	if(msg->obj & OBJ_BILL){
		MDB_billEnable(msg->opt);
	}
	
	if(msg->obj & OBJ_COIN){
		MDB_coinEnable(msg->opt);
	}
	
	return err;
	
}


Q_MSG *DEV_getReqMsg(void)
{
	return &dev_msg_s[dev_in_s];
	
}

uint8 DEV_payoutReq(uint32 billAmount,uint32 coinAmount)
{
	dev_msg_s[dev_in_s].billAmount = billAmount;
	dev_msg_s[dev_in_s].coinAmount = coinAmount;

	return DEV_msg_req(DEV_PAYOUT);
}

uint8 DEV_payoutRpt(Q_MSG *msg)
{
	uint8 i,n;
	uint32 amount = 0,changed = 0,remain = 0;
	uint16 changedCount;
	uint16 hp[HP_SUM] = {0};
	
	ST_CHANGE_RATO *ratio = NULL;
	if(msg == NULL){return 0;}
	
	print_dev("DEV_payoutRpt:b_ratio=%d,c_ratio=%d\r\n",msg->billRatioIndex,msg->coinRatioIndex);
	
	MDB_billEnable(0);
	MDB_coinEnable(0);
	
	changed = 0;
	memset(hp,0,sizeof(uint16) * HP_SUM);
	
	if(msg->billRatioIndex > 0){ //有纸币兑币比例
		ratio = &stMdb.billRato[msg->billRatioIndex - 1];
		if(ratio->amount > 0){
			n = msg->billAmount / ratio->amount;
			for(i = 0;i < 8;i++){
				changedCount = HP_payout_by_level(i,ratio->num[i] * n,hp);
				changed += changedCount * ratio->ch[i];
				
			}
		}
		
		
	}
	
	if(msg->coinRatioIndex > 0){
		ratio = &stMdb.coinRato[msg->coinRatioIndex - 1];
		if(ratio->amount > 0){
			n = msg->coinAmount / ratio->amount;
			for(i = 0;i < 8;i++){
				changedCount = HP_payout_by_level(i,ratio->num[i] * n,hp);
				changed += changedCount * ratio->ch[i];
			}
		}
	}
	
	//兑零比例找完  需要再次查看 是否还需要找零
	amount = msg->billAmount + msg->coinAmount;
	if(amount >  changed){
		changed += HP_payout(amount - changed,hp);
		
	}
	
	print_dev("DEV_payoutRpt:amount=%d,changed=%d\r\n",amount,changed);
	remain = MDB_billCost(changed);
	print_dev("DEV_payoutRpt:remain=%d\r\n",remain);
	remain = MDB_coinCost(remain);
	
	print_dev("DEV_payoutRpt:remain=%d\r\n",remain);
	
	dev_msg_r[dev_in_r].billAmount = msg->billAmount;
	dev_msg_r[dev_in_r].coinAmount = msg->coinAmount;
	dev_msg_r[dev_in_r].coinChanged = changed;
	print_dev("\r\n");
	for(i = 0;i < HP_SUM;i++){
		print_dev("hp[%d] = %d\r\n",i,hp[i]);
		dev_msg_r[dev_in_r].hp[i] = hp[i];
	}
	print_dev("\r\n");
	if(amount > changed){
		remain = amount - changed;
		if(remain < g_hpMinCh){ //无法找零
			dev_msg_r[dev_in_r].iou = 0;
		}
		else{
			MDB_coinCost(remain);
			dev_msg_r[dev_in_r].iou = remain;
		}
	}
	else{
		dev_msg_r[dev_in_r].iou = 0;
	}
	

	return DEV_msg_rpt(DEV_PAYOUT);
}




uint8 DEV_hpPayoutReq(uint8 no,uint16 nums)
{
	dev_msg_s[dev_in_s].hp_no = no;
	dev_msg_s[dev_in_s].hp_nums = nums;
	return DEV_msg_req(DEV_HP_PAYOUT);
}


uint8 DEV_hpPayoutRpt(Q_MSG *msg)
{
	uint16 changed = 0;
	changed = HP_payout_by_no(msg->hp_no,msg->hp_nums);
	
	dev_msg_r[dev_in_r].coinChanged = changed;
	
	
	return DEV_msg_rpt(DEV_HP_PAYOUT);
	
}



static void DEV_reqPoll(void)
{
	uint8 err;
	Q_MSG *msg; 
	msg = (Q_MSG *)OSQPend(dev_Q_s,2,&err);
	if(err == OS_NO_ERR){ //有请求
		print_dev("DEV_reqPoll:type=%d\r\n",msg->type);
		switch(msg->type){
			case DEV_ENABLE:
				DEV_enableRpt(msg);
				break;
			case DEV_PAYOUT:
				DEV_payoutRpt(msg);
				break;
			case DEV_HP_PAYOUT:
				DEV_hpPayoutRpt(msg);
				break;
			default:break;
		}
	}
}


Q_MSG *DEV_rptPoll(void)
{
	uint8 err;
	Q_MSG *msg = NULL;
	msg = (Q_MSG *)OSQPend(dev_Q_r,2,&err);
	if(err == OS_NO_ERR){
		return msg;
	}
	else{
		return NULL;
	}
}


Q_MSG *DEV_msgRpt(uint8 type,uint32 timeout)
{
	uint8 err;
	Q_MSG *msg = NULL;
	Timer.dev_msg_rpt = timeout / 10 + 1;
	while(Timer.dev_msg_rpt){
		msg = (Q_MSG *)OSQPend(dev_Q_r,50,&err);
		print_dev("DEV_msgRpt:err=%d,type=%d\r\n",err,type);
		if(err == OS_NO_ERR && msg->type == type){
			print_dev("DEV_msgRpt:type=%d\r\n",type);
			return msg;
		}
	}
	return NULL;
}








void MT_devInit(void)
{
	uint8 res,type,i,j;
	type = MDB_getCoinAcceptor();//初始化硬币器
	if(type == COIN_ACCEPTOR_PPLUSE){
		LED_showString("CO--");
		PCOIN_initParallelPluse(stMdb.highEnable);
		LED_showString("CO-1");
	}
	else if(type == COIN_ACCEPTOR_SPLUSE){
		LED_showString("CO--");
		PCOIN_initSerialPluse(stMdb.highEnable);
		LED_showString("CO-1");
	}
	msleep(100);
	
	
	type = MDB_getCoinDispenser();
	if(type == COIN_DISPENSER_HOPPER){
		LED_show("HP--");
		HP_init();
		msleep(500);
		for(i = 0;i < HP_SUM;i++){
			if(stHopper[i].ch > 0){
				LED_show("HP%d-",i + 1);
				res = HP_send_check(&stHopper[i]);
				if(res == 1){
					LED_show("HP%d%d",i + 1,1);
					msleep(500);
				}
				else{
					LED_show("HP%d%d",i + 1,0);
					msleep(500);
				}
			}
			
			if(stHopperLevel[i].ch == 0){
				if(i > 0){
					g_hpMinCh = stHopperLevel[i - 1].ch;
				}
			}
		}
		
		for(i = 0; i < 8;i++){
			print_dev("ratio[%d]\r\n",i);
			for(j = 0;j < HP_SUM;j++){
				print_dev("ch[%d]=%d\r\n",j,stHopperLevel[j].ch);
				stMdb.billRato[i].ch[j] = stHopperLevel[j].ch;
				stMdb.coinRato[i].ch[j] = stHopperLevel[j].ch;
			}
		}
		
		
	}
	msleep(100);

	type = MDB_getBillAcceptor();
	if(type == BILL_ACCEPTOR_MDB){
		stBill.s.status |= BILL_BIT_FAULT;
		stBill.s.errNo |= BILL_ERR_COM;
		LED_show("BL--");	
		for(i = 0;i < 3;i++){
			res = billInit();
			if(res == 1){
				stBill.s.status &= ~BILL_BIT_FAULT;
				stBill.s.errNo = 0;
				LED_show("BL-1");
				break;
			}
			else{
				msleep(500);
			}
		}
		
		if(i >= 3){
			LED_show("BL-0");
			msleep(1000);
		}
	}
}






void task_dev(void *pdata)
{
	uint8 temp;
	pdata = pdata;
	DEV_registerOSQ();
	LED_ctrl(1,0);
	LED_ctrl(2,0);
	LED_ctrl(3,0);
	MT_devInit();
	//msleep(500);
	DEV_msg_rpt(DEV_INIT);
	
	while(1){
		temp = MDB_getBillAcceptor();
		if(temp == BILL_ACCEPTOR_MDB){
			billTaskPoll();
		}
		temp = MDB_getCoinAcceptor();
		if(temp == COIN_ACCEPTOR_MDB){
			coinTaskPoll();
		}
		
		temp = MDB_getCoinDispenser();
		if(temp == COIN_DISPENSER_HOPPER){
			HP_task();
		}	
		
		DEV_reqPoll();
		msleep(80);
	}
	
	
}








