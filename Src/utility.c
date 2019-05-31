#include "main.h"

void delay_us(uint32_t nus)
{
    uint32_t ticks;
	uint32_t t_old,t_now,t_cnt=0;
	uint32_t reload = SysTick->LOAD;//LOAD��ֵ	    	 
	ticks = nus * 216; 			//��Ҫ�Ľ����� 
    t_old = SysTick->VAL;        	//�ս���ʱ�ļ�����ֵ
    while(1)
    {
        t_now = SysTick->VAL;	
		if(t_now!=t_old)
		{	    
			if(t_now<t_old)
                t_cnt += t_old - t_now;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
                t_cnt += reload - t_now + t_old;	
            
			t_old = t_now;
            
			if(t_cnt>=ticks)
                break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
    }	
}

void delay_ms(uint32_t ms){
    delay_us(ms * 1000);
}










