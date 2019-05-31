#ifndef __LCD_DRIVER_H
#define __LCD_DRIVER_H

#include "stdint.h"
#include "cmsis_armcc.h"

typedef struct
{
    volatile uint16_t reg;
    volatile uint16_t data;
} LCD_TypeDef;
#define LCD_BASE ((uint32_t)(0x60000000 | 0x0007FFFE))
#define LCD      ((LCD_TypeDef *) LCD_BASE)

#define LCD_CMD   (*(uint32_t *)(0x60000000 | 0))
#define LCD_DATA  (*(uint32_t *)(0x60000000 | 0x80000))

#define SET_X_CMD 0x2A
#define SET_Y_CMD 0x2B
#define MEM_WRITE 0x2C;
#define MEM_READ  0x2E

#define LCD_WIDTH   800
#define LCD_LENGTH  480



/*��ָ���ļĴ���д����*/
__inline void lcd_w_reg_data(uint16_t reg,uint16_t data)
{
	LCD->reg = reg;		//д��Ҫд�ļĴ������	 
	LCD->data = data;   //д������	    		 
}

void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
void LCD_InitSequence(void);

#endif
