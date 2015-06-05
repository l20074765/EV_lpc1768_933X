#include "task_trade.h"
#include "..\config.h"
#include "..\APP\Menu.h"

#define DEBUG_MAIN

#ifdef 	DEBUG_MAIN
#define print_main(...)	do{Trace("MAIN:");Trace(__VA_ARGS__);}while(0)
#else
#define print_main(...)
#endif

/*********************************************************************************************************
  定义全局变量
*********************************************************************************************************/
#if 1
#define TRADE_BEGIN						(0x01)				//
#define TRADE_DISP						(0x02)				//显示信息
#define TRADE_HPOUT						(0x03)				//找零
#define TRADE_SAVETRADE					(0x04)				//保存交易记录
#define TRADE_BILLIN					(0x05)				//检测是否有纸币或硬币进入
#define TRADE_ISCANOUTGOODS				(0x06)				//检测是否可以出货
#define TRADE_TRADEOVER					(0x07)				//一次交易流程完成
#define TRADE_WEIHU						(0x08)				//进入维护模式
#define TRADE_CHECKCHANNEL				(0x12)				//查询货道
#define TRADE_OUTGOODS					(0x13)				//出货
#define TRADE_SELECTGOODS				(0x14)				//是否有选货按键
#define TRADE_CHECKDEVICEERR			(0x15)				//检测设备是否故障
#define TRADE_STOPSERVICE				(0x16)				//停止服务


#define TRADE_FAULT						(0x17)				//故障
#endif


//MAIN
#if 1
#define MAIN_START						(1)//交易开始
#define MAIN_FREE						(2)//交易空闲
#define MAIN_CHECK_GOODS				(4)//出货前检查
#define MAIN_OUTGOODS					(5)//出货
#define MAIN_OVER						(6)//交易结束
#define MAIN_WEIHU						(7)//维护模式
#define MAIN_FAULT						(0x16)//故障模式
#define MAIN_PAYOUT						(9)//找零
#endif



static volatile uint8 g_enterMenu = 0;//维护模式标志


volatile unsigned int TestPluse;
volatile uint16_t WaitMoneyInTimer;
volatile uint32_t WaitMoneyInTimer_1;

uint32_t nMinDispMoney = 0;
unsigned int MaxColumnMoney=0;


extern uint16_t HardWareErr;
volatile uint16_t WaitCmdTimer;



/*********************************************************************************************************
** Function name:       CreateMBox
** Descriptions:        为任务之间通信创建邮箱和信号量
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void CreateMBox(void)
{
	
}



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
	InitKeyboard();//维护按键初始化
	EnableKeyBoard();
	InitDisplay();
	LED_showString("EU1.4");//显示版本号
	msleep(1500);		
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
		MDB_coinEnable(0);
		MDB_billEnable(0);
		MN_userMenu();
		
	}
	else if(g_enterMenu == 2){
		MDB_coinEnable(0);
		MDB_billEnable(0);
		MN_adminMenu();
	}
	
	if(g_enterMenu){
		if(state == TRADE_FAULT){//退出维护选择显示页面
			//DispErrPage();
			MDB_coinEnable(0);
			MDB_billEnable(0);
		}	
		else{
			//DispFreePage(hopperIsEmpty());
			MDB_coinEnable(1);
			MDB_billEnable(1);
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
void MT_checkDev(void)
{
	uint8 s;
	s = MDB_getBillStatus();
	s = MDB_getBillErrNo();
	if(s > 0){ //纸币器有故障
		
	}
}





/*********************************************************************************************************
** Function name:       MT_checkPayout
** Descriptions:        检查是否可找零
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8 MT_checkPayout(uint32 amount)
{
	return 0;
}



/*********************************************************************************************************
** Function name:       CreateMBox
** Descriptions:        为任务之间通信创建邮箱和信号量
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void setPayKey(const unsigned char flag)
{

	static unsigned char on = 0;
	on = flag;
	
#if 0
	if(flag >= 2)
	{
		FIO0DIR |= (1ul << 30);
		FIO0CLR	|= (1ul << 30);
		on = 0;
		Trace("setPayKey:%d\r\n",flag);
	}
	else
	{
		if(on != flag)
		{
			Trace("setPayKey:%d\r\n",flag);
			FIO0DIR |= (1ul << 30);
			if(flag == 1)
				FIO0SET |= (1ul << 30);
			else
				FIO0CLR	|= (1ul << 30);

			on = flag;
		}		
	}
#endif	

}



/*********************************************************************************************************
** Function name:       hopperIsEmpty
** Descriptions:        hopper缺币 
** input parameters:    
** output parameters:   无
** Returned value:      1 缺币  0 正常
*********************************************************************************************************/
static uint8 hopperIsEmpty(void)
{
	uint8 i;
	for(i = 0;i < 3;i++){
		if(stHopper[i].ch > 0 && stHopper[i].state != HP_STATE_QUEBI){
			return 0;
		}
	}
	return 1;
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
	uint8 err,billtype=0,hopperState = 0,hasBuy = 0,rcx;
	uint32 iouMoney;
	uint8 state = 0;
	static uint32 remainAmount = 0,curBillAmount = 0,curCoinAmount = 0;//当前剩余金额invalue = 0,isBuyGoods = 0,HpOutMoney=0,
	
	state = TRADE_BEGIN;
	disp_clear_screen();
	//主流程
	while(1){
		switch(state){
			case TRADE_BEGIN://起始流程
				remainAmount = 0;
				curBillAmount = 0;
				curCoinAmount = 0;		
				hasBuy = 0;
				Timer.usr_opt = 1000; //10s
				MDB_coinEnable(1);
				MDB_billEnable(1);
				state = TRADE_DISP;
				break;
			case TRADE_DISP://显示欢迎光临
				disp_free_page(hopperIsEmpty());
				state = TRADE_BILLIN;
				break;
			case TRADE_BILLIN:
				//接收纸币
				curBillAmount = MDB_getBillRecvAmount();
				//接收硬币
				curCoinAmount = MDB_getCoinRecvAmount();
				//总接收金额
				remainAmount = curBillAmount + curCoinAmount;
				if(remainAmount > 0){ // 有金额进入找零环节
					//显示金额
					if(MT_checkPayout(remainAmount)){ //检查是否可找零
						state = TRADE_HPOUT;
					}
				}
				else{ //空闲状态 
					MT_checkMenu(state); //检测按键
					MT_checkDev();	//检测设备状态
				}
				
				break;
			case TRADE_HPOUT://找零
				DEV_payoutReady(curBillAmount,curCoinAmount);//正在找零
				while(!DEV_isPayoutFinish()){
					msleep(100);	
				}
				break;
			default:
				break;
		}
		
		OSTimeDly(10);
	}	



}
