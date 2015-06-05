#ifndef _GLOBAL_H
#define _GLOBAL_H


#define VMC_TRACE_ON					1
#define VMC_TRACE_OFF					0
#define VMC_TRACE_FLAG					VMC_TRACE_ON //调试开关
#define VMC_OPEN_COINFG					0 //是否开启配置文件1开启  0关闭


//兑零机配置
//硬币器串行脉冲
#define  VMC_COIN_SERIAL	1
#define  VMC_COIN_PARALLEL  2
#define  VMC_COIN_NO		0//关硬币器
#define  VMC_COIN_TYPE     VMC_COIN_SERIAL


//找零器通道面值
#define VMC_COIN_CHANNEL_1	100
#define VMC_COIN_CHANNEL_2	0
#define VMC_COIN_CHANNEL_3	0
#define VMC_COIN_CHANNEL_4	0
#define VMC_COIN_CHANNEL_5	0
#define VMC_COIN_CHANNEL_6	0
#define VMC_COIN_CHANNEL_7	0
#define VMC_COIN_CHANNEL_8	0

//纸币器MDB

#define VMC_BILL_NO			0	//关纸币器
#define VMC_BILL_RS232		1 	//GBA
#define VMC_BILL_MDB		2	//MDB
#define VMC_BILL_MDB_ITL	3	// MDB ITL

#define VMC_BILL_TYPE     VMC_BILL_MDB

//找零器

#define VMC_CHANGER_HOPPER		1
#define VMC_CHANGER_NO			0//关找零器
#define VMC_CHANGER_TYPE        VMC_CHANGER_HOPPER

//定义hopper找零 比例
#define HOPPER_LEVEL_RATIO			 {2,1,0} //与hopper数要对应按照hopper面值斗 从大到小 
#define HOPPER_BASE_CHANGED			  500   //找零基数以分为单位

//找零器通道面值
#define VMC_CHANGER_CHANNEL_1	100
#define VMC_CHANGER_CHANNEL_2	200
#define VMC_CHANGER_CHANNEL_3	0




//小数点位置
#define VMC_PIONT_0					0
#define VMC_PIONT_1					1
#define VMC_PIONT_2					2


#define VMC_PIONT_TYPE      VMC_PIONT_1

//定义货道配置文件
#define VMC_HUODAO_OPEN_CONFIG     1 //是否开启货道文件配置
#define VMC_HUODAO_LEVEL_OPEN     {1,0} //定义层开关

//定义货道开关
#define VMC_HUODAO_COLUMN_OPEN    {{1,0,0,1,1,1,0,0},   \
								   {0,0,0,0,0,0,0,0} }



//======================机型配置结束=======================================================





//是否开启Hopper找零功能
#define HOPPER_OPEN						(3)
//是否开启纸币器收币功能
#define BILL_OPEN						(4)

extern uint8_t GoToMenuFlag;
extern uint32_t nMinDispMoney;
extern unsigned char debatflag;

extern unsigned int MaxColumnMoney;

extern OS_EVENT *g_KeyMsg;









#endif
