/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**--------------File Info--------------------------------------------------------------------------------
** File name:           target.c
** Last modified Date:  2009-07-22
** Last Version:        V1.00
** Descriptions:        the specific codes for LPC1700 target boards
**                      Every project should include a copy of this file, user may modify it as needed
**------------------------------------------------------------------------------------------------------
** Created by:          chenmingji
** Created date:        2009-07-22
** Version:             V1.00
** Descriptions:        The original version
*********************************************************************************************************/
#include  "..\config.h"
/*********************************************************************************************************
** Function name:           defaultVectorHandle
** Descriptions:            Ĭ���쳣�������
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void defaultVectorHandle (void)
{
#ifdef DEBUG 
    while (1);
#else
    zyReset(ZY_HARD_RESET);
#endif                                                                  /*  DEBUG                       */
}
/*********************************************************************************************************
** Function name:           targetInit
** Descriptions:            Initialize the target
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
void targetInit (void)
{
    extern void *__GpvVectorTable[];
#if VECTOR_TABLE_IN_FLASH == 0
    extern void *__GpvVectorRom[];
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */

#if VECTOR_TABLE_IN_FLASH == 0
    VTOR    = (unsigned long)__GpvVectorRom;
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */
    VTOR    = (unsigned long)__GpvVectorTable;
    zyIrqDisable();
    //��FLASH�ڰ�ȫ�������У�������Դ���  
    FLASHCFG = ((0x05ul << 12) & (~(0x003f))) | 0x003a;
	//Set system timers for each component  
#if (FPCLK / (FCCLK / 4)) == 1
    PCLKSEL0 = 0x00000000;                                              /*  PCLK is 1/4 CCLK            */
    PCLKSEL1 = 0x00000000;
#endif                                                                  /*  (FPCLK / (FCCLK / 4))       */

#if (FPCLK / (FCCLK / 4)) == 2
    PCLKSEL0 = 0xAAAAAAAA;                                              /*  PCLK is 1/2 CCLK            */
    PCLKSEL1 = 0xAAAAAAAA;   
#endif                                                                  /*  (FPCLK / (FCCLK / 4))       */

#if (FPCLK / (FCCLK / 4)) == 4
    PCLKSEL0 = 0x55555555;                                              /*  PCLK is the same as CCLK    */
    PCLKSEL1 = 0x55555555;      
#endif                                                                  /*  (FPCLK / (FCCLK / 4))       */
    CLKOUTCFG = 0;  
    //�ر�PLL��������ٴ���
    if ((PLL0STAT >> 24) == 1) 
	{
        PLL0CON = 1;                                                    /*  Enable PLL, disconnected    */
        PLL0FEED = 0xAA;
        PLL0FEED = 0x55;
    }
    PLL0CON = 0;                                                        /*  Disable PLL, disconnected   */
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;
    while(PLL0STAT & (3 << 24)) {}   
    //������Ҫʱ��
    SCS = (SCS & 0x04) | 0x20;                                          /*  Enable mainOSC,1MHz~20MHz   */ 
    while ((SCS & (1ul << 6)) == 0) {}
    CLKSRCSEL = 0x01;                                                   /*  select main OSC as the PLL  */
    PLL0CFG   = (((PLL_NVALUE - 1) << 16) | (PLL_MVALUE - 1));    
    PLL0FEED  = 0xAA;                                                   /*  Enable but disconnect PLL   */
    PLL0FEED  = 0x55;    
    PLL0CON   = 1;
    PLL0FEED  = 0xAA;                                                   /*  Enable but disconnect PLL   */
    PLL0FEED  = 0x55;    
    while ((PLL0STAT & (1ul << 24)) == 0) {}
    CCLKCFG = (FCCO / FCCLK) - 1;   
    while (((PLL0STAT & (1ul << 26)) == 0)) {                           /*  Check lock bit status       */
    }
    while (((PLL0STAT & 0x00007FFF) != (PLL_MVALUE - 1)) && 
           (((PLL0STAT & 0x00FF0000) >> 16) != (PLL_NVALUE - 1))) {
    }
    PLL0CON  = 3;                                                       /*  connect the PLL             */
    PLL0FEED = 0xAA;
    PLL0FEED = 0x55;                                
    while ((PLL0STAT & (1ul << 25)) == 0) {                             /*  Wait until the PLL is       */ 
    }
                                                                        /*  connected and locked        */																		  
    //����USBʱ��
#if FUSBCLK_EN == 1
#if (FCCO % (FUSBCLK * 2)) != 0
    PLL1CON = 1;
    PLL1CFG = ((FUSBCLK / FOSC) - 1) | (1 << 5);

    PLL1FEED = 0xaa;
    PLL1FEED = 0x55;
    while ((PLL1STAT & (1 << 10)) == 0) {
    }
    PLL1CON  = 3;
    PLL1FEED = 0xaa;
    PLL1FEED = 0x55;
#else
    USBCLKCFG = (FCCO / (FUSBCLK * 2)) - 1;
    PLL1CON   = 0;
    PLL1FEED  = 0xaa;
    PLL1FEED  = 0x55;    
#endif                                                                  /*  (FCCO % (FUSBCLK * 2))      */
#endif                                                                  /*  FUSBCLK_EN                  */
    //���ô洢������ģ��
#if FCCLK <= 20000000
    FLASHCFG = ((0x01ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash����ʹ��1��CPUʱ��     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 20000000 && FCCLK <= 40000000
    FLASHCFG = ((0x02ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash����ʹ��2��CPUʱ��     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 40000000 && FCCLK <= 60000000
    FLASHCFG = ((0x03ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash����ʹ��3��CPUʱ��     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 60000000 && FCCLK <= 80000000
    FLASHCFG = ((0x04ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash����ʹ��4��CPUʱ��     */
#endif                                                                  /*  FCCLK                       */

#if FCCLK > 80000000 && FCCLK <= 100000000
    FLASHCFG = ((0x05ul << 12) & (~(0x003f))) | 0x003a;                 /*  Flash����ʹ��5��CPUʱ��     */
#endif                                                                  /*  FCCLK                       */
    STCALIB = FCCLK / 100;
    zyIsrSet(SVCI, (unsigned long)svcallHandle, 0x00);
	zyIrqEnable();
    zyIfInit();
    return;
}
/**************************************End Of File*******************************************************/
