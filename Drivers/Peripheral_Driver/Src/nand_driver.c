#include "stm32f7xx_hal.h"

#define BUSY_GPIO	GPIOD
#define BUSY_PIN	GPIO_PIN_6

/* ����NAND Flash�������ַ���������Ӳ�������� */
#define Bank_NAND_ADDR     ((uint32_t)0x80000000)

/* �������NAND Flash�õ�3���� */
#define NAND_CMD_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | CMD_AREA)
#define NAND_ADDR_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | ADDR_AREA)
#define NAND_DATA_AREA		*(__IO uint8_t *)(Bank_NAND_ADDR | DATA_AREA)

//NAND FLASH���������ʱ����
#define NAND_TADL_DELAY				30			//tADL�ȴ��ӳ�,����70ns
#define NAND_TWHR_DELAY				25			//tWHR�ȴ��ӳ�,����60ns
#define NAND_TRHW_DELAY				35			//tRHW�ȴ��ӳ�,����100ns
#define NAND_TPROG_DELAY			200			//tPROG�ȴ��ӳ�,����ֵ200us,�����Ҫ700us
#define NAND_TBERS_DELAY			4			//tBERS�ȴ��ӳ�,����ֵ3.5ms,�����Ҫ10ms

#define NAND_OK   0
#define NAND_FAIL 1

/* ����ȴ���������������쳣, �˴�Ӧ���жϳ�ʱ*/
#define WAIT_BUSY()	{	\
	uint16_t k; 	\
	for (k = 0; k < 200; k++)	\
	{	\
		if ((BUSY_GPIO->IDR & BUSY_PIN) == 0) break;	\
	}	\
	for (k = 0; k < 2000; k++)	\
	{	\
		if ((BUSY_GPIO->IDR & BUSY_PIN) != 0) break;	\
	}	\
}



/**
  \fn          int32_t Driver_NANDx_GetDeviceBusy (uint32_t dev_num)
  \brief       NAND Driver GetDeviceBusy callback.
               Needs to be implemented by user.
  \param[in]   dev_num   Device number
  \return      1=busy, 0=not busy, or error
*/
int32_t Driver_NAND0_GetDeviceBusy (uint32_t dev_num)
{
	WAIT_BUSY();
	
	return 0;
}


/*
*********************************************************************************************************
*	�� �� ��: FSMC_NAND_ReadStatus
*	����˵��: ʹ��Read statuc �����NAND Flash�ڲ�״̬
*	��    ��:  - Address: �������Ŀ��������ַ
*	�� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_BUSY: �ڲ���æ
*             - NAND_READY: �ڲ����У����Խ����²�����
*             - NAND_ERROR: ��ǰ������ִ��ʧ��
*********************************************************************************************************
*/
static uint32_t FMC_NAND_ReadStatus(void)
{
	uint8_t ucData;
	uint8_t ucStatus = NAND_BUSY;

	/* ��״̬���� */
	NAND_CMD_AREA = NAND_CMD_STATUS;
	ucData = *(__IO uint8_t *)(Bank_NAND_ADDR);

	if((ucData & NAND_ERROR) == NAND_ERROR){
		ucStatus = NAND_ERROR;
	} else if((ucData & NAND_READY) == NAND_READY){
		ucStatus = NAND_READY;
	} else {
		ucStatus = NAND_BUSY;
	}

	return (ucStatus);
}

/*
*********************************************************************************************************
*	�� �� ��: FSMC_NAND_GetStatus
*	����˵��: ��ȡNAND Flash����״̬
*	��    ��:  - Address: �������Ŀ��������ַ
*	�� �� ֵ: NAND����״̬�������¼���ֵ��
*             - NAND_TIMEOUT_ERROR  : ��ʱ����
*             - NAND_READY          : �����ɹ�
*             - NAND_ERROR: ��ǰ������ִ��ʧ��
*********************************************************************************************************
*/
static uint32_t FMC_NAND_WaitForReady(void)
{
	uint32_t ulTimeout = 0x10000;
	uint32_t ucStatus = NAND_READY;

	ucStatus = FMC_NAND_ReadStatus();

	/* �ȴ�NAND������������ʱ����˳� */
	while ((ucStatus != NAND_READY) &&( ulTimeout != 0x00))
	{
		ucStatus = FMC_NAND_ReadStatus();
		if(ucStatus == NAND_ERROR){
			return (ucStatus);/* ���ز���״̬ */
		}
		ulTimeout--;
	}

	if(ulTimeout == 0x00)
	{
		ucStatus =  NAND_TIMEOUT_ERROR;
	}

	/* ���ز���״̬ */
	return (ucStatus);
}

/*
*********************************************************************************************************
*	�� �� ��: FMC_NAND_Reset
*	����˵��: ��λNAND Flash
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t NAND_Reset(void)
{
	NAND_CMD_AREA = NAND_CMD_RESET;

	/* ������״̬ */
	if (FMC_NAND_WaitForReady() == NAND_READY){
		return NAND_OK;
	}
	return NAND_FAIL;
}

/*
*********************************************************************************************************
*	�� �� ��: NAND_ReadID
*	����˵��: ��NAND Flash��ID��ID�洢���β�ָ���Ľṹ������С�
*	��    ��:  ��
*	�� �� ֵ: 32bit��NAND Flash ID
*********************************************************************************************************
*/
uint32_t NAND_ReadID(void)
{
	uint8_t deviceId[5];
    uint32_t id;  
	/* �������� Command to the command area */
	NAND_CMD_AREA = 0x90;
	NAND_ADDR_AREA = 0x00;
    
	/* ˳���ȡNAND Flash��ID */
    	//IDһ����5���ֽ�
    deviceId[0]=*(__IO uint8_t*)Bank_NAND_ADDR;      
    deviceId[1]=*(__IO uint8_t*)Bank_NAND_ADDR;  
    deviceId[2]=*(__IO uint8_t*)Bank_NAND_ADDR; 
    deviceId[3]=*(__IO uint8_t*)Bank_NAND_ADDR; 
    deviceId[4]=*(__IO uint8_t*)Bank_NAND_ADDR;  

    id = ((uint32_t)deviceId[1])<<24|((uint32_t)deviceId[2])<<16|((uint32_t)deviceId[3])<<8|deviceId[4];
	return id;
}

