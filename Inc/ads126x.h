/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs 版权所有
 ****************************************************************************
 * 文件名称             :  ADS126x.h
 * 版本                 :  0.0.1
 * 作者                 :  lvchunhao
 * 创建日期             :　2019年8月21日
 * 描述                 :  ADS1263初始化函数
                           1. 模拟SPI
													 2. ADS1263初始化及配置
 ****************************************************************************/
 
#ifndef _ADS126X_H
#define _ADS126X_H

#include "common.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
/* AD263片选CS */
#define ADS126X_CS_HIGH()  HAL_GPIO_WritePin(ADS1263_CS_GPIO_Port, ADS1263_CS_Pin, GPIO_PIN_SET)
#define ADS126X_CS_LOW()   HAL_GPIO_WritePin(ADS1263_CS_GPIO_Port, ADS1263_CS_Pin, GPIO_PIN_RESET)
/* AD263时钟CLK */
#define ADS126X_CLK_HIGH()  HAL_GPIO_WritePin(ADS1263_CLK_GPIO_Port, ADS1263_CLK_Pin, GPIO_PIN_SET) 
#define ADS126X_CLK_LOW()   HAL_GPIO_WritePin(ADS1263_CLK_GPIO_Port, ADS1263_CLK_Pin, GPIO_PIN_RESET)
/* AD263输入DIN */
#define ADS126X_DIN_HIGH()  HAL_GPIO_WritePin(ADS1263_DIN_GPIO_Port, ADS1263_DIN_Pin, GPIO_PIN_SET) 
#define ADS126X_DIN_LOW()   HAL_GPIO_WritePin(ADS1263_DIN_GPIO_Port, ADS1263_DIN_Pin, GPIO_PIN_RESET) 	
/* AD263输出状态DOUT/DRDY */
#define ADS126X_DOUT_READ() HAL_GPIO_ReadPin(ADS1263_DOUT_GPIO_Port, ADS1263_DOUT_Pin)
#define ADS126X_DRDY_READ() HAL_GPIO_ReadPin(ADS1263_DRDY_GPIO_Port, ADS1263_DRDY_Pin)	
	 
/* RS485发送使能 */
#define RS485_SEND_ENABLE() HAL_GPIO_WritePin(Rs485_ENABLE_GPIO_Port, Rs485_ENABLE_Pin, GPIO_PIN_SET)
#define RS485_RECV_ENABLE() HAL_GPIO_WritePin(Rs485_ENABLE_GPIO_Port, Rs485_ENABLE_Pin, GPIO_PIN_RESET)		 
	 
#define ADS1263
	 
#ifdef  ADS1262
	#define ADS126x_NUM_REG 		(0x15)			/* ADS1262 has 21 registers */
#endif
#ifdef  ADS1263
	#define ADS126x_NUM_REG 		(0x1B)			/* ADS1263 has 27 registers */
#endif

/* ADC Commands */
#define NOP									  (0x00)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define RESET 								(0x06)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define START1								(0x08)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define STOP1								  (0x0A)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define RDATA1								(0x12)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define SYOCAL1								(0x16)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define SYGCAL1								(0x17)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define SFOCAL1								(0x19)			/* ID/CFG REGISTER (ADDRESS 00h) */
//Multi-Byte Commands
#define RREG							    (0x20)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define WREG							    (0x40)			/* ID/CFG REGISTER (ADDRESS 00h) */
/* Additional ADS1263 Commands */
#ifdef  ADS1263
#define START2							  (0x0C)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define STOP2							    (0x0E)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define RDATA2							  (0x14)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define SYOCAL2							  (0x1B)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define SYGCAL2							  (0x1C)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define SFOCAL2							  (0x1E)			/* ID/CFG REGISTER (ADDRESS 00h) */
#endif

/* STATUS Byte Masks */
#define	ADC2_NEW							(0x80)			/* Indicates new ADC2 data */
#define	ADC1_NEW							(0x40)			/* Indicates new ADC1 data */
#define	EXTCLK								(0x20)			/* Indicates ADC clock source */
#define	REF_ALM								(0x10)			/* Low Reference Alarm   - Only used with ADC1 */
#define	PGAL_ALM							(0x08)			/* PGA Output Low Alarm  - Only used with ADC1 */
#define	PGAH_ALM							(0x04)			/* PGA Output High Alarm - Only used with ADC1 */
#define	PGAD_ALM							(0x02)			/* PGA Diff Output Alarm - Only used with ADC1 */
#define	RST_ALM								(0x01)			/* Indicates device reset (re-named to avoid conflict) */


/* Register Addresses */
#define ID										(0x00)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define POWER1 								(0x01)			/* CONFIGURATION REGISTER 0 (ADDRESS 01h) */
#define INTERFACE							(0x02)			/* CONFIGURATION REGISTER 1 (ADDRESS 02h) */
#define MODE0									(0x03)			/* High-Pass Filter Corner Frequency, Low Byte (Address 03h) */
#define MODE1									(0x04)			/* High-Pass Filter Corner Frequency, High Byte (Address 04h) */
#define MODE2									(0x05)			/* Offset Calibration, Low Byte (Address 05h) */
#define INPMUX								(0x06)			/* Offset Calibration, Mid Byte (Address 06h) */
#define OFCAL0								(0x07)			/* Offset Calibration, High Byte (Address 07h) */
#define OFCAL1								(0x08)			/* Full-Scale Calibration, Low Byte (Address 08h) */
#define OFCAL2								(0x09)			/* Full-Scale Calibration, Mid Byte (Address 09h) */
#define FSCAL0								(0x0A)			/* Full-Scale Calibration, High Byte (Address 0Ah) */
#define FSCAL1								(0x0B)
#define FSCAL2								(0x0C)
#define IDACMUX								(0x0D)
#define IDACMAG								(0x0E)
#define REFMUX								(0x0F)
#define TDACP									(0x10)
#define TDACN									(0x11)
#define GPIOCON								(0x12)
#define GPIODIR								(0x13)
#define GPIODAT								(0x14)
/* Additional ADS1263 Registers */
#ifdef  ADS1263
#define ADC2CFG								(0x15)
#define ADC2MUX								(0x16)
#define ADC2OFC0							(0x17)
#define ADC2OFC1							(0x18)
#define ADC2FSC0							(0x19)
#define ADC2FSC1							(0x1A)
#endif /* ADS1263 */ 


/* Default Register Values */
#define ID_DEFAULT_VALUE					  (0x23)			/* ID/CFG REGISTER (ADDRESS 00h) */
#define POWER_DEFAULT_VALUE					(0x11)			/* CONFIGURATION REGISTER 0 (ADDRESS 01h) */
#define INTERFACE_DEFAULT_VALUE			(0x05)		  /* CONFIGURATION REGISTER 1 (ADDRESS 02h) */
#define MODE0_DEFAULT_VALUE					(0x00)			/* High-Pass Filter Corner Frequency, Low Byte (Address 03h) */
#define MODE1_DEFAULT_VALUE					(0x80)			/* High-Pass Filter Corner Frequency, High Byte (Address 04h) */
#define MODE2_DEFAULT_VALUE					(0x04)			/* Offset Calibration, Low Byte (Address 05h) */
#define INPMUX_DEFAULT_VALUE				(0x01)			/* Offset Calibration, Mid Byte (Address 06h) */
#define OFCAL0_DEFAULT_VALUE				(0x00)			/* Offset Calibration, High Byte (Address 07h) */
#define OFCAL1_DEFAULT_VALUE				(0x00)			/* Full-Scale Calibration, Low Byte (Address 08h) */
#define OFCAL2_DEFAULT_VALUE				(0x00)			/* Full-Scale Calibration, Mid Byte (Address 09h) */
#define FSCAL0_DEFAULT_VALUE				(0x00)			/* Full-Scale Calibration, High Byte (Address 0Ah) */
#define FSCAL1_DEFAULT_VALUE				(0x00)
#define FSCAL2_DEFAULT_VALUE				(0x40)
#define IDACMUX_DEFAULT_VALUE				(0xBB)
#define IDACMAG_DEFAULT_VALUE				(0x00)
#define REFMUX_DEFAULT_VALUE				(0x00)
#define TDACP_DEFAULT_VALUE					(0x00)
#define TDACN_DEFAULT_VALUE					(0x00)
#define GPIOCON_DEFAULT_VALUE				(0x00)
#define GPIODIR_DEFAULT_VALUE				(0x00)
#define GPIODAT_DEFAULT_VALUE				(0x00)
#ifdef  ADS1263
#define ADC2CFG_DEFAULT_VALUE			  (0x00)
#define ADC2MUX_DEFAULT_VALUE			  (0x01)
#define ADC2OFC0_DEFAULT_VALUE			(0x00)
#define ADC2OFC1_DEFAULT_VALUE			(0x00)
#define ADC2FSC0_DEFAULT_VALUE			(0x00)
#define ADC2FSC1_DEFAULT_VALUE			(0x40)
#endif /* ADS1263 */






void User_ADS126xInit(void);
void User_ADS126xAINxSwitch(uint8_t u8AIN);


	  
#ifdef __cplusplus
}
#endif

#endif /* _ADS126X_H */
