/**
  ******************************************************************************
  * @file   
  * @author  
  * @brief   
  ******************************************************************************
  */ 
#include "soft_iic_driver.h"
#include "utility.h"
#include "stm32f7xx_hal.h"

//����I2C�ٶȵ���ʱ
void SOFT_IIC_Delay(void)
{
    delay_us(2);
}

/***************************************************************************************
  * @brief  ���ݴ���оƬIIC�ӿڳ�ʼ��
***************************************************************************************/
void SOFT_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOH_CLK_ENABLE();   //ʹ��GPIOHʱ��
    
    //PH4,5��ʼ������
    GPIO_Initure.Pin = SOFT_IIC_SCL_PIN | SOFT_IIC_SDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull = GPIO_PULLUP;          //����
    GPIO_Initure.Speed = GPIO_SPEED_FAST;     //����
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
    
    SOFT_IIC_SDA(1);
    SOFT_IIC_SCL(1);  
}

/***************************************************************************************
  * @brief  ����IIC��ʼ�ź�,��SCLΪ���ڼ䣬SDA�ɸߵ��͵�����
***************************************************************************************/
void SOFT_IIC_Start(void)
{
    SOFT_IIC_SDA_OUT();     //sda�����
    SOFT_IIC_SCL(1);
    SOFT_IIC_SDA(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA(0);
}


/***************************************************************************************
  * @brief  ����IICֹͣ�ź�,��SCLΪ���ڼ䣬SDA�ɵ͵��ߵ�����
***************************************************************************************/
void SOFT_IIC_Stop(void)
{
    SOFT_IIC_SDA_OUT();//sda�����
    SOFT_IIC_SCL(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA(0);//STOP:when CLK is high DATA change form low to high
    SOFT_IIC_Delay();
    SOFT_IIC_SDA(1);
}

/***************************************************************************************
  * @brief   �ȴ�Ӧ���źŵ���,�ȴ��������ݶ�����SDA
  * @input
  * @return  1������Ӧ��ʧ�� ; 0������Ӧ��ɹ�
***************************************************************************************/
uint8_t SOFT_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    SOFT_IIC_SDA(1);
    SOFT_IIC_SCL(1);
    SOFT_IIC_SDA_IN();      //SDA����Ϊ����
    while(SOFT_IIC_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250) {
            SOFT_IIC_Stop();
            return 1;
        }
        SOFT_IIC_Delay();
    }
    SOFT_IIC_SCL(0);
    return 0;
}

/***************************************************************************************
  * @brief  ����ACKӦ��,��SCL�ߵ�ƽ�ڼ䣬SDAΪ�͵�ƽ״̬
***************************************************************************************/
void SOFT_IIC_Ack(void)
{
    SOFT_IIC_SCL(0);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA_OUT();
    SOFT_IIC_SDA(0);
    SOFT_IIC_SCL(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief  ������ACKӦ��
***************************************************************************************/
void SOFT_IIC_NAck(void)
{
    SOFT_IIC_SCL(0);
    SOFT_IIC_Delay();
    SOFT_IIC_SDA_OUT();
    SOFT_IIC_SDA(1);
    SOFT_IIC_SCL(1);
    SOFT_IIC_Delay();
    SOFT_IIC_SCL(0);
}

/***************************************************************************************
  * @brief   IIC����һ���ֽ�
  * @input
  * @return
***************************************************************************************/
void SOFT_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    SOFT_IIC_SDA_OUT();
    SOFT_IIC_SCL(0);//����ʱ�ӿ�ʼ���ݴ���
    SOFT_IIC_Delay();
    for(t = 0; t < 8; t++)
    {
        SOFT_IIC_SDA((txd & 0x80) >> 7); //�ȷ��͸��ֽ�
        txd <<= 1;
        SOFT_IIC_SCL(1);
        SOFT_IIC_Delay();
        SOFT_IIC_SCL(0);
        SOFT_IIC_Delay();
    }
}

/***************************************************************************************
  * @brief   ��1���ֽ�
  * @input   ack=1ʱ������ACK��ack=0������nACK
  * @return
***************************************************************************************/
uint8_t SOFT_IIC_Read_Byte(unsigned char ack)
{
    uint8_t i, receive = 0;
    SOFT_IIC_SDA_IN();//SDA����Ϊ����
    SOFT_IIC_Delay();
    for(i = 0; i < 8; i++ )
    {
        SOFT_IIC_SCL(0);
        SOFT_IIC_Delay();
        SOFT_IIC_SCL(1);
        receive <<= 1;
        if(SOFT_IIC_READ_SDA)receive++;
    }
    if (!ack)
        SOFT_IIC_NAck();//����nACK
    else
        SOFT_IIC_Ack(); //����ACK
    return receive;
}
