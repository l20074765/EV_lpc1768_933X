/*******************************************************************************************
								ģ�����ƣ�display
								ģ�鹦�ܣ�ʵ�ֶ��������ʾ�ؼ��Ŀ��Ʋ���
								����ʱ�䣺2011-12-12
								������Ա������ΰ
********************************************************************************************/


#include <intrins.h>
#include "display.h"
#include <stdio.h>
#include <string.h>

unsigned char TM1637_ACK ;     
unsigned char DIS_BUF[6];
//            �����ַ�����ʾ             0    1    2    3     4   5    6    7    8    9
unsigned char Digital_Seg_Table[10] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f };
//                 �����ַ�	     F    H    P    B    L    E	   C
unsigned char SpecialByte[7] = {0x71,0x76,0xf3,0x7f,0xb8,0x79,0x39};
//��ʾ��ʼֵ0000
unsigned char StartDisp[6] = {0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};
/*
unsigned char rootmemu[6] =	{ 0x71,0x3f,0xbf,0x3f,0xff,0xff };		//F00.0���˵�
unsigned char submemu1[6] =	{ 0x71,0x3f,0x86,0x3f,0xff,0xff };		//F01.0�Ӳ˵�
unsigned char submemu2[6] =	{ 0x71,0x3f,0xdb,0x3f,0xff,0xff };		//F02.0�Ӳ˵�
unsigned char submemu3[6] =	{ 0x71,0x3f,0xcf,0x3f,0xff,0xff };		//F03.0�Ӳ˵�
unsigned char submemu4[6] =	{ 0x71,0x3f,0xe6,0x3f,0xff,0xff };		//F04.0�Ӳ˵�
unsigned char submemu5[6] =	{ 0x71,0x3f,0xed,0x3f,0xff,0xff };		//F05.0�Ӳ˵�
unsigned char HopperState1[6] = { 0x76,0x00,0x3f,0x3f,0xff,0xff};   //HX.00����������
unsigned char HopperState2[6] = { 0x76,0x00,0x3f,0x86,0xff,0xff};	//HX.01������ȱ��
unsigned char HopperState3[6] = { 0x76,0x00,0x71,0x71,0xff,0xff};	//HX.FF����������
unsigned char HopHardReport[6] = { 0x76,0xf3,0x79,0x79,0xff,0xff};	//HP.EE����������
unsigned char BillHardReport[6] = { 0x7f,0xb8,0x79,0x79,0xff,0xff};	//BL.EE�ձ�������
unsigned char AllHardReport[6] = { 0x76,0x7f,0x79,0x79,0xff,0xff};	//HB.EE�ձ���������������
unsigned char ClrRecordOK[6] = { 0x39,0xb8,0x3f,0x3f,0xff,0xff};	//�����¼�ɹ�
unsigned char ClrRecordEr[6] = { 0x39,0xb8,0x79,0x79,0xff,0xff};	//�����¼ʧ��
*/
void tm1637_i2c_write_onebyte( unsigned char ); 			//I2Cд��һ���ֽ�    
void tm1637_i2c_stop( void ); 								//I2C���߽����ź�      
void tm1637_display( void );								//����tm1637��ʾ����
void update_dis_buf( unsigned char * );						//�����Դ�

/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           InitEsdLed
** Descriptions:            ��ʾ��ʼ��������ȫ������ܣ���С����,�����ڼ��������Ƿ����
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void InitEsdLed()
{
	unsigned char i;
	for( i=0;i<6;i++)
	{
		DIS_BUF[i] = 0x3F;
	}
	tm1637_display();
}

/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           DigitalDisplay
** Descriptions:            4λ8���������ʾAPI
** input parameters:        DisData,������ʾ���ݣ�
							DotNumb,С����λ����
								0-��ʾXXXX
								1-��ʾXXX.X
								2-��ʾXX.XX
								3-��ʾX.XXX
								����ֵ��Ч
							DisMode��ʾģʽ:0-�����ʾģʽ��1-�˵���ʾģʽ  3-һЩ������Ϣ��˵���Ϣ����ʾ������DotNumb
							0�����ʾģʽ:
								DisData<=9999;DotNumb<=3								
							1�˵���ʾģʽ:
								DisData=0��F00.0
								DisData=1��F01.0
								.
								.
								.
								DisData=5��F05.0							
** output parameters:       none
** Returned value:          0-��������1-��ʾ����
*********************************************************************************************************/
bit DigitalDisplay( unsigned int DisData,unsigned char DotNumb,unsigned char DisMode )
{
	unsigned char buf_temp[6];
	unsigned char i = 0;
	for(i=0;i<6;i++)
		{
			buf_temp[i] = 0xff;
		}
	i=0;
	if(DisData > 9999)
		return 0;
	else
	{
		if(DisMode == 1)
		{
			StartDisp[0] = SpecialByte[0];
			StartDisp[2] = Digital_Seg_Table[DisData]|0x80;
			if((DotNumb == 1)&&(DisData == 4))
			{
				StartDisp[3] = Digital_Seg_Table[1];
			}
			else
			if((DotNumb == 2)&&(DisData == 4))
			{
				StartDisp[3] = Digital_Seg_Table[2];
			}
			else
			if((DotNumb == 3)&&(DisData == 4))
			{
				StartDisp[3] = Digital_Seg_Table[3];
			}	
			update_dis_buf(&StartDisp[0]);
			tm1637_display();
			return 1;
		}
		else
		if(DisMode == 2)
		{
			StartDisp[0] = SpecialByte[1];
			StartDisp[1] = Digital_Seg_Table[DisData]|0x80;
			if(DotNumb == 1)
			{
				StartDisp[3] = Digital_Seg_Table[2];  //HX.02
			}
			else
			if(DotNumb == 2)
			{
				StartDisp[2] = SpecialByte[0];
				StartDisp[3] = SpecialByte[0];		  //HX.FF
			}	
			update_dis_buf(&StartDisp[0]);
			tm1637_display();
			return 1;	
		}
		else
		if(DisMode == 3)
		{
			StartDisp[2] = SpecialByte[5];
			StartDisp[3] = SpecialByte[5];
			//HP.EE
			if(DotNumb == 1)
			{
				StartDisp[0] = SpecialByte[1];
				StartDisp[1] = SpecialByte[2]|0x80; 
			}
			else
			//BL.EE
			if(DotNumb == 2)
			{
				StartDisp[0] = SpecialByte[3];
				StartDisp[1] = SpecialByte[4]|0x80; 
			}
			else
			//HB.EE
			if(DotNumb == 3)
			{
				StartDisp[0] = SpecialByte[1];
				StartDisp[1] = SpecialByte[3]|0x80; 
			}
			else
			//CL.EE
			if(DotNumb == 4)
			{
				StartDisp[0] = SpecialByte[6];
				StartDisp[1] = SpecialByte[4]|0x80; 
			}	
			else
			//CL.00
			if(DotNumb == 5)
			{
				StartDisp[0] = SpecialByte[6];
				StartDisp[1] = SpecialByte[4]|0x80;
				StartDisp[2] = Digital_Seg_Table[0];
				StartDisp[3] = Digital_Seg_Table[0]; 
			}
			update_dis_buf(&StartDisp[0]);
			tm1637_display();
			return 1;
		}
	}
	/*if( DisData > 9999 )
		return 0;
	else
		{
			if( DisMode == 1 )								//����˵���ʾ
				{
					if( DisData == 0 )
						{
							update_dis_buf( &rootmemu[0] );
							tm1637_display(  );
							return 1;
						}
					else if( DisData == 1 )
						{
							update_dis_buf( &submemu1[0] );
							tm1637_display(  );
							return 1;
						 }
					else if( DisData == 2 )
						{
							update_dis_buf( &submemu2[0] );
							tm1637_display(  );
							return 1;
						 }
					else if( DisData == 3 )
						{
							update_dis_buf( &submemu3[0] );
							tm1637_display(  );
							return 1;
						 }
					else if( DisData == 4 )
						{
							update_dis_buf( &submemu4[0] );
							tm1637_display(  );
							return 1;
						 }
					else if( DisData == 5 )
						{
							update_dis_buf( &submemu5[0] );
							tm1637_display(  );
							return 1;
						 }
					else
						return 0;
				}
			else
			if( DisMode == 2)
			{
				//��ʾHX.00
				if(DotNumb == 1)
				{
					//��ʾH1.00
					if(DisData == 1)
						HopperState1[1] = 0x86;
					else
					//��ʾH1.01
					if(DisData == 2)
						HopperState1[1] = 0xdb;
					else
					//��ʾH1.ff
					if(DisData == 3)
						HopperState1[1] = 0xcf;
					update_dis_buf( &HopperState1[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//��ʾHX.00
				if(DotNumb == 2)
				{
					//��ʾH2.00
					if(DisData == 1)
						HopperState2[1] = 0x86;
					else
					//��ʾH2.01
					if(DisData == 2)
						HopperState2[1] = 0xdb;
					else
					//��ʾH2.ff
					if(DisData == 3)
						HopperState2[1] = 0xcf;
					update_dis_buf( &HopperState2[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//��ʾHX.00
				if(DotNumb == 3)
				{
					//��ʾH3.00
					if(DisData == 1)
						HopperState3[1] = 0x86;
					else
					//��ʾH3.01
					if(DisData == 2)
						HopperState3[1] = 0xdb;
					else
					//��ʾH3.ff
					if(DisData == 3)
						HopperState3[1] = 0xcf;
					update_dis_buf( &HopperState3[0] );
					tm1637_display(  );
					return 1;
				}
				else
					return 0;
			}
			else
			if(DisMode == 3)
			{
				//��ʾF04.1
				if(DotNumb == 1)
				{
					strcpy(ClearHopBuf,submemu4);
					ClearHopBuf[3] = 0x06;
					update_dis_buf( &ClearHopBuf[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//��ʾF04.2
				if(DotNumb == 2)
				{
					strcpy(ClearHopBuf,submemu4);
					ClearHopBuf[3] = 0x5b;
					update_dis_buf( &ClearHopBuf[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//��ʾF04.3
				if(DotNumb == 3)
				{
					strcpy(ClearHopBuf,submemu4);
					ClearHopBuf[3] = 0x4f;
					update_dis_buf( &ClearHopBuf[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//��ʾHP.EE
				if(DotNumb == 4)
				{
					update_dis_buf( &HopHardReport[0] );
					tm1637_display(  );
					return 1;		
				}
				else
				//��ʾBL.EE
				if(DotNumb == 5)
				{
					update_dis_buf( &BillHardReport[0] );
					tm1637_display(  );
					return 1;		
				}
				else
				//��ʾHB.EE
				if(DotNumb == 6)
				{
					update_dis_buf( &AllHardReport[0] );
					tm1637_display(  );
					return 1;		
				}
				else
				if(DotNumb == 7)
				{
					update_dis_buf( &ClrRecordOK[0] );
					tm1637_display(  );
					return 1;
				}
				else
				if(DotNumb == 8)
				{
					update_dis_buf( &ClrRecordEr[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//HP.02
				if(DotNumb == 9)
				{
					HopHardReport[2] = 0x3f;
					HopHardReport[3] = 0x5b;
					update_dis_buf( &HopHardReport[0] );
					tm1637_display(  );
					return 1;		
				}
				else
					return 0;	
			}
			else
				{
					if( DisData > 999 )
						{
							if( DotNumb == 3 )
								{
									temp = DisData/1000;										//��ǧλ
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//��ʾС����
									i++;
									temp = ( DisData%1000 )/100;								//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];		
									i++;
									temp = ((DisData%1000 )%100 )/10;							//��ʮλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ((DisData%1000 )%100 )%10;							//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else if( DotNumb == 2 )
								{
									temp = DisData/1000;										//��ǧλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%1000 )/100;								//���λ
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//��ʾС����
									i++;
									temp = ((DisData%1000 )%100 )/10;							//��ʮλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];		
									i++;
									temp = ((DisData%1000 )%100 )%10;							//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else if( DotNumb == 1 )
								{
									temp = DisData/1000;										//��ǧλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%1000 )/100;								//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ((DisData%1000 )%100 )/10;							//��ʮλ
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//��ʾС����
									i++;
									temp = ((DisData%1000 )%100 )%10;							//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else
								{
									temp = DisData/1000;										//��ǧλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%1000 )/100;								//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ((DisData%1000 )%100 )/10;							//��ʮλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];					//��ʾС����
									i++;
									temp = ((DisData%1000 )%100 )%10;							//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							update_dis_buf( &buf_temp[0] );
							tm1637_display( );
							return 1;
						}
					else if( DisData > 99 )
						{
							if( DotNumb == 2 )
								{
									buf_temp[ i ] = 0x00;										//ǧλ����ʾ
									i++;
									temp = DisData/100;											//���λ
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//��ʾС����		
									i++;
									temp = ( DisData%100 )/10;									//��ʮλ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%100 )%10;									//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else if( DotNumb == 1 )
								{
									buf_temp[ i ] = 0x00;										//ǧλ����ʾ
									i++;
									temp = DisData/100;											//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];		
									i++;
									temp = ( DisData%100 )/10;									//��ʮλ
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//��ʾС����	
									i++;
									temp = ( DisData%100 )%10;									//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else
								{
									if(DisMode == 5)
									{
										buf_temp[ i ] = 0x3f;										//ǧλ����ʾ
										i++;
										temp = DisData/100;											//���λ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )/10;									//��ʮλ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )%10;									//���λ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i = 0;
									}
									else
									{
										buf_temp[ i ] = 0x00;										//ǧλ����ʾ
										i++;
										temp = DisData/100;											//���λ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )/10;									//��ʮλ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )%10;									//���λ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i = 0;
									}									
								}
							update_dis_buf( &buf_temp[0] );
							tm1637_display( );
							return 1;
							
						}
					else if( DisData > 9 )
						{
							if( DotNumb == 1 )
								{
									buf_temp[ i ] = 0x00;										//ǧλ����ʾ
									i++;
									buf_temp[ i ] = 0x00;										//��λ����ʾ	
									i++;
									temp = DisData/10;											//��ʮλ
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//��ʾС����	
									i++;
									temp = DisData%10;											//���λ
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else
								{
									if(DisMode == 5)
									{
										buf_temp[ i ] = 0x3f;										//ǧλ����ʾ
										i++;
										buf_temp[ i ] = 0x3f;										//��λ����ʾ
										i++;
										temp = DisData/10;											//��ʮλ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = DisData%10;											//���λ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i = 0;
									}
									else
									{
										buf_temp[ i ] = 0x00;										//ǧλ����ʾ
										i++;
										buf_temp[ i ] = 0x00;										//��λ����ʾ
										i++;
										temp = DisData/10;											//��ʮλ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = DisData%10;											//���λ
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i = 0;
									}									
								}
							update_dis_buf( &buf_temp[0] );
							tm1637_display( );
							return 1;
						}
					else 
						{
							if(DisMode==5)
							{
								buf_temp[ i ] = 0x3f;
								i++;
								buf_temp[ i ] = 0x3f;
								i++;
								buf_temp[ i ] = 0x3f;
								i++;
								buf_temp[ i ] = Digital_Seg_Table[ DisData ];
								i = 0;
								update_dis_buf( &buf_temp[0] );
								tm1637_display( );
							}
							else
							{
								buf_temp[ i ] = 0x00;
								i++;
								buf_temp[ i ] = 0x00;
								i++;
								buf_temp[ i ] = 0x00;
								i++;
								buf_temp[ i ] = Digital_Seg_Table[ DisData ];
							}
							i = 0;
							update_dis_buf( &buf_temp[0] );
							tm1637_display( );
							return 1;
						}
				}
		} */
}

/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           ���״̬��
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserLedSet( unsigned char Mode )
{
	if( Mode == 1 )
		user_led = 1;
	else
		user_led = 0;
}		
/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           Ho
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserSeloutLedSet( unsigned char Mode  )
{
	if( Mode == 1 )
		user_selout_led = 1;
	else
		user_selout_led = 0;
}
/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           �����״̬��
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserNochargeLedSet( unsigned char Mode  )
{
	if( Mode == 1 )
		user_nocharge_led = 1;
	else
		user_nocharge_led = 0;
}
/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           Hopper��ֽ�������ϵ�
** Descriptions:            ���ָʾ�ƿ��غ���
** input parameters:        Mode:1-��/0-�ر�							
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void UserErrorLedSet( unsigned char Mode  )
{
	if( Mode == 1 )
		user_error_led = 1;
	else
		user_error_led = 0;
}

/*****************************************API END********************************************************/


/*______________________________________Ӳ����������____________________________________________________*/
void update_dis_buf( unsigned char * buf )
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		DIS_BUF[i] = *buf;
		buf++;
	}
}

void tm1637_i2c_write_onebyte( unsigned char p )  		//��TM1637д��һ���ֽ� 
{   
	unsigned int i;   
	tm1637_clk = 1;  									//TM1637 I2C���߿�ʼ�ź�    
	tm1637_dio = 1;   
	_nop_( );   
	_nop_( );   
	tm1637_dio = 0;   
	_nop_( );   	
	_nop_( );   
	tm1637_clk = 0;  									//��ʼ�źŴ�����    
	for( i=0;i<8;i++ )									//��ʼ����8λ����    
	{   
		tm1637_clk =0;   
		if( ( p&0x01 ) != 0 )   
		{   
			tm1637_dio = 1;   
		}   
		else   
		{   
			tm1637_dio = 0;   
		}   
		tm1637_clk = 1;   
		p = p>>1;   
	}													//8λ���ݴ�����1    
	tm1637_clk = 0;										//�ж�оƬ�������� ACK Ӧ���ź�    
	_nop_( );   
	_nop_( );   
	tm1637_dio = 1;   
	_nop_( );   
	_nop_( );   
	tm1637_clk = 1;   
	TM1637_ACK  = tm1637_dio;							//��Ӧ���źŸ� ACK ��    
	_nop_( );   
	_nop_( );   
}   
  
void tm1637_i2c_stop( ) 								//I2C���߽����ź�  
{   
	tm1637_clk = 0;   
	_nop_( );   
	_nop_( );   
	tm1637_dio = 0;   
	_nop_( );   
	_nop_( );   
	tm1637_clk = 1;   
	_nop_( );   
	_nop_( );   
	_nop_( );   
	tm1637_dio = 1;   
}   
   
void tm1637_display(  )   
{   
	unsigned char i=0,a=0;
	unsigned char addr = 0xc0 ;
	tm1637_dio = 1;   
	tm1637_clk = 1;   
	tm1637_i2c_write_onebyte(0x44);					//д����44H���̶���ַģʽ����ֹ�ж�Ӱ�촫�͹���    
	while( TM1637_ACK ); 							//�ȴ����ͽ���  
	tm1637_i2c_stop(); 								//���ý����źţ����뷢����һ������    
	_nop_( );   
	_nop_( );   
	_nop_( );   
	_nop_( );   
	for(a=0;a<6;a++)   
	{   
		tm1637_dio = 1;   
		tm1637_clk = 1; 
		tm1637_i2c_write_onebyte( addr++ );			//���͵�ַ  
		while(  TM1637_ACK  );						//�ȴ����ͽ���
		_nop_( );   
		_nop_( );   
		_nop_( );
		_nop_( );
		tm1637_i2c_write_onebyte( DIS_BUF[i++] );	//��������
		while(  TM1637_ACK  );						//�ȴ����
		tm1637_i2c_stop();
		_nop_( );
		_nop_( );
		_nop_( );
		_nop_( );	
	}   
	tm1637_i2c_stop(); 								//��������������󣬷������źţ����Է�����һ������    
	_nop_( );   
	_nop_( );   
	_nop_( );   
	tm1637_i2c_write_onebyte(0x8f); 				//����ʾ���������������    
	while( TM1637_ACK );   
	tm1637_i2c_stop();  
}
