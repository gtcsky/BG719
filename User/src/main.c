/*********************************************************************************************************//**
 * @file    ADC/Two_Group_MaxCH/main.c
 * @version $Rev:: 4192         $
 * @date    $Date:: 2019-08-13 #$
 * @brief   Main program.
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
// <<< Use Configuration Wizard in Context Menu >>>

/* Includes ------------------------------------------------------------------------------------------------*/

#include "adc_group.h"
#include "user_adc.h"
#include "user_menu.h"
#include "user_lptimer.h"
#include "user_basetime.h"
#include	"user_exti.h"
#include	"user_key.h"
#include "user_advtime.h"
#include	"user_i2c.h"
#include	"user_lcd.h"
#include "user_datastore.h"
/** @addtogroup HT32_Series_Peripheral_Examples HT32 Peripheral Examples
  * @{
  */

/** @addtogroup ADC_Examples ADC
  * @{
  */

/** @addtogroup Two_Group_MaxCH
  * @{
  */
extern	u16 vSystem5ms;
//extern	u8	vKeyValue;


/* Private function prototypes -----------------------------------------------------------------------------*/
//void PDMA_Configuration(void);

void NVIC_Configuration(void);
void CKCU_Configuration(void);
void SysTick_Configuration(void);

/* Global variables ----------------------------------------------------------------------------------------*/


/* Global functions ----------------------------------------------------------------------------------------*/
/***********************************************************************************************************
  * @brief  Main program.
  * @retval None
  ***********************************************************************************************************/
int main(void)
 {
	u16 usedAdcChl = 0;
	NVIC_Configuration(); /* NVIC configuration                                                 				*/
	// CKCU_Configuration();               /* System Related configuration                                      	 */
	HT_CKCU->AHBCCR = 	0xFFFFFFFF;
	HT_CKCU->APBCCR0 = 0xFFFFFFFF;
	HT_CKCU->APBCCR1 = 0xFFFFFFFF;
	SysTick_Configuration(); /* SysTick configuration                                              			*/
	powerOnIoInit();
	LPTIMER_Config();
	BASIC_TIM_Mode_Config();
	EXTI_Key_Config();
	usedAdcChl = ADC_VCAP_USED_CH;
	ADC_Configuration(usedAdcChl); /* ADC configuration                                                  		*/
	user_I2C_EEPROM_Init(HT_I2C0);
	FlashInit();
	sigmaAdTest();
	userPwmParaStruct.period = ADVICE_TIM_PERIOD; //ADVICE_TIM_PERIOD;
	userPwmParaStruct.channels = BUZZER_CHANNEL;
	ADVTIM_PWMMODE_Config(&userPwmParaStruct, BUZZER_PWM_TIMER);
//  	  ADC_TriggerConfig_Software();
//	fIsSensorTestMode=TRUE;
//	fIsSensorTestMode=FALSE;
	powerOnLcdInit();
	while (1) {
		checkSigmaAdConvertOver(&sigmaAdParas);
		sigmaResultExecute(&sigmaAdParas);
		funcKeyTask();
		if (vKeyValue) {
			MenuFunction();
		}
//   		ADC_Test_MainRoutine();
//		uartReceivedDataProcess();
//		uartSendDataProcess();
		if (vSystem5ms != 0) {
			vSystem5ms--;
			fucnPer5ms();
			//iCNTVAL=IWDG_GetReload();
//			IWDG_GetReload();
//			IWDG_Feed();
//			IWDG_RELOAD();
		}
	}
}

/***********************************************************************************************************
  * @brief  Configure the NVIC vector table.
  * @retval None
  ***********************************************************************************************************/
void NVIC_Configuration(void)
 {
	NVIC_SetVectorTable(NVIC_VECTTABLE_FLASH, 0x0); /* Set the Vector Table base location at 0x00000000   */
}

/***********************************************************************************************************
  * @brief  Configure the system clocks.
  * @retval None
  ***********************************************************************************************************/
void CKCU_Configuration(void)
 {
	CKCU_PeripClockConfig_TypeDef CKCUClock = { { 0 } };
	CKCUClock.Bit.PDMA = 0;
	CKCUClock.Bit.USBD = 0;
	CKCUClock.Bit.CKREF = 0;
	CKCUClock.Bit.EBI = 0;
	CKCUClock.Bit.CRC = 0;
	CKCUClock.Bit.PA = 0;
	CKCUClock.Bit.PB = 0;
	CKCUClock.Bit.PC = 0;
	CKCUClock.Bit.PD = 0;
	CKCUClock.Bit.I2C0 = 0;
	CKCUClock.Bit.I2C1 = 0;
	CKCUClock.Bit.SPI0 = 0;
	CKCUClock.Bit.SPI1 = 0;
	CKCUClock.Bit.USART0 = 1;
	CKCUClock.Bit.USART1 = 1;
	CKCUClock.Bit.UART0 = 1;
	CKCUClock.Bit.UART1 = 1;
	CKCUClock.Bit.AFIO = 0;
	CKCUClock.Bit.EXTI = 0;
	CKCUClock.Bit.SCI0 = 0;
	CKCUClock.Bit.SCI1 = 0;
	CKCUClock.Bit.I2S = 0;
	CKCUClock.Bit.MCTM0 = 0;
	CKCUClock.Bit.WDT = 0;
	CKCUClock.Bit.BKP = 0;
	CKCUClock.Bit.GPTM0 = 0;
	CKCUClock.Bit.GPTM1 = 0;
	CKCUClock.Bit.BFTM0 = 0;
	CKCUClock.Bit.BFTM1 = 0;
	CKCUClock.Bit.CMP = 0;
	CKCUClock.Bit.ADC = 0;
	CKCUClock.Bit.SCTM0 = 0;
	CKCUClock.Bit.SCTM1 = 0;
	CKCUClock.Bit.SCTM2 = 0;
	CKCUClock.Bit.SCTM3 = 0;
	CKCU_PeripClockConfig(CKCUClock, ENABLE);
}

/***********************************************************************************************************
  * @brief  SysTick Configuration.
  * @retval R1DESCRIPTION
  ***********************************************************************************************************/
void SysTick_Configuration(void)
{
  SYSTICK_ClockSourceConfig(SYSTICK_SRC_STCLK);         			/* Default : CK_SYS/8                               	*/
  SYSTICK_SetReloadValue(SystemCoreClock / 8 / 2);      				/* (CK_SYS/8/2) = 500ms                             	*/
  SYSTICK_IntConfig(ENABLE);                           						 /* Enable SYSTICK Interrupt                         */
  SYSTICK_CounterCmd(SYSTICK_COUNTER_CLEAR);
  SYSTICK_CounterCmd(SYSTICK_COUNTER_ENABLE);
}

#if (HT32_LIB_DEBUG == 1)
/***********************************************************************************************************
  * @brief  Report both the error name of the source file and the source line number.
  * @param  filename: pointer to the source file name.
  * @param  uline: error line source number.
  * @retval None
  ***********************************************************************************************************/
void assert_error(u8* filename, u32 uline)
{
  /*
     This function is called by IP library that the invalid parameters has been passed to the library API.
     Debug message can be added here.
     Example: printf("Parameter Error: file %s on line %d\r\n", filename, uline);
  */

  while (1)
  {
  }
}
#endif
/* Private functions ---------------------------------------------------------------------------------------*/
