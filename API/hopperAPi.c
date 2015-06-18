#include "..\config.h"


#define DEBUG_HOPPER

#ifdef 	DEBUG_HOPPER
#define print_hopper(...)	Trace(__VA_ARGS__)
#else
#define print_hopper(...)
#endif



ST_HOPPER stHopper[HP_SUM];//����8��hopper�� �ṹ������
//����hopper���� �ṹ������ ���������λ��ּ��� 1 > 2 > 3
//���� hopper1 hopper2 ͨ����ֵ 1Ԫ hopper3 ͨ����ֵΪ 5ë �� hopper1 hopper2
//�����ڼ���1��,hopper3 �����ڼ���2��  ����3Ϊ��  ��
ST_HOPPER_LEVEL stHopperLevel[HP_SUM];

static volatile uint8 sendbuf[64] = {0};
static volatile uint8 recvbuf[64] = {0};
static uint8 sn[HP_SUM] = {0};


/*********************************************************************************************************
** Function name:     	HP_send
** Descriptions:	    EVBͨѶ
** input parameters:    Chl:ѡ��ͨ����Head:��ͷ��Sn�����кţ�Type:��Ϣ���Addr:�豸��ַ;Data�����ݣ�
** output parameters:   *ACK��Ӧ���
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
uint8 HP_send(uint8 addr,uint8 cmd,uint16 data)
{
	uint8 index = 0,i,ch,len,crc;
	memset((void *)recvbuf,0,sizeof(recvbuf));
	
	sn[addr] = (cmd == HP_PAYOUT) ? sn[addr] + 1: sn[addr]; //����sn
	
	sendbuf[index++] = 0xED;
	sendbuf[index++] = 0x08; //len
	sendbuf[index++] = sn[addr];
	sendbuf[index++] = cmd;
	sendbuf[index++] = addr;
	sendbuf[index++] = LUINT16(data);
	sendbuf[index++] = HUINT16(data);
	sendbuf[index++] = XorCheck((uint8 *)sendbuf,7);
	
	uart3_clr_buf();
	//�������ݽ�ֹ�л�����
	OSSchedLock();
	Uart3PutStr((uint8 *)sendbuf,8);
	OSSchedUnlock();
	
	#ifdef DEBUG_HOPPER
	print_hopper("HP-SEND[%d]:",index);
	for(i = 0;i < index;i++){
		print_hopper("%02x ",sendbuf[i]);
	}
	print_hopper("\r\n");
	#endif
	
	index = 0;
	Timer.hopper_recv_timeout = 100;
	while(Timer.hopper_recv_timeout){ //1000ms���յ�ACK,����ʱ
		if(Uart3BuffIsNotEmpty() == 1){
			ch = Uart3GetCh();
			if(index == 0){
				if(ch == 0xFD){
					recvbuf[index++] = ch;
				}
			}
			else if(index == 1){
				len = ch;
				recvbuf[index++] = ch;
			}
			else if(index >= (len - 1)){
				recvbuf[index++] = ch;
				crc = XorCheck((uint8 *)recvbuf,len - 1);
				if(crc == ch){
#ifdef DEBUG_HOPPER
					print_hopper("HP-RECV[%d]:",recvbuf[1]);
					for(i = 0;i < recvbuf[1];i++){
						print_hopper("%02x ",recvbuf[i]);
					}
					print_hopper("\r\n");
#endif
					return 1;
				}
			}
			else{
				recvbuf[index++] = ch;
			}
		}
		else{
			msleep(20);
		}
	}
	return 0;
}



/*********************************************************************************************************
** Function name:     	HP_check
** Descriptions:	    hopper�豸�������ƺ���
** input parameters:    addr:�豸��Ӳ�������ַ				
** output parameters:   ��
** Returned value:      1���յ�Ӧ��0��Ӧ��ʱ  2 æ
*********************************************************************************************************/
uint8 HP_send_check(ST_HOPPER *hopper)
{
	uint8 res,s;
	res = HP_send(hopper->addr,HP_QUERY,0x00);
	if(res == 1){
		if(recvbuf[1] == 0x08 && hopper->addr == recvbuf[4]) {
			s = recvbuf[3];
			print_hopper("HP[%d]=%x\r\n",hopper->addr + 1,s);
			
			if(s & (0x01U << 1)){ //hopperæ
				return 3;
			}
			
			if((s & (0x01U << 2)) || (s & (0x01U << 7))){ //ָ�����
				return 3;
			}
			
			if(s == 0){ //hopper ����
				hopper->state = HP_STATE_NORMAL;
			}
			else{
				if((s & (0x01U << 3)) || (s & (0x01U << 5))){ //hopper ȱ��
					hopper->state = HP_STATE_QUEBI;
				}
				
				if((s & (0x01U << 4)) || (s & (0x01U << 6))){ //hopper ����
					hopper->state = HP_STATE_FAULT;
				}
			}
			hopper->lastCount = INTEG16(recvbuf[6], recvbuf[5]);
			return 1;
		}
		return 99;			
	}
	
	return 0;	
}


/*********************************************************************************************************
** Function name:     	HP_send_output
** Descriptions:	    EVBhopper�豸�������ƺ���
** input parameters:    addr:�豸��Ӳ�������ַ		
						num:Hopper���Ҳ����ĳ�������
** output parameters:   ��
** Returned value:      1���յ�Ӧ��0��δ�յ�Ӧ��ͨѶʧ��
*********************************************************************************************************/
uint8 HP_send_output(ST_HOPPER *hopper,uint16 num)
{
	uint8 i,res;
	for(i = 0;i < 3;i++){
		res = HP_send(hopper->addr,HP_PAYOUT,num);
		if(res == 1 && recvbuf[1] == 0x06){
			return 1;
		}
		else{
			msleep(100);
		}
	}
	return 0;
}



/*********************************************************************************************************
** Function name:     	HP_init
** Descriptions:	    hopper�豸��ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void HP_init(void)
{
	uint8 i,j,levelNum = 0;
	uint32 value;
	for(i = 0; i < HP_SUM;i++){
		value = stHopper[i].ch;
		memset((void *)&stHopper[i],0,sizeof(ST_HOPPER));
		memset((void *)&stHopperLevel[i],0,sizeof(ST_HOPPER_LEVEL));
		stHopper[i].ch = value;
		stHopper[i].addr = i;
	}
	
	//�������
	for(i = 0;i < HP_SUM;i++){
		value = 0;
		for(j = 0;j < HP_SUM;j++){ //��ѯδ���伶���е����
			if(!stHopper[j].level && value < stHopper[j].ch){
				value = stHopper[j].ch;
			}
		}
		stHopperLevel[i].ch = value;
		if(value){
			for(j = 0;j < HP_SUM;j++){   //��ѯ��ͬ ��ֵ�Ķ� ���з���
				if(value == stHopper[j].ch){
					levelNum = stHopperLevel[i].num++;
					stHopperLevel[i].hopper[levelNum] = &stHopper[j];
					stHopper[j].level = 1;//��������־
				}
			}
		}
		
		print_hopper("Level=%d num =%d value = %d\r\n",
			i,stHopperLevel[i].num,stHopperLevel[i].ch);
	}
	

	print_hopper("hopperInit over..\r\n");
}




uint8 HP_setCh(uint8 no,uint32 value)
{
	if(no > HP_SUM || no == 0){
		return 0;
	}
	stHopper[no - 1].ch = value;
	return 1;
	
}

/*********************************************************************************************************
** Function name:     	HP_payout_by_addr
** Descriptions:	    hopper����ַ����
** input parameters:    num ��Ҫ�ұ�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 HP_payout_by_addr(ST_HOPPER *hopper,uint16 num)
{
	uint8 res;
	res = HP_send_output(hopper,num);
	if(res != 1){
		return 0;
	}
	
	msleep(1000); //�ȴ�1s
	Timer.hopper_payout_timeout = 5000 + num * 250;
	while(Timer.hopper_payout_timeout){
		res = HP_send_check(hopper);
		if(res == 1){ //�������
			if(num == hopper->lastCount){
				return 1;
			}
			else{
				return 0;
			}
		}
		msleep(100);
	}
	return 0;
	
}


/*********************************************************************************************************
** Function name:     	HP_payout_by_addr
** Descriptions:	    hopper?????
** input parameters:    num ??????
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
uint16 HP_payout_by_no(uint8 addr,uint16 num)
{
	uint8 res;
	ST_HOPPER *hopper;
	if(addr <= 0 || addr > HP_SUM || num == 0){
		return 0;
	}
	
	hopper = &stHopper[addr - 1];
	res = HP_send_output(hopper,num);
	if(res != 1){
		return 0;
	}
	
	msleep(1000); //??1s
	Timer.hopper_payout_timeout = 5000 + num * 250;
	while(Timer.hopper_payout_timeout){
		res = HP_send_check(hopper);
		if(res == 1){ //????
			return hopper->lastCount;
		}
		msleep(100);
	}
	return 0;
	
}



/*********************************************************************************************************
** Function name:     	HP_payout_by_level
** Descriptions:	    hopper�豸�����������
** input parameters:    
** output parameters:   remainCount��������ʧ�ܺ�ʣ�������
						levelNo :ѡ���ĸ����� ����
** Returned value:      changeCount������Ҫ���������
*********************************************************************************************************/
uint16  HP_payout_by_level(uint8 l,uint16 payCount,uint16 *hp)
{
	uint8 i,cycleFlag = 0,res;
	uint16 payCountTemp = 0;
	uint16 changedCount = 0;
	ST_HOPPER *hopper;
	
	if(payCount == 0){
		return 0;
	}
	
	//���� hopper�����Ƿ����
	if(l >= HP_SUM){
		print_hopper("The level = %d > %d(max) \r\n",l,HP_SUM);
		return 0;
	}
	
	
	if(stHopperLevel[l].num  == 0) { //û�п��õĶ�
		print_hopper("The level = %d useableNum = is 0 \r\n",l);
		return 0;
	}
	
	
	
	//��ѯ�Ƿ���ѭ����
	for(i = 0; i < stHopperLevel[l].num;i++){
		hopper = stHopperLevel[l].hopper[i];
		if(hopper->isCycle && hopper->state == HP_STATE_NORMAL){
			cycleFlag = 1;
			break;
		}		
	}
	
	if(cycleFlag){	//��ѭ���� ������ѭ��������
		hopper = stHopperLevel[l].hopper[i];
		res = HP_payout_by_addr(hopper,payCount);
		if(res == 1){
			changedCount  += payCount;
			hp[hopper->addr] += hopper->lastCount;
			return changedCount;
		}
		else{
			hp[hopper->addr] += hopper->lastCount;
			changedCount += hopper->lastCount;	//ѭ��������ʧ�� ����������	
		}
	}
	
	//û��ѭ���� �����ѡ��һ�����õĶ� ����
	for(i = 0; i < stHopperLevel[l].num;i++)
	{	
		hopper = stHopperLevel[l].hopper[i];
		if(hopper->lastPayFail >= HP_PAYFAIL_NUM)//����豸�ϴ�����ʧ��
			continue;
		payCountTemp = payCount - changedCount;
		res = HP_payout_by_addr(hopper,payCountTemp);
		print_hopper("Select--level = %d addr= %d res =%d\r\n",l,hopper->addr,res);
		if(res == 1){
			changedCount  += payCountTemp;
			hp[hopper->addr] += hopper->lastCount;
			hopper->lastPayFail =  0;//�������
			return changedCount;
		}
		else{
			hp[hopper->addr] += hopper->lastCount;
			changedCount += hopper->lastCount;
			print_hopper("PayCount = %d,hopper[%d]->lastCount =%d,changedCount =%d\r\n",
					payCount,i,hopper->lastCount,changedCount);
			//���˱�ʾ�ö� �Ѿ��޷��ұ� ��������ϲ����		
			hopper->lastPayFail++;//����+1
		}				
	}
	return changedCount;
}



/*********************************************************************************************************
** Function name:     	HP_payout
** Descriptions:	    Ӳ�Ҷ����㷨 ��ֵ�Ӵ�С�ұ�
** input parameters:   
** output parameters:   
** Returned value:      1���ұҳɹ���0��ʧ��
*********************************************************************************************************/
uint32 HP_payout(uint32 payAmount,uint16 *hp)
{
	uint8 i,res;
	uint16 changedCount = 0,payCount = 0;
	uint32 changedAmount;
	
	changedAmount = 0;
	//Ӳ���������㷨
	for(i = 0;i < HP_SUM;i++){
		if(stHopperLevel[i].ch > 0){
			payCount = payAmount / stHopperLevel[i].ch; //�����ұ�����
			print_hopper("Coin level[%d].payCount = %d \r\n",i,payCount);
			changedCount = HP_payout_by_level(i,payCount,hp);
			changedAmount += changedCount * stHopperLevel[i].ch;
			if(changedAmount <= payAmount)
				payAmount = payAmount - changedAmount;
			else
				payAmount = 0;
			//����ɹ�
			if(res == 1 && payAmount == 0){
				return changedAmount;		
			}
		}
	}
	//���㲻�ɹ�
	return changedAmount;	
}


static uint8 HP_getCheckNo(void)
{
	static uint8 no = 0;
	uint8 i ;
	for(i = no;i < HP_SUM;i++){
		if(stHopper[i].ch > 0){
			no = i + 1;
			return i;
		}
	}
	
	for(i = 0;i < no;i++){
		if(stHopper[i].ch > 0){
			no = i + 1;
			return i;
		}
	}
	
	return 0xFF;
}

static uint16 HP_getCheckTimer(void)
{
	uint16 time;
	uint8 i;
	time = 2400;
	for(i = 0;i < HP_SUM;i++){
		if(stHopper[i].ch > 0){
			time = (time <= 300) ? 300 : time - 300;
		}
	}
	return time;
}

uint8 HP_allHopperCheck(void){
	uint8 i = 0;
	if(Timer.hopper_check_timeout == 0){
		i = HP_getCheckNo();
		if(i != 0xFF){
			if(HP_send_check(&stHopper[i]) == 0){
				stHopper[i].err_com = (stHopper[i].err_com < 100) ? stHopper[i].err_com + 1: 100;
			}
			else{
				stHopper[i].err_com = 0;
			}
			if(stHopper[i].err_com >= 5){
				stHopper[i].state = HP_STATE_COM; //ͨ��ʧ��
			}
		}
		Timer.hopper_check_timeout = 300;//HP_getCheckTimer();//3����һ����
	}
	return 1;
}


/*********************************************************************************************************
** Function name:     	hopperChangerFailCheck
** Descriptions:	    ��ѯhopper����ʧ�ܵ�ԭ��
** input parameters:   
** output parameters:   
** Returned value:      1
*********************************************************************************************************/
uint8 hopperChangerFailCheck(const uint32 remainAmount)
{
	uint8 i;
	for(i = HP_SUM;i > 0;i--){
		if(stHopperLevel[i - 1].ch)
			break;
	}

	
	if(i && (remainAmount < stHopperLevel[i - 1].ch))
		return 1;
	else
		return 0;
}

void HP_task(void)
{
	HP_allHopperCheck();
}




/**************************************End Of File*******************************************************/
