/*
 * user_menu.h
 *
 *  Created on: 2020年3月16日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_MENU_H_
#define USER_INC_USER_MENU_H_
#include	"const.h"
#include	"ht32f5xxxx_01.h"
#include "io_define.h"
#include "user_i2c.h"
extern	UserPwmInitTypeDef  userPwmParaStruct;
extern	u8	fIsSigmaDataReady;
extern	float vResAmbient;
extern	float vVoltOfObj;
extern	float vDacOut;
extern	float vVts;
extern	float vResCoe;
extern	double vVcm_coe;
extern	displayParamsStruct 	displayParams;
extern	displayParamsStruct   	*pDisplayParams;

extern	u8	fIsSensorTestMode;
extern	u8	vtTestArray[20];

extern	void MenuFunction(void );
extern	void fucnPer5ms(void );
extern	void resetEntrySleepTime(void);
extern	void entryNormalModeChk(void);
extern	void powerOnIoInit(void);
extern	void set_LED_Toggle( u32 GPIO_Af_Port_ID,uint16_t GPIO_Pin);
extern	void GPIO_Init(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef * GPIO_InitStruct) ;
extern	void	sigmaResultExecute(sigmaAdTypeDef * pAdParas);
extern	float calcTemperature(u16 u16TemperatureValue,displayUnitEnum unit);
extern	void irSensorDataProcess(u16 originalIrData);
#endif /* USER_INC_USER_MENU_H_ */
