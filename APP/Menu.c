#include "..\config.h"
#include "Menu.h"

#define DEBUG_MENU

#ifdef 	DEBUG_MENU
#define print_menu(...)	do{Trace("MENU:");Trace(__VA_ARGS__);}while(0)
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



uint32 MN_getInputValue(uint32 value)
{
	uint8 key,returnFlag = 0;
	uint32 temp = 0;
	LED_showData(value);
	while(1){
		key = MN_getKey();
		switch(key){
			case '1': case '2': case '3': case '4': case '5':case '6':case '7':case '8':case '9':case '0':
				temp = temp * 10 + (key - '0');
				
			break;
			
			default:break;
		}
		
	}
}


/*********************************************************************************************************
** Function name:       setDeviceChannelValue
** Descriptions:        设置交易设备通道值菜单
** input parameters:    type 1纸币器 2硬币器 3hopper找零器
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void setDeviceChannelValue(uint8 type)
{
	uint8 key,enterEdit = 0,subKey,index = 0,topReturnFlag = 0,subMenu = 0,topIndex = 0;
	uint32 value;
	
	LED_showString("A01.0");
	while(1){
		key = MN_getKey();
		switch(key){
			case '1':
				LED_showString("A01.1");
				value = MN_getInputValue(value);
				break;
			case 0x0e:
				if(topIndex)
					topIndex = 0;
				else
					topReturnFlag = 1;

				print_log("topReturnFlag = %d\r\n",topReturnFlag);
				displayAdminMemuLED(menu,topIndex);
				break;
			case 0x0f://进入下一级菜单
				if(topIndex)
				{
					enterEdit = 0;
					if(type == 3 && topIndex > 3)
					{
						subMenu = 0;
						displayAdminMemuLED(menu,topIndex);
					}
					else
					{
						index = topIndex;
						if(type == 1)
							value = amountTurnByPoint(stDevValue.BillValue[index - 1]);
						else if(type == 2)
							value = amountTurnByPoint(stDevValue.CoinValue[index - 1]);
						else if(type == 3)
							value = amountTurnByPoint(stDevValue.HpValue[(index - 1) % 3]);
						else 
							value = 0;
						
						subMenu = 1;
						DisplayDecimalNumberWithDot(value,SYSPara.nPoinValue);
					}

					
				}
				print_log("subMenu = %d, index = %d\r\n",subMenu,index);	
				break;
			default:
				
				break;
		}
		
		while(subMenu)
		{
			
			subKey = GetKeyInValue();
			if(subKey == 0x0f)
			{
				enterEdit = 1;
				value = 0;
				DisplayDecimalNumberWithDot(0,0);	
			}
			else if(subKey == 0x0e)
			{	
				subMenu = 0;//退出
				displayAdminMemuLED(menu,topIndex);	
			}
			
				
			while(enterEdit)//进入编辑模式
			{
				subKey = GetKeyInValue();
				switch(subKey)
				{
					case 0x01: case 0x02: case 0x03: case 0x04: case 0x05:case 0x06:case 0x07:case 0x08:case 0x09:case 0x00:
						value = value * 10 + subKey;
						DisplayDecimalNumberWithDot(value,SYSPara.nPoinValue);
						break;
					case 0x0e://取消修改
						if(value)
						{
							DisplayDecimalNumberWithDot(0,0);
							value = 0;
						}
						else
						{
							enterEdit = 0;
							if(type == 1)
								value = amountTurnByPoint(stDevValue.BillValue[index - 1]);
							else if(type == 2)
								value = amountTurnByPoint(stDevValue.CoinValue[index - 1]);
							else if(type == 3)
								value = amountTurnByPoint(stDevValue.HpValue[(index - 1) % 3]);
							else
								value = 0;
							DisplayDecimalNumberWithDot(value,SYSPara.nPoinValue);
						}
							
						
						break;
					case 0x0f://确定修改
						led_dispaly_off();
						OSTimeDly(600/5);
						if(type == 1)
							stDevValue.BillValue[index - 1] = amountTurnTocents(value);
						else if(type == 2)
							stDevValue.CoinValue[index - 1] = amountTurnTocents(value);
						else if(type == 3)
							stDevValue.HpValue[(index - 1) % 3] = amountTurnTocents(value);
						
						enterEdit = 0;
						saveSystemParaFromFlash();
						DisplayDecimalNumberWithDot(value,SYSPara.nPoinValue);
						break;
					default:
						break;
				}
				
				OSTimeDly(10);	
			}
			
			OSTimeDly(10);	

		}

		if(topReturnFlag)
			return;
	
		OSTimeDly(10);
	
	}
		
	
		
}


/*********************************************************************************************************
** Function name:       MN_adminMenu
** Descriptions:        管理员菜单
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
uint8 MN_adminMenu(void)
{
	uint8 key,entered = 1;
	LED_showString("A00.0");
	while(entered){
		key = MN_getKey();
		switch(key){
			case '1': //配置纸币器通道面值
				LED_showString("A01.0");
				break;
			case '2': //配置硬币器通道面值
				LED_showString("A02.0");
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







