#include "..\config.h"
#include "Menu.h"

#define DEBUG_MENU

#ifdef 	DEBUG_MENU
#define print_menu(...)	Trace(__VA_ARGS__)
#else
#define print_menu(...)
#endif

/*********************************************************************************************************
* Function Name:        ReadKeyValue
* Description:          ����API�������Ӽ�����Ϣ������ȡ������ֵ�����ظð���ֵ
* Input:                None
* Output:               None
* Return:               ����ֵ ASCII��  0x00��ʾ�ް���ֵ
*********************************************************************************************************/
uint8 MN_getKey(void)
{
	return ReadKeyValue();
}



/*********************************************************************************************************
** Function name:       UserMenu
** Descriptions:        �û��˵�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
uint8 MN_userMenu(void)
{	
	uint8 key,entered = 1;
	print_menu("MN_userMenu\r\n");
	LED_showString("F00.0");
	
	while(entered){
		key = MN_getKey();
		switch(key){
			case '1': //��ѯhopper��������
				break;
			case '2': //ֽ�����ձҽ��
				break;
			case 'C':
				entered = 0;
				break;
			default:break;
		
		}
	
	}
	return 0;	
}


/*********************************************************************************************************
** Function name:       MN_getInputValue
** Descriptions:        ͨ����ֵ����ҳ��
** input parameters:    
** output parameters:   ��
** Returned value:      1 �����и���  0 �����޸���
*********************************************************************************************************/
uint8 MN_getInputValue(uint32 *ch,uint8 in)
{
	uint8 key,returnFlag = 0,toFlush = 1,isEdit = 0,isChanged = 0;
	uint32 temp = 0,value;
	value = MDB_valueFromCents(ch[in]);
	temp = value;
	while(1){
		if(toFlush == 1){
			toFlush = 0;
			LED_showAmount(temp);
		}
		key = MN_getKey();
		switch(key){
			case '1': case '2': case '3': case '4': case '5':case '6':case '7':case '8':case '9':case '0':
				if(isEdit){
					temp = temp * 10 + (key - '0');
					toFlush = 1;
				}
				break;
			case 'E':
				if(isEdit){
					isEdit = 0;
					ch[in] = MDB_valueToCents(temp);
					LED_showString("####");
					msleep(500);	
					LED_showAmount(temp);	
					msleep(500);
					returnFlag = 1;	
					isChanged = 1;
				}
				else{
					LED_showString("####");
					msleep(200);
					isEdit = 1;
					temp = 0;
				}
				toFlush = 1;
				break;
			case 'C':
				if(isEdit){
					isEdit = 0;
					toFlush = 1;
					temp = value;
				}
				else{
					returnFlag = 1;
				}
				break;
			default:break;
		}
		
		if(returnFlag == 1){
			return isChanged;
		}
		
	}
}


/*********************************************************************************************************
** Function name:       MN_setChannel
** Descriptions:        ���ý����豸ͨ��ֵ�˵�
** input parameters:    1:ֽ���� 2 Ӳ����  3hopper������
** output parameters:   ��
** Returned value:      1 �����и���  0 �����޸���
*********************************************************************************************************/
uint8 MN_setChannel(uint8 type)
{
	uint8 key,i,index = 0,topReturnFlag = 0;
	uint8 topFlush = 1;
	uint32 ch[16] = {0};
	uint16 isChanged = 0;
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			for(i = 0;i < 16;i++){
				if(type == 1)
					ch[i] = stBill.setup.ch[i];
				else if(type == 2)
					ch[i] = stPcoin.ch[i];
				else if(type == 3)
					ch[i] = stHopper[i].ch;
				else 
					ch[i] = ch[i];
			}
			LED_show("A0%d.0",type);
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':
				LED_show("A0%d.%c",type,key);
				index = key - '0';
				break;
			case 'C':
				if(index > 0){
					index = 0;
					topFlush = 1;
				}
				else{
					topReturnFlag = 1;
				}
				break;
			case 'E'://������һ���˵�
				if(index > 0){
					if(MN_getInputValue(ch,index-1) > 0){ //�и�����Ҫ����flash
						for(i = 0;i < 16;i++){
							if(type == 1)
								stBill.setup.ch[i] = ch[i];
							else if(type == 2)
								stPcoin.ch[i] = ch[i];
							else if(type == 3)
								stHopper[i].ch = ch[i];
							else
								ch[i] = ch[i];
						}
						FM_writeToFlash();
						isChanged++;
					}
					index = 0;
				}
				
				topFlush = 1;	
				break;
			default:
				break;
		}
		
		if(topReturnFlag > 0){
			return (isChanged > 0 ? 1 : 0);
		}
		msleep(50);
	}
			
}

uint8 MN_setRato(ST_CHANGE_RATO *rato)
{
	uint8 key,returnFlag = 0,toFlush = 1,isEdit = 0,isChanged = 0;
	uint32 temp = 0,value;
	value =  MDB_valueFromCents(rato->amount);
	temp = value;
	while(1){
		if(toFlush == 1){
			toFlush = 0;
			LED_showAmount(temp);
		}
		key = MN_getKey();
		switch(key){
			case '1': case '2': case '3': case '4': case '5':case '6':case '7':case '8':case '9':case '0':
				if(isEdit){
					temp = temp * 10 + (key - '0');
					toFlush = 1;
				}
				break;
			case 'E':
				if(isEdit){
					isEdit = 0;
					rato->amount = MDB_valueToCents(temp);
					LED_showString("####");
					msleep(500);	
					LED_showAmount(temp);	
					msleep(500);
					returnFlag = 1;	
					isChanged = 1;
				}
				else{
					LED_showString("####");
					msleep(200);
					isEdit = 1;
					temp = 0;
				}
				toFlush = 1;
				break;
			case 'C':
				if(isEdit){
					isEdit = 0;
					toFlush = 1;
					temp = value;
				}
				else{
					returnFlag = 1;
				}
				break;
			default:break;
		}
		
		if(returnFlag == 1){
			return isChanged;
		}
		
		
	}
	
	
}


/*********************************************************************************************************
** Function name:       MN_setBillRato
** Descriptions:        ����ֽ�Ҷұұ���
** input parameters:    
** output parameters:   ��
** Returned value:      1 �����и���  0 �����޸���
*********************************************************************************************************/
uint8 MN_setBillRato(ST_CHANGE_RATO *rato)
{
	uint8 key,i,index = 0,topReturnFlag = 0,isEdit = 0;
	uint8 topFlush = 1;
	uint32 ch[16] = {0};
	uint16 isChanged = 0;
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_showString("A05.0");
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':
				LED_show("A05.%c",key);
				index = key - '0';
				break;
			case 'C':
				if(index > 0){
					index = 0;
					topFlush = 1;
				}
				else{
					topReturnFlag = 1;
				}
				break;
			case 'E'://������һ���˵�
				if(index > 0){
					if(MN_setRato(&rato[index - 1]) > 0){ //�и�����Ҫ����flash
						
						FM_writeToFlash();
						isChanged++;
					}
					index = 0;
				}
				
				topFlush = 1;	
				break;
			default:
				break;
		}
		
		if(topReturnFlag > 0){
			return (isChanged > 0 ? 1 : 0);
		}
		msleep(50);
	}
	
}



/*********************************************************************************************************
** Function name:       MN_setPointValue
** Descriptions:        ����С����
** input parameters:    ��
** output parameters:   ��
** Returned value:      1 �����и���  0 �����޸���
*********************************************************************************************************/
uint8 MN_setPointValue(uint8 *p)
{
	uint8 key,returnFlag = 0,isEdit = 0;
	uint8 topFlush = 1,isChanged = 0;
	uint8 point;
	point = *p;
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_showData(point);
			print_menu("point=%d\r\n",point);
		}
		key = MN_getKey();
		switch(key){
			case '0':case '1': case '2': case '3':
				if(isEdit){
					point = key - '0';
					topFlush = 1;
					
				}
				break;
			case 'C':
				if(isEdit){
					isEdit = 0;
					point = *p;
					topFlush = 1;
				}
				else{
					returnFlag = 1;
				}
				break;
			case 'E':
				if(isEdit){
					isEdit = 0;
					*p = point;
					LED_showString("####");
					msleep(500);	
					LED_showData(point);	
					msleep(500);
					returnFlag = 1;	
					FM_writeToFlash();
					isChanged = 1;
				}
				else{
					LED_showString("####");
					msleep(200);
					isEdit = 1;
					point = 0;
				}
				topFlush = 1;
				
				break;
			default:
				break;
		}
	
		if(returnFlag){
			return isChanged;
		}
		msleep(50);
	}
}



/*********************************************************************************************************
** Function name:       MN_adminMenu
** Descriptions:        ����Ա�˵�
** input parameters:    ��
** output parameters:   ��
** Returned value:      1 �����и���  0�����޸���
*********************************************************************************************************/
uint8 MN_adminMenu(void)
{
	uint8 key,toFlush = 1,topReturnFlag = 0;
	uint16 isChanged = 0;
	while(1){
		if(toFlush == 1){
			toFlush = 0;
			LED_showString("A00.0");
		}
		key = MN_getKey();
		switch(key){
			case '1': //����ֽ����ͨ����ֵ
				LED_showString("A01.0");
				isChanged += MN_setChannel(1);
				toFlush = 1;
				break;
			case '2': //����Ӳ����ͨ����ֵ
				LED_showString("A02.0");
				isChanged += MN_setChannel(2);
				toFlush = 1;
				break;
			case '3': //����hopperͨ����ֵ
				LED_showString("A03.0");
				isChanged += MN_setChannel(3);
				toFlush = 1;
				break;
			case '4': //����С����
				LED_showString("A04.0");
				msleep(500);
				LED_showString("####");
				msleep(500);
				isChanged += MN_setPointValue(&stMdb.pointValue);
				toFlush = 1;
				break;
			case '5': //����ֽ�Ҷ������
				LED_showString("A05.0");
				isChanged += MN_setBillRato(stMdb.billRato);
				toFlush = 1;
				break;
			case 'C':
				topReturnFlag = 1;
				break;
			default:break;
		
		}
		if(topReturnFlag == 1){
			return (isChanged > 0) ? 1 : 0;
		}
		
		msleep(50);
	}
}















/*********************************************************************************************************
** Function name:       MN_isMenuEntered
** Descriptions:        �ж��Ƿ��н���ά���˵�����ϼ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      0û�н���ά���˵� 1�������ģʽ  2�����û�ģʽ
*********************************************************************************************************/
uint8 MN_isMenuEntered(void)
{
	uint8 key = 0x00;
	static uint8 adminFlag = 0,userFalg = 0;
    
	key = MN_getKey();
	//print_menu("key=%c %d \r\n",(char)key,key);
	if(key != 0x00){
		if(key == 'D'){
			adminFlag = 0;
			userFalg++;
		}
		else if(key == '>'){
			userFalg = 0;
			adminFlag++;
		}
		else{
			userFalg = 0;
			adminFlag = 0;
		}
		if(adminFlag >= 3){
			adminFlag = 0;
			return 2;
		}
		
		if(userFalg >= 3){
			userFalg = 0;
			return 1;
		}
	}
	
	return 0;
}







