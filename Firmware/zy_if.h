/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**
**                                 http://www.embedtools.com
**--------------File Info---------------------------------------------------------------------------------
** File name:               zy_if.h
** Latest modified Date:    2009-07-23
** Latest Version:          1.00
** Descriptions:            �����ӿں���,��������Ȩģʽ����
**--------------------------------------------------------------------------------------------------------
** Created by:              Chenmingji
** Created date:            2009-07-23
** Version:                 1.0
** Descriptions:            The original version
*********************************************************************************************************/
#ifndef __ZY_IF_H
#define __ZY_IF_H

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  ����ֵ����
*********************************************************************************************************/
#define     ZY_OK               0                                       /*  �����ɹ�                    */
#define     ZY_NOT_OK           1                                       /*  ����ʧ��                    */
#define     ZY_PARAMETER_ERR    2                                       /*  ��������                    */
#define     ZY_NO_FIND_FUNCTION 6                                       /*  û�з���ָ������            */
#define     ZY_NO_MEMORY        12                                      /*  �ڴ治��                    */
#define     ZY_TIME_OUT         13                                      /*  ��ʱ                        */
#define     ZY_NO_FIND_OBJECT   16                                      /*  û�з��ֶ���                */

/*********************************************************************************************************
  64λ������ض���
*********************************************************************************************************/
typedef long long   INT64S;                                             /*  64λ�з�����                */

#define zyLlAdd(a, b, c) a = (INT64S)(b) + (INT64S)(c)                  /*  64λ�з��żӷ�a=b+c         */
#define zyLlSub(a, b, c) a = (INT64S)(b) - (INT64S)(c)                  /*  64λ�з��ż���a=b-c         */
#define zyLlMul(a, b, c) a = (INT64S)(b) * (INT64S)(c)                  /*  64λ�з��ų˷�a=b*c         */
#define zyLlDiv(a, b, c) a = (INT64S)(b) / (INT64S)(c)                  /*  64λ�з��ų���a=b/c         */
#define zyLlMod(a, b, c) a = (INT64S)(b) % (INT64S)(c)                  /*  64λ�з��ų���a=b/c         */
#define zyLlSet(a, b)    a = (INT64S)(b)                                /*  64λ�з��Ÿ�ֵ              */
#define zyLlIsLess(a, b) ((INT64S)(a) < (INT64S)(b))                    /*  64λ�з���С���ж�          */
#define zyLlSet32(a, b)  a = (INT32U)(b)                                /*  64λ�з��Ÿ�ֵ��32λ�޷�����*/
#define zyLlSet32s(a, b) a = (INT32S)(b)                                /*  64λ�з��Ÿ�ֵ��32λ�з�����*/

/*********************************************************************************************************
  ��λģʽ
*********************************************************************************************************/
#define ZY_POWER_RESET      0                                           /*  �ϵ縴λ                    */
#define ZY_HARD_RESET       1                                           /*  Ӳ����λ                    */
#define ZY_SOFT_RESET       2                                           /*  �����λ                    */

/*********************************************************************************************************
** Function name:           zyIfInit
** Descriptions:            �ӿڳ�ʼ��
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIfInit(void);

/*********************************************************************************************************
** Function name:           zyReset
** Descriptions:            ϵͳ��λ
** input parameters:        uiMode: ZY_POWER_RESET: �ϵ縴λ
**                                  ZY_HARD_RESET:  Ӳ����λ
**                                  ZY_SOFT_RESET:  �����λ
**                                  ����:           ��ϵͳ���
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
extern void zyReset(unsigned int uiMode);

/*********************************************************************************************************
** Function name:           zyIrqDisable
** Descriptions:            ��ֹ�ж�
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIrqDisable(void);

/*********************************************************************************************************
** Function name:           zyIrqEnable
** Descriptions:            �����ж�
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIrqEnable(void);

/*********************************************************************************************************
** Function name:           zyIsrSet
** Descriptions:            �����жϷ������
** input parameters:        uiChannel:  �ж�ͨ����
**                          ulFunction: �жϷ�������ַ
**                          uiPrio:     �ж����ȼ�
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrSet(unsigned int uiChannel, unsigned long ulFunction, unsigned int uiPrio);

/*********************************************************************************************************
** Function name:           zyIsrClr
** Descriptions:            ����жϷ������
** input parameters:        uiChannel:  �ж�ͨ����
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrClr(unsigned int uiChannel);

/*********************************************************************************************************
** Function name:           zyIsrDisable
** Descriptions:            ��ָֹ���ж�
** input parameters:        uiChannel:  �ж�ͨ����
** output parameters:       none
** Returned value:          zy_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrDisable(unsigned int uiChannel);

/*********************************************************************************************************
** Function name:           zyIsrEnable
** Descriptions:            ����ָ���ж�
** input parameters:        uiChannel:  �ж�ͨ����
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyIsrEnable(unsigned int uiChannel);

/*********************************************************************************************************
** Function name:           zyHeapMalloc
** Descriptions:            �ѷ����ڴ�
** input parameters:        ulSize: �ڴ��С
** output parameters:       none
** Returned value:          �ڴ��ַ,NULLΪ���ɹ�
*********************************************************************************************************/
extern void *zyHeapMalloc(INT32U ulSize);

/*********************************************************************************************************
** Function name:           zyHeapFree
** Descriptions:            ���ͷ��ڴ�
** input parameters:        pvPrt: Ҫ�ͷŵ��ڴ�
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyHeapFree(void *pvPrt);

/*********************************************************************************************************
** Function name:           zyThreadCreate
** Descriptions:            �����߳�
** input parameters:        pvThreedMain: �߳�������
**                          pvArg:        �߳����������
**                          uiPrio:       �߳����ȼ�
**                          ulSize:       ��ջ��С
** output parameters:       none
** Returned value:          �߳�ID,0Ϊ���ɹ�
*********************************************************************************************************/
extern unsigned long zyThreadCreate(void *pvThreedMain, void *pvArg, unsigned int uiPrio, INT32U ulSize);

/*********************************************************************************************************
** Function name:           zyThreadDel
** Descriptions:            ɾ���߳�
** input parameters:        ulThreadId: �߳�ID
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadDel(unsigned long ulThreadId);

/*********************************************************************************************************
** Function name:           zyThreadSuspendSelf
** Descriptions:            �����߳�����
** input parameters:        none
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadSuspendSelf(void);

/*********************************************************************************************************
** Function name:           zyThreadResume
** Descriptions:            �ָ��߳�
** input parameters:        ulThreadId: �߳�ID
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadResume(unsigned long ulThreadId);

/*********************************************************************************************************
** Function name:           zyThreadGetCurId
** Descriptions:            ��õ�ǰ�߳�ID
** input parameters:        none
** output parameters:       none
** Returned value:          �߳�ID
*********************************************************************************************************/
extern unsigned long zyThreadGetCurId(void);

/*********************************************************************************************************
** Function name:           zyThreadSuspendCheck
** Descriptions:            ����߳��Ƿ�ȷʵ������
** input parameters:        ulThreadId: �߳�ID
** output parameters:       none
** Returned value:          zy_OK:      ����
**                          -ZY_NOT_OK: δ����
**                          ��������:   ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadSuspendCheck(unsigned long ulThreadId);

/*********************************************************************************************************
** Function name:           zyThreadDelay
** Descriptions:            ��ǰ�߳���ʱ
** input parameters:        ulTimes: �Ժ���Ϊ��λ�ȴ�ʱ��
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyThreadDelay(unsigned long ulTimes);

/*********************************************************************************************************
** Function name:           zySemCreate
** Descriptions:            �����ź���
** input parameters:        ulCnt:  �ź�����ʼֵ
** output parameters:       none
** Returned value:          �ź�����ʶ
**                          0: ���ɹ�
*********************************************************************************************************/
extern unsigned long zySemCreate(INT32S ulCnt);

/*********************************************************************************************************
** Function name:           zySemDel
** Descriptions:            ɾ���ź���
** input parameters:        ulSem: zySemCreate����ֵ
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zySemDel(unsigned long ulSem);

/*********************************************************************************************************
** Function name:           zySemAccept
** Descriptions:            �޵ȴ�����ź���
** input parameters:        ulSem: zySemCreate����ֵ
** output parameters:       none
** Returned value:          >=0:   �ź�����ǰֵ
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zySemAccept(unsigned long ulSem);

/*********************************************************************************************************
** Function name:           zySemPend
** Descriptions:            �ȴ��ź���
** input parameters:        ulSem: zySemCreate����ֵ
**                          ulDly: ��ȴ�ʱ��,����Ϊ��λ
** output parameters:       none
** Returned value:          >=0:   �ź�����ǰֵ
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zySemPend(unsigned long ulSem, INT32U ulDly);

/*********************************************************************************************************
** Function name:           zySemPost
** Descriptions:            ����ָ���ж�
** input parameters:        ulSem: zySemCreate����ֵ
**                          ucMod: ����ģʽ,һ��Ϊ0
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zySemPost(unsigned long ulSem, INT8U ucMod);

/*********************************************************************************************************
** Function name:           zyMsgCreate
** Descriptions:            ������Ϣͨ��
** input parameters:        ulMsgs:    ���Ի�����Ϣ��Ŀ
**                          ulMsgSize: ��Ϣ��С
** output parameters:       none
** Returned value:          ��Ϣͨ����ʶ
**                          0: ���ɹ�
*********************************************************************************************************/
extern unsigned long zyMsgCreate(INT32S ulMsgs, INT32U ulMsgSize);

/*********************************************************************************************************
** Function name:           zyMsgDel
** Descriptions:            ɾ����Ϣͨ��
** input parameters:        ulMsg:  zyMsgCreate����ֵ
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgDel(unsigned long ulMsg);

/*********************************************************************************************************
** Function name:           zyMsgAccept
** Descriptions:            �޵ȴ������Ϣ
** input parameters:        ulMsg:  zyMsgCreate����ֵ
** output parameters:       pucMsg: �����õ���Ϣ
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgAccept(unsigned long ulMsg, INT8U *pucMsgRt);

/*********************************************************************************************************
** Function name:           zyMsgPend
** Descriptions:            �ȴ���Ϣ
** input parameters:        ulMsg:  zyMsgCreate����ֵ
**                          ulDly: ��ȴ�ʱ��,����Ϊ��λ
** output parameters:       pucMsgRt: �����õ���Ϣ
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgPend(unsigned long ulMsg, INT8U *pucMsgRt, INT32U ulDly);

/*********************************************************************************************************
** Function name:           zyMsgPost
** Descriptions:            ������Ϣ
** input parameters:        ulMsg:      zyMsgCreate����ֵ
**                          pucMsgSend: Ҫ���͵���Ϣ
**                          ucMod:      ����ģʽ,һ��Ϊ0
** output parameters:       none
** Returned value:          ZY_OK: �ɹ�
**                          ����:  ����,����ֵ�ο�zy_if.h
*********************************************************************************************************/
extern INT32S zyMsgPost(unsigned long ulMsg, INT8U *pucMsgSend, INT8U ucMod);

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  __ZY_IF_H                   */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
