#ifndef __MALLOC_H
#define __MALLOC_H

#include "stdint.h"
#include "string.h"

#ifndef NULL
#define NULL 0
#endif

//SDRAM�ڴ�����趨
#define MEM_MAX_SIZE			20*1024*1024  					//�������ڴ�20M
#define MEM_BLOCK_SIZE			64  	  						//�ڴ���СΪ64�ֽ�
#define MEM_MAP_SIZE        	(MEM_MAX_SIZE / MEM_BLOCK_SIZE)   //�ڴ���С


//�û����ú���
void sdram_mem_free(void *ptr)  ;  			//�ڴ��ͷ�(�ⲿ����)
void *sdram_mem_malloc(uint32_t size);			//�ڴ����(�ⲿ����)
#endif
