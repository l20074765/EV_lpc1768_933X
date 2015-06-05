#ifndef _HOPPERACCEPTER_H
#define _HOPPERACCEPTER_H


//三个地址Hopper的单位金额存储地址
#define ADD_HPVALUE1			(1)
#define ADD_HPVALUE2			(2)
#define ADD_HPVALUE3			(3)
//地址为00的Hopper类型
#define EVBDEV_HP1						(1)					
//地址为01的Hopper类型			
#define EVBDEV_HP2						(2)					
//地址为10的Hopper类型
#define EVBDEV_HP3						(3)					

//Hopper的查询操作指令
#define EVBHANDLE_HPQUERY				(0x51)
//Hopper的出币操作指令				
#define EVBHANDLE_HPOUTPUT				(0x50)				

struct EvbHopper
{
	//硬件连接上得索引编号
	unsigned char Index;
	//Hp排序列表中的编号
	unsigned char Num;
	//上次通信后得到的状态:0,可用;2,缺币;3,故障;
	//返回的信息：0x00:正常;0x20:缺币;0x11:出币时卡币了；0x39:光侧无币时出不了币；0x41:光侧有币时出币出不了币
	unsigned char State;
	//面值
	unsigned short Price;
	//物理固定地址
	unsigned char	Add; 
	//上次操作出币数量
	unsigned char OutPutCount;
	//Hp故障不可用标志
	unsigned char DevBadFlag;
	unsigned int MemoryAdd;
};
//定义三个地址Hopper的结构体
extern struct EvbHopper stEvbHp[3];
//Hopper找零单位面值大小设置标志：1：三个Hopper的面值不相等；2：000地址Hopper与001地址Hopper地址面值相等，与010不相等
//							  	  3：000地址Hopper与010地址Hopper地址面值相等，与001不相等
//							  	  4：001地址Hopper与010地址Hopper地址面值相等，与000不相等	
//							  	  5：三个Hopper的面值相等
extern unsigned char HpValueFlag;


/************************************************函数接口*****************************************************/

void HopperAccepter_CheckHPOk(void);

unsigned char HopperAccepter_Handle(unsigned char DeviceCmdType,unsigned char Address,unsigned char *nbuf);

unsigned char HopperAccepter_HpOutHandle(unsigned char Count,unsigned char HpNum,unsigned char *HpOutNum);

void HopperAccepter_GetState(unsigned char DevType);


#endif
