#ifndef __AP3216_IIC_DRIVER_H
#define __AP3216_IIC_DRIVER_H

#define AP3216C_ADDR    0X3C	//AP3216C����IIC��ַ(������һλ)



unsigned char AP3216C_Init(void);
void AP3216C_ReadData(unsigned short* ir,unsigned short* ps,unsigned short* als);


#endif
