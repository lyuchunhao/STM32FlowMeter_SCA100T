/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs ��Ȩ����
 ****************************************************************************
 * �ļ�����             :  uart_task.c
 * �汾                 :  0.0.1
 * ����                 :  lvchunhao
 * ��������             :��2019��8��11��
 * ����                 :  Uart�ն˷����߳�
                           1. ���е��·�����ͨ�����������
 ****************************************************************************/
#include "common.h"




//�жϽ��մ�����ز��� 
uint8_t g_u8Rs232RecvLen  = 0; 
uint8_t g_u8Rs232RecvStep = STEP_FIND_HEAD; 
uint8_t g_u8Rs232RecvCompeletFlag = 0;         //1-������� 0-û����ɼ�������

uint8_t u8Rs232RxBuff[MAX_RECV_SIZE];        //���ջ�����
uint8_t u8Rs232TxBuff[MAX_SEND_SIZE];        //���ͻ�����
uint8_t u8Rs232KpBuff[MAX_RECV_SIZE];        //������ʱ������

static uint32_t u32Rs232WriteIndex = 0; 
static xQueueHandle s_s32MsgQueID  = NULL;





/*
 * ������      : GetRs232WriteIndex
 * ����        : Rs232ָ���ȡ
 * ����        : void
 * ����        : ָ��ֵ
 * ����        : lvchunhao  2017-11-14
 */
static uint32_t GetRs232WriteIndex(void)
{
		return u32Rs232WriteIndex;
}
/*
 * ������      : Rs232WriteIndexClearZero
 * ����        : Rs232дָ������
 * ����        : void
 * ����        : void
 * ����        : lvchunhao  2017-11-14
 */
static void Rs232WriteIndexClearZero(void)
{
		u32Rs232WriteIndex = 0;
}
/*
 * ������      : Rs232RecvOneData
 * ����        : ���жϴ�����One�ֽڴ��뻺����
 * ����        : u8Bytes [in] : ����
 * ����        : ��
 * ����        : lvchunhao  2017-11-14
 */
void Rs232RecvOneData(uint8_t u8Bytes)
{
		if(u32Rs232WriteIndex == MAX_RECV_SIZE)
		{
				u32Rs232WriteIndex = 0;
		}
		
		u8Rs232RxBuff[u32Rs232WriteIndex++] = u8Bytes;
}

int32_t Rs232RecvProtolData(uint8_t u8Data)
{
		int32_t s32Ret = -1;
	
	  switch(g_u8Rs232RecvStep)
		{
				case STEP_FIND_HEAD:
						 if(u8Data == 0x68)	
						 {
							  Rs232RecvOneData(u8Data);
							  g_u8Rs232RecvStep = STEP_LENGTH_GET;
						 }
						 break;
				case STEP_LENGTH_GET:
					   if((u8Data < 4) || (u8Data > 20))
						 {
								g_u8Rs232RecvStep = STEP_FIND_HEAD;
							  Rs232WriteIndexClearZero();
						 }
						 else
						 {
								Rs232RecvOneData(u8Data);
								g_u8Rs232RecvLen = u8Data - 1;
							  g_u8Rs232RecvStep = STEP_DATA_GET;
						 }
						 break;
				case STEP_DATA_GET:
						 g_u8Rs232RecvLen--;
						 Rs232RecvOneData(u8Data);
						 if(g_u8Rs232RecvLen == 0)
						 {
								s32Ret = 0;
								g_u8Rs232RecvStep = STEP_FIND_HEAD;
						 }
						 break;
						 
				default :
						 g_u8Rs232RecvStep = STEP_FIND_HEAD;
						 break;	 
		}
		
		return s32Ret;
		
}

/*
 * ������    ��PostUartWriteMsg()
 * ����      : Ͷ��һ���ն˴�����Ϣ
 * ����      : pData [in] : ��Ҫд������ָ��
 *           : u32DataLen [in] : ���ݳ���
 * ����      : 0-�ɹ� ����-1
 * ����      : lyuchunhao 2019-8-11
 */
int32_t PostUartWriteMsg(uint8_t *pData, uint32_t u32DataLen)
{
    StMsgPkg stPkg;
    uint8_t *pDataCopy = NULL;
		portBASE_TYPE s32Ret;

    if(!pData || 0 == u32DataLen)
    {
    	return -1;
    }

    if(NULL == s_s32MsgQueID)
    {
        return -1;
    }

		//����256�ֽ�
    pDataCopy = (uint8_t *)pvPortMalloc(MSG_BUFF_SIZE);   
    if(NULL == pDataCopy)
    {
    	return -1;
    }
    else
    {
    	memcpy(pDataCopy, pData, u32DataLen);
    }

    memset(&stPkg, 0, sizeof(stPkg));
    stPkg.pData = pDataCopy;
    stPkg.u32DataLen = u32DataLen;

		s32Ret = xQueueSendToBack(s_s32MsgQueID, (const void *)(&stPkg), 0);
    if(s32Ret != pdPASS)
    {
        //���Ͷ�̬�ռ�,��ʧ�����ͷſռ�
        if(stPkg.pData)
        {
            vPortFree(stPkg.pData);
        }
        return -1;
    }

    return 0;
}

/*
 * ������    ��vTaskUartWrite()
 * ����      : �ն˷��ͺ���
 * ����      : pArgs [in] : �̲߳���
 * ����      : none
 * ����      : lyuchunhao 2019-8-11
 */
void vTaskUartWrite(void *pArgs)
{
		StTaskObj *pTaskObj = (StTaskObj *)pArgs;
		StMsgPkg stMsg;
		portBASE_TYPE s32Ret;

		s_s32MsgQueID = pTaskObj->s32MsgQueID;
		PostUartWriteMsg((uint8_t *)"vTaskUartWrite is Start!\n", 25);
	
		while(1)
		{
				s32Ret = xQueueReceive(s_s32MsgQueID, &stMsg, portMAX_DELAY);
				if(s32Ret != pdPASS)
				{
					continue;
				}
			
				RS485_SEND_ENABLE();
				vTaskDelay(1);
				STM32L_UART_Transmit(&huart1, stMsg.pData, stMsg.u32DataLen);
				vTaskDelay(5);
				RS485_RECV_ENABLE();
				
				/* �ͷŻ����� */
				if(stMsg.pData)
				{
						vPortFree(stMsg.pData);
						stMsg.pData = NULL;
						stMsg.u32DataLen = 0;
				}
		}
}
uint8_t User_GetCRCData(uint8_t *u8Data, uint8_t u8Len)
{
		uint8_t i = 0;
		uint8_t u8Ret = 0;
	
		for(i = 1; i < u8Len; i ++)
	  {
				u8Ret += u8Data[i];
		}
		
		return u8Ret;
}

/*
 * ������    ��vTaskUartRecev()
 * ����      : �ն˽��պ���
 * ����      : pArgs [in] : �̲߳���
 * ����      : none
 * ����      : lyuchunhao 2019-8-11
 */
void vTaskUartRecev(void *pArgs)
{
		int32_t s32SendLen = 0;
	  int32_t s32RecvLen = 0;
	  double  dbCalibratValue = 0;
	  uint8_t u8CalibratData[4];
		Unfloat unFloatValue;
	
	  uint8_t u8TmpBCD = 0;
	  double dbTmpValue = 0.0;
	  uint32_t u32TmpValue = 0;
	  	  
		uint32_t i = 0;
		double dbOffcorr   = 0.0;
		double dbTmpValueX1 = 0.0;
		double dbTmpValueY1 = 0.0;
		double dbTmpValueX2 = 0.0;
		double dbTmpValueY2 = 0.0;
		
	  uint8_t u8SendData[32] = {0};
	  g_u8OperatePrintFlag = 0;
		g_u8OperateAnglXFlag = 0;
	
		vTaskDelay(2);
		RS232_IT_ENABLE();
	  Rs232WriteIndexClearZero();
		PostUartWriteMsg((uint8_t *)"vTaskUartRecev is Start!\n", 25);
	
		while(1)
		{
				/******************** 115200������ÿmsԼ����12Bytes ****************/
				if(g_u8Rs232RecvCompeletFlag == 1)
				{
						/* �����ж��н��յ������ݸ��Ƶ����������� */
						s32RecvLen = GetRs232WriteIndex();
					  memset(u8Rs232KpBuff, 0, MAX_RECV_SIZE);
					  memcpy(u8Rs232KpBuff, u8Rs232RxBuff, s32RecvLen);
					
					  /* ���дָ�����ɱ�־�ٴν��� */
					  Rs232WriteIndexClearZero();
					  g_u8Rs232RecvCompeletFlag = 0;
					
						/* Э������봦�� */
					  if((u8Rs232KpBuff[0] == 0x68) && (s32RecvLen >= 5))
						{
								switch(u8Rs232KpBuff[3])
								{
										case 0x04://��ȡ�Ƕ�									 
															/* CPOL = 0 CPHA =1 */
															/* ADS1263����20bit:���λΪ����λ������Ϊ2~19 = 524287 */
													    g_stAngleData.u32AverageTemp = 0;
															g_stAngleData.u32AverageA1T1 = 0;
															g_stAngleData.u32AverageA1T2 = 0;
															g_stAngleData.u32AverageA2T1 = 0;
															g_stAngleData.u32AverageA2T2 = 0;
															for(i = 0; i < ADS_BUFF_SIZE; i ++)
															{
																	g_stAngleData.u32AverageTemp += g_stAngleData.u8AngleTp[i];
																	g_stAngleData.u32AverageA1T1 += g_stAngleData.u32A1OUT1[i];
																	g_stAngleData.u32AverageA1T2 += g_stAngleData.u32A1OUT2[i];
																	g_stAngleData.u32AverageA2T1 += g_stAngleData.u32A2OUT1[i];
																	g_stAngleData.u32AverageA2T2 += g_stAngleData.u32A2OUT2[i];
															}
															g_stAngleData.u32AverageTemp = g_stAngleData.u32AverageTemp/ADS_BUFF_SIZE;
															g_stAngleData.u32AverageA1T1 = g_stAngleData.u32AverageA1T1/ADS_BUFF_SIZE;
															g_stAngleData.u32AverageA1T2 = g_stAngleData.u32AverageA1T2/ADS_BUFF_SIZE;
															g_stAngleData.u32AverageA2T1 = g_stAngleData.u32AverageA2T1/ADS_BUFF_SIZE;
															g_stAngleData.u32AverageA2T2 = g_stAngleData.u32AverageA2T2/ADS_BUFF_SIZE;
															
															//X�᣺IC2/X1-Y1 Y�᣺IC1/X2-Y2
															dbTmpValueX1 = 5.0/524287*g_stAngleData.u32AverageA2T1;
															dbTmpValueY1 = 5.0/524287*g_stAngleData.u32AverageA2T2;
															dbTmpValueX2 = 5.0/524287*g_stAngleData.u32AverageA1T1;
															dbTmpValueY2 = 5.0/524287*g_stAngleData.u32AverageA1T2;
															//2019-09-27������X���Y���˳��(���ų�Ҫ��)
															g_stAngleData.dbAngleY = asin((dbTmpValueX1 - dbTmpValueY1)/16)*180.0/PI;
															g_stAngleData.dbAngleX = asin((dbTmpValueX2 - dbTmpValueY2)/16)*180.0/PI;
															
															g_stAngleData.dbAngleTp = (double)((uint8_t)g_stAngleData.u32AverageTemp - 197)/(-1.083);
															dbOffcorr = -0.0000006*g_stAngleData.dbAngleTp*g_stAngleData.dbAngleTp*g_stAngleData.dbAngleTp + \
																		0.0001*g_stAngleData.dbAngleTp*g_stAngleData.dbAngleTp - 0.0039*g_stAngleData.dbAngleTp \
																		- 0.0522; 
															g_stAngleData.dbAngleOffcorr = dbOffcorr;
															
															if(g_u8OperatePrintFlag == 0)
															{
																	memset(u8SendData, 0, 32);
																  //��ͷ+����+��ַ+����
																	u8SendData[0] = 0x68;
																  u8SendData[1] = 0x10;
																  u8SendData[2] = 0x00;
																	u8SendData[3] = 0x84;
																
																  //X��ѹ��BCD��
																	//dbTmpValue = g_stAngleData.dbAngleX;
																  dbTmpValue = (g_stAngleData.dbAngleX - g_stAngleData.dbAngleZeroX)*g_stAngleData.dbCalibratKValuX;
																  if(dbTmpValue > 0)
																	{								
																			u32TmpValue = dbTmpValue*10000;
																			u8SendData[4] |= 0<<4;
																		  u8TmpBCD = u32TmpValue/1000000;
																		  u8SendData[4] |= u8TmpBCD;
																		
																		  u32TmpValue = u32TmpValue%1000000;
																		  u8TmpBCD = u32TmpValue/100000;
																		  u8SendData[5] |= u8TmpBCD<<4;
																			u32TmpValue = u32TmpValue%100000;
																		  u8TmpBCD = u32TmpValue/10000;
																			u8SendData[5] |= u8TmpBCD;

																			u32TmpValue = u32TmpValue%10000;
																		  u8TmpBCD = u32TmpValue/1000;
																			u8SendData[6] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%1000;
																		  u8TmpBCD = u32TmpValue/100;
																			u8SendData[6] |= u8TmpBCD;	

																			u32TmpValue = u32TmpValue%100;
																		  u8TmpBCD = u32TmpValue/10;
																			u8SendData[7] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%10;
																		  u8TmpBCD = u32TmpValue/1;
																			u8SendData[7] |= u8TmpBCD;		
																	}
																	else if(dbTmpValue < 0)
																	{
																			u32TmpValue = dbTmpValue*(-10000);
																			u8SendData[4] |= 1<<4;
																		  u8TmpBCD = u32TmpValue/1000000;
																		  u8SendData[4] |= u8TmpBCD;
																		
																		  u32TmpValue = u32TmpValue%1000000;
																		  u8TmpBCD = u32TmpValue/100000;
																		  u8SendData[5] |= u8TmpBCD<<4;
																			u32TmpValue = u32TmpValue%100000;
																		  u8TmpBCD = u32TmpValue/10000;
																			u8SendData[5] |= u8TmpBCD;

																			u32TmpValue = u32TmpValue%10000;
																		  u8TmpBCD = u32TmpValue/1000;
																			u8SendData[6] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%1000;
																		  u8TmpBCD = u32TmpValue/100;
																			u8SendData[6] |= u8TmpBCD;	

																			u32TmpValue = u32TmpValue%100;
																		  u8TmpBCD = u32TmpValue/10;
																			u8SendData[7] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%10;
																		  u8TmpBCD = u32TmpValue/1;
																			u8SendData[7] |= u8TmpBCD;		
																	}
																	else
																	{
																			u8SendData[4] = 0x00;
																			u8SendData[5] = 0x00;
																			u8SendData[6] = 0x00;
																			u8SendData[7] = 0x00;
																	}
	
																  //Y��ѹ��BCD��
																	//dbTmpValue = g_stAngleData.dbAngleY;
																	dbTmpValue = (g_stAngleData.dbAngleY - g_stAngleData.dbAngleZeroY)*g_stAngleData.dbCalibratKValuY;
																  if(dbTmpValue > 0)
																	{								
																			u32TmpValue = dbTmpValue*10000;
																			u8SendData[8] |= 0<<4;
																		  u8TmpBCD = u32TmpValue/1000000;
																		  u8SendData[8] |= u8TmpBCD;
																		
																		  u32TmpValue = u32TmpValue%1000000;
																		  u8TmpBCD = u32TmpValue/100000;
																		  u8SendData[9] |= u8TmpBCD<<4;
																			u32TmpValue = u32TmpValue%100000;
																		  u8TmpBCD = u32TmpValue/10000;
																			u8SendData[9] |= u8TmpBCD;

																			u32TmpValue = u32TmpValue%10000;
																		  u8TmpBCD = u32TmpValue/1000;
																			u8SendData[10] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%1000;
																		  u8TmpBCD = u32TmpValue/100;
																			u8SendData[10] |= u8TmpBCD;	

																			u32TmpValue = u32TmpValue%100;
																		  u8TmpBCD = u32TmpValue/10;
																			u8SendData[11] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%10;
																		  u8TmpBCD = u32TmpValue/1;
																			u8SendData[11] |= u8TmpBCD;		
																	}
																	else if(dbTmpValue < 0)
																	{
																			u32TmpValue = dbTmpValue*(-10000);
																			u8SendData[8] |= 1<<4;
																		  u8TmpBCD = u32TmpValue/1000000;
																		  u8SendData[8] |= u8TmpBCD;
																		
																		  u32TmpValue = u32TmpValue%1000000;
																		  u8TmpBCD = u32TmpValue/100000;
																		  u8SendData[9] |= u8TmpBCD<<4;
																			u32TmpValue = u32TmpValue%100000;
																		  u8TmpBCD = u32TmpValue/10000;
																			u8SendData[9] |= u8TmpBCD;

																			u32TmpValue = u32TmpValue%10000;
																		  u8TmpBCD = u32TmpValue/1000;
																			u8SendData[10] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%1000;
																		  u8TmpBCD = u32TmpValue/100;
																			u8SendData[10] |= u8TmpBCD;	

																			u32TmpValue = u32TmpValue%100;
																		  u8TmpBCD = u32TmpValue/10;
																			u8SendData[11] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%10;
																		  u8TmpBCD = u32TmpValue/1;
																			u8SendData[11] |= u8TmpBCD;		
																	}
																	else
																	{
																			u8SendData[8] = 0x00;
																			u8SendData[9] = 0x00;
																			u8SendData[10] = 0x00;
																			u8SendData[11] = 0x00;
																	}			

																	//�¶�ѹ��BCD��
																	dbTmpValue = g_stAngleData.dbAngleTp;																	
																	if(dbTmpValue > 0)
																	{								
																			u32TmpValue = dbTmpValue*10000;
																			u8SendData[12] |= 0<<4;
																			u8TmpBCD = u32TmpValue/1000000;
																			u8SendData[12] |= u8TmpBCD;
																		
																			u32TmpValue = u32TmpValue%1000000;
																			u8TmpBCD = u32TmpValue/100000;
																			u8SendData[13] |= u8TmpBCD<<4;
																			u32TmpValue = u32TmpValue%100000;
																			u8TmpBCD = u32TmpValue/10000;
																			u8SendData[13] |= u8TmpBCD;

																			u32TmpValue = u32TmpValue%10000;
																			u8TmpBCD = u32TmpValue/1000;
																			u8SendData[14] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%1000;
																			u8TmpBCD = u32TmpValue/100;
																			u8SendData[14] |= u8TmpBCD;	

																			u32TmpValue = u32TmpValue%100;
																			u8TmpBCD = u32TmpValue/10;
																			u8SendData[15] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%10;
																			u8TmpBCD = u32TmpValue/1;
																			u8SendData[15] |= u8TmpBCD;		
																	}
																	else if(dbTmpValue < 0)
																	{
																			u32TmpValue = dbTmpValue*(-10000);
																			u8SendData[12] |= 1<<4;
																			u8TmpBCD = u32TmpValue/1000000;
																			u8SendData[12] |= u8TmpBCD;
																		
																			u32TmpValue = u32TmpValue%1000000;
																			u8TmpBCD = u32TmpValue/100000;
																			u8SendData[13] |= u8TmpBCD<<4;
																			u32TmpValue = u32TmpValue%100000;
																			u8TmpBCD = u32TmpValue/10000;
																			u8SendData[13] |= u8TmpBCD;

																			u32TmpValue = u32TmpValue%10000;
																			u8TmpBCD = u32TmpValue/1000;
																			u8SendData[14] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%1000;
																			u8TmpBCD = u32TmpValue/100;
																			u8SendData[14] |= u8TmpBCD;	

																			u32TmpValue = u32TmpValue%100;
																			u8TmpBCD = u32TmpValue/10;
																			u8SendData[15] |= u8TmpBCD<<4;																		
																			u32TmpValue = u32TmpValue%10;
																			u8TmpBCD = u32TmpValue/1;
																			u8SendData[15] |= u8TmpBCD;		
																	}
																	else
																	{
																			u8SendData[12] = 0x00;
																			u8SendData[13] = 0x00;
																			u8SendData[14] = 0x00;
																			u8SendData[15] = 0x00;
																	}

																	u8SendData[16] = User_GetCRCData(u8SendData, 16);
																	s32SendLen = 17;	
                                  PostUartWriteMsg(u8SendData, s32SendLen);																	
																	
															}
															else
															{
																	PRINT("X = %6.4f Y = %6.4f T = %6.4f\n", (g_stAngleData.dbAngleX - g_stAngleData.dbAngleZeroX)*g_stAngleData.dbCalibratKValuX,\
																			(g_stAngleData.dbAngleY - g_stAngleData.dbAngleZeroY)*g_stAngleData.dbCalibratKValuY, g_stAngleData.dbAngleTp);
															}
															break;
										
										case 0x05://�������
															if(u8Rs232KpBuff[2] == 0x00)
															{
																	if(u8Rs232KpBuff[4] == 0x00)
																	{
																			//X������������
																			g_stAngleData.dbAngleZeroX = 0;
																		  unFloatValue.ftValue = 0;
																			EEPROM_WrteBytes(ANGLE_XZ_ADDR, unFloatValue.u32Value);
																		  PRINT("X Axis Absolute Zero Set Success\n");
																	}
																	else
																	{
																			//X������������
																			g_stAngleData.dbAngleZeroX = g_stAngleData.dbAngleX;
																		  unFloatValue.ftValue = (float)g_stAngleData.dbAngleZeroX;
																			EEPROM_WrteBytes(ANGLE_XZ_ADDR, unFloatValue.u32Value);
																			PRINT("X Axis Relative Zero Set Success %6.4f\n", g_stAngleData.dbAngleZeroX);
																	}
															}
															else
															{
																	if(u8Rs232KpBuff[4] == 0x00)
																	{
																			//Y������������
																			g_stAngleData.dbAngleZeroY = 0;
																			unFloatValue.ftValue = 0;
																			EEPROM_WrteBytes(ANGLE_YZ_ADDR, unFloatValue.u32Value);
																		  PRINT("Y Axis Absolute Zero Set Success\n");
																	}
																	else
																	{
																			//Y������������
																			g_stAngleData.dbAngleZeroY = g_stAngleData.dbAngleY;
																			unFloatValue.ftValue = (float)g_stAngleData.dbAngleZeroY;
																			EEPROM_WrteBytes(ANGLE_YZ_ADDR, unFloatValue.u32Value);
																			PRINT("Y Axis Relative Zero Set Success %6.4f\n", g_stAngleData.dbAngleZeroY);
																	}
															}
															break;
														 
										case 0x1A://���̱궨(��ֵ�궨) 
															dbCalibratValue = 0.0;
															memcpy(u8CalibratData, u8Rs232KpBuff + 4, 4);
															if(u8Rs232KpBuff[2] == 0x00)
															{
																	//X��궨
																	dbCalibratValue += (u8CalibratData[0] & 0x0F)*100;
															    dbCalibratValue += (u8CalibratData[1] >> 4)*10;
																	dbCalibratValue += (u8CalibratData[1] & 0x0F)*1;
															    dbCalibratValue += (u8CalibratData[2] >> 4)*0.1;
																	dbCalibratValue += (u8CalibratData[2] & 0x0F)*0.01;
															 		dbCalibratValue += (u8CalibratData[3] >> 4)*0.001;
																	dbCalibratValue += (u8CalibratData[3] & 0x0F)*0.0001;
															    if((u8CalibratData[0]>>4) == 1) 
																	{
																			dbCalibratValue *= -1.0;
																	}
																	else
																	{
																		  dbCalibratValue *=  1.0;
																	}
																	
																	g_stAngleData.dbCalibratValueX  = dbCalibratValue;
																	g_stAngleData.dbCalibratKValuX = dbCalibratValue/g_stAngleData.dbAngleX;
																	unFloatValue.ftValue = (float)g_stAngleData.dbCalibratKValuX;
																	EEPROM_WrteBytes(ANGLE_KX_ADDR, unFloatValue.u32Value);
																	PRINT("X Calibration Value is %6.4f(Kx = %6.4f)\n", dbCalibratValue,g_stAngleData.dbCalibratKValuX);
															}
															else
															{
																	//Y��궨
																	dbCalibratValue += (u8CalibratData[0] & 0x0F)*100;
															    dbCalibratValue += (u8CalibratData[1] >> 4)*10;
																	dbCalibratValue += (u8CalibratData[1] & 0x0F)*1;
															    dbCalibratValue += (u8CalibratData[2] >> 4)*0.1;
																	dbCalibratValue += (u8CalibratData[2] & 0x0F)*0.01;
															 		dbCalibratValue += (u8CalibratData[3] >> 4)*0.001;
																	dbCalibratValue += (u8CalibratData[3] & 0x0F)*0.0001;
															    if((u8CalibratData[0]>>4) == 1) 
																	{
																			dbCalibratValue *= -1.0;
																	}
																	else
																	{
																		  dbCalibratValue *=  1.0;
																	}
																	
																	g_stAngleData.dbCalibratValueY  = dbCalibratValue;
																	g_stAngleData.dbCalibratKValuY = dbCalibratValue/g_stAngleData.dbAngleY;
																	unFloatValue.ftValue = (float)g_stAngleData.dbCalibratKValuY;
																	EEPROM_WrteBytes(ANGLE_KY_ADDR, unFloatValue.u32Value);
																	PRINT("Y Calibration Value is %6.4f(Ky = %6.4f)\n", dbCalibratValue,g_stAngleData.dbCalibratKValuY);															 
															}
														 
															break;
														 
										case 0x1B://���̱궨(ϵ���궨)
															dbCalibratValue = 0.0;
															memcpy(u8CalibratData, u8Rs232KpBuff + 4, 4);
															if(u8Rs232KpBuff[2] == 0x00)
															{
																	//X��궨
																	dbCalibratValue += (u8CalibratData[0] & 0x0F)*100;
															    dbCalibratValue += (u8CalibratData[1] >> 4)*10;
																	dbCalibratValue += (u8CalibratData[1] & 0x0F)*1;
															    dbCalibratValue += (u8CalibratData[2] >> 4)*0.1;
																	dbCalibratValue += (u8CalibratData[2] & 0x0F)*0.01;
															 		dbCalibratValue += (u8CalibratData[3] >> 4)*0.001;
																	dbCalibratValue += (u8CalibratData[3] & 0x0F)*0.0001;
															    if((u8CalibratData[0]>>4) == 1) 
																	{
																			dbCalibratValue *= -1.0;
																	}
																	else
																	{
																		  dbCalibratValue *=  1.0;
																	}
																	
																	g_stAngleData.dbCalibratKValuX = dbCalibratValue;
																	unFloatValue.ftValue = (float)g_stAngleData.dbCalibratKValuX;
																	EEPROM_WrteBytes(ANGLE_KX_ADDR, unFloatValue.u32Value);
																	PRINT("X Calibration Kx = %6.4f\n", dbCalibratValue);
															}
															else
															{
																	//Y��궨
																	dbCalibratValue += (u8CalibratData[0] & 0x0F)*100;
															    dbCalibratValue += (u8CalibratData[1] >> 4)*10;
																	dbCalibratValue += (u8CalibratData[1] & 0x0F)*1;
															    dbCalibratValue += (u8CalibratData[2] >> 4)*0.1;
																	dbCalibratValue += (u8CalibratData[2] & 0x0F)*0.01;
															 		dbCalibratValue += (u8CalibratData[3] >> 4)*0.001;
																	dbCalibratValue += (u8CalibratData[3] & 0x0F)*0.0001;
															    if((u8CalibratData[0]>>4) == 1) 
																	{
																			dbCalibratValue *= -1.0;
																	}
																	else
																	{
																		  dbCalibratValue *=  1.0;
																	}
																	
																	g_stAngleData.dbCalibratKValuY  = dbCalibratValue;
																	unFloatValue.ftValue = (float)g_stAngleData.dbCalibratKValuY;
																	EEPROM_WrteBytes(ANGLE_KY_ADDR, unFloatValue.u32Value);
																	PRINT("Y Calibration Ky = %6.4f\n", dbCalibratValue);															 
															}							
															break;
														 
										case 0x1C://�ָ���������
															g_stAngleData.dbAngleZeroX = 0;
															g_stAngleData.dbAngleZeroY = 0;
															g_stAngleData.dbCalibratKValuX = 1.0;
															g_stAngleData.dbCalibratKValuY = 1.0;
															unFloatValue.ftValue = 0;
															EEPROM_WrteBytes(ANGLE_XZ_ADDR, unFloatValue.u32Value);
															EEPROM_WrteBytes(ANGLE_YZ_ADDR, unFloatValue.u32Value);
															unFloatValue.ftValue = 1.0;
															EEPROM_WrteBytes(ANGLE_KX_ADDR, unFloatValue.u32Value);
															EEPROM_WrteBytes(ANGLE_KY_ADDR, unFloatValue.u32Value);
											
															PRINT("Factory Set Success\n");
															break;
										
										case 0x1D://ԭʼ���ݴ�ӡ����
															if(g_u8OperatePrintFlag == 0)
															{
																	g_u8OperatePrintFlag = 1;
																  PRINT("PRINT Set Success[String]\n");
															}
															else
															{
																	g_u8OperatePrintFlag = 0;
																  PRINT("PRINT Set Success[Hex]\n");
															}
															break;
														
										case 0x1E://�������
															PRINT("Config Data: %6.4f %6.4f %6.4f %6.4f \n", g_stAngleData.dbAngleZeroX, g_stAngleData.dbAngleZeroY,\
																	g_stAngleData.dbCalibratKValuX, g_stAngleData.dbCalibratKValuY);
															break;
										
										case 0x1F://X���Ƿ��������
											        if(g_u8OperateAnglXFlag == 0)
															{
																	g_u8OperateAnglXFlag = 1;
																  PRINT("Axis X out open\n");
															}
															else
															{
																	g_u8OperateAnglXFlag = 0;
																  PRINT("Axis X out closed\n");
															}
															break;
										
										default  :
											      break;
											
								}
							
						}
					
				}
				else
				{
						vTaskDelay(10);
				}
				/*******************************************************************/
				
		}
	
	
}

