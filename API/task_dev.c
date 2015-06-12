#include "..\config.h"


#define DEBUG_DEV

#ifdef 	DEBUG_DEV
#define print_dev(...)	Trace(__VA_ARGS__)
#else
#define print_dev(...)
#endif

static volatile uint32  g_billPay = 0; 
static volatile uint32  g_coinPay = 0;
static volatile uint32  g_payMoney = 0;
static volatile uint32  g_changedMoney = 0;




#define DEV_PAY_IDLE		0
#define DEV_PAY_READY		1
#define DEV_PAY_FINISH		2

static volatile uint8   g_pay = DEV_PAY_IDLE;





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
	err = OSQPost(dev_Q_r,&dev_msg_s[dev_in_r]); 
	dev_in_r = (dev_in_r + 1) % DEV_QUEUE_SIZE;
	return (err == OS_NO_ERR);
}


/*********************************************************************************************************			
** Function name:       DEV_billEnableReq
** Descriptions:        发送纸币器使能命令
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
uint8 DEV_enableReq(uint8 obj,uint8 opt)
{
	dev_msg_s[dev_in_s].opt = opt;
	return DEV_msg_req(DEV_BILL_ENABLE);
	
}


/*********************************************************************************************************			
** Function name:       DEV_billEnableRpt
** Descriptions:        发送纸币器使能命令回应结果
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
uint8 DEV_billEnableRpt(Q_MSG *msg)
{
	uint8 err;
	if(msg == NULL){return 0;}
	MDB_billEnable(msg->opt);
	//err = DEV_msg_rpt(DEV_BILL_ENABLE);
	return err;
	
}

/*********************************************************************************************************			
** Function name:       DEV_coinEnableReq
** Descriptions:        发送硬币器使能命令
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
uint8 DEV_coinEnableReq(uint8 opt)
{
	dev_msg_s[dev_in_s].opt = opt;
	return DEV_msg_req(DEV_COIN_ENABLE);
	
}


/*********************************************************************************************************			
** Function name:       DEV_coinEnableRpt
** Descriptions:        发送硬币器使能命令回应结果
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
uint8 DEV_coinEnableRpt(Q_MSG *msg)
{
	uint8 err;
	if(msg == NULL){return 0;}
	MDB_coinEnable(msg->opt);
	//err = DEV_msg_rpt(DEV_BILL_ENABLE);
	return err;
	
}




static void DEV_reqPoll(void)
{
	uint8 i,err;
	Q_MSG *msg;
	msg = (Q_MSG *)OSQPend(dev_Q_s,10,&err);
	if(err == OS_NO_ERR){ //有请求
		switch(msg->type){
			case DEV_BILL_ENABLE:
				DEV_billEnableRpt(msg);
				break;
			case DEV_COIN_ENABLE:
				DEV_coinEnableRpt(msg);
				break;
			default:break;
		}
	}
}



void DEV_payoutReady(uint32 billAmount,uint32 coinAmount)
{
	g_payMoney = coinAmount + billAmount;
	g_pay = DEV_PAY_READY;
}

uint8 DEV_isPayoutFinish(void)
{
	if(g_pay == DEV_PAY_FINISH){
		g_pay = DEV_PAY_IDLE;
		return 1;
	}
	else{
		return 0;
	}
	
}


void MT_devInit(void)
{
	uint8 res,type,i,j;
	//初始化硬币器
	type = MDB_getCoinAcceptor();
	if(type == COIN_ACCEPTOR_PPLUSE){
		LED_show("CO--");
		PCOIN_initParallelPluse(stMdb.highEnable);
		res = 1;
		LED_show("CO-1%d",res);
	}
	else if(type == COIN_ACCEPTOR_SPLUSE){
		LED_show("CO--");
		PCOIN_initSerialPluse(stMdb.highEnable);
		res = 1;
		LED_show("CO-1%d",res);
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
					msleep(1500);
				}
			}
		}
		
		for(i = 0; i < 8;i++){
			for(j = 0;j < HP_SUM;j++){
				stMdb.billRato[i].ch[j] = stHopperLevel[i].ch;
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
			msleep(2000);
		}
	}
}






void task_dev(void *pdata)
{
	uint8 temp;
	pdata = pdata;
	
	DEV_registerOSQ();
	MT_devInit();
	
	msleep(500);
	while(1){
		//print_dev("task_dev\r\n");
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
		
		if(g_pay == DEV_PAY_READY){
			//g_changedMoney = MDB_bill_payout(g_payMoney);
			g_changedMoney = MDB_coin_payout(g_payMoney);
			g_pay = DEV_PAY_FINISH;
		}
		
		
		
		DEV_reqPoll();
		msleep(90);
	}
	
	
}








