#include "main.h"




void Drow_2D(void)
{
    static uint8_t count = 0;
    count++;
    switch(count % 2) {
    case 0:
//        LCD_Fill(0, 0, 480, 800, GUI_GREEN);
        GUI_SetBkColor(GUI_BLUE);
        GUI_Clear();
        GUI_SetFont(&GUI_Font16_ASCII);
        GUI_DispStringHCenterAt("ALIENEK BIN&HEX NUM DISPLAY", 160, 0);
        GUI_SetColor(GUI_RED);
        GUI_SetFont(&GUI_Font13_ASCII);
//�����������̺�����ʾ
        GUI_SetBkColor(GUI_YELLOW);
        GUI_ClearRect(5, 20, 95, 110); //�˺���ʹ�ñ�����ɫ���
        GUI_SetBkColor(GUI_BLUE);
//����ˮƽ��ɫ�ݶ����ľ���
        GUI_DrawGradientH(100, 20, 190, 110, 0X4117BB, 0XC6B6F5);
//���ƴ�ֱ��ɫ�ݶ�����Բ�Ǿ���
        GUI_DrawGradientRoundedV(195, 20, 285, 110, 25, 0X4117BB, 0XC6B6F5);
        GUI_DrawRect(5, 115, 95, 205); //��ָ��λ�û��ƾ���
        GUI_FillRect(100, 115, 190, 205); //��ָ��λ�û������ľ�������
        GUI_FillRoundedRect(195, 115, 285, 205, 25); //��ָ��λ�û�������Բ�Ǿ���
        break;
    case 1:
//        LCD_Fill(0, 0, 480, 800, GUI_WHITE);
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();
        GUI_SetFont(&GUI_Font16_ASCII);
        GUI_SetColor(GUI_BLACK);
        GUI_DispStringHCenterAt("ALIENEK ALPHA DISPLAY", 160, 0);
        GUI_EnableAlpha(1); //���� Alpha
        GUI_SetColor(GUI_BLACK);
        GUI_DispStringHCenterAt("Alphablending", 45, 61);
        GUI_SetAlpha(40); //���� Alpha ֵ
        GUI_SetColor( GUI_RED);
        GUI_FillRect(0, 20, 49, 69);
        GUI_SetAlpha(80); //���� Alpha ֵ
        GUI_SetColor(GUI_GREEN);
        GUI_FillRect(20, 40, 69, 89);
        GUI_SetAlpha(40); //���� Alpha ֵ
        GUI_SetColor(GUI_BLUE);
        GUI_FillRect(40, 60, 89, 109);
        GUI_EnableAlpha(0); //�ر� Alpha
        break;
        
    default:
        LCD_Fill(0, 0, 480, 800, 0xff);
        break;
    }






}





