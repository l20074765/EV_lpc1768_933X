/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        主程序文件                     
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
  交易主 任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/
#define TASK_Trade_ID                          7                             /*   交易主任务ID                       */
#define TASK_Trade_PRIO                        TASK_Trade_ID                      /* 任务优先级                   */
#define TASK_Trade_STACK_SIZE                  1024                           /* 定义用户堆栈长度             */
OS_STK  TASK_Trade_STACK[TASK_Trade_STACK_SIZE];                                  /* 定义任务B堆栈                */
extern void    TASK_Trade(void *pdata);                                      /* 声明任务B                    */




/*********************************************************************************************************
  设备 任务ID、优先级、堆栈设置及函数声明                                
*********************************************************************************************************/   
#define TASK_Device_ID                          5                             /* 任务ID                       */
#define TASK_Device_PRIO                        TASK_Device_ID                      /* 任务优先级                   */
#define TASK_Device_STACK_SIZE                  512                           /* 定义用户堆栈长度             */
OS_STK  TASK_Device_STACK[TASK_Device_STACK_SIZE];                                  /* 定义任务C 堆栈               */










/*********************************************************************************************************
** Function name:       TASK_Trade
** Descriptions:       交易主任务
** input parameters:   
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void TASK_Trade(void *pvData)
{
	pvData = pvData;
	SystemInit();//系统基本接口初始化
	SystemParaInit();//系统参数初始化
	CreateMBox();//建立邮箱、信号量	
	
   //创建 设备 包括硬币器 纸币器 hopper找零器 货道任务
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
** Descriptions:          用户程序入口函数
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
int main(void)
{
    targetInit();																//初始化目标板
    pinInit();      															//IO初始化                                                                     
    OSInit();                                                                                                       
	//创建 交易主任务	
	OSTaskCreateExt(TASK_Trade, 
					(void *)0,
					&TASK_Trade_STACK[TASK_Trade_STACK_SIZE-1],
					TASK_Trade_PRIO, 
					TASK_Trade_ID, 
					&TASK_Trade_STACK[0], 
					TASK_Trade_STACK_SIZE, 
					(void *)0,
					OS_TASK_OPT_STK_CHK);

	OSStart();																	//启动多任务
}
/**************************************End Of File*******************************************************/
