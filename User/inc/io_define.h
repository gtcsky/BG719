/*
 * io_define.h
 *
 *  Created on: 2020年3月16日
 *      Author: Bough_004
 */

#ifndef USER_INC_IO_DEFINE_H_
#define USER_INC_IO_DEFINE_H_

#include "const.h"

#ifdef __cplusplus
 extern "C" {
#endif
#define HTCFG_OUTPUT_LED0_ID                                  		(GPIO_PB)
#define HTCFG_OUTPUT_LED1_ID                                  		(GPIO_PB)
#define HTCFG_OUTPUT_LED2_ID                                  		(GPIO_PB)
#define HTCFG_INPUT_WAKE_ID                                   		(GPIO_PB)
#define HTCFG_INPUT_SettingKey_ID                                   	(GPIO_PC)
#define HTCFG_INPUT_KEY2_ID                                   		(GPIO_PC)

#define HTCFG_OUTPUT_LED0_CLK(CK)                             	(CK.Bit.PB)
#define HTCFG_OUTPUT_LED1_CLK(CK)                             	(CK.Bit.PB)
#define HTCFG_OUTPUT_LED2_CLK(CK)                             	(CK.Bit.PB)
#define HTCFG_INPUT_WAKE_CLK(CK)                             	(CK.Bit.PB)
#define HTCFG_INPUT_SettingKey_CLK(CK)                            (CK.Bit.PC)
#define HTCFG_INPUT_KEY2_CLK(CK)                              		(CK.Bit.PC)

#define HTCFG_LED0                                            			(HT_GPIOB)
#define HTCFG_LED1                                            			(HT_GPIOB)
#define HTCFG_LED2                                            			(HT_GPIOB)
#define HTCFG_WAKE                                            			(HT_GPIOB)
#define HTCFG_SettingKey                                            		(HT_GPIOC)
#define HTCFG_KEY2                                            			(HT_GPIOC)

#define HTCFG_OUTPUT_LED0_AFIO_PIN                            	(AFIO_PIN_6)
#define HTCFG_OUTPUT_LED1_AFIO_PIN                            	(AFIO_PIN_7)
#define HTCFG_OUTPUT_LED2_AFIO_PIN                            	(AFIO_PIN_8)
#define HTCFG_INPUT_WAKE_AFIO_PIN                             	(AFIO_PIN_12)
#define HTCFG_INPUT_SettingKey_AFIO_PIN                          (AFIO_PIN_2)
#define HTCFG_INPUT_KEY2_AFIO_PIN                             	(AFIO_PIN_3)

#define HTCFG_OUTPUT_LED0_GPIO_PIN                            	(GPIO_PIN_6)
#define HTCFG_OUTPUT_LED1_GPIO_PIN                           	(GPIO_PIN_7)
#define HTCFG_OUTPUT_LED2_GPIO_PIN                            	(GPIO_PIN_8)
#define HTCFG_INPUT_WAKE_GPIO_PIN                             	(GPIO_PIN_12)
#define HTCFG_INPUT_SettingKey_GPIO_PIN                          (GPIO_PIN_2)
#define HTCFG_INPUT_KEY2_GPIO_PIN                             	(GPIO_PIN_3)



#define	EnableGPIOAClock(CK) 						(CK.Bit.PA)
#define	EnableGPIOBClock(CK) 						(CK.Bit.PB)
#define	EnableGPIOCClock(CK) 						(CK.Bit.PC)

#define 	SettingKeyPortID                                  			(GPIO_PA)
#define 	SettingKeyPort                                  				(HT_GPIOA)
#define 	SettingKeyPin                                  				(GPIO_PIN_11)
#define 	SettingKey_CLK(CK)                                    			(CK.Bit.PA)
#define	SettingKeyExtiChl								(AFIO_EXTI_CH_11)
#define	set_SettingKey								GPIO_WriteOutBits(SettingKeyPort, SettingKeyPin, SET)
#define	clr_SettingKey								GPIO_WriteOutBits(SettingKeyPort, SettingKeyPin, RESET;
#define	read_SettingKeyStts							GPIO_ReadInBit(SettingKeyPort, SettingKeyPin)
#define 	Setting_KEY_PRESSED   						0x04
#define	checkSettingKeyPressed						(vIsKeyPressed&Setting_KEY_PRESSED)
#define	clrSettingKeyPressed							vIsKeyPressed&=~Setting_KEY_PRESSED//0xEF
#define	setSettingKeyPressed							vIsKeyPressed|=Setting_KEY_PRESSED
#define	GET_SETTING_KEY_PIN_STTS					GPIO_ReadInBit(SettingKeyPort, SettingKeyPin)


#define 	KeyIncPortID                                  				(GPIO_PA)
#define 	KeyIncPort                                  					(HT_GPIOA)
#define 	KeyIncPin                                  					(GPIO_PIN_10)
#define 	KeyInc_CLK(CK)                                    				(CK.Bit.PA)
#define	KeyIncExtiChl									(AFIO_EXTI_CH_10)
#define	set_KeyInc									GPIO_WriteOutBits(KeyIncPort, KeyIncPin, SET)
#define	clr_KeyInc									GPIO_WriteOutBits(KeyIncPort, KeyIncPin, RESET)
#define 	Inc_KEY_PRESSED   							0x20
#define	checkIncKeyPressed							(vIsKeyPressed&Inc_KEY_PRESSED)
#define	clrIncKeyPressed								vIsKeyPressed&=~Inc_KEY_PRESSED//0xFB
#define	setIncKeyPressed								vIsKeyPressed|=Inc_KEY_PRESSED
#define	GET_INC_KEY_PIN_STTS						GPIO_ReadInBit(KeyIncPort, KeyIncPin)
#define	read_KeyIncStts								GPIO_ReadInBit(KeyIncPort, KeyIncPin)

#define 	DecKeyPortID                                  				(GPIO_PA)
#define 	DecKeyPort                                  					(HT_GPIOA)
#define 	DecKeyPin                                  					(GPIO_PIN_9)
#define 	DecKey_CLK(CK)                                    			(CK.Bit.PA)
#define	DecKeyExtiChl								(AFIO_EXTI_CH_9)
#define	set_DecKey									GPIO_WriteOutBits(DecKeyPort, DecKeyPin, SET)
#define	clr_DecKey									GPIO_WriteOutBits(DecKeyPort, DecKeyPin, RESET)
#define	read_DecKeyStts								GPIO_ReadInBit(DecKeyPort, DecKeyPin)
#define 	Dec_KEY_PRESSED   							0x10
#define	checkDecKeyPressed							(vIsKeyPressed&Dec_KEY_PRESSED)
#define	clrDecKeyPressed								vIsKeyPressed&=~Dec_KEY_PRESSED//0xF7
#define	setDecKeyPressed								vIsKeyPressed|=Dec_KEY_PRESSED
#define	GET_DEC_KEY_PIN_STTS						GPIO_ReadInBit(DecKeyPort, DecKeyPin)

#define 	PowerKeyPortID                                  				(GPIO_PA)
#define 	PowerKeyPort                                  				(HT_GPIOA)
#define 	PowerKeyPin                                  				(GPIO_PIN_8)
#define 	PowerKey_CLK(CK)                                    			(CK.Bit.PA)
#define	PowerKeyExtiChl								(AFIO_EXTI_CH_8)
#define	set_PowerKey								GPIO_WriteOutBits(PowerKeyPort, PowerKeyPin, SET)
#define	clr_PowerKey									GPIO_WriteOutBits(PowerKeyPort, PowerKeyPin, RESET)
#define 	Power_KEY_PRESSED   							0x01
#define	checkPowerKeyPressed						(vIsKeyPressed&Power_KEY_PRESSED)
#define	clrPowerKeyPressed							vIsKeyPressed&=~Power_KEY_PRESSED//0xFD
#define	setPowerKeyPressed							vIsKeyPressed|=Power_KEY_PRESSED
#define	GET_POWER_KEY_PIN_STTS					GPIO_ReadInBit(PowerKeyPort, PowerKeyPin)

#define 	RedLedPortID                                  				(GPIO_PB)
#define 	RedLedPort                                 					(HT_GPIOB)
#define 	RedLedPin                                  					(GPIO_PIN_15)
#define 	RedLedCLK(CK)                                    				(CK.Bit.PB)
#define	setRedLed									GPIO_WriteOutBits(RedLedPort, RedLedPin, SET)
#define	clrRedLed									GPIO_WriteOutBits(RedLedPort, RedLedPin, RESET)
//#define 	RedLedPortID                                  				(GPIO_PA)
//#define 	RedLedPort                                 					(HT_GPIOA)
//#define 	RedLedPin                                  					(GPIO_PIN_2)
//#define 	RedLedCLK(CK)                                    				(CK.Bit.PA)
//#define	setRedLed									GPIO_WriteOutBits(RedLedPort, RedLedPin, SET)
//#define	clrRedLed									GPIO_WriteOutBits(RedLedPort, RedLedPin, RESET)

//#define 	GreenLedPortID                                  				(GPIO_PA)
//#define 	GreenLedPort                                 				(HT_GPIOA)
//#define 	GreenLedPin                                  				(GPIO_PIN_3)
//#define 	GreenLedCLK(CK)                                    			(CK.Bit.PA)
//#define	setGreenLed									GPIO_WriteOutBits(GreenLedPort, GreenLedPin, SET)
//#define	clrGreenLed									GPIO_WriteOutBits(GreenLedPort, GreenLedPin, RESET)
#define 	GreenLedPortID                                  				(GPIO_PB)
#define 	GreenLedPort                                 				(HT_GPIOB)
#define 	GreenLedPin                                  				(GPIO_PIN_12)
#define 	GreenLedCLK(CK)                                    			(CK.Bit.PB)
#define	setGreenLed									GPIO_WriteOutBits(GreenLedPort, GreenLedPin, SET)
#define	clrGreenLed									GPIO_WriteOutBits(GreenLedPort, GreenLedPin, RESET)


#define 	LcdResPortID                                  				(GPIO_PA)
#define 	LcdResPort                                 					(HT_GPIOA)
#define 	LcdResPin                                  					(GPIO_PIN_3)
#define 	LcdResCLK(CK)                                    				(CK.Bit.PA)
#define	set_LcdResPin								GPIO_WriteOutBits(LcdResPort, LcdResPin, SET)
#define	clr_LcdResPin									GPIO_WriteOutBits(LcdResPort, LcdResPin, RESET)

#define 	LcdDcPortID                                  				(GPIO_PA)
#define 	LcdDcPort                                 					(HT_GPIOA)
#define 	LcdDcPin                                  					(GPIO_PIN_6)
#define 	LcdDcCLK(CK)                                    				(CK.Bit.PA)
#define	set_LcdDcPin									GPIO_WriteOutBits(LcdDcPort, LcdDcPin, SET)
#define	clr_LcdDcPin									GPIO_WriteOutBits(LcdDcPort, LcdDcPin, RESET)

//-------------------------------------------------------
//define for PWM
#define	BUZZER_PortID							(GPIO_PC)
#define	BUZZER_Port								(HT_GPIOC)
#define	BUZZER_PortPin							(GPIO_PIN_5)
#define	BUZZER_CHANNEL						userPWM_CH1
#define	BUZZER_PWM_TIMER						(HT_PWM1)


//-------------------------------------------------------
//define for ADC
#define	ADC_AGND_CHL							ADC_CH_GND_VREF
#define	ADC_AGND_GROUP_INDEX				0					//index of ADC group convert result

#define	ADC_AVDD_CHL							ADC_CH_VDD_VREF
#define	ADC_AVDD_GROUP_INDEX					1					//index of ADC group convert result

 #define	ADC_VCAP_CHL							ADC_CH_10
#define	ADC_VCAP_USED_CH						UserChannel10
#define	ADC_VCAP_GROUP_INDEX					2					//index of ADC group convert result
#define	ADC_VCAP_PortID						HTCFG_ADCCH10_GPIO_ID
#define	ADC_VCAP_Pin							HTCFG_ADCCH10_AFIO_PIN


 //-------------------------------------------------------


 typedef struct
 {
   uint16_t GPIO_Pin;              /*!< Specifies the GPIO pins to be configured.
                                        This parameter can be any value of @ref GPIO_pins_define */

   GPIO_DIR_Enum GPIO_Mode;     /*!< Specifies the operating mode for the selected pins.
                                        This parameter can be a value of @ref GPIOMode_TypeDef */

   GPIO_DV_Enum GPIO_OType;   /*!< Specifies the operating output type for the selected pins.
                                        This parameter can be a value of @ref GPIOOType_TypeDef */

   GPIO_PR_Enum GPIO_PuPd;     /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                        This parameter can be a value of @ref GPIOPuPd_TypeDef */

   u32 GPIO_Af_Port_ID;			/* 	GPIOA=0,GPIOB=1,GPIOC=2.	*/
 }GPIO_InitTypeDef;

#ifdef __cplusplus
}
#endif
#endif /* USER_INC_IO_DEFINE_H_ */
