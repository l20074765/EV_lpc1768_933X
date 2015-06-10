/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           timer.c
** Last modified Date:  2013-01-07
** Last Version:         
** Descriptions:        ��ʱ����������                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-07
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"
TIMER_ST Timer;
static unsigned short timerSize = sizeof(TIMER_ST) / 2;
static unsigned short timer_i;
static unsigned short *timer_ptr = (unsigned short *)&Timer;


extern volatile unsigned int EVBCONVERSATIONWAITACKTIMEOUT;


extern volatile unsigned int UART3RECVACKMSGTIMEOUT;
extern volatile uint16_t RS232WAITRECVPACKTIME;
extern volatile unsigned int TestPluse;
extern volatile uint16_t Hptime1,Hptime2,Hptime3;


extern volatile uint16_t WaitCmdTimer;

extern volatile unsigned short HpHandleTimer;






/*********************************************************************************************************
** Function name:     	InitTimer
** Descriptions:	    LPC1768�ڲ�4����ʱ���ĳ�ʼ��
** input parameters:    TimerNumb:��ʱ����(0~3);TimerInterval��ʱʱ�����
** output parameters:   ��
** Returned value:      ��
** ��ʱʱ����㷽����	��ʱʱ��(Second) = (TimerInterval*(PR+1))/FPCLK,����FPCLK = 24000000,PR = 0;
						�綨ʱ10ms(0.01s),��TimerInterval = 240000
*********************************************************************************************************/
void InitTimer(unsigned char TimerNumb,unsigned int TimerInterval)
{
	switch(TimerNumb)
	{
		case 0:	T0TCR	= 0x02;
				T0IR	= 1;
				T0CTCR	= 0;
				T0TC	= 0;
				T0PR	= 0;
				T0MR0	= TimerInterval;
				T0MCR	= 0x03;
				zyIsrSet(NVIC_TIMER0,(unsigned long)TIMER0_IRQHandler,PRIO_TWO);
				T0TCR	= 0x01;
				break;
		case 1:	T1TCR	= 0x02;
				T1IR	= 1;
				T1CTCR	= 0;
				T1TC	= 0;
				T1PR	= 0;
				T1MR0	= TimerInterval;
				T1MCR	= 0x03;
				zyIsrSet(NVIC_TIMER1,(unsigned long)TIMER1_IRQHandler,PRIO_ELEVEN);
				T1TCR	= 0x01;
				break;
		case 2:	T2TCR	= 0x02;
				T2IR	= 1;
				T2CTCR	= 0;
				T2TC	= 0;
				T2PR	= 0;
				T2MR0	= TimerInterval;
				T2MCR	= 0x03;
				zyIsrSet(NVIC_TIMER2,(unsigned long)TIMER2_IRQHandler,PRIO_TWELVE);
				T2TCR	= 0x01;
				break;
		case 3:	T3TCR	= 0x02;
				T3IR	= 1;
				T3CTCR	= 0;
				T3TC	= 0;
				T3PR	= 0;
				T3MR0	= TimerInterval;
				T3MCR	= 0x03;
				zyIsrSet(NVIC_TIMER3,(unsigned long)TIMER3_IRQHandler,PRIO_THIRTEEN);
				T3TCR	= 0x01;
				break;
		default:break;
	}
}
/*********************************************************************************************************
* Function Name:        TIMER0_IRQHandler
* Description:          ��ʱ��0�жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void TIMER0_IRQHandler (void)
{  
	
	OSIntEnter();
	T0IR = 1;
	
	for(timer_i = 0;timer_i < timerSize;timer_i++){
		if(timer_ptr[timer_i]) timer_ptr[timer_i]--;
	}

	if(EVBCONVERSATIONWAITACKTIMEOUT)
		EVBCONVERSATIONWAITACKTIMEOUT--;

	if(RS232WAITRECVPACKTIME)
		RS232WAITRECVPACKTIME--;
	if(UART3RECVACKMSGTIMEOUT)
		UART3RECVACKMSGTIMEOUT--;

	if(TestPluse)
		TestPluse--;
	if(WaitCmdTimer)
		WaitCmdTimer--;		
	if(paomaLedTimer)
		paomaLedTimer--;
	
	
	OSIntExit();
}
/*********************************************************************************************************
* Function Name:        TIMER1_IRQHandler
* Description:          ��ʱ��1�жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void TIMER1_IRQHandler (void)
{
	OSIntEnter();
	T1IR = 1;
	if(MDB_getCoinAcceptor() == COIN_ACCEPTOR_PPLUSE)
		PCOIN_scanParallelPluse();
	else if(MDB_getCoinAcceptor() == COIN_ACCEPTOR_SPLUSE)	
		PCOIN_scanSerialPluse(); 
	OSIntExit();
}
/*********************************************************************************************************
* Function Name:        TIMER2_IRQHandler
* Description:          ��ʱ��2�жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void TIMER2_IRQHandler (void)
{
	OSIntEnter();
	T2IR = 1;	
	OSIntExit();
}
/*********************************************************************************************************
* Function Name:        TIMER3_IRQHandler
* Description:          ��ʱ��3�жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void TIMER3_IRQHandler (void)
{
	OSIntEnter();
	T3IR = 1;	
	OSIntExit();
}
/**************************************End Of File*******************************************************/
