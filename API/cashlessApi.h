#ifndef _CASHLESS_API_H_
#define _CASHLESS_API_H_

#include "..\Drivers\board.h"

#define CARD_JUST_RESET					0x0001UL
#define CARD_BEGIN_SESSION				0x0002UL   //卡已插入进入交易
#define CARD_SESSION_CANCEL_REQUEST		0x0004UL   //取消交易
#define CARD_VEND_APPROVED				0x0008UL   //卡金额足够，返回允许交易
#define CARD_VEND_DENIED				0x0010UL   //卡金额不够，返回拒绝交易
#define CARD_END_SESSION				0x0020UL   //结束本session,返回空闲使能状态	
#define CARD_MALFUNCTION_ERROR			0x0040UL	//故障
#define CARD_DISABLE					0x0080UL	//禁能


#define CARD_TRADE_IDLE					0
#define CARD_TRADE_BEGIN				1
#define CARD_TRADE_REQ					2
#define CARD_TRADE_SUC					3
#define CARD_TRADE_FAIL					4
#define CARD_TRADE_END					5





typedef struct {
	//读卡器相关参数
	uint8  level;			  //读卡器等级
	uint8  scale;			  //比例因子
	uint8  recTime;		  //最大回应时间
	uint8  miscelOpt;	  //是否支持其他功能，比如允许退回钱等等
	uint8  cashSale;	  //1允许现金详细交易记录都提交给他
	uint16 code;		  //国家代码
	uint16 decimal;		  //10^小数位数
	uint16 status;			//当前状态
	
	uint8 tradeStatus;	//当前交易状态
	uint32 cost;
}MDB_CARD;






typedef struct{
	uint8 type;
	uint8 paymentType;
	uint16 s;
	uint32 recvAmount;
	uint32 paymentId;
}CARD_POLL;


extern MDB_CARD stCard;


uint8 cardTaskPoll(void);

#endif


/**************************************End Of File*******************************************************/
