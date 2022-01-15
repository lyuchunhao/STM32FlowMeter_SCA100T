/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs 版权所有
 ****************************************************************************
 * 文件名称             :  ADS126x.h
 * 版本                 :  0.0.1
 * 作者                 :  lvchunhao
 * 创建日期             :　2019年8月29日
 * 描述                 :  ADS1263初始化函数
                           1. 模拟SPI
													 2. ADS1263初始化及配置
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
	 


	 
	 
/* AD263片选CS */
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

