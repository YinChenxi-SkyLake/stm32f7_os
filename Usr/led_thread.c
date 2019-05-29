#include "main.h"

extern rt_sem_t sem;/* ָ���ź�����ָ�� */

extern rt_mutex_t mutex;/* ���������� */

extern rt_event_t event;/* ָ���¼��ĵ�ַָ�� */

void led0_thread(void* arg)
{
    arg = arg;
    while(1)
    {
        rt_sem_take(sem, RT_WAITING_FOREVER);/* ��ͼ����һ���ź�����������ɹ���һֱ�ȴ�֪���ɹ� */
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
        rt_sem_release(sem); /* �ͷ�һ���ź��� */
        
        rt_thread_delay(500); /* ��ʱ 500 �� tick */
    }
}

void led1_thread(void* arg)
{
    arg = arg;
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        rt_thread_delay(500); /* ��ʱ 500 �� tick */
    }
}





