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
	//³õÊ¼»¯Ó²±ÒÆ÷
	
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
		
		msleep(90);
	}
	
	
}








