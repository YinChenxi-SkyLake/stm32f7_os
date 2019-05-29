#include "main.h"

SemaphoreHandle_t  BinarySem = NULL;
SemaphoreHandle_t  CountSem = NULL;
SemaphoreHandle_t  MuxSem = NULL;
EventGroupHandle_t Event = NULL;
QueueHandle_t      Queue = NULL;

TimerHandle_t SoftTmr = NULL; /*  �����ʱ����� */

TimerHandle_t SoftTmrStatic = NULL;//��̬�����ʱ��
StaticTimer_t xTimerBuffers;

void SoftTmr_Callback(TimerHandle_t xTimer)
{
    
}

void led0_task(void* arg)
{
    BinarySem = xSemaphoreCreateBinary();    /*  ���� ��ֵ �ź��� */
    CountSem = xSemaphoreCreateCounting(5, 5); /*  ���� �����ź��� */
    MuxSem = xSemaphoreCreateMutex();        /*  ���� �����ź��� */
    Event = xEventGroupCreate();/*  ���� �¼��� */
    Queue = xQueueCreate((UBaseType_t ) 4,/*  ��Ϣ���еĳ��� */
                         (UBaseType_t ) 4);/*  ��Ϣ�Ĵ�С */

    SoftTmr = xTimerCreate((const char*)"AutoReloadTimer",
                           (TickType_t)1000,   /* ��ʱ������ 1000(tick) */
                           (UBaseType_t)pdTRUE,/*  ����ģʽ */
                           (void*)1,           /* Ϊÿ����ʱ������һ��������Ψһ ID */
                            SoftTmr_Callback);
    if (SoftTmr != NULL) {
        xTimerStart(SoftTmr, 0); // �������ڶ�ʱ��
    }
    
    SoftTmrStatic = xTimerCreateStatic("Timer",
                                       1000,
                                       pdTRUE,
                                       ( void * )2,
                                       SoftTmr_Callback,
                                       &xTimerBuffers);

    arg = arg;
    uint32_t send_data1 = 1;
    while(1)
    {
        xSemaphoreGive( BinarySem );
        xEventGroupSetBits(Event, 0x1); /*  ����һ���¼� 1 */
        xQueueSend( Queue,      /*  ��Ϣ���еľ�� */
                    &send_data1,/*  ���͵���Ϣ���� */
                    0 );        /*  �ȴ�ʱ�� 0 */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        vTaskDelay(500); /* ��ʱ 500 �� tick */
    }
}

void led1_task(void* arg)
{
    EventBits_t r_event; /*  ����һ���¼����ձ��� */
    uint32_t r_queue; /*  ����һ��������Ϣ�ı��� */
    arg = arg;
    while(1)
    {
        xSemaphoreTake(BinarySem, portMAX_DELAY);
        r_event = xEventGroupWaitBits(Event, /*  �¼������� */
                                      0x01,/*  ���� ���� ����Ȥ���¼� */
                                      pdTRUE, /*  �˳�ʱ����¼�λ */
                                      pdTRUE, /*  �������Ȥ�������¼� */
                                      portMAX_DELAY);/*  ָ����ʱ�¼�, һֱ�� */
        xQueueReceive( Queue, /*  ��Ϣ���еľ�� */
                       &r_queue, /*  ���͵���Ϣ���� */
                       portMAX_DELAY); /*  �ȴ�ʱ��  һֱ�� */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        vTaskDelay(500); /* ��ʱ 500 �� tick */
    }
}





