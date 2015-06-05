/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           RTC
** Last modified Date:  2013-09-12
** Last Version:         
** Descriptions:        RTCʵʱʱ����������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-03-04
** Version:             V2.0
** Descriptions:        The original version
**------------------------------------------------------------------------------------------------------
** Modified by:         
** Modified date:       
** Version:             
** Descriptions:        
********************************************************************************************************/
#include "..\config.h"

/*********************************************************************************************************
** Function name:       RTCStartOrStop
** Descriptions:        Start��Stop RTC
** input parameters:    cmd     1       ����RTC
**                              0       �ر�RTC
** Output parameters:   FALSE or TRUE
** Returned value:      NONE
*********************************************************************************************************/
void InitRtc(void)
{
    PCONP |= (1 << 9);
	if(RTC_AUX & (0x1<<4) )
	{
		RTC_AUX |= (0x1<<4);	
	}
	RTCCCR   = 0;                                          
    RTCILR   = 0x03;                                         
    RTCCIIR  = 0;                                          
    RTCAMR   = 0;
	RTCStartOrStop(1);
}
/*********************************************************************************************************
** Function name:       RTCStartOrStop
** Descriptions:        Start��Stop RTC
** input parameters:    cmd     1       ����RTC
**                              0       �ر�RTC
** Output parameters:   FALSE or TRUE
** Returned value:      NONE
*********************************************************************************************************/
unsigned char RTCStartOrStop(unsigned char cmd)
{
	if(cmd == 1)
	{
		RTCCCR |= 0x01;;
		RTCILR = 0x01;
	}
	else
	{
		RTCCCR &= ~0x01;
	}
	return 1;
}
/*********************************************************************************************************
** Function name:       RTCReadTime
** Descriptions:        ��ȡPCF8563,��YMWDHMSȫ������
** input parameters:    tp:ָ��ʱ��ṹ���ָ��
** Output parameters:   FALSE or TRUE
** Returned value:      NONE
*********************************************************************************************************/
unsigned char RTCReadTime(RTC_DATE *tp)
{
	tp->second = RTCSEC;
	tp->minute = RTCMIN;
	tp->hour   = RTCHOUR;
	tp->day    = RTCDOM;
	tp->week   = RTCDOW;
	tp->month  = RTCMONTH;
	tp->year   = RTCYEAR;
	//Trace("readRTC:%04d-%02d-%02d,%02d:%02d:%02d",tp->year,tp->month,tp->day,tp->hour,tp->minute,tp->second);
	return TRUE;
}
/*********************************************************************************************************
** Function name:       RTCSetTime
** Descriptions:        ����RTC,һ���趨YMWDHMSȫ��ֵ
** input parameters:    NONE
** Output parameters:   NONE
** Returned value:      NONE
*********************************************************************************************************/
unsigned char RTCSetTime(RTC_DATE *tp)
{
    RTCYEAR  = tp->year;
    RTCMONTH = tp->month;
    RTCDOW   = tp->week;
    RTCDOM   = tp->day;
    RTCHOUR  = tp->hour;
    RTCMIN   = tp->minute;
    RTCSEC   = tp->second;
	//Trace("setRTC:%04d-%02d-%02d,%02d:%02d:%02d",tp->year,tp->month,tp->day,tp->hour,tp->minute,tp->second);
	return TRUE;	
}
/*********************************************************************************************************
** Function name:       RTCSetYMWDHMS
** Descriptions:        ��������RTC��YMWDHMS
** input parameters:    cmd       Second         ������
**                                Minute         ���÷�
**                                Hour           ����Сʱ
**                                Day            ������
**                                Week           ��������
**                                Month          ������
**                                Year           ������
**                        TD        �趨ֵ
** Output parameters:   FALSE or TRUE
** Returned value:      NONE
*********************************************************************************************************/
unsigned char RTCSetYMWDHMS(unsigned char cmd,unsigned short TD)
{
	switch(cmd)
	{
		case RTCSecond:	RTCSEC = TD;
						break;
		case RTCMinute:	RTCMIN = TD;		
						break;
		case RTCHour:	RTCHOUR= TD;	
						break;
		case RTCDay:	RTCDOM = TD;	
						break;
		case RTCWeek:	RTCDOW = TD;	
						break;
		case RTCMonth:	RTCMONTH = TD;	
						break;
		case RTCYear:	RTCYEAR	= TD;
						break;
		default:break;		
	}
	return TRUE;	
}
/**************************************End Of File*******************************************************/