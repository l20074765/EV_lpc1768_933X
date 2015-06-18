/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           common.h
** Last modified Date:  2014-04-04
** Last Version:        No
** Descriptions:        系统参数定义及通用函数部分                     
**------------------------------------------------------------------------------------------------------
** Created by:          yanbo
** Created date:        2013-04-04
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#ifndef _DISPLAYAPI_H_ 
#define _DISPLAYAPI_H_
#include "..\Drivers\board.h"





void LED_ctrl(uint8 no,uint8 s);



void initDisplay(void);
void LED_hopperCheck(void);

void LED_showData(uint32 data);
void LED_showAmount(uint32 amount);
void LED_showLargeData(uint32 value,uint8 point);
void LED_showLargeDataByPage(uint32 value,uint8 point);
void LED_show(char *format,...);
void LED_showString(char *str);
void disp_init_page(unsigned char type);
void disp_free_page(unsigned char vmcChangeLow);
void disp_clear_screen(void);
void disp_sale_page(unsigned char hasBuy,unsigned int amount,unsigned char point);
#endif


