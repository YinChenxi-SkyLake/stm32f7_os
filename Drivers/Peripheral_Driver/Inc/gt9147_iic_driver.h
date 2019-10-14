#ifndef __GT9147_IIC_DRIVER_H
#define __GT9147_IIC_DRIVER_H

#define GT9147_SDA_GPIO  GPIOI
#define GT9147_SDA_PIN   GPIO_PIN_3
#define GT9147_SCL_GPIO  GPIOH
#define GT9147_SCL_PIN   GPIO_PIN_6

#define CT_SDA_IN()  MODIFY_REG(GT9147_SDA_GPIO->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(GT9147_SDA_PIN) * 2U)), (GPIO_MODE_INPUT << (POSITION_VAL(GT9147_SDA_PIN) * 2U)));
#define CT_SDA_OUT() MODIFY_REG(GT9147_SDA_GPIO->MODER, (GPIO_MODER_MODER0 << (POSITION_VAL(GT9147_SDA_PIN) * 2U)), (GPIO_MODE_OUTPUT_PP << (POSITION_VAL(GT9147_SDA_PIN) * 2U)));

//IO��������	 
#define CT_IIC_SCL(n) (n ? HAL_GPIO_WritePin(GT9147_SCL_GPIO, GT9147_SCL_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(GT9147_SCL_GPIO, GT9147_SCL_PIN, GPIO_PIN_RESET))//SCL
#define CT_IIC_SDA(n) (n ? HAL_GPIO_WritePin(GT9147_SDA_GPIO, GT9147_SDA_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(GT9147_SDA_GPIO, GT9147_SDA_PIN, GPIO_PIN_RESET))//SDA	 
#define CT_READ_SDA   HAL_GPIO_ReadPin(GT9147_SDA_GPIO,GT9147_SDA_PIN)//����SDA 


//IO��������	 
#define GT_RST(n)  (n?HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_SET):HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,GPIO_PIN_RESET))//GT9147��λ����
#define GT_INT      HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)  //GT9147�ж�����		
 
//I2C��д����	
#define GT_CMD_WR 		0X28     	//д����
#define GT_CMD_RD 		0X29		//������

//GT9147 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT9147���ƼĴ���
#define GT_CFGS_REG 	0X8047   	//GT9147������ʼ��ַ�Ĵ���
#define GT_CHECK_REG 	0X80FF   	//GT9147У��ͼĴ���
#define GT_PID_REG 		0X8140   	//GT9147��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT9147��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X8150  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8158		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X8160		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8168		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X8170		//��������������ݵ�ַ  



extern unsigned short touch_x,touch_y;

unsigned char GT9147_Init(void);


#endif
