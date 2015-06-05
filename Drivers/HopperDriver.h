#ifndef _HOPPERACCEPTER_H
#define _HOPPERACCEPTER_H


//������ַHopper�ĵ�λ���洢��ַ
#define ADD_HPVALUE1			(1)
#define ADD_HPVALUE2			(2)
#define ADD_HPVALUE3			(3)
//��ַΪ00��Hopper����
#define EVBDEV_HP1						(1)					
//��ַΪ01��Hopper����			
#define EVBDEV_HP2						(2)					
//��ַΪ10��Hopper����
#define EVBDEV_HP3						(3)					

//Hopper�Ĳ�ѯ����ָ��
#define EVBHANDLE_HPQUERY				(0x51)
//Hopper�ĳ��Ҳ���ָ��				
#define EVBHANDLE_HPOUTPUT				(0x50)				

struct EvbHopper
{
	//Ӳ�������ϵ��������
	unsigned char Index;
	//Hp�����б��еı��
	unsigned char Num;
	//�ϴ�ͨ�ź�õ���״̬:0,����;2,ȱ��;3,����;
	//���ص���Ϣ��0x00:����;0x20:ȱ��;0x11:����ʱ�����ˣ�0x39:����ޱ�ʱ�����˱ң�0x41:����б�ʱ���ҳ����˱�
	unsigned char State;
	//��ֵ
	unsigned short Price;
	//����̶���ַ
	unsigned char	Add; 
	//�ϴβ�����������
	unsigned char OutPutCount;
	//Hp���ϲ����ñ�־
	unsigned char DevBadFlag;
	unsigned int MemoryAdd;
};
//����������ַHopper�Ľṹ��
extern struct EvbHopper stEvbHp[3];
//Hopper���㵥λ��ֵ��С���ñ�־��1������Hopper����ֵ����ȣ�2��000��ַHopper��001��ַHopper��ַ��ֵ��ȣ���010�����
//							  	  3��000��ַHopper��010��ַHopper��ַ��ֵ��ȣ���001�����
//							  	  4��001��ַHopper��010��ַHopper��ַ��ֵ��ȣ���000�����	
//							  	  5������Hopper����ֵ���
extern unsigned char HpValueFlag;


/************************************************�����ӿ�*****************************************************/

void HopperAccepter_CheckHPOk(void);

unsigned char HopperAccepter_Handle(unsigned char DeviceCmdType,unsigned char Address,unsigned char *nbuf);

unsigned char HopperAccepter_HpOutHandle(unsigned char Count,unsigned char HpNum,unsigned char *HpOutNum);

void HopperAccepter_GetState(unsigned char DevType);


#endif
