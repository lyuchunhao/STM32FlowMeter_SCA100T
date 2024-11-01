/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * Copyright (c) 2019 STMicroelectronics International N.V. 
  * All rights reserved.
  *
  * Redistribution and use in source and binary forms, with or without 
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice, 
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other 
  *    contributors to this software may be used to endorse or promote products 
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this 
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under 
  *    this license is void and will automatically terminate your rights under 
  *    this license. 
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SCA1_ST1_Pin GPIO_PIN_2
#define SCA1_ST1_GPIO_Port GPIOA
#define SCA1_ST2_Pin GPIO_PIN_3
#define SCA1_ST2_GPIO_Port GPIOA
#define SCA1_CS_Pin GPIO_PIN_4
#define SCA1_CS_GPIO_Port GPIOA
#define SCA1_SCK_Pin GPIO_PIN_5
#define SCA1_SCK_GPIO_Port GPIOA
#define SCA1_MISO_Pin GPIO_PIN_6
#define SCA1_MISO_GPIO_Port GPIOA
#define SCA1_MOSI_Pin GPIO_PIN_7
#define SCA1_MOSI_GPIO_Port GPIOA
#define ADS1263_RESET_Pin GPIO_PIN_0
#define ADS1263_RESET_GPIO_Port GPIOB
#define SCA2_ST1_Pin GPIO_PIN_10
#define SCA2_ST1_GPIO_Port GPIOB
#define SCA2_ST2_Pin GPIO_PIN_11
#define SCA2_ST2_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_12
#define SPI2_CS_GPIO_Port GPIOB
#define Rs485_ENABLE_Pin GPIO_PIN_8
#define Rs485_ENABLE_GPIO_Port GPIOA
#define ADS1263_DRDY_Pin GPIO_PIN_4
#define ADS1263_DRDY_GPIO_Port GPIOB
#define ADS1263_DOUT_Pin GPIO_PIN_5
#define ADS1263_DOUT_GPIO_Port GPIOB
#define ADS1263_DIN_Pin GPIO_PIN_6
#define ADS1263_DIN_GPIO_Port GPIOB
#define ADS1263_CLK_Pin GPIO_PIN_7
#define ADS1263_CLK_GPIO_Port GPIOB
#define ADS1263_CS_Pin GPIO_PIN_8
#define ADS1263_CS_GPIO_Port GPIOB
#define ADS1263_START_Pin GPIO_PIN_9
#define ADS1263_START_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
