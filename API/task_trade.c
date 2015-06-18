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
  ����ȫ�ֱ���
*********************************************************************************************************/
#if 1
#define TRADE_BEGIN						(0x01)				//
#define TRADE_DISP						(0x02)				//��ʾ��Ϣ
#define TRADE_HPOUT						(0x03)				//����
#define TRADE_SAVETRADE					(0x04)				//���潻�׼�¼
#define TRADE_BILLIN					(0x05)				//����Ƿ���ֽ�һ�Ӳ�ҽ���
#define TRADE_ISCANOUTGOODS				(0x06)				//����Ƿ���Գ���
#define TRADE_TRADEOVER					(0x07)				//һ�ν����������
#define TRADE_WEIHU						(0x08)				//����ά��ģʽ
#define TRADE_CHECKCHANNEL				(0x12)				//��ѯ����
#define TRADE_OUTGOODS					(0x13)				//����
#define TRADE_SELECTGOODS				(0x14)				//�Ƿ���ѡ������
#define TRADE_CHECKDEVICEERR			(0x15)				//����豸�Ƿ����
#define TRADE_STOPSERVICE				(0x16)				//ֹͣ����


#define TRADE_FAULT						(0x17)				//����
#endif



static volatile uint8 g_enterMenu = 0;//ά��ģʽ��־


volatile unsigned int TestPluse;


uint32_t nMinDispMoney = 0;
unsigned int MaxColumnMoney=0;


extern uint16_t HardWareErr;
volatile uint16_t WaitCmdTimer;


volatile static uint8 g_billRatio = 0,g_coinRatio = 0;




/*********************************************************************************************************
** Function name:       SystemInit
** Descriptions:        ϵͳ��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void SystemInit(void)
{
	//ע�⸴λ��Uart0ʹ�ܣ�Uart2/3������
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
	LED_showString("EU1.4");//��ʾ�汾��
	msleep(1500);	
	LED_showString("E---");
	InitKeyboard();//ά��������ʼ��
	EnableKeyBoard();
	LED_showString("E--1");
	msleep(100);	
}





/*********************************************************************************************************
** Function name:       SystemParaInit
** Descriptions:        ϵͳ������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
** Descriptions:        ����Ƿ����ά��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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
		if(state == TRADE_FAULT){//�˳�ά��ѡ����ʾҳ��
			DEV_enableReq(OBJ_ALL,0);
		}	
		else{
			DEV_enableReq(OBJ_ALL,1);
		}	
		g_enterMenu = 0;//����ά�� ���־
		
		return 1;
	}
	return 0;
}



/*********************************************************************************************************
** Function name:       MT_checkDev
** Descriptions:        ����豸
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 MT_checkDev(void)
{
	uint16 errNo = 0;
	uint8 i,hpIsEmpty,hpIsFault,hpUsedNum;
	
	errNo = MDB_getBillErrNo();
	if(errNo > 0){ //ֽ�����й���
		LED_showString("BLEE");
		return 1;
	}
	
	//���hopper ȫ���� ���� ȫȱ��
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

	return 0;
}





/*********************************************************************************************************
** Function name:       MT_checkPayout
** Descriptions:        ����Ƿ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 MT_checkPayout(uint32 billAmount,uint32 coinAmount)
{
	uint8 i;
	#if 0
	for(i = 0;i < HP_SUM;i++){
		if(stHopperLevel[i].ch == 0){
			if(i != 0){
				if(coinAmount <= stHopperLevel[i - 1].ch){ //Ӳ�Ҳ�������
					
					return 0;
				}
			}
		}
	}
	#endif
	g_billRatio = 0;
	g_coinRatio = 0;
	for(i = 0; i < 8;i++){
		if(billAmount == stMdb.billRato[i].amount){
			g_billRatio = i + 1;
			break;
		}
	}
	for(i = 0; i < 8;i++){
		if(coinAmount == stMdb.coinRato[i].amount){
			g_coinRatio = i + 1;
			break;
		}
	}

	return 1;
}



/*********************************************************************************************************
** Function name:       CreateMBox
** Descriptions:        Ϊ����֮��ͨ�Ŵ���������ź���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
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



/*********************************************************************************************************
** Function name:       trade_task_changer
** Descriptions:        �������������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void task_trade(void)
{
	uint8 state = 0;
	static uint32 remainAmount = 0,curBillAmount = 0,curCoinAmount = 0;
	Q_MSG *msg;
	state = TRADE_BEGIN;
	disp_clear_screen();
	DEV_msgRpt(DEV_INIT,500000);	
	
	//������
	while(1){
		switch(state){
			case TRADE_BEGIN://��ʼ����
				remainAmount = 0;
				curBillAmount = 0;
				curCoinAmount = 0;		
				
				Timer.usr_opt = 1000; //10s
				DEV_enableReq(OBJ_ALL,1);
				state = TRADE_DISP;
				break;
			case TRADE_DISP://��ʾ��ӭ����
				LED_showAmount(0);
				state = TRADE_BILLIN;
				break;
			case TRADE_BILLIN:
				curBillAmount = MDB_getBillRecvAmount();//����ֽ��
				curCoinAmount = MDB_getCoinRecvAmount();//����Ӳ��
				remainAmount = curBillAmount + curCoinAmount;//�ܽ��ս��
				LED_showAmount(remainAmount);
				if(remainAmount > 0){ // �н��������㻷��
					//��ʾ���
					print_main("remainAmount1=%d\r\n",remainAmount);
					if(MT_checkPayout(curBillAmount,curCoinAmount) > 0){ //����Ƿ������
						state = TRADE_HPOUT;
					}
				}
				else{ //����״̬ 
					MT_checkMenu(state); //��ⰴ��
					if(MT_checkDev() > 0){	//����豸״̬
						state = TRADE_FAULT;
						DEV_enableReq(OBJ_ALL,0);
					}
				}
				
				break;
			case TRADE_HPOUT://����
				msg = DEV_getReqMsg();
				msg->billRatioIndex = g_billRatio;
				msg->coinRatioIndex = g_coinRatio;
				DEV_payoutReq(curBillAmount,curCoinAmount);//��������
			
				msg = DEV_msgRpt(DEV_PAYOUT,900000); //�ȴ�������
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
