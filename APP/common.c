/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        系统参数定义及通用函数部分                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"



/***********************************************************************************************
*  工程系统参数
*  add  by yoc 2014.2.18
************************************************************************************************************/

/***********************************************************************************************
*  定时器
*  add  by yoc 2014.2.18
************************************************************************************************************/
unsigned short paomaLedTimer;
ST_TOTAL_TRADE_SUM stTotalTrade;

//================================系统故障码==================================================
unsigned int hopperErrNo;//hopper故障标志位
unsigned short HardWareErr = SYS_ERR_NO_NORMAL;//售货机系统故障状态

ST_DEV_STATE stDevState;


//交易设备的通道面值数据结构
struct STDevValue stDevValue;

//货道数据结构体		
ST_COLUMN_LEVEL 	stHuodao[2];
struct STSYSPARAM 	SYSPara;//系统参数结构体






/********************************************************************************************************
** Function name:     	PrintfMoney
** Descriptions:	    依据小数位数，显示金额的函数
** input parameters:    dispnum:投币金额，以分为单位 
** output parameters:   无
** Returned value:      显示字符串
*********************************************************************************************************/
char *PrintfMoney(uint32_t dispnum)
{  
	char strnum[10];
	
	if(SYSPara.nPoinValue==0)
		dispnum = dispnum*100;
	else if(SYSPara.nPoinValue==1)
		dispnum = dispnum*10;
	if(SYSPara.nPoinValue==2)
		dispnum = dispnum;
   switch(SYSPara.nPoinValue) 
   {
      case 2://以分为单位
	  	  sprintf(strnum,"%d.%02d",dispnum/100,dispnum%100);	
		  
		  break;

	  case 1://以角为单位
	  	  dispnum /= 10;
		  sprintf(strnum,"%d.%d",dispnum/10,dispnum%10);
		  break;
	  
	  case 0://以元为单位
		  sprintf(strnum,"%d",dispnum/100);
		  break;
   }
   //Trace("\r\n pp=%s",strnum);
   return &strnum[0];
}




/*********************************************************************************************************
** Function name:     	ledControl
** Descriptions:	    流水的控制函数
** input parameters:    no 灯号 num开灯数量
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void ledControl(unsigned char no,unsigned char num)
{
	switch(no)
	{
		case 1:			
			FIO1SET |= (1ul<<19);		
			FIO1CLR |= (1ul<<18); 
			FIO1CLR |= (1ul<<31);
			FIO1CLR |= (1ul<<20);				
			break;
		case 2:
			FIO1CLR |= (1ul<<19);		
			FIO1SET |= (1ul<<18); 
			FIO1CLR |= (1ul<<31);
			FIO1CLR |= (1ul<<20);
			break;
		case 3:
			FIO1CLR |= (1ul<<19);		
			FIO1CLR |= (1ul<<18); 
			FIO1SET |= (1ul<<31);
			FIO1CLR |= (1ul<<20);
			break;
		case 4:
			FIO1CLR |= (1ul<<19);
			FIO1CLR |= (1ul<<18); 
			FIO1CLR |= (1ul<<31);
			FIO1SET |= (1ul<<20);
			break;
		default:
			FIO1CLR |= (1ul<<19);
			FIO1CLR |= (1ul<<18); 
			FIO1CLR |= (1ul<<31);
			FIO1CLR |= (1ul<<20);
			break;
	}
	
	switch(num)
    {
        case 4:
        FIO1SET |= (1ul<<19);
		
        case 3:
        FIO1SET |= (1ul<<18);
		
        case 2:
        FIO1SET |= (1ul<<31);
		
        case 1:
        FIO1SET |= (1ul<<20);
        	break;
		default:
			break;
    }
}




//空闲流水灯
void idlePaoMaLed(void)
{

	static unsigned char maxLedNum  = 4;
	static unsigned char curLedONNum = 0,curLedOnIndex = 0;
	if(!paomaLedTimer)
	{		
		if(curLedOnIndex >= (maxLedNum - curLedONNum))
        {
            if(curLedONNum < 4)
                curLedONNum++;
            else
                curLedONNum = 0;
            curLedOnIndex = 0;

        }
        else
            curLedOnIndex++;
        ledControl(curLedOnIndex,curLedONNum);
		
		paomaLedTimer = 60;// led liushui speed
	}
	
	
	
}


/*********************************************************************************************************
** Function name:     	setColumnLedON
** Descriptions:	      开启选货按键灯
** input parameters:    price 当前金额
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/

void setColumnLedON(unsigned int price)
{
	unsigned char i,j,index = 0;
	static unsigned int huodaoLed[16] = {HUODAO_SELCET_LED1,HUODAO_SELCET_LED2,
										 HUODAO_SELCET_LED3,HUODAO_SELCET_LED4};
		
	for(i = 0;i < COLUMN_LEVEL_NUM;i++)
	{
		if(!stHuodao[i].isOpen)
			continue;
		for(j = 0;j < COLUMN_ONE_LEVEL_NUM;j++)
		{
			if(!stHuodao[i].stColumn[j].isOpen)
				continue;
			if( price >= stHuodao[i].stColumn[j].price 	&& 
				stHuodao[i].stColumn[j].remainCount != 0	&& 
				stHuodao[i].stColumn[j].state == HUODAO_STATE_NORMAL )
			{
				FIO1SET |= huodaoLed[index];
				stHuodao[i].stColumn[j].canShip = 1;
			}		
			else
			{
				FIO1CLR |= huodaoLed[index];
				stHuodao[i].stColumn[j].canShip = 0;
			}	
			index++;
		}
	}

#if 0

	FIO1CLR |= (1ul<<18);//灯1
	FIO1CLR |= (1ul<<19);//灯2
	FIO1CLR |= (1ul<<20);//灯3
	FIO1CLR |= (1ul<<31);//灯4

	//货道1
	if(getColumnIndex(&i,&j, 3, 1))
	{
		if(price >= stHuodao[i].stColumn[j].price&& 
			stHuodao[i].stColumn[j].remainCount && 
			(stHuodao[i].stColumn[j].state== HUODAO_STATE_NORMAL)
			)
		{
			FIO1SET |= (1ul<<18);
		}
	}
	//货道2
	if(getColumnIndex(&i,&j, 3, 2))
	{
		if(price >= stHuodao[i].stColumn[j].price&& 
			stHuodao[i].stColumn[j].remainCount && 
			(stHuodao[i].stColumn[j].state== HUODAO_STATE_NORMAL)
			)
		{
			FIO1SET |= (1ul<<19);
		}
	}

	if(getColumnIndex(&i,&j, 3, 3))
	{
		if(price >= stHuodao[i].stColumn[j].price&& 
			stHuodao[i].stColumn[j].remainCount && 
			(stHuodao[i].stColumn[j].state== HUODAO_STATE_NORMAL)
			)
		{
			FIO1SET |= (1ul<<20);
		}
	}


	if(getColumnIndex(&i,&j, 3, 3))
	{
		if(price >= stHuodao[i].stColumn[j].price&& 
			stHuodao[i].stColumn[j].remainCount && 
			(stHuodao[i].stColumn[j].state== HUODAO_STATE_NORMAL)
			)
		{
			FIO1SET |= (1ul<<31);
		}
	}
#endif
		
}



/**************************************End Of File*******************************************************/
