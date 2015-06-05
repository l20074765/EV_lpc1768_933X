/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           Buzer.c
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        Buzzer��������                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-04
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

#define GPIO_Buzzer	(1ul << 30)	//ע����Ҫ��LPC1700PinCfg.c�н�P1.30������ΪGPIO
/*********************************************************************************************************
** Function name:     	InitBuzzer
** Descriptions:	    ������IO��ʼ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitBuzzer(void)
{
	FIO1DIR |= GPIO_Buzzer;
	FIO1CLR |= GPIO_Buzzer;	
}

/*********************************************************************************************************
** Function name:     	Buzzer()
** Descriptions:	    ��������һ��
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Buzzer(void)
{
	unsigned int i,j;
	for(j=50;j>0;j--)
	{
		FIO1DIR |= GPIO_Buzzer;
		FIO1SET |= GPIO_Buzzer;
		for(i=0;i<8500;i++);
		FIO1DIR |= GPIO_Buzzer;
		FIO1CLR |= GPIO_Buzzer;
		for(i=0;i<8500;i++);
	}	
}
/**************************************End Of File*******************************************************/
