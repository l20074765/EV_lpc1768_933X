#ifndef _HOPPER_API_H_
#define _HOPPER_API_H_
#include "..\Drivers\board.h"

//��ǰֻ��Hopper��ָ��������л����Ĳ���ָ���
#define HP_QUERY				(0x51)				//Hopper�Ĳ�ѯ����ָ��
#define HP_PAYOUT				(0x50)				//Hopper�ĳ��Ҳ���ָ��


#define HP_SUM              	8  		//����hopper���� ���֧��8��

#define HP_PAYFAIL_NUM         	1   	//����hopper�����ֵ�����ʧ�ܴ���


#define HP_STATE_NORMAL			0		//hopper����
#define HP_STATE_QUEBI			1   	//hopperȱ��
#define HP_STATE_FAULT			2		//hopper����
#define HP_STATE_COM			3 		//hopper ͨ�Ź���








//����hopper�� �ṹ��
typedef struct __st_hopper_{
    uint8 addr;			//hopper �����ַ
    uint8 no;			//hopper���
    uint8 state;		//hopper״̬  0 ���� 1 ȱ�� 2 ����
	uint8 level;		//��������־
	uint8 isCycle;		//ѭ������־
	uint8 lastPayFail;	//�ϴ�����ʧ��
	uint8 err_com;			//ͨ�Ŵ������
	uint16 lastCount;	//�ϴβ�����������
	uint32 ch;			//ͨ����ֵ
	
}ST_HOPPER;
extern ST_HOPPER stHopper[HP_SUM];

//����hopper���� �ṹ������ ���������λ��ּ��� 1 > 2 > 3
//���� hopper1 hopper2 ͨ����ֵ 1Ԫ hopper3 ͨ����ֵΪ 5ë �� hopper1 hopper2
//�����ڼ���1��,hopper3 �����ڼ���2��  ����3Ϊ��  ��
typedef struct _st_hopper_level_{
    uint8 		num;//������hopper����
    uint32 		ch;//������ͨ����ֵ
	ST_HOPPER 	*hopper[HP_SUM];//����hopper���ṹ��ָ������
}ST_HOPPER_LEVEL;
extern ST_HOPPER_LEVEL stHopperLevel[HP_SUM];


void HP_task(void);
uint32 HP_payout(uint32 payAmount);
void HP_init(void);
uint8 HP_setCh(uint8 no,uint32 value);
uint8 hopperChangerFailCheck(const uint32 remainAmount);
uint8 HP_payout_by_addr(ST_HOPPER *hopper,uint16 num);
uint8 HP_send_check(ST_HOPPER *hopper);
#endif


/**************************************End Of File*******************************************************/
