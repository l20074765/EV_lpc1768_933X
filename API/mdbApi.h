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

typedef struct _st_mdb_{
	uint8 bill_type;
	uint8 coin_type;
	uint8 highEnable;
}ST_MDB;

extern ST_MDB xdata stMdb;


uint8 MDB_billInit(void);
uint8 MDB_coinInit(void);
uint8 MDB_getBillAcceptor(void);
uint8 MDB_getBillDispenser(void);
uint8 MDB_getCoinAcceptor(void);
uint8 MDB_getCoinDispenser(void);
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
#endif

/**************************************End Of File*******************************************************/
