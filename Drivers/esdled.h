/*******************************************************************************************
								ģ�����ƣ�display
								ģ�鹦�ܣ�ʵ�ֶ��������ʾ�ؼ��Ŀ��Ʋ�������
								����ʱ�䣺2011-12-12
								������Ա������ΰ
********************************************************************************************/


#ifndef	__DISPLAY_H__
#define	__DISPLAY_H__

#include <reg51.h>

sbit tm1637_clk = P2^1; 
sbit tm1637_dio = P2^0;

sbit user_led			= P3^7;
sbit user_selout_led	= P3^4;
sbit user_nocharge_led	= P3^5;
sbit user_error_led		= P3^6;

/******************************�����API����***************************************/

bit DigitalDisplay( unsigned int DD,unsigned char DN,unsigned char DM );//��ʾ����
void InitDisplay( void );										//��ʼ���������ʾ

/****************************���LEDָʾ��API����**********************************/

void UserLedSet( unsigned char );		

void UserSeloutLedSet( unsigned char );

void UserNochargeLedSet( unsigned char );

void UserErrorLedSet( unsigned char );


#endif   

