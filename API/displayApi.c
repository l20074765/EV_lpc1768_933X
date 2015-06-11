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
#include "..\APP\Menu.h"

#define DEBUG_LED

#ifdef 	DEBUG_LED
#define print_led(...)	Trace(__VA_ARGS__)
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
	LED_showString(",,,0");
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
		LED_showData(5);
	else
		LED_showData(0);
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



/*********************************************************************************************************			
** Function name:           LED_showDataByPoint
** Descriptions:        	数码管显示数字自带小数点
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void LED_showDataByPoint(uint32 data,uint8 point)
{
	uint8 d[4] = {0},i = 0,ch;
	uint32 value = data;
	i = 0;
	if(point == 1){
		if(data < 10){
			led_display(0,0,ledTab[0] | L8_8,ledTab[data % 10]);
			return;
		}
	}
	else if(point == 2){
		if(data < 100){
			led_display(0,ledTab[0] | L8_8,ledTab[data / 10],ledTab[data % 10]);
			return;
		}
	}
	else if(point == 3){
		if(data < 1000){
			led_display(ledTab[0] | L8_8,ledTab[data / 100] ,ledTab[(data % 100) / 10],ledTab[data % 10]);
			return;
		}
	}
	else {
		if(data == 0){
			led_display(0,0,0,ledTab[0]);
			return;
		}
	}
	while(value > 0 && i < 4){
		ch = value % 10;
		d[i++] = ledTab[ch];
		value /= 10;
	}
	//插入小数点
	switch(point){
		case 1:d[1] |= L8_8;break;
		case 2:d[2] |= L8_8;break;
		case 3:d[3] |= L8_8;break;
		default:break;
	}
	led_display(d[3],d[2],d[1],d[0]);
}


/*********************************************************************************************************
** Programmer:		 led_display_large_digit		
** Function name:          
** Descriptions:        数码管显示滚动数据
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void LED_showLargeData(uint32 value,uint8 point)
{
	uint32 temp; 
	uint8 i = 0,num = 0,buf[30] ={0},v;//声请最高12位的数据
	uint8 key,returnFlag = 0;
	temp = value;
	num = 0;
	while(temp > 0){//将大数 拆分到数组中去
		v = temp % 10;
		temp /= 10;
		buf[num++] = ledTab[v];	
	}
	buf[num++] = 0;
	buf[num++] = 0;
	buf[num++] = 0;
	
	//插入小数点
	switch(point){
		case 1:buf[1] |= L8_8;break;
		case 2:buf[2] |= L8_8;break;
		case 3:buf[3] |= L8_8;break;
		default:break;
	}
	
	//  " ----0.4321"  这是一幅画 固定不动 假想让LED动
	for(i = num;i >= 4;i--){
		led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
		msleep(300);	
	}
	
}


/*********************************************************************************************************
** Programmer:		 LED_showLargeDataByPage		
** Function name:          
** Descriptions:        数码管显示滚动数据带翻页功能
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void LED_showLargeDataByPage(uint32 value,uint8 point)
{
	uint32 temp; 
	uint8 i = 0,num = 0,buf[30] ={0},v;//声请最高12位的数据
	uint8 key,returnFlag = 0;
	temp = value;
	num = 0;
	while(temp > 0){//将大数 拆分到数组中去
		v = temp % 10;
		temp /= 10;
		buf[num++] = ledTab[v];	
	}
	buf[num++] = 0;
	buf[num++] = 0;
	buf[num++] = 0;
	
	//插入小数点
	switch(point){
		case 1:buf[1] |= L8_8;break;
		case 2:buf[2] |= L8_8;break;
		case 3:buf[3] |= L8_8;break;
		default:break;
	}
	
	//  " ----0.4321"  这是一幅画 固定不动 假想让LED动
	for(i = num;i >= 4;i--){
		led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
		msleep(250);	
	}
	
	i = 4;
	while(1){
		key = MN_getKey();
		switch(key){
			case '>': //下翻
				if(i < num){
					i++;
					led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
				}
				else{
					led_display(0,0,0,0);
					msleep(300);
					led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
				}
				
				break;
			case '<': //上翻
				if(i > 4){
					i--;
					led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
				}
				else{
					led_display(0,0,0,0);
					msleep(300);
					led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
				}
				break;
			case 'E':
				led_display(0,0,0,0);
				msleep(300);
				led_display(buf[i - 1],buf[i - 2],buf[i - 3],buf[i - 4]);
				break;
			case 'C':
				led_display(0,0,0,0);
				msleep(500);
				returnFlag = 1;
				break;
			default:break;
		}
		if(returnFlag == 1){
			return;
		}
		msleep(50);
	}
		
}



/*********************************************************************************************************			
** Function name:           LED_showData
** Descriptions:        	数码管显示数字
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void LED_showData(uint32 data)
{
	if(data >= 10000){
		LED_showLargeData(data,0);
	}
	else{
		LED_showDataByPoint(data,0);
	}
}


/*********************************************************************************************************			
** Function name:           LED_showAmount
** Descriptions:        	数码管显示金额
** output parameters:       amount 分为单位
** Returned value:          none
*********************************************************************************************************/
void LED_showAmount(uint32 amount)
{
	if(amount >= 10000){
		LED_showLargeData(amount,stMdb.pointValue);
	}
	else{
		LED_showDataByPoint(amount,stMdb.pointValue);
	}
}


/*********************************************************************************************************			
** Function name:       led_show
** Descriptions:        数码管显示
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void LED_showString(char *str)
{
	uint8 i,led[10] = {0},in = 0;
	uint8 len = strlen(str);
	for(i = 0;i < len;i++){
		Trace("str[%d]=%c\r\n",i,str[i]);
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
			case '-' :	led[in] = L8_7; break;
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



void LED_show(char *format,...)
{
	va_list arg_ptr;
	uint8 buf[64] = {0},len;
	memset(buf,0,sizeof(buf));
	va_start(arg_ptr, format);
	len = vsprintf((char *)buf,(const char *)format,arg_ptr);
	va_end(arg_ptr);
	LED_showString((char *)buf);
	
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

