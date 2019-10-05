#include "main.h"

void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000));				//��ͨ��ʽ��ʱ
}

void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t t_old, t_now, t_cnt = 0;
    uint32_t reload = SysTick->LOAD;//LOAD��ֵ
    ticks = nus * 216; 			//��Ҫ�Ľ�����
    t_old = SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
    while(1)
    {
        t_now = SysTick->VAL;
        if(t_now != t_old)
        {
            if(t_now < t_old)
                t_cnt += t_old - t_now;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
            else
                t_cnt += reload - t_now + t_old;

            t_old = t_now;

            if(t_cnt >= ticks)
                break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
        }
    }
}

//#pragma import(__use_no_semihosting)
//��׼����Ҫ��֧�ֺ���
struct __FILE
{
    int handle;
};

//FILE __stdout;

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









