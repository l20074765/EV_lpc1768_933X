/*******************************************************************************************
								模块名称：display
								模块功能：实现对数码管显示控件的控制操作
								创建时间：2011-12-12
								开发人员：陈孙伟
********************************************************************************************/


#include <intrins.h>
#include "display.h"
#include <stdio.h>
#include <string.h>

unsigned char TM1637_ACK ;     
unsigned char DIS_BUF[6];
//            数字字符的显示             0    1    2    3     4   5    6    7    8    9
unsigned char Digital_Seg_Table[10] = { 0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f };
//                 特殊字符	     F    H    P    B    L    E	   C
unsigned char SpecialByte[7] = {0x71,0x76,0xf3,0x7f,0xb8,0x79,0x39};
//显示初始值0000
unsigned char StartDisp[6] = {0x3f,0x3f,0x3f,0x3f,0x3f,0x3f};
/*
unsigned char rootmemu[6] =	{ 0x71,0x3f,0xbf,0x3f,0xff,0xff };		//F00.0主菜单
unsigned char submemu1[6] =	{ 0x71,0x3f,0x86,0x3f,0xff,0xff };		//F01.0子菜单
unsigned char submemu2[6] =	{ 0x71,0x3f,0xdb,0x3f,0xff,0xff };		//F02.0子菜单
unsigned char submemu3[6] =	{ 0x71,0x3f,0xcf,0x3f,0xff,0xff };		//F03.0子菜单
unsigned char submemu4[6] =	{ 0x71,0x3f,0xe6,0x3f,0xff,0xff };		//F04.0子菜单
unsigned char submemu5[6] =	{ 0x71,0x3f,0xed,0x3f,0xff,0xff };		//F05.0子菜单
unsigned char HopperState1[6] = { 0x76,0x00,0x3f,0x3f,0xff,0xff};   //HX.00兑零器正常
unsigned char HopperState2[6] = { 0x76,0x00,0x3f,0x86,0xff,0xff};	//HX.01兑零器缺币
unsigned char HopperState3[6] = { 0x76,0x00,0x71,0x71,0xff,0xff};	//HX.FF兑零器故障
unsigned char HopHardReport[6] = { 0x76,0xf3,0x79,0x79,0xff,0xff};	//HP.EE找零器错误
unsigned char BillHardReport[6] = { 0x7f,0xb8,0x79,0x79,0xff,0xff};	//BL.EE收币器错误
unsigned char AllHardReport[6] = { 0x76,0x7f,0x79,0x79,0xff,0xff};	//HB.EE收币器、找零器错误
unsigned char ClrRecordOK[6] = { 0x39,0xb8,0x3f,0x3f,0xff,0xff};	//清楚记录成功
unsigned char ClrRecordEr[6] = { 0x39,0xb8,0x79,0x79,0xff,0xff};	//清楚记录失败
*/
void tm1637_i2c_write_onebyte( unsigned char ); 			//I2C写入一个字节    
void tm1637_i2c_stop( void ); 								//I2C总线结束信号      
void tm1637_display( void );								//启动tm1637显示函数
void update_dis_buf( unsigned char * );						//更新显存

/*********************************************************************************************************
** Programmer:				chensunw
** Function name:           InitEsdLed
** Descriptions:            显示初始化，点亮全部数码管，含小数点,可用于检测数码管是否故障
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
** Descriptions:            4位8段数码管显示API
** input parameters:        DisData,输入显示内容；
							DotNumb,小数点位数：
								0-显示XXXX
								1-显示XXX.X
								2-显示XX.XX
								3-显示X.XXX
								其他值无效
							DisMode显示模式:0-金额显示模式，1-菜单显示模式  3-一些错误信息或菜单信息的显示，需结合DotNumb
							0金额显示模式:
								DisData<=9999;DotNumb<=3								
							1菜单显示模式:
								DisData=0，F00.0
								DisData=1，F01.0
								.
								.
								.
								DisData=5，F05.0							
** output parameters:       none
** Returned value:          0-参数错误，1-显示正常
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
			if( DisMode == 1 )								//处理菜单显示
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
				//显示HX.00
				if(DotNumb == 1)
				{
					//显示H1.00
					if(DisData == 1)
						HopperState1[1] = 0x86;
					else
					//显示H1.01
					if(DisData == 2)
						HopperState1[1] = 0xdb;
					else
					//显示H1.ff
					if(DisData == 3)
						HopperState1[1] = 0xcf;
					update_dis_buf( &HopperState1[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//显示HX.00
				if(DotNumb == 2)
				{
					//显示H2.00
					if(DisData == 1)
						HopperState2[1] = 0x86;
					else
					//显示H2.01
					if(DisData == 2)
						HopperState2[1] = 0xdb;
					else
					//显示H2.ff
					if(DisData == 3)
						HopperState2[1] = 0xcf;
					update_dis_buf( &HopperState2[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//显示HX.00
				if(DotNumb == 3)
				{
					//显示H3.00
					if(DisData == 1)
						HopperState3[1] = 0x86;
					else
					//显示H3.01
					if(DisData == 2)
						HopperState3[1] = 0xdb;
					else
					//显示H3.ff
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
				//显示F04.1
				if(DotNumb == 1)
				{
					strcpy(ClearHopBuf,submemu4);
					ClearHopBuf[3] = 0x06;
					update_dis_buf( &ClearHopBuf[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//显示F04.2
				if(DotNumb == 2)
				{
					strcpy(ClearHopBuf,submemu4);
					ClearHopBuf[3] = 0x5b;
					update_dis_buf( &ClearHopBuf[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//显示F04.3
				if(DotNumb == 3)
				{
					strcpy(ClearHopBuf,submemu4);
					ClearHopBuf[3] = 0x4f;
					update_dis_buf( &ClearHopBuf[0] );
					tm1637_display(  );
					return 1;
				}
				else
				//显示HP.EE
				if(DotNumb == 4)
				{
					update_dis_buf( &HopHardReport[0] );
					tm1637_display(  );
					return 1;		
				}
				else
				//显示BL.EE
				if(DotNumb == 5)
				{
					update_dis_buf( &BillHardReport[0] );
					tm1637_display(  );
					return 1;		
				}
				else
				//显示HB.EE
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
									temp = DisData/1000;										//求千位
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//显示小数点
									i++;
									temp = ( DisData%1000 )/100;								//求百位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];		
									i++;
									temp = ((DisData%1000 )%100 )/10;							//求十位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ((DisData%1000 )%100 )%10;							//求个位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else if( DotNumb == 2 )
								{
									temp = DisData/1000;										//求千位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%1000 )/100;								//求百位
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//显示小数点
									i++;
									temp = ((DisData%1000 )%100 )/10;							//求十位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];		
									i++;
									temp = ((DisData%1000 )%100 )%10;							//求个位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else if( DotNumb == 1 )
								{
									temp = DisData/1000;										//求千位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%1000 )/100;								//求百位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ((DisData%1000 )%100 )/10;							//求十位
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//显示小数点
									i++;
									temp = ((DisData%1000 )%100 )%10;							//求个位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else
								{
									temp = DisData/1000;										//求千位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%1000 )/100;								//求百位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ((DisData%1000 )%100 )/10;							//求十位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];					//显示小数点
									i++;
									temp = ((DisData%1000 )%100 )%10;							//求个位
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
									buf_temp[ i ] = 0x00;										//千位不显示
									i++;
									temp = DisData/100;											//求百位
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//显示小数点		
									i++;
									temp = ( DisData%100 )/10;									//求十位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i++;
									temp = ( DisData%100 )%10;									//求个位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else if( DotNumb == 1 )
								{
									buf_temp[ i ] = 0x00;										//千位不显示
									i++;
									temp = DisData/100;											//求百位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];		
									i++;
									temp = ( DisData%100 )/10;									//求十位
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//显示小数点	
									i++;
									temp = ( DisData%100 )%10;									//求个位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else
								{
									if(DisMode == 5)
									{
										buf_temp[ i ] = 0x3f;										//千位不显示
										i++;
										temp = DisData/100;											//求百位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )/10;									//求十位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )%10;									//求个位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i = 0;
									}
									else
									{
										buf_temp[ i ] = 0x00;										//千位不显示
										i++;
										temp = DisData/100;											//求百位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )/10;									//求十位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = ( DisData%100 )%10;									//求个位
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
									buf_temp[ i ] = 0x00;										//千位不显示
									i++;
									buf_temp[ i ] = 0x00;										//百位不显示	
									i++;
									temp = DisData/10;											//求十位
									buf_temp[ i ] = ( Digital_Seg_Table[ temp ] | 0x80 );		//显示小数点	
									i++;
									temp = DisData%10;											//求个位
									buf_temp[ i ] = Digital_Seg_Table[ temp ];
									i = 0;									
								}
							else
								{
									if(DisMode == 5)
									{
										buf_temp[ i ] = 0x3f;										//千位不显示
										i++;
										buf_temp[ i ] = 0x3f;										//百位不显示
										i++;
										temp = DisData/10;											//求十位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = DisData%10;											//求个位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i = 0;
									}
									else
									{
										buf_temp[ i ] = 0x00;										//千位不显示
										i++;
										buf_temp[ i ] = 0x00;										//百位不显示
										i++;
										temp = DisData/10;											//求十位
										buf_temp[ i ] = Digital_Seg_Table[ temp ];
										i++;
										temp = DisData%10;											//求个位
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
** Function name:           面板状态灯
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
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
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
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
** Function name:           无零币状态灯
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
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
** Function name:           Hopper或纸币器故障灯
** Descriptions:            面板指示灯开关函数
** input parameters:        Mode:1-打开/0-关闭							
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


/*______________________________________硬件驱动程序____________________________________________________*/
void update_dis_buf( unsigned char * buf )
{
	unsigned char i;
	for(i=0;i<4;i++)
	{
		DIS_BUF[i] = *buf;
		buf++;
	}
}

void tm1637_i2c_write_onebyte( unsigned char p )  		//向TM1637写入一个字节 
{   
	unsigned int i;   
	tm1637_clk = 1;  									//TM1637 I2C总线开始信号    
	tm1637_dio = 1;   
	_nop_( );   
	_nop_( );   
	tm1637_dio = 0;   
	_nop_( );   	
	_nop_( );   
	tm1637_clk = 0;  									//开始信号传送完    
	for( i=0;i<8;i++ )									//开始传送8位数据    
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
	}													//8位数据传送完1    
	tm1637_clk = 0;										//判断芯片发过来的 ACK 应答信号    
	_nop_( );   
	_nop_( );   
	tm1637_dio = 1;   
	_nop_( );   
	_nop_( );   
	tm1637_clk = 1;   
	TM1637_ACK  = tm1637_dio;							//将应答信号给 ACK 。    
	_nop_( );   
	_nop_( );   
}   
  
void tm1637_i2c_stop( ) 								//I2C总线结束信号  
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
	tm1637_i2c_write_onebyte(0x44);					//写命令44H，固定地址模式，防止中断影响传送过程    
	while( TM1637_ACK ); 							//等待传送结束  
	tm1637_i2c_stop(); 								//调用结束信号，进入发送下一个命令    
	_nop_( );   
	_nop_( );   
	_nop_( );   
	_nop_( );   
	for(a=0;a<6;a++)   
	{   
		tm1637_dio = 1;   
		tm1637_clk = 1; 
		tm1637_i2c_write_onebyte( addr++ );			//传送地址  
		while(  TM1637_ACK  );						//等待传送结束
		_nop_( );   
		_nop_( );   
		_nop_( );
		_nop_( );
		tm1637_i2c_write_onebyte( DIS_BUF[i++] );	//传送数据
		while(  TM1637_ACK  );						//等待完成
		tm1637_i2c_stop();
		_nop_( );
		_nop_( );
		_nop_( );
		_nop_( );	
	}   
	tm1637_i2c_stop(); 								//将所有数据送完后，发结束信号，可以发送下一条命令    
	_nop_( );   
	_nop_( );   
	_nop_( );   
	tm1637_i2c_write_onebyte(0x8f); 				//打开显示并把亮度跳到最大    
	while( TM1637_ACK );   
	tm1637_i2c_stop();  
}
