#ifndef __LTDC_DRIVER
#define __LTDC_DRIVER

#include "main.h"

#define LTDC_WIDTH  480
#define LTDC_HEIGHT 272

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ

extern uint16_t ltdc_layer0[LTDC_HEIGHT][LTDC_WIDTH];

/***************************************************************************************
  * @brief   ��ȡ��ĳ�����ɫֵ
  * @input   x,y:����
  * @return  ����ֵ:�˵����ɫ
***************************************************************************************/
__inline uint32_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
    return *(uint16_t*)((uint32_t)ltdc_layer0[0] + 2 * (LTDC_WIDTH * y + x));
}


/***************************************************************************************
  * @brief   ����
  * @input   x,y:����  color:��ɫ
  * @return
***************************************************************************************/
__inline void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color)
{
    *(uint16_t*)((uint32_t)ltdc_layer0[0] + 2 * (LTDC_WIDTH * y + x)) = color;
}


void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
void LTDC_Clear(uint32_t color);



#endif
