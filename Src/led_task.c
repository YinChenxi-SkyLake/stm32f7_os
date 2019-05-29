#include "main.h"

UINT16 TimerId;/*  ���嶨ʱ�������ID �� */
UINT32 BinarySemId;/*  �����ֵ�ź����ľ�� */
UINT32 CountSemId;/*  ��������ź����ľ�� */
UINT32 MutexId;/*  ���� ������ �ľ�� */
EVENT_CB_S EventGroup_CB;/*  �����¼���־��Ŀ��ƿ� */
UINT32 QueueId;/*  ������Ϣ���еľ�� */

static void Timer_Callback(UINT32 arg)
{
    UINT32 tick_num1;

    tick_num1 = (UINT32)LOS_TickCountGet();  /*  ��ȡ�δ�ʱ���ļ���ֵ */
}

void led0_task(void)
{
    UINT32 send_data1 = 1;
    UINT32 *p_mem = NULL;
    
    LOS_SwtmrCreate(1000,  /*  �����ʱ���Ķ�ʱʱ�䣨ms ��*/
                    LOS_SWTMR_MODE_PERIOD,/*  �����ʱ��ģʽ  ����ģʽ */
                    (SWTMR_PROC_FUNC)Timer_Callback,/*  �����ʱ���Ļص����� */
                    &TimerId,  /*  �����ʱ���� id */
                    0);
    
    LOS_SwtmrStart(TimerId);
    
    LOS_BinarySemCreate(1,&BinarySemId);/*  ����һ����ֵ�ź���*/
    
    LOS_SemCreate (5,&CountSemId);/*  ����һ�������ź�������ʼ������ֵΪ 5*/
    
    LOS_MuxCreate(&MutexId);/*  ����һ�� ������*/
    
    LOS_EventInit(&EventGroup_CB);/*  ����һ���¼���־��*/

    /*  ����һ�����Զ���*/
    LOS_QueueCreate("Test_Queue",  /*  ���е����� */
                    16,  /*  ���еĳ��� */
                    &QueueId, /*  ���е� ID( ���) */
                    0,  /*  ����ģʽ���ٷ���ʱδʹ�� */
                    16); /*  �ڵ��С����λΪ�ֽ� */
    
    while(1)
    {
        LOS_SemPost( BinarySemId ); // ������ֵ�ź���
        
        LOS_SemPost(CountSemId);/* �ͷ�һ�������ź�����LiteOS  �ļ����ź�������һֱ�ͷ�*/
        
        LOS_MuxPost( MutexId ); // ���� ������
        
        LOS_EventWrite(&EventGroup_CB,0x01);/*  ����һ���¼� 1 */
        
        /* ������д�루���ͣ��������У��ȴ�ʱ��Ϊ 0 */
        LOS_QueueWrite(QueueId,  /* д�루���ͣ����е� ID(���)  */
                       &send_data1, /* д�루���ͣ������� */
                       sizeof(send_data1), /* ���ݵĳ��� */
                       0); 
        
        p_mem = (UINT32*)LOS_MemAlloc(m_aucSysMem0,20);
                       
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        LOS_TaskDelay(500);
    }
}


void led1_task(void)
{
    UINT32 *r_queue;
    while(1)
    {
        LOS_SemPend( BinarySemId, LOS_WAIT_FOREVER ); // ��ȡ��ֵ�ź��� xSemaphore, û��ȡ����һֱ�ȴ�
        
        LOS_SemPend ( CountSemId,0);/*  ��ȡһ�������ź������ȴ�ʱ�� 0 */
        
        LOS_MuxPend( MutexId, LOS_WAIT_FOREVER );// ��ȡ ������, û��ȡ����һֱ�ȴ�
        
        /*  �ȴ��¼���־�� */
        LOS_EventRead(&EventGroup_CB,   // �¼���־�����
                      0x01,             // �ȴ��������Ȥ���¼�
                      LOS_WAITMODE_AND, // �ȴ���λ������λ
                      LOS_WAIT_FOREVER );// �����޵ȴ�
        LOS_EventClear(&EventGroup_CB,~0x01); // ����¼���־
        
        LOS_QueueRead(QueueId,           /*  ��ȡ�����գ����е� ID( ���) */
                      &r_queue,          /*  ��ȡ�����գ������ݱ���λ�� */
                      16,                /*  ��ȡ�����գ������ݵĳ���*/
                      LOS_WAIT_FOREVER); /*  �ȴ�ʱ�䣺һֱ�� */
        
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_1);
        
        LOS_TaskDelay(500);
    }
}





