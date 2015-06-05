/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**--------------File Info---------------------------------------------------------------------------------
** File name:               zy_if.c
** Latest modified Date:    2009-07-23
** Latest Version:          1.00
** Descriptions:            �û���д�Ļ����ӿں���,��������Ȩģʽ����
**--------------------------------------------------------------------------------------------------------
** Created by:              Chenmingji
** Created date:            2009-07-23
** Version:                 1.0
** Descriptions:            The original version
*********************************************************************************************************/
#include "..\config.h"
#include "zy_if.h"
#include "zy_if_cfg.h"
#include <string.h>
/*********************************************************************************************************
  ��Ϣͨ����Ϣ
*********************************************************************************************************/
struct __zy_if_msg {
    INT32U    ulMsgSize;                                                /*  ��Ϣ��С                    */
    OS_EVENT *poeQ;                                                     /*  ���ݶ���ָ��                */
    OS_MEM   *pomMsg;                                                   /*  �ڴ����ָ��                */
};
typedef  struct __zy_if_msg     __ZY_IF_MSG;
/*********************************************************************************************************
** Function name:           zyIfInit
** Descriptions:            �ӿڳ�ʼ��
** input parameters:        none
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIfInit (void)
{
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyReset
** Descriptions:            ϵͳ��λ
** input parameters:        uiMode: ZY_POWER_RESET: �ϵ縴λ
**                                  ZY_HARD_RESET:  Ӳ����λ
**                                  ZY_SOFT_RESET:  �����λ
**                                  ����:           ��ϵͳ���
** output parameters:       none
** Returned value:          none
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void zyReset (unsigned int uiMode)
{
    switch (uiMode) {

    case ZY_POWER_RESET:                                                /*  ��ϵͳ�ϵ縴λ��ͬӲ����λ   */

#if 0
        break;
#endif                                                                  /*  0                           */

    case ZY_HARD_RESET:
        changeToSYSMode();
        AITCR = (0x05fa << 16) + 4;
        break;

    case ZY_SOFT_RESET:
        changeToSYSMode();
        AITCR = (0x05fa << 16) + 1;
        break;
    
    default:                                                            /*  ��������ȷ����λ            */
        break;
    }
}
/*********************************************************************************************************
** Function name:           zyIrqDisable
** Descriptions:            ��ֹ�ж�
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIrqDisable (void)
{
    OS_ENTER_CRITICAL();
    return 0;
}
/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            �����ж�
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIrqEnable (void)
{
    OS_EXIT_CRITICAL();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyIsrSet
** Descriptions:            IOϵͳ�����жϷ������
** input parameters:        uiChannel:  �ж�ͨ����
**                          ulFunction: �жϷ�������ַ
**                          uiPrio:     �ж����ȼ�
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIsrSet (unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();

    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
   
        ((INT32U *)0xE000E100)[uiTmp1] = 1ul << uiTmp2;
        ((INT8U *)0xE000E400)[uiTmp3]  = uiPrio;
    } else {
        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;

        default:
            break;
        }
        if (uiChannel >= MMI) {
            uiTmp3 = uiChannel - MMI;
            ((INT8U *)0xE000ED18)[uiTmp3]  = uiPrio;
        }
    }

#if VECTOR_TABLE_IN_FLASH == 0
    ((unsigned long *)VTOR)[uiChannel] = ulFunction;
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zdIsrClr
** Descriptions:            IOϵͳ����жϷ������
** input parameters:        uiChannel:  �ж�ͨ����
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S __s_zyIsrClr (unsigned int uiChannel)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();
    
    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
    
        ((INT32U *)0xE000E180)[uiTmp1] = 1ul << uiTmp2;
        ((INT8U *)0xE000E400)[uiTmp3]  = 0;

    } else {

        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR & ~(1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR & ~(1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR & ~(1 << 18);
            break;

        default:
            break;
        }
        if (uiChannel >= MMI) {
            uiTmp3 = uiChannel - MMI;
            ((INT8U *)0xE000ED18)[uiTmp3]  = 0;
        }
    }

#if VECTOR_TABLE_IN_FLASH == 0
    ((unsigned long *)VTOR)[uiChannel] = 0;
#endif                                                                  /*  VECTOR_TABLE_IN_FLASH       */

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyIsrDisable
** Descriptions:            ��ָֹ���ж�
** input parameters:        uiChannel:  �ж�ͨ����
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S __s_zyIsrDisable (unsigned int uiChannel)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }
    
    if (uiChannel < 16) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();    

    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
    
        ((INT32U *)0xE000E180)[uiTmp1] = 1ul << uiTmp2;

    } else {

        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR & ~(1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR & ~(1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR & ~(1 << 18);
            break;

        default:
            break;
        }
    }

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyIsrEnable
** Descriptions:            ����ָ���ж�
** input parameters:        uiChannel:  �ж�ͨ����
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyIsrEnable (unsigned int uiChannel)
{
    unsigned int uiTmp1, uiTmp2, uiTmp3;
    
    if (uiChannel > MAX_VICS) {
        return -ZY_NOT_OK;
    }

    zyIrqDisable();
    changeToSYSMode();

    if (uiChannel >= 16) {
        uiTmp3 = uiChannel - 16;
        uiTmp1 = uiTmp3 / 32;
        uiTmp2 = uiTmp3 % 32;
        ((INT32U *)0xE000E100)[uiTmp1] = 1ul << uiTmp2;

    } else {

        switch (uiChannel) {
        
        case MMI:
            SHCSR = SHCSR | (1 << 16);
            break;

        case BFI:
            SHCSR = SHCSR | (1 << 17);
            break;

        case UFI:
            SHCSR = SHCSR | (1 << 18);
            break;

        default:
            break;
        }
    }

    changeToUSRMode();
    zyIrqEnable();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyHeapMalloc
** Descriptions:            �ѷ����ڴ�
** input parameters:        ulSize: �ڴ��С
** output parameters:       none
** Returned value:          �ڴ��ַ,NULLΪ���ɹ�
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
void *zyHeapMalloc (INT32U ulSize)
{
    void *pvRt = NULL;                                                  /*  ����ֵ                      */

    zyIrqDisable();
    pvRt = malloc((size_t)ulSize);
    zyIrqEnable();
    return pvRt;
}
/*********************************************************************************************************
** Function name:           zyHeapFree
** Descriptions:            ���ͷ��ڴ�
** input parameters:        pvPrt: Ҫ�ͷŵ��ڴ�
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyHeapFree (void *pvPrt)
{
    zyIrqDisable();
    free(pvPrt);
    zyIrqEnable();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyThreadCreate
** Descriptions:            �����߳�
** input parameters:        pvThreedMain: �߳�������
**                          pvArg:        ���̵߳Ĳ���
**                          uiPrio:       �߳����ȼ�
**                          ulSize:       ��ջ��С
** output parameters:       none
** Returned value:          �߳�ID,0Ϊ���ɹ�
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zyThreadCreate (void *pvThreedMain, void *pvArg, unsigned int uiPrio, INT32U ulSize)
{
    OS_STK       *pstpTmp1;
    OS_TCB      **pptcpTmp2;
    unsigned long ulRt;
    
    pstpTmp1 = (OS_STK *)zyHeapMalloc(ulSize * sizeof(OS_STK));
    if (pstpTmp1 == NULL) {
        return 0;
    }

    OSSchedLock();
    
    if (OSTaskCreate((void (*) (void *))pvThreedMain,
                     pvArg,
#if OS_STK_GROWTH == 1
                     pstpTmp1 + ulSize - 1,
#else
                     pstpTmp1,
#endif                                                                  /*  OS_STK_GROWTH               */
                     (INT8U)uiPrio) != OS_NO_ERR) {
        OSSchedUnlock();
        return 0;
    }    

    pptcpTmp2 = OSTCBPrioTbl;
    ulRt      = (unsigned long)pptcpTmp2[uiPrio];
    
    OSSchedUnlock();
    
    return ulRt;
}
/*********************************************************************************************************
** Function name:           zyThreadDel
** Descriptions:            ɾ���߳�
** input parameters:        ulThreadId: �߳�ID
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadDel (unsigned long ulThreadId)
{
    OS_TCB *ptcbTask;                                                   /*  ����TCB                     */
    void   *pvTmp1;

    ptcbTask = (OS_TCB *)ulThreadId;

    if (zyThreadGetCurId() == ulThreadId) {
        zyIrqDisable();
        pvTmp1 = (void *)(ptcbTask->OSTCBStkPtr);
        zyHeapFree(pvTmp1);
        OSTaskDel(ptcbTask->OSTCBPrio);
        zyIrqEnable();
        return ZY_OK;
    }

    OSSchedLock();

    pvTmp1 = (void *)(ptcbTask->OSTCBStkPtr);
    zyHeapFree(pvTmp1);
    OSTaskDel(ptcbTask->OSTCBPrio);

    OSSchedUnlock();
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyThreadSuspendSelf
** Descriptions:            �����߳�����
** input parameters:        none
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadSuspendSelf (void)
{
    OSTaskSuspend(OS_PRIO_SELF);
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyThreadResume
** Descriptions:            �ָ�����
** input parameters:        ulThreadId: �߳�ID
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadResume (unsigned long ulThreadId)
{
    OS_TCB *ptcbTask;                                                   /*  ����TCB                     */

    ptcbTask = (OS_TCB *)ulThreadId;

    OSTaskResume(ptcbTask->OSTCBPrio);
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyThreadGetCurId
** Descriptions:            ��õ�ǰ�߳�ID
** input parameters:        none
** output parameters:       none
** Returned value:          ��ǰ�߳�ID
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zyThreadGetCurId (void)
{
    return (unsigned long)OSTCBCur;
}
/*********************************************************************************************************
** Function name:           __zyThreadSuspendCheck
** Descriptions:            ����߳��Ƿ�ȷʵ������
** input parameters:        ulThreadId: �߳�ID
** output parameters:       none
** Returned value:          zy_OK:      ����
**                          -ZY_NOT_OK: δ����
**                          ��������:   ����,����ֵ�ο�zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadSuspendCheck (unsigned long ulThreadId)
{
    OS_TCB *ptcbTask;                                                   /*  ����TCB                     */

    ptcbTask = (OS_TCB *)ulThreadId;
    if(((ptcbTask->OSTCBStat) & OS_STAT_SUSPEND) != OS_STAT_RDY) {
        return ZY_OK;
    }

    return -ZY_NOT_OK;
}
/*********************************************************************************************************
** Function name:           zyThreadDelay
** Descriptions:            ��ǰ�߳���ʱ
** input parameters:        ulTimes: �ȴ�ʱ��(����)
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyThreadDelay (unsigned long ulTimes)
{
    if (ulTimes == 0) {
        return -ZY_PARAMETER_ERR;
    }

    OSTimeDly((INT16U)((ulTimes * OS_TICKS_PER_SEC + 999) / 1000));
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zySemCreate
** Descriptions:            �����ź���
** input parameters:        ulCnt:  �ź�����ʼֵ
** output parameters:       none
** Returned value:          �ź�����ʶ
**                          0: ���ɹ�
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zySemCreate (INT32S ulCnt)
{
    return (unsigned long)OSSemCreate((INT16U)ulCnt);
}
/*********************************************************************************************************
** Function name:           zySemDel
** Descriptions:            ɾ���ź���
** input parameters:        ulSem: zySemCreate����ֵ
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemDel (unsigned long ulSem)
{
    INT8U ucErr;                                                        /*  �������                    */

#if OS_ARG_CHK_EN == 0    
    /*
     *  �������
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    OSSemDel((OS_EVENT *)ulSem, OS_DEL_ALWAYS, &ucErr);
    
    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_ERR_DEL_ISR:
        return -ZY_NOT_OK;

    case OS_NO_ERR:
        return 0;
    
    default:
        return -ZY_NOT_OK;
    }
}
/*********************************************************************************************************
** Function name:           zySemAccept
** Descriptions:            �޵ȴ�����ź���
** input parameters:        ulSem: zySemCreate����ֵ
** output parameters:       none
** Returned value:          >=0:   �ź�����ǰֵ
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemAccept (unsigned long ulSem)
{
#if OS_ARG_CHK_EN == 0    
    /*
     *  �������
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    return OSSemAccept((OS_EVENT *)ulSem);
}
/*********************************************************************************************************
** Function name:           zySemPend
** Descriptions:            �ȴ��ź���
** input parameters:        ulSem: zySemCreate����ֵ
**                          ulDly: ��ȴ�ʱ��,����Ϊ��λ
** output parameters:       none
** Returned value:          >=0:   �ź�����ǰֵ
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemPend (unsigned long ulSem, INT32U ulDly)
{
    INT8U ucErr;                                                        /*  �������                    */
    
#if OS_ARG_CHK_EN == 0    
    /*
     *  �������
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    OSSemPend((OS_EVENT *)ulSem, (INT16U)((ulDly * OS_TICKS_PER_SEC + 999) / 1000), &ucErr);
    
    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_TIMEOUT:
        return -ZY_TIME_OUT;

    case OS_NO_ERR:
        return 0;
    
    default:
        return -ZY_NOT_OK;
    }
}
/*********************************************************************************************************
** Function name:           zySemPost
** Descriptions:            �����ź���
** input parameters:        ulSem: zySemCreate����ֵ
**                          ucMod: ����ģʽ,һ��Ϊ0
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zySemPost (unsigned long ulSem, INT8U ucMod)
{
#if OS_ARG_CHK_EN == 0    
    /*
     *  �������
     */
    if (ulSem == 0) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    switch (OSSemPost((OS_EVENT *)ulSem)) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_SEM_OVF:
        return -ZY_NOT_OK;

    case OS_NO_ERR:
        return 0;
    
    default:
        return -ZY_NOT_OK;
    }
}
/*********************************************************************************************************
** Function name:           zyMsgCreate
** Descriptions:            ������Ϣͨ��
** input parameters:        ulMsgs:    ���Ի�����Ϣ��Ŀ
**                          ulMsgSize: ��Ϣ��С
** output parameters:       none
** Returned value:          ��Ϣͨ����ʶ
**                          0: ���ɹ�
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
unsigned long zyMsgCreate (INT32S ulMsgs, INT32U ulMsgSize)
{
    INT32U       ulSize;                                                /*  ��Ҫ���ڴ�ռ�              */
    INT8U        ucErr;                                                 /*  �������                    */
    __ZY_IF_MSG *pimTmp1;
    INT8U       *pucTmp2;
    
    /*
     *  ����У��
     */
    if (ulMsgs == 0) {
        return 0;
    }

    if (ulMsgSize == 0) {
        return 0;
    }
    
    /*
     *  ����洢�ռ�
     */
    ulSize  = sizeof(__ZY_IF_MSG) + ulMsgs * sizeof(void *) + ulMsgs * ulMsgSize;
    pucTmp2 = (INT8U *)zyHeapMalloc(ulSize);
    if (pucTmp2 == NULL) {
        return 0;
    }

    pimTmp1 = (__ZY_IF_MSG  *)pucTmp2;

    /*
     *  ������Ϣ����
     */
    pimTmp1->poeQ = OSQCreate((void **)(pucTmp2 + sizeof(__ZY_IF_MSG)), (INT16U)ulMsgs);
    if (pimTmp1->poeQ == NULL) {
        zyHeapFree(pucTmp2);
        return 0;
    }
    
    /*
     *  �����ڴ����Ԫ
     */
    pimTmp1->pomMsg = OSMemCreate((void *)(pucTmp2 + sizeof(__ZY_IF_MSG) * ulMsgs),
                                  (INT16U)ulMsgs,
                                  ulMsgSize,
                                  &ucErr
                                  );


    if (pimTmp1->pomMsg == NULL) {
        OSQDel(pimTmp1->poeQ, OS_DEL_ALWAYS, &ucErr);
        zyHeapFree(pucTmp2);
        return 0;
    }
    
    pimTmp1->ulMsgSize = ulMsgSize;

    return (unsigned long)pimTmp1;
}
/*********************************************************************************************************
** Function name:           zyMsgDel
** Descriptions:            ɾ����Ϣͨ��
** input parameters:        ulMsg:  zyMsgCreate����ֵ
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgDel (unsigned long ulMsg)
{
    __ZY_IF_MSG *pimThis;                                               /*  ��Ϣͨ����Ϣ                */
    INT8U        ucErr;                                                 /*  �������                    */

    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
    
    pimThis = (__ZY_IF_MSG *)ulMsg;

    if (pimThis->poeQ != NULL) {
        OSQDel(pimThis->poeQ, OS_DEL_ALWAYS, &ucErr);
    }
    zyHeapFree((void *)pimThis);
    return -ZY_NOT_OK;
}
/*********************************************************************************************************
** Function name:           zyMsgAccept
** Descriptions:            �޵ȴ������Ϣ
** input parameters:        ulMsg:  zyMsgCreate����ֵ
** output parameters:       pucMsg: �����õ���Ϣ
** Returned value:          ZY_OK:  �ɹ�
**                          ����:   ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgAccept (unsigned long ulMsg, INT8U *pucMsgRt)
{
    __ZY_IF_MSG *pimThis;                                               /*  ��Ϣͨ����Ϣ                */
    void        *pvTmp;

    pimThis = (__ZY_IF_MSG *)ulMsg;

    /*
     *  �������
     */
    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
#if OS_ARG_CHK_EN == 0    
    if (pimThis->poeQ == NULL) {
        return -ZY_PARAMETER_ERR;
    }
    if (pimThis->pomMsg == NULL) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    pvTmp = OSQAccept(pimThis->poeQ);
    if (pvTmp == NULL) {
        return -ZY_NOT_OK;
    }

    memcpy(pucMsgRt, pvTmp, (size_t)(pimThis->ulMsgSize));              /*  ��������                    */
    OSMemPut(pimThis->pomMsg, pvTmp);                                   /*  �黹������                  */
    
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyMsgPend
** Descriptions:            �ȴ���Ϣ
** input parameters:        ulMsg:  zyMsgCreate����ֵ
**                          ulDly: ��ȴ�ʱ��,����Ϊ��λ
** output parameters:       pucMsgRt: �����õ���Ϣ
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgPend (unsigned long ulMsg, INT8U *pucMsgRt, INT32U ulDly)
{
    __ZY_IF_MSG *pimThis;                                               /*  ��Ϣͨ����Ϣ                */
    INT8U        ucErr;                                                 /*  �������                    */
    void        *pvTmp;

    pimThis = (__ZY_IF_MSG *)ulMsg;

    /*
     *  �������
     */
    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
#if OS_ARG_CHK_EN == 0    
    if (pimThis->poeQ == NULL) {
        return -ZY_PARAMETER_ERR;
    }
    if (pimThis->pomMsg == NULL) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    pvTmp = OSQPend(pimThis->poeQ, (INT16U)((ulDly * OS_TICKS_PER_SEC + 999) / 1000), &ucErr);

    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_TIMEOUT:
        return -ZY_TIME_OUT;

    case OS_NO_ERR:
        break;
    
    default:
        return -ZY_NOT_OK;
    }

    memcpy(pucMsgRt, pvTmp, (size_t)(pimThis->ulMsgSize));              /*  ��������                    */
    OSMemPut(pimThis->pomMsg, pvTmp);                                   /*  �黹������                  */
    
    return ZY_OK;
}
/*********************************************************************************************************
** Function name:           zyMsgPost
** Descriptions:            ������Ϣ
** input parameters:        ulMsg:      zyMsgCreate����ֵ
**                          pucMsgSend: Ҫ���͵���Ϣ
**                          ucMod:      ����ģʽ,һ��Ϊ0
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
** Created by:              Chenmingji
** Created Date:            2009-07-23
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
*********************************************************************************************************/
INT32S zyMsgPost (unsigned long ulMsg, INT8U *pucMsgSend, INT8U ucMod)
{
    __ZY_IF_MSG *pimThis;                                               /*  ��Ϣͨ����Ϣ                */
    INT8U        ucErr;                                                 /*  �������                    */
    void        *pvTmp;

    pimThis = (__ZY_IF_MSG *)ulMsg;

    /*
     *  �������
     */
    if (ulMsg == 0) {
        return -ZY_PARAMETER_ERR;
    }
#if OS_ARG_CHK_EN == 0    
    if (pimThis->poeQ == NULL) {
        return -ZY_PARAMETER_ERR;
    }
    if (pimThis->pomMsg == NULL) {
        return -ZY_PARAMETER_ERR;
    }
#endif                                                                  /*  OS_ARG_CHK_EN               */

    /*
     *  ���뻺����
     */
    pvTmp = OSMemGet(pimThis->pomMsg, &ucErr);

    switch (ucErr) {

    case OS_MEM_INVALID_PMEM:
        return -ZY_NO_FIND_OBJECT;

    case OS_MEM_NO_FREE_BLKS:
        return -ZY_NO_MEMORY;

    case OS_NO_ERR:
        break;
    
    default:
        return -ZY_NOT_OK;
    }

    /*
     *  ������Ϣ
     */
    memcpy(pvTmp, pucMsgSend, (size_t)(pimThis->ulMsgSize));            /*  ��������                    */
    ucErr = OSQPostOpt(pimThis->poeQ, pvTmp, ucMod);                    /*  ������Ϣ�洢λ��            */
    
    if (ucErr == OS_NO_ERR) {
        return ZY_OK;
    }

    OSMemPut(pimThis->pomMsg, pvTmp);                                   /*  �黹������                  */

    switch (ucErr) {
    
    case OS_ERR_PEVENT_NULL:
        return -ZY_PARAMETER_ERR;

    case OS_ERR_EVENT_TYPE:
        return -ZY_NO_FIND_OBJECT;

    case OS_Q_FULL:
        return -ZY_NO_MEMORY;

    default:
        return -ZY_NOT_OK;
    }
}
/**************************************End Of File*******************************************************/
