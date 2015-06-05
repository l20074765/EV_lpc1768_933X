/*******************************************************************************************
								模块名称：display
								模块功能：实现对数码管显示控件的控制操作声明
								创建时间：2011-12-12
								开发人员：陈孙伟
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

/******************************数码管API函数***************************************/

bit DigitalDisplay( unsigned int DD,unsigned char DN,unsigned char DM );//显示函数
void InitDisplay( void );										//初始化数码管显示

/****************************面板LED指示灯API函数**********************************/

void UserLedSet( unsigned char );		

void UserSeloutLedSet( unsigned char );

void UserNochargeLedSet( unsigned char );

void UserErrorLedSet( unsigned char );


#endif   

