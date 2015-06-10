/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           display.h
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        display驱动程序头文件                  
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-04
** Version:              
** Descriptions:        The original version       
********************************************************************************************************/
#ifndef	__DISPLAY_H
#define	__DISPLAY_H


//=======================定义数码管故障码宏========================

#define LED_NO_ERR_0				20   //数码管故障显示"ERR.0" 下同
#define LED_NO_ERR_1				23
#define LED_NO_ERR_2				24
#define LED_NO_ERR_3				25
#define LED_NO_ERR_4				26
#define LED_NO_ERR_5				27
#define LED_NO_ERR_6				28
#define LED_NO_ERR_7				29
#define LED_NO_ERR_8				30





#define LED_ERR_HOPPER				0
#define LED_ERR_BILL				1
#define LED_ERR_COIN				2
#define LED_ERR_HUODAO				3


#if 0
#define LED_8_0  (0x00)
#define LED_8_1  (0x01 << 0)
#define LED_8_2  (0x01 << 1)
#define LED_8_3  (0x01 << 2)
#define LED_8_4  (0x01 << 3)
#define LED_8_5  (0x01 << 4)
#define LED_8_6  (0x01 << 5)
#define LED_8_7  (0x01 << 6)
#define LED_8_8  (0x01 << 7)

#define  LED_SPECIAL_A     0
#define  LED_SPECIAL_B     1
#define  LED_SPECIAL_C     2
#define  LED_SPECIAL_E     3
#define  LED_SPECIAL_F     4
#define  LED_SPECIAL_H     5
#define  LED_SPECIAL_I     6
#define  LED_SPECIAL_L     7
#define  LED_SPECIAL_P     8
#define  LED_SPECIAL_S     9
#define  LED_SPECIAL_U     10
#define  LED_SPECIAL_O     11

#define  LED_SPECIAL_OFF     19

#define  LED_1	  0x01
#define  LED_2	  0x02
#define  LED_3	  0x04
#define  LED_4	  0x08

#endif




extern const unsigned char ledTab[28];

/**********************************************************************************************************
*                                         数码管显示表数组标号
* 数字+ 字母像这种数码管应该用宏标记 增加代码阅读性 add by yoc
**********************************************************************************************************/           	         	 
#define  LED_0     0
#define  LED_1     1
#define  LED_2     2
#define  LED_3     3
#define  LED_4     4
#define  LED_5     5
#define  LED_6     6
#define  LED_7     7
#define  LED_8     8
#define  LED_9     9
#define  LED_A     10
#define  LED_B     11
#define  LED_C     12
#define  LED_E     13
#define  LED_F     14
#define  LED_H     15
#define  LED_I     16
#define  LED_L     17
#define  LED_P     18
#define  LED_R     19
#define  LED_S     20
#define  LED_U     21
#define  LED_O     22
#define  LED_OFF   23



/*****************************数码管8 位置最好在keil下面观看************************************************************
***********************************************************************************************************
**************************▕▔▔1▔▔▏*************************************************************	 	
**************************▕		 ▏*********************************************************************
************************** 6		 2 *********************************************************************
**************************▕		 ▏*********************************************************************
**************************▕▁▁7▁▁▏***************************************************************
**************************▕		 ▏*********************************************************************
**************************▕		 ▏*********************************************************************
************************** 5		 3 *********************************************************************
**************************▕		 ▏*********************************************************************
**************************▕▁▁4▁▁▏. 8 ************************************************************
**********************************************************************************************************/

#define L8_0  (0x00)
#define L8_1  (0x01 << 0) //1段
#define L8_2  (0x01 << 1)
#define L8_3  (0x01 << 2)
#define L8_4  (0x01 << 3)
#define L8_5  (0x01 << 4)
#define L8_6  (0x01 << 5)
#define L8_7  (0x01 << 6)
#define L8_8  (0x01 << 7)  //“.”









//=======================================================================


void InitDisplay(void);															//初始化数码管
void DisplayDecimalNumberWithDot(unsigned short Data,unsigned char DotPositon);	//显示十进制数字
void DosplaySettingMemu(unsigned char Memu,unsigned char Level);				//显示设置菜单
void DispLayExceptionInfomation(unsigned char Info);							//显示异常信息
void UserLedSet(unsigned char Mode);											//用户灯
void UserSeloutLedSet(unsigned char Mode);										//
void UserNochargeLedSet(unsigned char Mode);									//
void UserErrorLedSet(unsigned char Mode);										//
void displayUserMemuLED(unsigned char Memu,unsigned char subMenu);
void displayAdminMemuLED(unsigned char Memu,unsigned char subMenu);
void ledPaoMaShow(unsigned char type);
void ledPaoMaShow1(unsigned char type);

void led_dispaly_off(void);
void led_dispaly_mima(unsigned char type,unsigned char *num);

void led_err_display(unsigned char type);
void led_display(unsigned char led1,unsigned char led2,unsigned char led3,unsigned char led4);



#endif   
/**************************************End Of File*******************************************************/
