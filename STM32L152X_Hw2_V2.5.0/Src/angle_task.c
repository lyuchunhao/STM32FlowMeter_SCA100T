/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs 版权所有
 ****************************************************************************
 * 文件名称             :  angle_task.c
 * 版本                 :  0.0.1
 * 作者                 :  lvchunhao
 * 创建日期             :　2019年8月29日
 * 描述                 :  SCA100T角度处理
                           1. 模拟SPI
													 2. SCA100T初始化及配置
 ****************************************************************************/
#include "common.h"


StAngleData   g_stAngleData;
StADS126xData g_stADS126xData;
StSCA100TData g_stSCA100TData;

double g_dbDigtalZeroAngleXY = 0.0;
double g_dbAnalogZeroAngleXY = 0.0;

uint8_t g_u8OperatePrintFlag = 0;
uint8_t g_u8OperateAnglXFlag = 0;




void EEPROM_ReadBytes(uint16_t Addr, uint8_t *Buffer, uint16_t Length)
{
	uint8_t *wAddr;
	wAddr=(uint8_t *)(EEPROM_BASE_ADDR+Addr);
	while(Length--)
	{
		*Buffer++=*wAddr++;
	}	

}

void EEPROM_ReadWords(uint16_t Addr, uint16_t *Buffer, uint16_t Length)
{
	uint32_t *wAddr;
	wAddr=(uint32_t *)(EEPROM_BASE_ADDR+Addr);
	while(Length--)
	{
		*Buffer++=*wAddr++;
	}	
}


void EEPROM_WriteBytes(uint16_t Addr, uint8_t *Buffer, uint16_t Length)
{
	uint8_t *wAddr;
	wAddr=(uint8_t *)(EEPROM_BASE_ADDR+Addr);
	DIS_INT
	FLASH->PEKEYR=PEKEY1;				    //unlock
	FLASH->PEKEYR=PEKEY2;
	while(FLASH->PECR&FLASH_PECR_PELOCK);
	FLASH->PECR|=FLASH_PECR_FTDW;		//not fast write
	while(Length--){
		*wAddr++=*Buffer++;
		while(FLASH->SR&FLASH_SR_BSY);
	}
	FLASH->PECR|=FLASH_PECR_PELOCK;
	EN_INT
}

void EEPROM_WriteWords(uint16_t Addr,uint16_t *Buffer,uint16_t Length)
{
	uint32_t *wAddr;
	wAddr=(uint32_t *)(EEPROM_BASE_ADDR+Addr);
	DIS_INT
	FLASH->PEKEYR=PEKEY1;				    //unlock
	FLASH->PEKEYR=PEKEY2;
	while(FLASH->PECR&FLASH_PECR_PELOCK);
	FLASH->PECR|=FLASH_PECR_FTDW;		//not fast write
	while(Length--){
		*wAddr++=*Buffer++;
		while(FLASH->SR&FLASH_SR_BSY);
	}
	FLASH->PECR|=FLASH_PECR_PELOCK;
	EN_INT
}

void EEPROM_WrteBytes(uint32_t u32Addr, uint32_t u32Data)
{
		HAL_FLASHEx_DATAEEPROM_Unlock();
    HAL_FLASHEx_DATAEEPROM_Program( FLASH_TYPEPROGRAMDATA_WORD,  EEPROM_BASE_ADDR + u32Addr,  u32Data);
    //HAL_FLASHEx_DATAEEPROM_Erase(FLASH_TYPEERASEDATA_WORD, EEPROM_BASE_ADDR);
    HAL_FLASHEx_DATAEEPROM_Lock();
}


/*
 * 函数名    ：vTaskOperationAngle()
 * 功能      : 角度处理函数
 * 参数      : pArgs [in] : 线程参数
 * 返回      : none
 * 作者      : lyuchunhao 2019-8-29
 */
void vTaskOperationAngle(void *pArgs)
{
	  uint8_t u8TempValue = 0;
	  uint32_t u32OprateAngleNum = 0;
	  uint8_t u8AngleConfigData[CONFIG_INFO_SIZE] = {0};
		Unfloat unFloatValue;
	
	  /* 获取E2PPROM配置数据 */
		EEPROM_ReadBytes(CONFIG_INFO_ADDR, u8AngleConfigData, CONFIG_INFO_SIZE);
		g_stAngleData.u16AxOUTxIndex = 0;
		memcpy(unFloatValue.u8Data, u8AngleConfigData+0, 4);
		g_stAngleData.dbAngleZeroX = unFloatValue.ftValue;
		memcpy(unFloatValue.u8Data, u8AngleConfigData+4, 4);
		g_stAngleData.dbAngleZeroY = unFloatValue.ftValue;
		/* X轴零点+Y轴零点+X轴标定系数+Y轴标定系数 */
		memcpy(unFloatValue.u8Data, u8AngleConfigData+8, 4);
		if(unFloatValue.ftValue == 0)
		{
				g_stAngleData.dbCalibratKValuX = 1.0;
		}
		else
		{
			  g_stAngleData.dbCalibratKValuX = unFloatValue.ftValue;
		}
		memcpy(unFloatValue.u8Data, u8AngleConfigData+12, 4);
		if(unFloatValue.ftValue == 0)
		{
				g_stAngleData.dbCalibratKValuY = 1.0;
		}
		else
		{
			  g_stAngleData.dbCalibratKValuY = unFloatValue.ftValue;
		}
	
		User_ADS126xInit();
		User_SCA100TInit();
		PostUartWriteMsg((uint8_t *)"vTaskOperationAngle is Start!\n", 30);
		vTaskDelay(100);
		PRINT("Config Data: %6.4f %6.4f %6.4f %6.4f \n", g_stAngleData.dbAngleZeroX, g_stAngleData.dbAngleZeroY,\
					g_stAngleData.dbCalibratKValuX, g_stAngleData.dbCalibratKValuY);
	
		while(1)
		{
			
			/* CPOL = 0 CPHA =1 */
			/* ADS1263采用20bit:最高位为符号位满量程为2~19 = 524287 */

			/* CPOL = 0 CPHA = 0 */
			/* 获取SCA100T内部11bit角度:精度稍差 */
			
			/**************************************************************************************************/
			//切换通道AIN2-IC2(X轴)
			User_ADS126xAINxSwitch(ADS1263_AIN2_A2OUT1);
			while(1)
			{
					__nop();
				  __nop();
					if(0 == ADS126X_DRDY_READ())
					{
							g_stADS126xData = User_ADS126xReadADC1();
						  g_stAngleData.u32A2OUT1[g_stAngleData.u16AxOUTxIndex] = g_stADS126xData.u32ADValue >> 12;
						  break;
					}
					else
					{
							vTaskDelay(1);
					}
			}
			//切换通道AIN3-IC2(Y轴)
			//vTaskDelay(1);
			User_ADS126xAINxSwitch(ADS1263_AIN3_A2OUT2);
			while(1)
			{
					__nop();
				  __nop();
					if(0 == ADS126X_DRDY_READ())
					{
							g_stADS126xData = User_ADS126xReadADC1();
							g_stAngleData.u32A2OUT2[g_stAngleData.u16AxOUTxIndex] = g_stADS126xData.u32ADValue >> 12;
						  break;
					}
					else
					{
							vTaskDelay(1);
					}
			}
			
			
			if(g_u8OperateAnglXFlag == 1)
			{
				//切换通道AIN4-IC1(Y轴)
				//vTaskDelay(1);
				User_ADS126xAINxSwitch(ADS1263_AIN4_A1OUT2);
				while(1)
				{
						__nop();
						__nop();
						if(0 == ADS126X_DRDY_READ())
						{
								g_stADS126xData = User_ADS126xReadADC1();
								g_stAngleData.u32A1OUT2[g_stAngleData.u16AxOUTxIndex] = g_stADS126xData.u32ADValue >> 12;
								break;
						}
						else
						{
								vTaskDelay(1);
						}
				}
				//切换通道AIN5-IC1(X轴)
				//vTaskDelay(1);
				User_ADS126xAINxSwitch(ADS1263_AIN5_A1OUT1);
				while(1)
				{
						__nop();
						__nop();
						if(0 == ADS126X_DRDY_READ())
						{
								g_stADS126xData = User_ADS126xReadADC1();
								g_stAngleData.u32A1OUT1[g_stAngleData.u16AxOUTxIndex] = g_stADS126xData.u32ADValue >> 12;
								break;
						}
						else
						{
								vTaskDelay(1);
						}
				}
		  }
			else
			{
				g_stAngleData.u32A1OUT2[g_stAngleData.u16AxOUTxIndex] = 0;
				g_stAngleData.u32A1OUT1[g_stAngleData.u16AxOUTxIndex] = 0;
			}
			
			/* 获取SCA100T内部温度：精度+-1C */
			u8TempValue = User_SCA100TReadTempetature();
			g_stAngleData.u8AngleTp[g_stAngleData.u16AxOUTxIndex] = u8TempValue;
			
			u32OprateAngleNum++;
			g_stAngleData.u16AxOUTxIndex++;
			if(g_stAngleData.u16AxOUTxIndex >= ADS_BUFF_SIZE)
			{
					g_stAngleData.u16AxOUTxIndex = 0;
			}
			/**************************************************************************************************/
			
			vTaskDelay(1);
			
		}	
	
}
