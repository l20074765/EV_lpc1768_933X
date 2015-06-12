#ifndef _TASK_DEV_H_
#define _TASK_DEV_H_
#include "..\Drivers\board.h"



#define DEV_NONE			0
#define DEV_BILL_ENABLE		1
#define DEV_COIN_ENABLE		2

typedef struct {
	uint8 type;
	uint8 obj;
	uint8 opt;
}Q_MSG;


void DEV_payoutReady(uint32 billAmount,uint32 coinAmount);
uint8 DEV_isPayoutFinish(void);
void task_dev(void *pdata);
uint8 DEV_coinEnableReq(uint8 opt);
uint8 DEV_billEnableReq(uint8 opt);
#endif


/**************************************End Of File*******************************************************/
