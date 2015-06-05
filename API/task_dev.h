#ifndef _TASK_DEV_H_
#define _TASK_DEV_H_
#include "..\Drivers\board.h"




void DEV_payoutReady(uint32 billAmount,uint32 coinAmount);
uint8 DEV_isPayoutFinish(void);
void task_dev(void *pdata);

#endif


/**************************************End Of File*******************************************************/
