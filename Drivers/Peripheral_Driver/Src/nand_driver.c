#include "stm32f7xx_hal.h"
#include "utility.h"
#include "rl_fs.h"
#include "stdio.h"

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

/* FlashFS API�ķ���ֵ */
static const char * ReVal_Table[]= 
{
	"fsOK���ɹ�",				                        
	"fsError��δָ���Ĵ���",
	"fsUnsupported��������֧��",
	"fsAccessDenied����Դ���ʱ��ܾ�",
	
	"fsInvalidParameter��������Ч",
	"fsInvalidDrive��������Ч������������",
	"fsInvalidPath��·����Ч",
	"fsUninitializedDrive������δ��ʼ�� ",

	"fsDriverError����д����",
	"fsMediaError��ý�����",
	"fsNoMedia��ý�鲻���ڣ�����δ��ʼ��",
	"fsNoFileSystem���ļ�ϵͳδ��ʽ��",

	"fsNoFreeSpace��û�п��ÿռ�",
	"fsFileNotFound���ļ�δ�ҵ�",
	"fsDirNotEmpty���ļ��зǿ�",
	"fsTooManyOpenFiles�����ļ�̫��",
};

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

/*
*********************************************************************************************************
*	�� �� ��: DotFormatjavascript:;
*	����˵��: �����ݹ淶����ʾ�������û��鿴
*             ����
*             2345678   ---->  2.345.678
*             334426783 ---->  334.426.783
*             ��������Ϊ��λ������ʾ
*	��    ��: _ullVal   ��Ҫ�淶��ʾ����ֵ
*             _sp       �淶��ʾ�����ݴ洢��buf��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DotFormat(uint64_t _ullVal, char *_sp) 
{
	/* ��ֵ���ڵ���10^9 */
	if (_ullVal >= (uint64_t)1e9) 
	{
		_sp += sprintf (_sp, "%d.", (uint32_t)(_ullVal / (uint64_t)1e9));
		_ullVal %= (uint64_t)1e9;
		_sp += sprintf (_sp, "%03d.", (uint32_t)(_ullVal / (uint64_t)1e6));
		_ullVal %= (uint64_t)1e6;
		sprintf (_sp, "%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* ��ֵ���ڵ���10^6 */
	if (_ullVal >= (uint64_t)1e6) 
	{
		_sp += sprintf (_sp,"%d.", (uint32_t)(_ullVal / (uint64_t)1e6));
		_ullVal %= (uint64_t)1e6;
		sprintf (_sp,"%03d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* ��ֵ���ڵ���10^3 */
	if (_ullVal >= 1000) 
	{
		sprintf (_sp, "%d.%03d", (uint32_t)(_ullVal / 1000), (uint32_t)(_ullVal % 1000));
		return;
	}
	
	/* ������ֵ */
	sprintf (_sp,"%d",(uint32_t)(_ullVal));
}
/*
*********************************************************************************************************
*	�� �� ��: ViewRootDir
*	����˵��: ��ȡNand flash����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void GetCapacity(void)
{
    uint64_t capacity;
    uint8_t buf[15] = {0};
    fsMediaInfo info;
    fsStatus restatus;
    int32_t id;
     uint32_t ser_num;
    
        /* ��ȡvolume label */
	if (fvol ("N0:", (char *)buf, &ser_num) == 0) 
	{
		if (buf[0]) 
		{
			printf ("NAND��volume label�� %s\r\n", buf);
		}
		else 
		{
			printf ("NANDû��volume label\r\n");
		}
		
		printf ("NAND��volume serial number�� %d\r\n", ser_num);
	}
	else 
	{
		printf ("Volume���ʴ���\r\n");
	}
    
    /* ��ȡNand Flashʣ������ */
	capacity = ffree("N0:");
	DotFormat(capacity, (char *)buf);
	printf("NANDʣ������ = %10s�ֽ�\r\n", buf);
    
    /* ��ȡ�洢�豸��������Ϣ */
	restatus = fs_ioc_read_info (id, &info);
	if(restatus == fsOK)
	{
		/* �ܵ������� * ������С��SD����������С��512�ֽ� */
		capacity = (uint64_t)info.block_cnt << 9;
		DotFormat(capacity, (char *)buf);
		printf("NAND������ = %10s�ֽ�\r\nSD������������ = %d \r\n", buf, info.block_cnt);
	}
	else
	{
		printf("��ȡ������Ϣʧ�� %s\r\n", ReVal_Table[restatus]);
	}
	
	/* ���ʽ���Ҫ���� */
	fs_ioc_unlock (id);

	printf("NAND��������С = %d�ֽ�\r\n", info.read_blen);
	printf("NANDд������С = %d�ֽ�\r\n", info.write_blen);
}

/*
*********************************************************************************************************
*	�� �� ��: ViewRootDir
*	����˵��: ��ʾnand flash��Ŀ¼�µ��ļ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ViewRootDir(void)
{
    uint8_t result;
	fsFileInfo info;
	uint8_t buf[15];
	
    info.fileID = 0;   /* ÿ��ʹ��ffind����ǰ��info.fileID�����ʼ��Ϊ0 */
    printf("------------------------------------------------------------------\r\n");
	printf("�ļ���                 |  �ļ���С     | �ļ�ID  | ����      |����\r\n");
	
	/* 
	   ����Ŀ¼�µ������ļ��г�����
	   1. "*" ���� "*.*" ����ָ��·���µ������ļ�
	   2. "abc*"         ����ָ��·������abc��ͷ�������ļ�
	   3. "*.jpg"        ����ָ��·������.jpg��β�������ļ�
	   4. "abc*.jpg"     ����ָ��·������abc��ͷ��.jpg��β�������ļ�
	
	   ������ʵ��������Ŀ¼�������ļ�
	*/
	while(ffind ("N0:*.*", &info) == 0)  
	{ 
		/* �����ļ���ʾ��С��ʽ */
		DotFormat(info.size, (char *)buf);
		
		/* ��ӡ��Ŀ¼�µ������ļ� */
		printf ("%-20s %12s bytes, ID: %04d  ",
				info.name,
				buf,
				info.fileID);
		
		/* �ж����ļ�������Ŀ¼ */
		if (info.attrib & FS_FAT_ATTR_DIRECTORY){
			printf("(0x%02x)Ŀ¼", info.attrib);
		}else{
			printf("(0x%02x)�ļ�", info.attrib);
		}
		
		/* ��ʾ�ļ����� */
		printf ("  %04d.%02d.%02d  %02d:%02d\r\n",
                 info.time.year, info.time.mon, info.time.day,
               info.time.hr, info.time.min);
    }
	
	if (info.fileID == 0)  
	{
		printf ("����û�д���ļ�\r\n");
	}
}


/*
*********************************************************************************************************
*	�� �� ��: CreateNewFile
*	����˵��: ��SD����������text�ı����ֱ�ʹ��fwrite��fprintf��fputsд���ݡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void CreateNewFile(void)
{
	const uint8_t WriteText[] = {"�人�������������޹�˾\r\n2015-09-06\r\nwww.armfly.com\r\nWWW.ARMFLY.COM"};
	const uint8_t WriteText1[] = {"�人�������������޹�˾\r"};
	FILE *fout;
	uint32_t bw;
	uint32_t i = 2;
	uint8_t result;
    
	/**********************************************************************************************************/
	/* ���ļ���test�е��ļ�test1.txt�����û�����ļ��к�txt�ļ����Զ�����*/
	fout = fopen ("N0:\\test\\test1.txt", "w"); 
	if (fout != NULL) 
	{
		/* д���� */
		bw = fwrite (WriteText, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fout);
		if(bw == sizeof(WriteText)/sizeof(uint8_t)){
			printf("д�����ݳɹ�\r\n");
		}else{ 
			printf("д������ʧ��\r\n");
		}
		
		/* �ر��ļ� */
		fclose(fout);
	}
}



/*
*********************************************************************************************************
*	�� �� ��: ReadFileData
*	����˵��: �ֱ�ʹ��fread��fscan��fgets��ȡ������ͬ��txt�ļ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ReadFileData(void)
{
    const uint8_t WriteText[] = {"�人�������������޹�˾\r\n2015-09-06\r\nwww.armfly.com\r\nWWW.ARMFLY.COM"};
	uint8_t Readbuf[100];
	FILE *fin;
	uint32_t bw;
	uint32_t index1, index2;
	float  count = 0.0f;
	uint8_t result;
    
    	/* ����SD�� */
	result = fmount("N0:");
	if(result != NULL)
	{
		/* �������ʧ�ܣ���ز�Ҫ�ٵ���FlashFS������API��������ֹ����Ӳ���쳣 */
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", ReVal_Table[result]);
		goto access_fail;
	}
	else
	{
		printf("�����ļ�ϵͳ�ɹ� (%s)\r\n", ReVal_Table[result]);
	}
    
    	/* ���ļ���test�е��ļ�test1.txt */
	fin = fopen ("N0:\\test\\test1.txt", "r"); 
	if (fin != NULL) 
	{
		printf("<1>���ļ�M0:\\test\\test1.txt�ɹ�\r\n");
		
		/* ��ֹ���� */
		(void) WriteText;
		
		/* ������ ��ʽһ */
		bw = fread(Readbuf, sizeof(uint8_t), sizeof(WriteText)/sizeof(uint8_t), fin);
		if(bw == sizeof(WriteText)/sizeof(uint8_t))
		{
			Readbuf[bw] = NULL;
			printf("test1.txt �ļ����� : \r\n%s\r\n", Readbuf);
		}else{ 
			printf("������ʧ��\r\n");
		}
        
        /* ������ ��ʽ��*/
//		if(fgets((char *)Readbuf, sizeof(Readbuf), fin) != NULL){
//			printf("test1.txt �ļ����� : \r\n%s\r\n", Readbuf);
//		}else{
//			printf("������ʧ��\r\n");
//		}
        
//        /* ������ ��ʽ��*/
//        bw = fscanf(fin, "%d %d %f", &index1, &index2, &count);
//		if (bw == 3)  {
//			printf("��������ֵ\r\nindex1 = %d index2 = %d count = %f\r\n", index1, index2, count);
//		}else{
//			printf("������ʧ��\r\n");	
//		}
		
		/* �ر��ļ� */
		fclose(fin);
	}
    
access_fail:
	/* ж��SD�� */
	result = funmount("N0:");
	if(result != NULL)
	{
		printf("ж���ļ�ϵͳʧ��\r\n");
	}
	else
	{
		printf("ж���ļ�ϵͳ�ɹ�\r\n");
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DeleteDirFile
*	����˵��: ɾ���ļ��к��ļ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DeleteDirFile(void)
{
	fsStatus restatus;
    
    /* ɾ���ļ���test */
	restatus = fdelete ("N0:\\test\\", NULL);
	if (restatus != NULL) 
	{
		printf("test�ļ��зǿջ򲻴���\r\n");
	}
	else
	{
		printf("ɾ��test�ļ��гɹ�\r\n");
	}
    
}
/***************************************************************************************
  * @brief   
  * @input   
  * @return  
***************************************************************************************/
void NAND_Init(void)
{
    uint8_t result;
    char *opt;
   
    NAND_Reset();
    delay_ms(100);
    NAND_ReadID();	        //��ȡID
    result = finit("N0:");
    if(result != NULL){
        printf("��ʼ���ļ�ϵͳʧ�� (%s)\r\n", ReVal_Table[result]);/* �������ʧ�ܣ���ز�Ҫ�ٵ���FlashFS������API��������ֹ����Ӳ���쳣 */
    }else{
        printf("��ʼ���ļ�ϵͳ�ɹ� (%s)\r\n", ReVal_Table[result]);
    }
    
	result = fmount("N0:");
	if(result != NULL){
		/* �������ʧ�ܣ���ز�Ҫ�ٵ���FlashFS������API��������ֹ����Ӳ���쳣 */
		printf("�����ļ�ϵͳʧ�� (%s)\r\n", ReVal_Table[result]);
        
        if (result == 0x0B){
            /* ��ʽ�� */
            opt = "/LL /L nand /FAT32";
            printf("�ļ�ϵͳ��ʽ��......\r\n");
            result = fformat ("N0:", opt);
            printf("�ļ�ϵͳ��ʽ�� (%s)\r\n", ReVal_Table[result]);
        } else return;
	}else{
		printf("�����ļ�ϵͳ�ɹ� (%s)\r\n", ReVal_Table[result]);
	}

    GetCapacity();
	CreateNewFile();
    
    ViewRootDir();
    
	/* ж��nand�ļ�ϵͳ */
	result = funmount("N0:");
	if(result != NULL)
	{
		printf("ж���ļ�ϵͳʧ��\r\n");
	}
	else
	{
		printf("ж���ļ�ϵͳ�ɹ�\r\n");
	}
    
    
    ReadFileData();
    
}
