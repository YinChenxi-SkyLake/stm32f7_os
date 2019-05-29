
#include "main.h"

UINT32 Start_TaskId;//����������
UINT32 Led0_TaskId;
UINT32 Led1_TaskId;

void start_task(VOID)
{
    TSK_INIT_PARAM_S task_init_param;
    
    task_init_param.pcName = "led0_task";
    task_init_param.usTaskPrio = 3;
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)led0_task;
    task_init_param.uwStackSize = 0;//ʹ��Ĭ�϶�ջ��С
    LOS_TaskCreate(&Led0_TaskId, &task_init_param);/* �������� */
    
    task_init_param.pcName = "led1_task";
    task_init_param.usTaskPrio = 4;
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)led1_task;
    task_init_param.uwStackSize = 0;//ʹ��Ĭ�϶�ջ��С
    LOS_TaskCreate(&Led0_TaskId, &task_init_param);/* �������� */
    
    LOS_TaskDelete(Start_TaskId);
}

void os_init(void)
{
    LOS_KernelInit();
    
    TSK_INIT_PARAM_S task_init_param;
    task_init_param.pcName = "start_task";
    task_init_param.usTaskPrio = 3;
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)start_task;
    task_init_param.uwStackSize = 128;
    
    LOS_TaskCreate(&Start_TaskId, &task_init_param);/* �������� */
    
    LOS_Start();
}








