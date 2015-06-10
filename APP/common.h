/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ϵͳ�������弰ͨ�ú�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#ifndef __COMMON_H 
#define __COMMON_H

#define print_log(...)


#define MENU_LCD_SHOW   1 //�Ƿ�ѡ��LCD��ʾ�˵�


/******************************************��Ļ��ʾ����*****************************************************/
#define LINE1                    0
#define LINE2                    1
#define LINE3                    2
#define LINE4                    3
#define LINE5                    4
#define LINE6                    5
#define LINE7                    6
#define LINE8                    7
#define LINE9                    8
#define LINE10                   9
#define LINE11                   10
#define LINE12                   11
#define LINE13                   12
#define LINE14                   13
#define LINE15                   14
#define LINE16                   15

#define MAXLANGUAGETYPE 5 //ϵͳ֧�������������࣬Ŀǰ�ݶ�5�֣����ģ�Ӣ�ģ������䣬����˹��������




//==============================�����йغ궨��========================================================

#define HUODAO_TYPE_ISOPEN   1
#define HUODAO_TYPE_STATE   2
#define HUODAO_TYPE_REMAIN   3
#define HUODAO_TYPE_PRICE   4
#define HUODAO_TYPE_TRADE_NUM   5
#define HUODAO_TYPE_TRADE_MAXCOUNT 6
#define HUODAO_TYPE_TRADE_LOGICNO 7
//======================================================================================




/************************************************************************************
*�豸״̬�ṹ��
***************************************************************************************/
typedef struct _st_dev_state_{
		unsigned char billState; // 1���� 0����
		unsigned char coinState;
		unsigned short hopperState;

}ST_DEV_STATE;

extern ST_DEV_STATE stDevState;

/************************************************************************************
*�ܽ������ݽṹ��
***************************************************************************************/
typedef struct _st_total_trade_sum_
{
	//����ֽ���ܽ��
	unsigned int BillMoneySum;
	//����Ӳ���ܽ��
	unsigned int CoinMoneySum;
	//�����ܽ��
	unsigned int DispenceSum;
	//00��ַHopper����������
	unsigned int Hopper1DispSum;
	//01��ַHopper����������
	unsigned int Hopper2DispSum;
	//10��ַHopper����������
	unsigned int Hopper3DispSum;
	unsigned int tradePageNo;//����ҳ�� 0 ��ʾû�н��׼�¼ 
	unsigned int tradeNums;//�ܽ�������
	unsigned int iouAmount;//Ƿ�ѽ��
	unsigned int tradeAmount;//�����ܽ��


	
	
}ST_TOTAL_TRADE_SUM;

extern ST_TOTAL_TRADE_SUM stTotalTrade;




/************************************************************************************
*��ϸ�������ݽṹ��
***************************************************************************************/
typedef struct _detail_trade_info_{

	RTC_DATE tradeTime;//����ʱ��  
	unsigned int billIncome;//�������ձ�  
	unsigned int coinIncome;//��������Ӳ��
	unsigned int tradePayout;//����������
	unsigned char tradeNum;//���ʹ������  ���8��
	unsigned char columnNo[8];//������
	unsigned char tradeResult[8];//�������
	unsigned short tradePrice[8];//�������
	
	//8 + 12 + 1 + 16 + 16 =  	
}DETAIL_TRADE_INFO;




/************************************************************************************
*������Ϣ�ṹ��  changed by yoc
***************************************************************************************/

#define COLUMN_LEVEL_NUM   2   //��������Ĳ���
#define COLUMN_ONE_LEVEL_NUM 8//���嵥���������������



#define HUODAO_STATE_NORMAL		1 
#define HUODAO_STATE_EMPTY		3     
#define HUODAO_STATE_FAULT		2 
#define HUODAO_STATE_N_A		0  





/************************************************************************************
*���������� �밴����һһ��Ӧ
***************************************************************************************/
#if 0 //�����Ƹ�ʽ
#define HUODAO_SELCET_LED1	(1ul<<18)//��1	
#define HUODAO_SELCET_LED2	(1ul<<19)//��2	
#define HUODAO_SELCET_LED3	(1ul<<20)//��3	
#define HUODAO_SELCET_LED4	(1ul<<31)//��4

#else //��ʽ���Ƹ�ʽ

#define HUODAO_SELCET_LED1	(1ul<<31)//��1	
#define HUODAO_SELCET_LED2	(1ul<<18)//��2	
#define HUODAO_SELCET_LED3	(1ul<<19)//��3	
#define HUODAO_SELCET_LED4	(1ul<<20)//��4	


#endif





struct STChannel
{
	unsigned char isOpen;//����������־ 1���� 0�ر�
	unsigned char addr;	//����ʵ�������ַ
	unsigned char physicNo;//����������
	unsigned char logicNo;//�������߼����		
	unsigned char state;//����״̬ :1��������  3����ȱ��   2��������	
	unsigned char selectNo;//����ѡ��������		
	
	unsigned char remainCount;//��Ʒ�������
	unsigned char maxCount;//��󴢻���
	unsigned int  price;//������Ʒ����
	unsigned int  tradeCount;//�����ɹ����״���
	unsigned char canShip;//���Գ�����־ 1��ʾ���Գ��� 0 ��ʾ���ܳ���
};

typedef struct _st_column_level_{
	
	unsigned char isOpen;
	struct STChannel stColumn[COLUMN_ONE_LEVEL_NUM];
		
}ST_COLUMN_LEVEL;

extern ST_COLUMN_LEVEL stHuodao[COLUMN_LEVEL_NUM];//����������� �ṹ��


//======================================================================================


//================================ϵͳ������======================================================

#define SYS_ERR_NO_NORMAL  (unsigned short)(0)  //����
#define SYS_ERR_NO_HOPPER  (unsigned short)(0x01 << 0)  //Hopper����
#define SYS_ERR_NO_BILL    (unsigned short)(0x01 << 1)  //ֽ��������
#define SYS_ERR_NO_HUODAO_PRICE   (unsigned short)(0x01 << 2)  //�����Լ�ȫ����������Ϊ��
#define SYS_ERR_NO_HUODAO_EMPTY   (unsigned short)(0x01 << 3)  //����ϵͳ�����еĴ�����ȫΪ0
#define SYS_ERR_NO_HUODAO_FAULT  (unsigned short)(0x01 << 4)  //�����Լ�ȫ������������
#define SYS_ERR_NO_COIN			(unsigned short)(0x01 << 5)  //Ӳ��������
#define SYS_ERR_NO_CHANGER_FAULT (unsigned short)(0x01 << 6)  //��hopper����������

//================================hopper������======================================================

#define HP_ERR_NO_NORMAL  (unsigned short)(0)//����
#define HP_ERR_NO_HOPPER1_FAULT  (unsigned short)(0x01 << 0)//hopper1����
#define HP_ERR_NO_HOPPER1_EMPTY  (unsigned short)(0x01 << 1)//hopper1ȱ��
#define HP_ERR_NO_HOPPER2_FAULT  (unsigned short)(0x01 << 2)//hopper2����
#define HP_ERR_NO_HOPPER2_EMPTY  (unsigned short)(0x01 << 3)//hopper2ȱ��
#define HP_ERR_NO_HOPPER3_FAULT  (unsigned short)(0x01 << 4)//hopper3����
#define HP_ERR_NO_HOPPER3_EMPTY  (unsigned short)(0x01 << 5)//hopper3ȱ��
#define HP_ERR_NO_HOPPER4_FAULT  (unsigned short)(0x01 << 6)//hopper4����
#define HP_ERR_NO_HOPPER4_EMPTY  (unsigned short)(0x01 << 7)//hopper4ȱ��


//hopper������
extern unsigned int hopperErrNo;
//�ۻ���������
extern uint16_t HardWareErr;







//======================================================================================









void setColumnLedON(unsigned int price);


char *PrintfMoney(uint32_t dispnum);


extern unsigned short paomaLedTimer;

void idlePaoMaLed(void);

#endif
/**************************************End Of File*******************************************************/
