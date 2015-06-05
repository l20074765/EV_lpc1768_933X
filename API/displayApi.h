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

void initDisplay(void);

void ledPaomaDisplay(void);
void LED_showData(uint32 data);
void LED_showString(char *str);
void disp_init_page(unsigned char type);
void disp_err_page(void);
void disp_free_page(unsigned char vmcChangeLow);
void disp_clear_screen(void);
void disp_sale_page(unsigned char hasBuy,unsigned int amount,unsigned char point);
#endif


