#include "main.h"
#include "fatfs.h"

#define BMP_MAX_SIZE	(5120 * 1024)	//ͼƬ��С������5M,�ú궨���СӦ��GUIConf.c�е�GUI_NUMBYTESһ��

//����������ص�RAM�е�BMPͼƬʱ��ͼƬÿ�е��ֽ���
#define BMP_PERLINE_SIZE	2*1024	

static FIL PicFile;
static char bmpBuffer[BMP_PERLINE_SIZE];
/*********************************************************************
*
*       BmpGetData
*
* Function description
*   This routine is called by GUI_BMP_DrawEx(). The routine is responsible
*   for setting the data pointer to a valid data location with at least
*   one valid byte.
*
* Parameters:
*   p           - Pointer to application defined data.
*   NumBytesReq - Number of bytes requested.
*   ppData      - Pointer to data pointer. This pointer should be set to
*                 a valid location.
*   StartOfFile - If this flag is 1, the data pointer should be set to the
*                 beginning of the data stream.
*
* Return value:
*   Number of data bytes available.
*/
static int GetData(void * p, const U8 ** ppData, unsigned NumBytesReq, U32 Off) 
{
	static int readAddr = 0;
	FIL * phFile;
	UINT br;
	
	phFile = (FIL *)p;
	
	if (NumBytesReq > sizeof(bmpBuffer)) {
		NumBytesReq = sizeof(bmpBuffer);
	}

	//�ƶ�ָ�뵽Ӧ�ö�ȡ��λ��
	if(Off == 1) 
        readAddr = 0;
	else 
        readAddr = Off;

	f_lseek(phFile,readAddr);
	f_read(phFile, bmpBuffer, NumBytesReq, &br);//��ȡ���ݵ���������

	*ppData = (U8 *)bmpBuffer;
    
	return br;//���ض�ȡ�����ֽ���
}

/***************************************************************************************
  * @brief   
  * @input   BMPFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_bmp(char *fileName)
{
    int XSize,YSize;
    
    /*######1. ��bmp�ļ�,�ж��ļ���С#######*/
    retSD = f_open(&PicFile, (const TCHAR*)fileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######2. ��ȡbmpͼ��ߴ�#######*/
    XSize = GUI_BMP_GetXSizeEx(GetData, &PicFile);	//��ȡͼƬ��X���С
    YSize = GUI_BMP_GetYSizeEx(GetData, &PicFile);	//��ȡͼƬ��Y���С
    
    /*######3. ͨ��GUI_BMP_GetXSizeEx������LCD�м���ʾbmpͼ��#######*/
    GUI_BMP_DrawEx(GetData, &PicFile, (LCD_WIDTH-XSize)/2-1, (LCD_HEIGHT-YSize)/2-1);
    
    
    GUI_BMP_DrawScaledEx(GetData, &PicFile, (LCD_WIDTH - XSize * 0.5)/2, (LCD_HEIGHT - YSize * 0.5)/2, 1, 2);
    
    f_close(&PicFile);
    
    return 0;
}


/***************************************************************************************
  * @brief   
  * @input   JPGFileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_jpg(char *FileName)
{
    GUI_JPEG_INFO JpegInfo;
    
    /*######1. ��jpg�ļ�,�ж��ļ���С#######*/
    retSD = f_open(&PicFile, (const TCHAR*)FileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######3. ��LCD�м���ʾjpgͼ��#######*/
    GUI_JPEG_DrawEx(GetData, &PicFile,(LCD_WIDTH-JpegInfo.XSize)/2, (LCD_HEIGHT-JpegInfo.YSize)/2);
    GUI_Delay(500);
    GUI_Clear();
    
    /*######4. ��0.5����LCD�м���ʾjpgͼ��#######*/
    GUI_JPEG_DrawScaledEx(GetData, &PicFile, (LCD_WIDTH - JpegInfo.XSize * 0.5)/2, (LCD_HEIGHT - JpegInfo.YSize * 0.5)/2, 1, 2);
    
    f_close(&PicFile);
    return 0;
}


/***************************************************************************************
  * @brief   
  * @input   FileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_gif(char *FileName)
{
    UINT br;
    char *bmpBuf;
    GUI_GIF_INFO       GifInfo;
    GUI_GIF_IMAGE_INFO ImageInfo;
    
    /*######1. ��gif�ļ�,�ж��ļ���С#######*/
    retSD = f_open(&PicFile, (const TCHAR*)FileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######2. �����ڴ����ڱ���bmp�ļ�����#######*/
    bmpBuf = mem_malloc(PicFile.obj.objsize);
    if(bmpBuf == NULL){
        return 2;
    }
    
    /*######3. ���ļ����ݶ������õ�bmpBuf����#######*/
    retSD = f_read(&PicFile, bmpBuf, PicFile.obj.objsize, &br);
    if(retSD != FR_OK){
        return 3;
    }
    
    /*######4. ��ȡgif�ļ���Ϣ#######*/
    GUI_GIF_GetInfo(bmpBuf, PicFile.obj.objsize, &GifInfo);
    
    /*######5. ��LCD�м���ʾgifͼ��#######*/
    for(int i = 0; i < GifInfo.NumImages; i++)
    {
        GUI_MULTIBUF_Begin();
        GUI_GIF_DrawSub(bmpBuf, PicFile.obj.objsize, (LCD_WIDTH - GifInfo.xSize)/2, (LCD_HEIGHT - GifInfo.ySize)/2,i);
        GUI_GIF_GetImageInfo(bmpBuf, PicFile.obj.objsize, &ImageInfo, i);
        GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10 : 100 );//��ʱ
    }
    
    /*######6. ��0.5����ϵ��LCD�м���ʾgifͼ��#######*/
    for(int i=0; i < GifInfo.NumImages; i++)
    {
        GUI_GIF_DrawSubScaled(bmpBuf, PicFile.obj.objsize, (LCD_WIDTH - GifInfo.xSize * 0.5)/2, (LCD_HEIGHT - GifInfo.ySize * 0.5)/2, i, 1 ,2);
        GUI_GIF_GetImageInfo(bmpBuf, PicFile.obj.objsize, &ImageInfo, i);
        GUI_Delay(ImageInfo.Delay ? ImageInfo.Delay*10 : 100 );//��ʱ
    }
    
    mem_free(bmpBuf);
    f_close(&PicFile);
    return 0;
}
/***************************************************************************************
  * @brief   
  * @input   FileName:ͼƬ��SD�����������洢�豸�е�·��(���ļ�ϵͳ֧�֣�)
  * @return  
***************************************************************************************/
uint8_t ReadDisp_png(char *FileName)
{
    int XSize,YSize;
    
    /*######1. ��png�ļ�,�ж��ļ���С#######*/
    retSD = f_open(&PicFile, (const TCHAR*)FileName, FA_READ);
    if(retSD != FR_OK || PicFile.obj.objsize > BMP_MAX_SIZE){
        return 1;
    }
    
    /*######2. ��ȡpng�ߴ�#######*/
//    XSize = GUI_PNG_GetXSizeEx(GetData, &PicFile);//PNGͼƬX��С
//	YSize = GUI_PNG_GetYSizeEx(GetData, &PicFile);//PNGͼƬY��С
//    
//    /*######3. ��LCD�м���ʾpngͼ��#######*/
//    GUI_PNG_DrawEx(GetData,&PicFile,(LCD_WIDTH - XSize)/2,(LCD_HEIGHT - YSize)/2);
    
    f_close(&PicFile);
    return 0;
}

