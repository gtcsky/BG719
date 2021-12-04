/*
 * user_exti.c
 *
 *  Created on: 2020年3月19日
 *      Author: Bough_004
 */

#include	"user_exti.h"
#include	"io_define.h"
#include	"user_menu.h"


void GPIO_EXTILineConfig(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef * pGPIO_InitStruct,EXTI_InitTypeDef * pEXTI_InitStruct)
{
//  u32 bGpioId  = BUTTON_GPIO_ID[BUTTON_x];
//  u32 uGpioPin = BUTTON_GPIO_PIN[BUTTON_x];
//  HT_GPIO_TypeDef* GPIOx = GPIO_PORT[bGpioId];

  /* Enable the GPIO Clock                                                                                  */
  /* Enable the AFIO Clock                                                                                  */
//  _HT32F_DVB_ClockConfig(bGpioId);
    CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};

  /* Configure Button pin as input pull high                                                      */
//  	  AFIO_GPxConfig(bGpioId, uGpioPin, AFIO_FUN_GPIO);
//  	  GPIO_PullResistorConfig(GPIOx, uGpioPin, GPIO_PR_UP);
//  	  GPIO_DirectionConfig(GPIOx, uGpioPin, GPIO_DIR_IN);
//  	  GPIO_InputConfig(GPIOx, uGpioPin, ENABLE);
	GPIO_Init(GPIOx,pGPIO_InitStruct);
 //   EXTI_InitTypeDef EXTI_InitStruct = {0};

    /* Enable the EXTI Clock                                                                                */
    CKCUClock.Bit.EXTI       = 1;
    CKCU_PeripClockConfig(CKCUClock, ENABLE);

    /* Connect Button EXTI Channel to Button GPIO Pin                                                       */
    AFIO_EXTISourceConfig((AFIO_EXTI_CH_Enum)pEXTI_InitStruct->EXTI_Channel,(AFIO_ESS_Enum)pGPIO_InitStruct->GPIO_Af_Port_ID);

    /* Configure button EXTI Line on falling edge                                                           */
//    pEXTI_InitStruct->EXTI_Channel = BUTTON_EXTI_CH[BUTTON_x];
//    pEXTI_InitStruct->EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
//    pEXTI_InitStruct->EXTI_DebounceCnt = 0;
//    pEXTI_InitStruct->EXTI_IntType = (BUTTON_x == BUTTON_WAKEUP) ? EXTI_POSITIVE_EDGE : EXTI_NEGATIVE_EDGE;
    EXTI_Init(pEXTI_InitStruct);

    /* Enable Button EXTI Interrupt                                                                         */
    EXTI_IntConfig(pEXTI_InitStruct->EXTI_Channel, ENABLE);
    switch (pEXTI_InitStruct->EXTI_Channel)
    {
      case 0:
      case 1:
        NVIC_EnableIRQ(EXTI0_1_IRQn);
        break;
      case 2:
      case 3:
        NVIC_EnableIRQ(EXTI2_3_IRQn);
        break;
      default:
        NVIC_EnableIRQ(EXTI4_15_IRQn);
        break;
    }
}

void EXTI_Key_Config(void) {
//	GPI0_IRQ_InitTypeDef GPI0_IRQ_InitStruct;
	GPIO_InitTypeDef GPIO_InitStruct;
	EXTI_InitTypeDef EXTI_InitStruct = { 0 };

	GPIO_InitStruct.GPIO_Mode = GPIO_DIR_IN;							//input 模式
	GPIO_InitStruct.GPIO_Pin = PowerKeyPin;								//Pin
	GPIO_InitStruct.GPIO_Af_Port_ID = PowerKeyPortID;						//AF设定时的 Port ID
	GPIO_InitStruct.GPIO_PuPd=GPIO_PR_UP;
//	GPIO_Init(userKey3Port, &GPIO_InitStruct);								//GPIO Port

	EXTI_InitStruct.EXTI_Channel = PowerKeyExtiChl;
	EXTI_InitStruct.EXTI_Debounce = EXTI_DEBOUNCE_DISABLE;
	EXTI_InitStruct.EXTI_DebounceCnt = 0;
	EXTI_InitStruct.EXTI_IntType = EXTI_NEGATIVE_EDGE;
	GPIO_EXTILineConfig(PowerKeyPort,&GPIO_InitStruct,&EXTI_InitStruct);

	GPIO_InitStruct.GPIO_Pin = SettingKeyPin;								//Pin
	GPIO_InitStruct.GPIO_Af_Port_ID = SettingKeyPortID;						//AF设定时的 Port ID
	EXTI_InitStruct.EXTI_Channel = SettingKeyExtiChl;
	GPIO_EXTILineConfig(SettingKeyPort,&GPIO_InitStruct,&EXTI_InitStruct);

	GPIO_InitStruct.GPIO_Pin = KeyIncPin;									//Pin
	GPIO_InitStruct.GPIO_Af_Port_ID = KeyIncPortID;						//AF设定时的 Port ID
	EXTI_InitStruct.EXTI_Channel = KeyIncExtiChl;
	GPIO_EXTILineConfig(KeyIncPort,&GPIO_InitStruct,&EXTI_InitStruct);

	GPIO_InitStruct.GPIO_Pin = DecKeyPin;									//Pin
	GPIO_InitStruct.GPIO_Af_Port_ID = DecKeyPortID;						//AF设定时的 Port ID
	EXTI_InitStruct.EXTI_Channel = DecKeyExtiChl;
	GPIO_EXTILineConfig(DecKeyPort,&GPIO_InitStruct,&EXTI_InitStruct);

}
/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI0_1_IRQHandler(void) {
	entryNormalModeChk();
	if (EXTI_GetEdgeFlag(EXTI_CHANNEL_0)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_0);
//		EXTI_ClearWakeupFlag(0);
	} else {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_1);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI2_3_IRQHandler(void) {
	entryNormalModeChk();
	if (EXTI_GetEdgeFlag(EXTI_CHANNEL_2)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_2);
//		EXTI_ClearWakeupFlag(2);
	} else {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_3);
	}
}

/*********************************************************************************************************//**
 * @brief   This function handles EXTI interrupt.
 * @retval  None
 ************************************************************************************************************/
void EXTI4_15_IRQHandler(void) {
	entryNormalModeChk();
	// WAKEUP_Button_Process(); // Fixed in PB12
	if (EXTI_GetEdgeFlag(EXTI_CHANNEL_4)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_4);
//		EXTI_ClearWakeupFlag(4);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_5)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_5);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_6)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_6);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_7)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_7);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_8)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_8);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_9)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_9);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_10)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_10);
		fIsSigmaDataReady=TRUE;
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_11)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_11);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_12)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_12);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_13)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_13);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_14)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_14);
	} else if (EXTI_GetEdgeFlag(EXTI_CHANNEL_15)) {
		EXTI_ClearEdgeFlag(EXTI_CHANNEL_15);
	}
}
