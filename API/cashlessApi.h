#ifndef _CASHLESS_API_H_
#define _CASHLESS_API_H_

#include "..\Drivers\board.h"

#define CARD_JUST_RESET					0x0001UL
#define CARD_BEGIN_SESSION				0x0002UL   //���Ѳ�����뽻��
#define CARD_SESSION_CANCEL_REQUEST		0x0004UL   //ȡ������
#define CARD_VEND_APPROVED				0x0008UL   //������㹻������������
#define CARD_VEND_DENIED				0x0010UL   //�����������ؾܾ�����
#define CARD_END_SESSION				0x0020UL   //������session,���ؿ���ʹ��״̬	
#define CARD_MALFUNCTION_ERROR			0x0040UL	//����
#define CARD_DISABLE					0x0080UL	//����


#define CARD_TRADE_IDLE					0
#define CARD_TRADE_BEGIN				1
#define CARD_TRADE_REQ					2
#define CARD_TRADE_SUC					3
#define CARD_TRADE_FAIL					4
#define CARD_TRADE_END					5





typedef struct {
	//��������ز���
	uint8  level;			  //�������ȼ�
	uint8  scale;			  //��������
	uint8  recTime;		  //����Ӧʱ��
	uint8  miscelOpt;	  //�Ƿ�֧���������ܣ����������˻�Ǯ�ȵ�
	uint8  cashSale;	  //1�����ֽ���ϸ���׼�¼���ύ����
	uint16 code;		  //���Ҵ���
	uint16 decimal;		  //10^С��λ��
	uint16 status;			//��ǰ״̬
	
	uint8 tradeStatus;	//��ǰ����״̬
	uint32 cost;
}MDB_CARD;






typedef struct{
	uint8 type;
	uint8 paymentType;
	uint16 s;
	uint32 recvAmount;
	uint32 paymentId;
}CARD_POLL;


extern MDB_CARD stCard;


uint8 cardTaskPoll(void);

#endif


/**************************************End Of File*******************************************************/
