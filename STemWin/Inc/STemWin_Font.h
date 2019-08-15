#ifndef __STEMWIN_FONT_H
#define __STEMWIN_FONT_H

//�ֿ�����ռ�õ�����������С(4���ֿ�+unigbk��+�ֿ���Ϣ=6302984�ֽ�,Լռ1539��W25QXX����,һ������4K�ֽ�)
#define FONTSECSIZE	 	1539
//�ֿ�����ʼ��ַ
#define FONTINFOADDR 	1024*1024*25 	//ǰ��25M��fatfsռ����;��25M��ַ�Ժ�ʼ����ֿ�;
//25M�Ժ����4���ֿ�+UNIGBK.BIN,�ܴ�С6.01M;31.01M�Ժ�,�û���������ʹ��.

/* USER CODE BEGIN Prototypes */
//�ֿ���Ϣ�ṹ�嶨��
//���������ֿ������Ϣ����ַ����С��
__packed typedef struct 
{
	uint8_t  fontok;			//�ֿ���ڱ�־��0XAA���ֿ��������������ֿⲻ����
	uint32_t ugbkaddr; 			//unigbk�ĵ�ַ
	uint32_t ugbksize;			//unigbk�Ĵ�С	 
	uint32_t f12addr;			//gbk12��ַ	
	uint32_t gbk12size;			//gbk12�Ĵ�С	 
	uint32_t f16addr;			//gbk16��ַ
	uint32_t gbk16size;			//gbk16�Ĵ�С		 
	uint32_t f24addr;			//gbk24��ַ
	uint32_t gbk24size;			//gbk24�Ĵ�С 	 
	uint32_t f32addr;			//gbk32��ַ
	uint32_t gbk32size;			//gbk32�Ĵ�С 
}_font_info; 

extern _font_info ftinfo;
extern GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16;

void update_font(void);

#endif
