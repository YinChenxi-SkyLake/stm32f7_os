/**
  ******************************************************************************
  * @file
  * @author
  * @brief   GT9147���ݴ���������,ʹ�����ģ��IIC��ʽ����
  SCL -----> PH6
  SDA -----> PI3
  RST -----> PI8
  INT -----> PH7
  ******************************************************************************
  */
#include "main.h"

const uint16_t GT9147_TPX_TBL[5] = {GT_TP1_REG, GT_TP2_REG, GT_TP3_REG, GT_TP4_REG, GT_TP5_REG};

//GT9147���ò�����:��һ���ֽ�Ϊ�汾��(0X60),���뱣֤�µİ汾�Ŵ��ڵ���GT9147�ڲ�flashԭ�а汾��,�Ż��������.
const uint8_t GT9147_CFG_TBL[] =
{
    0x60, 0xe0, 0x01, 0x10, 0x01, 0x05, 0x0C, 0x00, 0x01, 0x08,
    0x28, 0x05, 0x50, 0x32, 0x03, 0x5F, 0x00, 0x00, 0xff, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x89, 0x28, 0x0a,
    0x17, 0x15, 0x31, 0x0d, 0x00, 0x00, 0x02, 0x9b, 0x03, 0x25,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 0x00,
    0x00, 0x0f, 0x94, 0x94, 0xc5, 0x02, 0x07, 0x00, 0x00, 0x04,
    0x8d, 0x13, 0x00, 0x5c, 0x1e, 0x00, 0x3c, 0x30, 0x00, 0x29,
    0x4c, 0x00, 0x1e, 0x78, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x08, 0x0a, 0x0c, 0x0e, 0x10, 0x12, 0x14, 0x16,
    0x18, 0x1a, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0x00, 0x02, 0x04, 0x05, 0x06, 0x08, 0x0a, 0x0c,
    0x0e, 0x1d, 0x1e, 0x1f, 0x20, 0x22, 0x24, 0x28, 0x29, 0xff,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    0xff, 0xff, 0xff, 0xff,
};


//����I2C�ٶȵ���ʱ
void CT_Delay(void)
{
    delay_us(2);
}

/***************************************************************************************
  * @brief  ���ݴ���оƬIIC�ӿڳ�ʼ��
***************************************************************************************/
void CT_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOH_CLK_ENABLE();			//����GPIOHʱ��
    __HAL_RCC_GPIOI_CLK_ENABLE();			//����GPIOIʱ��

    GPIO_Initure.Pin = GPIO_PIN_6;          //PH6
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    GPIO_Initure.Pull = GPIO_PULLUP;        //����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //����
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);    //��ʼ��

    GPIO_Initure.Pin = GPIO_PIN_3;          //PI3
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);    //��ʼ��
}

/***************************************************************************************
  * @brief  ����IIC��ʼ�ź�,��SCLΪ���ڼ䣬SDA�ɸߵ��͵�����
***************************************************************************************/
void CT_IIC_Start(void)
{
    CT_SDA_OUT();     //sda�����
    CT_IIC_SCL(1);
    CT_IIC_SDA(1);
    CT_Delay();
    CT_IIC_SDA(0);
}


/***************************************************************************************
  * @brief  ����IICֹͣ�ź�,��SCLΪ���ڼ䣬SDA�ɵ͵��ߵ�����
***************************************************************************************/
void CT_IIC_Stop(void)
{
    CT_SDA_OUT();//sda�����
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    CT_Delay();
    CT_IIC_SDA(1);
}

/***************************************************************************************
  * @brief   �ȴ�Ӧ���źŵ���,�ȴ��������ݶ�����SDA
  * @input
  * @return  1������Ӧ��ʧ�� ; 0������Ӧ��ɹ�
***************************************************************************************/
uint8_t CT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_SDA_IN();      //SDA����Ϊ����
    while(CT_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250) {
            CT_IIC_Stop();
            return 1;
        }
        CT_Delay();
    }
    CT_IIC_SCL(0);
    return 0;
}

/***************************************************************************************
  * @brief  ����ACKӦ��,��SCL�ߵ�ƽ�ڼ䣬SDAΪ�͵�ƽ״̬
***************************************************************************************/
void CT_IIC_Ack(void)
{
    CT_IIC_SCL(0);
    CT_Delay();
    CT_SDA_OUT();
    CT_IIC_SDA(0);
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief  ������ACKӦ��
***************************************************************************************/
void CT_IIC_NAck(void)
{
    CT_IIC_SCL(0);
    CT_Delay();
    CT_SDA_OUT();
    CT_IIC_SDA(1);
    CT_IIC_SCL(1);
    CT_Delay();
    CT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief   IIC����һ���ֽ�
  * @input
  * @return
***************************************************************************************/
void CT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    CT_SDA_OUT();
    CT_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    CT_Delay();
    for(t = 0; t < 8; t++)
    {
        CT_IIC_SDA((txd & 0x80) >> 7); //�ȷ��͸��ֽ�
        txd <<= 1;
        CT_IIC_SCL(1);
        CT_Delay();
        CT_IIC_SCL(0);
        CT_Delay();
    }
}

/***************************************************************************************
  * @brief   ��1���ֽ�
  * @input   ack=1ʱ������ACK��ack=0������nACK
  * @return
***************************************************************************************/
uint8_t CT_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    CT_SDA_IN();//SDA����Ϊ����
    CT_Delay();
    for(i = 0; i < 8; i++ )
    {
        CT_IIC_SCL(0);
        CT_Delay();
        CT_IIC_SCL(1);
        receive <<= 1;
        if(CT_READ_SDA)receive++;
    }
    if (!ack)
        CT_IIC_NAck();//����nACK
    else
        CT_IIC_Ack(); //����ACK
    return receive;
}


/***************************************************************************************
  * @brief   ��GT9147д��һ������
  * @input   reg:��ʼ�Ĵ�����ַ;
             buf:���ݻ�������
             len:д���ݳ���
  * @return  0,�ɹ�; 1,ʧ��.
***************************************************************************************/
uint8_t GT9147_WR_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    uint8_t ret = 0;
    CT_IIC_Start();
    CT_IIC_Send_Byte(GT_CMD_WR);   	//����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg >> 8);   	//���͸�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF);   	//���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    for(i = 0; i < len; i++)
    {
        CT_IIC_Send_Byte(buf[i]);  	//������
        ret = CT_IIC_Wait_Ack();
        if(ret)break;
    }
    CT_IIC_Stop();					//����һ��ֹͣ����
    return ret;
}


/***************************************************************************************
  * @brief   ��GT9147����һ������
  * @input   reg:��ʼ�Ĵ�����ַ
             buf:���ݻ�������
             len:�����ݳ���
  * @return
***************************************************************************************/
void GT9147_RD_Reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t i;
    CT_IIC_Start();
    CT_IIC_Send_Byte(GT_CMD_WR);   //����д����
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg >> 8);   	//���͸�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg & 0XFF);   	//���͵�8λ��ַ
    CT_IIC_Wait_Ack();
    CT_IIC_Start();
    CT_IIC_Send_Byte(GT_CMD_RD);   //���Ͷ�����
    CT_IIC_Wait_Ack();
    for(i = 0; i < len; i++)
    {
        buf[i] = CT_IIC_Read_Byte(i == (len - 1) ? 0 : 1); //������
    }
    CT_IIC_Stop();//����һ��ֹͣ����
}

/***************************************************************************************
  * @brief   ����GT9147���ò���
  * @input   0,���������浽flash;1,�������浽flash
  * @return
***************************************************************************************/
uint8_t GT9147_Send_Cfg(uint8_t mode)
{
    uint8_t buf[2];
    uint8_t i = 0;
    buf[0] = 0;
    buf[1] = mode;	//�Ƿ�д�뵽GT9147 FLASH?  ���Ƿ���籣��
    for(i = 0; i < sizeof(GT9147_CFG_TBL); i++)
        buf[0] += GT9147_CFG_TBL[i]; //����У���
    buf[0] = (~buf[0]) + 1;
    GT9147_WR_Reg(GT_CFGS_REG, (uint8_t*)GT9147_CFG_TBL, sizeof(GT9147_CFG_TBL)); //���ͼĴ�������
    GT9147_WR_Reg(GT_CHECK_REG, buf, 2); //д��У���,�����ø��±��
    return 0;
}


/***************************************************************************************
  * @brief   ��ʼ��GT9147������
  * @input
  * @return  0,��ʼ���ɹ�;1,��ʼ��ʧ��
***************************************************************************************/
uint8_t GT9147_Init(void)
{
    uint8_t temp[5];
    GPIO_InitTypeDef GPIO_Initure;
    GPIO_Initure.Pin = GPIO_PIN_7;          //PH7
    GPIO_Initure.Mode = GPIO_MODE_INPUT; //���
    GPIO_Initure.Pull = GPIO_PULLUP;        //����
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   //����
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);    //��ʼ��
    GPIO_Initure.Pin = GPIO_PIN_8;          //PI8
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; //�������
    HAL_GPIO_Init(GPIOI, &GPIO_Initure);    //��ʼ��
    CT_IIC_Init();      	//��ʼ����������I2C����

    GT_RST(0);				//��λ
    delay_ms(1);           //INT���ŵ�ƽά��100us����
    GT_RST(1);				//�ͷŸ�λ
    delay_ms(10);           //�ͷŸ�λ��ά��5ms���ϣ�����INTΪ��������
    GPIO_Initure.Pin = GPIO_PIN_7;
    GPIO_Initure.Mode = GPIO_MODE_IT_RISING;
    GPIO_Initure.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOH, &GPIO_Initure);    //��ʼ��
    delay_ms(60);

    GT9147_RD_Reg(GT_PID_REG, temp, 4); //��ȡ��ƷID
    temp[4] = 0;
    printf("CTP ID:0x%s\r\n", temp);	//��ӡID

    GT9147_Send_Cfg(1);//���²���������

    return 1;
}

/***************************************************************************************
  * @brief   PH7�ж�, �����ж�
  * @input
  * @return
***************************************************************************************/
void     LCD_LL_FillRect(int LayerIndex, int x0, int y0, int x1, int y1, U32 PixelIndex);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint8_t buf[4];
    uint8_t i = 0, mode = 0, temp = 0;
    uint16_t x[5], y[5];
    if(GPIO_Pin == GPIO_PIN_7) { //��ȡ����, �����һֱ��INT����
        GT9147_RD_Reg(GT_GSTID_REG, &mode, 1);	//��ȡ�������״̬
        if(mode & 0X80 && ((mode & 0XF) < 6)) //������ɶ�ȡ
        {
            temp = 0;
            GT9147_WR_Reg(GT_GSTID_REG, &temp, 1);//���־
        }
        if( (mode & 0xF) && ((mode & 0xF) < 6)) //�жϴ��������
        {
            for(i = 0; i < (mode & 0xF); i++)
            {
                GT9147_RD_Reg(GT9147_TPX_TBL[i], buf, 4);	//��ȡXY����ֵ
                x[i] = (((uint16_t)buf[1] << 8) + buf[0]);
                y[i] = (((uint16_t)buf[3] << 8) + buf[2]);
            }
            for(i = 0; i < (mode & 0xF); i++)
            {
                LCD_DrawPoint(x[i], y[i], i * 0xaa + 0xff);
//                printf("x=%d, y=%d\r\n",x[i],y[i]);
            }
        }
    }
}


