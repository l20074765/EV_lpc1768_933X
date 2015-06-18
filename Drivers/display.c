/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           display.c
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        display驱动程序                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-04
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

#define GPIO_LED_OKK (1ul << 4)		//注意需要在LPC1700PinCfg.c中将P2.4脚设置为GPIO,用户指示灯
#define GPIO_LED_SO	 (1ul << 5)		//注意需要在LPC1700PinCfg.c中将P2.5脚设置为GPIO,已售完
#define GPIO_LED_HE	 (1ul << 6)		//注意需要在LPC1700PinCfg.c中将P2.6脚设置为GPIO,Hopper故障
#define GPIO_LED_BE	 (1ul << 7)		//注意需要在LPC1700PinCfg.c中将P2.7脚设置为GPIO,纸币器故障

#define I2C1_SDA	 (1ul << 19)	//注意需要在LPC1700PinCfg.c中将P0.19脚设置为GPIO,模拟I2C总线数据线
#define I2C1_SCL	 (1ul << 20)	//注意需要在LPC1700PinCfg.c中将P0.20脚设置为GPIO,模拟I2C总线时钟线

#define SET_I2C1_SDA()	FIO0DIR |= I2C1_SDA; FIO0SET |= I2C1_SDA //模拟I2C总线数据线拉高
#define CLR_I2C1_SDA()	FIO0DIR |= I2C1_SDA; FIO0CLR |= I2C1_SDA //模拟I2C总线数据线拉低

#define SET_I2C1_SCL()	FIO0DIR |= I2C1_SCL; FIO0SET |= I2C1_SCL //模拟I2C总线时钟线拉高
#define CLR_I2C1_SCL()	FIO0DIR |= I2C1_SCL; FIO0CLR |= I2C1_SCL //模拟I2C总线时钟线拉低

volatile unsigned char TM1637_ACK;     
volatile unsigned char DIS_BUF[6];

/**********************************************************************************************************
*                                               ledTab
* Description : 数码管显示表 数字+ 字母像这种数码管应该用宏标记 增加代码阅读性 add by yoc
**********************************************************************************************************/           	         	 
const unsigned char ledTab[28] = {
			 //0    1    2    3    4    5    6    7    8    9 
			  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
			 //A    B   C    E    F    H	I    L     P   R    S    U    O	   关
			 0x77,0x7F,0x39,0x79,0x71,0x76,0x30,0x38,0x73,0x77,0x6D,0x3E,0x3F,0x00,
			 
			 0x00,0x00,0x00,0x00
			 };



void tm1637_i2c_start(void);
void tm1637_i2c_ack(void);
void tm1637_i2c_stop(void);
void tm1637_display(void);
void update_dis_buf(unsigned char *buf);
void tm1637_i2c_write_onebyte(unsigned char p);

/*********************************************************************************************************
** Programmer:				sunway
** Function name:           DisplayDecimalNumberWithDot
** Descriptions:            显示4位十进制数据，超过4位部分弃掉(不显示)
** 							如DisplayDecimalNumberWithDot(999,0)则显示999，无小数点
** 							如DisplayDecimalNumberWithDot(9999,0)则显示999，无小数点
** 							如DisplayDecimalNumberWithDot(19991,0)则显示9991,万位被丢弃
** 							如DisplayDecimalNumberWithDot(9999,1)则显示999.9,一为小数点
** 							如DisplayDecimalNumberWithDot(9999,2)则显示99.99,两位小数点
** input parameters:        Data:要显示的数字；DotPositon:小数点位置,至多两位，超过两位按无小数点处理
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DisplayDecimalNumberWithDot(unsigned short Data,unsigned char DotPositon)
{
	unsigned char buf_temp[6];
	unsigned int temp = 0;
	//对于大于10000的数字，只显示千以下
	if(Data >= 10000)
		temp = (Data%10000);
	else
		temp = Data;
	//计算千位，数码管从左到右第一位
	if(temp/1000 == 0x00)								
		buf_temp[0] = 0x00;
	else
		buf_temp[0] = ledTab[temp/1000];
	//计算百位，数码管从左到右第二位
	if((temp%1000)/100 == 0x00)
	{							
		if(buf_temp[0] == 0x00)
			buf_temp[1] = 0x00;
		else
			buf_temp[1] = ledTab[0];
	}
	else
		buf_temp[1] = ledTab[(temp%1000)/100];
	//计算十位，数码管从左到右第三位
	if((temp%100)/10 == 0x00)
	{							
		if(buf_temp[1] == 0x00)
			buf_temp[2] = 0x00;
		else
			buf_temp[2] = ledTab[0];
	}
	else
		buf_temp[2] = ledTab[(temp%100)/10];
	//计算个位，数码管从左到右第四位
	buf_temp[3] = ledTab[temp%10];
	buf_temp[4]	= 0x00;
	buf_temp[5] = 0x00;
	//处理小数点
	if(DotPositon == 1)
	{
		if(buf_temp[2]!=0)
			buf_temp[2] |= 0x80;
		else
			buf_temp[2] = ledTab[0]|0x80;
	}
	if(DotPositon == 2)
	{
		if(buf_temp[1]!=0)
			buf_temp[1] |= 0x80;
		else
		{
			buf_temp[1] = ledTab[0]|0x80;
			if(buf_temp[2] == 0)
				buf_temp[2] = ledTab[0];
		}
	}
	if(DotPositon==3)
	{
		if(buf_temp[0]==0)
			buf_temp[0] = ledTab[0];
		if(buf_temp[1]==0)
			buf_temp[1] = ledTab[0];	
		if(buf_temp[2]==0)
			buf_temp[2] = ledTab[0];
		if(buf_temp[3]==0)
			buf_temp[3] = ledTab[0];
	}
	if(DotPositon==4)
	{
		if(buf_temp[0]==0)
			buf_temp[0] = ledTab[0];
		if(buf_temp[1]==0)
			buf_temp[1] = ledTab[0];	
		if(buf_temp[2]==0)
			buf_temp[2] = ledTab[0]|0x80;
		else
			buf_temp[2] |= 0x80;
		if(buf_temp[3]==0)
			buf_temp[3] = ledTab[0];
	}
	if(DotPositon==5)
	{
		if(buf_temp[0]==0)
			buf_temp[0] = ledTab[0];
		if(buf_temp[1]==0)
			buf_temp[1] = ledTab[0]|0x80;
		else
			buf_temp[1] |= 0x80;		
		if(buf_temp[2]==0)
			buf_temp[2] = ledTab[0];
		if(buf_temp[3]==0)
			buf_temp[3] = ledTab[0];
	}
	if(DotPositon == 10)
	{
		buf_temp[0] = ledTab[1];	
		buf_temp[1] = ledTab[2]|0x80;
		buf_temp[2] = ledTab[temp/10];
		buf_temp[3] = ledTab[temp%10];
	}
	update_dis_buf(buf_temp);
	tm1637_display();
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           DosplaySettingMemu
** Descriptions:            显示设置菜单
** input parameters:        Memu:要显示菜单号；Level:0-主菜单；其他-二级菜单
** 							如:DosplaySettingMemu(0,0)则显示F00.0
** 							如:DosplaySettingMemu(9,0)则显示F09.0
** 							如:DosplaySettingMemu(0,1)则显示F10.0
** 							如:DosplaySettingMemu(9,1)则显示F19.0
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DosplaySettingMemu(unsigned char Memu,unsigned char Level)
{
	unsigned char DisBuf[6];
	switch(Level)
	{
		case 0:	//处理主菜单,格式F00.0  LED_F0M_0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[Memu%10] | 0x80;
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 1:	//处理子菜单,格式F10.0  LED_F01_M
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[1] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 2:	//处理子菜单,格式F10.0  LED_F02_M
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[2] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 3:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[3] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 4:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[4] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 5:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[5] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 6:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[6] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 7:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[7] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 8:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[8] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 9:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[9] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 10:	//处理子菜单,格式F10.0  
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[1];
				DisBuf[2] = ledTab[0] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 11:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[1];
				DisBuf[2] = ledTab[1] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 12:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[2];
				DisBuf[2] = ledTab[2] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 13:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[3];
				DisBuf[2] = ledTab[3] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 14:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[4];
				DisBuf[2] = ledTab[4] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 15:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[5];
				DisBuf[2] = ledTab[5] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 16:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[6];
				DisBuf[2] = ledTab[6] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
	   	case 17:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[7];
				DisBuf[2] = ledTab[7] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 18:	//处理子菜单,格式F10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[8];
				DisBuf[2] = ledTab[8] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		default:break;
	}
}



/*********************************************************************************************************
** Programmer:				
** Function name:           drawByLed
** Descriptions:          用数码管绘图
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void drawByLed(unsigned char type,unsigned char ledNo)
{
	unsigned char DisBuf[6];
		
	if(ledNo & LED_1)
		DisBuf[0] = ledTab[type];
	if(ledNo & LED_2)
		DisBuf[1] = ledTab[type];
	if(ledNo & LED_2)
		DisBuf[2] = ledTab[type];
	if(ledNo & LED_2)
		DisBuf[3] = ledTab[type];

	DisBuf[4] = 0x00;
	DisBuf[5] = 0x00;
	update_dis_buf(DisBuf);
	tm1637_display();

	
	
}


/*********************************************************************************************************
** Programmer:				
** Function name:           led_dispaly_off
** Descriptions:         关数码管
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void led_dispaly_off(void)
{
	unsigned char DisBuf[6];

	
	DisBuf[0] = 0x00;
	DisBuf[1] = 0x00;
	DisBuf[2] = 0x00;
	DisBuf[3] = 0x00;
	DisBuf[4] = 0x00;
	DisBuf[5] = 0x00;
	update_dis_buf(DisBuf);
	tm1637_display();
}




/*********************************************************************************************************
** Programmer:				
** Function name:           led_display
** Descriptions:        数码管显示
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void led_display(unsigned char led1,unsigned char led2,unsigned char led3,unsigned char led4)
{
	unsigned char DisBuf[6]; 
	DisBuf[0] = led1;
	DisBuf[1] = led2;
	DisBuf[2] = led3;
	DisBuf[3] = led4;
	DisBuf[4] = 0x00;
	DisBuf[5] = 0x00;
	update_dis_buf(DisBuf);
	tm1637_display();
}








/*********************************************************************************************************
** Programmer:				
** Function name:           led_err_display
** Descriptions:        数码管故障显示
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void led_err_display(unsigned char type)
{
	unsigned char DisBuf[6]; 


	switch(type)
	{
		case 0:// HPEE  hopper故障
			DisBuf[0] = ledTab[LED_H];
			DisBuf[1] = ledTab[LED_P];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;
		case 1://BLEE   纸币器故障
			DisBuf[0] = ledTab[LED_B];
			DisBuf[1] = ledTab[LED_L];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;

		case 2://COEE   硬币器故障
			DisBuf[0] = ledTab[LED_C];
			DisBuf[1] = ledTab[LED_O];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;

		case 3://货道故障
			DisBuf[0] = ledTab[LED_C];
			DisBuf[1] = ledTab[LED_L];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;

		default:
			DisBuf[0] = ledTab[LED_OFF];
			DisBuf[1] = ledTab[LED_OFF];
			DisBuf[2] = ledTab[LED_OFF];
			DisBuf[3] = ledTab[LED_OFF];
			break;
	}
	


	DisBuf[4] = 0x00;
	DisBuf[5] = 0x00;
	update_dis_buf(DisBuf);
	tm1637_display();
}






/*********************************************************************************************************
** Programmer:				
** Function name:           ledPaoMaShow
** Descriptions:          跑马灯
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void ledPaoMaShow(unsigned char type)
{
	unsigned char DisBuf[6];
	switch(type)
	{
		case 1:
			DisBuf[0] =  L8_6 | L8_5 | L8_4;
			DisBuf[1] =  L8_4;
			DisBuf[2] =  L8_0;
			DisBuf[3] =  L8_0;
			break;
		case 2:
			DisBuf[0] =  L8_5 | L8_4;
			DisBuf[1] =  L8_4 ;
			DisBuf[2] =  L8_4;
			DisBuf[3] =  L8_0;
			break;
			
		case 3:
			DisBuf[0] = L8_4;
			DisBuf[1] = L8_4;
			DisBuf[2] = L8_4;
			DisBuf[3] = L8_4;
			break;
		case 4:
			DisBuf[0] = L8_0;
			DisBuf[1] = L8_4;
			DisBuf[2] = L8_4;
			DisBuf[3] = L8_4 | L8_3 ;
			break;
		case 5:
			DisBuf[0] = L8_0;
			DisBuf[1] = L8_0;
			DisBuf[2] = L8_4;
			DisBuf[3] = L8_4 | L8_3 | L8_2 ;
			break;
		case 6:
			DisBuf[0] = L8_0;
			DisBuf[1] = L8_0;
			DisBuf[2] = L8_0;
			DisBuf[3] = L8_4 | L8_3 | L8_2 | L8_1 ;
			break;
		case 7:
			DisBuf[0] = L8_0;
			DisBuf[1] = L8_0;
			DisBuf[2] = L8_1;
			DisBuf[3] = L8_3 | L8_2 | L8_1;
			break;
		case 8:
			DisBuf[0] = L8_0;
			DisBuf[1] = L8_1;
			DisBuf[2] = L8_1;
			DisBuf[3] = L8_2 | L8_1;
			break;
		case 9:
			DisBuf[0] = L8_1;
			DisBuf[1] = L8_1;
			DisBuf[2] = L8_1;
			DisBuf[3] = L8_1 ;
			break;		
		case 10:
			DisBuf[0] = L8_6 | L8_1;
			DisBuf[1] = L8_1;
			DisBuf[2] = L8_1;
			DisBuf[3] = L8_0 ;
			break;		
		case 11:
			DisBuf[0] = L8_6 | L8_5 | L8_1;
			DisBuf[1] = L8_1;
			DisBuf[2] = L8_0;
			DisBuf[3] = L8_0;
			break;		
		case 12:
			DisBuf[0] = L8_6 | L8_5 | L8_4 | L8_1;
			DisBuf[1] = L8_0;
			DisBuf[2] = L8_0;
			DisBuf[3] = L8_0;
			break;
		default:
			DisBuf[0] = 0x00;
			DisBuf[1] = 0x00;
			DisBuf[2] = 0x00;
			DisBuf[3] = 0x00;
			break;
	}
	
	DisBuf[4] = 0x00;
	DisBuf[5] = 0x00;
	update_dis_buf(DisBuf);
	tm1637_display();

	
	
}




/*********************************************************************************************************
** Programmer:				sunway
** Function name:           DispLayExceptionInfomation
** Descriptions:            显示异常信息
** input parameters:        Info:要显示异常信息号,Info在0~6
** 							DispLayExceptionInfomation(0)则显示HP1E,Hopper1故障
** 							DispLayExceptionInfomation(1)则显示HP2E,Hopper2故障
** 							DispLayExceptionInfomation(3)则显示HP3E,Hopper3故障
** 							DispLayExceptionInfomation(4)则显示HPEE,Hopper都故障
** 							DispLayExceptionInfomation(5)则显示BLLE，纸币器故障
** 							DispLayExceptionInfomation(6)则显示EEPE，存储器故障
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DispLayExceptionInfomation(unsigned char Info)
{
	unsigned char DisBuf[6];
	switch(Info)
	{
		case 0:	//显示Hoper1故障，格式HP1E---Hopper 1 Error
		case 1:	//显示Hoper2故障，格式HP2E---Hopper 2 Error
		case 2: //显示Hoper3故障，格式HP3E---Hopper 3 Error
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P];
				DisBuf[2] = ledTab[Info%10+1];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;

		case 3: //显示所有Hoper故障，格式HPEE---Hopper  Error
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P];
				DisBuf[2] = ledTab[LED_E];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 4://显示纸币器故障， 格式BLLE---Billvalidator Error
				DisBuf[0] = ledTab[LED_B];
				DisBuf[1] = ledTab[LED_I];
				DisBuf[2] = ledTab[LED_L];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 5://售空即无零币可兑，格式HP50--Hopper Sale Out
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P];
				DisBuf[2] = ledTab[LED_5];
				DisBuf[3] = ledTab[LED_0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;	
		case 6://存储器故障,格式EEPE---------EEPROM Errot
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_E];
				DisBuf[2] = ledTab[LED_P];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 10:	//显示Hoper1正常，格式HP.00---Hopper 1 OK
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P] | L8_8;
				DisBuf[2] = ledTab[0];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 11:	//显示Hoper1正常，格式HP.00---Hopper 1 OK
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P] | L8_8;
				DisBuf[2] = ledTab[1];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 12:	//显示Hoper1正常，格式HP.00---Hopper 1 OK
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P] | L8_8;
				DisBuf[2] = ledTab[2];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 20:	//显示故障ERR.0
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]| L8_8;
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 21:	//显示货道正常，格式CL.00
				DisBuf[0] = ledTab[LED_C];
				DisBuf[1] = ledTab[LED_L]|0x80;
				DisBuf[2] = ledTab[0];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 22:	//显示货道故障，格式CL.ER
				DisBuf[0] = ledTab[LED_C];
				DisBuf[1] = ledTab[LED_L]|0x80;
				DisBuf[2] = ledTab[LED_E];
				DisBuf[3] = ledTab[LED_R];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 23:	//显示故障ERR.1
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[1];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 24:	//显示故障ERR.2
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[2];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 25:	//显示故障ERR.3
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[3];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		//add by yoc
		case LED_NO_ERR_4: // 示故障ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[4];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
				
		case LED_NO_ERR_5: // 示故障ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[5];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;

		case LED_NO_ERR_6: // 示故障ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[6];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case LED_NO_ERR_7: // 示故障ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[7];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
		break;
		case LED_NO_ERR_8: // 示故障ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[8];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		
		default:break;
	}	
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           面板状态灯
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserLedSet(unsigned char Mode)
{
	if(Mode > 0)
	{
		FIO2DIR |= GPIO_LED_OKK;
		FIO2SET |= GPIO_LED_OKK;	
	}
	else
	{
		FIO2DIR |= GPIO_LED_OKK;
		FIO2CLR |= GPIO_LED_OKK;
	}
}		
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           Ho
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserSeloutLedSet(unsigned char Mode)
{
	if(Mode > 0)
	{
		FIO2DIR |= GPIO_LED_SO;
		FIO2SET |= GPIO_LED_SO;	
	}
	else
	{
		FIO2DIR |= GPIO_LED_SO;
		FIO2CLR |= GPIO_LED_SO;
	}
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           无零币状态灯
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserNochargeLedSet(unsigned char Mode)
{
	if(Mode > 0)
	{
		FIO2DIR |= GPIO_LED_HE;
		FIO2SET |= GPIO_LED_HE;	
	}
	else
	{						 
		FIO2DIR |= GPIO_LED_HE;
		FIO2CLR |= GPIO_LED_HE;
	}
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           Hopper或纸币器故障灯
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserErrorLedSet(unsigned char Mode)
{
	if(Mode > 0)
	{
		FIO2DIR |= GPIO_LED_BE;
		FIO2SET |= GPIO_LED_BE;	
	}
	else
	{
		FIO2DIR |= GPIO_LED_BE;
		FIO2CLR |= GPIO_LED_BE;
	}
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           init_display
** Descriptions:            显示初始化，点亮全部数码管，含小数点,可用于检测数码管是否故障
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void InitDisplay(void)
{
	unsigned char i;
	for(i=0;i<6;i++)
	{
		DIS_BUF[i] = 0x3F;
	}
	tm1637_display();
	tm1637_i2c_start();   
	tm1637_i2c_write_onebyte(0x8C); 				//打开显示并把亮度跳到最大
	tm1637_i2c_ack();    
	tm1637_i2c_stop();
	UserLedSet(1);		
	UserSeloutLedSet(0);
	UserNochargeLedSet(0);
	UserErrorLedSet(0);
	DisplayDecimalNumberWithDot(8888,0);

}
/*****************************************API END********************************************************/
 /*______________________________________硬件驱动程序___________________________________________________*/
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           update_dis_buf
** Descriptions:            更新显存
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void update_dis_buf(unsigned char * buf)
{
	unsigned char i;
	for(i=0;i<6;i++)
	{
		DIS_BUF[i] = *buf;
		buf++;
	}
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           DelayNus
** Descriptions:            软件延时,n个us
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DelayNus(unsigned long n)
{
	unsigned long i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<25;j++)
		{
			__nop();
		}	
	}
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           tm1637_i2c_start
** Descriptions:            软件模拟方式启动I2C总线
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/    
void tm1637_i2c_start(void) 								//I2C总线启动信号  
{   
	SET_I2C1_SCL();    
	SET_I2C1_SDA();   
	DelayNus(5);   		  
	CLR_I2C1_SDA(); 	//SDA|SCL都为高电平期间，SDA下拉；启动 
	DelayNus(1); 
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           tm1637_i2c_ack
** Descriptions:            以软件模拟方式接收ACK
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/    
void tm1637_i2c_ack(void)  
{   
	CLR_I2C1_SCL();    
	DelayNus(1);   
	FIO0DIR &= ~I2C1_SDA;		//SDA设置为输入
	while(1) 					//等待ACK
	{
		if((FIO0PIN & I2C1_SDA) == 0)
			break;
	}
	SET_I2C1_SCL();
	DelayNus(3);
	CLR_I2C1_SCL();   
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           tm1637_i2c_stop
** Descriptions:            软件模拟方式停止I2C总线
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/    
void tm1637_i2c_stop(void) 								//I2C总线结束信号  
{   
	SET_I2C1_SCL();    
	CLR_I2C1_SDA();   
	DelayNus(5);   
	SET_I2C1_SDA();
	DelayNus(1);   
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           tm1637_i2c_write_onebyte
** Descriptions:            以模拟I2C总线方式，向TM1637写入一个字节
** input parameters:        p:需要写入的字节							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void tm1637_i2c_write_onebyte(unsigned char p)  		//向TM1637写入一个字节 
{   
	unsigned char i;      
	for(i=0;i<8;i++)									//开始传送8位数据    
	{   
		CLR_I2C1_SCL();
		DelayNus(3);   
		if((p&0x01) != 0)   
		{   
			SET_I2C1_SDA();   
		}   
		else   
		{   
			CLR_I2C1_SDA();   
		}   
		DelayNus(3);
		p = p>>1; 
		SET_I2C1_SCL();
		DelayNus(3);  
	}													//8位数据传送完1    
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           DelayNns
** Descriptions:            软件延时
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/   
void tm1637_display(void)   
{   
	unsigned char i=0;
	OS_ENTER_CRITICAL();							//进入临界区
	tm1637_i2c_start();
	tm1637_i2c_write_onebyte(0x40);
	tm1637_i2c_ack();
	tm1637_i2c_stop();
	tm1637_i2c_start();
	tm1637_i2c_write_onebyte(0xc0);
	tm1637_i2c_ack();
	for(i=0;i<6;i++)   
	{   
		tm1637_i2c_write_onebyte(DIS_BUF[i]);
		tm1637_i2c_ack();	
	}
	tm1637_i2c_stop();
	OS_EXIT_CRITICAL();								//退出临界区
}
/**************************************End Of File*******************************************************/
