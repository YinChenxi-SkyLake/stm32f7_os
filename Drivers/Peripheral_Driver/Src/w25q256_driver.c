#include "main.h"

extern QSPI_HandleTypeDef hqspi;

#define W25Q_RESET()             QSPI_SendCmd(RESET_ENABLE_CMD,QSPI_INSTRUCTION_1_LINE,0,0,0,0);\
                                 QSPI_SendCmd(RESET_MEMORY_CMD,QSPI_INSTRUCTION_1_LINE,0,0,0,0)
#define W25QXX_ENTER_QSPI()      QSPI_SendCmd(W25X_EnterQPIMode,QSPI_INSTRUCTION_1_LINE,0,0,0,0)
#define W25Q_WRITE_ENABLE()      QSPI_SendCmd(W25X_WriteEnable,QSPI_INSTRUCTION_4_LINES,0,0,0,0)
#define W25QXX_ENTER_4BYTEADDR() QSPI_SendCmd(W25X_Enable4ByteAddr,QSPI_INSTRUCTION_4_LINES,0,0,0,0)


/***************************************************************************************
  * @brief   ����ָ��
  * @input   
  * @return  
***************************************************************************************/
void QSPI_SendCmd(uint32_t cmd,uint32_t cmdMode,uint32_t addr,uint32_t addrMode,uint32_t addrSize,uint32_t dataMode)
{
    QSPI_CommandTypeDef s_command = {0};
    
    s_command.Instruction     = cmd;          //ָ��
    s_command.InstructionMode = cmdMode;	  //ָ��ģʽ
    s_command.Address         = addr;         //��ַ
    s_command.AddressMode     = addrMode;     //��ַģʽ
    s_command.AddressSize     = addrSize;     //��ַ����
    s_command.DataMode        = dataMode;     //����ģʽ
    
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}


/***************************************************************************************
  * @brief   QSPI����ָ�����ȵ�����
  * @input   buf:�������ݻ������׵�ַ
             len:Ҫ��������ݳ���
  * @return  
***************************************************************************************/
void QSPI_Receive(uint8_t* buf,uint32_t len)
{
    hqspi.Instance->DLR = len - 1;                 //�������ݳ���
    if(HAL_QSPI_Receive(&hqspi, buf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}


/***************************************************************************************
  * @brief   QSPI����ָ�����ȵ�����
  * @input   buf:�������ݻ������׵�ַ
             len:Ҫ��������ݳ���
  * @return  
***************************************************************************************/
void QSPI_Transmit(uint8_t* buf,uint32_t len)
{
    hqspi.Instance->DLR = len - 1;                            //�������ݳ���
    if(HAL_QSPI_Transmit(&hqspi, buf, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}


/***************************************************************************************
  * @brief   W25QXX����QSPIģʽ
  * @input
  * @return
***************************************************************************************/
void W25QXX_SetQE(void)
{
    uint8_t value = 0x02;
    /* 1.дʹ�� */
    QSPI_SendCmd(W25X_WriteEnable,QSPI_INSTRUCTION_1_LINE,0,0,0,0);
    /* 2.����д״̬�Ĵ���2���� */
    QSPI_SendCmd(W25X_WriteStatusReg2,QSPI_INSTRUCTION_1_LINE,0,0,0,QSPI_DATA_1_LINE);
    /* 3.�������� */
    QSPI_Transmit(&value, 1);
}


/***************************************************************************************
  * @brief   0XEF18,��ʾоƬ�ͺ�ΪW25Q256
  * @input
  * @return
***************************************************************************************/
uint16_t W25QXX_ReadId(void)
{
    uint8_t pData[2];

    QSPI_SendCmd(W25X_ManufactDeviceID,QSPI_INSTRUCTION_4_LINES,
                 0,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_24_BITS,QSPI_DATA_4_LINES);
    QSPI_Receive(pData,2);

    return pData[1] | ( pData[0] << 8 );
}

/***************************************************************************************
  * @brief   ���ö�����
  * @input   
  * @return  
***************************************************************************************/
void W25QXX_SetReadParam(void)
{
    uint8_t para = 3 << 4;
    
    /* 1.�������� */
    QSPI_SendCmd(W25X_SetReadParam,QSPI_INSTRUCTION_4_LINES,0,0,0,QSPI_DATA_4_LINES);
    /* 2.�������� */
    QSPI_Transmit(&para, 1);
}


/***************************************************************************************
  * @brief  �ȴ�����
  * @input   
  * @return  
***************************************************************************************/
void W25QXX_WaitIdle(void)   
{
    uint8_t sta_reg1 = 0x00;
    do{
        QSPI_SendCmd(W25X_ReadStatusReg1,QSPI_INSTRUCTION_4_LINES,0,0,0,QSPI_DATA_4_LINES);
        QSPI_Receive(&sta_reg1,1);
    }while( (sta_reg1&0x01) == 0x01 );
}

/***************************************************************************************
  * @brief   ����QSPIĳ������
  * @input   
  * @return  
***************************************************************************************/
void W25QXX_EraseSector(uint32_t sector_id)
{
    uint32_t addr = sector_id * 4096;
	
    W25Q_WRITE_ENABLE();
    W25QXX_WaitIdle();
    QSPI_SendCmd(W25X_SectorErase,QSPI_INSTRUCTION_4_LINES,addr,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,0);
    W25QXX_WaitIdle();
}


/***************************************************************************************
  * @brief   ��ȡSPI FLASH,��֧��QPIģʽ
  * @input   pBuffer:���ݴ洢��
             ReadAddr:��ʼ��ȡ�ĵ�ַ(���32bit)
             NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
  * @return
***************************************************************************************/
void W25QXX_Read(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    QSPI_CommandTypeDef s_command = {0};
    s_command.Instruction       = W25X_FastReadData;
    s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
    s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
    s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
    s_command.DataMode          = QSPI_DATA_4_LINES;
    s_command.DummyCycles       = 8;
    s_command.NbData            = NumByteToRead;
    s_command.Address           = ReadAddr;
    if (HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_QSPI_Receive(&hqspi, pBuffer, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        Error_Handler();
    }
}




/***************************************************************************************
  * @brief   SPI��һҳ(0~65535)��д������256���ֽڵ�����
  * @input   pBuffer:���ݴ洢��
             WriteAddr:��ʼд��ĵ�ַ(���32bit)
             NumByteToWrite:Ҫд����ֽ���(���256),������Ӧ�ó�����ҳ��ʣ���ֽ���!!!	 
  * @return  
***************************************************************************************/
void W25QXX_Write_Page(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)
{
	W25Q_WRITE_ENABLE();				//дʹ��
    W25QXX_WaitIdle();
    QSPI_SendCmd(W25X_PageProgram,QSPI_INSTRUCTION_4_LINES,
                 WriteAddr,QSPI_ADDRESS_4_LINES,QSPI_ADDRESS_32_BITS,QSPI_DATA_4_LINES);
	QSPI_Transmit(pBuffer, NumByteToWrite);	         	      
	W25QXX_WaitIdle();					//�ȴ�д�����
}

/***************************************************************************************
  * @brief   �޼���дSPI FLASH ,�����Զ���ҳ���� 
  * @input   pBuffer:���ݴ洢��
             WriteAddr:��ʼд��ĵ�ַ(���32bit)
             NumByteToWrite:Ҫд����ֽ���(���65535)
  * @return  
***************************************************************************************/
void W25QXX_Write_NoCheck(uint8_t* pBuffer,uint32_t WriteAddr,uint16_t NumByteToWrite)   
{
	uint16_t page_remain = 256 - WriteAddr % 256; //��ҳʣ����ֽ���		
    
	if(NumByteToWrite <= page_remain){
        page_remain = NumByteToWrite;//������256���ֽ�
    }
    
	while(1)
	{
		W25QXX_Write_Page(pBuffer, WriteAddr, page_remain);
        
		if(NumByteToWrite == page_remain){
            break;//д�������
	 	} else {
			pBuffer += page_remain;
			WriteAddr += page_remain;	

			NumByteToWrite -= page_remain;//��ȥ�Ѿ�д���˵��ֽ���
			if(NumByteToWrite > 256)
                page_remain = 256;        //һ�ο���д��256���ֽ�
			else 
                page_remain = NumByteToWrite; //����256���ֽ���
		}
	}
}

/***************************************************************************************
  * @brief   дSPI FLASH  
  * @input   pBuffer:���ݴ洢��
             WriteAddr:��ʼд��ĵ�ַ(���32bit)	
             NumByteToWrite:Ҫд����ֽ���(���65535)   
  * @return  
***************************************************************************************/
uint8_t W25QXX_Buffer[4096];		 
void W25QXX_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)   
{
	uint32_t sec_id;
	uint16_t sec_offset;
	uint16_t sec_remain;	   
 	uint16_t i;    
	uint8_t * W25Q_Buf = W25QXX_Buffer;	  
    
 	sec_id     = WriteAddr / 4096;//������ַ  
	sec_offset = WriteAddr % 4096;//�������ڵ�ƫ��
	sec_remain = 4096 - sec_offset;//����ʣ��ռ��С   
    
 	if(NumByteToWrite <= sec_remain)//
        sec_remain = NumByteToWrite;//������4096���ֽ�
    
	while(1) 
	{
		W25QXX_Read(W25Q_Buf, sec_id*4096, 4096);//������������������
		for(i=0; i<sec_remain; i++)//У������
		{
			if(W25Q_Buf[sec_offset+i] != 0xFF)
                break;//��Ҫ����  	  
		}
        
		if(i < sec_remain){//��Ҫ����
			W25QXX_EraseSector(sec_id);//�����������
			for(i=0;i<sec_remain;i++){	   //����
				W25Q_Buf[i+sec_offset] = pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25Q_Buf, sec_id*4096, 4096);//д����������  
		} else {
            W25QXX_Write_NoCheck(pBuffer,WriteAddr,sec_remain);//д�Ѿ������˵�,ֱ��д������ʣ������. 	
        }
        
		if(NumByteToWrite == sec_remain){
            break;       //д�������
        } else {         //д��δ����
			sec_id++;    //������ַ��1
			sec_offset = 0;//ƫ��λ��Ϊ0 	 

		   	pBuffer += sec_remain;       //ָ��ƫ��
			WriteAddr += sec_remain;     //д��ַƫ��	   
		   	NumByteToWrite -= sec_remain;//�ֽ����ݼ�
			if(NumByteToWrite > 4096)
                sec_remain = 4096;	     //��һ����������д����
			else 
                sec_remain = NumByteToWrite;//��һ����������д����
		}
	}
}


uint16_t W25_ID;
uint8_t TEXT_Buffer[] = {"abcdefghijklmnopqrstuvwxyz"};
uint8_t W25_RxBuf[50] = {0};
#define SIZE sizeof(TEXT_Buffer)
/***************************************************************************************
  * @brief   W25Q256��ʼ��
  * @input
  * @return
***************************************************************************************/
void W25QXX_Init(void)
{
    W25QXX_SetQE();
    delay_ms(20);
    W25QXX_ENTER_QSPI();
    W25QXX_ENTER_4BYTEADDR();
    W25QXX_SetReadParam();//���ö�����
    
    W25QXX_Write(TEXT_Buffer,W25Q256_SIZE - 100, SIZE);
    W25_ID = W25QXX_ReadId();
    W25QXX_Read(W25_RxBuf, W25Q256_SIZE - 100, SIZE);
}

