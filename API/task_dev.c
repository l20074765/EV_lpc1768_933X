#include "..\config.h"

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


void task_dev(void *pdata)
{
	uint8 temp;
	pdata = pdata;
	
	MDB_billInit();
	MDB_coinInit();
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
			
		if(g_pay == DEV_PAY_READY){
			//g_changedMoney = MDB_bill_payout(g_payMoney);
			g_changedMoney = MDB_coin_payout(g_payMoney);
			g_pay = DEV_PAY_FINISH;
		}
		
		msleep(90);
	}
	
	
}








