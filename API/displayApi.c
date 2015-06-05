/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2014-04-04
** Last Version:        No
** Descriptions:        系统参数定义及通用函数部分                     
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo
** Created date:        2013-04-04
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/

#include "..\config.h"


#define DEBUG_LED

#ifdef 	DEBUG_LED
#define print_led(...)	do{Trace("LED:");Trace(__VA_ARGS__);}while(0)
#else
#define print_led(...)
#endif

/*********************************************************************************************************
** Function name:       disp_clear_screen
** Descriptions:        清屏函数自动识别各种屏
** input parameters:   
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void initDisplay(void)
{
	InitDisplay();
}

/*********************************************************************************************************
** Function name:       disp_clear_screen
** Descriptions:        ???????????
** input parameters:   
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
void disp_clear_screen(void)
{

}

/*********************************************************************************************************
** Function name:       disp_sale_page
** Descriptions:        ????
** input parameters:   
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/

void disp_sale_page(unsigned char hasBuy,unsigned int amount,unsigned char point)
{
	unsigned int money;

		//money = amountTurnByPoint(amount);
		//????
		if(money>=10000)
		{
			DisplayDecimalNumberWithDot(money/10000,0);
			OSTimeDly(200);
			DisplayDecimalNumberWithDot(money%10000,point+3);
		}
		else
			DisplayDecimalNumberWithDot(money,point);
	
	

}


/*********************************************************************************************************
** Function name:       disp_free_page
** Descriptions:       空闲页面
** input parameters:   
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void disp_free_page(unsigned char vmcChangeLow)
{
	
		if(vmcChangeLow)
			DispLayExceptionInfomation(5);
		else
			DisplayDecimalNumberWithDot(0,SYSPara.nPoinValue);
	
	

	
}





/*********************************************************************************************************
** Function name:       disp_err_page
** Descriptions:        清屏函数自动识别各种屏
** input parameters:   
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void disp_err_page(void)
{
	
		if(HardWareErr & SYS_ERR_NO_BILL)
			led_err_display(LED_ERR_BILL);
		else if(HardWareErr & SYS_ERR_NO_HOPPER)//hopper有故障
		{
			if(hopperErrNo & HP_ERR_NO_HOPPER1_FAULT)
				LED_showString("HP1E");
			else if(hopperErrNo & HP_ERR_NO_HOPPER1_EMPTY)
				LED_showString("HP10");
			else if(hopperErrNo & HP_ERR_NO_HOPPER2_EMPTY)
				LED_showString("HP20");
			else if(hopperErrNo & HP_ERR_NO_HOPPER2_FAULT)
				LED_showString("HP2E");
			else 
				LED_showString("HPEE");
		}	
		else if(HardWareErr & SYS_ERR_NO_COIN)
			led_err_display(LED_ERR_COIN);
		
	
}

















/*********************************************************************************************************
** Function name:       disp_init_page
** Descriptions:         初始化页面
** input parameters:   type :1 初始化硬币器 2 初始化纸币器3找零器  4 初始化货道
**					
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void disp_init_page(unsigned char type)
{
		
	switch(type)
	{
		case 1:
	
				led_display(ledTab[LED_C],ledTab[LED_O],L8_7, L8_7);
			
			
			break;
		case 2:
			
				led_display(ledTab[LED_B],ledTab[LED_L],L8_7, L8_7);
			
			break;
		case 3:
			
				led_display(ledTab[LED_H],ledTab[LED_P],L8_7, L8_7);
		
			break;
		case 4:
			
				led_display(ledTab[LED_C], ledTab[LED_L],L8_7, L8_7);
			
			break;
		case 99://初始化完成
			
			break;
		default:
			break;
	}


	
}





void LED_showData(uint32 data)
{
	DisplayDecimalNumberWithDot(data,1);
}

/*********************************************************************************************************			
** Function name:           led_show
** Descriptions:        数码管显示
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void LED_showString(char *str)
{
	uint8 i,led[10] = {0},in = 0;
	uint8 len = strlen(str);
	for(i = 0;i < len;i++){
		switch(str[i]){
			case '0' : 	led[in] = ledTab[LED_O]; break;
			case '1' : 	led[in] = ledTab[LED_1]; break;
			case '2' : 	led[in] = ledTab[LED_2]; break;
			case '3' : 	led[in] = ledTab[LED_3]; break;
			case '4' : 	led[in] = ledTab[LED_4]; break;
			case '5' : 	led[in] = ledTab[LED_5]; break;
			case '6' : 	led[in] = ledTab[LED_6]; break;
			case '7' : 	led[in] = ledTab[LED_7]; break;
			case '8' : 	led[in] = ledTab[LED_8]; break;
			case '9' : 	led[in] = ledTab[LED_9]; break;
			case 'A' : 	led[in] = ledTab[LED_A]; break;
			case 'B' : 	led[in] = ledTab[LED_B]; break;
			case 'C' : 	led[in] = ledTab[LED_C]; break;
			case 'E' : 	led[in] = ledTab[LED_E]; break;
			case 'F' : 	led[in] = ledTab[LED_F]; break;
			case 'H' : 	led[in] = ledTab[LED_H]; break;
			case 'I' : 	led[in] = ledTab[LED_I]; break;
			case 'L' : 	led[in] = ledTab[LED_L]; break;
			case 'P' : 	led[in] = ledTab[LED_P]; break;
			case 'R' : 	led[in] = ledTab[LED_R]; break;
			case 'S' : 	led[in] = ledTab[LED_S]; break;
			case 'U' : 	led[in] = ledTab[LED_U]; break;
			case 'O' : 	led[in] = ledTab[LED_O]; break;
			case '.' :
				if(in == 0){
					led[0] |= L8_8;
				}
				else{
					led[in - 1] |= L8_8;
				}
				break;
			default: 	led[in] = ledTab[LED_OFF]; break;
		}

		in = (str[i] == '.') ? in : in + 1;
		
		
	}
	led_display(led[0],led[1],led[2],led[3]);
}








void ledPaomaDisplay(void)
{
	#if 0
	static unsigned char paomaIndex  = 1;
	//print_log("paoma.....timer = %d\r\n",Timer.led_paoma_timer);
	if(!Timer.led_paoma_timer)
	{
		//显示跑马灯
#if 0
		ledPaoMaShow(paomaIndex);
		if(paomaIndex >= 12)
			paomaIndex = 1;
		else
			paomaIndex++;
#endif
		ledPaoMaShow1(paomaIndex);
		if(paomaIndex >= 4)
			paomaIndex = 1;
		else
			paomaIndex++;	

		
		Timer.led_paoma_timer = 500/10;
	}						
	#endif
	
}

