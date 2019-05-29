#include "main.h"

extern OS_TCB DS1TaskTCB; 
extern OS_MEM mem;

void led0_task(void *p_arg)
{
    OS_ERR  err;
    p_arg = p_arg;
    char * p_mem_blk;
    
    while(DEF_TRUE){
        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
        
        /*  �� mem  ��ȡ�ڴ�� */
        p_mem_blk = OSMemGet ((OS_MEM *)&mem,(OS_ERR *)&err);
        strcpy(p_mem_blk,"mem test");
        // ������Ϣ���������ö���
        OSTaskQPost((OS_TCB* )&DS1TaskTCB, // ������ DS1TaskTCB  ������Ϣ
                    (void*  )p_mem_blk,
                    (OS_MSG_SIZE )strlen(p_mem_blk),
                    (OS_OPT  )OS_OPT_POST_FIFO,
                    (OS_ERR*  )&err);

        OSTimeDly (1000, OS_OPT_TIME_DLY, &err );   //�������ʱ1000��ʱ�ӽ��ģ�1s��
    }
}

void led1_task(void *p_arg)
{
    OS_ERR  err;
    p_arg = p_arg;
    OS_MSG_SIZE size;
    char *p_msg;
    while(1){
        
        // ������Ϣ
        p_msg = OSTaskQPend((OS_TICK  )10,                    //�ȴ���Ϣ�ĳ�ʱʱ��
                            (OS_OPT )OS_OPT_PEND_BLOCKING,//�Ƿ�ʹ������ģʽ
                            (OS_MSG_SIZE*  )&size,            //ָ������Ϣ��С�ı���
                            (CPU_TS* )0,                      //ָ��һ��ʱ���������ʲôʱ����յ���Ϣ��
                            (OS_ERR* )&err );
        if(err == OS_ERR_NONE){
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
            printf ("led1_task��%d �ֽڣ����ݣ�%s\r\n", size, (char *)p_msg );
        }
        
        /*  �˻��ڴ�� */
        OSMemPut((OS_MEM *)&mem, // ָ���ڴ�������
                (void *)p_msg, // �ڴ����׵�ַ
                (OS_ERR *)&err); //
        OSTimeDly (500, OS_OPT_TIME_DLY, &err );   //�������ʱ1000��ʱ�ӽ��ģ�1s��
    }
}


