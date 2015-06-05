/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           Uart1.c
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        Uart1��������                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-04
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

#define	UART1_BPS			9600
#define	UART1BUFFERLEN		128

volatile unsigned char Uart1RevBuff[UART1BUFFERLEN];
volatile unsigned char Uart1RxdHead;
volatile unsigned char Uart1RxdTail;

/*********************************************************************************************************
** Function name:     	InitUart1
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitUart1 (void)
{
    unsigned short int ulFdiv;
	PCONP = PCONP | (1<<4);   
    U1LCR  = 0x83;                                                  //�������ò�����
    ulFdiv = (FPCLK / 16) / UART1_BPS;                              //���ò�����
    U1DLM  = ulFdiv / 256;
    U1DLL  = ulFdiv % 256; 
    U1LCR  = 0x03;                                                  //����������
    U1FCR  = 0x87;                                                  //ʹ��FIFO������8���ֽڴ�����
    U1IER  = 0x01;                                                  //ʹ�ܽ����ж�
	Uart1RxdHead  = 0;
	Uart1RxdTail  = 0;
	memset((void *)Uart1RevBuff,0x00,UART1BUFFERLEN);				//��ʼ��������
	zyIsrSet(NVIC_UART1,(unsigned long)Uart1IsrHandler,PRIO_ONE);	//ʹ�ܽ����ж�
}
/*********************************************************************************************************
* Function Name:        Uart1IsrHandler
* Description:          Uart1 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void Uart1IsrHandler (void)
{
	unsigned char Num;
	unsigned char rxd_head;
	unsigned char rxd_data;
	OSIntEnter();      
    while((U1IIR & 0x01) == 0)                          			//�ж��Ƿ����жϹ���
	{
        switch(U1IIR & 0x0E) 										//�ж��жϱ�־
		{                                         
            case 0x04 : for (Num = 0; Num < 8; Num++)				//���������ж�
						{
		                	rxd_data = U1RBR;
							rxd_head = (Uart1RxdHead + 1);
					        if( rxd_head >= UART1BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart1RxdTail) 
					        {
					           	Uart1RevBuff[Uart1RxdHead] = rxd_data;
					           	Uart1RxdHead = rxd_head;
					        }
		                }
		                break;
            case 0x0C : while((U1LSR & 0x01) == 0x01)				//�ַ���ʱ�жϣ��ж������Ƿ�������
						{                         
		                	rxd_data = U1RBR;
							rxd_head = (Uart1RxdHead + 1);
					        if( rxd_head >= UART1BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart1RxdTail) 
					        {
					           	Uart1RevBuff[Uart1RxdHead] = rxd_data;
					           	Uart1RxdHead = rxd_head;
					        }
		                }
		                break;
            default	: break;
        }
    }
    OSIntExit();
}
/*********************************************************************************************************
** Function name:	    Uart1PutChar
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ch:Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart1PutChar(unsigned char ch)
{
    U1THR = ch;                                                      //д������
    while((U1LSR & 0x20) == 0);                                      //�ȴ����ݷ������
}

/*********************************************************************************************************
** Function name:	    Uart1PutStr
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    Str:  Ҫ���͵��ַ���ָ��
**                      Len:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart1PutStr(unsigned char const *Str, unsigned int Len)
{
    unsigned int i;  
    for (i=0;i<Len;i++) 
	{
        Uart1PutChar(*Str++);
    }
}
/*********************************************************************************************************
** Function name:	    Uart1BuffIsNotEmpty
** Descriptions:	    �жϻ������Ƿ�ǿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      1���ǿգ�0����
*********************************************************************************************************/
unsigned char Uart1BuffIsNotEmpty(void)
{
	if(Uart1RxdHead==Uart1RxdTail) 
		return 0;
	else
		return 1;
}
/*********************************************************************************************************
** Function name:	    Uart1GetCh
** Descriptions:	    �Ӵ��ڻ�������ȡһ���ֽڣ���֮ǰ��Ҫ�жϻ������ǿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ȡ��������
*********************************************************************************************************/
unsigned char Uart1GetCh(void)
{
    unsigned char ch;
    ch = Uart1RevBuff[Uart1RxdTail];
    Uart1RevBuff[Uart1RxdTail] = 0;
    Uart1RxdTail++;
    if(Uart1RxdTail >= UART1BUFFERLEN)
		Uart1RxdTail = 0;
	return ch;
}
/**************************************End Of File*******************************************************/
