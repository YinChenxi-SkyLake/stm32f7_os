#include "main.h"
#include "fatfs.h"
//#include "GUI.h"
#include "GUI_Private.h"
#include "EmWin_ASCII_Font.h"
/* USER CODE BEGIN Variables */


//�ֿ����ڴ����е�·��
char* const FONT_PATH[5] =
{
    "FONT/UNIGBK.BIN",	//UNIGBK.BIN�Ĵ��λ��
    "FONT/GBK12.FON",	//GBK12�Ĵ��λ��
    "FONT/GBK16.FON",	//GBK16�Ĵ��λ��
    "FONT/GBK24.FON",	//GBK24�Ĵ��λ��
    "FONT/GBK32.FON",	//GBK32�Ĵ��λ��
};

//���������ֿ������Ϣ����ַ����С��
_font_info ftinfo;
char     full_font_path[64];
uint8_t  font_buf[4096];
uint32_t fontSectorSize;  
uint32_t offx = 0, flashaddr = 0;

//��ģ�����ݴ�����,������ģ������ֽ���
//������ģ���Ϊ1024
#define BYTES_PER_FONT	1024	//32*32=1024�������ʾ32*32�����ֿ�	
U8 GUI_FontDataBuf[BYTES_PER_FONT];

//����һ���µ�����ṹ
#define GUI_FONTTYPE_PROP_USER      \
   GUIPROP_X_DispChar,             	\
  (GUI_GETCHARDISTX*)GUIPROP_X_GetCharDistX,\
   GUIMONO_GetFontInfo,          	\
   GUIMONO_IsInFont,             	\
  (GUI_GETCHARINFO *)0,         	\
  (tGUI_ENC_APIList*)0  

void GUIPROP_X_DispChar(U16P c);
int GUIPROP_X_GetCharDistX(U16P c);
/*-----12��12����----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ12_CharInfo[2] = 
{    
	{ 6, 	6, 	1, (void*)"0"},  //
	{ 12, 	12, 2, (void*)"0"},  //��ʾ�˵����ַ���X�᳤����12�����ص㣬ʵ����ʾҲ��12�����ص㣬��ʾһ����Ҫ2�ֽڣ�ʹ���ַ���0����Ϊ��ʶ
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ12_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ12_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ12_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ12_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropHZ, 
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ12 = 
{
      GUI_FONTTYPE_PROP_USER, 
      12, 
      12, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ12_PropASC
};
/*-----16��16����----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ16_CharInfo[2] = 
{    
	{ 8, 	8, 	1, (void*)"0"},  
	{ 16, 	16, 2, (void*)"0"},
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ16_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ16_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ16_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ16_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropHZ, 
};


GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ16 = 
{
      GUI_FONTTYPE_PROP_USER, 
      16, 
      16, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ16_PropASC
};
/*-----24��24����----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ24_CharInfo[2] = 
{
	{ 12, 	12, 2, (void*)"0"},  
	{ 24, 	24, 3, (void*)"0"},
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ24_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ24_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ24_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ24_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropHZ, 
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24 = 
{
      GUI_FONTTYPE_PROP_USER, 
      24, 
      24, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropASC
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ24x2 = 
{
      GUI_FONTTYPE_PROP_USER, 
      24, 
      24, 
      2,  
      2,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ24_PropASC
};
/*-----32��32����----------------------------------------------------------*/
GUI_CONST_STORAGE GUI_CHARINFO GUI_FontHZ32_CharInfo[2] = 
{    
	{ 16, 	16, 2, (void*)"0"},  
	{ 32, 	32, 4, (void*)"0"},
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ32_PropHZ = {
      0x4081, 
      0xFFFF, 
      &GUI_FontHZ32_CharInfo[1],
      (void *)0, 
};

GUI_CONST_STORAGE GUI_FONT_PROP GUI_FontHZ32_PropASC = {
      0x0000, 
      0x007F, 
      &GUI_FontHZ32_CharInfo[0],
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropHZ, 
};

GUI_CONST_STORAGE  GUI_FONT GUI_FontHZ32 = 
{
      GUI_FONTTYPE_PROP_USER, 
      32, 
      32, 
      1,  
      1,  
      (void GUI_CONST_STORAGE *)&GUI_FontHZ32_PropASC
};


/***************************************************************************************
  * @brief   ���������ļ�,UNIGBK,GBK12,GBK16,GBK24,GBK32һ�����
  * @input   src:�ֿ���Դ����."0:",SD��;"1:",FLASH��,"2:",U��.
  * @return
***************************************************************************************/
void update_font(void)
{
    uint8_t rval;
    UINT br;
    
    /*#### 1.finnd UNIGBK,GBK12,GBK16,GBK24,GBK32,calculate the total size.######## */
    fontSectorSize = 0;
    for(uint8_t i = 0; i < 5; i++)	//
    {
        strcpy(full_font_path, SDPath);			//copy src���ݵ�pname
        strcat(full_font_path, FONT_PATH[i]);		//׷�Ӿ����ļ�·��
        retSD = f_open(&SDFile, (const TCHAR*)full_font_path, FA_READ);	//���Դ�
        if(retSD){
            rval |= 1 << 7;	//��Ǵ��ļ�ʧ��
            break;		   //������,ֱ���˳�
        }
        fontSectorSize += SDFile.obj.objsize;
        f_close(&SDFile);
    }
    GUI_SetFont(&GUI_FontHZ16); // ��������
    GUI_DispStringAt("��ʼ�����ֿ�!!",100,180);
    if(rval == 0)			//�ֿ��ļ�������.
    {
        fontSectorSize = fontSectorSize / 4096  + (fontSectorSize%4096 ? 1 : 0);
        
        /*#### 2.Erase sector #################################################### */
        for(uint32_t i = 0; i < fontSectorSize; i++)			//�Ȳ����ֿ�����,���д���ٶ�
        {
            W25QXX_EraseSector((FONTINFOADDR / 4096) + i);	//��Ҫ����������
        }
        
        for(uint8_t i = 0; i < 5; i++)	//���θ���UNIGBK,GBK12,GBK16,GBK24,GBK32
        {
            offx = 0;
            strcpy(full_font_path, SDPath);			//copy src���ݵ�pname
            strcat(full_font_path, FONT_PATH[i]);		//׷�Ӿ����ļ�·��
            if(f_open(&SDFile, (const TCHAR*)full_font_path, FA_READ) == FR_OK) {
                switch(i)
                {
                case 0:												//����UNIGBK.BIN
                    ftinfo.ugbkaddr = FONTINFOADDR + sizeof(ftinfo);//��Ϣͷ֮�󣬽���UNIGBKת�����
                    ftinfo.ugbksize = SDFile.obj.objsize;			//UNIGBK��С
                    flashaddr = ftinfo.ugbkaddr;
                    break;
                case 1:
                    ftinfo.f12addr = ftinfo.ugbkaddr + ftinfo.ugbksize;	//UNIGBK֮�󣬽���GBK12�ֿ�
                    ftinfo.gbk12size = SDFile.obj.objsize;				//GBK12�ֿ��С
                    flashaddr = ftinfo.f12addr;                         //GBK12����ʼ��ַ
                    break;
                case 2:
                    ftinfo.f16addr = ftinfo.f12addr + ftinfo.gbk12size;	//GBK12֮�󣬽���GBK16�ֿ�
                    ftinfo.gbk16size = SDFile.obj.objsize;				//GBK16�ֿ��С
                    flashaddr = ftinfo.f16addr;						    //GBK16����ʼ��ַ
                    break;
                case 3:
                    ftinfo.f24addr = ftinfo.f16addr + ftinfo.gbk16size;	//GBK16֮�󣬽���GBK24�ֿ�
                    ftinfo.gbk24size = SDFile.obj.objsize;				//GBK24�ֿ��С
                    flashaddr = ftinfo.f24addr;						    //GBK24����ʼ��ַ
                    break;    
                case 4:
                    ftinfo.f32addr = ftinfo.f24addr + ftinfo.gbk24size;	//GBK24֮�󣬽���GBK32�ֿ�
                    ftinfo.gbk32size = SDFile.obj.objsize;				//GBK32�ֿ��С
                    flashaddr = ftinfo.f32addr;						    //GBK32����ʼ��ַ
                    break;
                }
                /*#### 3.copy font frome SD card to SPI flash. ##################### */
                while(retSD == FR_OK)//��ѭ��ִ��
                {
                    retSD = f_read(&SDFile, font_buf, 4096, (UINT *)&br);//��ȡ����
                    if(retSD != FR_OK) break;
                    W25QXX_Write_NoCheck(font_buf, offx + flashaddr, 4096);//�����������ֿ�����д��SPI Flash
                    offx += br;
                    if(br != 4096) break;
                }
            }
            f_close(&SDFile);
        }
        //ȫ�����º���
		ftinfo.fontok = 0xAA;
		W25QXX_Write((uint8_t*)&ftinfo, FONTINFOADDR, sizeof(ftinfo));	//�����ֿ���Ϣ
        GUI_SetFont(&GUI_FontHZ32); // ��������
        GUI_DispStringAt("�ɹ������ֿ�!!\n",100,200);
    }
}

/***************************************************************************************
  * @brief   ��ȡ��ģ����
  * @input   
  * @return  
***************************************************************************************/
static void GUI_GetDataFromMemory(const GUI_FONT_PROP GUI_UNI_PTR *pProp, U16P c)
{
	unsigned char qh,ql;
	unsigned char i;					  
    unsigned long foffset; 
	unsigned char t;
	unsigned char *mat;
    U8 size,csize;//�����С
    U16 BytesPerFont;
	GUI_FONT EMWINFONT;
	EMWINFONT = *GUI_pContext->pAFont;
    BytesPerFont = GUI_pContext->pAFont->YSize * pProp->paCharInfo->BytesPerLine; //ÿ����ģ�������ֽ���
    if (BytesPerFont > BYTES_PER_FONT) BytesPerFont = BYTES_PER_FONT;
	
	//�ж�����Ĵ�С
	if(memcmp(&EMWINFONT,&GUI_FontHZ12,sizeof(GUI_FONT)) == 0) size=12;			//12����
	else if(memcmp(&EMWINFONT,&GUI_FontHZ16,sizeof(GUI_FONT)) == 0) size=16;	//16����
	else if(memcmp(&EMWINFONT,&GUI_FontHZ24,sizeof(GUI_FONT)) == 0)	size=24;	//24����  
    else if(memcmp(&EMWINFONT,&GUI_FontHZ32,sizeof(GUI_FONT)) == 0)	size=32;	//32����
    
	csize = (size/8+((size%8)?1:0))*(size);	//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
    memset(GUI_FontDataBuf,0,csize);        //������    
    if (c < 0x80)	//Ӣ���ַ���ַƫ���㷨
    {
		switch(size)
		{
			case 12:
				for(t=0;t<12;t++) GUI_FontDataBuf[t]=emwin_asc2_1206[c-0x20][t];
				break;
			case 16:
				for(t=0; t<16; t++) GUI_FontDataBuf[t] = emwin_asc2_1608[c-0x20][t];
				break;
			case 24:
				for(t=0;t<48;t++) GUI_FontDataBuf[t]=emwin_asc2_2412[c-0x20][t];
				break;	
            case 32:
				for(t=0;t<64;t++) GUI_FontDataBuf[t]=emwin_asc2_3216[c-0x20][t];
				break;	
		} 
        if(c=='\r'||c=='\n')  memset(GUI_FontDataBuf,0,csize);    
	}else{
		ql=c/256;
		qh=c%256;
		if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)//�ǳ��ú���
		{
			for(i=0;i<(size*2);i++) *mat++=0x00;//�������
			return; //��������
		}          
		if(ql<0x7f)ql -= 0x40;
		else ql-=0x41;
		qh -= 0x81;   
		foffset=((unsigned long)190*qh+ql) * csize;//�õ��ֿ��е��ֽ�ƫ����	    
		switch(size)
		{
			case 12:
				W25QXX_Read(GUI_FontDataBuf,foffset+ftinfo.f12addr,csize);
				break;
			case 16:
				W25QXX_Read(GUI_FontDataBuf,foffset + ftinfo.f16addr,csize);
				break;
			case 24:
				W25QXX_Read(GUI_FontDataBuf,foffset+ftinfo.f24addr,csize);
				break;	
            case 32:
				W25QXX_Read(GUI_FontDataBuf,foffset+ftinfo.f32addr,csize);
				break;
		} 
	}   	
}


/*********************************************************************
*
*       GUIPROP_DispChar
*
* Purpose:
*   This is the routine that displays a character. It is used by all
*   other routines which display characters as a subroutine.
*/
void GUIPROP_X_DispChar(U16P c)
{
	int BytesPerLine;
    GUI_DRAWMODE DrawMode = GUI_pContext->TextMode;
    const GUI_FONT_PROP GUI_UNI_PTR *pProp = GUI_pContext->pAFont->p.pProp;
    //������λ�ֿ�������Ϣ 
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last)) break;
    }
    if (pProp)
    {
        GUI_DRAWMODE OldDrawMode;
        const GUI_CHARINFO GUI_UNI_PTR * pCharInfo = pProp->paCharInfo;
        GUI_GetDataFromMemory(pProp, c);//ȡ����ģ����
        BytesPerLine = pCharInfo->BytesPerLine;                
        OldDrawMode  = LCD_SetDrawMode(DrawMode);
        LCD_DrawBitmap(GUI_pContext->DispPosX, GUI_pContext->DispPosY,
                       pCharInfo->XSize, GUI_pContext->pAFont->YSize,
                       GUI_pContext->pAFont->XMag, GUI_pContext->pAFont->YMag,
                       1,     /* Bits per Pixel */
                       BytesPerLine,
                       &GUI_FontDataBuf[0],
                       &LCD_BKCOLORINDEX
                       );
        /* Fill empty pixel lines */
        if (GUI_pContext->pAFont->YDist > GUI_pContext->pAFont->YSize) 
        {
            int YMag = GUI_pContext->pAFont->YMag;
            int YDist = GUI_pContext->pAFont->YDist * YMag;
            int YSize = GUI_pContext->pAFont->YSize * YMag;
            if (DrawMode != LCD_DRAWMODE_TRANS) 
            {
                LCD_COLOR OldColor = GUI_GetColor();
                GUI_SetColor(GUI_GetBkColor());
                LCD_FillRect(GUI_pContext->DispPosX, GUI_pContext->DispPosY + YSize, 
                             GUI_pContext->DispPosX + pCharInfo->XSize, 
                             GUI_pContext->DispPosY + YDist);
                GUI_SetColor(OldColor);
            }
        }
        LCD_SetDrawMode(OldDrawMode); /* Restore draw mode */
		GUI_pContext->DispPosX += pCharInfo->XDist * GUI_pContext->pAFont->XMag;
    }
}

/*********************************************************************
*
*       GUIPROP_GetCharDistX
*/
int GUIPROP_X_GetCharDistX(U16P c) 
{
    const GUI_FONT_PROP GUI_UNI_PTR * pProp = GUI_pContext->pAFont->p.pProp;  
    for (; pProp; pProp = pProp->pNext)                                         
    {
        if ((c >= pProp->First) && (c <= pProp->Last))break;
    }
    return (pProp) ? (pProp->paCharInfo)->XSize * GUI_pContext->pAFont->XMag : 0;
}

