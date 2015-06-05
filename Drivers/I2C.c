/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:                  I2C0.c
** Latest modified Date:       2013-01-04
** Latest Version:
** Descriptions:
**--------------------------------------------------------------------------------------------------------
** Modified by:                sunway
** Modified date:              2013-01-04
** Version:					   V0.1
** Descriptions:               I2C0��������
*********************************************************************************************************/
#include "..\config.h"
/*********************************************************************************************************/
#define ONE_BYTE_SUBA   1
#define TWO_BYTE_SUBA   2
#define X_ADD_8_SUBA    3
/*********************************************************************************************************/
volatile unsigned char I2C0_sla;             //I2C�����ӵ�ַ
volatile unsigned int  I2C0_suba;            //I2C�����ڲ��ӵ�ַ
volatile unsigned char I2C0_suba_num;        //I2C�ӵ�ַ�ֽ���
volatile unsigned char *I2C0_buf;            //���ݻ�����ָ��
volatile unsigned int  I2C0_num;             //Ҫ��ȡ/д������ݸ���
volatile unsigned char I2C0_end;             //I2C���߽�����־��������������1
volatile unsigned char I2C0_suba_en;     	 //�ӵ�ַ����:0--�ӵ�ַ�Ѿ�������߲���Ҫ�ӵ�ַ 1--��ȡ���� 2--д����
/*********************************************************************************************************/
unsigned char Wait_I2C0_End(unsigned int dly);
unsigned char I2C0RecvByte(unsigned char sla,unsigned char *dat);
unsigned char I2C0SendByte(unsigned char sla,unsigned char  dat);
/*********************************************************************************************************
** Function name:       InitI2C0
** Descriptions:        ��ģʽI2C��ʼ��������IRQ�ж�
** input parameters:    Freq:��ʼ��I2C�������ʣ����ֵΪ400K
** Output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void InitI2C0(void)
{
    I2C0SCLH = (FPCLK/400000+1)/2; 			//����I2Cʱ��
    I2C0SCLL = (FPCLK/400000)/2;
    I2C0CONCLR = 0x2C;
    I2C0CONSET = 0x40;                  	//ʹ����I2C 
	zyIsrSet(NVIC_I2C0,(unsigned long)I2C0_IRQHandler,PRIO_THREE);   
}
/*********************************************************************************************************
** Function name:           Wait_I2C0_End
** Descriptions:            �����ʱ���ṩ��I2C���ߵȴ���ʱʹ��
** input parameters:        dly:��ʱ������ֵԽ����ʱԽ��
** Output parameters:       NONE
** Returned value:          NONE
*********************************************************************************************************/
unsigned char Wait_I2C0_End(unsigned int dly)
{  
	unsigned int i;
    if(I2C0_end == 1)
		return(1);
    for(;dly>0;dly--)
	{
	    for(i=0;i<5000;i++)
	    {
	        if(I2C0_end == 1)
				return(1);
	    }
	}
    return(0);
}
/*********************************************************************************************************
** Function name:       I2C0SendByte
** Descriptions:        �����ӵ�ַ��������1�ֽ����ݡ�
** input parameters:    sla:������ַ
**                      dat:Ҫ���͵�����
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
unsigned char I2C0SendByte(unsigned char sla, unsigned char dat)
{   
    I2C0_sla     = sla;           //д������������ַ
    I2C0_buf     = &dat;          //�����͵����� 
    I2C0_num     = 1;             //����1�ֽ����� 
    I2C0_suba_en = 0;             //���ӵ�ַ  
    I2C0_end     = 0;
    I2C0CONCLR   = 0x2C;
    I2C0CONSET   = 0x60;          //����Ϊ����������������
    return(Wait_I2C0_End(20));
}
/*********************************************************************************************************
** Function name:       I2C0RcvByte
** Descriptions:        �����ӵ�ַ������ȡ1�ֽ����ݡ�
** input parameters:    sla:������ַ
**                      dat:Ҫ���͵�����
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
unsigned char I2C0RecvByte(unsigned char sla, unsigned char *dat)
{                          
    I2C0_sla     = sla+1;         //��������������ַ 
    I2C0_buf     = dat;
    I2C0_num     = 1;
    I2C0_suba_en = 0;             //���ӵ�ַ
    I2C0_end     = 0;

    I2C0CONCLR   = 0x2C;	
    I2C0CONSET   = 0x60;          //����Ϊ����������������

    return(Wait_I2C0_End(20));
}
/*********************************************************************************************************
** Function name:       I2C0_ReadNByte
** Descriptions:        �����ӵ�ַ���������ַ��ʼ��ȡN�ֽ�����
** input parameters:    sla         :������ַ
**                      suba_type   :�ӵ�ַ�ṹ    1�����ֽڵ�ַ    3��8+X�ṹ    2��˫�ֽڵ�ַ
**                      suba        :�����ӵ�ַ
**                      s           :���ݽ��ջ�����ָ��
**                      num         :��ȡ�ĸ���
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
unsigned char I2C0RdNByte(unsigned char sla,unsigned int suba_type,unsigned int suba,unsigned char *s,unsigned int num)
{
    if(num > 0)                                         //�ж�num�����ĺϷ���   
    {                                                   //��������              
        if(suba_type == 1)
        {                                               //�ӵ�ַΪ���ֽ�        
            I2C0_sla         = sla + 1;                 //�������Ĵӵ�ַ��R=1   
            I2C0_suba        = suba;                    //�����ӵ�ַ            
            I2C0_suba_num    = 1;                       //�����ӵ�ַΪ1�ֽ�     
        }
        if(suba_type == 2)
        {                                               //�ӵ�ַΪ2�ֽ�         
            I2C0_sla         = sla + 1;                 //�������Ĵӵ�ַ��R=1   
            I2C0_suba        = suba;                    //�����ӵ�ַ            
            I2C0_suba_num    = 2;                       //�����ӵ�ַΪ2�ֽ�     
        }
        if(suba_type == 3)
        {                                               //�ӵ�ַ�ṹΪ8+X       
            I2C0_sla   = sla + ((suba >> 7 )& 0x0e) + 1;//�������Ĵӵ�ַ��R=1   
            I2C0_suba        = suba & 0x0ff;            //�����ӵ�ַ            
            I2C0_suba_num    = 1;                       //�����ӵ�ַΪ8+x       
        }
        I2C0_buf     = s;                               //���ݽ��ջ�����ָ��    
        I2C0_num     = num;                             //Ҫ��ȡ�ĸ���          
        I2C0_suba_en = 1;                               //���ӵ�ַ��            
        I2C0_end     = 0;
        I2C0CONCLR = (1 << 2)|(1 << 3)|(1 << 5);        //���STA,SI,AA��־λ                
        I2C0CONSET = (1 << 5)|(1 << 6);                 //��λSTA,����I2C����       
        return(Wait_I2C0_End(20));
    }
    return(FALSE);
}
/*********************************************************************************************************
** Function name:       I2C0_WriteNByte
** Descriptions:        �����ӵ�ַ����д��N�ֽ�����
** input parameters:    sla         :������ַ
**                      suba_type   :�ӵ�ַ�ṹ    1�����ֽڵ�ַ    3��8+X�ṹ    2��˫�ֽڵ�ַ
**                      suba        :�����ӵ�ַ
**                      s           :��Ҫд������ݵ�ָ��
**                      num         :��Ҫд������ݵĸ���
** Output parameters:   ����ֵΪ0ʱ��ʾ����Ϊ1ʱ��ʾ������ȷ��
** Returned value:      NONE
*********************************************************************************************************/
unsigned char I2C0WrNByte(unsigned char sla,unsigned char suba_type,unsigned int suba,unsigned char *s,unsigned int num)
{
    if(num > 0)                                         //�����ȡ�ĸ���Ϊ0���򷵻ش� 
    {                                                   //���ò���                    
        if(suba_type == 1)
        {                                               //�ӵ�ַΪ���ֽ�              
            I2C0_sla         = sla;                     //�������Ĵӵ�ַ              
            I2C0_suba        = suba;                    //�����ӵ�ַ                  
            I2C0_suba_num    = 1;                       //�����ӵ�ַΪ1�ֽ�           
        }
        if(suba_type == 2)
        {                                               //�ӵ�ַΪ2�ֽ�               
            I2C0_sla         = sla;                     //�������Ĵӵ�ַ              
            I2C0_suba        = suba;                    //�����ӵ�ַ                  
            I2C0_suba_num    = 2;                       //�����ӵ�ַΪ2�ֽ�           
        }
        if(suba_type == 3)
        {                                               //�ӵ�ַ�ṹΪ8+X             
            I2C0_sla       = sla + ((suba >> 7 )& 0x0e);//�������Ĵӵ�ַ              
            I2C0_suba        = suba & 0x0ff;            //�����ӵ�ַ                  
            I2C0_suba_num    = 1;                       //�����ӵ�ַΪ8+X             
        }
        I2C0_buf     = s;                               //����                        
        I2C0_num     = num;                             //���ݸ���                    
        I2C0_suba_en = 2;                               //���ӵ�ַ��д����            
        I2C0_end     = 0;
        I2C0CONCLR = (1 << 2)|(1 << 3)|(1 << 5);        //���STA,SI,AA��־λ                                       
        I2C0CONSET = (1 << 5)|(1 << 6);                 //��λSTA,����I2C����                            
        return(Wait_I2C0_End(20));						//�ȴ�I2C�������
    }
    return(FALSE);
}
/*********************************************************************************************************
** Function name:       I2C0_IRQHandler
** Descriptions:        Ӳ��I2C�жϷ������
** input parameters:    ��
** Output parameters:   ��
** Returned value:      ע�⴦���ӵ�ַΪ2�ֽڵ������
*********************************************************************************************************/
void I2C0_IRQHandler(void)
{   
	//��ȡI2C״̬�Ĵ���I2DAT,����ȫ�ֱ��������ý��в��������������־,����ж��߼�,�жϷ���
	//����״̬�������Ӧ�Ĵ���
	OSIntEnter();
    switch(I2C0STAT & 0xF8)
    {   
        case 0x08:	//�ѷ�����ʼ����,�����ͺ������ն���,װ��SLA+W����SLA+R 
					if(I2C0_suba_en == 1)				//SLA+R ָ���ӵ�ַ��
						I2C0DAT = I2C0_sla & 0xFE;      //��д���ַ
					else                                //SLA+W
						I2C0DAT = I2C0_sla;             //����ֱ�ӷ��ʹӻ���ַ
					I2C0CONCLR = (1<<3)|(1<<5);  		//SI,STA
					break;
        case 0x10:	//�ѷ����ظ���ʼ����,�����ͺ������ն���
        			I2C0DAT = I2C0_sla;					//װ��SLA+W����SLA+R,�������ߺ��ط��ӵ�ַ                                              
        			I2C0CONCLR = 0x28;    				// ����SI,STA
        			break;
        case 0x18:
        case 0x28:  if(I2C0_suba_en == 0)				//�ѷ���I2DAT�е�����,�ѽ���ACK
					{
						if(I2C0_num > 0)
						{    
							I2C0DAT = *I2C0_buf++;
							I2C0CONCLR = 0x28;          //����SI,STA
							I2C0_num--;
						}
						else                            //û�����ݷ�����
						{                               //ֹͣ����
							I2C0CONSET = (1<<4);	    // STO
							I2C0CONCLR = 0x28;          //����SI,STA
							I2C0_end = 1;               //�����Ѿ�ֹͣ
						}
					}
					if(I2C0_suba_en == 1)               //����ָ����ַ��,��������������
					{
					    if(I2C0_suba_num == 2)
					    {   
							I2C0DAT = ((I2C0_suba >> 8) & 0xff);
					        I2C0CONCLR = 0x28;          //����SI,STA
					        I2C0_suba_num--;
					        break;
					    }
					    if(I2C0_suba_num == 1)
					    {   
							I2C0DAT = (I2C0_suba & 0xff);
					        I2C0CONCLR = 0x28;          //����SI,STA
					        I2C0_suba_num--;
					        break;
					    }
					    if(I2C0_suba_num == 0)
					    {
					        I2C0CONCLR = 0x08;
					        I2C0CONSET = 0x20;
								
					        I2C0_suba_en = 0;           //�ӵ�ַ������
					        break;
					    }
					}
					if(I2C0_suba_en == 2)               //ָ���ӵ�ַд,�ӵ�ַ��δָ��, �����ӵ�ַ 
					{
					    if(I2C0_suba_num > 0)
					    {    
							if (I2C0_suba_num == 2)
					        {    
								I2C0DAT = ((I2C0_suba >> 8) & 0xff);
					            I2C0CONCLR = 0x28;
					            I2C0_suba_num--;
					            break;
					        }
					        if (I2C0_suba_num == 1)
					        {   
								I2C0DAT    = (I2C0_suba & 0xff);
					            I2C0CONCLR = 0x28;
					            I2C0_suba_num--;
					            I2C0_suba_en  = 0;
					            break;
					        }
					    }
					}
					break;
        case 0x40:  if (I2C0_num <= 1)                  //�ѷ���SLA+R,�ѽ���ACK����������һ���ֽ�           
						I2C0CONCLR = 1 << 2;            //�´η��ͷ�Ӧ���ź�           
			        else								
						I2C0CONSET = 1 << 2;            //�´η���Ӧ���ź�             
			        I2C0CONCLR = 0x28;                  //����SI,STA                   
			        break;

        case 0x20:                                      //�ѷ���SLA+W,�ѽ��շ�Ӧ��     
        case 0x30:                                      //�ѷ���I2DAT�е����ݣ��ѽ��գ���Ӧ��  
        case 0x38:                                      //��SLA+R/W�������ֽ��ж�ʧ��  
        case 0x48:  I2C0CONCLR = 0x28;					//�ѷ���SLA+R,�ѽ��շ�Ӧ��
			        I2C0_end = 0xFF;
			        break;

        case 0x50:  *I2C0_buf++ = I2C0DAT;				//�ѽ��������ֽڣ��ѷ���ACK
			        I2C0_num--;
			        if(I2C0_num == 1)                   //�������һ���ֽ�             
						I2C0CONCLR = 0x2C;              //STA,SI,AA = 0                
			        else
			        {   
						I2C0CONSET = 0x04;              //AA=1                         
			            I2C0CONCLR = 0x28;
			        }
			        break;
        case 0x58:  *I2C0_buf++ = I2C0DAT;              //�ѽ��������ֽڣ��ѷ��ط�Ӧ��/��ȡ���һ�ֽ�����      
			        I2C0CONSET = 0x10;                  //��������                    
			        I2C0CONCLR = 0x28;
			        I2C0_end = 1;
			        break;
        default:	break;
    }
	OSIntExit();
}
/**************************************End Of File*******************************************************/
