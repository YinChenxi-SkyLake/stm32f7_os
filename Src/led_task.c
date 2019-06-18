#include "main.h"

SemaphoreHandle_t  BinarySem = NULL;
SemaphoreHandle_t  CountSem = NULL;
SemaphoreHandle_t  MuxSem = NULL;
EventGroupHandle_t Event = NULL;
QueueHandle_t      Queue = NULL;
TimerHandle_t SoftTmr = NULL; /*  �����ʱ����� */

void SoftTmr_Callback(TimerHandle_t xTimer)
{
    
}

void led0_task(void* arg)
{
    arg = arg;
    uint32_t send_data1 = 1;
    static uint8_t index = 0;
    GUI_Init();
    while(1)
    {
        xSemaphoreGive( BinarySem );
        xEventGroupSetBits(Event, 0x1); /*  ����һ���¼� 1 */
        xQueueSend( Queue,      /*  ��Ϣ���еľ�� */
                    &send_data1,/*  ���͵���Ϣ���� */
                    0 );        /*  �ȴ�ʱ�� 0 */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        
        index++;
        switch(index%3)
        {
            case 0:
                GUI_SetBkColor(GUI_RED);
                break;
            case 1:
                GUI_SetBkColor(GUI_BLUE);
                break;
            case 2:
                GUI_SetBkColor(GUI_GRAY);
                break;
        }
        GUI_Clear();
        GUI_SetFont(&GUI_Font24_ASCII); //��������
        GUI_DispStringAt("Hello world!",100,100);
        vTaskDelay(1000); /* ��ʱ 500 �� tick */
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
        vTaskDelay(1000); /* ��ʱ 500 �� tick */
    }
}





