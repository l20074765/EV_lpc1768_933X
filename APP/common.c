/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.c
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ϵͳ�������弰ͨ�ú�������                     
**------------------------------------------------------------------------------------------------------
** Created by:          sunway
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#include "..\config.h"



/***********************************************************************************************
*  ����ϵͳ����
*  add  by yoc 2014.2.18
************************************************************************************************************/

/***********************************************************************************************
*  ��ʱ��
*  add  by yoc 2014.2.18
************************************************************************************************************/
unsigned short paomaLedTimer;
ST_TOTAL_TRADE_SUM stTotalTrade;

//================================ϵͳ������==================================================
unsigned int hopperErrNo;//hopper���ϱ�־λ
unsigned short HardWareErr = SYS_ERR_NO_NORMAL;//�ۻ���ϵͳ����״̬

ST_DEV_STATE stDevState;




//�������ݽṹ��		
ST_COLUMN_LEVEL 	stHuodao[2];









/*********************************************************************************************************
** Function name:     	ledControl
** Descriptions:	    ��ˮ�Ŀ��ƺ���
** input parameters:    no �ƺ� num��������
** output parameters:   ��
** Returned value:      ��
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




//������ˮ��
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
** Descriptions:	      ����ѡ��������
** input parameters:    price ��ǰ���
** output parameters:   ��
** Returned value:      ��
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

	FIO1CLR |= (1ul<<18);//��1
	FIO1CLR |= (1ul<<19);//��2
	FIO1CLR |= (1ul<<20);//��3
	FIO1CLR |= (1ul<<31);//��4

	//����1
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
	//����2
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
