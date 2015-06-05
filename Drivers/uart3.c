/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           Uart3.c
** Last modified Date:  2013-01-04
** Last Version:         
** Descriptions:        Uart3��������                   
**------------------------------------------------------------------------------------------------------
** Created by:          sunway 
** Created date:        2013-01-04
** Version:             V0.1 
** Descriptions:        The original version       
********************************************************************************************************/
#include "..\config.h"

#define	UART3_BPS			9600
#define	UART3BUFFERLEN		128

volatile unsigned char Uart3RevBuff[UART3BUFFERLEN];
volatile unsigned char Uart3RxdHead;
volatile unsigned char Uart3RxdTail;

/*********************************************************************************************************
** Function name:     	InitUart3
** Descriptions:	    ���ڳ�ʼ��������Ϊ8λ����λ��1λֹͣλ������żУ�飬������Ϊ9600
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitUart3 (void)
{
    unsigned short int ulFdiv;
	PCONP = PCONP | (1<<25);   
    U3LCR  = 0x83;                                                  //�������ò�����
    ulFdiv = (FPCLK / 16) / UART3_BPS;                              //���ò�����
    U3DLM  = ulFdiv / 256;
    U3DLL  = ulFdiv % 256; 
    U3LCR  = 0x03;                                                  //����������
    U3FCR  = 0x87;                                                  //ʹ��FIFO������8���ֽڴ�����
    U3IER  = 0x01;                                                  //ʹ�ܽ����ж�
	Uart3RxdHead  = 0;
	Uart3RxdTail  = 0;
	memset((void *)Uart3RevBuff,0x00,UART3BUFFERLEN);				//��ʼ��������
	zyIsrSet(NVIC_UART3,(unsigned long)Uart3IsrHandler,PRIO_FIVE);	//ʹ�ܽ����ж�
}
/*********************************************************************************************************
* Function Name:        Uart3IsrHandler
* Description:          Uart3 �жϴ�����
* Input:                None
* Output:               None
* Return:               None
*********************************************************************************************************/
void Uart3IsrHandler (void)
{
	unsigned char Num;
	unsigned char rxd_head;
	unsigned char rxd_data;
	OSIntEnter();      
    while((U3IIR & 0x01) == 0)                          			//�ж��Ƿ����жϹ���
	{
        switch(U3IIR & 0x0E) 										//�ж��жϱ�־
		{                                         
            case 0x04 : for (Num = 0; Num < 8; Num++)				//���������ж�
						{
		                	rxd_data = U3RBR;
							rxd_head = (Uart3RxdHead + 1);
					        if( rxd_head >= UART3BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart3RxdTail) 
					        {
					           	Uart3RevBuff[Uart3RxdHead] = rxd_data;
					           	Uart3RxdHead = rxd_head;
					        }
		                }
		                break;
            case 0x0C : while((U3LSR & 0x01) == 0x01)				//�ַ���ʱ�жϣ��ж������Ƿ�������
						{                         
		                	rxd_data = U3RBR;
							rxd_head = (Uart3RxdHead + 1);
					        if( rxd_head >= UART3BUFFERLEN ) 
					           	rxd_head = 0;
					        if( rxd_head != Uart3RxdTail) 
					        {
					           	Uart3RevBuff[Uart3RxdHead] = rxd_data;
					           	Uart3RxdHead = rxd_head;
					        }
		                }
		                break;
            default	: break;
        }
    }
    OSIntExit();
}
/*********************************************************************************************************
** Function name:	    Uart3PutChar
** Descriptions:	    �򴮿ڷ����ӽ����ݣ����ȴ����ݷ�����ɣ�ʹ�ò�ѯ��ʽ
** input parameters:    ch:Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart3PutChar(unsigned char ch)
{
    U3THR = ch;                                                      //д������
    while((U3LSR & 0x20) == 0);                                      //�ȴ����ݷ������
}

/*********************************************************************************************************
** Function name:	    Uart3PutStr
** Descriptions:	    �򴮿ڷ����ַ���
** input parameters:    Str:  Ҫ���͵��ַ���ָ��
**                      Len:   Ҫ���͵����ݸ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void Uart3PutStr(unsigned char const *Str, unsigned int Len)
{
    unsigned int i;  
    for (i=0;i<Len;i++) 
	{
        Uart3PutChar(*Str++);
    }
}
/*********************************************************************************************************
** Function name:	    Uart3BuffIsNotEmpty
** Descriptions:	    �жϻ������Ƿ�ǿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      1���ǿգ�0����
*********************************************************************************************************/
unsigned char Uart3BuffIsNotEmpty(void)
{
	if(Uart3RxdHead==Uart3RxdTail) 
		return 0;
	else
		return 1;
}


/*********************************************************************************************************
** Function name:     	uart3_clr_buf
** Descriptions:	    �������3���ջ�����
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uart3_clr_buf(void) 
{
	Uart3RxdHead = 0;
	Uart3RxdTail = 0;
}




/*********************************************************************************************************
** Function name:	    Uart3GetCh
** Descriptions:	    �Ӵ��ڻ�������ȡһ���ֽڣ���֮ǰ��Ҫ�жϻ������ǿ�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��ȡ��������
*********************************************************************************************************/
unsigned char Uart3GetCh(void)
{
    unsigned char ch;
    ch = Uart3RevBuff[Uart3RxdTail];
    Uart3RevBuff[Uart3RxdTail] = 0;
    Uart3RxdTail++;
    if(Uart3RxdTail >= UART3BUFFERLEN)
		Uart3RxdTail = 0;
	return ch;
}
/**************************************End Of File*******************************************************/
