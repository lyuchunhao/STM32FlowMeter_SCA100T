/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs 版权所有
 ****************************************************************************
 * 文件名称             :  angle_task.h
 * 版本                 :  0.0.1
 * 作者                 :  lvchunhao
 * 创建日期             :　2019年8月29日
 * 描述                 :  SCA100T角度处理
                           1. 模拟SPI
													 2. SCA100T初始化及配置
 ****************************************************************************/
 
#ifndef _ANGLE_TASK_H
#define _ANGLE_TASK_H

#include "common.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
#define  ADS_BUFF_SIZE      8	 
	
#define  CONFIG_INFO_ADDR	  0x10          //配置地址
#define  CONFIG_INFO_SIZE   32            //配置大小
	 
#define  ANGLE_XZ_ADDR      CONFIG_INFO_ADDR+0
#define  ANGLE_YZ_ADDR      CONFIG_INFO_ADDR+4
#define  ANGLE_KX_ADDR      CONFIG_INFO_ADDR+8
#define  ANGLE_KY_ADDR      CONFIG_INFO_ADDR+12

#define  EEPROM_BASE_ADDR		0x08080000	
#define  EEPROM_BYTE_SIZE		0x0FFF
#define  PEKEY1							0x89ABCDEF		//FLASH_PEKEYR
#define  PEKEY2							0x02030405		//FLASH_PEKEYR
	 
#define  EN_INT      	__enable_irq();		  //系统开全局中断
#define  DIS_INT     	__disable_irq();	  //系统关全局中断
	 
typedef union tag_Unfloat
{
		float    ftValue;
	  uint8_t  u8Data[4];
		uint32_t u32Value;
}Unfloat;
	 
typedef struct tag_StADS126xData{
		uint32_t u32ADValue;
		uint8_t  u8ADStatus;
		uint8_t  u8ADCheckSum;
}StADS126xData;
	 
typedef struct tag_StSCA100TData{
		uint16_t u16AngleX;
		uint16_t u16AngleY;
		uint16_t u16AngleXY;
	  uint16_t u16AngleTp;
	
		double dbAngleX;
	  double dbAngleY;
		double dbAngleXY;
	  double dbAngleTp;
}StSCA100TData;	 
	 
typedef struct tag_StAngleData{
		uint32_t u32A1OUT1[ADS_BUFF_SIZE];
		uint32_t u32A1OUT2[ADS_BUFF_SIZE];
		uint32_t u32A2OUT1[ADS_BUFF_SIZE];
		uint32_t u32A2OUT2[ADS_BUFF_SIZE];
	
		uint8_t  u8AngleTp[ADS_BUFF_SIZE];
	  
		uint16_t u16AxOUTxIndex;
	  uint16_t u16SCA100TTemp;
	
		uint32_t u32AverageTemp;
	  uint32_t u32AverageA1T1;
		uint32_t u32AverageA1T2;
		uint32_t u32AverageA2T1;
		uint32_t u32AverageA2T2;
	
	  double dbAngleX;
	  double dbAngleY;
	  double dbAngleTp;
	  double dbAngleOffcorr;
	  double dbAngleZeroX;
	  double dbAngleZeroY;
	  double dbCalibratValueX;    //输入标定值
	  double dbCalibratValueY;
		double dbCalibratKValuX;    //标定系数
		double dbCalibratKValuY;

}StAngleData;
	 
extern double g_dbDigtalZeroAngleXY;
extern double g_dbAnalogZeroAngleXY;
extern StAngleData   g_stAngleData;
extern StSCA100TData g_stSCA100TData;
extern StADS126xData g_stADS126xData;

extern uint8_t g_u8OperatePrintFlag;
extern uint8_t g_u8OperateAnglXFlag;
	 
void vTaskOperationAngle(void *pArgs);	 
StADS126xData User_ADS126xReadADC1(void);	 

void EEPROM_ReadBytes(uint16_t Addr, uint8_t *Buffer, uint16_t Length);
void EEPROM_WrteBytes(uint32_t u32Addr, uint32_t u32Data);
	 

#ifdef __cplusplus
}
#endif

#endif /* _ANGLE_TASK_H */
