#include "main.h"


static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];/*  �������������ջ */
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];/*  ��ʱ�������ջ */
static StackType_t AppTaskCreate_Stack[128];/* AppTaskCreate���������ջ */

static StaticTask_t Idle_Task_TCB;/*  ����������ƿ� */
static StaticTask_t Timer_Task_TCB;/*  ��ʱ��������ƿ� */
static StaticTask_t AppTaskCreate_TCB;/* AppTaskCreate ������ƿ� */



static TaskHandle_t AppTaskCreate_Handle; /* ���������� */
static TaskHandle_t LED1_Task_Handle = NULL;/* LED1  ������ */
static TaskHandle_t LED2_Task_Handle = NULL;/* LED2  ������ */
/**
*******************************************************************
* @brief ��ȡ��������������ջ��������ƿ��ڴ�
* ppxTimerTaskTCBBuffer  :  ������ƿ��ڴ�
* ppxTimerTaskStackBuffer  :  �����ջ�ڴ�
* pulTimerTaskStackSize  :  �����ջ��С
**********************************************************************
*/
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &Idle_Task_TCB; /*  ������ƿ��ڴ� */
    *ppxIdleTaskStackBuffer = Idle_Task_Stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE; /*  �����ջ��С */
}


/**
*********************************************************************
* @brief ��ȡ��ʱ������������ջ��������ƿ��ڴ�
* ppxTimerTaskTCBBuffer  :  ������ƿ��ڴ�
* ppxTimerTaskStackBuffer  :  �����ջ�ڴ�
* pulTimerTaskStackSize  :  �����ջ��С
**********************************************************************
*/
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &Timer_Task_TCB; /*  ������ƿ��ڴ� */
    *ppxTimerTaskStackBuffer = Timer_Task_Stack; /*  �����ջ�ڴ� */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH; /*  �����ջ��С */
}

/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
    taskENTER_CRITICAL(); //�����ٽ���

    /*  ���� LED_Task  ���� */
    xTaskCreate((TaskFunction_t )led0_task, /*  ������ں��� */
                (const char* )"LED1_Task",/*  �������� */
                (uint16_t )128, /*  ����ջ��С */
                (void* )NULL, /*  ������ں������� */
                (UBaseType_t )4,  /*  ��������ȼ� */
                (TaskHandle_t* )&LED1_Task_Handle);/*  ������ƿ�ָ�� */


    /*  ���� LED_Task  ���� */
    xTaskCreate((TaskFunction_t )led1_task, /*  ������ں��� */
                (const char* )"LED2_Task",/*  �������� */
                (uint16_t )128, /*  ����ջ��С */
                (void* )NULL, /*  ������ں������� */
                (UBaseType_t )5,  /*  ��������ȼ� */
                (TaskHandle_t* )&LED2_Task_Handle);/*  ������ƿ�ָ�� */

    vTaskDelete(AppTaskCreate_Handle); //ɾ�� AppTaskCreate ����

    taskEXIT_CRITICAL(); //�˳��ٽ���
}

void os_init(void)
{
    taskENTER_CRITICAL();           //�����ٽ���

    /* ���� AppTaskCreate ���� */
    AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate,	//������
                           (const char*  )"AppTaskCreate",//��������
                           (uint32_t 	   )128,	        //�����ջ��С
                           (void* 	   )NULL,	        //���ݸ��������Ĳ���
                           (UBaseType_t  )3,           	//�������ȼ�
                           (StackType_t* )AppTaskCreate_Stack,//�����ջ
                           (StaticTask_t*)&AppTaskCreate_TCB);//������ƿ�

    if(NULL != AppTaskCreate_Handle)/* �����ɹ� */
        vTaskStartScheduler();   /* �������񣬿������� */

    taskEXIT_CRITICAL();            //�˳��ٽ���
    vTaskStartScheduler();   /* �������񣬿������� */
}




