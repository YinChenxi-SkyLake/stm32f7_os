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
	while((USART1->ISR&0X40)==0);//ѭ������,ֱ���������   
	USART1->TDR=(uint8_t)ch;      
	return ch;
}





