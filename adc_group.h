/*********************************************************************************************************//**
 * @file    adc_group.h
 * @version $Rev:: 4033         $
 * @date    $Date:: 2019-07-12 #$
 * @brief   The header file of ADC Group.
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
#ifndef __ADC_GROUP_H
#define __ADC_GROUP_H

#include	"ht32f5xxxx_01.h"
#ifdef __cplusplus
 extern "C" {
#endif



// You shall change the Trigger source to meet your condition, and modify the TM_Configuration()
#define ADC_TRIGGER_TM_SOURCE         (ADC_TRIG_GPTM0_CH0O)

/* Exported constants --------------------------------------------------------------------------------------*/
#define ADC_GROUP_TRIGGER_MODE_SW     (0)
#define ADC_GROUP_TRIGGER_MODE_TM     (1)
#define ADC_GROUP_DATA_MODE_CPU       (0)
#define ADC_GROUP_DATA_MODE_DMA       (1)

#define ADC_GROUP_FINISH              (2)

/* Exported macro ------------------------------------------------------------------------------------------*/
#if !defined(LIBCFG_PDMA) && (ADC_GROUP_DATA_MODE == 1)
  #error "The device you selected did not support PDMA!"
#endif

/* Exported functions --------------------------------------------------------------------------------------*/
void ADC_GroupProcess(void);

#ifdef __cplusplus
}
#endif

#endif
