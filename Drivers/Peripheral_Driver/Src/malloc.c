#include "malloc.h"

uint8_t   mem_rdy; 				//�ڴ�����Ƿ����

//�ڴ��(32�ֽڶ���), �ⲿSDRAM�ڴ��,ǰ��2M��LTDC����(1280*800*2)
__align(32) uint8_t mem_base[MEM_MAX_SIZE] __attribute__((at(0xC0600000)));	

//�ڴ�����
uint32_t mem_map[MEM_MAP_SIZE] __attribute__((at(0xC0600000 + MEM_MAX_SIZE)));

/***************************************************************************************
  * @brief   �ڴ�����ʼ��  
  * @input   
  * @return  
***************************************************************************************/
static void mem_init(void)
{
    memset(mem_map, 0, MEM_MAP_SIZE *4);	//�ڴ�״̬����������  
 	mem_rdy = 1;						//�ڴ�����ʼ��OK  
}


/***************************************************************************************
  * @brief   �ڴ����(�ڲ�����)
  * @input   size:Ҫ������ڴ��С(�ֽ�)
  * @return  ����ֵ:0XFFFFFFFF,�������;����,�ڴ�ƫ�Ƶ�ַ 
***************************************************************************************/
static uint32_t sdram_malloc(uint32_t size)  
{
    signed long offset=0;  
    uint32_t nmemb;	//��Ҫ���ڴ����  
	uint32_t cmemb = 0;//�������ڴ����
    uint32_t i;
    if( !mem_rdy ){
        mem_init();//δ��ʼ��,��ִ�г�ʼ�� 
    }
    if(size == 0){
        return 0xFFFFFFFF;//����Ҫ����
    }
    nmemb = size / MEM_BLOCK_SIZE;//��ȡ��Ҫ����������ڴ����
    if(size % MEM_BLOCK_SIZE){
        nmemb++;  
    }
    for(offset = MEM_MAP_SIZE - 1; offset >= 0; offset--)//���������ڴ������  
    {
		if( !mem_map[offset] )
            cmemb++;//�������ڴ��������
		else
            cmemb=0;								//�����ڴ������
        
		if(cmemb == nmemb){							//�ҵ�������nmemb�����ڴ��
            for(i=0; i < nmemb; i++) { 					//��ע�ڴ��ǿ� 
                mem_map[offset+i] = nmemb;  
            }
            return (offset * MEM_BLOCK_SIZE);//����ƫ�Ƶ�ַ  
		}
    }  
    return 0XFFFFFFFF;//δ�ҵ����Ϸ����������ڴ��  
}


/***************************************************************************************
  * @brief   �ͷ��ڴ�(�ڲ�����) 
  * @input   memx:�����ڴ��;
             offset:�ڴ��ַƫ��
  * @return  ����ֵ:0,�ͷųɹ�;1,�ͷ�ʧ��;  
***************************************************************************************/
static uint8_t sdram_free(uint32_t offset)  
{
    int i;
    if( !mem_rdy )//δ��ʼ��,��ִ�г�ʼ��
	{
		mem_init();    
        return 1;//δ��ʼ��  
    }
    if(offset < MEM_MAX_SIZE)                 //ƫ�����ڴ����. 
    {  
        int index = offset / MEM_BLOCK_SIZE;  //ƫ�������ڴ�����  
        int nmemb = mem_map[index];	  //�ڴ������
        for(i=0; i < nmemb; i++)  			  //�ڴ������
        {
            mem_map[index+i] = 0;  
        }  
        return 0;  
    }else 
        return 2;//ƫ�Ƴ�����.  
}  


/***************************************************************************************
  * @brief   �ͷ��ڴ�(�ⲿ����) 
  * @input   ptr:�ڴ��׵�ַ 
  * @return  
***************************************************************************************/
void mem_free(void *ptr)  
{
	uint32_t offset;   
	if(ptr == NULL) 
        return;//��ַΪ0.  
 	offset = (uint32_t)ptr - (uint32_t)mem_base;     
    sdram_free(offset);	//�ͷ��ڴ�      
}


/***************************************************************************************
  * @brief   �����ڴ�(�ⲿ����)
  * @input   size:�ڴ��С(�ֽ�)
  * @return  
***************************************************************************************/
void *mem_malloc(uint32_t size)  
{
    uint32_t offset;   
	offset = sdram_malloc(size);  	   	 	   
    if(offset == 0XFFFFFFFF)
        return NULL;  
    else 
        return (void*)((uint32_t)mem_base + offset);  
}



