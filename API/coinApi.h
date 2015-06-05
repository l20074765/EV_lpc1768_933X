#ifndef _COIN_API_H
#define _COIN_API_H

#include "..\Drivers\board.h"

#define COIN_BIT_FAULT						0x01UL 	//����
#define COIN_BIT_DISABLE					(0x01UL << 1) //����λ
#define COIN_BIT_QUEBI						(0x01UL << 2) //ȱ��λ
#define COIN_BIT_BUSY						(0x01UL << 3) //Ӳ����æλ
#define COIN_BIT_RESET						(0x01UL << 4) //Ӳ������λ
#define COIN_BIT_OK							(0x01UL << 6) //Ӳ�����ָ�
#define COIN_BIT_PAYBACK					(0x01UL << 7) //�˱�λ


#define COIN_ERR_COM					0x0001UL //ͨ�Ź���
#define COIN_ERR_SENSOR					0x0002UL //����������
#define COIN_ERR_TUBEJAM				0x0004UL //���ҿڿ���
#define COIN_ERR_ROM					0x0008UL //rom����
#define COIN_ERR_ROUTING				0x0010UL //����ͨ������
#define COIN_ERR_JAM					0x0020UL //Ͷ�ҿ���
#define COIN_ERR_REMOVETUBE				0x0040UL //�Ҷ��ƿ�
//#define COIN_ERR_DISABLE				0x0100UL //���ܹ���
#define COIN_ERR_UNKNOW					0x8000UL //��������







typedef struct _coin_tube_{
	uint16 full;
	uint8  num[16];
	uint16 routing;
	uint8  credit[16];
	uint32 ch[16];//Ӳ��ͨ����ֵ ��Ϊ��λ
}COIN_TUBE;


typedef struct _coin_identifict_{
	int8 manufacturerCode[3];	//���Ҵ��� ASCII
	int8 sn[12];							//ASCII
	int8 model[12];						//ASCII
	int8 softwareVersion[2];	//BCD
	uint8 feature[4];

}COIN_IDENTIFICT;


typedef struct _coin_state_{
	// bit0 ��״̬: 	0-���� 		1-����  
	// bit1 ����״̬: 	0-ʹ�� 		1-���� 
	// bit7 �˱Ұ�ť:   0-�Ǵ���	1-����
	uint8  s;	
	uint16 err;	 //	
}COIN_STATE;

typedef struct _coin_amount{
	uint32 recv_amount;
	uint32 remain_amount;
	uint8 payout;
}COIN_AMOUNT;



typedef struct _st_dev_coin_{	
	uint8  type;
	uint8  level;				
	uint8  scale;			 	
	uint8  codel;			 	
	uint8  decimal;		 		
	uint16  rato;	

	COIN_STATE state;	
	COIN_TUBE tube;
	COIN_IDENTIFICT identifict;
	COIN_AMOUNT amount;
}ST_DEV_COIN;

extern ST_DEV_COIN xdata stCoin;

uint32 coinPayout(uint32 payMoney);
uint8 coin_enable(uint8 flag) ;
uint8 coinInit(void);
uint8 coinTaskPoll(void);


#endif
