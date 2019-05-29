#include "main.h"

CPU_TS             ts_start;       //ʱ�������
CPU_TS             ts_end; 

void TmrCallback (OS_TMR *p_tmr, void *p_arg) //�����ʱ��MyTmr�Ļص�����
{
    CPU_INT32U       cpu_clk_freq;
    
    printf ( "%s", ( char * ) p_arg );
    
    cpu_clk_freq = HAL_RCC_GetSysClockFreq();  //��ȡCPUʱ�ӣ�ʱ������Ը�ʱ�Ӽ���
    ts_end = OS_TS_GET() - ts_start;              //��ȡ��ʱ���ʱ�������CPUʱ�ӽ��м�����һ������ֵ��
    
    printf ( "\r\n��ʱ1s��ͨ��ʱ�����ö�ʱ%04d ms��\r\n", ts_end / (cpu_clk_freq/1000) );//����ʱʱ������� ms
    ts_start = OS_TS_GET();                            //��ȡ��ʱǰʱ���
}


void  tmr_task( void * p_arg )
{
    OS_ERR      err;
    OS_TMR      my_tmr;   //���������ʱ������
    (void)p_arg;

    /* ���������ʱ�� */
    OSTmrCreate ((OS_TMR              *)&my_tmr,             //�����ʱ������
                 (CPU_CHAR            *)"MySoftTimer",       //���������ʱ��
                 (OS_TICK              )10,                  //��ʱ����ʼֵ����10Hzʱ�����㣬��Ϊ1s
                 (OS_TICK              )10,                  //��ʱ����������ֵ����10Hzʱ�����㣬��Ϊ1s
                 (OS_OPT               )OS_OPT_TMR_PERIODIC, //�����Զ�ʱ
                 (OS_TMR_CALLBACK_PTR  )TmrCallback,         //�ص�����
                 (void                *)"Timer Over!",       //����ʵ�θ��ص�����
                 (OS_ERR              *)err);                //���ش�������

    /* ���������ʱ�� */
    OSTmrStart(&my_tmr, &err);
    ts_start = OS_TS_GET();                       //��ȡ��ʱǰʱ���
                 
    while (DEF_TRUE) {                            //�����壬ͨ��д��һ����ѭ��
        OSTimeDly ( 1000, OS_OPT_TIME_DLY, &err ); //��������������
    }
}




