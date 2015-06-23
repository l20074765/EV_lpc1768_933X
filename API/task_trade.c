#include "task_trade.h"
#include "..\config.h"
#include "..\APP\Menu.h"

#define DEBUG_MAIN

#ifdef 	DEBUG_MAIN
#define print_main(...)	Trace(__VA_ARGS__)
#else
#define print_main(...)
#endif

/*********************************************************************************************************
  定义全局变量
*********************************************************************************************************/

#define TRADE_BEGIN						(0x01)				//
#define TRADE_DISP						(0x02)				//显示信息
#define TRADE_HPOUT						(0x03)				//找零
#define TRADE_SAVETRADE					(0x04)				//保存交易记录
#define TRADE_BILLIN					(0x05)				//检测是否有纸币或硬币进入
#define TRADE_FAULT						(0x17)				//故障




static volatile uint8 g_enterMenu = 0;//维护模式标志

volatile static uint8 g_billRatio = 0,g_coinRatio = 0;
static uint32 g_iou = 0;



/*********************************************************************************************************
** Function name:       SystemInit
** Descriptions:        系统初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SystemInit(void)
{
	//注意复位后，Uart0使能，Uart2/3被禁能
	InitUart0();
	InitUart1();
	InitUart2();
	InitUart3();
    InitBuzzer();
	InitI2C0();
	InitTimer(0,240000);
	InitRtc();
	RTCStartOrStop(1);
	InitDisplay();
	LED_showString("EU1.4");//显示版本号
	msleep(1500);	
	LED_showString("E---");
	InitKeyboard();//维护按键初始化
	EnableKeyBoard();
	LED_showString("E--1");
	msleep(100);	
}





/*********************************************************************************************************
** Function name:       SystemParaInit
** Descriptions:        系统参数初始化
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void SystemParaInit(void)
{
	memset((void *)&stBill,0,sizeof(stBill));
	memset((void *)&stCoin,0,sizeof(stCoin));
	memset((void *)&stMdb,0,sizeof(stMdb));
	FM_readFromFlash();
	FM_readLogFromFlash();
	MDB_setBillAcceptor(BILL_ACCEPTOR_MDB);
	MDB_setBillDispenser(BILL_DISPENSER_MDB);
	MDB_setCoinAcceptor(COIN_ACCEPTOR_PPLUSE);
	MDB_setCoinDispenser(COIN_DISPENSER_HOPPER);
	
	
}



/*********************************************************************************************************
** Function name:       MT_checkMenu
** Descriptions:        检查是否进入维护
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static uint8 MT_checkMenu(const uint8 state)
{

	g_enterMenu = MN_isMenuEntered();
	if(g_enterMenu == 1){
		DEV_enableReq(OBJ_ALL,0);
		MN_userMenu();
		
	}
	else if(g_enterMenu == 2){
		DEV_enableReq(OBJ_ALL,0);
		MN_adminMenu();
	}
	
	if(g_enterMenu){
		if(state == TRADE_FAULT){//退出维护选择显示页面
			DEV_enableReq(OBJ_ALL,0);
		}	
		else{
			DEV_enableReq(OBJ_ALL,1);
		}	
		g_enterMenu = 0;//跳出维护 清标志
		
		return 1;
	}
	return 0;
}



/*********************************************************************************************************
** Function name:       MT_checkDev
** Descriptions:        检查设备
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8 MT_checkDev(void)
{
	uint16 errNo = 0;
	uint8 i,hpIsEmpty,hpIsFault,hpUsedNum;
	uint8 lastPayFail;
	ST_HOPPER *hopper;
	errNo = MDB_getBillErrNo();
	if(errNo > 0){ //纸币器有故障
		LED_showString("BLEE");
		return 1;
	}
	
	//检查hopper 全故障 或者 全缺币
	hpIsEmpty = 0;
	hpIsFault = 0;
	hpUsedNum = 0;
	for(i = 0;i < HP_SUM;i++){
		if(stHopper[i].ch == 0){
			continue;
		}
		hpUsedNum++;
		if(stHopper[i].state == HP_STATE_QUEBI){
			hpIsEmpty++;
		}
		else if(stHopper[i].state == HP_STATE_FAULT || stHopper[i].state == HP_STATE_COM){
			hpIsFault++;
		}
		
	}
	
	if(hpIsFault >= hpUsedNum){
		LED_showString("HPEE");
		return 1;
	}
	
	if(hpIsEmpty >= hpUsedNum){
		LED_showString("HP00");
		return 1;
	}


	if(g_hpNo > 0){
		lastPayFail = 0;
		//print_main("g_hpNo=%d\r\n",g_hpNo);
		for(i = 0;i < stHopperLevel[g_hpNo - 1].num;i++){
			hopper = stHopperLevel[g_hpNo - 1].hopper[i];
			if(hopper->lastPayFail > 0){
				lastPayFail++;
			}
			else{
				break;
			}
		}
		 //所有最小面值hopper找币都失败
		if(lastPayFail >= stHopperLevel[g_hpNo - 1].num){
			LED_showString("####");
			msleep(1000);
			LED_showString("HP--");	
			msleep(1000);
			LED_showString("####");
			msleep(1000);
			LED_showAmount(g_iou);
			msleep(1000);
			return 1;
		}
	}

	return 0;
}





/*********************************************************************************************************
** Function name:       MT_checkPayout
** Descriptions:        检查是否可找零
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8 MT_checkPayout(uint32 billAmount,uint32 coinAmount)
{
	uint8 i;
	#if 0
	for(i = 0;i < HP_SUM;i++){
		if(stHopperLevel[i].ch == 0){
			if(i != 0){
				if(coinAmount <= stHopperLevel[i - 1].ch){ //硬币不符兑零
					
					return 0;
				}
			}
		}
	}
	#endif
	g_billRatio = 0;
	g_coinRatio = 0;
	for(i = 0; i < 8;i++){
		if(stMdb.billRato[i].amount == 0){
			continue;
		}
		if(billAmount == stMdb.billRato[i].amount){
			g_billRatio = i + 1;
			break;
		}
	}
	for(i = 0; i < 8;i++){
		if(stMdb.coinRato[i].amount == 0){
			continue;
		}
		if(coinAmount == stMdb.coinRato[i].amount){
			g_coinRatio = i + 1;
			break;
		}
	}

	
	//print_main("bill=%d,coin=%d,minch=%d\r\n",billAmount,coinAmount,g_hpMinCh);
	if(billAmount == 0 && coinAmount < g_hpMinCh){ //硬币比最小面值还小 无法找零
		return 0;
	}

	return 1;
}




static void MT_saveTradeLog(Q_MSG *msg)
{
	uint8 i;
	stLog.billRecv += msg->billAmount;
	stLog.coinRecv += msg->coinAmount;
	stLog.iou += msg->iou;
	stLog.coinChanged += msg->coinChanged;
	print_main("LOG:bRecv=%d,cRecv=%d,iou=%d,changed=%d\r\n",
		stLog.billRecv,stLog.coinRecv,stLog.iou,stLog.coinChanged);
	
	for(i = 0;i < HP_SUM;i++){
		stLog.hpChanged[i] += msg->hp[i];
		print_main("hpChanged[%d]= %d\r\n",i,stLog.hpChanged[i]);
	}
	FM_writeLogToFlash();
}


static void MT_ledPaomaDisplay(uint8 init)
{
	static uint8 in  = 1;
	
	if(init == 1){in = 1;return;}
	if(Timer.led_paoma == 0){
		switch(in){
			case 1:LED_showString("0###");break;
			case 2:LED_showString("#0##");break;
			case 3:LED_showString("##0#");break;
			case 4:LED_showString("###0");break;
			default:in = 1;break;
		}
		
		in = (in >= 4) ? 1 : in + 1;
		Timer.led_paoma = 500/10;
	}						
	
	
	
}



/*********************************************************************************************************
** Function name:       trade_task_changer
** Descriptions:        兑零机交易流程
** input parameters:   
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void task_trade(void)
{
	uint8 state = 0;
	static uint32 remainAmount = 0,curBillAmount = 0,curCoinAmount = 0;
	Q_MSG *msg;
	state = TRADE_BEGIN;
	disp_clear_screen();
	DEV_msgRpt(DEV_INIT,500000);	
	
	//主流程
	while(1){
		switch(state){
			case TRADE_BEGIN://起始流程
				remainAmount = 0;
				curBillAmount = 0;
				curCoinAmount = 0;		
				Timer.usr_opt = 1000; //10s
				MT_ledPaomaDisplay(1);
				DEV_enableReq(OBJ_ALL,1);
				state = TRADE_DISP;
				break;
			case TRADE_DISP://显示欢迎光临
				LED_showAmount(0);
				state = TRADE_BILLIN;
				break;
			case TRADE_BILLIN:
				curBillAmount = MDB_getBillRecvAmount();//接收纸币
				curCoinAmount = MDB_getCoinRecvAmount();//接收硬币
				remainAmount = curBillAmount + curCoinAmount;//总接收金额
				if(remainAmount > 0){ // 有金额进入找零环节
					LED_showAmount(remainAmount);//显示金额
					print_main("remainAmount1=%d\r\n",remainAmount);
					if(MT_checkPayout(curBillAmount,curCoinAmount) > 0){ //检查是否可找零
						state = TRADE_HPOUT;
					}
				}
				else{ //空闲状态 
					MT_checkMenu(state); //检测按键
					if(Timer.usr_opt == 0){
						MT_ledPaomaDisplay(0);
					}
					else{
						LED_showAmount(0);
					}
					
					if(MT_checkDev() > 0){	//检测设备状态
						state = TRADE_FAULT;
						DEV_enableReq(OBJ_ALL,0);
					}
				}
				
				break;
			case TRADE_HPOUT://找零
				msg = DEV_getReqMsg();
				msg->billRatioIndex = g_billRatio;
				msg->coinRatioIndex = g_coinRatio;
				DEV_payoutReq(curBillAmount,curCoinAmount);//正在找零
			
				msg = DEV_msgRpt(DEV_PAYOUT,900000); //等待找零结果
				if(msg->iou > 0){
					LED_showString("####");
					msleep(500);
					LED_showAmount(msg->iou);
					msleep(500);
					LED_showString("####");
					msleep(500);
					LED_showAmount(msg->iou);
					msleep(2000);
					g_iou = msg->iou;
				}

				MT_saveTradeLog(msg);
				state = TRADE_BEGIN;
				break;
			
			case TRADE_FAULT:
				MT_checkMenu(state); 
				if(MT_checkDev() == 0){
					state = TRADE_BEGIN;
				}
				
				break;
			default:
				break;
		}
		
		msleep(50);
	}	



}
