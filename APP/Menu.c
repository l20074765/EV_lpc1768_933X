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
* Description:          键盘API函数：从键盘消息队列中取出案件值并返回该按键值
* Input:                None
* Output:               None
* Return:               按键值 ASCII码  0x00表示无按键值
*********************************************************************************************************/
uint8 MN_getKey(void)
{
	return ReadKeyValue();
}



/*********************************************************************************************************
** Function name:       UserMenu
** Descriptions:        用户菜单
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8 MN_userMenu(void)
{	
	uint8 key,entered = 1;
	print_menu("MN_userMenu\r\n");
	LED_showString("F00.0");
	
	while(entered){
		key = MN_getKey();
		switch(key){
			case '1': //查询hopper斗出币数
				break;
			case '2': //纸币器收币金额
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
** Descriptions:        通道面值设置页面
** input parameters:    
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
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
** Descriptions:        设置交易设备通道值菜单
** input parameters:    1:纸币器 2 硬币器  3hopper找零器
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
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
			case 'E'://进入下一级菜单
				if(index > 0){
					if(MN_getInputValue(ch,index-1) > 0){ //有更改需要保存flash
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
	uint32 temp = 0,value,temp32,point,v1;
	uint8 buf[20] = {0},buf1[20] = {0},index = 0,num,num1,v,i,j,cursor = 0,dataFlush = 1;
	ST_CHANGE_RATO ratio;
	
	//拷贝 比例数据
	for(i = 0;i < 8;i++){
		ratio.ch[i] = rato->ch[i];
		ratio.num[i] = rato->num[i];
	}
	ratio.amount = rato->amount;

	
	
	while(1){
		if(toFlush == 1){
			toFlush = 0;
			if(index == 0){
				if(dataFlush == 1){
					LED_showString("####");
					msleep(500);
					dataFlush = 0;
					temp = MDB_valueFromCents(ratio.amount);
				}
				LED_showAmount(temp);
			}
			else{
				if(dataFlush == 1){
					dataFlush = 0;
					LED_showString("####");
					msleep(500);
					
					temp32 = MDB_valueFromCents(ratio.ch[index - 1]);
					memset(buf,0,sizeof(buf));	
					
					num = 0;num1 = 0;
					while(temp32 > 0){	//将大数 拆分到数组中去
						v = temp32 % 10;
						temp32 /= 10;
						buf1[num1++] = ledTab[v];	
					}
					
					if(num1 < 4){
						num = num + (4 - num1);
					}
					buf[3] = ledTab[0];
					for(i = num1;i > 0;i--){
						buf[num++] = buf1[i - 1];
					}
					
					//插入小数点
					point = (uint32)stMdb.pointValue;
					switch(point){
						case 1:
							if(buf[2] == 0){
								buf[2] = ledTab[0] | L8_8;
							}
							else{
								buf[2] |= L8_8;
							}
							break;
						case 2:
							if(buf[1] == 0){
								buf[1] = ledTab[0] | L8_8;
							}
							else{
								buf[1] |= L8_8;
							}
							if(buf[2] == 0){
								buf[2] = ledTab[0];
							}
							break;
						case 3:
							if(buf[0] == 0){
								buf[0] = ledTab[0] | L8_8;
							}
							else{
								buf[0] |= L8_8;
							}
							if(buf[1] == 0){
								buf[1] = ledTab[0] ;
							}
							if(buf[2] == 0){
								buf[2] = ledTab[0];
							}
							break;
						default:break;
					}
					
				
					buf[num++] = L8_7; //'-'
					
					temp = ratio.num[index - 1];
					
					buf[num++] = ledTab[temp / 10];
					buf[num++] = ledTab[temp % 10];
					buf[num++] = 0;
					buf[num++] = 0;
					buf[num++] = 0;
					cursor = num -3 - 4;
				}
				//  0.08-xx 
				if(isEdit){
					buf[num - 6] = L8_4;
				}
				else{
					buf[num - 6] = L8_7;
				}
				buf[num - 5] = ledTab[temp / 10];
				buf[num - 4] = ledTab[temp % 10];
				
				led_display(buf[cursor],buf[cursor + 1],
							buf[cursor + 2],buf[cursor + 3]);
			
			}
			
			
		}
		key = MN_getKey();
		switch(key){
			case '1': case '2': case '3': case '4': case '5':case '6':case '7':case '8':case '9':case '0':
				if(isEdit){
					temp = temp * 10 + (key - '0');
					toFlush = 1;
				}
				else{	//翻页
					index = (key - '0') % 9;
					toFlush = 1;
					dataFlush = 1;
				}
				break;
				
			case 'E':
				if(isEdit){
					isEdit = 0;
					if(index == 0){
						ratio.amount = MDB_valueToCents(temp);
					}
					else{
						temp = temp % 100;
						ratio.num[index - 1] = temp;
					}
					toFlush = 1;
					dataFlush = 1;
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
					dataFlush = 1;
				}
				else{
					//需要验证 兑币比例合理性 
					temp = 0;
					for(i = 0;i < 8;i++){
						temp += ratio.ch[i] * ratio.num[i];
					}
					if(temp == ratio.amount){ //符合比例
						LED_showString("HP00");
						rato->amount = ratio.amount;
						for(i = 0;i < 8;i++){
							rato->ch[i] = ratio.ch[i];
							rato->num[i] = ratio.num[i];
						}
					}
					else{
						LED_showString("HPEE");
						msleep(1000);
					}
					
					returnFlag = 1;
				}
				break;
			case '>': 		//下翻
				if(cursor < (num - 4)){
					cursor++;
					toFlush = 1;
				}
				break;
			case '<':		//上翻
				if(cursor > 0){
					cursor--;
					toFlush = 1;
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
** Descriptions:        设置纸币兑币比例
** input parameters:    
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
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
			case 'E'://进入下一级菜单
				if(index > 0){
					
					if(MN_setRato(&rato[index - 1]) > 0){ //有更改需要保存flash
						
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
** Descriptions:        设置小数点
** input parameters:    无
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
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
** Descriptions:        管理员菜单
** input parameters:    无
** output parameters:   无
** Returned value:      1 配置有更改  0配置无更改
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
			case '1': //配置纸币器通道面值
				LED_showString("A01.0");
				isChanged += MN_setChannel(1);
				toFlush = 1;
				break;
			case '2': //配置硬币器通道面值
				LED_showString("A02.0");
				isChanged += MN_setChannel(2);
				toFlush = 1;
				break;
			case '3': //配置hopper通道面值
				LED_showString("A03.0");
				isChanged += MN_setChannel(3);
				toFlush = 1;
				break;
			case '4': //配置小数点
				LED_showString("A04.0");
				msleep(500);
				LED_showString("####");
				msleep(500);
				isChanged += MN_setPointValue(&stMdb.pointValue);
				toFlush = 1;
				break;
			case '5': //配置纸币兑零比例
				LED_showString("A05.0");
				isChanged += MN_setBillRato(stMdb.billRato);
				toFlush = 1;
				break;
			case '6': //配置硬币兑零比例
				LED_showString("A06.0");
				isChanged += MN_setBillRato(stMdb.coinRato);
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
** Descriptions:        判断是否有进入维护菜单的组合键输入
** input parameters:    无
** output parameters:   无
** Returned value:      0没有进入维护菜单 1进入管理模式  2进入用户模式
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







