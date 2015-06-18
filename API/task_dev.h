#ifndef _TASK_DEV_H_
#define _TASK_DEV_H_
#include "..\Drivers\board.h"
#include "..\API\hopperApi.h"


#define DEV_NONE			0
#define	DEV_INIT			1
#define DEV_ENABLE			2
#define	DEV_PAYOUT			3
#define	DEV_HP_PAYOUT		4


#define OBJ_BILL		0x01
#define OBJ_COIN		0x02
#define OBJ_ALL			0xFF

typedef struct {
	uint8 type;
	uint8 obj;
	uint8 opt;
	uint8 billRatioIndex;
	uint8 coinRatioIndex;
	uint8 hp_no;
	uint32 coinAmount;
	uint32 billAmount;
	uint32 billChanged;
	uint32 coinChanged;
	uint32 iou;
	uint16 hp[HP_SUM];
	uint16 hp_nums;
}Q_MSG;



void task_dev(void *pdata);
Q_MSG *DEV_rptPoll(void);
uint8 DEV_enableReq(uint8 obj,uint8 opt);
uint8 DEV_payoutReq(uint32 billAmount,uint32 coinAmount);
uint8 DEV_hpPayoutReq(uint8 no,uint16 nums);
Q_MSG *DEV_msgRpt(uint8 type,uint32 timeout);
Q_MSG *DEV_getReqMsg(void);
#endif


/**************************************End Of File*******************************************************/
