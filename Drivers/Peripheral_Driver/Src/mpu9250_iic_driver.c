#include "mpu9250_iic_driver.h"
#include "soft_iic_driver.h"
#include "utility.h"


/***************************************************************************************
  * @brief   ��ʼ��MPU9250
  * @input   
  * @return  ����ֵ:0,�ɹ�
***************************************************************************************/
uint8_t MPU9250_Init(void)
{
    uint8_t res=0;
    SOFT_IIC_Init();     //��ʼ��IIC����
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU9250
    delay_ms(100);  //��ʱ100ms
    MPU_Write_Byte(MPU9250_ADDR,MPU_PWR_MGMT1_REG,0X00);  //����MPU9250
    MPU_Set_Gyro_Fsr(3);					        	  //�����Ǵ�����,��2000dps
	MPU_Set_Accel_Fsr(0);					       	 	  //���ٶȴ�����,��2g
    MPU_Set_Rate(50);						       	 	  //���ò�����50Hz
    MPU_Write_Byte(MPU9250_ADDR, MPU_INT_EN_REG,    0X00);//�ر������ж�
	MPU_Write_Byte(MPU9250_ADDR, MPU_USER_CTRL_REG, 0X00);//I2C��ģʽ�ر�
	MPU_Write_Byte(MPU9250_ADDR, MPU_FIFO_EN_REG,   0X00);//�ر�FIFO
	MPU_Write_Byte(MPU9250_ADDR, MPU_INTBP_CFG_REG, 0X82);//INT���ŵ͵�ƽ��Ч������bypassģʽ������ֱ�Ӷ�ȡ������
    res = MPU_Read_Byte(MPU9250_ADDR, MPU_DEVICE_ID_REG); //��ȡMPU6500��ID
    if(res == MPU6500_ID1 || res == MPU6500_ID2) 		  //����ID��ȷ
    {
        MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT1_REG, 0X01);  	//����CLKSEL,PLL X��Ϊ�ο�
        MPU_Write_Byte(MPU9250_ADDR, MPU_PWR_MGMT2_REG, 0X00);  	//���ٶ��������Ƕ�����
		MPU_Set_Rate(50);						       			//���ò�����Ϊ50Hz   
    }else 
        return 1;
 
    res = MPU_Read_Byte(AK8963_ADDR, MAG_WIA);    			//��ȡAK8963 ID   
    if(res == AK8963_ID)
    {
        MPU_Write_Byte(AK8963_ADDR,MAG_CNTL2,0X01);		//��λAK8963
		delay_ms(50);
        MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11);		//����AK8963Ϊ���β���
    }else 
        return 1;

    return 0;
}


/***************************************************************************************
  * @brief   ����MPU9250�����Ǵ����������̷�Χ
  * @input   fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  * @return  ����ֵ:0,���óɹ�
***************************************************************************************/
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_GYRO_CFG_REG,(fsr<<3)|3);//���������������̷�Χ  
}


/***************************************************************************************
  * @brief   ����MPU9250���ٶȴ����������̷�Χ
  * @input   fsr:0,��2g;1,��4g;2,��8g;3,��16g
  * @return  ����ֵ:0,���óɹ�
***************************************************************************************/
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
	return MPU_Write_Byte(MPU9250_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);//���ü��ٶȴ����������̷�Χ  
}


/***************************************************************************************
  * @brief   ����MPU9250�����ֵ�ͨ�˲���
  * @input   lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
  * @return  ����ֵ:0,���óɹ�
***************************************************************************************/
uint8_t MPU_Set_LPF(uint16_t lpf)
{
	uint8_t data=0;
	if(lpf>=188)data=1;
	else if(lpf>=98)data=2;
	else if(lpf>=42)data=3;
	else if(lpf>=20)data=4;
	else if(lpf>=10)data=5;
	else data=6; 
	return MPU_Write_Byte(MPU9250_ADDR,MPU_CFG_REG,data);//�������ֵ�ͨ�˲���  
}


/***************************************************************************************
  * @brief   ����MPU9250�Ĳ�����(�ٶ�Fs=1KHz)
  * @input   rate:4~1000(Hz)
  * @return  ����ֵ:0,���óɹ�
***************************************************************************************/
uint8_t MPU_Set_Rate(uint16_t rate)
{
	uint8_t data;
	if(rate>1000)rate=1000;
	if(rate<4)rate=4;
	data=1000/rate-1;
	data=MPU_Write_Byte(MPU9250_ADDR,MPU_SAMPLE_RATE_REG,data);	//�������ֵ�ͨ�˲���
 	return MPU_Set_LPF(rate/2);	//�Զ�����LPFΪ�����ʵ�һ��
}


/***************************************************************************************
  * @brief   �õ��¶�ֵ
  * @input   
  * @return  ����ֵ:�¶�ֵ(������100��)
***************************************************************************************/
short MPU_Get_Temperature(void)
{
    uint8_t buf[2]; 
    short raw;
	float temp;
	MPU_Read_Len(MPU9250_ADDR,MPU_TEMP_OUTH_REG,2,buf); 
    raw=((uint16_t)buf[0]<<8)|buf[1];  
    temp=21+((double)raw)/333.87;  
    return temp*100;;
}


/***************************************************************************************
  * @brief   �õ�������ֵ(ԭʼֵ)
  * @input   gx,gy,gz:������x,y,z���ԭʼ����(������)
  * @return  ����ֵ:0,�ɹ�
***************************************************************************************/
uint8_t MPU_Get_Gyroscope(short *gx,short *gy,short *gz)
{
    uint8_t buf[6],res; 
	res=MPU_Read_Len(MPU9250_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
	if(res==0)
	{
		*gx=((uint16_t)buf[0]<<8)|buf[1];  
		*gy=((uint16_t)buf[2]<<8)|buf[3];  
		*gz=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


/***************************************************************************************
  * @brief   �õ����ٶ�ֵ(ԭʼֵ)
  * @input   gx,gy,gz:������x,y,z���ԭʼ����(������)
  * @return  ����ֵ:0,�ɹ�
***************************************************************************************/
uint8_t MPU_Get_Accelerometer(short *ax,short *ay,short *az)
{
    uint8_t buf[6],res;  
	res=MPU_Read_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
	if(res==0)
	{
		*ax=((uint16_t)buf[0]<<8)|buf[1];  
		*ay=((uint16_t)buf[2]<<8)|buf[3];  
		*az=((uint16_t)buf[4]<<8)|buf[5];
	} 	
    return res;;
}


/***************************************************************************************
  * @brief   �õ�������ֵ(ԭʼֵ)
  * @input   mx,my,mz:������x,y,z���ԭʼ����(������)
  * @return  ����ֵ:0,�ɹ�
***************************************************************************************/
uint8_t MPU_Get_Magnetometer(short *mx,short *my,short *mz)
{
    uint8_t buf[6],res;  
 	res=MPU_Read_Len(AK8963_ADDR,MAG_XOUT_L,6,buf);
	if(res==0)
	{
		*mx=((uint16_t)buf[1]<<8)|buf[0];  
		*my=((uint16_t)buf[3]<<8)|buf[2];  
		*mz=((uint16_t)buf[5]<<8)|buf[4];
	} 	 
	MPU_Write_Byte(AK8963_ADDR,MAG_CNTL1,0X11); //AK8963ÿ�ζ����Ժ���Ҫ��������Ϊ���β���ģʽ
    return res;;
}


/***************************************************************************************
  * @brief   IIC����д
  * @input   addr:������ַ 
             reg:�Ĵ�����ַ
             len:д�볤��
             buf:������
  * @return  ����ֵ:0,����
***************************************************************************************/
uint8_t MPU_Write_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{
    uint8_t i;
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //����������ַ+д����
    if(SOFT_IIC_Wait_Ack())          //�ȴ�Ӧ��
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Send_Byte(reg);         //д�Ĵ�����ַ
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    for(i=0;i<len;i++)
    {
        SOFT_IIC_Send_Byte(buf[i]);  //��������
        if(SOFT_IIC_Wait_Ack())      //�ȴ�ACK
        {
            SOFT_IIC_Stop();
            return 1;
        }
    }
    SOFT_IIC_Stop();
    return 0;
} 


/***************************************************************************************
  * @brief   IIC������
  * @input   addr:������ַ
             reg:Ҫ��ȡ�ļĴ�����ַ
             len:Ҫ��ȡ�ĳ���
             buf:��ȡ�������ݴ洢��
  * @return  ����ֵ:0,����
***************************************************************************************/
uint8_t MPU_Read_Len(uint8_t addr,uint8_t reg,uint8_t len,uint8_t *buf)
{ 
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //����������ַ+д����
    if(SOFT_IIC_Wait_Ack())          //�ȴ�Ӧ��
    {
        SOFT_IIC_Stop();
        return 1;
    }
    SOFT_IIC_Send_Byte(reg);         //д�Ĵ�����ַ
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
	SOFT_IIC_Start();                
    SOFT_IIC_Send_Byte((addr<<1)|1); //����������ַ+������
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    while(len)
    {
        if(len==1)*buf=SOFT_IIC_Read_Byte(0);//������,����nACK 
		else *buf=SOFT_IIC_Read_Byte(1);		//������,����ACK  
		len--;
		buf++;  
    }
    SOFT_IIC_Stop();                 //����һ��ֹͣ����
    return 0;       
}


/***************************************************************************************
  * @brief   IICдһ���ֽ� 
  * @input   devaddr:����IIC��ַ
             reg:�Ĵ�����ַ
             data:����
  * @return  ����ֵ:0,����
***************************************************************************************/
uint8_t MPU_Write_Byte(uint8_t addr,uint8_t reg,uint8_t data)
{
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //����������ַ+д����
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
  * @brief   IIC��һ���ֽ� 
  * @input   reg:�Ĵ�����ַ 
  * @return  ����ֵ:����������
***************************************************************************************/
uint8_t MPU_Read_Byte(uint8_t addr,uint8_t reg)
{
    uint8_t res;
    SOFT_IIC_Start();
    SOFT_IIC_Send_Byte((addr<<1)|0); //����������ַ+д����
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    SOFT_IIC_Send_Byte(reg);         //д�Ĵ�����ַ
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
	SOFT_IIC_Start();                
    SOFT_IIC_Send_Byte((addr<<1)|1); //����������ַ+������
    SOFT_IIC_Wait_Ack();             //�ȴ�Ӧ��
    res=SOFT_IIC_Read_Byte(0);		//������,����nACK  
    SOFT_IIC_Stop();                 //����һ��ֹͣ����
    return res;  
}


