/**
  ******************************************************************************
  * @file   
  * @author  
  * @brief   PCF8574ΪIO��չоƬ��ʹ�����ģ��IIC����
  ******************************************************************************
  */
  
#include "pcf8574_iic_driver.h"
#include "stm32f7xx_hal.h"
#include "utility.h"
#include "soft_iic_driver.h"
#include "stdint.h"


//��ʼ��PCF8574
uint8_t PCF8574_Init(void)
{
    uint8_t temp=0;
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOB_CLK_ENABLE();           //ʹ��GPIOBʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_12;           //PB12
    GPIO_Initure.Mode=GPIO_MODE_INPUT;      //����
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);     //��ʼ��
    
    SOFT_IIC_Init();					    //IIC��ʼ��
    
	//���PCF8574�Ƿ���λ
    SOFT_IIC_Start();    	 	   
	SOFT_IIC_Send_Byte(PCF8574_ADDR);        //д��ַ			   
	temp = SOFT_IIC_Wait_Ack();		        //�ȴ�Ӧ��,ͨ���ж��Ƿ���ACKӦ��,���ж�PCF8574��״̬
    SOFT_IIC_Stop();					    //����һ��ֹͣ����
    PCF8574_WriteOneByte(0xFF);	            //Ĭ�����������IO����ߵ�ƽ
	return temp;
}

//��ȡPCF8574��8λIOֵ
//����ֵ:����������
uint8_t PCF8574_ReadOneByte(void)
{
	uint8_t temp=0;		  	    																 
    SOFT_IIC_Start();    	 	   
	SOFT_IIC_Send_Byte(PCF8574_ADDR|0X01);   //�������ģʽ			   
	SOFT_IIC_Wait_Ack();	 
    temp = SOFT_IIC_Read_Byte(0);		   
    SOFT_IIC_Stop();							//����һ��ֹͣ����	    
	return temp;
}


//��PCF8574д��8λIOֵ  
//DataToWrite:Ҫд�������
void PCF8574_WriteOneByte(uint8_t DataToWrite)
{				   	  	    																 
    SOFT_IIC_Start();  
    SOFT_IIC_Send_Byte(PCF8574_ADDR|0X00);   //����������ַ0X40,д���� 	 
	SOFT_IIC_Wait_Ack();	    										  		   
	SOFT_IIC_Send_Byte(DataToWrite);    	 	//�����ֽ�							   
	SOFT_IIC_Wait_Ack();      
    SOFT_IIC_Stop();							//����һ��ֹͣ���� 
	delay_ms(10);	 
}

//����PCF8574ĳ��IO�ĸߵ͵�ƽ
//bit:Ҫ���õ�IO���,0~7
//sta:IO��״̬;0��1
void PCF8574_WriteBit(uint8_t bit,uint8_t sta)
{
    uint8_t data;
    data = PCF8574_ReadOneByte(); //�ȶ���ԭ��������
    if(sta==0)
        data&=~(1<<bit);     
    else 
        data|=1<<bit;
    
    PCF8574_WriteOneByte(data); //д���µ�����
}

//��ȡPCF8574��ĳ��IO��ֵ
//bit��Ҫ��ȡ��IO���,0~7
//����ֵ:��IO��ֵ,0��1
uint8_t PCF8574_ReadBit(uint8_t bit)
{
    uint8_t data;
    data = PCF8574_ReadOneByte(); //�ȶ�ȡ���8λIO��ֵ 
    if(data&(1<<bit))
        return 1;
    else 
        return 0;   
}  

