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

#define SET_X_CMD 0x2A00
#define SET_Y_CMD 0x2B00
#define SET_DIR   0x3600
#define MEM_WRITE 0x2C00
#define MEM_READ  0x2E00


//ɨ�跽����
#define L2R_U2D  0 		//������,���ϵ���
#define L2R_D2U  1 		//������,���µ���
#define R2L_U2D  2 		//���ҵ���,���ϵ���
#define R2L_D2U  3 		//���ҵ���,���µ���

#define U2D_L2R  4 		//���ϵ���,������
#define U2D_R2L  5 		//���ϵ���,���ҵ���
#define D2U_L2R  6 		//���µ���,������
#define D2U_R2L  7		//���µ���,���ҵ���	 

#define DFT_SCAN_DIR  L2R_U2D  //Ĭ�ϵ�ɨ�跽��

#define LCD_WIDTH   800
#define LCD_LENGTH  480

//LCD��Ҫ������
typedef struct  
{		 	 
	uint16_t width;			//LCD ���
	uint16_t height;			//LCD �߶�
	uint16_t id;				//LCD ID
	uint8_t  dir;			//���������������ƣ�0��������1��������	
	uint16_t wramcmd;		//��ʼдgramָ��
	uint16_t setxcmd;		//����x����ָ��
	uint16_t setycmd;		//����y����ָ�� 
}_lcd_dev; 	
extern _lcd_dev lcddev;

/*��ָ���ļĴ���д����*/
__inline void lcd_w_reg_data(uint16_t reg,uint16_t data)
{
	LCD->reg = reg;		//д��Ҫд�ļĴ������	 
	LCD->data = data;   //д������	    		 
}

void LCD_ReadId(void);
void LCD_Display_Dir(uint8_t dir);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
uint32_t LCD_ReadPoint(uint16_t x,uint16_t y);
void LCD_DrawPoint(uint16_t x,uint16_t y,uint32_t color);
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint32_t color);
void LCD_InitSequence(void);

#endif
