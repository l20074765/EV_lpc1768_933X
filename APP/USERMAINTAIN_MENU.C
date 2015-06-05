/****************************************Copyright (c)*************************************************
**                      Fujian Junpeng Communicaiton Technology Co.,Ltd.
**                               http://www.easivend.com.cn
**--------------File Info------------------------------------------------------------------------------
** File name:           USERMAINTAIN_MENU.C
** Last modified Date:  2013-03-04
** Last Version:         
** Descriptions:        用户模式主菜单                  
**------------------------------------------------------------------------------------------------------
** Created by:          gzz 
** Created date:        2013-03-04
** Version:              
** Descriptions:        The original version       
********************************************************************************************************/


#define MAXLANGUAGETYPE 5 //系统支持最大的语言种类，目前暂定5种，中文，英文，土耳其，俄罗斯，葡萄牙



/**************************************密码输入菜单****************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//密码模式提示
	char *EnterPin[MAXLANGUAGETYPE];				//提示输入密码
	char *PinError[MAXLANGUAGETYPE];				//密码错误提示
	char *ReturnCapion[MAXLANGUAGETYPE];			//返回提示
} PASSWORDNMENULIST;

const PASSWORDNMENULIST PasswordMenuList = 
{
	{
		"输入密码",
		"Input password",
		"Input password",
		"SIFRE GIR",
		"SENHA"
	},
	{
		"请输入密码：",
		"Pls input password:",
		"Pls input password:",
		"LUTFEN SIFRE GIRIN:",
		"FAVOR INSERIR A SENHA:"
	},
	{
		"密码错误！！",
		"Password Error!",
		"Password Error!",
		"Password Error!",
		"Password Error!"
	},
	{
		"请按“取消”返回",
		"Press“Cancel” to return",
		"Press“Cancel” to return",
		"IPTAL E BAS GERI DON",
		"PRESSIONE CANCELAR"
	}
};

/**************************************确认取消页面****************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//确认取消页面提示
	char *SureCapion[MAXLANGUAGETYPE];				//确认取消页面提示
	char *OKPin[MAXLANGUAGETYPE];					//确认按键
	char *CancelPin[MAXLANGUAGETYPE];				//取消按键	
	char *setOK[MAXLANGUAGETYPE];//设置成功
	char *setFail[MAXLANGUAGETYPE];//设置失败
} OKNMENULIST;

const OKNMENULIST OKMenuList = 
{
	{
		"请选择",
		"Select",
		"Select",
		"Select",
		"SELECIONAR"
	},
	{
		"确认吗?",
		"Are you sure?",
		"Are you sure?",
		"EMIN MISINIZ?",
		"VOC\x85 TEM CERTEZA?"
	},
	{
		"确认",
		"Enter",
		"Enter",
		"GIRIS",
		"PRESSIONE"
	},
	{
		"取消",
		"Cancel",
		"Cancel",
		"IPTAL",
		"CANCELAR"
	}
		,
	{
		"设置成功",
		"Cancel",
		"Cancel",
		"IPTAL",
		"CANCELAR"
	}
		,
	{
		"设置失败",
		"Cancel",
		"Cancel",
		"IPTAL",
		"CANCELAR"
	}
		
};


/*************************************用户模式维护主菜单按键选择************************************************/
typedef struct
{
	unsigned char KeyCurrent;						//当前菜单索引号
	unsigned char ExtSelfCheck;						//退出设备自检菜单并返回维护主菜单
	unsigned char PageNumb;							//菜单页索引
	void (*TestOperate)();							//当前状态应该执行的功能操作
} USERMAINTAINMAINMENU;	//维护主菜单


USERMAINTAINMAINMENU UserMaintainMainMenu;									//维护主菜单结构体

/**************************************用户模式维护主菜单****************************************************/


typedef struct _user_maintain_mainmenu_list_{

	char *ModeCapion[MAXLANGUAGETYPE];				//自检模式提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *Record[MAXLANGUAGETYPE];		
	char *Error[MAXLANGUAGETYPE];
	char *Column[MAXLANGUAGETYPE];	
	char *Device[MAXLANGUAGETYPE];
	
}USERMAINTAINMAINMENULIST;

const USERMAINTAINMAINMENULIST UserMaintainMenuList = 
{
	{
		"维护菜单",
		"MAINTAIN MODE",
		"MAINTAIN MODE",
		"MAINTAIN MODE",
		"MAINTAIN MODE"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"① 交易记录",
		"1.Sales record",
		"1.Запись запроса",
		"1.SATIS KAYITLARI",
		"1.VENDAS"
	},
	{
		"② 故障查询",
		"2.Error inqiry",	
		"2.Код ошибки",
		"2.HATALI GIRIS",
		"2.ERRO CONSULTADO"
	},
	
	{
		"③ 设备管理",
		"3.Debug mode",
		"3.Trace",
		"3.HATA KONTROL",
		"3.MODO DE AJUSTE"
	},
	{
		"④ 货道管理",
		"4.Column",	
		"4.Колонка",
		"4.SUTUN",
		"4.BANDEJA"
	}
		
};







/**************************************错误查询菜单****************************************************/
typedef struct
{
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	//纸币器故障提示	
	char *BillCapion[MAXLANGUAGETYPE];				
	char *BillCommunicate[MAXLANGUAGETYPE];	//1代表纸币器通讯不上	
	char *Billmoto[MAXLANGUAGETYPE];		//1代表马达故障
	char *Billsensor[MAXLANGUAGETYPE];		//1代表传感器故障
	char *Billromchk[MAXLANGUAGETYPE];		//1代表rom出错
	char *Billjam[MAXLANGUAGETYPE];			//1代表投币卡币
	char *BillremoveCash[MAXLANGUAGETYPE];	//1移除纸币钞箱
	char *BillcashErr[MAXLANGUAGETYPE];		//1纸币钞箱故障
	char *Billdisable[MAXLANGUAGETYPE];		//1纸币因为各种原因被禁能了
	char *BillCoinEmp[MAXLANGUAGETYPE];		//1硬币找零不足
	//硬币器故障提示
	char *CoinCapion[MAXLANGUAGETYPE];					
	char *CoinCommunicate[MAXLANGUAGETYPE];	//1代表硬币器通讯不上	
	char *Coinsensor[MAXLANGUAGETYPE];		//1代表传感器故障
	char *Cointubejam[MAXLANGUAGETYPE];		//1代表出币口卡币
	char *Coinromchk[MAXLANGUAGETYPE];		//1代表rom出错
	char *Coinrouting[MAXLANGUAGETYPE];		//1代表进币通道出错
	char *Coinjam[MAXLANGUAGETYPE];			//1代表投币卡币
	char *CoinremoveTube[MAXLANGUAGETYPE];	//1移除硬币斗
	char *Coindisable[MAXLANGUAGETYPE];		//1硬币因为各种原因被vmc禁能了
	char *CoinunknowError[MAXLANGUAGETYPE];	//1硬币其他各种故障
	//找零器故障提示
	char *HopperCapion[MAXLANGUAGETYPE];					
	char *Hopper1State[MAXLANGUAGETYPE];	//Hopper1状态，0正常，1缺币，2故障	
	char *Hopper2State[MAXLANGUAGETYPE];	//Hopper2状态，0正常，1缺币，2故障
	char *Hopper3State[MAXLANGUAGETYPE];	//Hopper3状态，0正常，1缺币，2故障
	//货道故障提示
	char *GOCCapion[MAXLANGUAGETYPE];
	char *GOCError[MAXLANGUAGETYPE];	    //主箱体GOC状态，0正常，1故障
	char *GOCErrorSub[MAXLANGUAGETYPE];	    //从箱体GOC状态，0正常，1故障
	char *ColBoardError[MAXLANGUAGETYPE];	//主箱体货道板状态，0正常，1故障
	char *ColBoardErrorSub[MAXLANGUAGETYPE];//从箱体货道板状态，0正常，1故障
	//升降机故障提示
	char *Error_FMD[MAXLANGUAGETYPE];	//取货电机故障
	char *Error_FUM[MAXLANGUAGETYPE];	//上下电机故障
	char *Error_GOC[MAXLANGUAGETYPE];	    //出货确认对射板故障
	char *Error_FLD[MAXLANGUAGETYPE];		//层架检测传感器故障
	char *Error_FOD[MAXLANGUAGETYPE];		//起始位置传感器故障

	char *Error_UDS[MAXLANGUAGETYPE];		//用户取货门故障
	char *Error_GCD[MAXLANGUAGETYPE];	    //货道驱动板故障
	char *Error_SOVB[MAXLANGUAGETYPE];		//出货机构忙
	char *Error_SOMD1[MAXLANGUAGETYPE];		//机器大门状态
	char *Error_SOEC[MAXLANGUAGETYPE];			//收到错误数据包

	char *Error_SFHG[MAXLANGUAGETYPE];		//升降台取货
	//char *ElevatStart[MAXLANGUAGETYPE];		//升降机是否在原点
	char * Error_SOFP[MAXLANGUAGETYPE];	//货柜门是否打开取货
	//char *MotorBusy[MAXLANGUAGETYPE];		//电机忙
	char *Error_SOMD2[MAXLANGUAGETYPE];	 //取货门没关
	char *Emp_Gol[MAXLANGUAGETYPE];	 //货道全部不可用
	//设备正常
	char *DeviceOK[MAXLANGUAGETYPE];	//设备正常


	//add by yoc 2014.02.19
	char *allColumnEmpty[MAXLANGUAGETYPE];//全货道无货
	char *vmcFaultList[MAXLANGUAGETYPE];//售货机故障列表
	char *hopperFault[MAXLANGUAGETYPE];//hopper故障
	char *billFault[MAXLANGUAGETYPE];
	
} USERMAINTAINERRORMENULIST;

const USERMAINTAINERRORMENULIST UserMaintainErrorMenuList = 
{		
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"纸币器故障列表",
		"Bill acceptor fault list",
		"Приемнк купюр",
		"KAGIT PARA HATASI",
		"LISTA ERRADA DE NOTAS"
	},
	{
		"① 纸币器通讯不上",
		"1.Comminication failure",
		"1.Запись запроса",
		"1.BAGLANTI HATASI",
		"1.FALHA NA COMUNICA\x83\x82\O"
	},
	{
		"② 马达故障",
		"2.Motor error",	
		"2.Код ошибки",
		"2.MOTOR HATASI",
		"2.ERRO MOTOR"
	},
	{
		"③ 传感器故障",
		"3.Sensor error",
		"3.Система",
		"3.SENSOR HATASI",
		"3.ERRO SENSOR"
	},
	{
		"④ rom出错",
		"4.Rom error",	
		"4.Колонка",
		"4.ROM HATASI",
		"4.ERRO ROM"
	},
	{
		"⑤ 投币卡币",
		"5.Bill jammed",
		"5.Добавить полную еду",
		"5.KAGIT PARA SIKISMASI",
		"5.NOTA AMASSADA"
	},
	{
		"⑥ 移除纸币钞箱",
		"6.Stacker unlocked",
		"6.Система",
		"6.KASA KILITLENMEMIS",
		"6.EMPILHADOR DESBLOQUEADO"
	},
	{
		"⑦ 纸币钞箱故障",
		"7.Stacker error",	
		"7.Колонка",
		"7.KASA HATASI",
		"7.ERRO EMPILHADOR"
	},
	{
		"⑧ 纸币因为各种原因被禁能",
		"8.Bill acceptor disabled",
		"8.Система",
		"8.KAGIT PARA AKTIF DEGIL",
		"8.RECEPTOR NOTA DESATIVADO"
	},
	{
		"⑨ 硬币不足",
		"9. Coin empty",
		"9.Coin empty",
		"9.Coin empty",
		"9.Coin empty"
	},
	{
		"硬币器故障列表",
		"Coin acceptor Error list",
		"Приемник монет",
		"BOZUK PARA HATASI",
		"RECEPTOR MOEDA DESATIVADO"
	},
	{
		"① 硬币器通讯不上",
		"1.Communication failure",
		"1.Запись запроса",
		"1.BAGLANTI HATASI",
		"1.FALHA COMUNICA\x83\x82\O"
	},
	{
		"② 传感器故障",
		"2.Sensor error",	
		"2.Код ошибки",
		"2.SENSOR HATASI",
		"2.ERRO SENSOR"
	},
	{
		"③ 出币口卡币",
		"3.Coin exit jammed",
		"3.Система",
		"3.PARA CIKISI SIKISMIS",
		"3.MOEDA PRESA"
	},
	{
		"④ rom出错",
		"4.rom error",	
		"4.Колонка",
		"4.ROM HATASI",
		"4.ERRO ROM"
	},
	{
		"⑤ 进币通道出错",
		"5.coin channel error",
		"5.Добавить полную еду",
		"5.PARA KANALI HATASI",
		"5.ERRO CANAL DA MOEDA"
	},
	{
		"⑥ 投币卡币",
		"6.Coin entrance jammed",
		"6.Система",
		"6.PARA GIRISI SIKISMIS",
		"6.ENTRADA MOEDA EMPERRADA"
	},
	{
		"⑦ 移除硬币斗",
		"7.Coin holder removed",	
		"7.Колонка",
		"7.PARA TUTUCU SIKISMIS",
		"7.REMO\x83\x82\O DE MOEDA"
	},
	{
		"⑧ 硬币器被vmc禁能",
		"8.Disabled by vmc",
		"8.Система",
		"8.PARA MEK IPTAL",
		"8.DESATIVADO POR VMC"
	},
	{
		"⑨ 硬币器其他故障",
		"9. Other errors",
		"9.Система",
		"9.DIGER HATALAR",
		"9.OUTROS ERROS"
	},
	{
		"找零器故障列表",
		"Changer Error list",
		"Изменение",
		"BOZUK PARA M HATA LISTESI",
		"ERRO NO TROCADOR"
	},
	{
		"① Hopper1:",
		"1.Hopper1",
		"1.Hopper1",
		"1.Hopper1",
		"1.HOPPER 1"
	},
	{
		"② Hopper2:",
		"2.Hopper2",	
		"2.Hopper2",
		"2.Hopper2",
		"2.HOPPER 2"
	},
	{
		"③ Hopper3:",
		"3.Hopper3",
		"3.Hopper3",
		"3.Hopper3",
		"3.HOPPER 3"
	},
	{
		"货道故障列表",
		"Column Error list",
		"GOC",
		"SUTUN HATALARI",
		"ERRO NA LISTA DA BANDEJA"
	},
	{
		"① 主箱体出货检测故障",
		"1.Main cabinet GOC error",
		"1.A GOC Error",
		"1.ANA KABIN GOC HATASI",
		"1.ERRO GOC NO ARM\x81\RIO CENTRAL"
	},
	{
		"② 从箱体出货检测故障",
		"2.Sub-cabinet GOC error",
		"2.B GOC Error",
		"2.KOLE KABIN GOC HATASI",
		"2.ERRO GOC NO SUB ARM\x81\RIO"
	},
	{
		"③ 主箱体货道板故障",
		"3.Main cabinet dispensing board error",
		"3.A Board Error",
		"3.ANA KABIN KART HATA",
		"3.ERRO NO ARMARIO CENTRAL"
	},
	{
		"④ 从箱体货道板故障",
		"4.Sub-cabinet dispensing board error",
		"4.B Board Error",
		"4.KOLE KABIN KART HATA",
		"4.ERRO NO DISPENSER DO SUB CENTRAL"
	},
	{
		"① 取货电机故障",
		"1.Collect motor error",
		"1.Запись запроса",
		"1.Kayxf1x31t sorgulama",
		"1.COLETAR ERRO MOTOR"
	},
	{
		"② 上下电机故障",
		"2.Elevator motor error",	
		"2.Код ошибки",
		"2.Hata kodu",
		"2.MOTOR DO ELEVADOR COM ERRO"
	},
	{
		"③ 出货确认板故障",
		"3.GOC board error",
		"3.Система",
		"3.Sistem parametreleri",
		"3.ERRO DE PLACA GOC"
	},
	{
		"④ 层架检测传感器故障",
		"4.Tray sensor error",	
		"4.Колонка",
		"4.Sutun parametre",
		"4.SENSOR DA BANDEIJA C/ ERRO"
	},
	{
		"⑤ 开始位置传感器故障",
		"5.Start sensor error",
		"5.Добавить полную еду",
		"5.xf0xdcrxf0xfcn girin",
		"5.SENSOR DE INICIALIZAR C/ ERRO"
	},
	{
		"① 用户取货门故障",
		"1.Collect door error",
		"1.Запись запроса",
		"1.Kayxf1x31t sorgulama",
		"1.COLETOR DE PORTA COM ERRO"
	},
	{
		"② 货道驱动板故障",
		"2.Dispensing board error",	
		"2.Код ошибки",
		"2.Hata kodu",
		"2.ERRO DA PLACA DISTRIBUICAO"
	},
	{
		"③ 出货机构忙",
		"3.Dispensing mech busy",
		"3.Система",
		"3.Sistem parametreleri",
		"3.APLICADORA MECH OCUPADO"
	},
	{
		"④ 取货门没关",
		"4.Machine door status",	
		"4.Колонка",
		"4.Sutun parametre",
		"4.SITUACAO DAS PORTAS MAQUINA"
	},
	{
		"⑤ 收到错误数据包",
		"5.Incorrect data received",
		"5.Добавить полную еду",
		"5.xf0xdcrxf0xfcn girin",
		"5.DADOS INCORRETOS RECEBIDOS"
	},
	{
		"① 升降台取货",
		"1.Elevator collecting",
		"1.Запись запроса",
		"1.Kayxf1x31t sorgulama",
		"1.ELEVADOR COLETA"
	},	
	{
		"② 货柜门是否打开取货",
		"2.Collect door open",
		"2.Система",
		"2.Sistem parametreleri",
		"2.PORTA ABERTA"
	},	
	{
		"③ 货柜大门没关",
		"3.Collect door not closed ",
		"3.Добавить полную еду",
		"3.xf0xdcrxf0xfcn girin",
		"3.COLETOR DE PORTA ABERTO"
	},
	{
		"全货道故障",
		"Column Empty",
		"Column Empty",
		"xf0xdcrxf0xfcn girin",
		"COLETOR DE PORTA ABERTO"
	},
	{
		"设备正常",
		"OK",
		"Хорошо",
		"OK",
		"OK"
	},
	{
		"全货道无货",
		"Column Empty",
		"Column Empty",
		"xf0xdcrxf0xfcn girin",
		"COLETOR DE PORTA ABERTO"
	}
	,
	{
		"设备故障列表",
		"Column Empty",
		"Column Empty",
		"xf0xdcrxf0xfcn girin",
		"COLETOR DE PORTA ABERTO"
	},
	{
		"Hopper: 故障",
		"Hopper",
		"Hopper",
		"Hopper",
		"HOPPER"
	}
		,
	{
		"纸币器: 故障",
		"Bill",
		"Bill",
		"Bill",
		"Bill"
	}
		
		
};


/*************************************用户模式维护主菜单按键选择************************************************/
typedef struct
{
	unsigned char KeyCurrent;						//当前菜单索引号
	unsigned char ExtSelfCheck;						//退出设备自检菜单并返回维护主菜单
	unsigned char PageNumb;							//菜单页索引
	void (*TestOperate)();							//当前状态应该执行的功能操作
} LOGMAINTAINMAINMENU;											//维护主菜单
LOGMAINTAINMAINMENU LogMaintainMainMenu;									//维护主菜单结构体


/*************************************交易记录菜单选择************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//交易记录提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *Record[MAXLANGUAGETYPE];		
	char *ColumnRecord[MAXLANGUAGETYPE];
	char *ClearRecord[MAXLANGUAGETYPE];
	char *RecordTotal[MAXLANGUAGETYPE];
	char *RecordPrint[MAXLANGUAGETYPE];
	char *RecordIng[MAXLANGUAGETYPE];
	char *CancelPin[MAXLANGUAGETYPE];
	char *printTrade[MAXLANGUAGETYPE];
} LOGMAINTAINMAINMENULIST;

const LOGMAINTAINMAINMENULIST LogMaintainMenuList = 
{
	{
		"交易记录",
		"Transaction record",
		"Transaction record",
		"SATIS BILGILERI",
		"VENDAS"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"① 交易记录",
		"1.Sales record",
		"1.Платежи",
		"1.SATIS RAPORU",
		"1.VENDAS"
	},
	{
		"② 货道统计",
		"2.Column record",	
		"2.Столбец транзакций:",
		"2.SUTUN RAKAMLARI",
		"2.BANDEJA"
	},
	{
		"③ 清除交易记录",
		"3.Clear record",
		"3.Очистить запись",
		"3.KAYITLARI SIL",
		"3.REGISTRO APAGADO"
	},
	{
		"④ 总交易记录",
		"4.Total sales record",	
		"4.квартал Платежи",
		"4.TOPLAM SATIS TUTARI",
		"4.VENDA TOTAL"	
	},
	{
		"⑤ 交易记录明细",
		"5.Sales detail",	
		"5.Колонка",
		"5.SATIS DETAYLARI",
		"5.DETALHE DAS VENDAS"
	},
	{
		"正在打印,请稍等...",
		"Under printing, pls wait...",	
		"Print,Please Wait...",
		"Print,Please Wait...",
		"IMPRIMINDO, AGUARDE..."
	},
	{
		"返回",
		"Under printing, pls wait...",	
		"Print,Please Wait...",
		"Print,Please Wait...",
		"IMPRIMINDO, AGUARDE..."
	},
	{
	"打印交易记录",
	"Under printing, pls wait...",	
	"Print,Please Wait...",
	"Print,Please Wait...",
	"IMPRIMINDO, AGUARDE..."
	}
};


/*************************************区域交易记录,总交易记录菜单选择************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//交易记录提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *Income[MAXLANGUAGETYPE];	//总收币	
	char *NoteIncome[MAXLANGUAGETYPE];
	char *CoinsIncome[MAXLANGUAGETYPE];
	char *TotalTrans[MAXLANGUAGETYPE];
	char *TotalChange[MAXLANGUAGETYPE];
	//char *SuccessNum[MAXLANGUAGETYPE];
	char *DoubtNum[MAXLANGUAGETYPE];
	char *Iou[MAXLANGUAGETYPE];
	//char *Card[MAXLANGUAGETYPE];

	
	//add by yoc 2013.9.11
	/*
	char *batch_num[MAXLANGUAGETYPE];
	char *batch_money[MAXLANGUAGETYPE];
	char *batch_cash_num[MAXLANGUAGETYPE];
	char *batch_cash_money[MAXLANGUAGETYPE];
	char *batch_game_num[MAXLANGUAGETYPE];
	char *batch_game_money[MAXLANGUAGETYPE];
	char *batch_onecard_num[MAXLANGUAGETYPE];
	char *batch_onecard_money[MAXLANGUAGETYPE];
	char *batch_unioncard_num[MAXLANGUAGETYPE];
	char *batch_unioncard_money[MAXLANGUAGETYPE];
	char *batch_pc_num[MAXLANGUAGETYPE];
	char *batch_pc_money[MAXLANGUAGETYPE];
	*/
	char *total_num[MAXLANGUAGETYPE];
	char *total_money[MAXLANGUAGETYPE];
	char *total_cash_num[MAXLANGUAGETYPE];
	char *total_cash_money[MAXLANGUAGETYPE];
	char *total_card_num[MAXLANGUAGETYPE];
	char *total_card_money[MAXLANGUAGETYPE];
	char *total_pc_num[MAXLANGUAGETYPE];
	char *total_pc_money[MAXLANGUAGETYPE];
	char *total_game_num[MAXLANGUAGETYPE];
	char *total_game_money[MAXLANGUAGETYPE];
	
	
} LOGTRANSMENULIST;

const LOGTRANSMENULIST LogTransMenuList = 
{
	{
		"交易记录",
		"Sales record",
		"Sales record",
		"SATIS RAKAMLARI",
		"VENDAS"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"1.货币总收入:",
		"1.Total income:",
		"1.Суммарная выручка:",
		"1.TOPLAM SATIS:",
		"1.RECEITA TOTAL:"
	},
	{
		"2.纸币收入  :",
		"2.Bill income:",	
		"2.Принято купюр:",
		"2.KAGIT PARA SATTIS:",
		"2.NOTAS RECEBIDAS:"
	},
	{
		"3.硬币收入  :",
		"3.Coin income:",
		"3.Принято монет:",
		"3.MADENI PARA SATIS:",
		"3.MOEDAS RECEBIDAS:"
	},
	{
		"4.交易总金额  :",
		"4.Total Trade :",
		"4.Принято монет:",
		"4.MADENI PARA SATIS:",
		"4.MOEDAS RECEBIDAS:"
	}
		,	
	{
		"5.找零总金额:",
		"5.Total change:",	
		"5.Всего разменов:",
		"5.TOPLAM PARA USTU:",
		"5.TROCO TOTAL:"
	},	
	{
		"1.疑问交易次数:",
		"1.Doubtful deal qty:",	
		"1.Кол-во сомнений:",
		"1.SUPHELI SATIS:",
		"1.D\x84\VIDA NA QUANTIDADE:"
	},
	{
		"1.欠条总额:",
		"1.Total IOU amount:",
		"1.IOU:",
		"1.TOPLAM BORC MIKTARI:",
		"1.VALOR TOTAL DE IOU:"
	},
	{
		"7.总交易次数:",
		"7.Total trade num:",
		"7.Total trade num:",
		"7.TOPLAM SATIS NO:",
		"7.Total trade num:",
	},
	{
		"8.总交易金额:",
		"8.Total trade amount:",
		"8.Total trade amount:",
		"8.TOPLAM MIKTAR:",
		"8.Total trade amount:",
	},
	{
		"9.现金交易次数:",
		"9.Cash trade num:",
		"9.Cash trade num:",
		"9.Cash trade num:",
		"9.Cash trade num:",
	},
	{
		"10.现金交易金额:",
		"10.Cash trade amount:",
		"10.Cash trade amount:",
		"10.Cash trade amount:",
		"10.Cash trade amount:",
	},
		{
		"11.卡交易次数:",
		"11.Card trade num:",
		"11.Card trade num:",
		"11.Card trade num:",
		"11.Card trade num:",
	},
	{
		"12.卡交易金额:",
		"12.Card trade amount:",
		"12.Card trade amount:",
		"12.Card trade amount:",
		"12.Card trade amount:",
	},
	{
		"13.在线交易次数:",
		"13.Online trade num:",
		"13.Online trade num:",
		"13.Online trade num:",
		"13.Online trade num:",
	},
	{
		"14.在线交易金额:",
		"14.Online trade amount:",
		"14.Online trade amount:",
		"14.Online trade amount:",
		"14.Online trade amount:",
	},
	{
		"15.游戏交易数量:",
		"15.Game trade num:",
		"15.Game trade num:",
		"15.Game trade num:",
		"15.Game trade num:",
	},
	{
		"16.游戏交易金额:",
		"16.Game trade amount:",
		"16.Game trade amount:",
		"16.Game trade amount:",
		"16.Game trade amount:",
	}
	
		
};



/*************************************交易设置菜单按键选择************************************************/
typedef struct
{
	unsigned char KeyCurrent;						//当前菜单索引号
	unsigned char ExtSelfCheck;						//退出设备自检菜单并返回维护主菜单
	unsigned char PageNumb;							//菜单页索引
	void (*TestOperate)();							//当前状态应该执行的功能操作
} USERTRADEMAINMENU;											//维护主菜单
USERTRADEMAINMENU UserTradeMainMenu;									//维护主菜单结构体


/*************************************交易设置菜单选择************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//交易记录提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *RTC[MAXLANGUAGETYPE];		
	char *Compressor[MAXLANGUAGETYPE];
	char *LED[MAXLANGUAGETYPE];
	char *Multivend[MAXLANGUAGETYPE];
	char *Refund[MAXLANGUAGETYPE];
	char *BillValidatorTest[MAXLANGUAGETYPE];		
	char *CoinAcceptorTest[MAXLANGUAGETYPE];
	char *CoinChangerTest[MAXLANGUAGETYPE];
	char *SelectButtonTest[MAXLANGUAGETYPE];
	char *ButtonTest[MAXLANGUAGETYPE];
	char *InputButton[MAXLANGUAGETYPE];
	char *ButtonLightTest[MAXLANGUAGETYPE];
	char *SelectButton[MAXLANGUAGETYPE];
	char *PCSet[MAXLANGUAGETYPE];
	char *MaxValue[MAXLANGUAGETYPE];
	char *BillValidatorType[MAXLANGUAGETYPE];
	char *CoinAcceptorType[MAXLANGUAGETYPE];
	char *CashlessDeviceType[MAXLANGUAGETYPE];
} USERTRADEMAINMENULIST;

const USERTRADEMAINMENULIST UserTradeMenuList = 
{
	{
		"设备管理",
		"Purchase setting",
		"Purchase setting",
		"SATIS AYARLARI",
		"PROGRAMA\x83\x82\O DE COMPRA"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"① 时间设置",
		"1.Time",
		"1.установка времени",
		"1.ZAMAN",
		"1.HOR\x81\RIO"
	},
	{
		"② 制冷控制",
		"2.Refrigeration",	
		"2.регулирование температуры",
		"2.SOGUTUCU",
		"2.REFRIGERA\x83\x82\O"
	},
	{
		"③ 照明控制",
		"3.LIGHT",
		"3.LED температуры",
		"3.ISIK",
		"3.LUZ"
	},
	{
		"④ 多次购买:",
		"4.Multi-vend:",	
		"4.Мулти решение:",
		"4.COKLU SATIS ACIK:",
		"4.MULTI-VENDA:"
	},
	{
		"⑤ 强制购买:",
		"5.Force-vend:",	
		"5.переопределение:",
		"5.ZORUNLU SATIS:",
		"5.ENERGIA-VENDA:"
	},
	{
		"① 纸币器测试",
		"1.Bill acceptor test",
		"1.Приемнк купюр: ",
		"1.KAGIT PARA TESTI ",
		"1.TESTE RECEPTOR DE NOTA"
	},
	{
		"② 硬币器测试",
		"2.Coin acceptor test",
		"2.Приемник монет: ",
		"2.BOZUK PARA TESTI ",
		"2.TESTE RECEPTOR MOEDA"
	},
	{
		"③ 找零器测试",
		"3.Coin changer test",
		"3.Изменение 1UINT: ",
		"3.PARA BOZMA TESTI ",
		"3.TESTE TROCADOR MOEDA"
	},
	{
		"④ 选货按键测试",
		"4.Select button test",
		"4.Изменение 1UINT: ",
		"4.PARA BOZMA TESTI ",
		"4.TESTE TROCADOR MOEDA"
	}
	,
	{
		"① 测试按键",
		"1.Button test",
		"1.Изменение 1UINT: ",
		"1.PARA BOZMA TESTI ",
		"1.TESTE TROCADOR MOEDA"
	},
	{
		"请按下选货按键号:",
		"Pls click:",
		"Изменение 1UINT: ",
		"PARA BOZMA TESTI ",
		"TESTE TROCADOR MOEDA"
	},
	{
		"② 测试按键指示灯",
		"2.Button light test",
		"2.Изменение 1UINT: ",
		"2.PARA BOZMA TESTI ",
		"2.TESTE TROCADOR MOEDA"
	},
	{
		"按键号",
		"Button NO.",
		" ",
		" ",
		" "
	},
	{
		"④ 工控机设置",
		"4.PC set",
		"4.PC set",
		"4.PC set",
		"4.PC set"
	},
	{
		"⑤ 纸币上限:",
		"5.MaxValue:",
		"5.MaxValue:",
		"5.MAX KAGIT PARA:",
		"5.MaxValue:"
	},
	{
		"① 纸币器类型:",
		"1.BillValidatorType:",	
		"1.BillValidatorType:",
		"1.KAGIT PARA AYARI:",
		"1.BillValidatorType:"
	},
	{
		"② 硬币器类型:",
		"2.CoinAcceptorType:",	
		"2.CoinAcceptorType:",
		"2.BOZUK PARA AYARI:",
		"2.CoinAcceptorType:"
	},
	{
		"③ 读卡器类型:",
		"3.CashlessDeviceType:",
		"3.CashlessDeviceType:",
		"3.KART OKUYUCU AYARI:",
		"3.CashlessDeviceType:"
	}
};

/*************************************时间设置菜单选择************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//交易记录提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *year[MAXLANGUAGETYPE];		
	char *month[MAXLANGUAGETYPE];
	char *day[MAXLANGUAGETYPE];
	char *hour[MAXLANGUAGETYPE];
	char *minute[MAXLANGUAGETYPE];
	char *week[MAXLANGUAGETYPE];
} RTCMAINMENULIST;

const RTCMAINMENULIST RtcMenuList = 
{
	{
		"时间设置",
		"Time",
		"Time",
		"ZAMAN",
		"HOR\x81\RIO"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"① 年:   ",
		"1.Year:   ",
		"1.Год:   ",
		"1.YIL:   ",
		"1.ANO:   "
	},
	{
		"② 月:   ",
		"2.Month:  ",	
		"2.Месяц:   ",
		"2.AY:   ",
		"2.M\x85\S:   "
	},
	{
		"③ 日:   ",
		"3.Day:    ",
		"3.День:   ",
		"3.GUN:   ",
		"3.DIA:   "
	},
	{
		"④ 时:   ",
		"4.Hour:   ",	
		"4.Часы:   ",
		"4.SAAT:   ",
		"4.HORA:  "
	},
	{
		"⑤ 分:   ",
		"5.Minute: ",	
		"5.Минуты:   ",
		"5.DAKIKA:   ",
		"5.MINUTO:"
	},
	{
		"① 星期:   ",
		"1.Week: ",	
		"1.неделя:   ",
		"1.HAFTA:   ",
		"1.semana:"
	}
};



/*************************************设备状态值选择************************************************/
typedef struct
{
	char *EMPTY[MAXLANGUAGETYPE];
	char *ERR[MAXLANGUAGETYPE];	
	char *UNSET[MAXLANGUAGETYPE];	
} ERRORSTATETO;

const ERRORSTATETO ErrorStateTo = 
{
	{
		"缺币",
		"Noenough",
		"Достаточно",
		" yetersiz"
	},	
	{
		"故障",
		"Error",
		"Ошибка",
		"Hata"
	},	
	{
		"未配",
		"UnKnow",
		"UnKnow",
		"UnKnow"
	}
};

/*************************************输入菜单值选择************************************************/
typedef struct
{
	char *Choose[MAXLANGUAGETYPE];		
	char *ON[MAXLANGUAGETYPE];
	char *OFF[MAXLANGUAGETYPE];	
	char *UNSET[MAXLANGUAGETYPE];	
} USERCHOOSE;

const USERCHOOSE UserChoose = 
{
	{
		"1-开启,0-关闭",
		"1-ON,0-OFF",
		"1-ON,0-OFF",
		"1-ON,0-OFF",
		"1-ON,0-OFF"
	},	
	{
		"开启",
		"ON",
		"ON",
		"ON",
		"LIGADO"
	},
	{
		"关闭",
		"OFF",
		"OFF",
		"OFF",
		"DESLIGADO"
	}
		,
	{
		"未配",
		"UnKnow",
		"UnKnow",
		"UnKnow",
		"UnKnow"
	}
};


/**************************************工控机设置菜单****************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//自检模式提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *VPAddCoin[MAXLANGUAGETYPE];		    //补硬币完成
	char *VPSynGoodsCol[MAXLANGUAGETYPE];       //全系统同步
	char *VPAddBill[MAXLANGUAGETYPE];           //取纸币完成 
	char *VPSuccess[MAXLANGUAGETYPE];       //操作成功
	char *VPFail[MAXLANGUAGETYPE];           //操作失败
	char *VPSyning[MAXLANGUAGETYPE];       //全系统同步
	char *VPBuhuo[MAXLANGUAGETYPE];       //补货完成
} USERUBOXPCMENULIST;

const USERUBOXPCMENULIST UserUboxPCMenuList = 
{
	{
		"工控机设置",
		"MAINTAIN MODE",
		"MAINTAIN MODE",
		"MAINTAIN MODE",
		"MAINTAIN MODE"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"① 补硬币完成",
		"1.Sales record",
		"1.Запись запроса",
		"1.Kayxf1x31t sorgulama",
		"1.VENDAS"
	},
	{
		"② 全系统同步",
		"2.Error inqiry",	
		"2.Код ошибки",
		"2.Hata kodu",
		"2.ERRO CONSULTADO"
	},
	{
		"③ 取纸币完成",
		"3.Purchase setting",
		"3.Система",
		"3.Sistem parametreleri",
		"3.COMPRA"
	},
	{
		"操作成功",
		"SUCCESSFUL",	
		"SUCCESSFUL",
		"SUCCESSFUL",
		"SUCCESSFUL"
	},
	{
		"操作失败",
		"FAIL",
		"FAIL",
		"FAIL",
		"FAIL"
	},
	{
		"正在全系统同步...",
		"Busy...",
		"Busy...",
		"Busy...",
		"Busy..."
	},
	{
		"④ 补货完成",
		"4.AddColumn",
		"4.AddColumn",
		"4.AddColumn",
		"4.AddColumn"
	}	
};



/*************************************调试菜单选择************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//交易记录提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *billTrace[MAXLANGUAGETYPE];		
	char *coinTrace[MAXLANGUAGETYPE];
	char *changelTrace[MAXLANGUAGETYPE];
	char *readerTrace[MAXLANGUAGETYPE];
	char *channelTrace[MAXLANGUAGETYPE];
	char *compressorTrace[MAXLANGUAGETYPE];
	char *ledTrace[MAXLANGUAGETYPE];	
	char *selectTrace[MAXLANGUAGETYPE];
	char *PCTrace[MAXLANGUAGETYPE];
	char *TraceFlag[MAXLANGUAGETYPE];
} USERTRACEMAINMENULIST;

const USERTRACEMAINMENULIST UserTraceMenuList = 
{
	{
		"调试设置",
		"Debug mode",
		"Debug mode",
		"AYAR MODU",
		"MODO DE AJUSTE"
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"① 纸币器监控:",
		"1.Bill acceptor:",
		"1.купюр:",
		"1.KAGIT PARA ACIK:",
		"1.RECEPTOR DE NOTA:"
	},
	{
		"② 硬币器监控:",
		"2.Coin acceptor:",	
		"2.ионет:",
		"2.BOZUK P KAPALI:",
		"2.RECEPTOR DE MOEDA:"
	},
	{
		"③ 找零器监控:",
		"3.Changer:",
		"3.Изменение:",
		"3.BOZUK P ACIK:",
		"3.TROCA:"
	},
	{
		"④ 读卡器监控:",
		"4.Card reader:",	
		"4.Читатель типа:",
		"4.KART OKUYUCU ACIK:",
		"4.LEITURA CART\x82\O:"
	},
	{
		"⑤ 货道监控:",
		"5.Column:",	
		"5.Тест колонки:",
		"5.SUTUN ACIK:",
		"5.BANDEJA:"
	},
	{
		"① 压缩机监控:",
		"1.Temp. controller:",
		"1.регулирование:",
		"1.ISI KONTROLU KAPALI:",
		"1.TEMPERATURA:"
	},
	{
		"② 展示灯监控:",
		"2.Light:",	
		"2.LED:",
		"2.ISIK ACIK:",
		"2.LUZ:"
	},
	{
		"③ 选货按键监控:",
		"3.SelectKey:",
		"3.Изменение:",
		"3.SelectKey:",
		"3.SelectKey:"
	},
	{
		"④ PC监控:",
		"4.PC:",
		"4.PC:",
		"4.PC:",
		"4.PC:"
	},
	{
		"⑤ 监控:",
		"5.TraceFlag:",	
		"5.Тест колонки:",
		"5.ACIK:",
		"5.BANDEJA:"
	}
};


/********************************************************************************************
*  货道销售查询菜单
*  add by yoc 2014.02.19
******************************************************************************************************/
typedef struct _user_column_trace_sum_
{
	char *ModeCapion[MAXLANGUAGETYPE];				//交易记录提示
	char *PageDown[MAXLANGUAGETYPE];					//上页提示
	char *PageUp[MAXLANGUAGETYPE];				//下页提示
	char *column1[MAXLANGUAGETYPE]; //货道1销售数量
	char *column2[MAXLANGUAGETYPE]; //货道2销售数量
	char *column3[MAXLANGUAGETYPE]; //货道3销售数量
	char *column4[MAXLANGUAGETYPE]; //货道4销售数量
	
}USER_COLUMN_TRACE_SUM;


const USER_COLUMN_TRACE_SUM user_column_trade_sum = 
{

	{
		"货道交易次数",
		"Column Sale Sum",
		"Column Sale Sum",
		".....",
		"....."
	},	
	{
		"“下一页”",
		"<Next page>",
		"<Next page>",
		"<DIGER SAYFA>",
		"<PR\x80\XIMA P\x81\GINA>"
	},
	{
		"“上一页”",
		"<Pre page>",
		"<Pre page>",
		"<ONCEKI SAYFA>",
		"<P\x81\GINA ANTERIOR>"
	},
	{
		"货道01:",
		".....",
		".....",
		".....",
		"....."
	},
	{
		"货道02:",
		".....",
		".....",
		".....",
		"....."
	},
	{
	    "货道03:",
		".....",
		".....",
		".....",
		"....."
	},
	{
		"货道04:",
		".....",
		".....",
		".....",
		"....."
	}
	
	

};


/**************************************设备自检提示语****************************************************/
typedef struct
{
	char *Balance[MAXLANGUAGETYPE];	
	char *Change[MAXLANGUAGETYPE];	
} SELFCHECKTEXT;

const SELFCHECKTEXT SelfCheckText = 
{
	{
		"金额:",
		"Amount:"	,
		"Баланс:",
		"para girin:",
		"SALDO:"
	},
	{
		"找零:",
		"Change:"	,
		"Change:",
		"Change:",
		"Change:"
	}
};

typedef struct
{
	char *ChannelManage[MAXLANGUAGETYPE];
	char *ChannelAttribute[MAXLANGUAGETYPE];
	char *ChannelAddGoods[MAXLANGUAGETYPE];
	char *ChannelStateLookUp[MAXLANGUAGETYPE];
	char *ChannelCheck[MAXLANGUAGETYPE];
	char *ChannelBad[MAXLANGUAGETYPE];
	char *ChannelEmpty[MAXLANGUAGETYPE];
	char *ChannelPriceZero[MAXLANGUAGETYPE];
	char *ChannelOK[MAXLANGUAGETYPE];
	char *ChannelTest[MAXLANGUAGETYPE];
	char *ChannelLeaveFactory[MAXLANGUAGETYPE];
	char *isTesttingChannel[MAXLANGUAGETYPE];
	char *TestColumnNormal[MAXLANGUAGETYPE];
	char *TestColumnEmpty[MAXLANGUAGETYPE];
	char *TestColumnFault[MAXLANGUAGETYPE];
	char *inputTestColumn[MAXLANGUAGETYPE];
	char *listIsEmpty[MAXLANGUAGETYPE];

	
}LOGCHANNELMENULIST;

const LOGCHANNELMENULIST LogChannelMenuList = 
{
	{
		"货道管理",
		"Column",
		"Column",
		"SUTUN",
		"BANDEJA"
	},
	{
		"① 货道属性 ",
		"1.Column attribute",
		"1.Column attribute",
		"1.SUTUN DEGER",
		"1.ATRIBUTO DA BANDEJA"
	},
	{
		"② 添货",
		"2.Load products",
		"2.Load products",
		"2.URUN YUKLE",
		"2.CARREGANDO PRODUTOS"
	},
	{
		"③ 查看货道状态",
		"3.Check column status",
		"3.Check column status",
		"3.SUTUN KONTROL",
		"3.SITUA\x83\x82\O DAS BANDEJAS"
	},
	{
		"④ 货道检测",
		"4.Column self-test",
		"4.Column self-test",
		"4.SUTUN TEST ",
		"4.TESTE DE BANDEJA"
	},
	{
		"故障货道:         ",
		"Errory column",
		"Errory column",
		"HATALI SUTUN",
		"ERRO BANDEJA"
	},
	{
		"缺货货道:         ",
		"Empty column",
		"Empty column",
		"BOS SUTUN",
		"BANDEJA VAZIA"
	},
	{
		"单价为0的货道:    ",
		"Price in 0 column",
		"Price in 0 column",
		"FIYATSIZ SUTUN",
		"PRE\x83\O NA BANDEJA 0"
	},
	{
		"正常货道:       ",
		"Normal Column",
		"Normal Column",
		"NORMAL SUTUN",
		"BANDEJA NORMAL"
	},
	{
		"货道测试",
		"Column test",
		"Column test",
		"SUTUN OTOMATIK TEST",
		"TESTE DA BANDEJA"
	},
	{
		"⑤货道恢复出厂配置",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	},
	{
		"正在测试货道:%d",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	}
		,
	{
		"货道正常",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	}
		,
	{
		"货道缺货",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	}
		,
	{
		"货道故障",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	}
	,
	{
		"请输入测试货道:",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	}
	,
	{
		"空",
		"5.Restore to default",
		"5.Restore to default",
		"5.FABRIKA AYARI",
		"5.RESTAURAR PADR\x82\O"
	}
	
};



/*****************************************管理级主菜单****************************************************/
typedef struct
{
	char *ModeCapion[MAXLANGUAGETYPE];				//模式提示，但软件版本号统一为英文
	char *MenuCapion[MAXLANGUAGETYPE];				//菜单选择前提示
	char *SelfCheckCapion[MAXLANGUAGETYPE];			//设备自检提示语
	char *SystemSetCapion[MAXLANGUAGETYPE];			//系统设置提示语
	char *AppSetCapion[MAXLANGUAGETYPE];			//应用设置提示语
	char *ReturnVendCapion[MAXLANGUAGETYPE];		//返回交易模式提示语
	char *columnSetCapion[MAXLANGUAGETYPE];		//货道设置
	
} MAINTAINMAINMENULIST;

/*************************************维护主菜单按键选择************************************************/
const MAINTAINMAINMENULIST MaintainMenuList =
{
	{
		"管理员模式",
		"MAINTAIN MODE"
	},
	{
		"请选择菜单",
		"Please select menu"
	},
	{
		"设备自检",
		"SelfCheck"
	},
	{
		"① 参数设置",
		"System Set"
	},
	{
		"货道设置",
		"Column Set"
	},
	{
		"出厂配置",
		"Factory Set"
	},
	{
		"② 货道设置",
		"System Set"
	},
};

typedef struct
{
	char *Channel99Test[MAXLANGUAGETYPE];
	char *ChannelEnterBinNum[MAXLANGUAGETYPE];
	char *ChannelEnterChnlNum[MAXLANGUAGETYPE];
	char *ChannelCheckWait[MAXLANGUAGETYPE];
	char *ChannelEmpty[MAXLANGUAGETYPE];
	char *ChannelOK[MAXLANGUAGETYPE];
	char *ChannelBad[MAXLANGUAGETYPE];
	char *ChannelNOHave[MAXLANGUAGETYPE];
	char *ChannelCommunicationFail[MAXLANGUAGETYPE];
	char *ChannelParamSet[MAXLANGUAGETYPE];
	char *ChannelSuccess[MAXLANGUAGETYPE];
}LOGCHANNELUI;

const LOGCHANNELUI LogChannelUi = 
{
	{
		"正在检测全部货道",
		"Self-testing",
		"Self-testing",
		"TEST EDILIYOR LUTFEN BEKLEYIN",
		"FAVOR AGUARDAR"
	},
	{
		" 请选择箱柜: ",
		" Please Enter Bin Num:",
		" Please Enter Bin Num:",
		" Please Enter Bin Num:",
		" CODIGO DO ARMARIO:"
	},
	{
		" 请输入货道号:     ",
		" Input column code:",
		" Input column code:",
		" SUTUN KODUNU GIR:",
		" C\x80\DIGO DA BANDEJA:"
	},
	{
		" 正在检测%d货道, 请稍等...",
		" Self-testing %d,pls wait...",
		" Self-testing %d,pls wait...",
		" TEST EDILIYOR LUTFEN BEKLEYIN",
		" EM TESTE %d,AGUARDE..."
	},
	{
		" 货道%d检测: 无货! ",
		" Column %d: Empty!",
		" Column %d: Empty!",
		" SUTUN BOS!",
		" BANDEJA %d: VAZIA!"
	},
	{
		" 货道%d检测: 正常! ",
		" Column %d: Normal!",
		" Column %d: Normal!",
		" SUTUN NORMAL!",
		" BANDEJA %d: NORMAL!"
	},
	{
		" 货道%d检测: 故障! ",
		" Column %d: Error!",
		" Column %d: Error!",
		" SUTUN HATALI!",
		" BANDEJA %d: ERRO!"
	},
	{
		" 货道%d不存在! ",
		" Column: nonentity!",
		" Column: nonentity!",
		" NUMARA!",
		" Column: nonentity!"
	},
	{
		" 货道%d检测: 通信故障! ",
		" Column %d:Communication failure!",
		" Column %d:Communication failure!",
		" SUTUN BAGLANTI HATASI!",
		" BANDEJA %d:ERRO COMUNICA\x83\x82O!"
	},
	{
		" 货道参数设置",
		" Column Param Set",
		" Column Param Set",
		" SUTUN DEGER",
		" ATRIBUTO DA BANDEJA"
	},
	{
		" 成功交易次数",
		" Success deal qty",
		" Success deal qty",
		" BASARILI ISLEM SAYISI",
		" SUCESSO TOTAL DE VENDAS"
	}
};

typedef struct
{
	char *EnterBinnum[MAXLANGUAGETYPE];
	char *A_Bin[MAXLANGUAGETYPE];
	char *B_Bin[MAXLANGUAGETYPE];
	char *IsHandleWait[MAXLANGUAGETYPE];
	char *EnterColumnNum[MAXLANGUAGETYPE];//请输入货道号
	char *NoColumn[MAXLANGUAGETYPE];
	char *ColumnPrice[MAXLANGUAGETYPE];
	char *ColumnNum[MAXLANGUAGETYPE];
	char *ColumnStatus[MAXLANGUAGETYPE];
	char *ColumnSuccess[MAXLANGUAGETYPE];
	char *Yes[MAXLANGUAGETYPE];
	char *Cancel[MAXLANGUAGETYPE];
	char *AddingWait[MAXLANGUAGETYPE];
	char *EnterTrayNum[MAXLANGUAGETYPE];
	char *SureAddTray[MAXLANGUAGETYPE];
	char *LoadAllColumn[MAXLANGUAGETYPE];
	char *Restoring[MAXLANGUAGETYPE];
	char *SureRest[MAXLANGUAGETYPE];
	char *ColumnRest[MAXLANGUAGETYPE];//是否确定初始化?
	char *TrayManage[MAXLANGUAGETYPE];
	char *OpeningColumn[MAXLANGUAGETYPE];
	char *ClosingColumn[MAXLANGUAGETYPE];
	char *BinAttributeSet[MAXLANGUAGETYPE];
	char *ColumnSetClose[MAXLANGUAGETYPE];
	char *ColumnSetOpen[MAXLANGUAGETYPE];
	char *Open[MAXLANGUAGETYPE];//" ① 开启",
	char *Close[MAXLANGUAGETYPE];
	char *CurrentColumn[MAXLANGUAGETYPE];
	char *IsOpen[MAXLANGUAGETYPE];
	char *MaxCapacity[MAXLANGUAGETYPE];
	char *ColumnCapacity[MAXLANGUAGETYPE];
	char *TrayOpen[MAXLANGUAGETYPE];//" %d层架状态: 开启",
	char *TrayClose[MAXLANGUAGETYPE];
	char *ColumnSet[MAXLANGUAGETYPE];
	char *CurrentTray[MAXLANGUAGETYPE];
	char *ColumnOpen[MAXLANGUAGETYPE];
	char *ColumnClose[MAXLANGUAGETYPE];
	char *ColumnSelectNum[MAXLANGUAGETYPE];
	char *ColumnGoodsNum[MAXLANGUAGETYPE];
	char *Columnrest[MAXLANGUAGETYPE];
	char *Columnmanage[MAXLANGUAGETYPE];
	char *Columnopen[MAXLANGUAGETYPE];
	char *Columnclose[MAXLANGUAGETYPE];
	char *columnOK[MAXLANGUAGETYPE];
	char *columnFault[MAXLANGUAGETYPE];
	char *columnEmpty[MAXLANGUAGETYPE];
	char *ColumnOpenORClose[MAXLANGUAGETYPE];//货道开关
	char *columnInitOK[MAXLANGUAGETYPE];//货道初始化成功
	char *columnLevelOpenOrClose[MAXLANGUAGETYPE];//层架开关
	char *InputColumnNo[MAXLANGUAGETYPE];//请选择层号: 
	
	
	
}LOGCHANNELUIDISP;


const LOGCHANNELUIDISP LogChannelUiDisp=
{
	{
		" 输入箱柜编号: ",
		" Input cabinet code: ",
		" Input cabinet code: ",
		" Input cabinet code: ",
		" CODIGO DO ARMARIO: "
	},
	{
		" ① A 柜",
		" 1.A cabinet",
		" 1.A cabinet",
		" 1.A cabinet",
		" 1.A ARMARIO"
	},
	{
		" ② B 柜",
		" 2.B cabinet",
		" 2.B cabinet",
		" 2.B cabinet",
		" 2.B ARMARIO"
	},
	{
		" 正在操作, 请稍等...",
		" operating,pls waiting...",
		" operating,pls waiting...",
		" LUTFEN BEKLEYIN...",
		" FAVOR AGUARDAR..."
	},
	{
		" 货道号: %d ",
		" Input column code:%d ",
		" Input column code:%d ",
		" SUTUN KODUNU GIR:%d ",
		" C\x80\DIGO DA BANDEJA:%d "
	},
	{
		" 货道不存在! ",
		" Column N/A!",
		" Column N/A!",
		" NUMARA!",
		" COLUNA N\x82\O DISPONIVEL!"
	},
	{
		" ① 货道商品单价 :",
		" 1.Unit price:%s",
		" 1.Unit price:%s",
		" 1.BIRIM FIYAT:%s",
		" 1.PRE\x83\O UNIT\x81\RIO:%s"
	},
	{
		" ② 货道储货数量 :",
		" 2.Column capacity:%s  ",
		" 2.Column capacity:%s  ",
		" 2.SUTUN KAPASITESI:%s  ",
		" 2.CAPACIDADE DA BANDEJA:%s  "
	},
	{
		" ③ 货道当前状态 :",
		" 3.Column status:%s  ",
		" 3.Column status:%s  ",
		" 3.SUTUN STATUS:%s  ",
		" 3.SITUA\x83\x82\O DA BANDEJA:%s  "
	},
	{
		" ④ 成功交易次数:",
		" %d Success deal qty: %d  ",
		" %d Success deal qty: %d  ",
		" BASARILI SATIS: %d  ",
		" %d SUCESSO : %d  "
	},
	{
		" 确定",
		" YES",
		" ① YES",
		" ① GIRIS",
		" ① ENTER"
	},
	{
		" 取消",
		" CANCEL",
		" ② CANCEL",
		" ② IPTAL",
		" ② CANCELAR"
	},
	{
		" 正在添货, 请稍等...",
		" Loading, pls waiting...",
		" Loading, pls waiting...",
		" YUKLENIYOR LUTFEN BEKLEYIN...",
		" CARREGANDO, POR FAVOR ESPERE"
	},
	{
		" 请选择层号: ",
		" Input tray code:",
		" Input tray code:",
		" RAF KODUNU GIR:",
		" INSIRA C\x80\DIGO DA BANDEJA:"
	},
	{
		" 确定%d 层添货?",
		" Load for %d tray?",
		" Load for %d tray?",
		" DOLDUR?",
		" CARREGANDO PARA %d BANDEIJA?"
	},
	{
		" 确定所有货道添货? ",
		" Load for all columns?",
		" Load for all columns?",
		" TUM SUTUNLARI DOLDUR?",
		" CARREGAR TODAS AS BANDEJAS?"
	},
	{
		" 请稍等, 正在恢复...",
		" Restoring, pls wait...",
		" Restoring, pls wait...",
		" AYARLANIYOR LUTFEN BEKLEYIN",
		" Restoring, pls wait..."
	},
	{
		" 是否确定初始化? ",
		" Be sure to initialize?",
		" Be sure to initialize?",
		" Be sure to initialize?",
		" CERTIFIQUE-SE DE INICIALIZAR?"
	},
	{
		" ① 货道初始化",
		" ① Column initialize",
		" ① Column initialize",
		" ① Column initialize",
		" ① COLUNA INICIALIZAR"
	},
	{
		" ② 层架管理",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② GERENCIADOR DE BANDEIJA"
	},
	{
		" 正在开启%d货道...",
		" Enable %d Column...",
		" Enable %d Column...",
		" Enable %d Column...",
		" ATIVAR %d COLUNA..."
	},
	{
		" 正在关闭%d货道...",
		" Disable %d Column...",
		" Disable %d Column...",
		" Disable %d Column...",
		" DESATIVAR %d COLUNA..."
	},
	{
		"货道属性设置",
		"Cabinet setting",
		"Cabinet setting",
		"Cabinet setting",
		"CONFIGURA\x83\x82\O DE ATRIBUTO DE ARMARIO"
	},
	{
		" %d货道属性设置: 关闭",
		" %d column setting:OFF",
		" %d column setting:OFF",
		" %d column setting:OFF",
		" %d CONFIGURA\x83\x82\O DE ATRIBUTO DE COLUNA:DESLIGADO"
	},
	{
		" %d货道属性设置: 开启",
		" %d column setting:ON",
		" %d column setting:ON",
		" %d column setting:ON",
		" %d CONFIGURA\x83\x82\O DE ATRIBUTO DE COLUNA:ATIVAR"
	},
	{
		" ① 开启",
		" ① ON",
		" ① ON",
		" ① ON",
		" ① ATIVAR"
	},
	{
		" ② 关闭",
		" ② OFF",
		" ② OFF",
		" ② OFF",
		" ② DESLIGADO"
	},
	{
		" 当前货道:%d",
		" Current Column:%d",
		" Current Column:%d",
		" Current Column:%d",
		"  SITUA\x83\x82\O DA COLUNA:%d"
	},
	{
		" ① 开启",
		" ① Column enabling",
		" ① Column enabling",
		" ① Column enabling",
		" ① ATIVAR"
	},
	{
		" ② 最大存货量: %s ",
		" ② Capacity: %s ",
		" ② Capacity: %s ",
		" ② Capacity: %s ",
		" ② CAPACIDADE: %s "
	},
	{
		" %d货道当前最大存货量:%s ",
		" Capacity of %d column:%s ",
		" Capacity of %d column:%s ",
		" Capacity of %d column:%s ",
		" CAPACIDADE DESLIGADA %d COLUNA:%s "
	},
	{
		" %d层架状态: 开启",
		" ① Tray Status: ON",
		" ① Tray Status: ON",
		" ① Tray Status: ON",
		" ① BENDEIJA SITUA\x83\x82\O:ATIVAR"
	},
	{
		" %d层架状态: 关闭",
		" ① Tray Status: OFF",
		" ① Tray Status: OFF",
		" ① Tray Status: OFF",
		" ① BENDEIJA SITUA\x83\x82\O:DESLIGADO"
	},
	{
		" ③ 单货道配置",
		" ③ Column setting",
		" ③ Column setting",
		" ③ Column setting",
		" ③ CONFIGURA\x83\x82\O DA COLUNA"
	},
	{
		" 当前层编号:%d",
		" Current Tray:%d",
		" Current Tray:%d",
		" Current Tray:%d",
		" BENDEIJA C\x80\DIGO:%d"
	},
	{
		" 货道状态:开启",
		" Column Status: ON",
		" ① Column Status: ON",
		" ① Column Status: ON",
		" ① SITUA\x83\x82\O DA BANDEJA: ON"
	},
	{
		" 货道状态: 关闭",
		" Column Status: OFF",
		" ① Column Status: OFF",
		" ① Column Status: OFF",
		" ① SITUA\x83\x82\O DA BANDEJA: OFF"
	},
	{
		" 货道选货按键:%s",
		" Column SelectNum:%s",
		" Column SelectNum:%s",
		" Column SelectNum:%s",
		" Column SelectNum:%s"
	},
	{
		" 货道商品编号:%s",
		" Column Goods Num:%s",
		" Column Goods Num:%s",
		" Column Goods Num:%s",
		" Column Goods Num:%s"
	},
	{
		" 货道初始化",
		" Column initialize",
		" Column initialize",
		" Column initialize",
		" COLUNA INICIALIZAR"
	},
	{
		" 货道配置",
		" ③ Column Manage",
		" ③ Column Manage",
		" ③ SUTUN",
		" ③ COLUNA DE BANDEIJA"
	},
	{
		" 货道状态:开启",
		" Column Status: ON",
		" Column Status: ON",
		" Column Status: ON",
		" SITUA\x83\x82\O DA BANDEJA: ON"
	},
	{
		" 货道状态: 关闭",
		" Column Status: OFF",
		" Column Status: OFF",
		" Column Status: OFF",
		" SITUA\x83\x82\O DA BANDEJA: OFF"
	}
		,
	{
		" 正常",
		" Column Status: OFF",
		" Column Status: OFF",
		" Column Status: OFF",
		" SITUA\x83\x82\O DA BANDEJA: OFF"
	}
		,
	{
		" 故障",
		" Column Status: OFF",
		" Column Status: OFF",
		" Column Status: OFF",
		" SITUA\x83\x82\O DA BANDEJA: OFF"
	}
		,
	{
		" 缺货",
		" Column Status: OFF",
		" Column Status: OFF",
		" Column Status: OFF",
		" SITUA\x83\x82\O DA BANDEJA: OFF"
	}
	,
	{
		" ③ 货道开关",
		" ③ Tray Manage",
		" ③ Tray Manage",
		" ③ Tray Manage",
		" ③ GERENCIADOR DE BANDEIJA"
	}
	,
	{
		" 初始化成功",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② GERENCIADOR DE BANDEIJA"
	}
	,
	{
		" ② 层架开关",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② GERENCIADOR DE BANDEIJA"
	}
	,
	{
		" 请输入货道号:",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② Tray Manage",
		" ② GERENCIADOR DE BANDEIJA"
	}
};


typedef struct
{
	char *ChannelOneAdd[MAXLANGUAGETYPE];
	char *ChannelLevelAdd[MAXLANGUAGETYPE];
	char *ChannelAllAdd[MAXLANGUAGETYPE];
	char *ChannelSelectHandle[MAXLANGUAGETYPE];
	char *inputAddGoodsNum[MAXLANGUAGETYPE];
	char *inputAddGoodsNumAndValue[MAXLANGUAGETYPE];
	char *isSaveHuodao[MAXLANGUAGETYPE];
	
}LOGCHANNELADDGOODS;


const LOGCHANNELADDGOODS LogChannelAddGoods=
{
	{
		"①单货道添货",
		"1.Load for column",
		"1.Load for column",
		"1.SUTUN DOLDUR",
		"1.CARREGANDO A BANDEJA"
	},
	{
		"②层货道添货",
		"2.Load for tray",
		"2.Load for tray",
		"2.RAF DOLDUR",
		"2.CARREGANDO A BANDEJA"
	},
	{
		"③全货道添货",
		"3.Load for all columns",
		"3.Load for all columns",
		"3.TUM SUTUNLARI DOLDUR",
		"3.CARREGANDO TODAS AS BANDEJAS"
	},
	{
		" 请选择操作: ",
		" Pls choose:",
		" Pls choose:",
		" LUTFEN SECIN:",
		" POR FAVOR ESCOLHA:"
	},
	{
		" 请输入添货数量:",
		" Pls choose:",
		" Pls choose:",
		" LUTFEN SECIN:",
		" POR FAVOR ESCOLHA:"
	}
		,
	{
		" 请输入添货数量:%d",
		" Pls choose:",
		" Pls choose:",
		" LUTFEN SECIN:",
		" POR FAVOR ESCOLHA:"
	}
	,
	{
		" 设置成功",
		" Pls choose:",
		" Pls choose:",
		" LUTFEN SECIN:",
		" POR FAVOR ESCOLHA:"
	}
		
}; 



/*************************************维护主菜单按键选择************************************************/
typedef struct
{
	unsigned char KeyCurrentState;							//当前菜单索引号
	unsigned char KeyEnterState;							//按下"回车"转向的菜单索引号
	void (*CurrentOperate)();								//当前状态应该执行的功能操作
} MAINTAINMAINMENU;											//维护主菜单

MAINTAINMAINMENU MaintainMainMenu;


/*************************************工程菜单选择************************************************/
typedef struct
{
	char *Language[MAXLANGUAGETYPE];		
	char *CoinAcceptorType[MAXLANGUAGETYPE];
	char *CoinChangerType[MAXLANGUAGETYPE];
	char *BillValidatorType[MAXLANGUAGETYPE];
	char *BillRecyclerType[MAXLANGUAGETYPE];
	char *CashlessDeviceType[MAXLANGUAGETYPE];
	char *DecimalNum[MAXLANGUAGETYPE];	
	char *Logo[MAXLANGUAGETYPE];	
	char *UserSelectKeyBoard[MAXLANGUAGETYPE];		
	char *ChannelType[MAXLANGUAGETYPE];
	char *SubBinOpen[MAXLANGUAGETYPE];
	char *SubBinChannelType[MAXLANGUAGETYPE];
	char *GOCIsOpen[MAXLANGUAGETYPE];
	char *BillITLValidator[MAXLANGUAGETYPE];
	char *SaleTime[MAXLANGUAGETYPE];	
	char *PcEnable[MAXLANGUAGETYPE];
	char *coin1[MAXLANGUAGETYPE];		
	char *coin2[MAXLANGUAGETYPE];
	char *coin3[MAXLANGUAGETYPE];
	char *coin4[MAXLANGUAGETYPE];
	char *coin5[MAXLANGUAGETYPE];
	char *coin6[MAXLANGUAGETYPE];
	char *coin7[MAXLANGUAGETYPE];	
	char *coin8[MAXLANGUAGETYPE];
	char *hopper1[MAXLANGUAGETYPE];		
	char *hopper2[MAXLANGUAGETYPE];
	char *hopper3[MAXLANGUAGETYPE];
	char *bill1[MAXLANGUAGETYPE];		
	char *bill2[MAXLANGUAGETYPE];
	char *bill3[MAXLANGUAGETYPE];
	char *bill4[MAXLANGUAGETYPE];
	char *bill5[MAXLANGUAGETYPE];
	char *bill6[MAXLANGUAGETYPE];
	char *bill7[MAXLANGUAGETYPE];	
	char *bill8[MAXLANGUAGETYPE];
	char *CunhuoKou[MAXLANGUAGETYPE];
	char *EasiveEnable[MAXLANGUAGETYPE];
	char *MaxValue[MAXLANGUAGETYPE];
	char *Channel[MAXLANGUAGETYPE];	
	char *SubChannel[MAXLANGUAGETYPE];
} SYSMAINMENULIST;

const SYSMAINMENULIST SysMenuList = 
{	
	{
		"① 语言:",
		"1.Language:",
		"1.Language:",
		"1.Language:",
		"1.Language:"
	},
	{
		"② 硬币器类型:",
		"2.CoinAcceptorType:",	
		"2.CoinAcceptorType:",
		"2.CoinAcceptorType:",
		"2.CoinAcceptorType:"
	},
	{
		"③ 找零器类型:",
		"3.CoinChangerType:",
		"3.CoinChangerType:",
		"3.CoinChangerType:",
		"3.CoinChangerType:"
	},
	{
		"④ 纸币器类型:",
		"4.BillValidatorType:",	
		"4.BillValidatorType:",
		"4.BillValidatorType:",
		"4.BillValidatorType:"
	},
	{
		"⑤ 纸币找零类型:",
		"5.BillRecyclerType:",	
		"5.BillRecyclerType:",
		"5.BillRecyclerType:",
		"5.BillRecyclerType:"
	},
	{
		"⑥ 读卡器类型:",
		"6.CashlessDeviceType:",
		"6.CashlessDeviceType:",
		"6.CashlessDeviceType:",
		"6.CashlessDeviceType:"
	},
	{
		"⑤ 小数点位数:",
		"7.DecimalNum:⑤",	
		"7.DecimalNum:⑦",
		"7.DecimalNum:",
		"7.DecimalNum:"
	},
	{
		"⑧ 商标:",
		"8.Logo:",
		"8.Logo:",
		"8.Logo:",
		"8.Logo:"
	},
	{
		"① 选货按键:",
		"1.UserSelectKeyBoard:",
		"1.UserSelectKeyBoard:",
		"1.UserSelectKeyBoard:",
		"1.UserSelectKeyBoard:"
	},
	{
		"② 主柜类型:",
		"2.ChannelType:",	
		"2.ChannelType:",
		"2.ChannelType:",
		"2.ChannelType:"
	},
	{
		"③ 开启从柜:",
		"3.SubBinOpen:",
		"3.SubBinOpen:",
		"3.SubBinOpen:",
		"3.SubBinOpen:"
	},
	{
		"④ 从柜类型:",
		"4.SubBinChannelType:",	
		"4.SubBinChannelType:",
		"4.SubBinChannelType:",
		"4.SubBinChannelType:"
	},
	{
		"⑤ 开启GOC:",
		"5.GOCIsOpen:",	
		"5.GOCIsOpen:",
		"5.GOCIsOpen:",
		"5.GOCIsOpen:"
	},
	{
		"⑥ ITL纸币器:",
		"6.BillITLValidator:",
		"6.BillITLValidator:",
		"6.BillITLValidator:",
		"6.BillITLValidator:"
	},
	{
		"⑦ 交易时间:",
		"7.SaleTime:",	
		"7.SaleTime:",
		"7.SaleTime:",
		"7.SaleTime:"
	},
	{
		"⑧ PC类型:",
		"8.PcEnable:",
		"8.PcEnable:",
		"8.PcEnable:",
		"8.PcEnable:"
	},
	{
		"① 硬币1:",
		"1.coin1:",
		"1.coin1:",
		"1.coin1:",
		"1.coin1:"
	},
	{
		"② 硬币2:",
		"2.coin2:",	
		"2.coin2:",
		"2.coin2:",
		"2.coin2:"
	},
	{
		"③ 硬币3:",
		"3.coin3:",
		"3.coin3:",
		"3.coin3:",
		"3.coin3:"
	},
	{
		"④ 硬币4:",
		"4.coin4:",	
		"4.coin4:",
		"4.coin4:",
		"4.coin4:"
	},
	{
		"⑤ 硬币5:",
		"5.coin5:",	
		"5.coin5:",
		"5.coin5:",
		"5.coin5:"
	},
	{
		"⑥ 硬币6:",
		"6.coin6:",
		"6.coin6:",
		"6.coin6:",
		"6.coin6:"
	},
	{
		"⑦ 硬币7:",
		"7.coin7:",	
		"7.coin7:",
		"7.coin7:",
		"7.coin7:"
	},
	{
		"⑧ 硬币8:",
		"8.coin8:",
		"8.coin8:",
		"8.coin8:",
		"8.coin8:"
	},
	{
		"① hopper1:",
		"1.hopper1:",
		"1.hopper1:",
		"1.hopper1:",
		"1.hopper1:"
	},
	{
		"② hopper2:",
		"2.hopper2:",	
		"2.hopper2:",
		"2.hopper2:",
		"2.hopper2:"
	},
	{
		"③ hopper3:",
		"3.hopper3:",
		"3.hopper3:",
		"3.hopper3:",
		"3.hopper3:"
	},
	{
		"① 纸币1:",
		"1.bill1:",
		"1.bill1:",
		"1.bill1:",
		"1.bill1:"
	},
	{
		"② 纸币2:",
		"2.bill2:",	
		"2.bill2:",
		"2.bill2:",
		"2.bill2:"
	},
	{
		"③ 纸币3:",
		"3.bill3:",
		"3.bill3:",
		"3.bill3:",
		"3.bill3:"
	},
	{
		"④ 纸币4:",
		"4.bill4:",	
		"4.bill4:",
		"4.bill4:",
		"4.bill4:"
	},
	{
		"⑤ 纸币5:",
		"5.bill5:",	
		"5.bill5:",
		"5.bill5:",
		"5.bill5:"
	},
	{
		"⑥ 纸币6:",
		"6.bill6:",
		"6.bill6:",
		"6.bill6:",
		"6.bill6:"
	},
	{
		"⑦ 纸币7:",
		"7.bill7:",	
		"7.bill7:",
		"7.bill7:",
		"7.bill7:"
	},
	{
		"⑧ 纸币8:",
		"8.bill8:",
		"8.bill8:",
		"8.bill8:",
		"8.bill8:"
	},
	{
		"① 手动补货:",
		"1.CunhuoKou:",
		"1.CunhuoKou:",
		"1.CunhuoKou:",
		"1.CunhuoKou:"
	},
	{
		"② 易丰PC:",
		"2.easivendPC:",	
		"2.easivendPC:",
		"2.easivendPC:",
		"2.easivendPC:"
	},
	{
		"③ 纸币上限:",
		"3.MaxValue:",
		"3.MaxValue:",
		"3.MaxValue:",
		"3.MaxValue:"
	},
	{
		"④ 主柜种类:",
		"4.Channel:",	
		"4.Channel:",
		"4.Channel:",
		"4.Channel:"
	},
	{
		"⑤ 从柜种类:",
		"5.SubChannel:",	
		"5.SubChannel:",
		"5.SubChannel:",
		"5.SubChannel:"
	}
};
/**************************************End Of File*******************************************************/

