/**
  ******************************************************************************
  * @file   
  * @author  
  * @brief   
  ******************************************************************************
  */ 
  
  
#include "stm32f7xx_hal.h"
#include "ap3216_iic_driver.h"
#include "soft_iic_driver.h"
#include "utility.h"


/***************************************************************************************
  * @brief   IICдһ���ֽ�  
  * @input   reg:�Ĵ�����ַ;     data:Ҫд�������
  * @return  0,����              ����,�������
***************************************************************************************/
uint8_t AP3216C_WriteOneByte(uint8_t reg,uint8_t data)
{
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte(AP3216C_ADDR|0X00);//����������ַ+д����
    if(SOFT_IIC_Wait_Ack())          //�ȴ�Ӧ��
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Send_Byte(reg);         //д�Ĵ�����ַ
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    SOFT_IIC_Send_Byte(data);        //��������
    if(SOFT_IIC_Wait_Ack())          //�ȴ�ACK
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Stop();
    return 0;
}

/***************************************************************************************
  * @brief   IIC���Ĵ���
  * @input   reg:�Ĵ�����ַ 
  * @return  ����ֵ:����������
***************************************************************************************/
uint8_t AP3216C_ReadOneByte(uint8_t reg)
{
    uint8_t res;
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte(AP3216C_ADDR|0X00); //����������ַ+д����
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    SOFT_IIC_Send_Byte(reg);         //д�Ĵ�����ַ
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
	SOFT_IIC_Start();                
    SOFT_IIC_Send_Byte(AP3216C_ADDR|0X01); //����������ַ+������
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    res = SOFT_IIC_Read_Byte(0);		//������,����nACK  
    SOFT_IIC_Stop();                 //����һ��ֹͣ����
    return res;  
}

/***************************************************************************************
  * @brief   ��ȡAP3216C��ALS,PS��IR���ݣ�
             ע�⣡���ͬʱ��ALS,IR+PS�Ļ��������ݶ�ȡ��ʱ����Ҫ����112.5ms
  * @input   
  * @return  
***************************************************************************************/
void AP3216C_ReadData(uint16_t* ir,uint16_t* ps,uint16_t* als)
{
    uint8_t buf[6];
    uint8_t i;
    for(i=0;i<6;i++)	
    {
        buf[i]=AP3216C_ReadOneByte(0X0A+i);		//ѭ����ȡ���д���������
    }
    if(buf[0]&0X80)*ir=0;						//IR_OFλΪ1,��������Ч
	else *ir=((uint16_t)buf[1]<<2)|(buf[0]&0X03); 	//��ȡIR������������  
	*als=((uint16_t)buf[3]<<8)|buf[2];				//��ȡALS������������   
    if(buf[4]&0x40)*ps=0;    					//IR_OFλΪ1,��������Ч
	else *ps=((uint16_t)(buf[5]&0X3F)<<4)|(buf[4]&0X0F); //��ȡPS������������   
}

/***************************************************************************************
  * @brief   
  * @input   
  * @return  
***************************************************************************************/
uint8_t AP3216C_Init(void)
{
	uint8_t temp=0;
    SOFT_IIC_Init();         				//��ʼ��IIC
    AP3216C_WriteOneByte(0x00,0X04);	//��λAP3216C
    delay_ms(50);						//AP33216C��λ����10ms
    AP3216C_WriteOneByte(0x00,0X03);	//����ALS��PS+IR   
	temp = AP3216C_ReadOneByte(0X00);		//��ȡ�ո�д��ȥ��0X03
	if(temp==0X03)
        return 0;				//AP3216C����
	else return 1;						//AP3216Cʧ��
} 




