#include "main.h"

extern OS_MUTEX TEST_MUTEX;  //����һ�������ź���
extern OS_SEM SYNC_SEM;
extern OS_TCB SEM1TaskTCB;  

uint8_t share_res[30]; //������Դ��

void sem0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSTaskSemPost(&SEM1TaskTCB, OS_OPT_POST_NONE,&err); // ʹ��ϵͳ�ڽ��ź������������ź���
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );//��ʱ
    }
}

void sem1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,0,&err); // ���������ڽ����ź���
        printf("�ɹ����������ź���\r\n");
        OSSched();  
    }
}

#if 0
void sem0_task(void *p_arg)
{
    OS_ERR err;
    uint8_t task0_str[]="sem0 Running!";
    p_arg = p_arg;
    while(1)
    {
        OSMutexPend (&TEST_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);// ���󻥳��ź���
        memcpy(share_res,task0_str,sizeof(task0_str));
        printf("%s",share_res);
        OSMutexPost(&TEST_MUTEX,OS_OPT_POST_NONE,&err);// �ͷŻ����ź���
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );//��ʱ
    }
}


void sem1_task(void *p_arg)
{
    OS_ERR err;
    uint8_t task1_str[]="sem1 Running!";
    p_arg = p_arg;
    while(1)
    {
        OSMutexPend (&TEST_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);// ���󻥳��ź���
        memcpy(share_res,task1_str,sizeof(task1_str));
        printf("%s",share_res);
        OSMutexPost(&TEST_MUTEX,OS_OPT_POST_NONE,&err);// �ͷŻ����ź���
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );//��ʱ
    }
}
#endif

#if 0
void sem0_task(void *p_arg)
{
    OS_ERR err;
    uint8_t task0_str[]="sem0 Running!";
    p_arg = p_arg;
    while(1)
    {
        printf("sem0_task:\r\n");
        OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); // �����ź���
        memcpy(share_res,task0_str,sizeof(task0_str));    // ������Դ����������
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //�������ʱ1000��ʱ�ӽ��ģ�1s��
        OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);           // �ͷ��ź���
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //�������ʱ1000��ʱ�ӽ��ģ�1s��
    }
}

void sem1_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    uint8_t task1_str[]="sem1 Running!";
    while(1)
    {
        printf("sem1_task:\r\n");
        OSSemPend(&MY_SEM,0,OS_OPT_PEND_BLOCKING,0,&err); // �����ź���
        memcpy(share_res,task1_str,sizeof(task1_str));    // ������Դ����������
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //�������ʱ1000��ʱ�ӽ��ģ�1s��
        OSSemPost (&MY_SEM,OS_OPT_POST_1,&err);           // �ͷ��ź���
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );          //�������ʱ1000��ʱ�ӽ��ģ�1s��
    }
}
#endif

#if 0
void sem0_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;
    while(1)
    {
        OSSemPost(&SYNC_SEM,OS_OPT_POST_1,&err);//�����ź���
        OSTimeDly (100, OS_OPT_TIME_DLY, &err );//��ʱ
    }
}

void sem1_task(void *p_arg)
{
    OS_ERR err;
    uint16_t num = 0;
    p_arg = p_arg;
    while(1)
    {
        OSSemPend(&SYNC_SEM,0,OS_OPT_PEND_BLOCKING,0,&err);// �����ź���
        num++;
        printf("�����ź��������� %d\r\n",num);
        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );          //��ʱ
    }
}
#endif



