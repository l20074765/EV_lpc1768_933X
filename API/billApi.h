#ifndef _BILL_API_H
#define _BILL_API_H
#include "..\Drivers\board.h"

#define BILL_BIT_FAULT						(0x01UL) 		  //����
#define BILL_BIT_DISABLE					(0x01UL << 1) //����λ
#define BILL_BIT_QUEBI						(0x01UL << 2) //ȱ��λ
#define BILL_BIT_BUSY						(0x01UL << 3) //Ӳ����æλ
#define BILL_BIT_RESET						(0x01UL << 4)
#define BILL_BIT_PAYBACK					(0x01UL << 7) //�˱�λ




#define BILL_ERR_COM					0x0001UL //ͨ�Ź���
#define BILL_ERR_SENSOR					0x0002UL //����������
#define BILL_ERR_TUBEJAM				0x0004UL //���ҿڿ���
#define BILL_ERR_ROM					0x0008UL //rom����
#define BILL_ERR_ROUTING				0x0010UL //����ͨ������
#define BILL_ERR_JAM					0x0020UL //Ͷ�ҿ���
#define BILL_ERR_REMOVECASH				0x0040UL //�Ƴ�ֽ�ҳ���
#define BILL_ERR_DISABLE				0x0080UL //����
#define BILL_ERR_MOTO					0x0100UL //������
#define BILL_ERR_CASH					0x0200UL //ֽ�ҳ������
#define BILL_ERR_UNKNOW					0x8000UL //�������ֹ���


#define BILL_POLL_N_A					0x0000 //�޶���
#define BILL_POLL_ESCROW_POSITION		0x0001 //Ѻ��
#define BILL_POLL_STACKED				0x0002 //�ݴ�
#define BILL_POLL_RETURNED				0x0004 //ֽ�ұ��˻�
#define BILL_POLL_DISABLED_REJECTED		0x0008 //
#define BILL_POLL_RECYCLER				0x0010 //ֽ�ҽ���ѭ����
#define BILL_POLL_RECYCLER_MANUAL		0x0020 //�ֶ���ֽ�ҽ���ѭ����
#define BILL_POLL_TRANSFERRED			0x0080 //ֽ�Ҵ�ѭ����ת�Ƶ�����		


typedef struct _bill_poll_{
	uint8 	s;
	uint8  	type; 
	uint16 	err;
	uint16 	routing;	
}BILL_POLL;


typedef struct _bill_id_{
	int8 	manuCode[3];			//���Ҵ��� ASCII��
	int8 	sn[12];				//ASCII��
	int8 	model[12];				//ASCII��
	int8 	softVer[2];			//����汾 BCD
	uint32 	feature;
}BILL_ID;


typedef struct _bill_setup_{
	uint8  	level; 			 //ֽ�����ȼ�
	uint8  	escrowFun;		 //�ݴ湦��
	uint8  	isRecycler;		//�Ƿ�֧�� ѭ����
	uint16 	codel;		  	 //���Ҵ���
	uint32 	scale;			 //��������
	uint16 	decimal;		     //10^С��λ��
	uint32 	rato;
	uint16 	stkCapacity;	    //��������
	uint16 	security;		//��ȫ�ȼ�
	uint32  ch[16]; 		//ͨ����ֵ
}BILL_SETUP;


typedef struct _bill_state_{
	// bit0 ��״̬: 	0-���� 		1-����  
	// bit1 ����״̬: 	0-ʹ�� 		1-���� 
	uint8 	status;  	//����״̬
	uint16 	errNo; 		//������
}BILL_STATE;


typedef struct _bill_amount_{
	uint32 recv_amount;
	uint32 remain_amount;
}BILL_AMOUNT;


#define BILL_DISPENSE_IDLE 		0
#define BILL_DISPENSE_RUNING	1
#define BILL_DISPENSE_FINISH	2


typedef struct _bill_recycler_{
	uint16 	routing;
	uint16  en;
	uint16	full;
	uint16  ch_count[16];
	uint16  pay_num[16];
	uint32  ch[16];
}BILL_RECYCLER;




typedef struct	_st_dev_bill_{
	
	BILL_STATE 	s;
	BILL_AMOUNT amount;
	BILL_SETUP 	setup;
	BILL_ID 	id;
	BILL_RECYCLER recycler; //������
	
}ST_DEV_BILL;

extern ST_DEV_BILL xdata stBill;


uint8 	billInit(void);
uint8 	bill_enable(uint8 flag);
uint32 	bill_recycler_payout_by_value(uint32 amount);
uint8 	billTaskPoll(void);
#endif
