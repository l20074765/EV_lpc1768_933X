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


void MT_devInit(void)
{
	uint8 res,type,i;
	//��ʼ��Ӳ����
	
	type = MDB_getCoinAcceptor();
	if(type == COIN_ACCEPTOR_PPLUSE){
		LED_show("CO--");
		PCOIN_initParallelPluse(stMdb.highEnable);
		res = 1;
		LED_show("CO-1%d",res);
	}
	else if(type == COIN_ACCEPTOR_SPLUSE){
		LED_show("CO--");
		PCOIN_initSerialPluse(stMdb.highEnable);
		res = 1;
		LED_show("CO-1%d",res);
	}
	msleep(100);
	
	
	type = MDB_getCoinDispenser();
	if(type == COIN_DISPENSER_HOPPER){
		LED_show("HP--");
		HP_init();
		msleep(500);
		for(i = 0;i < HP_SUM;i++){
			if(stHopper[i].ch > 0){
				LED_show("HP%d-",i + 1);
				res = HP_send_check(&stHopper[i]);
				if(res == 1){
					LED_show("HP%d%d",i + 1,1);
					msleep(500);
				}
				else{
					LED_show("HP%d%d",i + 1,0);
					msleep(1500);
				}
			}
		}
	}
	msleep(100);

	type = MDB_getBillAcceptor();
	if(type == BILL_ACCEPTOR_MDB){
		stBill.s.status |= BILL_BIT_FAULT;
		stBill.s.errNo |= BILL_ERR_COM;
		LED_show("BL--");	
		for(i = 0;i < 3;i++){
			res = billInit();
			if(res == 1){
				stBill.s.status &= ~BILL_BIT_FAULT;
				stBill.s.errNo = 0;
				LED_show("BL-1");
				break;
			}
			else{
				msleep(500);
			}
		}
		
		if(i >= 3){
			LED_show("BL-0");
			msleep(2000);
		}
	}
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
		if(state == TRADE_FAULT){//�˳�ά��ѡ����ʾҳ��
			//DispErrPage();
			MDB_coinEnable(0);
			MDB_billEnable(0);
		}	
		else{
			//DispFreePage(hopperIsEmpty());
			MDB_coinEnable(1);
			MDB_billEnable(1);
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
void MT_checkDev(void)
{
	uint8 s;
	s = MDB_getBillStatus();
	s = MDB_getBillErrNo();
	if(s > 0){ //ֽ�����й���
		
	}
}





/*********************************************************************************************************
** Function name:       MT_checkPayout
** Descriptions:        ����Ƿ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 MT_checkPayout(uint32 amount)
{
	
	return 0;
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



/*********************************************************************************************************
** Function name:       hopperIsEmpty
** Descriptions:        hopperȱ�� 
** input parameters:    
** output parameters:   ��
** Returned value:      1 ȱ��  0 ����
*********************************************************************************************************/
static uint8 hopperIsEmpty(void)
{
	uint8 i;
	for(i = 0;i < HP_SUM;i++){
		print_main("[%d].ch=%d s = %d\r\n",i,stHopper[i].ch,stHopper[i].state);
		if(stHopper[i].ch > 0 && stHopper[i].state != HP_STATE_QUEBI){
			return 0;
		}
	}
	return 1;
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
	uint8 err,billtype=0,hopperState = 0,hasBuy = 0,rcx;
	uint32 iouMoney = 0;
	uint8 state = 0;
	static uint32 remainAmount = 0,curBillAmount = 0,curCoinAmount = 0;
	
	state = TRADE_BEGIN;
	disp_clear_screen();
	//������
	while(1){
		switch(state){
			case TRADE_BEGIN://��ʼ����
				remainAmount = 0;
				curBillAmount = 0;
				curCoinAmount = 0;		
				hasBuy = 0;
				Timer.usr_opt = 1000; //10s
				MDB_coinEnable(1);
				MDB_billEnable(1);
				state = TRADE_DISP;
				break;
			case TRADE_DISP://��ʾ��ӭ����
				LED_showAmount(0);
				state = TRADE_BILLIN;
				break;
			case TRADE_BILLIN:
				//����ֽ��
				curBillAmount = MDB_getBillRecvAmount();
				//����Ӳ��
				curCoinAmount = MDB_getCoinRecvAmount();
				//�ܽ��ս��
				remainAmount = curBillAmount + curCoinAmount;
				if(remainAmount > 0){ // �н��������㻷��
					//��ʾ���
					LED_showAmount(remainAmount);
					if(MT_checkPayout(remainAmount) > 0){ //����Ƿ������
						state = TRADE_HPOUT;
					}
				}
				else{ //����״̬ 
					MT_checkMenu(state); //��ⰴ��
					MT_checkDev();	//����豸״̬
					if(hopperIsEmpty()){
						LED_showString("HP00");
					}
					else{
						LED_showAmount(0);
					}
				}
				
				break;
			case TRADE_HPOUT://����
				DEV_payoutReady(curBillAmount,curCoinAmount);//��������
				while(!DEV_isPayoutFinish()){
					msleep(100);	
				}
				break;
			default:
				break;
		}
		
		msleep(50);
	}	



}
