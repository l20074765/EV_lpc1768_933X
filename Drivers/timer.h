/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           timer.c
** Last modified Date:  2013-01-07
** Last Version:         
** Descriptions:        定时器驱动程序头文件                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-07
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#ifndef __TIMER_H 
#define __TIMER_H


//==============================??????????========================================================
//???????unsigned short ?,????????


typedef struct _timer_st_{
	unsigned short timer_bill_reset;
	unsigned short timer_coin_reset;
	unsigned short bill_escrow_timeout;
	unsigned short hopper_recv_timeout;
	unsigned short hopper_payout_timeout;
	unsigned short hopper_check_timeout;
	unsigned short mdb_timeout;
	unsigned short usr_opt;
	unsigned short dev_msg_rpt;
	unsigned short led_paoma;
	unsigned short pcoin_opt;
	unsigned short coin_opt;
}TIMER_ST;
extern TIMER_ST Timer;



void InitTimer(unsigned char TimerNumb,unsigned int TimerInterval);
void TIMER0_IRQHandler (void);
void TIMER1_IRQHandler (void);
void TIMER2_IRQHandler (void);
void TIMER3_IRQHandler (void);

#endif
/**************************************End Of File*******************************************************/
