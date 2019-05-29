#include "main.h"

/*
******************************************************************************
                                      �������ȼ�
******************************************************************************
*/
#define LED0_PRIO 5
#define LED1_PRIO 5

/*
******************************************************************************
                                      ����ջ��С
******************************************************************************
*/
#define LED0_STACK_SIZE 128
#define LED1_STACK_SIZE 128

/*
******************************************************************************
                                      ����ʱ��Ƭ
******************************************************************************
*/
#define LED0_TIMESLICE 5
#define LED1_TIMESLICE 5
 

/* ��ʱ���Ŀ��ƿ� */
static rt_timer_t timer1;

rt_sem_t sem = RT_NULL;/* ָ���ź�����ָ�� */

rt_mutex_t mutex = RT_NULL;/* ���������� */

rt_event_t event = RT_NULL;/* ָ���¼��ĵ�ַָ�� */



/* ��ʱ��1��ʱ���� */
void timeout1(void* parameter)
{
    static uint32_t count = 0;
    count++;
    printf("count = %d\r\n",count);
}


/*
******************************************************************************
                                  ָ���߳̿��ƿ��ָ��
******************************************************************************
*/
rt_thread_t led0 = RT_NULL;
rt_thread_t led1 = RT_NULL;


/* �û�Ӧ����� */
int rtt_init(void)
{
    /* ������ʱ��1 */
    timer1 = rt_timer_create("timer1", /* ��ʱ�������� timer1 */
            timeout1, 				   /* ��ʱʱ�ص��Ĵ����� */
            RT_NULL, 				   /* ��ʱ��������ڲ��� */
            1000, 					   /* ��ʱ���ȣ���OS TickΪ��λ����10��OS Tick */
            RT_TIMER_FLAG_PERIODIC);   /* �����Զ�ʱ�� */

    /* ������ʱ�� */
    if (timer1 != RT_NULL) rt_timer_start(timer1);

    /* ����һ���ź�������ʼֵ��1 */
    sem = rt_sem_create("sem", 1, RT_IPC_FLAG_FIFO);

    /* ����һ�������� */
    mutex = rt_mutex_create("mutex", RT_IPC_FLAG_FIFO);
    
    /* �����¼� */
    event = rt_event_create( "event", RT_IPC_FLAG_FIFO);
    
    /* �����߳�1 */
    led0 = rt_thread_create("t1",led0_thread, (void*)1,
                            LED0_STACK_SIZE, LED0_PRIO, LED0_TIMESLICE);
    if (led0 != RT_NULL)
        rt_thread_startup(led0);
    else
        return -1;
    
    /* �����߳�2 */
    led1 = rt_thread_create("t2",led1_thread, (void*)2,
                            LED1_STACK_SIZE, LED1_PRIO, LED1_TIMESLICE);
    if (led1 != RT_NULL)
        rt_thread_startup(led1);
    else
        return -1;
    return 0;
}
