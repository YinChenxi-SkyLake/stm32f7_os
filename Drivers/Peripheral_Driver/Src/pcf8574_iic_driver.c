#include "main.h"

//����I2C�ٶȵ���ʱ
void PCF8574_Delay(void)
{
    delay_us(2);
}

/***************************************************************************************
  * @brief  ���ݴ���оƬIIC�ӿڳ�ʼ��
***************************************************************************************/
void PCF8574_IIC_Init(void)
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
void PCF8574_IIC_Start(void)
{
    PCF8574_SDA_OUT();     //sda�����
    PCF8574_IIC_SCL(1);
    PCF8574_IIC_SDA(1);
    PCF8574_Delay();
    PCF8574_IIC_SDA(0);
}


/***************************************************************************************
  * @brief  ����IICֹͣ�ź�,��SCLΪ���ڼ䣬SDA�ɵ͵��ߵ�����
***************************************************************************************/
void PCF8574_IIC_Stop(void)
{
    PCF8574_SDA_OUT();//sda�����
    PCF8574_IIC_SCL(1);
    PCF8574_Delay();
    PCF8574_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    PCF8574_Delay();
    PCF8574_IIC_SDA(1);
}

/***************************************************************************************
  * @brief   �ȴ�Ӧ���źŵ���,�ȴ��������ݶ�����SDA
  * @input
  * @return  1������Ӧ��ʧ�� ; 0������Ӧ��ɹ�
***************************************************************************************/
uint8_t PCF8574_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    PCF8574_IIC_SDA(1);
    PCF8574_IIC_SCL(1);
    PCF8574_SDA_IN();      //SDA����Ϊ����
    while(PCF8574_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250) {
            PCF8574_IIC_Stop();
            return 1;
        }
        PCF8574_Delay();
    }
    PCF8574_IIC_SCL(0);
    return 0;
}

/***************************************************************************************
  * @brief  ����ACKӦ��,��SCL�ߵ�ƽ�ڼ䣬SDAΪ�͵�ƽ״̬
***************************************************************************************/
void PCF8574_IIC_Ack(void)
{
    PCF8574_IIC_SCL(0);
    PCF8574_Delay();
    PCF8574_SDA_OUT();
    PCF8574_IIC_SDA(0);
    PCF8574_IIC_SCL(1);
    PCF8574_Delay();
    PCF8574_IIC_SCL(0);
}

/***************************************************************************************
  * @brief  ������ACKӦ��
***************************************************************************************/
void PCF8574_IIC_NAck(void)
{
    PCF8574_IIC_SCL(0);
    PCF8574_Delay();
    PCF8574_SDA_OUT();
    PCF8574_IIC_SDA(1);
    PCF8574_IIC_SCL(1);
    PCF8574_Delay();
    PCF8574_IIC_SCL(0);
}

/***************************************************************************************
  * @brief   IIC����һ���ֽ�
  * @input
  * @return
***************************************************************************************/
void PCF8574_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    PCF8574_SDA_OUT();
    PCF8574_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    PCF8574_Delay();
    for(t = 0; t < 8; t++)
    {
        PCF8574_IIC_SDA((txd & 0x80) >> 7); //�ȷ��͸��ֽ�
        txd <<= 1;
        PCF8574_IIC_SCL(1);
        PCF8574_Delay();
        PCF8574_IIC_SCL(0);
        PCF8574_Delay();
    }
}

/***************************************************************************************
  * @brief   ��1���ֽ�
  * @input   ack=1ʱ������ACK��ack=0������nACK
  * @return
***************************************************************************************/
uint8_t PCF8574_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    PCF8574_SDA_IN();//SDA����Ϊ����
    PCF8574_Delay();
    for(i = 0; i < 8; i++ )
    {
        PCF8574_IIC_SCL(0);
        PCF8574_Delay();
        PCF8574_IIC_SCL(1);
        receive <<= 1;
        if(PCF8574_READ_SDA)receive++;
    }
    if (!ack)
        PCF8574_IIC_NAck();//����nACK
    else
        PCF8574_IIC_Ack(); //����ACK
    return receive;
}




