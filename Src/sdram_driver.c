#include "main.h"


/***************************************************************************************
  * @brief   
  * @input   
  * @return  
***************************************************************************************/
uint8_t SDRAM_Send_Cmd(SDRAM_HandleTypeDef *hsdram, uint8_t cmd,uint8_t refresh,uint16_t regval)
{
    FMC_SDRAM_CommandTypeDef Command;
    Command.CommandMode = cmd;                           //����
    Command.CommandTarget = FMC_SDRAM_CMD_TARGET_BANK1;  //Ŀ��SDRAM�洢����
    Command.AutoRefreshNumber = refresh;                 //��ˢ�´���
    Command.ModeRegisterDefinition = regval;             //Ҫд��ģʽ�Ĵ�����ֵ
    if(HAL_SDRAM_SendCommand(hsdram, &Command, 0x1000)==HAL_OK) {//��SDRAM��������
        return 0;  
    }
    else 
        return 1;    
}


/***************************************************************************************
  * @brief   ����SDRAM��ʼ������
  * @input   
  * @return  
***************************************************************************************/
void SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram)
{
	uint32_t temp=0;
    //SDRAM��������ʼ������Ժ���Ҫ��������˳���ʼ��SDRAM
    SDRAM_Send_Cmd(hsdram, FMC_SDRAM_CMD_CLK_ENABLE, 1, 0); //ʱ������ʹ��
    delay_us(500);                                          //������ʱ200us
    SDRAM_Send_Cmd(hsdram, FMC_SDRAM_CMD_PALL, 1, 0);       //�����д洢��Ԥ���
    SDRAM_Send_Cmd(hsdram, FMC_SDRAM_CMD_AUTOREFRESH_MODE, 8, 0);//������ˢ�´��� 
    
    //����ģʽ�Ĵ���,SDRAM��bit0~bit2Ϊָ��ͻ�����ʵĳ��ȣ�
	//bit3Ϊָ��ͻ�����ʵ����ͣ�bit4~bit6ΪCASֵ��bit7��bit8Ϊ����ģʽ
	//bit9Ϊָ����дͻ��ģʽ��bit10��bit11λ����λ
	temp = SDRAM_MODEREG_BURST_LENGTH_1          |	//����ͻ������:1(������1/2/4/8)
           SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   |	//����ͻ������:����(����������/����)
           SDRAM_MODEREG_CAS_LATENCY_3           |	//����CASֵ:3(������2/3)
           SDRAM_MODEREG_OPERATING_MODE_STANDARD |   //���ò���ģʽ:0,��׼ģʽ
           SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;     //����ͻ��дģʽ:1,�������
    
    SDRAM_Send_Cmd(0,FMC_SDRAM_CMD_LOAD_MODE,1,temp);   //����SDRAM��ģʽ�Ĵ���
    
    //ˢ��Ƶ�ʼ�����(��SDCLKƵ�ʼ���),���㷽��:
	//COUNT=SDRAMˢ������/����-20=SDRAMˢ������(us)*SDCLKƵ��(Mhz)/����
    //����ʹ�õ�SDRAMˢ������Ϊ64ms,SDCLK=216/2=108Mhz,����Ϊ8192(2^13).
	//����,COUNT=64*1000*108/8192-20=823
	HAL_SDRAM_ProgramRefreshRate(hsdram, 823);
    
//      for(uint8_t i = 0; i< 255; i++){
//        ltdc_layer0[0][i] = i;
//      }
}








