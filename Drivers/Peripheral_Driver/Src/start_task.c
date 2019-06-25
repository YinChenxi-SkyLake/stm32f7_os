#include "main.h"

static TaskHandle_t AppTaskCreate_Handle; /* ���������� */
static TaskHandle_t LED1_Task_Handle = NULL;/* LED1  ������ */
static TaskHandle_t LED2_Task_Handle = NULL;/* LED2  ������ */

extern SemaphoreHandle_t  BinarySem;
extern SemaphoreHandle_t  CountSem;
extern SemaphoreHandle_t  MuxSem;
extern EventGroupHandle_t Event;
extern QueueHandle_t      Queue;
extern TimerHandle_t      SoftTmr;


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    taskENTER_CRITICAL(); //�����ٽ���
    
    SDRAM_Initialization_Sequence(&hsdram1);
    
    LTDC_Clear(0xffff);
    
    GT9147_Init();//���ݴ�������������ʼ��
    
    /*  ���� LED_Task  ���� */
    xTaskCreate((TaskFunction_t )led1_task, /*  ������ں��� */
                (const char* )"LED2_Task",/*  �������� */
                (uint16_t )128, /*  ����ջ��С */
                (void* )NULL, /*  ������ں������� */
                (UBaseType_t )5,  /*  ��������ȼ� */
                (TaskHandle_t* )&LED2_Task_Handle);/*  ������ƿ�ָ�� */
    
    /*  ���� LED_Task  ���� */
    xTaskCreate((TaskFunction_t )led0_task, /*  ������ں��� */
                (const char* )"LED1_Task",/*  �������� */
                (uint16_t )128, /*  ����ջ��С */
                (void* )NULL, /*  ������ں������� */
                (UBaseType_t )6,  /*  ��������ȼ� */
                (TaskHandle_t* )&LED1_Task_Handle);/*  ������ƿ�ָ�� */




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
    
    vTaskDelete(AppTaskCreate_Handle); //ɾ�� AppTaskCreate ����

    taskEXIT_CRITICAL(); //�˳��ٽ���
}

void os_init(void)
{
    taskENTER_CRITICAL();           //�����ٽ���

    /*  ���� LED_Task  ���� */
    xTaskCreate((TaskFunction_t )AppTaskCreate, /*  ������ں��� */
                (const char* )"StartTask",/*  �������� */
                (uint16_t )128,   /*  ����ջ��С */
                (void* )NULL,     /*  ������ں������� */
                (UBaseType_t )5,  /*  ��������ȼ� */
                (TaskHandle_t* )&AppTaskCreate_Handle);/*  ������ƿ�ָ�� */
                
    vTaskStartScheduler();   /* �������񣬿������� */

    taskEXIT_CRITICAL();            //�˳��ٽ���
}




