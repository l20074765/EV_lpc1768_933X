/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info--------------------------------------------------------------------------------
** File name:			LPC1700PinCfg.c
** Last modified Date:  2009-05-12
** Last Version:		V1.01
** Descriptions:		��������		
**-------------------------------------------------------------------------------------------------------
** Modified by:			sunway
** Modified date:		2013-01-06
** Version:				V1.2
** Descriptions:		�Գ���ķ���Լ�ע��������������������
********************************************************************************************************/
#ifndef __LPC1700PINCFG_H
#define __LPC1700PINCFG_H
/*********************************************************************************************************
** P0�����Ź�������
*********************************************************************************************************/
// P0.0
#define	P0_0_GPIO             0x00                                      /* GPIO                         */
#define P0_0_RD1              0x01                                      /* CAN-1��������������          */
#define P0_0_TXD3             0x02                                      /* UART3��������                */
#define P0_0_SDA1             0x03                                      /* I2C-1�����ߣ���©��          */
#define P0_0_FNUC             P0_0_TXD3
//P0.1
#define P0_1_GPIO             0x00                                      /* GPIO                         */
#define P0_1_TD1              0x01                                      /* CAN-1��������������          */
#define P0_1_RXD3             0x02                                      /* UART3��������                */
#define P0_1_SCL1             0x03                                      /* I2C-1ʱ���ߣ���©��          */
#define P0_1_FNUC             P0_1_RXD3
//P0.2
#define P0_2_GPIO             0x00                                      /* GPIO                         */
#define P0_2_TXD0             0x01                                      /* UART0��������                */
#define P0_2_AD07             0x02                                      /* AD0.7����                    */
//...   Reserved              0x03
#define P0_2_FNUC             P0_2_TXD0
//P0.3
#define P0_3_GPIO             0x00                                      /* GPIO                         */
#define P0_3_RXD0             0x01                                      /* UART0��������                */
#define P0_3_AD06             0x02                                      /* AD0.6����                    */
//...   Reserved              0x03 
#define P0_3_FNUC             P0_3_RXD0
//P0.4
#define P0_4_GPIO             0x00                                      /* GPIO                         */
#define P0_4_I2SRX_CLK        0x01                                      /* I2S���߽���ʱ��SCK           */
#define P0_4_RD2              0x02                                      /* CAN-2������                  */
#define P0_4_CAP20            0x03                                      /* ��ʱ��2�������룬ͨ��0       */		
#define P0_4_FNUC             P0_4_GPIO
//P0.5
#define P0_5_GPIO             0x00                                      /* GPIO                         */
#define P0_5_I2SRX_WS         0x01                                      /* I2S���߽�����ѡ��WS          */
#define P0_5_TD2              0x02                                      /* CAN-2������                  */
#define P0_5_CAP21            0x03                                      /* ��ʱ��2�������룬ͨ��1       */		
#define P0_5_FNUC             P0_5_GPIO
//P0.6
#define P0_6_GPIO             0x00                                      /* GPIO                         */
#define P0_6_I2SRX_SDA        0x01                                      /* I2S���߽���������SD          */
#define P0_6_SSEL1            0x02                                      /* SSP-1���ߴӻ�ѡ��            */
#define P0_6_MAT20            0x03                                      /* ��ʱ��2ƥ�������ͨ��0       */		
#define P0_6_FNUC             P0_6_GPIO
//P0.7
#define P0_7_GPIO             0x00                                      /* GPIO                         */
#define P0_7_I2STX_CLK        0x01                                      /* I2S���߷���ʱ����SCK         */
#define P0_7_SSCK1            0x02                                      /* SSP-1ʱ����                  */
#define P0_7_MAT21            0x03                                      /* ��ʱ��2ƥ�������ͨ��1       */	
#define P0_7_FNUC             P0_7_GPIO
//P0.8
#define P0_8_GPIO             0x00                                      /* GPIO                         */
#define P0_8_I2STX_WS         0x01                                      /* I2S���߷�����ѡ��WS          */
#define P0_8_SMISO1           0x02                                      /* SSP-1����������������      */
#define P0_8_MAT22            0x03                                      /* ��ʱ��2ƥ�������ͨ��2       */
#define P0_8_FNUC             P0_8_GPIO		
//P0.9
#define P0_9_GPIO             0x00                                      /* GPIO                         */
#define P0_9_I2STX_SDA        0x01                                      /* I2S���߷���������SD          */
#define P0_9_SMOSI1           0x02                                      /* SSP-1�����������������      */
#define P0_9_MAT23            0x03                                      /* ��ʱ��2ƥ�������ͨ��3       */
#define P0_9_FNUC             P0_9_GPIO
//P0.10
#define P0_10_GPIO            0x00                                      /* GPIO                         */
#define P0_10_TXD2            0x01                                      /* UART2��������                */
#define P0_10_SDA2            0x02                                      /* I2C-2������(�ǿ�©����)      */
#define P0_10_MAT30           0x03                                      /* ��ʱ��3ƥ�������ͨ��0       */
#define P0_10_FNUC            P0_10_TXD2               
//P0.11
#define P0_11_GPIO            0x00                                      /* GPIO                         */
#define P0_11_RXD2            0x01                                      /* UART2��������                */
#define P0_11_SCL2            0x02                                      /* I2C-2ʱ����(�ǿ�©����)      */
#define P0_11_MAT31           0x03                                      /* ��ʱ��3ƥ�������ͨ��1       */
#define P0_11_FNUC            P0_11_RXD2                
//P0.15
#define P0_15_GPIO            0x00                                      /* GPIO                         */
#define P0_15_TXD1            0x01                                      /* UART1��������                */
#define P0_15_SCK0            0x02                                      /* SSP0ʱ����                   */
#define P0_15_SCK             0x03                                      /* SPIʱ����                    */                                             
#define P0_15_FNUC            P0_15_GPIO                                                                
//P0.16
#define P0_16_GPIO            0x00                                      /* GPIO                         */
#define P0_16_RXD1            0x01                                      /* UART1��������                */
#define P0_16_SSEL0           0x02                                      /* SSP0�ӻ�ѡ��                 */
#define P0_16_SSEL            0x03                                      /* SPI�ӻ�ѡ��                  */                                               
#define P0_16_FNUC            P0_16_GPIO                                                         
//P0.17
#define P0_17_GPIO            0x00                                      /* GPIO                         */
#define P0_17_CTS1            0x01                                      /* UART1���㷢���ź�CTS         */
#define P0_17_MISO0           0x02                                      /* SSP0����������������       */
#define P0_17_MISO            0x03                                      /* SPI����ӳ�������            */                                               
#define P0_17_FNUC            P0_17_GPIO                                                         
//P0.18
#define P0_18_GPIO            0x00                                      /* GPIO                         */
#define P0_18_DCD1            0x01                                      /* UART1�����ز��������DCD     */
#define P0_18_MOSI0           0x02                                      /* SSP-0�����������������      */
#define P0_18_MOSI            0x03                                      /* SPI��������������            */                                              
#define P0_18_FNUC            P0_18_GPIO                                                          
//P0.19
#define P0_19_GPIO            0x00                                      /* GPIO                         */
#define P0_19_DSR1            0x01                                      /* UART1�������þ���DSR         */
//...   Reserved              0x02                                      /* ����                         */
#define P0_19_SDA1            0x03                                      /* I2C-1�����ߣ��ǿ�©��        */                                              
#define P0_19_FNUC            P0_19_GPIO                                                          
//P0.20
#define P0_20_GPIO            0x00                                      /* GPIO                         */
#define P0_20_DTR1            0x01                                      /* UART1�ն˾���DTR             */
//...   Reserved              0x02                                  	/* ����                         */
#define P0_20_SCL1            0x03                                      /* I2C-1ʱ���ߣ��ǿ�©��        */                                          
#define P0_20_FNUC            P0_19_GPIO                                      
//P0.21
#define P0_21_GPIO            0x00                                      /* GPIO                         */
#define P0_21_RI1             0x01                                      /* UART1�����ź�                */
//...   Reserved              0x02                                      /* ����                         */
#define P0_21_RD1             0x03                                      /* CAN-1��������                */                                    
#define P0_21_FNUC            P0_21_GPIO                                      
//P0.22
#define P0_22_GPIO            0x00                                      /* GPIO                         */
#define P0_22_RTS1            0x01                                      /* UART1����������            */
//...   Reserved              0x02                                      /* ����                         */
#define P0_22_TD1             0x03                                      /* CAN-1������                  */                                   
#define P0_22_FNUC            P0_22_GPIO                                      
//P0.23
#define P0_23_GPIO            0x00                                      /* GPIO                         */
#define P0_23_AD00            0x01                                      /* ADC-0��ͨ��0                 */
#define P0_23_I2SRX_CLK       0x02                                      /* I2S����ʱ��SCK               */
#define P0_23_CAP30           0x03                                      /* ��ʱ��3�������룬ͨ��0       */                                     
#define P0_23_FNUC            P0_23_GPIO                                       
//P0.24
#define P0_24_GPIO            0x00                                      /* GPIO                         */
#define P0_24_AD01            0x01                                      /* ADC-0��ͨ��1                 */
#define P0_24_I2SRX_WS        0x02                                      /* I2S������ѡ��WS              */
#define P0_24_CAP31           0x03                                      /* ��ʱ��3�������룬ͨ��1       */                                      
#define P0_24_FNUC            P0_24_GPIO                                       
//P0.25
#define P0_25_GPIO            0x00                                      /* GPIO                         */
#define P0_25_AD02            0x01                                      /* ADC-0��ͨ��2                 */
#define P0_25_I2SRX_SDA       0x02                                      /* I2S��������SD                */
#define P0_25_TXD3            0x03                                      /* UART3��������                */                                  
#define P0_25_FNUC            P0_25_GPIO                                               
//P0.26
#define P0_26_GPIO            0x00                                      /* GPIO                         */
#define P0_26_AD03            0x01                                      /* ADC-0��ͨ��3                 */
#define P0_26_AOUT            0x02                                      /* DAC���                      */
#define P0_26_RXD3            0x03                                      /* UART-3��������               */                                        
#define P0_26_FNUC            P0_26_GPIO                                              
//P0.27
#define P0_27_GPIO            0x00                                      /* GPIO                         */
#define P0_27_SDA0            0x01                                      /* I2C0��������/�������©      */
#define P0_27_USB_SDA         0x02                                      /* USB�ӿ�I2C��������OTG�շ���  */
//..    Reserved              0x03                                      /* ����                         */                                         
#define P0_27_FNUC            P0_27_SDA0                                              
//P0.28
#define P0_28_GPIO            0x00                                      /* GPIO                         */
#define P0_28_SCL0            0x01                                      /* I2C0ʱ�����룬��©           */
#define P0_28_USB_SCL         0x02                                      /* USB�ӿ�I2Cʱ���ߣ�OTG�շ���  */
//..    Reserved              0x03                                   /* ����                         */                                     
#define P0_28_FNUC            P0_28_SCL0                                       
// P0.29
#define P0_29_GPIO            0x00                                      /* GPIO                         */
#define P0_29_USB_DD          0x01                                      /* USB˫������D+��              */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */                                      
#define P0_29_FNUC            P0_29_GPIO                                      
//P0.30
#define P0_30_GPIO            0x00                                      /* GPIO                         */
#define P0_30_USB_DS          0x01                                      /* USB˫������D-��              */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */                                     							  
#define P0_30_FNUC            P0_30_GPIO 
/*********************************************************************************************************
** P1�����Ź�������
*********************************************************************************************************/
//P1.0
#define P1_0_GPIO             0x00                                      /* GPIO                         */
#define P1_0_ENET_TXD0        0x01                                      /* ��̫���������ݽ�0            */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */
#define P1_0_FNUC             P1_0_GPIO
// P1.1
#define P1_1_GPIO             0x00                                      /* GPIO                         */
#define P1_1_ENET_TX_EN       0x01                                      /* ��̫���������ݽ�1            */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */
#define P1_1_FNUC             P1_1_GPIO
// P1.4
#define P1_4_GPIO             0x00                                      /* GPIO                         */
#define P1_4_ENET_TX_EN       0x01                                      /* ��̫������ʹ��               */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */
#define P1_4_FNUC             P1_4_GPIO
//P1.8
#define P1_8_GPIO             0x00                                      /* GPIO                         */
#define P1_8_ENET_CRS         0x01                                      /* ��̫���ز�����             */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */										     
#define P1_8_FNUC             P1_8_GPIO
//P1.9
#define P1_9_GPIO             0x00                                      /* GPIO                         */
#define P1_9_ENET_RXD0        0x01                                      /* ��̫���������ݽ�0            */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */													  
#define P1_9_FNUC             P1_9_GPIO
//P1.10
#define P1_10_GPIO            0x00                                      /* GPIO                         */
#define P1_10_ENET_RXD1       0x01                                      /* ��̫���������ݽ�1            */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */
#define P1_10_FNUC            P1_10_GPIO
//P1.11
#define P1_14_GPIO            0x00                                      /*  GPIO                        */
#define P1_14_ENET_RX_ER      0x01                                      /* ��̫�����մ���               */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */													     
#define P1_14_FNUC            P1_14_GPIO
//P1.15
#define P1_15_GPIO            0x00                                      /* GPIO                         */
#define P1_15_ENET_REF_CLK    0x01                                      /* ��̫���ο�ʱ������           */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */											   
#define P1_15_FNUC            P1_15_GPIO
//P1.16
#define P1_16_GPIO            0x00                                      /* GPIO                         */
#define P1_16_ENET_MDC        0x01                                      /* ��̫��MIIMʱ��               */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */
#define P1_16_FNUC            P1_16_GPIO
//P1.17
#define P1_17_GPIO            0x00                                      /* GPIO                         */
#define P1_17_ENET_MDIO       0x01                                      /* ��̫��MI��������\���        */
//..    Reserved              0x02                                      /* ����                         */
//..    Reserved              0x03                                      /* ����                         */
#define P1_17_FNUC            P1_17_GPIO
//P1.18
#define P1_18_GPIO            0x00                                      /*  GPIO                        */
#define P1_18_USB_UP_LED      0x01                                      /* USB GOODLINKָʾ��           */
#define P1_18_PWM11           0x02                                      /* PWM1�����ͨ��1              */
#define P1_18_CAP10           0x03                                      /* Timer1�������룬ͨ��0        */                                             
#define P1_18_FNUC            P1_18_GPIO                                                
//P1.19
#define P1_19_GPIO            0x00                                      /*  GPIO                        */
#define P1_19_MCO0A           0x01                                      /* �������PWMͨ��0�����A      */
#define P1_19_USB_PPWR        0x02                                      /* USB�˿ڵ�Դʹ���ź�          */
#define P1_19_CAP11           0x03                                      /* Timer1�������룬ͨ��1        */                                             
#define P1_19_FNUC            P1_19_GPIO                                                 
//P1.20
#define P1_20_GPIO            0x00                                      /*  GPIO                        */
#define P1_20_MCFB0           0x01                                      /* �������PWMͨ��0���ػ�����   */
#define P1_20_PWM12           0x02                                      /* PWM-2�����ͨ��2             */
#define P1_20_SCK0            0x03                                      /* SSP-0ʱ������                */                                            
#define P1_20_FNUC            P1_20_GPIO                                                
// P1.21
#define P1_21_GPIO            0x00                                      /* GPIO                         */
#define P1_21_MCABORT         0x01                                      /* �������PWM��������ֹ        */
#define P1_21_PWM13           0x02                                      /* PWM-1�����ͨ��3             */
#define P1_21_SSEL0           0x03                                      /* SSP-0�ӻ�ѡ��                */                                             
#define P1_21_FNUC            P1_21_GPIO                                                
//P1.22
#define P1_22_GPIO            0x00                                      /* GPIO                         */
#define P1_22_MC0B            0x01                                      /* �������PWMͨ��0�����B      */
#define P1_22_USB_PWRD        0x02                                      /* USB�˿ڵĵ�Դ״̬            */
#define P1_22_MAT10           0x03                                      /* ��ʱ��1ƥ�������ͨ��0       */                                             
#define P1_22_FNUC            P1_22_GPIO                                                 
//P1.23
#define P1_23_GPIO            0x00                                      /* GPIO                         */
#define P1_23_MCFB1           0x01                                      /* �������PWMͨ��1����������   */
#define P1_23_PWM14           0x02                                      /* PWM-1�����ͨ��4             */
#define P1_23_MISO0           0x03                                      /* SSP-0�������룬�ӻ����      */                                              
#define P1_23_FNUC            P1_23_GPIO                                                  
//P1.24
#define P1_24_GPIO            0x00                                      /* GPIO                         */
#define P1_24_MCFB2           0x01                                      /* �������PWMͨ��2����������   */
#define P1_24_PWM15           0x02                                      /* PWM-1�����ͨ��5             */
#define P1_24_MOSI0           0x03                                      /* SSP-0����������ӻ�����      */                                             
#define P1_24_FNUC            P1_24_GPIO                                               
//P1.25
#define P1_25_GPIO            0x00                                      /* GPIO                         */
#define P1_25_MC1A            0x01                                      /* �������PWMͨ��1�����A      */
#define P1_25_CLKOUT          0x02                                      /* ʱ���������                 */
#define P1_25_MAT11           0x03                                      /* ��ʱ��1ƥ�������ͨ��1       */                                             
#define P1_25_FNUC            P1_25_GPIO                                                 
//P1.26
#define P1_26_GPIO            0x00                                      /* GPIO                         */
#define P1_26_MC1B            0x01                                      /* �������PWMͨ��1�����B      */
#define P1_26_PWM16           0x02                                      /* PWM-1�����ͨ��6             */
#define P1_26_CAP00           0x03                                      /* ��ʱ��0�������룬ͨ��0       */                                             
#define P1_26_FNUC            P1_26_GPIO                                             
//P1.27
#define P1_27_GPIO            0x00                                      /* GPIO                         */
#define P1_27_CLKOUT          0x01                                      /* ʱ���������                 */
#define P1_27_USB_OVRCR       0x02                                      /* USB�˿ڹ��ص���״̬          */
#define P1_27_CAP01           0x03                                      /* ��ʱ��0�������룬ͨ��1       */                                              
#define P1_27_FNUC            P1_27_GPIO                                                 
// P1.28
#define P1_28_GPIO            0x00                                      /* GPIO                         */
#define P1_28_MC2A            0x01                                      /* �������PWMͨ��2�����A      */
#define P1_28_PCAP10          0x02                                      /* PWM-1�������룬ͨ��0         */
#define P1_28_MAT00           0x03                                      /* ��ʱ��0ƥ�������ͨ��0       */                                             
#define P1_28_FNUC            P1_28_GPIO                                                 
// P1.29
#define P1_29_GPIO            0x00                                      /* GPIO                         */
#define P1_29_MC2B            0x01                                      /* �������PWMͨ��2�����B      */
#define P1_29_PCAP11          0x02                                      /* PWM-1�������룬ͨ��1         */
#define P1_29_MAT01           0x03                                      /* ��ʱ��0ƥ�������ͨ��1       */                                             
#define P1_29_FNUC            P1_29_GPIO                                                  
//P1.30
#define P1_30_GPIO            0x00                                      /* GPIO                         */
//...   Reserved              0x01                                      /* ����                         */
#define P1_30_VBUS            0x02                                      /* ���ڵ�USB���ߵ�Դ            */
#define P1_30_AD04            0x03                                      /* ADCת����0.ͨ��4����         */                                             
#define P1_30_FNUC            P1_30_GPIO                                                 
//P1.31
#define P1_31_GPIO            0x00                                      /* GPIO                         */
//...   Reserved              0x01                                      /* ����                         */
#define P1_31_SSCK1           0x02                                      /* SSP-1ʱ����                  */
#define P1_31_AD05            0x03                                      /* ADC-0���룬ͨ��5             */                                            
#define P1_31_FNUC            P1_31_GPIO                                                
/*********************************************************************************************************
** P2�����Ź�������
*********************************************************************************************************/
//P2.0
#define P2_0_GPIO             0x00                                      /* GPIO                         */
#define P2_0_PWM11            0x01                                      /* PWM-1��ͨ��1                 */
#define P2_0_TXD1             0x02                                      /* UART1���ݷ�������            */
//...   Reserved              0x03                                      /* ����                         */
#define P2_0_FNUC             P2_0_GPIO
// P2.1
#define P2_1_GPIO             0x00                                      /* GPIO                         */
#define P2_1_PWM12            0x01                                      /* PWM-1��ͨ��2                 */
#define P2_1_RXD1             0x02                                      /* UART1���ݽ�������            */
//...   Reserved              0x03                                      /* ����                         */
#define P2_1_FNUC             P2_1_GPIO
// P2.2
#define P2_2_GPIO             0x00                                      /* GPIO                         */
#define P2_2_PWM13            0x01                                      /* PWM-1��ͨ��3                 */
#define P2_2_CTS1             0x02                                      /* UART1���㷢����              */
//...   Reserved              0x03                                      /* ����                         */
#define P2_2_FNUC             P2_2_GPIO
//P2.3
#define P2_3_GPIO             0x00                                      /* GPIO                         */
#define P2_3_PWM14            0x01                                      /* PWM-1��ͨ��4                 */
#define P2_3_DCD1             0x02                                      /* UART1�����ز������          */
//...   Reserved              0x03                                      /* ����                         */
#define P2_3_FNUC             P2_3_GPIO
// P2.4
#define P2_4_GPIO             0x00                                      /* GPIO                         */
#define P2_4_PWM15            0x01                                      /* PWM-1��ͨ��5                 */
#define P2_4_DSR1             0x02                                      /* UART1�������þ�����          */
//...   Reserved              0x03                                      /* ����                         */
#define P2_4_FNUC             P2_4_GPIO
//P2.5
#define P2_5_GPIO             0x00                                      /* GPIO                         */
#define P2_5_PWM16            0x01                                      /* PWM-1��ͨ��6                 */
#define P2_5_DTR1             0x02                                      /* UART1�����ն˾�����          */
//...   Reserved              0x03                                      /* ����                         */
#define P2_5_FNUC             P2_5_GPIO
//P2.6
#define P2_6_GPIO             0x00                                      /* GPIO                         */
#define P2_6_PCAP10           0x01                                      /* PWM-1�������룬ͨ��0         */
#define P2_6_RI1              0x02                                      /* UART1��������                */
//...   Reserved              0x03                                      /* ����                         */
#define P2_6_FNUC             P2_6_GPIO
//P2.7
#define P2_7_GPIO             0x00                                      /* GPIO                         */
#define P2_7_RD2              0x01                                      /* CAN-2������                  */
#define P2_7_RTS1             0x02                                      /* UART1���������            */                                                                       /* RS-485/EIA-485���ʹ���ź�   */
//...   Reserved              0x03                                      /* ����                         */
#define P2_7_FNUC             P2_7_GPIO
//P2.8
#define P2_8_GPIO             0x00                                      /* GPIO                         */
#define P2_8_TD2              0x01                                      /* CAN-2������                  */
#define P2_8_TXD2             0x02                                      /* UART2��������                */
#define P2_8_ENET_MDC         0x03                                      /* ��̫��MIIMʱ��               */
#define P2_8_FNUC             P2_8_GPIO
//P2.9
#define P2_9_GPIO             0x00                                      /* GPIO                         */
#define P2_9_U1CONNECT        0x01                                      /* USB1�����ӿ���               */
#define P2_9_RXD2             0x02                                      /* UART2��������                */
#define P2_9_ENET_MDIO        0x03                                      /* ��̫��MIIM������������     */
#define P2_9_FNUC             P2_9_GPIO
// P2.10��ISP��
#define P2_10_GPIO            0x00                                      /* GPIO                         */
#define P2_10_EINT0           0x01                                      /* �ⲿ�ж�0����                */
#define P2_10_NMI             0x02                                      /* �������ж�����               */
//...   Reserved              0x03                                      /* ����                         */                                             
#define P2_10_FNUC            P2_10_GPIO                                                 
//P2.11
#define P2_11_GPIO            0x00                                      /* GPIO                         */
#define P2_11_EINT1           0x01                                      /* �ⲿ�ж�1����                */
//...   Reserved              0x02                                      /* ����                         */
#define P2_11_I2STX_CLK       0x03                                      /* I2S����ʱ��SCK               */                                             
#define P2_11_FNUC            P2_11_GPIO                                                 
//P2.12
#define P2_12_GPIO            0x00                                      /* GPIO                         */
#define P2_12_EINT2           0x01                                      /* �ⲿ�ж�2����                */
//...   Reserved              0x02                                      /* ����                         */
#define P2_12_I2STX_WS        0x03                                      /* I2S������ѡ��WS              */                                            
#define P2_12_FNUC            P2_12_EINT2                                                 
// P2.13
#define P2_13_GPIO            0x00                                      /* GPIO                         */
#define P2_13_EINT3           0x01                                      /* �ⲿ�ж�3����                */
//...   Reserved              0x02                                      /* ����                         */
#define P2_13_I2STX_SDA       0x03                                      /* I2S��������SD                */                                              
#define P2_13_FNUC            P2_13_GPIO                                            
/*********************************************************************************************************
** P3�����Ź�������
*********************************************************************************************************/
//P3.25
#define P3_25_GPIO            0x00                                      /*  GPIO                        */
//...   Reserved              0x01                                      /*  ����                        */
#define P3_25_MAT00           0x02                                      /*  ��ʱ��0ƥ�������ͨ��0      */
#define P3_25_PWM12           0x03                                      /*  PWM-1�����ͨ��2            */                                              
#define P3_25_FNUC            P3_25_GPIO                                                    
//P3.26
#define P3_26_GPIO            0x00                                      /* GPIO                         */
#define P3_26_STCLK           0x01                                      /* ϵͳ���Ķ�ʱ��ʱ������       */
#define P3_26_MAT01           0x02                                      /* ��ʱ��0ƥ�������ͨ��1       */
#define P3_26_PWM13           0x03                                      /* PWM-1�����ͨ��3             */                                              
#define P3_26_FNUC            P3_26_GPIO                                             
/*********************************************************************************************************
** P4�����Ź�������
*********************************************************************************************************/
//P4.28
#define P4_28_GPIO            0x00                                      /* GPIO                         */
#define P4_28_RX_MCLK         0x01                                      /* ��I2S��������ʱ��            */
#define P4_28_MAT20           0x02                                      /* ��ʱ��2ƥ�������ͨ��0       */
#define P4_28_TXD3            0x03                                      /* UART3��������                */                                              
#define P4_28_FNUC            P4_28_GPIO                                                   
//P4.29
#define P4_29_GPIO            0x00                                      /* GPIO                         */
#define P4_29_TX_MCLK         0x01                                      /* I2S��������ʱ��              */
#define P4_29_MAT21           0x02                                      /* ��ʱ��2ƥ�������ͨ��1       */
#define P4_29_RXD3            0x03                                      /* UART3��������                */                                             
#define P4_29_FNUC            P4_29_GPIO                                               
/*********************************************************************************************************
**  P10  The Trace Port Interface Unit (TPIU)
*********************************************************************************************************/
//bit[3]
#define TPIU_Disable          0x00                                     /* TPIU interface ��ֹ          */
#define TPIU_Enable           0x01                                     /* TPIU interface ʹ��          */
#define P10_3_FNUC            TPIU_Disable
/*********************************************************************************************************
** �����ڲ���������������
*********************************************************************************************************/
#define PIN_PULL_UP           0x00                                      /* �����ڲ�����                 */
#define PIN_REPEATER          0x01                                      /* �����ڲ�����                 */
#define PIN_NO_PULL           0x02                                      /* ���żȲ�������Ҳ������       */
#define PIN_PULL_DOWN         0x03                                      /* �����ڲ�����                 */
//P0
#define P0_0_MODE             PIN_PULL_UP
#define P0_1_MODE             PIN_PULL_UP
#define P0_2_MODE             PIN_PULL_UP
#define P0_3_MODE             PIN_PULL_UP
#define P0_4_MODE             PIN_PULL_UP
#define P0_5_MODE             PIN_PULL_UP
#define P0_6_MODE             PIN_PULL_UP
#define P0_7_MODE             PIN_PULL_UP
#define P0_8_MODE             PIN_PULL_UP
#define P0_9_MODE             PIN_PULL_UP
#define P0_10_MODE            PIN_PULL_UP
#define P0_11_MODE            PIN_PULL_UP
#define P0_12_MODE            PIN_PULL_UP
#define P0_13_MODE            PIN_PULL_UP
#define P0_14_MODE            PIN_PULL_UP
#define P0_15_MODE            PIN_PULL_UP
#define P0_16_MODE            PIN_PULL_UP
#define P0_17_MODE            PIN_PULL_UP
#define P0_18_MODE            PIN_PULL_UP
#define P0_19_MODE            PIN_PULL_UP
#define P0_20_MODE            PIN_PULL_UP
#define P0_21_MODE            PIN_PULL_UP
#define P0_22_MODE            PIN_PULL_UP
#define P0_23_MODE            PIN_PULL_UP
#define P0_24_MODE            PIN_PULL_UP
#define P0_25_MODE            PIN_PULL_UP
#define P0_26_MODE            PIN_PULL_UP
#define P0_27_MODE            PIN_PULL_UP
#define P0_28_MODE            PIN_PULL_UP
#define P0_29_MODE            PIN_PULL_UP
#define P0_30_MODE            PIN_PULL_UP
#define P0_31_MODE            PIN_PULL_UP
//P1
#define P1_0_MODE             PIN_PULL_UP
#define P1_1_MODE             PIN_PULL_UP
#define P1_2_MODE             PIN_PULL_UP
#define P1_3_MODE             PIN_PULL_UP
#define P1_4_MODE             PIN_PULL_UP
#define P1_5_MODE             PIN_PULL_UP
#define P1_6_MODE             PIN_PULL_UP
#define P1_7_MODE             PIN_PULL_UP 
#define P1_8_MODE             PIN_PULL_UP
#define P1_9_MODE             PIN_PULL_UP
#define P1_10_MODE            PIN_PULL_UP
#define P1_11_MODE            PIN_PULL_UP
#define P1_12_MODE            PIN_PULL_UP
#define P1_13_MODE            PIN_PULL_UP
#define P1_14_MODE            PIN_PULL_UP
#define P1_15_MODE            PIN_PULL_UP
#define P1_16_MODE            PIN_PULL_UP
#define P1_17_MODE            PIN_PULL_UP    
#define P1_18_MODE            PIN_PULL_UP
#define P1_19_MODE            PIN_PULL_UP
#define P1_20_MODE            PIN_PULL_UP
#define P1_21_MODE            PIN_PULL_UP
#define P1_22_MODE            PIN_PULL_UP
#define P1_23_MODE            PIN_PULL_UP
#define P1_24_MODE            PIN_PULL_UP
#define P1_25_MODE            PIN_PULL_UP
#define P1_26_MODE            PIN_PULL_UP
#define P1_27_MODE            PIN_PULL_UP     
#define P1_28_MODE            PIN_PULL_UP     
#define P1_29_MODE            PIN_PULL_UP     
#define P1_30_MODE            PIN_PULL_UP     
#define P1_31_MODE            PIN_PULL_UP     
// P2
#define P2_0_MODE             PIN_PULL_UP
#define P2_1_MODE             PIN_PULL_UP
#define P2_2_MODE             PIN_PULL_UP
#define P2_3_MODE             PIN_PULL_UP
#define P2_4_MODE             PIN_PULL_UP
#define P2_5_MODE             PIN_PULL_UP
#define P2_6_MODE             PIN_PULL_UP
#define P2_7_MODE             PIN_PULL_UP
#define P2_8_MODE             PIN_PULL_UP
#define P2_9_MODE             PIN_PULL_UP
#define P2_10_MODE            PIN_PULL_UP
#define P2_11_MODE            PIN_PULL_UP
#define P2_12_MODE            PIN_PULL_UP
#define P2_13_MODE            PIN_PULL_UP
#define P2_14_MODE            PIN_PULL_UP
#define P2_15_MODE            PIN_PULL_UP
//P3
#define P3_25_MODE            PIN_PULL_UP
#define P3_26_MODE            PIN_PULL_UP
//P4
#define P4_28_MODE            PIN_PULL_UP     
#define P4_29_MODE            PIN_PULL_UP     
		
/*********************************************************************************************************
** �������λ��PCONP�ж���
*********************************************************************************************************/
#define PCTIM0_Enable         1                                         /* ��ֹ��ʱ��/������0����/ʱ��  */
#define PCTIM0_Disable        0                                         /* ʹ�ܶ�ʱ��/������0����/ʱ��  */
#define PCTIM0_FUNC           PCTIM0_Enable

#define PCTIM1_Enable         1                                         /* ��ֹ��ʱ��/������1����/ʱ��  */
#define PCTIM1_Disable        0                                         /* ��ֹ��ʱ��/������1����/ʱ��	*/
#define PCTIM1_FUNC           PCTIM1_Enable

#define PCUART0_Enable        1                                         /* ʹ��UART0����/ʱ�ӿ���λ     */
#define PCUART0_Disable       0                                         /* ��ֹUART0����/ʱ�ӿ���λ     */
#define PCUART0_FUNC          PCUART0_Enable

#define PCUART1_Enable        1                                         /* ʹ��UART1����/ʱ�ӿ���λ	    */
#define PCUART1_Disable       0                                         /* ��ֹUART1����/ʱ�ӿ���λ     */
#define PCUART1_FUNC          PCUART1_Enable

#define PWM1_Enable           1                                         /* ʹ��PWM1����/ʱ�ӿ���λ      */
#define PWM1_Disable          0                                         /* ��ֹPWM1����/ʱ�ӿ���λ      */
#define PWM1_FUNC             PWM1_Enable

#define PCI2C0_Enable         1                                         /* ʹ��I2C0�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C0_Disable        0                                         /* ��ֹI2C0�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C0_FUNC           PWM1_Disable

#define PCSPI_Enable          1                                         /* ʹ��SPI�ӿڹ���/ʱ�ӿ���λ	*/
#define PCSPI_Disable         0                                         /* ��ֹSPI�ӿڹ���/ʱ�ӿ���λ   */
#define PCSPI_FUNC            PCSPI_Enable

#define PCRTC_Enable          1                                         /* ʹ��RTC����/ʱ�ӿ���λ       */
#define PCRTC_Disable         0                                         /* ��ֹRTC����/ʱ�ӿ���λ       */
#define PCRTC_FUNC            PCRTC_Disable								

#define PCSSP1_Enable         1                                         /* ʹ��SSP1�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP1_Disable        0                                         /* ��ֹSSP1�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP1_FUNC           PCSSP1_Disable

#define PCAD_Enable           1                                         /* ʹ��A/Dת��������/ʱ�ӿ���λ */
#define PCAD_Disable          0                                         /* ��ֹA/Dת��������/ʱ�ӿ���λ */
#define PCAD_FUNC             PCAD_Disable

#define PCCAN1_Enable         1                                         /* ʹ��CAN������1����/ʱ�ӿ���λ*/
#define PCCAN1_Disable        0                                         /* ��ֹCAN������1����/ʱ�ӿ���λ*/
#define PCCAN1_FUNC           PCCAN1_Disable

#define PCCAN2_Enable         1                                         /* ʹ��CAN������2����/ʱ�ӿ���λ*/
#define PCCAN2_Disable        0                                         /* ��ֹCAN������2����/ʱ�ӿ���λ*/
#define PCCAN2_FUNC           PCCAN2_Disable

#define PCGPIO_Enable         1                                         /* ʹ��GPIO	                    */
#define PCGPIO_Disable        0                                         /* ��ֹGPIO                 	*/
#define PCGPIO_FUNC           PCGPIO_Enable

#define PCRIT_Enable          1                                         /* ʹ���ظ��ж϶�ʱ��		    */
                                                                        /* ����/ʱ�ӿ���λ              */
#define PCRIT_Disable         0                                         /* ��ֹ�ظ��ж϶�ʱ��           */
                                                                        /* ����/ʱ�ӿ���λ	            */
#define PCRIT_FUNC            PCRIT_Disable

#define PCMC_Enable           1                                         /* ʹ�ܵ������PWM              */
#define PCMC_Disable          0                                         /* ��ֹ�������PWM              */
#define PCMC_FUNC             PCMC_Disable

#define PCQEI_Enable          1                                         /* ʹ�������������ӿ�           */
                                                                        /* ����/ʱ�ӿ���λ              */
#define PCQEI_Disable         0                                         /* ��ֹ�����������ӿ�           */
                                                                        /* ����/ʱ�ӿ���λ              */
#define PCQEI_FUNC            PCQEI_Disable

#define PCI2C1_Enable         1                                         /* ʹ��I2C1�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C1_Disable        0                                         /* ��ֹI2C1�ӿڹ���/ʱ�ӿ���λ  */
#define PCI2C1_FUNC           PCI2C1_Enable

#define PCSSP0_Enable         1                                         /* ʹ��SSP0�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP0_Disable        0                                         /* ��ֹSSP0�ӿڹ���/ʱ�ӿ���λ  */
#define PCSSP0_FUNC           PCSSP0_Disable

#define PCTIM2_Enable         1                                         /* ʹ�ܶ�ʱ��2����/ʱ�ӿ���λ   */
#define PCTIM2_Disable        0                                         /* ��ֹ��ʱ��2����/ʱ�ӿ���λ	*/
#define PCTIM2_FUNC           PCTIM2_Disable

#define PCTIM3_Enable         1                                         /* ʹ�ܶ�ʱ��3����/ʱ�ӿ���λ   */
#define PCTIM3_Disable        0                                         /* ��ֹ��ʱ��3����/ʱ�ӿ���λ   */
#define PCTIM3_FUNC           PCTIM2_Enable

#define PCUART2_Enable        1                                         /* ʹ��UART2����/ʱ�ӿ���λ	    */
#define PCUART2_Disable       0                                         /* ��ֹUART2����/ʱ�ӿ���λ	    */
#define PCUART2_FUNC          PCUART2_Enable

#define PCUART3_Enable        1                                         /* ʹ��UART3����/ʱ�ӿ���λ	    */
#define PCUART3_Disable       0                                         /* ��ֹUART3����/ʱ�ӿ���λ	    */
#define PCUART3_FUNC          PCUART3_Enable

#define PCI2C2_Enable         1                                         /* ʹ��I2C�ӿ�2����/ʱ�ӿ���λ  */
#define PCI2C2_Disable        0                                         /* ��ֹI2C�ӿ�2����/ʱ�ӿ���λ	*/
#define PCI2C2_FUNC           PCI2C2_Enable

#define PCI2S_Enable          1                                         /* ʹ��I2S�ӿڹ���/ʱ�ӿ���λ   */
#define PCI2S_Disable         0                                         /* ��ֹI2S�ӿڹ���/ʱ�ӿ���λ   */
#define PCI2S_FUNC            PCI2S_Disable

#define PCGPDMA_Enable        1                                         /* ʹ��GP DMA���ܹ���/ʱ�ӿ���λ*/
#define PCGPDMA_Disable       0                                         /* ��ֹGP DMA���ܹ���/ʱ�ӿ���λ*/
#define PCGPDMA_FUNC          PCGPDMA_Disable

#define PCENET_Enable         1                                         /* ʹ����̫��ģ�鹦��/ʱ�ӿ���λ*/
#define PCENET_Disable        0                                         /* ��ֹ��̫��ģ�鹦��/ʱ�ӿ���λ*/
#define PCENET_FUNC           PCENET_Disable

#define PCUSB_Enable          1                                         /* ʹ��USB�ӿڹ���/ʱ�ӿ���λ   */
#define PCUSB_Disable         0                                         /* ��ֹUSB�ӿڹ���/ʱ�ӿ���λ	*/
#define PCUSB_FUNC            PCUSB_Disable

/*********************************************************************************************************
** Function name:     void pinInit
** Descriptions:      ��ʼ�����е��������ã�����������ӡ���������������
**                    ע�⣺�������� LPC1700PinCfg.h ͷ�ļ���ѡ�����Ӧ�ܽŵĹ���
** Input parameters:  ��
** Output parameters: ��
** Returned value:    ��
*********************************************************************************************************/
extern void pinInit(void);
/*********************************************************************************************************
** Function Name:     pconpInit
** Descriptions:      ���蹦�ܳ�ʼ����1 ��ʾ��Ӧ����ʹ��;0 ��ʾ��Ӧ�����ֹ
** Input parameters:  ��
** Output parameters: ��
** Returned value:    ��              
*********************************************************************************************************/
extern void pconpInit(void);
#endif           
/**************************************End Of File*******************************************************/
