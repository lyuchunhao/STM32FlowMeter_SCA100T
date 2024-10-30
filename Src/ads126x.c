/****************************************************************************
 * Copyright(c), 2001-2060, ihealthlabs 版权所有
 ****************************************************************************
 * 文件名称             :  ADS126x.c
 * 版本                 :  0.0.1
 * 作者                 :  lvchunhao
 * 创建日期             :　2019年8月21日
 * 描述                 :  ADS1263初始化函数
                           1. 模拟SPI
													 2. ADS1263初始化及配置
 ****************************************************************************/

#include "common.h"




static void SPI_Set_CS(char val)
{
    if (val)
        HAL_GPIO_WritePin(ADS1263_CS_GPIO_Port, ADS1263_CS_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(ADS1263_CS_GPIO_Port, ADS1263_CS_Pin, GPIO_PIN_RESET);
}
static void SPI_Set_CLK(char val)
{
    if (val)
        HAL_GPIO_WritePin(ADS1263_CLK_GPIO_Port, ADS1263_CLK_Pin, GPIO_PIN_SET); 
    else
        HAL_GPIO_WritePin(ADS1263_CLK_GPIO_Port, ADS1263_CLK_Pin, GPIO_PIN_RESET);
}
 
static void SPI_Set_Din(char val)
{
    if (val)
        HAL_GPIO_WritePin(ADS1263_DIN_GPIO_Port, ADS1263_DIN_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(ADS1263_DIN_GPIO_Port, ADS1263_DIN_Pin, GPIO_PIN_RESET);
}
static void SPI_Set_START(char val)
{
    if (val)
        HAL_GPIO_WritePin(ADS1263_START_GPIO_Port, ADS1263_START_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(ADS1263_START_GPIO_Port, ADS1263_START_Pin, GPIO_PIN_RESET);
}
static void SPI_Set_RESET(char val)
{
    if (val)
        HAL_GPIO_WritePin(ADS1263_RESET_GPIO_Port, ADS1263_RESET_Pin, GPIO_PIN_SET);
    else
        HAL_GPIO_WritePin(ADS1263_RESET_GPIO_Port, ADS1263_RESET_Pin, GPIO_PIN_RESET);
}
 /****************************************************************************
 * 函数名      : User_SPISendByte()
 * 功能        : SPI发送函数(单字节)
 * 参数        : val [in] : 要发送数据
 * 返回        : 无
 * 作者        : lvchunhao  2019-8-21
 ****************************************************************************/
void User_SPISendByte(uint8_t val)
{
    uint8_t i;
		uint8_t u8Tmp = val;
	
    for (i = 0; i < 8; i++)
    {
        SPI_Set_CLK(0);
        SPI_Set_Din(u8Tmp & 0x80);
        SPI_Set_CLK(1);
        u8Tmp <<= 1;
    }
}
/****************************************************************************
 * 函数名      : User_SPIRecvByte()
 * 功能        : SPI接收函数(单字节)
 * 参数        : 无
 * 返回        : 接收的数据
 * 作者        : lvchunhao  2019-8-21
 ****************************************************************************/
uint8_t User_SPIRecvByte(void)
{
		uint8_t i;
		uint8_t u8Ret = 0;
	
	  for (i = 0; i < 8; i++)
    {
        u8Ret <<= 1;
				SPI_Set_CLK(1);
			
        if(ADS126X_DOUT_READ() != 0)
						u8Ret |= 0x01;
				else
						u8Ret &= 0xFE;
				
        SPI_Set_CLK(0);
    }	
	
		return u8Ret;
}
/****************************************************************************
 * 函数名      : User_SPISendAndRecvByte()
 * 功能        : SPI发送函数(单字节)
 * 参数        : u8Data [in] : 要发送数据
 * 返回        : 无
 * 作者        : lvchunhao  2019-8-21
 * 描述        ：CPOL = 0 CPHA =1
 ****************************************************************************/
uint8_t User_SPISendAndRecvByte(uint8_t val)
{
		uint8_t i, u8Tmp;
	  uint8_t u8Ret = 0;
	
		u8Tmp = val;
		for(i = 0; i < 8 ; i++)
		{
				/* 时钟拉低 */
				SPI_Set_CLK(1);
			
				/* 发送数据 */
				SPI_Set_Din(u8Tmp & 0x80);
				
				u8Tmp <<= 1;
				u8Ret <<= 1;
				
				/* 接收数据 */
				if(ADS126X_DOUT_READ() != 0)
						u8Ret |= 0x01;
				else
						u8Ret &= 0xFE;
				
				/* 时钟拉高 */
				SPI_Set_CLK(0);		
		}
		
		return u8Ret;
}
/****************************************************************************
 * 函数名      : User_ADS126xCMD()
 * 功能        : ADC Commands
 * 参数        : u8CMD [in] : 要发送的命令
 * 返回        : 无
 * 作者        : lvchunhao  2019-8-21
 ****************************************************************************/
void User_ADS126xCMD(uint8_t u8CMD)
{
		SPI_Set_CS(0);
		User_SPISendAndRecvByte(u8CMD);
		SPI_Set_CS(1);
	
	  /* CMD之间需要保持一定时间 */
		__nop();__nop();__nop();__nop();
	  __nop();__nop();__nop();__nop();
}
// read a Signle registers
uint8_t User_ADS126xReadREG(uint32_t u32StartAddress)
{
		uint8_t u8Ret = 0;
	
		uint8_t u8ReadCmd[4] = {0};
		u8ReadCmd[0] = RREG | u32StartAddress;
		u8ReadCmd[1] = 0x00;
		u8ReadCmd[2] = 0x00;
		
		SPI_Set_CS(0);
		u8Ret = User_SPISendAndRecvByte(u8ReadCmd[0]);
		u8Ret = User_SPISendAndRecvByte(u8ReadCmd[1]);
		u8Ret = User_SPISendAndRecvByte(u8ReadCmd[2]);
		SPI_Set_CS(1);
		
		return u8Ret;
}
// read a number of consecutive registers to a given array pointer
void User_ADS126xReadMultiREG(uint32_t u32StartAddress, uint8_t u8Num, uint8_t *u8data)
{
	SPI_Set_CS(0);
	User_SPISendAndRecvByte(RREG + u32StartAddress);
	User_SPISendAndRecvByte(u8Num - 1);
	
	for(int i=0; i<u8Num; i++)
	{
		u8data[i] = User_SPISendAndRecvByte(NOP);
	}
	SPI_Set_CS(1);
}
// write a Signle registers
void User_ADS126xWriteREG(uint32_t u32StartAddress, uint8_t u8CMD)
{
		uint8_t u8WriteCmd[4] = {0};
		u8WriteCmd[0] = WREG | u32StartAddress;
		u8WriteCmd[1] = 0x00;
		u8WriteCmd[2] = u8CMD;
		
		SPI_Set_CS(0);
		User_SPISendAndRecvByte(u8WriteCmd[0]);
		User_SPISendAndRecvByte(u8WriteCmd[1]);
		User_SPISendAndRecvByte(u8WriteCmd[2]);
		SPI_Set_CS(1);
}
// write a number of consecutive registers to a given array pointer
void User_ADS126xWriteMultiREG(uint32_t u32StartAddress, uint8_t u8Num, uint8_t *u8data)
{
		int i = 0;
	  uint8_t u8WriteCmd[32] = {0};
	
		u8WriteCmd[0] = WREG | u32StartAddress;
		u8WriteCmd[1] = u8Num - 1;
		for(i = 0; i < u8Num; i ++)
		{
				u8WriteCmd[2+i] = u8data[i];
		}
		
		SPI_Set_CS(0);		
		for(i = 0; i < (u8Num + 2); i ++)
		{
				User_SPISendAndRecvByte(u8WriteCmd[i]);
		}
		SPI_Set_CS(1);
}
//read ADC data 参考AD1263数据手册章节9.4.7.3.3.1
//In checksum mode, the checksum byte is the lower 8-bit sum of 
//the data conversion bytes plus an offset value 9Bh.
//CRC = u8ReadCmd[2]+u8ReadCmd[3]+u8ReadCmd[4]+u8ReadCmd[5]+0x9B
StADS126xData User_ADS126xReadADC1(void)
{
		int i = 0;
		uint8_t u8ReadCmd[8]  = {0};
		uint8_t u8WriteCmd[8] = {0};
		StADS126xData stADS126xData;
		
		u8WriteCmd[0] = RDATA1;	
		SPI_Set_CS(0);		
		for(i = 0; i < 7; i ++)
		{
				u8ReadCmd[i] = User_SPISendAndRecvByte(u8WriteCmd[i]);
		}
		SPI_Set_CS(1);	

		stADS126xData.u8ADStatus   = u8ReadCmd[1];
		stADS126xData.u8ADCheckSum = u8ReadCmd[6];
		
		stADS126xData.u32ADValue  = 0;
		stADS126xData.u32ADValue |= u8ReadCmd[2] << 24;
		stADS126xData.u32ADValue |= u8ReadCmd[3] << 16;
		stADS126xData.u32ADValue |= u8ReadCmd[4] << 8;
		stADS126xData.u32ADValue |= u8ReadCmd[5] << 0;
		
		__nop();

		return stADS126xData;
}

/****************************************************************************
 * 函数名      : User_ADS126xInit()
 * 功能        : ADS1263初始化函数
 * 参数        : 无
 * 返回        : 无
 * 作者        : lvchunhao  2019-8-21
 ****************************************************************************/
uint8_t u8Read[ADS126x_NUM_REG] = {0};							
uint8_t u8Wrte[ADS126x_NUM_REG] = {0};	
void User_ADS126xInit(void)
{
	
		SPI_Set_RESET(0);
	  vTaskDelay(5);
		SPI_Set_RESET(1);
	
		SPI_Set_START(0);				
	
		/* Reset the ADC */
		User_ADS126xCMD(RESET);
		vTaskDelay(5);
	
		/* Stop ADC Conversions */
		User_ADS126xCMD(STOP1);
		User_ADS126xCMD(STOP2);
		vTaskDelay(10);
	
		/* Read All Register */
		User_ADS126xReadMultiREG(ID, ADS126x_NUM_REG, u8Read);
	
		__nop();
		__nop();
	
		/* 设置AIN7(TDACN)=2.75V AIN6(TDACP)=2.5V*/
		User_ADS126xWriteREG(TDACP, 0x80);
		User_ADS126xWriteREG(TDACN, 0x86);

		/* 设置校验模式为CheckSum */
		User_ADS126xWriteREG(INTERFACE, 0x05);
		/* 设置PGA为Bypassed 速率：20Hz*/
		User_ADS126xWriteREG(MODE2, 0x84);
		/* 设置外部参考电压反转(保证正参考) */
		User_ADS126xWriteREG(MODE0,  0x80);
		/* 设置参考电压为外部参考AN0/AN1 */
		User_ADS126xWriteREG(REFMUX, 0x09);
		/* 设置AN2与ANCOM之间的差分 */
		User_ADS126xWriteREG(INPMUX, 0x2A);
		
		/* 设置滤波模式为FIR Mode */
		User_ADS126xWriteREG(MODE1,  0x80);
	
		User_ADS126xCMD(START1);
		vTaskDelay(10);
}
/* ADS1263通道切换 */
void User_ADS126xAINxSwitch(uint8_t u8AIN)
{
		User_ADS126xCMD(STOP1);
		vTaskDelay(1);
		switch(u8AIN)
		{
				case  ADS1263_AIN2_A2OUT1:
					    //AIN2-AINCOM
							User_ADS126xWriteREG(INPMUX, 0x2A);
							break;

				case  ADS1263_AIN3_A2OUT2:
							//AIN3-AINCOM
							User_ADS126xWriteREG(INPMUX, 0x3A);
					    break;
				
				case  ADS1263_AIN4_A1OUT2:
					    //AIN4-AINCOM
							User_ADS126xWriteREG(INPMUX, 0x4A);
							break;

				case  ADS1263_AIN5_A1OUT1:
							//AIN5-AINCOM
							User_ADS126xWriteREG(INPMUX, 0x5A);
					    break;
			  
			  default:
					    break;
		}
		
		User_ADS126xCMD(START1);
		
}

