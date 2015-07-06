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
** Function name:       MN_dispAmount
** Descriptions:        
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void MN_dispAmount(uint32 data)
{
	uint8 key;
	LED_showAmount(data);
	while(1){
		key = MN_getKey();
		if(key == 'C')
			return;
		msleep(20);
	}
}


/*********************************************************************************************************
** Function name:       MN_dispAmount
** Descriptions:        
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void MN_dispData(uint32 data)
{
	uint8 key;
	LED_showData(data);
	while(1){
		key = MN_getKey();
		if(key == 'C')
			return;
		msleep(20);
	}
}


static void MN_hopperOutCheck(uint8 type)
{
	uint8 key,topFlush = 1,topReturn = 0;
	uint8 no;
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_show("U%02d.0",type);
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':
				no = key - '0';
				LED_show("U%02d.%d",type,no);
				break;
			case 'C':
				if(no > 0){
					no = 0;
				}
				else{
					topReturn = 1;
				}
				topFlush = 1;
				break;
			case 'E':
				if(no > 0){
					MN_dispData(stLog.hpChanged[no - 1]);
					no = 0;
					topFlush = 1;
				}
				break;
			default:break;
		}
		
		
		if(topReturn == 1){
			return;
		}
			
		msleep(20);
	}
}


static void MN_billTest(void)
{
	uint8 key;
	uint32 amount = 0,temp32 = 0;
	DEV_enableReq(OBJ_BILL,1);
	
	LED_showAmount(amount);
	while(1){
		key = MN_getKey();
		if(key == 'C'){
			temp32 = MDB_getBillRecvAmount();
			MDB_billCost(temp32);		
			break;
		}
		temp32 = MDB_getBillRecvAmount();
		if(amount != temp32){
			amount = temp32;
			LED_showAmount(amount);
		}
		msleep(200);
	}
	DEV_enableReq(OBJ_BILL,0);	
	msleep(1000);
}



//硬币器测试
static void MN_coinTest(void)
{
	uint8 key;
	uint32 amount = 0,temp32 = 0;
	DEV_enableReq(OBJ_COIN,1);
	
	LED_showAmount(amount);
	while(1){
		key = MN_getKey();
		if(key == 'C'){	 
			temp32 = MDB_getCoinRecvAmount();
			MDB_coinCost(temp32);		
			break;
		}
		temp32 = MDB_getCoinRecvAmount();
		msleep(200);
		if(amount != temp32){
			amount = temp32;
			LED_showAmount(amount);
		}
	}
	DEV_enableReq(OBJ_COIN,0);	
	msleep(1000);
}



/*********************************************************************************************************
** Function name:       MN_hopperTest
** Descriptions:        Hp测试
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void MN_hopperTest(uint8 menu,uint8 flag)
{
	uint8 key,returnFlag = 0,enterSubMenu = 0,hopperNo = 0,topFlush = 1;
	uint8 state=0;
	uint32 changed;
	Q_MSG *msg;
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_show("U%02d.%d",menu,hopperNo);
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':
				hopperNo = key - '0';
				topFlush = 1;
				break;
			case 'C':
				if(hopperNo)
					hopperNo = 0;
				else
					returnFlag = 1;
				topFlush = 1;
				break;
			case 'E':
				if(hopperNo)
					enterSubMenu = 1;
				break;
			default:
				break;
		}
		if(enterSubMenu){
			// HP01
			LED_show("HP%d0",hopperNo);
			if(flag == 1){
				DEV_hpPayoutReq(hopperNo,5);
				msg = DEV_msgRpt(DEV_HP_PAYOUT,50000);
				if(msg != NULL){
					LED_showData(msg->coinChanged);
					msleep(1000);
					LED_showString("####");
					msleep(1000);
				}
			}
			else{
				changed = 0;
				while(1){
					DEV_hpPayoutReq(hopperNo,10);
					msg = DEV_msgRpt(DEV_HP_PAYOUT,50000);
					if(msg == NULL){
						break;
					}
					else{
						changed += msg->coinChanged;
						LED_showData(changed);
						if(msg->coinChanged < 10){
							break;
						}
						
					}
					key = MN_getKey();
					if(key == 'C'){
					    break;			
					}
					
				}
			}

			
			
			state = stHopper[hopperNo - 1].state;
			if(state == HP_STATE_FAULT || state == HP_STATE_COM){
				LED_show("HP%dE",hopperNo);
			}	
			else{
				LED_show("HP%d0",hopperNo);
			}
				
			msleep(1000);
			hopperNo = 0;
			enterSubMenu = 0;
			topFlush = 1;
		}
		
		if(returnFlag){
			return;
		}
		msleep(20);
	}
}



/*********************************************************************************************************
** Function name:       MN_clearLog
** Descriptions:         清除交易记录
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
static void MN_clearLog(void)
{
	uint8 key,returnFlag = 0,enterSub = 0,index =0,topFlush = 1;
	char num[4] ;
	memset(num,'-',sizeof(num));
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_show("%c%c%c%c",num[0],num[1],num[2],num[3]);
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':case '4'://密码 1234
				num[index++] = key;
				topFlush = 1;
				break;
			case 'C':
				if(index)
					index = 0;
				else
					returnFlag = 1;
				break;
			case 'E':
				enterSub = 1;
				index= 0;
				break;
		}
		
		if(returnFlag)
			return;
		
		msleep(50);
		if(enterSub){
			if(num[0] == '1' && num[1] == '2' && num[2] == '3' && num[3] == '4'){
				LED_showString("CLEA");
				FM_clearLog();
				msleep(1000);
				LED_showString("####");
				msleep(1000);
				LED_showString("0000");
				msleep(1000);
				enterSub = 0;
				return;
				
			}
			else{
				LED_showString("####");
				msleep(1000);
				memset(num,'-',sizeof(num));
				topFlush = 1;
				enterSub = 0;
			}
		}

		
		
	}
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
	uint8_t key,subMenu = 0,returnFlag = 0,enterSubMenu = 0,topFlush = 1;
	print_menu("MN_userMenu\r\n");
	
	
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_showString("U00.0");
		}
		key = MN_getKey();
		switch(key){
			case '1': case '2': case '3': case '4': case '5':case '6':case '7':case '8':case '9':case '0':
				subMenu = subMenu * 10 + (key - '0');
				subMenu = (subMenu > 99) ? 00 : subMenu;
				LED_show("U%02d.0",subMenu);
				break;
			case 'E':
				if(subMenu){
					enterSubMenu = 1;	
					LED_showString("####");
					msleep(200);				
				}	
				break;
			case 'C':
				if(subMenu)
					subMenu = 0;
				else
					returnFlag = 1;
				topFlush = 1;
				break;
			default:break;
		
		}
		
		if(returnFlag){
			return 0;
		}
		msleep(20);
		
		if(enterSubMenu == 1){
			switch(subMenu){
//==============交易记录1- 19========================================
				case 1://纸币器收币
					MN_dispAmount(stLog.billRecv);
					break;
				case 2://硬币器收币
					MN_dispAmount(stLog.coinRecv);
					break;
				case 3://找零总金额
					MN_dispAmount(stLog.coinChanged);	
					break;
				case 4://成功交易次数
					
					break;
					
				case 5://欠费金额
					MN_dispAmount(stLog.iou);
					break;		

				case 6://上次交易欠费金额
					MN_dispAmount(stLog.lastIou);
					break;	

				case 7://hopper1出币数
					MN_hopperOutCheck(7);
					
					break;
				case 8://hopper2出币数
					//MN_dispData(stLog.hpChanged[1]);
					break;
				case 9://hopper3出币数
					//MN_dispData(stLog.hpChanged[2]);
					break;
				
//================================================================

//==============设备管理20 -39========================================
				case 20://测试纸币器收币
					MDB_clearRecvAmount();
					MN_billTest();
					break;
				case 21://测试硬币器收币
					MDB_clearRecvAmount();
					MN_coinTest();
					break;
				case 22://测试找零总金额	
					MN_hopperTest(22,1);	
					break;
				case 23://清零hopper斗			
					MN_hopperTest(23,0);	
					break;	
//================================================================
				

//==============货道管理40 -59========================================




					
//================================================================



				case 99:// 清除交易记录
					MN_clearLog();					
					break;
					
					
				default:
					break;
			}
			enterSubMenu = 0;
			subMenu = 0;
			topFlush = 1;
		}
	}
	
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
			
			LED_showAmount(MDB_valueToCents(temp));
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
					LED_showAmount(MDB_valueToCents(temp));	
					msleep(500);
					//returnFlag = 1;	
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
	uint32 temp = 0,temp32,point;
	uint8 buf[20] = {0},buf1[20] = {0},index = 0,num,num1,v,i,cursor = 0,dataFlush = 1;
	ST_CHANGE_RATO ratio;
	
	//拷贝 比例数据
	for(i = 0;i < 8;i++){
		print_menu("rato[%d] ch=%d num=%d\r\n",i,rato->ch[i],rato->num[i]);
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
				LED_showAmount(MDB_valueToCents(temp));
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
					if(index != 0 && temp > 99){
						temp = 0;
					}
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
					#if 0
					LED_showString("HP00");
					rato->amount = ratio.amount;
					for(i = 0;i < 8;i++){
						rato->ch[i] = ratio.ch[i];
						rato->num[i] = ratio.num[i];
					}
					#else
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
					#endif
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
** Function name:       MN_setCoinHighEnbale
** Descriptions:        设置硬币器高使能
** input parameters:    
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
*********************************************************************************************************/
uint8 MN_setCoinHighEnbale(uint8 type)
{
	uint8 isChanged = 0,isEdit = 0;
	uint8 key,topReturnFlag = 0;
	uint8 topFlush = 1;
	uint8 highEnable;
	
	
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			highEnable = stMdb.highEnable;
			LED_show("CE-%d",highEnable);
		}
		key = MN_getKey();
		switch(key){
			case '0':case '1':
				if(isEdit){
					highEnable = key - '0';
					LED_show("CE-%d",highEnable);
				}
				break;
			case 'E':
				if(isEdit){
					isEdit = 0;
					stMdb.highEnable = highEnable;
					isChanged = 1;
					topFlush = 1;
					LED_showString("####");
					msleep(200);
					FM_writeToFlash();					
				}
				else{
					LED_showString("####");
					msleep(200);
					LED_show("CE-0");
					isEdit = 1;
				}
				break;
			case 'C':
				if(isEdit > 0){
					isEdit = 0;
					topFlush = 1;
				}
				else{
					topReturnFlag = 1;
				}
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
** Function name:       MN_setCoinType
** Descriptions:        设置硬币器类型
** input parameters:    
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
*********************************************************************************************************/
uint8 MN_setCoinType(uint8 type)
{
	uint8 isChanged = 0,isEdit = 0;
	uint8 key,topReturnFlag = 0;
	uint8 topFlush = 1;
	uint8 coin_type;
	
	
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			coin_type = MDB_getCoinAcceptor();
			LED_show("C--%d",coin_type);
		}
		key = MN_getKey();
		switch(key){
			case '0':case '1':case '2':case '3':
				if(isEdit){
					coin_type = key - '0';
					LED_show("C--%d",coin_type);
				}
				break;
			case 'E':
				if(isEdit){
					isEdit = 0;
					MDB_setCoinAcceptor(coin_type);
					isChanged = 1;
					topFlush = 1;
					LED_showString("####");
					msleep(200);	
					FM_writeToFlash();
				}
				else{
					LED_showString("####");
					msleep(200);
					LED_show("C--0");
					isEdit = 1;
				}
				break;
			case 'C':
				if(isEdit > 0){
					isEdit = 0;
					topFlush = 1;
				}
				else{
					topReturnFlag = 1;
				}
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
** Function name:       MN_setBillRato
** Descriptions:        设置纸币兑币比例
** input parameters:    
** output parameters:   无
** Returned value:      1 配置有更改  0 配置无更改
*********************************************************************************************************/
uint8 MN_setBillRato(uint8 type,ST_CHANGE_RATO *rato)
{
	uint8 key,index = 0,topReturnFlag = 0;
	uint8 topFlush = 1;
	uint16 isChanged = 0;
	while(1){
		if(topFlush == 1){
			topFlush = 0;
			LED_show("A%02d.0",type);
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':
				LED_show("A%02d.%c",type,key);
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
	uint8 key,toFlush = 1,topReturnFlag = 0,menuKey = 0,enterSubMenu = 0;
	uint16 isChanged = 0;
	while(1){
		if(toFlush == 1){
			toFlush = 0;
			LED_showString("A00.0");
		}
		key = MN_getKey();
		switch(key){
			case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '0':
				menuKey = menuKey * 10 + key - '0';
				menuKey = (menuKey > 99) ? 00 : menuKey;
				LED_show("A%02d.0",menuKey);
				break;
			
			case 'E':
				if(menuKey){
					enterSubMenu = 1;	
					LED_showString("####");
					msleep(200);				
				}	
				break;
			case 'C':
				if(menuKey)
					menuKey = 0;
				else
					topReturnFlag = 1;
				toFlush = 1;
				break;
			
			
			default:break;
		}
		
		if(enterSubMenu){
			key = menuKey;
			enterSubMenu = 0;
			menuKey = 0;
			toFlush = 1;
			
			switch(key){
				case 1: //配置纸币器通道面值
					LED_showString("A01.0");
					isChanged += MN_setChannel(1);
					break;
				case 2: //配置硬币器通道面值
					LED_showString("A02.0");
					isChanged += MN_setChannel(2);
					break;
				case 3: //配置hopper通道面值
					LED_showString("A03.0");
					isChanged += MN_setChannel(3);
					//hopper 有改动 扫描 兑币比例
					//MN_scanRatio();
					if(isChanged > 0){
						print_menu("MENU:hopper changed...\r\n");
						HP_init();
						MT_devhopperFlush();
					}
					break;
				case 4: //配置小数点
					isChanged += MN_setPointValue(&stMdb.pointValue);
					break;
				case 5: //配置纸币兑零比例
					LED_showString("A05.0");
					isChanged += MN_setBillRato(5,stMdb.billRato);
					break;
				case 6: //配置硬币兑零比例
					LED_showString("A06.0");
					isChanged += MN_setBillRato(6,stMdb.coinRato);
					break;
				case 8: //配置硬币器类型
					isChanged += MN_setCoinType(8);
					break;
				case 9:
					isChanged += MN_setCoinHighEnbale(9);
					break;
				default:break;
			}
		}
		
		
		if(topReturnFlag == 1){
			if(isChanged > 0){
				
				return 1;
			}
			else{
				return 0;
			}
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
    while(1){
		key = MN_getKey();
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
		else{
			break;
		}
	}
	
	
	return 0;
}







