/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs ��Ȩ����
 ****************************************************************************
 * �ļ�����             :  ADS126x.h
 * �汾                 :  0.0.1
 * ����                 :  lvchunhao
 * ��������             :��2019��8��29��
 * ����                 :  ADS1263��ʼ������
                           1. ģ��SPI
													 2. ADS1263��ʼ��������
 ****************************************************************************/
 
#ifndef _SCA100T_H
#define _SCA100T_H

#include "common.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
	 
#define MEAS  0x00
#define RWTR  0x08
#define RDSR  0x0A
#define RLOAD 0x0B
#define STX   0x0E
#define STY   0x0F
#define RADX  0x10
#define RADY  0x11
	 


	 
	 
/* AD263ƬѡCS */
#define SCA100T_CS_HIGH()  HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_SET)
#define SCA100T_CS_LOW()   HAL_GPIO_WritePin(SPI2_CS_GPIO_Port, SPI2_CS_Pin, GPIO_PIN_RESET)
	 
	 
void User_SCA100TInit(void);
uint16_t User_SCA100TReadAngleX(void);
uint16_t User_SCA100TReadAngleY(void);
uint8_t User_SCA100TReadTempetature(void);
	 

#ifdef __cplusplus
}
#endif

#endif /* _SCA100T_H */

