#include "main.h"

OS_SEM SYNC_SEM;    //����һ���ź���
OS_MUTEX TEST_MUTEX;//����һ�������ź���
OS_FLAG_GRP flag_grp;
OS_Q msg_q;
OS_MEM mem;             //�����ڴ�������
uint8_t ucArray[3][20]; //�����ڴ������С
/*
*********************************************************************************************************
*                                         �������ȼ�
*********************************************************************************************************
*/
#define START_TASK_PRIO    3  //�������ȼ�
#define DS0_TASK_PRIO      4  //�������ȼ�
#define DS1_TASK_PRIO      5  //�������ȼ�
#define SEM0_TASK_PRIO     6  //�������ȼ�
#define SEM1_TASK_PRIO     7  //�������ȼ�
#define FLAG_GRP0_PRIO     8  //�������ȼ�
#define FLAG_GRP1_PRIO     9  //�������ȼ�
#define ROUND_ROBIN_TASK1  10  //ʱ��Ƭ��ת��������1
#define ROUND_ROBIN_TASK2  10  //ʱ��Ƭ��ת��������2
#define TMR_TASK_PRIO      11  //�����ʱ��

/*
*********************************************************************************************************
*                                         ������ƿ�TCB
*********************************************************************************************************
*/
OS_TCB StartTaskTCB;        
OS_TCB DS0TaskTCB;        
OS_TCB DS1TaskTCB;
OS_TCB SEM0TaskTCB;        
OS_TCB SEM1TaskTCB;  
OS_TCB FLAGGRP0TCB;        
OS_TCB FLAGGRP1TCB;  
OS_TCB RR1TaskTCB;
OS_TCB RR2TaskTCB;
OS_TCB TmrTaskTCB;
/*
*********************************************************************************************************
*                                         �����ջ
*********************************************************************************************************
*/
#define START_STK_SIZE 128 
CPU_STK START_TASK_STK[START_STK_SIZE];//�����ջ 

#define DS0_STK_SIZE 64  
CPU_STK DS0_TASK_STK[DS0_STK_SIZE];//�����ջ 

#define DS1_STK_SIZE 64 
CPU_STK DS1_TASK_STK[DS1_STK_SIZE];//�����ջ 

#define SEM0_STK_SIZE 64 
CPU_STK SEM0_TASK_STK[SEM0_STK_SIZE];//�����ջ 

#define SEM1_STK_SIZE 64  
CPU_STK SEM1_TASK_STK[SEM1_STK_SIZE];//�����ջ 

#define FLAG_GRP0_STK_SIZE 64 
CPU_STK FLAG_GRP0_STK[FLAG_GRP0_STK_SIZE];//�����ջ 

#define FLAG_GRP1_STK_SIZE 64  
CPU_STK FLAG_GRP1_STK[FLAG_GRP1_STK_SIZE];//�����ջ 

#define RR1_STK_SIZE 64 
CPU_STK RR1_TASK_STK[RR1_STK_SIZE];//�����ջ 

#define RR2_STK_SIZE 64  
CPU_STK RR2_TASK_STK[RR2_STK_SIZE];//�����ջ 

#define TMR_STK_SIZE 64  
CPU_STK TMR_TASK_STK[TMR_STK_SIZE];//�����ջ 
/*
*********************************************************************************************************
*                                            ����ԭ��
*********************************************************************************************************
*/
void start_task(void *p_arg);//������


void ucos_init(void)
{
    OS_ERR err;
    CPU_SR_ALLOC();//����cpu_sr����
    
    CPU_Init();    //��ʼ�� CPU �����ʱ��������ж�ʱ���������������
    OSInit(&err);  //��ʼ�� UCOSIII
    
    OS_CRITICAL_ENTER(); //�����ٽ���
    
    //������ʼ����
    OSTaskCreate((OS_TCB*  )&StartTaskTCB, //������ƿ�
        (CPU_CHAR* )"start task",          //��������
        (OS_TASK_PTR )start_task,          //������
        (void* )0,                         //���ݸ��������Ĳ���
        (OS_PRIO  )START_TASK_PRIO,        //�������ȼ�
        (CPU_STK* )&START_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )START_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )START_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )0,                    //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                       //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                         //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
    OS_CRITICAL_EXIT();                    //�˳��ٽ���
    OSStart(&err); //���� UCOSIII
}


//��ʼ������
void start_task(void *p_arg)
{
    OS_ERR err;
    p_arg = p_arg;

#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err); //ͳ������
#endif
#ifdef CPU_CFG_INT_DIS_MEAS_EN  
    CPU_IntDisMeasMaxCurReset();//��λ�����㣩��ǰ�����ж�ʱ��
#endif
#if OS_CFG_SCHED_ROUND_ROBIN_EN //��ʹ��ʱ��Ƭ��ת��ʱ��
    //ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����10ms
    OSSchedRoundRobinCfg(DEF_ENABLED,5,&err);
#endif
    /* ������Ϣ���� queue */
    OSQCreate((OS_Q *)&msg_q, //ָ����Ϣ���е�ָ��
              (CPU_CHAR *)"Queue For Test", //���е�����
              (OS_MSG_QTY )20, //���ɴ����Ϣ����Ŀ
              (OS_ERR *)&err); //���ش�������
    
    //����һ���ź���
    OSSemCreate ((OS_SEM*  )&SYNC_SEM, //ָ���ź���
                (CPU_CHAR*  )"MY_SEM", //�ź�������
                (OS_SEM_CTR )1, //�ź���ֵΪ 1
                (OS_ERR* )&err);
                
                
    //����һ�������ź���
    OSMutexCreate((OS_MUTEX* )&TEST_MUTEX,
                  (CPU_CHAR* )"TEST_MUTEX",
                  (OS_ERR* )&err);
    
    //�����¼���־��
    OSFlagCreate ((OS_FLAG_GRP *)&flag_grp, //ָ���¼���־���ָ��
                  (CPU_CHAR *)"FLAG For Test", //�¼���־�������
                  (OS_FLAGS )0,    //�¼���־��ĳ�ʼֵ
                  (OS_ERR *)&err); //���ش�������
    
    /*  �����ڴ������� mem */
    OSMemCreate ((OS_MEM *)&mem, // ָ���ڴ�������
                (CPU_CHAR *)"Mem For Test", // �����ڴ�������
                (void *)ucArray, // �ڴ�������׵�ַ
                (OS_MEM_QTY )3, // �ڴ�������ڴ����Ŀ
                (OS_MEM_SIZE )20, // �ڴ����ֽ���Ŀ
                (OS_ERR *)&err); //

    //����LED0����
    OSTaskCreate((OS_TCB*  )&DS0TaskTCB, //������ƿ�
        (CPU_CHAR* )"led0 task",          //��������
        (OS_TASK_PTR )led0_task,          //������
        (void* )0,                         //���ݸ��������Ĳ���
        (OS_PRIO  )DS0_TASK_PRIO,        //�������ȼ�
        (CPU_STK* )&DS0_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )DS0_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )DS0_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                    //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                       //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                         //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
    
    //����LED1����
    OSTaskCreate((OS_TCB*  )&DS1TaskTCB, //������ƿ�
        (CPU_CHAR* )"led1 task",         //��������
        (OS_TASK_PTR )led1_task,         //������
        (void* )0,                       //���ݸ��������Ĳ���
        (OS_PRIO  )DS1_TASK_PRIO,        //�������ȼ�
        (CPU_STK* )&DS1_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )DS1_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )DS1_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                  //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                     //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                       //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
     
    //����SEM0����
    OSTaskCreate((OS_TCB*  )&SEM0TaskTCB, //������ƿ�
        (CPU_CHAR* )"sem0 task",          //��������
        (OS_TASK_PTR )sem0_task,          //������
        (void* )0,                         //���ݸ��������Ĳ���
        (OS_PRIO  )SEM0_TASK_PRIO,        //�������ȼ�
        (CPU_STK* )&SEM0_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )SEM0_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )SEM0_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                    //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                       //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                         //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
    
    //����SEM1����
    OSTaskCreate((OS_TCB*  )&SEM1TaskTCB, //������ƿ�
        (CPU_CHAR* )"sem1 task",         //��������
        (OS_TASK_PTR )sem1_task,         //������
        (void* )0,                       //���ݸ��������Ĳ���
        (OS_PRIO  )SEM1_TASK_PRIO,        //�������ȼ�
        (CPU_STK* )&SEM1_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )SEM1_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )SEM1_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                  //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                     //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                       //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
        
    
    //����FLAG_GRP0����
    OSTaskCreate((OS_TCB*  )&FLAGGRP0TCB, //������ƿ�
        (CPU_CHAR* )"flag grp0 task",     //��������
        (OS_TASK_PTR )flag_grp0_task,          //������
        (void* )0,                         //���ݸ��������Ĳ���
        (OS_PRIO  )FLAG_GRP0_PRIO,        //�������ȼ�
        (CPU_STK* )&FLAG_GRP0_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )FLAG_GRP0_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )FLAG_GRP0_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                    //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                       //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                         //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ

    //����FLAG_GRP1����
    OSTaskCreate((OS_TCB*  )&FLAGGRP1TCB, //������ƿ�
        (CPU_CHAR* )"flag grp1 task",     //��������
        (OS_TASK_PTR )flag_grp1_task,          //������
        (void* )0,                         //���ݸ��������Ĳ���
        (OS_PRIO  )FLAG_GRP1_PRIO,        //�������ȼ�
        (CPU_STK* )&FLAG_GRP1_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )FLAG_GRP1_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )FLAG_GRP1_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                    //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                       //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                         //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
        
    //����ʱ��Ƭ��ת����1
    OSTaskCreate((OS_TCB*  )&RR1TaskTCB, //������ƿ�
        (CPU_CHAR* )"RR1 task",          //��������
        (OS_TASK_PTR )RR1_task,          //������
        (void* )0,                       //���ݸ��������Ĳ���
        (OS_PRIO  )ROUND_ROBIN_TASK1,    //�������ȼ�
        (CPU_STK* )&RR1_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )RR1_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )RR1_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                  //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )10,                    //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                       //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                 //��Ÿú�������ʱ�ķ���ֵ
   
    //����ʱ��Ƭ��ת����2    
    OSTaskCreate((OS_TCB*  )&RR2TaskTCB, //������ƿ�
        (CPU_CHAR* )"RR1 task",          //��������
        (OS_TASK_PTR )RR2_task,          //������
        (void* )0,                       //���ݸ��������Ĳ���
        (OS_PRIO  )ROUND_ROBIN_TASK2,    //�������ȼ�
        (CPU_STK* )&RR2_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )RR2_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )RR2_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                  //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )10,                    //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                       //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                 //��Ÿú�������ʱ�ķ���ֵ

    //���������ʱ������
    OSTaskCreate((OS_TCB*  )&TmrTaskTCB, //������ƿ�
        (CPU_CHAR* )"tmr task",         //��������
        (OS_TASK_PTR )tmr_task,         //������
        (void* )0,                       //���ݸ��������Ĳ���
        (OS_PRIO  )TMR_TASK_PRIO,        //�������ȼ�
        (CPU_STK* )&TMR_TASK_STK[0],     //�����ջ����ַ
        (CPU_STK_SIZE )TMR_STK_SIZE/10,  //�����ջ�����λ
        (CPU_STK_SIZE )TMR_STK_SIZE,     //�����ջ��С
        (OS_MSG_QTY )5,                  //�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ 0 ʱ��ֹ������Ϣ
        (OS_TICK )0,                     //��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ 0 ʱΪĬ�ϳ��ȡ�
        (void* )0,                       //�û�����Ĵ洢��
        (OS_OPT  ) OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR,
        (OS_ERR* )&err);                   //��Ÿú�������ʱ�ķ���ֵ
        
    OSTaskDel(0, &err ); //ɾ����ʼ������������������
}




