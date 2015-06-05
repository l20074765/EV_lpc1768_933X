#ifndef _SAILCONTROL_H
#define _SAILCONTROL_H

extern unsigned char IsCmdRecv(unsigned char *CMDType,unsigned char *SubCmd);

extern unsigned char ReturnAckPack(unsigned char CMDType,unsigned char databuf1,unsigned char databuf2,unsigned char databuf3);
extern unsigned char HopperDispence(unsigned int DispenceMoney,unsigned int *iouMoney);
uint8_t hopperIsEmpty(void);
uint8_t checkDeviceState(void);

#endif










