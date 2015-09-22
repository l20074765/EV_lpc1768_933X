#include "..\config.h"
#include "cashlessApi.h"

#define DEBUG_CARD

#ifdef 	DEBUG_CARD
#define print_card(...)	Trace(__VA_ARGS__)
#else
#define print_card(...)
#endif

#define MDB_CARD1_ADDR			0x10  //MDB������1�豸��ַ
#define MDB_CARD2_ADDR			0x60
#define MDB_CARD_ADDR			0x10	//��ǰ�������豸��Ч��ַ


#define MDB_CARD_RESET			0x00   		//��λ
#define MDB_CARD_SETUP			0x01   		//��ʼ��
#define MDB_CARD_VEND_REQUEST	0x03   		//����
#define MDB_CARD_POLL			0x02				//��ѵ
#define MDB_CARD_ENBALE			0x04   		//




static uint8 xdata recvbuf[36] = {0};
static uint8 xdata recvlen = 0;

static uint8 volatile card_reqeusted = 0;

MDB_CARD stCard;


/*********************************************************************************************************
** Function name:       card_send
** Descriptions:        ����������� 
** input parameters:    dev:�豸��(��5λ��ַ������λ����)��
**						*wdata:Ҫ���͵����� wdata��Ҫ�������ݵĳ���
** output parameters:   
** Returned value:      0:ͨ�ų�ʱ 1ͨѶ�ɹ� 2���մ��� 
*********************************************************************************************************/
static uint8 card_send(uint8 dev,uint8 *wdata,uint8 wlen)
{
	uint8  res;
	memset(recvbuf,0,sizeof(recvbuf));
	res = MDB_conversation(dev,wdata,wlen,recvbuf,&recvlen);
	return (res == 1) ?  1 : (res == 0 ? 0 : 2);
}


/*********************************************************************************************************
** Function name:       card_reset
** Descriptions:        �������豸��λ����
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_reset()
{
	uint8 res,addr;
	addr = MDB_CARD_ADDR + MDB_CARD_RESET;
	res = card_send(addr,NULL,0x00); //Reset
	return res;
}

/*********************************************************************************************************
** Function name:       card_setup
** Descriptions:        ��������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_setup(void)
{
	uint8 res,addr,buf[10] = {0},i;
	addr = MDB_CARD_ADDR + MDB_CARD_SETUP;
	
	buf[0] = 0x00;//Configuration data
	buf[1] = 0x01;//VMC Feature Level  level2
	buf[2] = 0x00;//Columns on Display
	buf[3] = 0x00;//Rows on Display
	buf[4] = 0x00;//Display Information	
	res = card_send(addr,buf,5); //setup
	
	if(res != 1){
		return 0;
	}
	
	stCard.level = recvbuf[1];
	stCard.code = INTEG16(recvbuf[2],recvbuf[3]);
	stCard.decimal = recvbuf[5];
	stCard.scale = recvbuf[4] * 100;
	if(stCard.scale == 0){
		print_card("stCard.scale == 0 stCard.scale == 0 stCard.scale == 0\r\n");
		return 0;
	}
    for(i = 0; i < stCard.decimal; i++) {
	   stCard.scale /= 10;
    }
	
	
	stCard.recTime = recvbuf[6];
	stCard.miscelOpt = recvbuf[7];
	if((stCard.miscelOpt >> 3) & 0x01) {//�����ֽ���ϸ���׼�¼���ύ��������
		stCard.cashSale = 1;
	}
	
	print_card("setup:level=%d,code=%x,decimal=%d,scale=%d,recTime=%d,miscelOpt=%d\r\n",
		stCard.level,stCard.code,stCard.decimal,stCard.scale,stCard.recTime,stCard.miscelOpt);
	
	return res;
}


/*********************************************************************************************************
** Function name:       card_setup
** Descriptions:        ��������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_setup_price(void)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_SETUP;
	
	buf[0] = 0x01;//Max / Min prices
	buf[1] = 0xFF;
	buf[2] = 0xFF;
	buf[3] = 0x00;
	buf[4] = 0x00;
	res = card_send(addr,buf,5); //setup
	
	if(res != 1){
		return 0;
	}
	else{
		return 1;
	}
	

}





/*********************************************************************************************************
** Function name:       card_setup
** Descriptions:        ��������ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_poll(CARD_POLL *poll)
{
	
	uint8 res,addr,i;
	uint8 temp,ch;
	addr = MDB_CARD_ADDR + MDB_CARD_POLL;
	res = card_send(addr,NULL,0); //setup
	if(res != 1){return 0;}
	poll->s = 0;
	for(i = 0;i < recvlen;){
		temp = recvbuf[i++];
		switch(temp){
			case 0x00: //JUST RESET
				poll->s |= CARD_JUST_RESET;
				break;
			case 0x01://Reader Config Info
				ch = recvbuf[i++]; //Reader Feature Level
				ch = recvbuf[i++]; //Country Code High
				ch = recvbuf[i++]; //Country Code Low
				ch = recvbuf[i++]; //Scale Factor
				ch = recvbuf[i++]; //Decimal Places
				ch = recvbuf[i++]; //Application Maximum Response Time
				ch = recvbuf[i++]; //Miscellaneous Options
				ch = ch;
				break;
			
			case 0x02: //DISPLAY REQUEST
				i = recvlen;
				break;
			case 0x03: //BEGIN SESSION  //��ȡ���
				if(stCard.level == 1){
					poll->s |= CARD_BEGIN_SESSION;
					poll->recvAmount = stCard.scale * INTEG16(recvbuf[i+1],recvbuf[i+2]);
					i += 2;
				}
				else if(stCard.level == 2 || stCard.level == 3){
					poll->s |= CARD_BEGIN_SESSION;
					poll->recvAmount = stCard.scale * INTEG16(recvbuf[i+1],recvbuf[i+2]);
					poll->paymentId = INTEG32(recvbuf[i+3],recvbuf[i+4],recvbuf[i+5],recvbuf[i+6]);
					poll->paymentType = recvbuf[i+7];
					ch = recvbuf[i+8];	
					ch = recvbuf[i+9];
					i += 9;
				}
				else{
					i = recvlen;
				}
				i = recvlen;
				
				break;
				
			case 0x04: //SESSION CANCEL REQUEST
				poll->s |= CARD_SESSION_CANCEL_REQUEST;
				
				break;
			
			case 0x05: //Approved ������㹻������������
				poll->s |= CARD_VEND_APPROVED;
				i+= 2;
				break;
			case 0x06://�����������ؾܾ�����
				poll->s |= CARD_VEND_DENIED;
				break;
			case 0x07: //������session,���ؿ���ʹ��״̬	
				poll->s |= CARD_END_SESSION;
				break;
			case 0x08:
				break;
			
			case 0x09:
				i = recvlen;
				break;
			
			case 0x0A:// ���� 
				poll->s |= CARD_MALFUNCTION_ERROR;
				i+= 1;
				break;
				
			case 0x0B: //ָ�����д���
				poll->s |= CARD_CMD_ERR;
				break;
			
			default:i = recvlen;break;
		}
	}
	
	return res;
}



/*********************************************************************************************************
** Function name:       card_vend_request
** Descriptions:        ����������������
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_vend_request(uint16 amount)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_VEND_REQUEST;
	
	buf[0] = 0x00;//VEND REQUEST
	buf[1] = HUINT16(amount);
	buf[2] = LUINT16(amount);
	buf[3] = 0x00;
	buf[4] = 0x01;
	res = card_send(addr,buf,5); //setup
	if(res != 1){return 0;}
	
	if(recvbuf[0] == 0x06){ //�ܾ�����
		return 3;
	}
	else{
		return 1;
	}
	
		
	
	
}

/*********************************************************************************************************
** Function name:       card_vend_cancel
** Descriptions:        ������ȡ����������
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_vend_cancel(void)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_VEND_REQUEST;
	
	buf[0] = 0x01;
	res = card_send(addr,buf,1); //setup
	if(res != 1){return 0;}
	return res;
}



/*********************************************************************************************************
** Function name:       card_vend_success
** Descriptions:        �������ɹ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_vend_success(void)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_VEND_REQUEST;
	
	buf[0] = 0x02;
	buf[1] = 0x00;
	buf[2] = 0x01;
	res = card_send(addr,buf,3); //setup
	if(res != 1){return 0;}
	return res;
}



/*********************************************************************************************************
** Function name:       card_vend_fail
** Descriptions:        ����������ʧ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_vend_fail(void)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_VEND_REQUEST;
	
	buf[0] = 0x03;
	res = card_send(addr,buf,1); //setup
	if(res != 1){return 0;}
	return res;
}


/*********************************************************************************************************
** Function name:       card_vend_complete
** Descriptions:        �������������
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_vend_complete(void)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_VEND_REQUEST;
	buf[0] = 0x04;
	res = card_send(addr,buf,1); //setup
	if(res != 1){return 0;}
	return res;
}


/*********************************************************************************************************
** Function name:       card_cash_sale
** Descriptions:        �ֽ���ϸ���׼�¼���ύ��������
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
static uint8 card_cash_sale(uint16 amount)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_VEND_REQUEST;
	buf[0] = 0x05;
	buf[1] = HUINT16(amount);
	buf[2] = LUINT16(amount);
	buf[3] = 0x00;
	buf[4] = 0x01;

	res = card_send(addr,buf,5); //setup
	if(res != 1){return 0;}
	return res;
}


/*********************************************************************************************************
** Function name:       card_enable
** Descriptions:        ʹ�ܿ���
** input parameters:    ��
** output parameters:   ��
** Returned value:      0��ʱ ,1�ɹ� 2���ݴ���
*********************************************************************************************************/
uint8 card_enable(uint8 en)
{
	uint8 res,addr,buf[10] = {0};
	addr = MDB_CARD_ADDR + MDB_CARD_ENBALE;
	buf[0] = (en == 0) ? 0 : 1;	
	res = card_send(addr,buf,1); //setup
	if(res != 1){return 0;}
	else{
		stCard.enabled = en;
		return res;
	}
}



static uint8 card_init(void)
{
	uint8 i,res;
	CARD_POLL poll;
	res = card_reset();
	print_card("card_reset:res = %d\r\n",res);
	if(res != 1){return 0;}
	for(i = 0;i < 5;i++){
		res = card_poll(&poll);
		if(res == 1 && poll.s & CARD_JUST_RESET){
			print_card("card_poll:CARD_JUST_RESET\r\n");
			break;
		}
		msleep(150);
	}
	
	res = card_setup();msleep(150);
	card_poll(&poll);msleep(150);
	print_card("card_setup:res = %d\r\n",res);
	res = card_setup_price();msleep(150);
	card_poll(&poll);msleep(150);
	print_card("card_setup_price:res = %d\r\n",res);
	res = card_enable(1);
	print_card("card_enable:res = %d\r\n",res);
	return res;
}



/*********************************************************************************************************
** Function name:       cardTaskPoll
** Descriptions:        �������豸��ѵ
** input parameters:    ��
** output parameters: 
** Returned value:      
*********************************************************************************************************/
uint8 cardTaskPoll(void)
{
	uint8 i;
	CARD_POLL poll;
	uint8 res = 0;
	if(stCard.status & CARD_MALFUNCTION_ERROR){
		if(Timer.card_reset > 0){return 0;}
		res = card_init();
		if(res == 1){
			stCard.status &= ~CARD_MALFUNCTION_ERROR;
			TIMER_SET(Timer.card_enable,90000);
		}
		else{
			TIMER_SET(Timer.card_reset,20000);
			return 0;
		}
	}
	
	card_poll(&poll);
	
	if(poll.s & CARD_BEGIN_SESSION){ //���׿�ʼ
		print_card("CARD_BEGIN_SESSION: remainAmount=%d\r\n",poll.recvAmount);
		card_reqeusted = 1;
		TIMER_SET(Timer.card_reqeusted,180000);
		if(stMdb.card_cost == 0 && stCard.recvAmount == 0){ //û�пۿ� ��û�����ڴ�������� ֹͣ����
			msleep(150);
			card_vend_complete();
		}
		else{
			print_card("Start vend request:cost = %d\r\n",stMdb.card_cost);
			msleep(150);
			res = card_vend_request(stMdb.card_cost / stCard.scale);
			if(res == 1){
				stCard.tradeStatus = CARD_TRADE_REQ;
				print_card("card_vend_request:suc_________\r\n");
			}
			else if(res == 3){ //�ܾ�
				//msleep(150);
				//card_vend_cancel();
			}
			else{
				card_vend_complete();
				print_card("card_vend_request:fail.......\r\n");
			}
		}
	}
	else if(poll.s & CARD_VEND_APPROVED){ //������㹻 ������	
		print_card("CARD_VEND_APPROVED:suc_________\r\n");
		for(i = 0;i < 10;i++){
			msleep(150);
			card_poll(&poll);
			if(poll.s & CARD_SESSION_CANCEL_REQUEST){
				msleep(150);
				card_vend_complete();
				return 0;
			}
		}
		msleep(150);
		res = card_vend_success();
		if(res == 1){ //�ۿ�ɹ�
			stCard.tradeStatus = CARD_TRADE_SUC;
			stCard.recvAmount = stMdb.card_cost; //����1000�� ��ʽ����Ҫ�޸�
			print_card("cost suc........!\r\n\r\n");
		}
		msleep(150);
		card_vend_complete();
	}
	else if(poll.s & CARD_VEND_DENIED){ //�� ���� �ܾ�����
		msleep(150);
		card_vend_complete();
		print_card("CARD_VEND_DENIED:fail.......\r\n");
	}
	else if(poll.s & CARD_SESSION_CANCEL_REQUEST){ //����������ȡ������ ���ذ�Ӧ�÷��ͽ����������
		msleep(150);
		card_vend_complete();
	}
	else if(poll.s & CARD_END_SESSION){
		card_reqeusted = 0;
	}
	else if(poll.s & CARD_MALFUNCTION_ERROR){
		stCard.status |= CARD_MALFUNCTION_ERROR;
	}
	else if(poll.s & CARD_CMD_ERR){
		stCard.status |= CARD_MALFUNCTION_ERROR;
	}
	
	print_card("Timer.card_reqeusted = %d,card_reqeusted = %d,Timer.card_enable = %d,stCard.enabled = %d,stCard.setEnable = %d\r\n"
		,Timer.card_reqeusted,card_reqeusted,Timer.card_enable,stCard.enabled,stCard.setEnable);
	if(Timer.card_reqeusted == 0){card_reqeusted = 0;}
	if(card_reqeusted == 0 ){
		if(stCard.setEnable != stCard.enabled){
			card_enable(stCard.setEnable);
		}
		
		if(Timer.card_enable == 0){
			card_enable(stCard.enabled);
			TIMER_SET(Timer.card_enable,90000);
		}
		
	}
	
	return 1;
}





/**************************************End Of File*******************************************************/
