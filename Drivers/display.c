/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           display.c
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        display��������                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-04
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

#define GPIO_LED_OKK (1ul << 4)		//ע����Ҫ��LPC1700PinCfg.c�н�P2.4������ΪGPIO,�û�ָʾ��
#define GPIO_LED_SO	 (1ul << 5)		//ע����Ҫ��LPC1700PinCfg.c�н�P2.5������ΪGPIO,������
#define GPIO_LED_HE	 (1ul << 6)		//ע����Ҫ��LPC1700PinCfg.c�н�P2.6������ΪGPIO,Hopper����
#define GPIO_LED_BE	 (1ul << 7)		//ע����Ҫ��LPC1700PinCfg.c�н�P2.7������ΪGPIO,ֽ��������

#define I2C1_SDA	 (1ul << 19)	//ע����Ҫ��LPC1700PinCfg.c�н�P0.19������ΪGPIO,ģ��I2C����������
#define I2C1_SCL	 (1ul << 20)	//ע����Ҫ��LPC1700PinCfg.c�н�P0.20������ΪGPIO,ģ��I2C����ʱ����

#define SET_I2C1_SDA()	FIO0DIR |= I2C1_SDA; FIO0SET |= I2C1_SDA //ģ��I2C��������������
#define CLR_I2C1_SDA()	FIO0DIR |= I2C1_SDA; FIO0CLR |= I2C1_SDA //ģ��I2C��������������

#define SET_I2C1_SCL()	FIO0DIR |= I2C1_SCL; FIO0SET |= I2C1_SCL //ģ��I2C����ʱ��������
#define CLR_I2C1_SCL()	FIO0DIR |= I2C1_SCL; FIO0CLR |= I2C1_SCL //ģ��I2C����ʱ��������

volatile unsigned char TM1637_ACK;     
volatile unsigned char DIS_BUF[6];

/**********************************************************************************************************
*                                               ledTab
* Description : �������ʾ�� ����+ ��ĸ�����������Ӧ���ú��� ���Ӵ����Ķ��� add by yoc
**********************************************************************************************************/           	         	 
const unsigned char ledTab[28] = {
			 //0    1    2    3    4    5    6    7    8    9 
			  0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,
			 //A    B   C    E    F    H	I    L     P   R    S    U    O	   ��
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
** Descriptions:            ��ʾ4λʮ�������ݣ�����4λ��������(����ʾ)
** 							��DisplayDecimalNumberWithDot(999,0)����ʾ999����С����
** 							��DisplayDecimalNumberWithDot(9999,0)����ʾ999����С����
** 							��DisplayDecimalNumberWithDot(19991,0)����ʾ9991,��λ������
** 							��DisplayDecimalNumberWithDot(9999,1)����ʾ999.9,һΪС����
** 							��DisplayDecimalNumberWithDot(9999,2)����ʾ99.99,��λС����
** input parameters:        Data:Ҫ��ʾ�����֣�DotPositon:С����λ��,������λ��������λ����С���㴦��
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DisplayDecimalNumberWithDot(unsigned short Data,unsigned char DotPositon)
{
	unsigned char buf_temp[6];
	unsigned int temp = 0;
	//���ڴ���10000�����֣�ֻ��ʾǧ����
	if(Data >= 10000)
		temp = (Data%10000);
	else
		temp = Data;
	//����ǧλ������ܴ����ҵ�һλ
	if(temp/1000 == 0x00)								
		buf_temp[0] = 0x00;
	else
		buf_temp[0] = ledTab[temp/1000];
	//�����λ������ܴ����ҵڶ�λ
	if((temp%1000)/100 == 0x00)
	{							
		if(buf_temp[0] == 0x00)
			buf_temp[1] = 0x00;
		else
			buf_temp[1] = ledTab[0];
	}
	else
		buf_temp[1] = ledTab[(temp%1000)/100];
	//����ʮλ������ܴ����ҵ���λ
	if((temp%100)/10 == 0x00)
	{							
		if(buf_temp[1] == 0x00)
			buf_temp[2] = 0x00;
		else
			buf_temp[2] = ledTab[0];
	}
	else
		buf_temp[2] = ledTab[(temp%100)/10];
	//�����λ������ܴ����ҵ���λ
	buf_temp[3] = ledTab[temp%10];
	buf_temp[4]	= 0x00;
	buf_temp[5] = 0x00;
	//����С����
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
** Descriptions:            ��ʾ���ò˵�
** input parameters:        Memu:Ҫ��ʾ�˵��ţ�Level:0-���˵�������-�����˵�
** 							��:DosplaySettingMemu(0,0)����ʾF00.0
** 							��:DosplaySettingMemu(9,0)����ʾF09.0
** 							��:DosplaySettingMemu(0,1)����ʾF10.0
** 							��:DosplaySettingMemu(9,1)����ʾF19.0
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DosplaySettingMemu(unsigned char Memu,unsigned char Level)
{
	unsigned char DisBuf[6];
	switch(Level)
	{
		case 0:	//�������˵�,��ʽF00.0  LED_F0M_0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[Memu%10] | 0x80;
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 1:	//�����Ӳ˵�,��ʽF10.0  LED_F01_M
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[1] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 2:	//�����Ӳ˵�,��ʽF10.0  LED_F02_M
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[2] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 3:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[3] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 4:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[4] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 5:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[5] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 6:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[6] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 7:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[7] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 8:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[8] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 9:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[0];
				DisBuf[2] = ledTab[9] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 10:	//�����Ӳ˵�,��ʽF10.0  
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[1];
				DisBuf[2] = ledTab[0] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 11:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[1];
				DisBuf[2] = ledTab[1] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 12:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[2];
				DisBuf[2] = ledTab[2] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 13:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[3];
				DisBuf[2] = ledTab[3] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 14:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[4];
				DisBuf[2] = ledTab[4] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 15:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[5];
				DisBuf[2] = ledTab[5] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 16:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[6];
				DisBuf[2] = ledTab[6] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
	   	case 17:	//�����Ӳ˵�,��ʽF10.0
				DisBuf[0] = ledTab[LED_F];
				DisBuf[1] = ledTab[7];
				DisBuf[2] = ledTab[7] | 0x80;
				DisBuf[3] = ledTab[Memu%10];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 18:	//�����Ӳ˵�,��ʽF10.0
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
** Descriptions:          ������ܻ�ͼ
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
** Descriptions:         �������
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
** Descriptions:        �������ʾ
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
** Descriptions:        ����ܹ�����ʾ
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void led_err_display(unsigned char type)
{
	unsigned char DisBuf[6]; 


	switch(type)
	{
		case 0:// HPEE  hopper����
			DisBuf[0] = ledTab[LED_H];
			DisBuf[1] = ledTab[LED_P];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;
		case 1://BLEE   ֽ��������
			DisBuf[0] = ledTab[LED_B];
			DisBuf[1] = ledTab[LED_L];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;

		case 2://COEE   Ӳ��������
			DisBuf[0] = ledTab[LED_C];
			DisBuf[1] = ledTab[LED_O];
			DisBuf[2] = ledTab[LED_E];
			DisBuf[3] = ledTab[LED_E];
			break;

		case 3://��������
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
** Descriptions:          �����
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
** Descriptions:            ��ʾ�쳣��Ϣ
** input parameters:        Info:Ҫ��ʾ�쳣��Ϣ��,Info��0~6
** 							DispLayExceptionInfomation(0)����ʾHP1E,Hopper1����
** 							DispLayExceptionInfomation(1)����ʾHP2E,Hopper2����
** 							DispLayExceptionInfomation(3)����ʾHP3E,Hopper3����
** 							DispLayExceptionInfomation(4)����ʾHPEE,Hopper������
** 							DispLayExceptionInfomation(5)����ʾBLLE��ֽ��������
** 							DispLayExceptionInfomation(6)����ʾEEPE���洢������
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void DispLayExceptionInfomation(unsigned char Info)
{
	unsigned char DisBuf[6];
	switch(Info)
	{
		case 0:	//��ʾHoper1���ϣ���ʽHP1E---Hopper 1 Error
		case 1:	//��ʾHoper2���ϣ���ʽHP2E---Hopper 2 Error
		case 2: //��ʾHoper3���ϣ���ʽHP3E---Hopper 3 Error
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P];
				DisBuf[2] = ledTab[Info%10+1];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;

		case 3: //��ʾ����Hoper���ϣ���ʽHPEE---Hopper  Error
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P];
				DisBuf[2] = ledTab[LED_E];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 4://��ʾֽ�������ϣ� ��ʽBLLE---Billvalidator Error
				DisBuf[0] = ledTab[LED_B];
				DisBuf[1] = ledTab[LED_I];
				DisBuf[2] = ledTab[LED_L];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 5://�ۿռ�����ҿɶң���ʽHP50--Hopper Sale Out
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P];
				DisBuf[2] = ledTab[LED_5];
				DisBuf[3] = ledTab[LED_0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;	
		case 6://�洢������,��ʽEEPE---------EEPROM Errot
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_E];
				DisBuf[2] = ledTab[LED_P];
				DisBuf[3] = ledTab[LED_E];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 10:	//��ʾHoper1��������ʽHP.00---Hopper 1 OK
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P] | L8_8;
				DisBuf[2] = ledTab[0];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 11:	//��ʾHoper1��������ʽHP.00---Hopper 1 OK
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P] | L8_8;
				DisBuf[2] = ledTab[1];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 12:	//��ʾHoper1��������ʽHP.00---Hopper 1 OK
				DisBuf[0] = ledTab[LED_H];
				DisBuf[1] = ledTab[LED_P] | L8_8;
				DisBuf[2] = ledTab[2];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 20:	//��ʾ����ERR.0
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]| L8_8;
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 21:	//��ʾ������������ʽCL.00
				DisBuf[0] = ledTab[LED_C];
				DisBuf[1] = ledTab[LED_L]|0x80;
				DisBuf[2] = ledTab[0];
				DisBuf[3] = ledTab[0];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 22:	//��ʾ�������ϣ���ʽCL.ER
				DisBuf[0] = ledTab[LED_C];
				DisBuf[1] = ledTab[LED_L]|0x80;
				DisBuf[2] = ledTab[LED_E];
				DisBuf[3] = ledTab[LED_R];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 23:	//��ʾ����ERR.1
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[1];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 24:	//��ʾ����ERR.2
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[2];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case 25:	//��ʾ����ERR.3
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
		case LED_NO_ERR_4: // ʾ����ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[4];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
				
		case LED_NO_ERR_5: // ʾ����ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[5];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;

		case LED_NO_ERR_6: // ʾ����ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[6];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
				break;
		case LED_NO_ERR_7: // ʾ����ERR.4
				DisBuf[0] = ledTab[LED_E];
				DisBuf[1] = ledTab[LED_R];
				DisBuf[2] = ledTab[LED_R]|0x80;
				DisBuf[3] = ledTab[7];
				DisBuf[4] = 0x00;
				DisBuf[5] = 0x00;
				update_dis_buf(DisBuf);
				tm1637_display();
		break;
		case LED_NO_ERR_8: // ʾ����ERR.4
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
** Function name:           ���״̬��
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
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
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
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
** Function name:           �����״̬��
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
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
** Function name:           Hopper��ֽ�������ϵ�
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
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
** Descriptions:            ��ʾ��ʼ��������ȫ������ܣ���С����,�����ڼ��������Ƿ����
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
	tm1637_i2c_write_onebyte(0x8C); 				//����ʾ���������������
	tm1637_i2c_ack();    
	tm1637_i2c_stop();
	UserLedSet(1);		
	UserSeloutLedSet(0);
	UserNochargeLedSet(0);
	UserErrorLedSet(0);
	DisplayDecimalNumberWithDot(8888,0);

}
/*****************************************API END********************************************************/
 /*______________________________________Ӳ����������___________________________________________________*/
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           update_dis_buf
** Descriptions:            �����Դ�
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
** Descriptions:            �����ʱ,n��us
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
** Descriptions:            ���ģ�ⷽʽ����I2C����
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/    
void tm1637_i2c_start(void) 								//I2C���������ź�  
{   
	SET_I2C1_SCL();    
	SET_I2C1_SDA();   
	DelayNus(5);   		  
	CLR_I2C1_SDA(); 	//SDA|SCL��Ϊ�ߵ�ƽ�ڼ䣬SDA���������� 
	DelayNus(1); 
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           tm1637_i2c_ack
** Descriptions:            �����ģ�ⷽʽ����ACK
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/    
void tm1637_i2c_ack(void)  
{   
	CLR_I2C1_SCL();    
	DelayNus(1);   
	FIO0DIR &= ~I2C1_SDA;		//SDA����Ϊ����
	while(1) 					//�ȴ�ACK
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
** Descriptions:            ���ģ�ⷽʽֹͣI2C����
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/    
void tm1637_i2c_stop(void) 								//I2C���߽����ź�  
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
** Descriptions:            ��ģ��I2C���߷�ʽ����TM1637д��һ���ֽ�
** input parameters:        p:��Ҫд����ֽ�							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void tm1637_i2c_write_onebyte(unsigned char p)  		//��TM1637д��һ���ֽ� 
{   
	unsigned char i;      
	for(i=0;i<8;i++)									//��ʼ����8λ����    
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
	}													//8λ���ݴ�����1    
}
/*********************************************************************************************************
** Programmer:				sunway
** Function name:           DelayNns
** Descriptions:            �����ʱ
** input parameters:        none:							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/   
void tm1637_display(void)   
{   
	unsigned char i=0;
	OS_ENTER_CRITICAL();							//�����ٽ���
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
	OS_EXIT_CRITICAL();								//�˳��ٽ���
}
/**************************************End Of File*******************************************************/
