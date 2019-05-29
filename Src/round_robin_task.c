#include "main.h"

extern OS_Q msg_q; //������Ϣ����

void RR1_task(void *p_arg)
{
    OS_ERR  err;
    p_arg = p_arg;
    while(1)
    {
        /* ������Ϣ����Ϣ���� queue */
        OSQPost ((OS_Q *)&msg_q,                    //��Ϣ����ָ��
                (void *)"msg_q test",               //Ҫ���͵����ݵ�ָ�룬���ڴ���׵�ַͨ�����С����ͳ�ȥ��
                (OS_MSG_SIZE )sizeof ("msg_q test"),//�����ֽڴ�С
                (OS_OPT )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //�Ƚ��ȳ��ͷ�����ȫ���������ʽ
                (OS_ERR *)&err);                         //���ش�������
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );   //�������ʱ1000��ʱ�ӽ��ģ�1s��
    }
}

void RR2_task(void *p_arg)
{
    OS_ERR  err;
    CPU_SR_ALLOC();
    p_arg = p_arg;
    OS_MSG_SIZE msg_size;
    //����һ���ֲ����������ڱ�����ж�ǰ�� CPU ״̬�Ĵ���
    char * pMsg;
    while(1)
    {
        /* ������Ϣ���� queue ����Ϣ */
        pMsg = OSQPend ((OS_Q *)&msg_q, //��Ϣ����ָ��
                        (OS_TICK )100, //�ȴ�ʱ��
                        (OS_OPT )OS_OPT_PEND_NON_BLOCKING,//���û�л�ȡ���ź����͵ȴ�
                        (OS_MSG_SIZE *)&msg_size, //��ȡ��Ϣ���ֽڴ�С
                        (CPU_TS *)0, //��ȡ������ʱ��ʱ���
                        (OS_ERR *)&err); //���ش���
        
        if ( err == OS_ERR_NONE ) //������ճɹ�
        {
            OS_CRITICAL_ENTER(); //�����ٽ��
            printf ( "RR2_task��%d �ֽڣ����ݣ�%s\r\n", msg_size, pMsg );
            OS_CRITICAL_EXIT();
        }
        OSTimeDly (1, OS_OPT_TIME_DLY, &err );
    }
}

