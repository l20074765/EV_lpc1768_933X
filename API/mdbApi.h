#ifndef _MDB_API_H_
#define _MDB_API_H_
#include "..\Drivers\board.h"


#define     BILL_ACCEPTOR_NONE		0
#define     BILL_ACCEPTOR_MDB		2

#define     BILL_DISPENSER_NONE		0
#define     BILL_DISPENSER_MDB		2	

#define     COIN_ACCEPTOR_NONE		0
#define     COIN_ACCEPTOR_PPLUSE	1
#define     COIN_ACCEPTOR_MDB		2
#define     COIN_ACCEPTOR_SPLUSE	3


#define     COIN_DISPENSER_NONE		0
#define     COIN_DISPENSER_HOPPER	1
#define     COIN_DISPENSER_MDB		2


typedef struct {
	uint32 amount; 	//要兑币的金额
	uint8  num[8]; 	//8个通道兑换枚数
	uint32 ch[8];  
}ST_CHANGE_RATO;


typedef struct _st_mdb_{
	uint8 bill_type;
	uint8 coin_type;
	uint8 highEnable;
	uint8 pointValue; 
	ST_CHANGE_RATO billRato[8];
	ST_CHANGE_RATO coinRato[8];
	
}ST_MDB;

extern ST_MDB xdata stMdb;


uint8 MDB_billInit(void);
uint8 MDB_coinInit(void);
uint8 MDB_getBillAcceptor(void);
void MDB_setBillAcceptor(uint8 type);
uint8 MDB_getBillDispenser(void);
void MDB_setBillDispenser(uint8 type);
uint8 MDB_getCoinAcceptor(void);
void MDB_setCoinAcceptor(uint8 type);
uint8 MDB_getCoinDispenser(void);
void MDB_setCoinDispenser(uint8 type);
uint8 MDB_coinEnable(uint8 en);
uint8 MDB_billEnable(uint8 en);
uint32 MDB_getBillRecvAmount(void);
uint32 MDB_getCoinRecvAmount(void);
uint8 MDB_getBillStatus(void);
uint16 MDB_getBillErrNo(void);
uint8 MDB_getCoinStatus(void);
uint16 MDB_getCoinErrNo(void);
uint32 MDB_bill_payout(uint32 payAmount);
uint32 MDB_coin_payout(uint32 payAmount);
uint32 MDB_valueFromCents(uint32 value);
uint32 MDB_valueToCents(uint32 value);
uint32 MDB_billCost(uint32 amount);
uint32 MDB_coinCost(uint32 amount);
#endif

/**************************************End Of File*******************************************************/
