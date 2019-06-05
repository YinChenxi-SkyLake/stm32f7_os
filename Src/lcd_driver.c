#include "main.h"


_lcd_dev lcddev;


/***************************************************************************************
  * @brief   设置xy坐标
  * @input   Xpos:横坐标   Ypos:纵坐标
  * @return
***************************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD->reg = (SET_X_CMD);
    LCD->data = (Xpos>>8); 		
    LCD->reg = (SET_X_CMD+1);
    LCD->data = (Xpos&0XFF);	
    
    LCD->reg = (SET_Y_CMD);
    LCD->data = (Ypos>>8);  		
    LCD->reg = (SET_Y_CMD+1);
    LCD->data = (Ypos&0XFF);
}

/***************************************************************************************
  * @brief   读取个某点的颜色值
  * @input   x,y:坐标
  * @return  返回值:此点的颜色
***************************************************************************************/
uint32_t LCD_ReadPoint(uint16_t x, uint16_t y)
{
    uint16_t r = 0, g = 0, b = 0;
    if(x >= LCD_WIDTH || y >= LCD_LENGTH)
        return 0;	//超过了范围,直接返回

    LCD_SetCursor(x, y);

    LCD->reg = MEM_READ;
    r = LCD->data;		//dummy Read
    r = LCD->data;  	//实际坐标颜色
    b = LCD->data;
    
    g = (r & 0XFF) << 8;		//第一次读取的是RG的值,R在前,G在后,各占8位
    return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); //公式转换一下
}


/***************************************************************************************
  * @brief   画点
  * @input   x,y:坐标  color:颜色
  * @return
***************************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint32_t color)
{
    LCD_SetCursor(x,y);
    
    LCD->reg = MEM_WRITE;
	LCD->data = color; 
}


/***************************************************************************************
  * @brief   在指定区域内填充单个颜色
  * @input   (sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
  * @input   color:要填充的颜色
  * @return
***************************************************************************************/
void LCD_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint32_t color)
{
    uint16_t i, j;
    uint16_t xlen = 0;
    xlen = ex - sx + 1;
    for(i = sy; i <= ey; i++)
    {
        LCD_SetCursor(sx, i);      				//设置光标位置
        LCD->reg = MEM_WRITE; ;     			//开始写入GRAM
        for(j = 0; j < xlen; j++)
            LCD->data = color;	//显示颜色
    }
}

void LCD_WriteReg(uint16_t reg, uint16_t value)
{
    LCD->reg = reg;		//写入要写的寄存器序号
    LCD->data = value;//写入数据
}

void LCD_ReadId(void)
{
    LCD->reg = (0xDA00);
    lcddev.id = LCD->data;		//读回0X00
    LCD->reg = (0xDB00);
    lcddev.id = LCD->data;		//读回0X80
    lcddev.id <<= 8;
    LCD->reg = (0xDC00);
    lcddev.id |= LCD->data;		//读回0X00
    if(lcddev.id == 0x8000)
        lcddev.id = 0x5510; //NT35510读回的ID是8000H,为方便区分,我们强制设置为5510
}



//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
/***************************************************************************************
  * @brief
  * @input
  * @return
***************************************************************************************/
void LCD_Scan_Dir(uint8_t dir)
{
    uint16_t regval = 0;
    if(dir == 1)
    {
        switch(dir)//方向转换
        {
        case 0:
            dir = 6;
            break;
        case 1:
            dir = 7;
            break;
        case 2:
            dir = 4;
            break;
        case 3:
            dir = 5;
            break;
        case 4:
            dir = 1;
            break;
        case 5:
            dir = 0;
            break;
        case 6:
            dir = 3;
            break;
        case 7:
            dir = 2;
            break;
        }
    }

    switch(dir)
    {
    case L2R_U2D://从左到右,从上到下
        regval |= (0 << 7) | (0 << 6) | (0 << 5);
        break;
    case L2R_D2U://从左到右,从下到上
        regval |= (1 << 7) | (0 << 6) | (0 << 5);
        break;
    case R2L_U2D://从右到左,从上到下
        regval |= (0 << 7) | (1 << 6) | (0 << 5);
        break;
    case R2L_D2U://从右到左,从下到上
        regval |= (1 << 7) | (1 << 6) | (0 << 5);
        break;
    case U2D_L2R://从上到下,从左到右
        regval |= (0 << 7) | (0 << 6) | (1 << 5);
        break;
    case U2D_R2L://从上到下,从右到左
        regval |= (0 << 7) | (1 << 6) | (1 << 5);
        break;
    case D2U_L2R://从下到上,从左到右
        regval |= (1 << 7) | (0 << 6) | (1 << 5);
        break;
    case D2U_R2L://从下到上,从右到左
        regval |= (1 << 7) | (1 << 6) | (1 << 5);
        break;
    }
    
    LCD_WriteReg(SET_DIR, regval);

    LCD->reg = (SET_X_CMD);
    LCD->data = (0);
    LCD->reg = (SET_X_CMD + 1);
    LCD->data = (0);
    LCD->reg = (SET_X_CMD + 2);
    LCD->data = ((lcddev.width - 1) >> 8);
    LCD->reg = (SET_X_CMD + 3);
    LCD->data = ((lcddev.width - 1) & 0XFF);
    LCD->reg = (SET_Y_CMD);
    LCD->data = (0);
    LCD->reg = (SET_Y_CMD + 1);
    LCD->data = (0);
    LCD->reg = (SET_Y_CMD + 2);
    LCD->data = ((lcddev.height - 1) >> 8);
    LCD->reg = (SET_Y_CMD + 3);
    LCD->data = ((lcddev.height - 1) & 0XFF);
}

//设置LCD显示方向
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(uint8_t dir)
{
    if(dir == 0)	//竖屏
    {
        lcddev.width = 480;
        lcddev.height = 800;
    } else {			//横屏
        lcddev.width = 800;
        lcddev.height = 480;
    }
    LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}

/***************************************************************************************
  * @brief  LCD初始化序列
  * @input
  * @return
***************************************************************************************/
void LCD_InitSequence(void)
{
    LCD_WriteReg(0xF000, 0x55);
    LCD_WriteReg(0xF001, 0xAA);
    LCD_WriteReg(0xF002, 0x52);
    LCD_WriteReg(0xF003, 0x08);
    LCD_WriteReg(0xF004, 0x01);
    //AVDD Set AVDD 5.2V
    LCD_WriteReg(0xB000, 0x0D);
    LCD_WriteReg(0xB001, 0x0D);
    LCD_WriteReg(0xB002, 0x0D);
    //AVDD ratio
    LCD_WriteReg(0xB600, 0x34);
    LCD_WriteReg(0xB601, 0x34);
    LCD_WriteReg(0xB602, 0x34);
    //AVEE -5.2V
    LCD_WriteReg(0xB100, 0x0D);
    LCD_WriteReg(0xB101, 0x0D);
    LCD_WriteReg(0xB102, 0x0D);
    //AVEE ratio
    LCD_WriteReg(0xB700, 0x34);
    LCD_WriteReg(0xB701, 0x34);
    LCD_WriteReg(0xB702, 0x34);
    //VCL -2.5V
    LCD_WriteReg(0xB200, 0x00);
    LCD_WriteReg(0xB201, 0x00);
    LCD_WriteReg(0xB202, 0x00);
    //VCL ratio
    LCD_WriteReg(0xB800, 0x24);
    LCD_WriteReg(0xB801, 0x24);
    LCD_WriteReg(0xB802, 0x24);
    //VGH 15V (Free pump)
    LCD_WriteReg(0xBF00, 0x01);
    LCD_WriteReg(0xB300, 0x0F);
    LCD_WriteReg(0xB301, 0x0F);
    LCD_WriteReg(0xB302, 0x0F);
    //VGH ratio
    LCD_WriteReg(0xB900, 0x34);
    LCD_WriteReg(0xB901, 0x34);
    LCD_WriteReg(0xB902, 0x34);
    //VGL_REG -10V
    LCD_WriteReg(0xB500, 0x08);
    LCD_WriteReg(0xB501, 0x08);
    LCD_WriteReg(0xB502, 0x08);
    LCD_WriteReg(0xC200, 0x03);
    //VGLX ratio
    LCD_WriteReg(0xBA00, 0x24);
    LCD_WriteReg(0xBA01, 0x24);
    LCD_WriteReg(0xBA02, 0x24);
    //VGMP/VGSP 4.5V/0V
    LCD_WriteReg(0xBC00, 0x00);
    LCD_WriteReg(0xBC01, 0x78);
    LCD_WriteReg(0xBC02, 0x00);
    //VGMN/VGSN -4.5V/0V
    LCD_WriteReg(0xBD00, 0x00);
    LCD_WriteReg(0xBD01, 0x78);
    LCD_WriteReg(0xBD02, 0x00);
    //VCOM
    LCD_WriteReg(0xBE00, 0x00);
    LCD_WriteReg(0xBE01, 0x64);
    //Gamma Setting
    LCD_WriteReg(0xD100, 0x00);
    LCD_WriteReg(0xD101, 0x33);
    LCD_WriteReg(0xD102, 0x00);
    LCD_WriteReg(0xD103, 0x34);
    LCD_WriteReg(0xD104, 0x00);
    LCD_WriteReg(0xD105, 0x3A);
    LCD_WriteReg(0xD106, 0x00);
    LCD_WriteReg(0xD107, 0x4A);
    LCD_WriteReg(0xD108, 0x00);
    LCD_WriteReg(0xD109, 0x5C);
    LCD_WriteReg(0xD10A, 0x00);
    LCD_WriteReg(0xD10B, 0x81);
    LCD_WriteReg(0xD10C, 0x00);
    LCD_WriteReg(0xD10D, 0xA6);
    LCD_WriteReg(0xD10E, 0x00);
    LCD_WriteReg(0xD10F, 0xE5);
    LCD_WriteReg(0xD110, 0x01);
    LCD_WriteReg(0xD111, 0x13);
    LCD_WriteReg(0xD112, 0x01);
    LCD_WriteReg(0xD113, 0x54);
    LCD_WriteReg(0xD114, 0x01);
    LCD_WriteReg(0xD115, 0x82);
    LCD_WriteReg(0xD116, 0x01);
    LCD_WriteReg(0xD117, 0xCA);
    LCD_WriteReg(0xD118, 0x02);
    LCD_WriteReg(0xD119, 0x00);
    LCD_WriteReg(0xD11A, 0x02);
    LCD_WriteReg(0xD11B, 0x01);
    LCD_WriteReg(0xD11C, 0x02);
    LCD_WriteReg(0xD11D, 0x34);
    LCD_WriteReg(0xD11E, 0x02);
    LCD_WriteReg(0xD11F, 0x67);
    LCD_WriteReg(0xD120, 0x02);
    LCD_WriteReg(0xD121, 0x84);
    LCD_WriteReg(0xD122, 0x02);
    LCD_WriteReg(0xD123, 0xA4);
    LCD_WriteReg(0xD124, 0x02);
    LCD_WriteReg(0xD125, 0xB7);
    LCD_WriteReg(0xD126, 0x02);
    LCD_WriteReg(0xD127, 0xCF);
    LCD_WriteReg(0xD128, 0x02);
    LCD_WriteReg(0xD129, 0xDE);
    LCD_WriteReg(0xD12A, 0x02);
    LCD_WriteReg(0xD12B, 0xF2);
    LCD_WriteReg(0xD12C, 0x02);
    LCD_WriteReg(0xD12D, 0xFE);
    LCD_WriteReg(0xD12E, 0x03);
    LCD_WriteReg(0xD12F, 0x10);
    LCD_WriteReg(0xD130, 0x03);
    LCD_WriteReg(0xD131, 0x33);
    LCD_WriteReg(0xD132, 0x03);
    LCD_WriteReg(0xD133, 0x6D);
    LCD_WriteReg(0xD200, 0x00);
    LCD_WriteReg(0xD201, 0x33);
    LCD_WriteReg(0xD202, 0x00);
    LCD_WriteReg(0xD203, 0x34);
    LCD_WriteReg(0xD204, 0x00);
    LCD_WriteReg(0xD205, 0x3A);
    LCD_WriteReg(0xD206, 0x00);
    LCD_WriteReg(0xD207, 0x4A);
    LCD_WriteReg(0xD208, 0x00);
    LCD_WriteReg(0xD209, 0x5C);
    LCD_WriteReg(0xD20A, 0x00);

    LCD_WriteReg(0xD20B, 0x81);
    LCD_WriteReg(0xD20C, 0x00);
    LCD_WriteReg(0xD20D, 0xA6);
    LCD_WriteReg(0xD20E, 0x00);
    LCD_WriteReg(0xD20F, 0xE5);
    LCD_WriteReg(0xD210, 0x01);
    LCD_WriteReg(0xD211, 0x13);
    LCD_WriteReg(0xD212, 0x01);
    LCD_WriteReg(0xD213, 0x54);
    LCD_WriteReg(0xD214, 0x01);
    LCD_WriteReg(0xD215, 0x82);
    LCD_WriteReg(0xD216, 0x01);
    LCD_WriteReg(0xD217, 0xCA);
    LCD_WriteReg(0xD218, 0x02);
    LCD_WriteReg(0xD219, 0x00);
    LCD_WriteReg(0xD21A, 0x02);
    LCD_WriteReg(0xD21B, 0x01);
    LCD_WriteReg(0xD21C, 0x02);
    LCD_WriteReg(0xD21D, 0x34);
    LCD_WriteReg(0xD21E, 0x02);
    LCD_WriteReg(0xD21F, 0x67);
    LCD_WriteReg(0xD220, 0x02);
    LCD_WriteReg(0xD221, 0x84);
    LCD_WriteReg(0xD222, 0x02);
    LCD_WriteReg(0xD223, 0xA4);
    LCD_WriteReg(0xD224, 0x02);
    LCD_WriteReg(0xD225, 0xB7);
    LCD_WriteReg(0xD226, 0x02);
    LCD_WriteReg(0xD227, 0xCF);
    LCD_WriteReg(0xD228, 0x02);
    LCD_WriteReg(0xD229, 0xDE);
    LCD_WriteReg(0xD22A, 0x02);
    LCD_WriteReg(0xD22B, 0xF2);
    LCD_WriteReg(0xD22C, 0x02);
    LCD_WriteReg(0xD22D, 0xFE);
    LCD_WriteReg(0xD22E, 0x03);
    LCD_WriteReg(0xD22F, 0x10);
    LCD_WriteReg(0xD230, 0x03);
    LCD_WriteReg(0xD231, 0x33);
    LCD_WriteReg(0xD232, 0x03);
    LCD_WriteReg(0xD233, 0x6D);
    LCD_WriteReg(0xD300, 0x00);
    LCD_WriteReg(0xD301, 0x33);
    LCD_WriteReg(0xD302, 0x00);
    LCD_WriteReg(0xD303, 0x34);
    LCD_WriteReg(0xD304, 0x00);
    LCD_WriteReg(0xD305, 0x3A);
    LCD_WriteReg(0xD306, 0x00);
    LCD_WriteReg(0xD307, 0x4A);
    LCD_WriteReg(0xD308, 0x00);
    LCD_WriteReg(0xD309, 0x5C);
    LCD_WriteReg(0xD30A, 0x00);

    LCD_WriteReg(0xD30B, 0x81);
    LCD_WriteReg(0xD30C, 0x00);
    LCD_WriteReg(0xD30D, 0xA6);
    LCD_WriteReg(0xD30E, 0x00);
    LCD_WriteReg(0xD30F, 0xE5);
    LCD_WriteReg(0xD310, 0x01);
    LCD_WriteReg(0xD311, 0x13);
    LCD_WriteReg(0xD312, 0x01);
    LCD_WriteReg(0xD313, 0x54);
    LCD_WriteReg(0xD314, 0x01);
    LCD_WriteReg(0xD315, 0x82);
    LCD_WriteReg(0xD316, 0x01);
    LCD_WriteReg(0xD317, 0xCA);
    LCD_WriteReg(0xD318, 0x02);
    LCD_WriteReg(0xD319, 0x00);
    LCD_WriteReg(0xD31A, 0x02);
    LCD_WriteReg(0xD31B, 0x01);
    LCD_WriteReg(0xD31C, 0x02);
    LCD_WriteReg(0xD31D, 0x34);
    LCD_WriteReg(0xD31E, 0x02);
    LCD_WriteReg(0xD31F, 0x67);
    LCD_WriteReg(0xD320, 0x02);
    LCD_WriteReg(0xD321, 0x84);
    LCD_WriteReg(0xD322, 0x02);
    LCD_WriteReg(0xD323, 0xA4);
    LCD_WriteReg(0xD324, 0x02);
    LCD_WriteReg(0xD325, 0xB7);
    LCD_WriteReg(0xD326, 0x02);
    LCD_WriteReg(0xD327, 0xCF);
    LCD_WriteReg(0xD328, 0x02);
    LCD_WriteReg(0xD329, 0xDE);
    LCD_WriteReg(0xD32A, 0x02);
    LCD_WriteReg(0xD32B, 0xF2);
    LCD_WriteReg(0xD32C, 0x02);
    LCD_WriteReg(0xD32D, 0xFE);
    LCD_WriteReg(0xD32E, 0x03);
    LCD_WriteReg(0xD32F, 0x10);
    LCD_WriteReg(0xD330, 0x03);
    LCD_WriteReg(0xD331, 0x33);
    LCD_WriteReg(0xD332, 0x03);
    LCD_WriteReg(0xD333, 0x6D);
    LCD_WriteReg(0xD400, 0x00);
    LCD_WriteReg(0xD401, 0x33);
    LCD_WriteReg(0xD402, 0x00);
    LCD_WriteReg(0xD403, 0x34);
    LCD_WriteReg(0xD404, 0x00);
    LCD_WriteReg(0xD405, 0x3A);
    LCD_WriteReg(0xD406, 0x00);
    LCD_WriteReg(0xD407, 0x4A);
    LCD_WriteReg(0xD408, 0x00);
    LCD_WriteReg(0xD409, 0x5C);
    LCD_WriteReg(0xD40A, 0x00);
    LCD_WriteReg(0xD40B, 0x81);

    LCD_WriteReg(0xD40C, 0x00);
    LCD_WriteReg(0xD40D, 0xA6);
    LCD_WriteReg(0xD40E, 0x00);
    LCD_WriteReg(0xD40F, 0xE5);
    LCD_WriteReg(0xD410, 0x01);
    LCD_WriteReg(0xD411, 0x13);
    LCD_WriteReg(0xD412, 0x01);
    LCD_WriteReg(0xD413, 0x54);
    LCD_WriteReg(0xD414, 0x01);
    LCD_WriteReg(0xD415, 0x82);
    LCD_WriteReg(0xD416, 0x01);
    LCD_WriteReg(0xD417, 0xCA);
    LCD_WriteReg(0xD418, 0x02);
    LCD_WriteReg(0xD419, 0x00);
    LCD_WriteReg(0xD41A, 0x02);
    LCD_WriteReg(0xD41B, 0x01);
    LCD_WriteReg(0xD41C, 0x02);
    LCD_WriteReg(0xD41D, 0x34);
    LCD_WriteReg(0xD41E, 0x02);
    LCD_WriteReg(0xD41F, 0x67);
    LCD_WriteReg(0xD420, 0x02);
    LCD_WriteReg(0xD421, 0x84);
    LCD_WriteReg(0xD422, 0x02);
    LCD_WriteReg(0xD423, 0xA4);
    LCD_WriteReg(0xD424, 0x02);
    LCD_WriteReg(0xD425, 0xB7);
    LCD_WriteReg(0xD426, 0x02);
    LCD_WriteReg(0xD427, 0xCF);
    LCD_WriteReg(0xD428, 0x02);
    LCD_WriteReg(0xD429, 0xDE);
    LCD_WriteReg(0xD42A, 0x02);
    LCD_WriteReg(0xD42B, 0xF2);
    LCD_WriteReg(0xD42C, 0x02);
    LCD_WriteReg(0xD42D, 0xFE);
    LCD_WriteReg(0xD42E, 0x03);
    LCD_WriteReg(0xD42F, 0x10);
    LCD_WriteReg(0xD430, 0x03);
    LCD_WriteReg(0xD431, 0x33);
    LCD_WriteReg(0xD432, 0x03);
    LCD_WriteReg(0xD433, 0x6D);
    LCD_WriteReg(0xD500, 0x00);
    LCD_WriteReg(0xD501, 0x33);
    LCD_WriteReg(0xD502, 0x00);
    LCD_WriteReg(0xD503, 0x34);
    LCD_WriteReg(0xD504, 0x00);
    LCD_WriteReg(0xD505, 0x3A);
    LCD_WriteReg(0xD506, 0x00);
    LCD_WriteReg(0xD507, 0x4A);
    LCD_WriteReg(0xD508, 0x00);
    LCD_WriteReg(0xD509, 0x5C);
    LCD_WriteReg(0xD50A, 0x00);
    LCD_WriteReg(0xD50B, 0x81);

    LCD_WriteReg(0xD50C, 0x00);
    LCD_WriteReg(0xD50D, 0xA6);
    LCD_WriteReg(0xD50E, 0x00);
    LCD_WriteReg(0xD50F, 0xE5);
    LCD_WriteReg(0xD510, 0x01);
    LCD_WriteReg(0xD511, 0x13);
    LCD_WriteReg(0xD512, 0x01);
    LCD_WriteReg(0xD513, 0x54);
    LCD_WriteReg(0xD514, 0x01);
    LCD_WriteReg(0xD515, 0x82);
    LCD_WriteReg(0xD516, 0x01);
    LCD_WriteReg(0xD517, 0xCA);
    LCD_WriteReg(0xD518, 0x02);
    LCD_WriteReg(0xD519, 0x00);
    LCD_WriteReg(0xD51A, 0x02);
    LCD_WriteReg(0xD51B, 0x01);
    LCD_WriteReg(0xD51C, 0x02);
    LCD_WriteReg(0xD51D, 0x34);
    LCD_WriteReg(0xD51E, 0x02);
    LCD_WriteReg(0xD51F, 0x67);
    LCD_WriteReg(0xD520, 0x02);
    LCD_WriteReg(0xD521, 0x84);
    LCD_WriteReg(0xD522, 0x02);
    LCD_WriteReg(0xD523, 0xA4);
    LCD_WriteReg(0xD524, 0x02);
    LCD_WriteReg(0xD525, 0xB7);
    LCD_WriteReg(0xD526, 0x02);
    LCD_WriteReg(0xD527, 0xCF);
    LCD_WriteReg(0xD528, 0x02);
    LCD_WriteReg(0xD529, 0xDE);
    LCD_WriteReg(0xD52A, 0x02);
    LCD_WriteReg(0xD52B, 0xF2);
    LCD_WriteReg(0xD52C, 0x02);
    LCD_WriteReg(0xD52D, 0xFE);
    LCD_WriteReg(0xD52E, 0x03);
    LCD_WriteReg(0xD52F, 0x10);
    LCD_WriteReg(0xD530, 0x03);
    LCD_WriteReg(0xD531, 0x33);
    LCD_WriteReg(0xD532, 0x03);
    LCD_WriteReg(0xD533, 0x6D);
    LCD_WriteReg(0xD600, 0x00);
    LCD_WriteReg(0xD601, 0x33);
    LCD_WriteReg(0xD602, 0x00);
    LCD_WriteReg(0xD603, 0x34);
    LCD_WriteReg(0xD604, 0x00);
    LCD_WriteReg(0xD605, 0x3A);
    LCD_WriteReg(0xD606, 0x00);
    LCD_WriteReg(0xD607, 0x4A);
    LCD_WriteReg(0xD608, 0x00);
    LCD_WriteReg(0xD609, 0x5C);
    LCD_WriteReg(0xD60A, 0x00);
    LCD_WriteReg(0xD60B, 0x81);

    LCD_WriteReg(0xD60C, 0x00);
    LCD_WriteReg(0xD60D, 0xA6);
    LCD_WriteReg(0xD60E, 0x00);
    LCD_WriteReg(0xD60F, 0xE5);
    LCD_WriteReg(0xD610, 0x01);
    LCD_WriteReg(0xD611, 0x13);
    LCD_WriteReg(0xD612, 0x01);
    LCD_WriteReg(0xD613, 0x54);
    LCD_WriteReg(0xD614, 0x01);
    LCD_WriteReg(0xD615, 0x82);
    LCD_WriteReg(0xD616, 0x01);
    LCD_WriteReg(0xD617, 0xCA);
    LCD_WriteReg(0xD618, 0x02);
    LCD_WriteReg(0xD619, 0x00);
    LCD_WriteReg(0xD61A, 0x02);
    LCD_WriteReg(0xD61B, 0x01);
    LCD_WriteReg(0xD61C, 0x02);
    LCD_WriteReg(0xD61D, 0x34);
    LCD_WriteReg(0xD61E, 0x02);
    LCD_WriteReg(0xD61F, 0x67);
    LCD_WriteReg(0xD620, 0x02);
    LCD_WriteReg(0xD621, 0x84);
    LCD_WriteReg(0xD622, 0x02);
    LCD_WriteReg(0xD623, 0xA4);
    LCD_WriteReg(0xD624, 0x02);
    LCD_WriteReg(0xD625, 0xB7);
    LCD_WriteReg(0xD626, 0x02);
    LCD_WriteReg(0xD627, 0xCF);
    LCD_WriteReg(0xD628, 0x02);
    LCD_WriteReg(0xD629, 0xDE);
    LCD_WriteReg(0xD62A, 0x02);
    LCD_WriteReg(0xD62B, 0xF2);
    LCD_WriteReg(0xD62C, 0x02);
    LCD_WriteReg(0xD62D, 0xFE);
    LCD_WriteReg(0xD62E, 0x03);
    LCD_WriteReg(0xD62F, 0x10);
    LCD_WriteReg(0xD630, 0x03);
    LCD_WriteReg(0xD631, 0x33);
    LCD_WriteReg(0xD632, 0x03);
    LCD_WriteReg(0xD633, 0x6D);
    //LV2 Page 0 enable
    LCD_WriteReg(0xF000, 0x55);
    LCD_WriteReg(0xF001, 0xAA);
    LCD_WriteReg(0xF002, 0x52);
    LCD_WriteReg(0xF003, 0x08);
    LCD_WriteReg(0xF004, 0x00);
    //Display control
    LCD_WriteReg(0xB100, 0xCC);
    LCD_WriteReg(0xB101, 0x00);
    //Source hold time
    LCD_WriteReg(0xB600, 0x05);
    //Gate EQ control
    LCD_WriteReg(0xB700, 0x70);
    LCD_WriteReg(0xB701, 0x70);
    //Source EQ control (Mode 2)
    LCD_WriteReg(0xB800, 0x01);
    LCD_WriteReg(0xB801, 0x03);
    LCD_WriteReg(0xB802, 0x03);
    LCD_WriteReg(0xB803, 0x03);
    //Inversion mode (2-dot)
    LCD_WriteReg(0xBC00, 0x02);
    LCD_WriteReg(0xBC01, 0x00);
    LCD_WriteReg(0xBC02, 0x00);
    //Timing control 4H w/ 4-delay
    LCD_WriteReg(0xC900, 0xD0);
    LCD_WriteReg(0xC901, 0x02);
    LCD_WriteReg(0xC902, 0x50);
    LCD_WriteReg(0xC903, 0x50);
    LCD_WriteReg(0xC904, 0x50);
    LCD_WriteReg(0x3500, 0x00);
    LCD_WriteReg(0x3A00, 0x55); //16-bit/pixel
    LCD->reg = (0x1100);
    delay_us(120);
    LCD->reg = (0x2900);
    LCD_Display_Dir(0);		//默认为竖屏
    HAL_GPIO_WritePin(GPIOB, LCD_BL_Pin, GPIO_PIN_SET);
}



