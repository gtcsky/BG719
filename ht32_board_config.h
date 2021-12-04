/*********************************************************************************************************//**
 * @file    ADC/Two_Group_MaxCH/ht32_board_config.h
 * @version $Rev:: 4033         $
 * @date    $Date:: 2019-07-12 #$
 * @brief   The header file of board configuration.
 *************************************************************************************************************
 * @attention
 *
 * Firmware Disclaimer Information
 *
 * 1. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, which is supplied by Holtek Semiconductor Inc., (hereinafter referred to as "HOLTEK") is the
 *    proprietary and confidential intellectual property of HOLTEK, and is protected by copyright law and
 *    other intellectual property laws.
 *
 * 2. The customer hereby acknowledges and agrees that the program technical documentation, including the
 *    code, is confidential information belonging to HOLTEK, and must not be disclosed to any third parties
 *    other than HOLTEK and the customer.
 *
 * 3. The program technical documentation, including the code, is provided "as is" and for customer reference
 *    only. After delivery by HOLTEK, the customer shall use the program technical documentation, including
 *    the code, at their own risk. HOLTEK disclaims any expressed, implied or statutory warranties, including
 *    the warranties of merchantability, satisfactory quality and fitness for a particular purpose.
 *
 * <h2><center>Copyright (C) Holtek Semiconductor Inc. All rights reserved</center></h2>
 ************************************************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------------------------------------*/
#ifndef __HT32_BOARD_CONFIG_H
#define __HT32_BOARD_CONFIG_H
#include "ht32f5xxxx_tm.h"
#ifdef __cplusplus
 extern "C" {
#endif

/* Settings ------------------------------------------------------------------------------------------------*/


#if defined(USE_HT32F50220_30) || defined(USE_HT32F50231_41) || defined(USE_HT32F52231_41) || defined(USE_HT32F52331_41)
  #define LIBCFG_ADC_CH_TYPE              (0)
#endif



//#if (LIBCFG_ADC_CH_TYPE==0)
  #define HTCFG_ADCCH0_GPIOX                      B
  #define HTCFG_ADCCH1_GPIOX                      B
  #define HTCFG_ADCCH2_GPIOX                      A
  #define HTCFG_ADCCH3_GPIOX                      A
  #define HTCFG_ADCCH4_GPIOX                      A
  #define HTCFG_ADCCH5_GPIOX                      A
  #define HTCFG_ADCCH6_GPIOX                      A
  #define HTCFG_ADCCH7_GPIOX                      A
  #define HTCFG_ADCCH8_GPIOX                      A
  #define HTCFG_ADCCH9_GPIOX                      A
  #define HTCFG_ADCCH10_GPIOX                     C
  #define HTCFG_ADCCH11_GPIOX                     C

  #define HTCFG_ADCCH0_AFION                      7
  #define HTCFG_ADCCH1_AFION                      8
  #define HTCFG_ADCCH2_AFION                      0
  #define HTCFG_ADCCH3_AFION                      1
  #define HTCFG_ADCCH4_AFION                      2
  #define HTCFG_ADCCH5_AFION                      3
  #define HTCFG_ADCCH6_AFION                      4
  #define HTCFG_ADCCH7_AFION                      5
  #define HTCFG_ADCCH8_AFION                      6
  #define HTCFG_ADCCH9_AFION                      7
  #define HTCFG_ADCCH10_AFION                     4
  #define HTCFG_ADCCH11_AFION                     5
//#endif


#define HTCFG_ADCCH0_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH0_GPIOX)
#define HTCFG_ADCCH1_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH1_GPIOX)
#define HTCFG_ADCCH2_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH2_GPIOX)
#define HTCFG_ADCCH3_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH3_GPIOX)
#define HTCFG_ADCCH4_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH4_GPIOX)
#define HTCFG_ADCCH5_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH5_GPIOX)
#define HTCFG_ADCCH6_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH6_GPIOX)
#define HTCFG_ADCCH7_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH7_GPIOX)
#define HTCFG_ADCCH8_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH8_GPIOX)
#define HTCFG_ADCCH9_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH9_GPIOX)
#define HTCFG_ADCCH10_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH10_GPIOX)
#define HTCFG_ADCCH11_GPIO_ID                      STRCAT2(GPIO_P,   HTCFG_ADCCH11_GPIOX)

#define HTCFG_ADCCH0_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH0_AFION)
#define HTCFG_ADCCH1_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH1_AFION)
#define HTCFG_ADCCH2_AFIO_PIN                    	 STRCAT2(AFIO_PIN_, HTCFG_ADCCH2_AFION)
#define HTCFG_ADCCH3_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH3_AFION)
#define HTCFG_ADCCH4_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH4_AFION)
#define HTCFG_ADCCH5_AFIO_PIN                    	 STRCAT2(AFIO_PIN_, HTCFG_ADCCH5_AFION)
#define HTCFG_ADCCH6_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH6_AFION)
#define HTCFG_ADCCH7_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH7_AFION)
#define HTCFG_ADCCH8_AFIO_PIN                     	STRCAT2(AFIO_PIN_, HTCFG_ADCCH8_AFION)
#define HTCFG_ADCCH9_AFIO_PIN                    	 STRCAT2(AFIO_PIN_, HTCFG_ADCCH9_AFION)
#define HTCFG_ADCCH10_AFIO_PIN              		STRCAT2(AFIO_PIN_, HTCFG_ADCCH10_AFION)
#define HTCFG_ADCCH11_AFIO_PIN                  	STRCAT2(AFIO_PIN_, HTCFG_ADCCH11_AFION)


#define HTCFG_INPUT_CAPTURE_GPTM_GPIO_ID                		(GPIO_PA)
#define HTCFG_INPUT_CAPTURE_GPTM_AFIO_PIN               		(AFIO_PIN_1)
#define HTCFG_INPUT_CAPTURE_GPTM_CHANNEL                	(TM_CH_1)
#define HTCFG_INPUT_CAPTURE_GPTM_IRQn                   		(GPTM0_IRQn)
#define HTCFG_INPUT_CAPTURE_GPTM                        			(HT_GPTM0)
#define HTCFG_INPUT_CAPTURE_GPTM_AFIO_MODE              	(AFIO_FUN_MCTM_GPTM)
#define HTCFG_INPUT_CAPTURE_GPTM_CLK(CK)                		(CK.Bit.GPTM0)
#define HTCFG_INPUT_CAPTURE_GPTM_CCR                    		(TM_INT_CH1CC)
#define HTCFG_INPUT_CAPTURE_PRESCALER                   		5000

#define HTCFG_OUTPUT_PWM_BY_SCTM_GPIO_ID                		(GPIO_PA)
#define HTCFG_OUTPUT_PWM_BY_SCTM_AFIO_PIN               	(AFIO_PIN_8)
#define HTCFG_OUTPUT_PWM_BY_SCTM                        			(HT_PWM1)
#define HTCFG_OUTPUT_PWM_BY_AFIO_MODE                   		(AFIO_FUN_PWM)
#define HTCFG_OUTPUT_PWM_BY_SCTM_CHANNEL                	(TM_CH_3)
#define HTCFG_OUTPUT_PWM_BY_SCTM_CLK(CK)                		(CK.Bit.PWM1)


#ifdef __cplusplus
}
#endif

#endif
