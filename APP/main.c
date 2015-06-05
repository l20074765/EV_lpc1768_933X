/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        �������ļ�                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"
#include "SailControl.h"
#include "Global.h"
#include "Menu.h"
#include "..\API\task_trade.h"



/*********************************************************************************************************
  ������ ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/
#define TASK_Trade_ID                          7                             /*   ����������ID                       */
#define TASK_Trade_PRIO                        TASK_Trade_ID                      /* �������ȼ�                   */
#define TASK_Trade_STACK_SIZE                  1024                           /* �����û���ջ����             */
OS_STK  TASK_Trade_STACK[TASK_Trade_STACK_SIZE];                                  /* ��������B��ջ                */
extern void    TASK_Trade(void *pdata);                                      /* ��������B                    */




/*********************************************************************************************************
  �豸 ����ID�����ȼ�����ջ���ü���������                                
*********************************************************************************************************/   
#define TASK_Device_ID                          5                             /* ����ID                       */
#define TASK_Device_PRIO                        TASK_Device_ID                      /* �������ȼ�                   */
#define TASK_Device_STACK_SIZE                  512                           /* �����û���ջ����             */
OS_STK  TASK_Device_STACK[TASK_Device_STACK_SIZE];                                  /* ��������C ��ջ               */










/*********************************************************************************************************
** Function name:       TASK_Trade
** Descriptions:       ����������
** input parameters:   
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/

void TASK_Trade(void *pvData)
{
	pvData = pvData;
	SystemInit();//ϵͳ�����ӿڳ�ʼ��
	SystemParaInit();//ϵͳ������ʼ��
	CreateMBox();//�������䡢�ź���	
	
   //���� �豸 ����Ӳ���� ֽ���� hopper������ ��������
	OSTaskCreateExt(task_dev, 
				(void *)0,
				&TASK_Device_STACK[TASK_Device_STACK_SIZE-1],
				TASK_Device_PRIO, 
				TASK_Device_ID, 
				&TASK_Device_STACK[0], 
				TASK_Device_STACK_SIZE, 
				(void *)0,
				OS_TASK_OPT_STK_CHK);
				
	task_trade();
	
}





/*********************************************************************************************************
** Function name:       main
** Descriptions:          �û�������ں���
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
int main(void)
{
    targetInit();																//��ʼ��Ŀ���
    pinInit();      															//IO��ʼ��                                                                     
    OSInit();                                                                                                       
	//���� ����������	
	OSTaskCreateExt(TASK_Trade, 
					(void *)0,
					&TASK_Trade_STACK[TASK_Trade_STACK_SIZE-1],
					TASK_Trade_PRIO, 
					TASK_Trade_ID, 
					&TASK_Trade_STACK[0], 
					TASK_Trade_STACK_SIZE, 
					(void *)0,
					OS_TASK_OPT_STK_CHK);

	OSStart();																	//����������
}
/**************************************End Of File*******************************************************/
