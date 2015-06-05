/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           config.h
** Last modified Date:  2013-01-06
** Last Version:        No
** Descriptions:        ͷ�ļ����ã�λ�ڹ�����Ŀ¼                     
**------------------------------------------------------------------------------------------------------
** Created by:          chensunwei
** Created date:        2013-01-06
** Version:             V0.1
** Descriptions:        The original version        
********************************************************************************************************/
#ifndef __CONFIG_H 
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************************
  ͨ�ÿ����ͷ�ļ�
*********************************************************************************************************/
#include <stdint.h> 
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <setjmp.h>
#include <rt_misc.h>
#include <stdarg.h>
/*********************************************************************************************************
  �̼������ͷ�ļ�
*********************************************************************************************************/
#include ".\firmware\firmware.h"
#include ".\firmware\zy_if.h"
#include ".\firmware\LPC1700.h"
#include ".\firmware\target_cfg.h"
#include ".\firmware\LPC1700PinCfg.h"
//#include ".\firmware\bsp.h"
/*********************************************************************************************************
  ��������ͷ�ļ�
*********************************************************************************************************/
#include ".\Drivers\uart0.h"
#include ".\Drivers\uart1.h"
#include ".\Drivers\uart2.h"
#include ".\Drivers\uart3.h"
#include ".\Drivers\MDB.h"
#include ".\Drivers\EVB.h"
#include ".\Drivers\buzzer.h"
#include ".\Drivers\display.h"
#include ".\Drivers\i2c.h"
#include ".\Drivers\timer.h"
#include ".\Drivers\key.h"
#include ".\Drivers\RS232.h"
#include ".\Drivers\SPI.h"
#include ".\Drivers\RTC.H"
#include ".\Drivers\board.h"

/*********************************************************************************************************
  Ӧ�ò�
*********************************************************************************************************/

#include ".\APP\common.h"
#include ".\App\Global.h"

/*********************************************************************************************************
  �м��
*********************************************************************************************************/
#include ".\API\mdbApi.h"
#include ".\API\hopperApi.h"
#include ".\API\displayApi.h"
#include ".\API\task_dev.h"
#include ".\API\flashApi.h"
#include ".\API\coinApi.h"
#include ".\API\billApi.h"
#include ".\API\PcoinApi.h"
#ifdef __cplusplus
	}
#endif 

#endif

/**************************************End Of File*******************************************************/
