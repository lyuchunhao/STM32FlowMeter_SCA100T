/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs ��Ȩ����
 ****************************************************************************
 * �ļ�����             :  sca100t.c
 * �汾                 :  0.0.1
 * ����                 :  lvchunhao
 * ��������             :��2019��8��29��
 * ����                 :  SCA100T��ʼ������
                           1. ģ��SPI2
													 2. SCA100T��ʼ��������
 ****************************************************************************/

#include "common.h"



void User_SCA100TInit(void)
{
		uint8_t  u8CMD[2]  = {0};
	
		u8CMD[0] = MEAS;
		SCA100T_CS_LOW();
		HAL_SPI_Transmit(&hspi2, u8CMD, 1, 10);
		SCA100T_CS_HIGH();
}

uint16_t User_SCA100TReadAngleX(void)
{
		uint8_t  u8Data[4] = {0};
		uint8_t  u8CMD[4]  = {0};
		uint16_t u16Ret = 0;
	
		u8CMD[0] = RADX;
		SCA100T_CS_LOW();
		HAL_SPI_TransmitReceive(&hspi2, u8CMD, u8Data, 3, 10);
		SCA100T_CS_HIGH();
		
		u16Ret |= u8Data[1] << 8;
		u16Ret |= u8Data[2];
		u16Ret = u16Ret >> 5;
		
		return u16Ret;
}

uint16_t User_SCA100TReadAngleY(void)
{
		uint8_t  u8Data[4] = {0};
		uint8_t  u8CMD[4]  = {0};
		uint16_t u16Ret = 0;
	
		u8CMD[0] = RADY;
		SCA100T_CS_LOW();
		HAL_SPI_TransmitReceive(&hspi2, u8CMD, u8Data, 3, 10);
		SCA100T_CS_HIGH();
		
		u16Ret |= u8Data[1] << 8;
		u16Ret |= u8Data[2];
		u16Ret = u16Ret >> 5;
		
		return u16Ret;
}

uint8_t User_SCA100TReadTempetature(void)
{
		uint8_t  u8Data[4] = {0};
		uint8_t  u8CMD[4]  = {0};
		uint8_t  u8Ret = 0;
	
		u8CMD[0] = RWTR;
		SCA100T_CS_LOW();
		HAL_SPI_TransmitReceive(&hspi2, u8CMD, u8Data, 2, 10);
		SCA100T_CS_HIGH();
		
		u8Ret |= u8Data[1];
		
		return u8Ret;
}

