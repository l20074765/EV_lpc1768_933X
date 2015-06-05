#ifndef _GLOBAL_H
#define _GLOBAL_H


#define VMC_TRACE_ON					1
#define VMC_TRACE_OFF					0
#define VMC_TRACE_FLAG					VMC_TRACE_ON //���Կ���
#define VMC_OPEN_COINFG					0 //�Ƿ��������ļ�1����  0�ر�


//���������
//Ӳ������������
#define  VMC_COIN_SERIAL	1
#define  VMC_COIN_PARALLEL  2
#define  VMC_COIN_NO		0//��Ӳ����
#define  VMC_COIN_TYPE     VMC_COIN_SERIAL


//������ͨ����ֵ
#define VMC_COIN_CHANNEL_1	100
#define VMC_COIN_CHANNEL_2	0
#define VMC_COIN_CHANNEL_3	0
#define VMC_COIN_CHANNEL_4	0
#define VMC_COIN_CHANNEL_5	0
#define VMC_COIN_CHANNEL_6	0
#define VMC_COIN_CHANNEL_7	0
#define VMC_COIN_CHANNEL_8	0

//ֽ����MDB

#define VMC_BILL_NO			0	//��ֽ����
#define VMC_BILL_RS232		1 	//GBA
#define VMC_BILL_MDB		2	//MDB
#define VMC_BILL_MDB_ITL	3	// MDB ITL

#define VMC_BILL_TYPE     VMC_BILL_MDB

//������

#define VMC_CHANGER_HOPPER		1
#define VMC_CHANGER_NO			0//��������
#define VMC_CHANGER_TYPE        VMC_CHANGER_HOPPER

//����hopper���� ����
#define HOPPER_LEVEL_RATIO			 {2,1,0} //��hopper��Ҫ��Ӧ����hopper��ֵ�� �Ӵ�С 
#define HOPPER_BASE_CHANGED			  500   //��������Է�Ϊ��λ

//������ͨ����ֵ
#define VMC_CHANGER_CHANNEL_1	100
#define VMC_CHANGER_CHANNEL_2	200
#define VMC_CHANGER_CHANNEL_3	0




//С����λ��
#define VMC_PIONT_0					0
#define VMC_PIONT_1					1
#define VMC_PIONT_2					2


#define VMC_PIONT_TYPE      VMC_PIONT_1

//������������ļ�
#define VMC_HUODAO_OPEN_CONFIG     1 //�Ƿ��������ļ�����
#define VMC_HUODAO_LEVEL_OPEN     {1,0} //����㿪��

//�����������
#define VMC_HUODAO_COLUMN_OPEN    {{1,0,0,1,1,1,0,0},   \
								   {0,0,0,0,0,0,0,0} }



//======================�������ý���=======================================================





//�Ƿ���Hopper���㹦��
#define HOPPER_OPEN						(3)
//�Ƿ���ֽ�����ձҹ���
#define BILL_OPEN						(4)

extern uint8_t GoToMenuFlag;
extern uint32_t nMinDispMoney;
extern unsigned char debatflag;

extern unsigned int MaxColumnMoney;

extern OS_EVENT *g_KeyMsg;









#endif
