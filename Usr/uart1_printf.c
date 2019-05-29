#include <stdio.h>
#include "main.h"

//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

FILE __stdout;

//����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
    x = x;
}

int fputc(int ch, FILE *f)
{
    while((USART1->ISR & 0X40) == 0); //ѭ������,ֱ���������
    USART1->TDR = (uint8_t)ch;
    return ch;
}

/**
  * @brief ��ӳ�䴮�� DEBUG_USARTx �� rt_kprintf()����
  * @param str��Ҫ��������ڵ��ַ���
  * @retval ��
  *
  * @attention
  *
*/
void rt_hw_console_output(const char *str)
{
    /* �����ٽ�� */
    rt_enter_critical();

    /* ֱ���ַ������� */
    while (*str != '\0')
    {
        /* ���� */
        if (*str == '\n')
        {
            USART1->TDR = '\r';
            while((USART1->ISR & 0X40) == 0); //ֱ���������
        }

        USART1->TDR = *str++;
        while((USART1->ISR & 0X40) == 0); //ֱ���������
    }

    /* �˳��ٽ�� */
    rt_exit_critical();
}




