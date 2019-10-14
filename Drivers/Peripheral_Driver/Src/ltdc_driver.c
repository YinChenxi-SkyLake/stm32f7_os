/**
  ******************************************************************************
  * @file   
  * @author  
  * @brief   
  ******************************************************************************
  */ 
  

#include "ltdc_driver.h"


uint16_t ltdc_layer0[LTDC_HEIGHT][LTDC_WIDTH] __attribute__((at(LAYER0_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С
uint16_t ltdc_layer1[LTDC_WIDTH][LTDC_HEIGHT] __attribute__((at(LAYER1_ADDR)));	//����������ֱ���ʱ,LCD�����֡���������С

uint32_t *ltdc_framebuf[2];					//LTDC LCD֡��������ָ��,����ָ���Ӧ��С���ڴ�����
/***************************************************************************************
  * @brief   ��ָ����������䵥����ɫ
  * @input   (sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
  * @input   color:Ҫ������ɫ
  * @return
***************************************************************************************/
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint32_t timeout = 0;
    uint32_t addr = ((uint32_t)ltdc_layer0[0] + 2 * (LTDC_WIDTH * sy + sx));//����洢����ַ
    uint16_t offline = LTDC_WIDTH - (ex - sx + 1); //������ƫ�� 
    
    RCC->AHB1ENR |= RCC_AHB1LPENR_DMA2DLPEN; //ʹ��DM2Dʱ��
	DMA2D->CR = DMA2D_CR_MODE;				 //�Ĵ������洢��ģʽ
	DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565;//������ɫ��ʽ
	DMA2D->OOR = offline;
	DMA2D->CR &= ~(DMA2D_CR_START);			//��ֹͣDMA2D
	DMA2D->OMAR = addr;
	DMA2D->NLR = (ey - sy + 1) | ((ex - sx + 1)<<16);//�趨�����Ĵ���
	DMA2D->OCOLR = color;				     //�趨�����ɫ�Ĵ��� 

	DMA2D->CR |= DMA2D_CR_START;		    //����DMA2D
	while((DMA2D->ISR & DMA2D_ISR_TCIF) == 0)	         //�ȴ��������
	{
		timeout++;
		if(timeout>0X1FFFFF)
            break;	//��ʱ�˳�
	}
	DMA2D->IFCR |= DMA2D_IFCR_CTCIF;				    //���������ɱ�־ 	
}

/***************************************************************************************
  * @brief   ����
  * @input   x,y:����  color:��ɫ
  * @return
***************************************************************************************/
void LTDC_Clear(uint32_t color)
{
	LCD_Fill(0, 0, LTDC_WIDTH-1, LTDC_HEIGHT-1, color);
}

