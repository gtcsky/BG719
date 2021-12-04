/*
 * user_menu.c
 *
 *  Created on: 2020年3月16日
 *      Author: Bough_004
 */
#include "user_menu.h"
#include "user_lcd.h"
#include "user_lptimer.h"
#include "user_key.h"
#include "user_adc.h"
#include "user_advtime.h"
#include "user_tone.h"
#include "sensor_data.h"
#include "user_datastore.h"
#include "user_ddl.h"


u8		fIsSigmaDataReady=FALSE;
float 	vResAmbient=0;
float	vResCoe=1.0;				//		res of respect/res of real
u8		vTempAmbient=0;
float 	vVoltOfObj=0;
float 	vTempOfObj=0;
float 	vDacOut=0;
float 	vVts=0;

u16		vSystem1s;
u16 		vTimeToSleep;										//vTimeToSleep*50ms  timeout , entry sleep mode
bool 	fIsSystemOff=FALSE;									//	0/1  系统开启/关闭
float vSystemVdd=0;
u16 vtTempU16=0;
UserPwmInitTypeDef  	userPwmParaStruct={{0}};
float	vPWMCh3Duty=0;
double 	vVcm_coe=0.94839888;

displayParamsStruct 	displayParams;
displayParamsStruct  	 *pDisplayParams=&displayParams;

//u8	fIsSensorTestMode=FALSE;

u8 	vtTestArray[20]={0};
void GPIO_Init(HT_GPIO_TypeDef* GPIOx, GPIO_InitTypeDef * GPIO_InitStruct) {
	/* Configure AFIO mode of input/output  pins                                                                     	*/
	AFIO_GPxConfig(GPIO_InitStruct->GPIO_Af_Port_ID, GPIO_InitStruct->GPIO_Pin, AFIO_MODE_1);
	GPIO_PullResistorConfig(GPIOx, GPIO_InitStruct->GPIO_Pin, GPIO_InitStruct->GPIO_PuPd);
	/* Configure GPIO direction of input pins                                                                 			*/
	GPIO_DirectionConfig(GPIOx, GPIO_InitStruct->GPIO_Pin, GPIO_InitStruct->GPIO_Mode);
	if (GPIO_InitStruct->GPIO_Mode == GPIO_DIR_IN) {
		/* Configure GPIO pull resistor of input pins                                                             		*/
		GPIO_InputConfig(GPIOx, GPIO_InitStruct->GPIO_Pin, ENABLE);
	} else {
		//GPIO_PullResistorConfig(GPIOx, GPIO_InitStruct->GPIO_Pin, GPIO_PR_DISABLE);
		/* Configure GPIO output driving current                                                   				*/
		GPIO_DriveConfig(GPIOx, GPIO_InitStruct->GPIO_Pin, GPIO_InitStruct->GPIO_OType);
	}
}

void set_LED_Toggle(u32 GPIO_Af_Port_ID, uint16_t GPIO_Pin) {
	GPIO_PORT[GPIO_Af_Port_ID]->DOUTR ^= GPIO_Pin;
}

void setGreenLEDToggle(void) {
	set_LED_Toggle(GreenLedPortID, GreenLedPin);
}
void setRedLEDToggle(void) {
	set_LED_Toggle(RedLedPortID, RedLedPin);
}


void	powerOnIoInit(void){
	  	 GPIO_InitTypeDef	GPIO_InitStruct;

		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_IN;							//input 模式
//		GPIO_InitStruct.GPIO_OType = GPIO_DV_8MA;							//GPIOx_OTYPER   0/1 推挽输出/开漏输出
		GPIO_InitStruct.GPIO_PuPd = GPIO_PR_DISABLE;						//不上拉
		GPIO_InitStruct.GPIO_Pin = HTCFG_INPUT_WAKE_GPIO_PIN;				//
		GPIO_InitStruct.GPIO_Af_Port_ID = HTCFG_INPUT_WAKE_ID;				//
		GPIO_Init(HTCFG_WAKE, &GPIO_InitStruct);							//位置  GPIOD.4 GPIOD.5

		GPIO_InitStruct.GPIO_Pin = HTCFG_INPUT_SettingKey_GPIO_PIN;			//Pin
		GPIO_InitStruct.GPIO_Af_Port_ID = HTCFG_INPUT_SettingKey_ID;			//AF设定时的 Port ID
		GPIO_Init(HTCFG_SettingKey, &GPIO_InitStruct);						//GPIO Port

		GPIO_InitStruct.GPIO_Pin = HTCFG_INPUT_KEY2_GPIO_PIN;				//Pin
		GPIO_InitStruct.GPIO_Af_Port_ID = HTCFG_INPUT_KEY2_ID;				//AF设定时的 Port ID
		GPIO_Init(HTCFG_KEY2, &GPIO_InitStruct);								//GPIO Port

		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//output 模式
		GPIO_InitStruct.GPIO_Pin = GreenLedPin;								//Pin
		GPIO_InitStruct.GPIO_Af_Port_ID = GreenLedPortID;						//AF设定时的 Port ID
		GPIO_Init(GreenLedPort, &GPIO_InitStruct);							//GPIO Port
		setGreenLed;

		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//output 模式
		GPIO_InitStruct.GPIO_Pin = RedLedPin;								//Pin
		GPIO_InitStruct.GPIO_Af_Port_ID = RedLedPortID;						//AF设定时的 Port ID
		GPIO_Init(RedLedPort, &GPIO_InitStruct);								//GPIO Port
		setRedLed;



//		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//input 模式
//		GPIO_InitStruct.GPIO_Pin = SettingKeyPin;								//Pin
//		GPIO_InitStruct.GPIO_Af_Port_ID = SettingKeyPortID;						//AF设定时的 Port ID
//		GPIO_Init(SettingKeyPort, &GPIO_InitStruct);							//GPIO Port
//		set_SettingKey;
//
//		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//input 模式
//		GPIO_InitStruct.GPIO_Pin = KeyIncPin;									//Pin
//		GPIO_InitStruct.GPIO_Af_Port_ID = KeyIncPortID;						//AF设定时的 Port ID
//		GPIO_Init(KeyIncPort, &GPIO_InitStruct);								//GPIO Port
//		set_KeyInc;
//
//		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//input 模式
//		GPIO_InitStruct.GPIO_Pin = DecKeyPin;								//Pin
//		GPIO_InitStruct.GPIO_Af_Port_ID = DecKeyPortID;						//AF设定时的 Port ID
//		GPIO_Init(DecKeyPort, &GPIO_InitStruct);								//GPIO Port
//		set_DecKey;
//
//		GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//input 模式
//		GPIO_InitStruct.GPIO_Pin = PowerKeyPin;								//Pin
//		GPIO_InitStruct.GPIO_Af_Port_ID = PowerKeyPortID;						//AF设定时的 Port ID
//		GPIO_Init(PowerKeyPort, &GPIO_InitStruct);							//GPIO Port
//		set_PowerKey;

//------------------------------------------------------------------------------------------------------------------------
	  //original code
//	  /* Configure WAKEUP, SettingKey, KEY2 pins as the input function                                                */
//
//	  /* Configure AFIO mode of input pins                                                                      			*/
//	  AFIO_GPxConfig(HTCFG_INPUT_WAKE_ID, HTCFG_INPUT_WAKE_AFIO_PIN, AFIO_FUN_GPIO);
//	  AFIO_GPxConfig(HTCFG_INPUT_SettingKey_ID, HTCFG_INPUT_SettingKey_AFIO_PIN, AFIO_FUN_GPIO);
//	  AFIO_GPxConfig(HTCFG_INPUT_KEY2_ID, HTCFG_INPUT_KEY2_AFIO_PIN, AFIO_FUN_GPIO);
//
//	  /* Configure GPIO direction of input pins                                                                 			*/
//	  GPIO_DirectionConfig(HTCFG_WAKE, HTCFG_INPUT_WAKE_GPIO_PIN, GPIO_DIR_IN);
//	  GPIO_DirectionConfig(HTCFG_SettingKey, HTCFG_INPUT_SettingKey_GPIO_PIN, GPIO_DIR_IN);
//	  GPIO_DirectionConfig(HTCFG_KEY2, HTCFG_INPUT_KEY2_GPIO_PIN, GPIO_DIR_IN);
//
//	  /* Configure GPIO pull resistor of input pins                                                             		*/
//	  GPIO_PullResistorConfig(HTCFG_WAKE, HTCFG_INPUT_WAKE_GPIO_PIN, GPIO_PR_DISABLE);
//	  GPIO_PullResistorConfig(HTCFG_SettingKey, HTCFG_INPUT_SettingKey_GPIO_PIN, GPIO_PR_DISABLE);
//	  GPIO_PullResistorConfig(HTCFG_KEY2, HTCFG_INPUT_KEY2_GPIO_PIN, GPIO_PR_DISABLE);
//
//	  GPIO_InputConfig(HTCFG_WAKE, HTCFG_INPUT_WAKE_GPIO_PIN, ENABLE);
//	  GPIO_InputConfig(HTCFG_SettingKey, HTCFG_INPUT_SettingKey_GPIO_PIN, ENABLE);
//	  GPIO_InputConfig(HTCFG_KEY2, HTCFG_INPUT_KEY2_GPIO_PIN, ENABLE);
//
//	  while (1)
//	  {
//	    /* Read WEAKUP and then output to LED1                                                                  */
//	    TmpStatus = GPIO_ReadInBit(HTCFG_WAKE, HTCFG_INPUT_WAKE_GPIO_PIN);
//	    GPIO_WriteOutBits(HTCFG_LED0, HTCFG_OUTPUT_LED0_GPIO_PIN, TmpStatus);
//
//	    /* Read SettingKey and then output to LED2                                                                    */
//	    TmpStatus = GPIO_ReadInBit(HTCFG_SettingKey, HTCFG_INPUT_SettingKey_GPIO_PIN);
//	    GPIO_WriteOutBits(HTCFG_LED1, HTCFG_OUTPUT_LED1_GPIO_PIN, TmpStatus);
//
//	    /* Read KEY2 and then output to LED3                                                                    */
//	    TmpStatus = GPIO_ReadInBit(HTCFG_KEY2, HTCFG_INPUT_KEY2_GPIO_PIN);
//	    GPIO_WriteOutBits(HTCFG_LED2, HTCFG_OUTPUT_LED2_GPIO_PIN, TmpStatus);
//	  }
//------------------------------------------------------------------------------------------------------------------------
}

void resetEntrySleepTime(void){
		if(fIsSystemOff)
			vTimeToSleep=POWER_DOWN_MODE_TIMEOUT;							//(5+1)*50ms=300ms
		else
			vTimeToSleep=NORMAL_MODE_TIMEOUT;								//60s
}
void entryNormalModeChk(void){
	  	RTC_Cmd(ENABLE);								//醒来后重启LPT定时器
		resetEntrySleepTime();								//重置进入睡眠时间
}


/**************************************************************
 *
 * function executed  per 50ms
 *
 ***************************************************************/
void fucnPer50ms(void) {

}
/**************************************************************
 *
 * function executed  per 100ms
 *
 ***************************************************************/
void fucnPer100ms(void) {
	holdingKeyFunc();
}
/**************************************************************
 *
 * function executed  per 500ms
 *
 ***************************************************************/
void fucnPer500ms(void) {

	//set_SettingKey_LEDToggle();
}
/**************************************************************
 *
 * function executed  per 1s
 *
 ***************************************************************/
void fucnPer1s(void) {

}
/**********************************************************************************
 *
 *  ADC结果处理
 *
 ***********************************************************************************/
float adcResultProcess(void) {
	u16	vDaValue=0;
	if(uADCGroupFinish){
		vDaValue=uADCData[ADC_VCAP_GROUP_INDEX];
		vSystemVdd=(SYSTEM_VCAP*ADC_CONST/vDaValue);
		if(vSystemVdd==0)
			return	0;
		uADCGroupFinish=0;
		//
		return	vSystemVdd;
	}
	return	0;
}


void	convertAmbientTemperature(void){
	sigmaAdParas.in2N=SIGMA_IN2N_VCM;
	sigmaAdParas.in2P=SIGMA_IN2P_AN0;
	sigmaAdParas.PgGain=SIGMA_PGAGN_1;
	sigmaAdParas.AdGain=SIGMA_ADGN_1;
	sigmaAdParas.VrefGain=SIGMA_VREFGN_DIV1;
	sigmaAdParas.totalGain=1;
	sigmaAdParas.convertOverFlag=NonConverting;
	updateAdChlAndStart(&sigmaAdParas,An0ChCoverting);
}
void	convertMcuTemperature(void){
	sigmaAdParas.in2N=SIGMA_IN2N_VTSN;
	sigmaAdParas.in2P=SIGMA_IN2P_VTSP;
	sigmaAdParas.PgGain=SIGMA_PGAGN_8;
	sigmaAdParas.AdGain=SIGMA_ADGN_1;
	sigmaAdParas.VrefGain=SIGMA_VREFGN_DIV1;
	sigmaAdParas.totalGain=8;
	sigmaAdParas.convertOverFlag=NonConverting;
	updateAdChlAndStart(&sigmaAdParas,VTSConverting);
}

void	convertObjectTemperature(void){
//	sigmaAdParas.in2N=SIGMA_IN2N_VCM;
	sigmaAdParas.in2N=SIGMA_IN2N_AN3;
	sigmaAdParas.in2P=SIGMA_IN2P_AN2;
//	sigmaAdParas.in2P=SIGMA_IN2P_VCM;
	sigmaAdParas.PgGain=SIGMA_PGAGN_64;
	sigmaAdParas.AdGain=SIGMA_ADGN_2;
	sigmaAdParas.VrefGain=SIGMA_VREFGN_DIV1;
	sigmaAdParas.totalGain=64*2*1;
	sigmaAdParas.convertOverFlag=NonConverting;
	updateAdChlAndStart(&sigmaAdParas,An23ChConverting);
}


void	convertDacOutput(void){
	sigmaAdParas.in2N=SIGMA_IN2N_AVSS;
	sigmaAdParas.in2P=SIGMA_IN2P_VDACO;
	sigmaAdParas.convertOverFlag=NonConverting;
	updateAdChlAndStart(&sigmaAdParas,VdacoChConverting);
}

void fucnPer5ms(void ){
		adcResultProcess();
		voiceFunc();
		if(fTime50ms==1){
					fTime50ms=0;
					fucnPer50ms();
		}
		if(fTime100ms==1){
					fTime100ms=0;
					fucnPer100ms();
		}
		if(fTime500ms==1){
					fTime500ms=0;
					fucnPer500ms();
		}
		if(fTime1s==1){
					fTime1s=0;
					vSystem1s+=1;
//					if(vSystem1s%2==0)
//					convertAmbientTemperature();
					fucnPer1s();
		}
}
/***********************************************
 *
 * 发热阈值获取
 *
 ************************************************/
float getHotThreshold(void){
	if(Celsius==displayParams.temperatureUnit)
		return	((float)(displayParams.hotThreshold+HOT_THRESH_CEL_START))/10;
	else
		return	((float)(displayParams.hotThreshold+HOT_THRESH_FAL_START))/10;
}
/***********************************************
 *
 * 单位设置处理函数
 *
 ************************************************/
void	temperatureUnitSettingFunc(void){
	u16   vtTemp16=0;
	float vtTemperature=0;
	displayParams.temperatureUnit^=Fahrenheit;
	if(Fahrenheit==displayParams.temperatureUnit){
		vtTemperature=((displayParams.hotThreshold+HOT_THRESH_CEL_START)*1.8+320);		//转成华氏
		vtTemp16=(u16)(vtTemperature+0.5);
		if(vtTemp16>HOT_THRESH_FAL_START+HOT_THRESHOLD_MAX_F)
			displayParams.hotThreshold=HOT_THRESHOLD_MAX_F;
		else if (vtTemp16<HOT_THRESH_FAL_START)
			displayParams.hotThreshold=0;
		else
			displayParams.hotThreshold=vtTemp16-HOT_THRESH_FAL_START;
	}else {
		vtTemperature=(displayParams.hotThreshold+HOT_THRESH_FAL_START-320)/1.8;
		vtTemp16=(u16)(vtTemperature+0.5);
//						vtTemp16=(displayParams.hotThreshold+HOT_THRESH_FAL_START-320-9)/18;		//转成摄氏
		if(vtTemp16>HOT_THRESH_CEL_START+HOT_THRESHOLD_MAX)
			displayParams.hotThreshold=HOT_THRESHOLD_MAX;
		else if (vtTemp16<HOT_THRESH_CEL_START)
			displayParams.hotThreshold=0;
		else
			displayParams.hotThreshold=vtTemp16-HOT_THRESH_CEL_START;
	}
//	StoreData2Flash();
	writeEepromData(UNIT_ADDR,&displayParams.temperatureUnit,1);
	pStorageParamsStruct->temperatureUnit=displayParams.temperatureUnit;
	updateUnitDisplay(&displayParams);
	updateThresholdDisplay(&displayParams);
}


void entryCalibrateMode(void){
 		displayParams.DisplayModeIndex=CalibrateMode;
 		clear(); /* Clear Screen */
 		displaySystemMenu(&displayParams, DISABLE);
// 		fIsSensorTestMode=TRUE;
 		sigmaAdTest();
}
void powerKeyPressedFunc(void) {
	if (vIsSettingKeyGod) {
		return;
	}
	setRedLEDToggle();
//		ADC_Group_SoftwareStart();
	if (fIsSystemOff) {
		fIsSystemOff = FALSE;
		displayParams.DisplayModeIndex = IdleIamgeDisplay;
		powerOnLcdInit();
	}
	if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
		convertAmbientTemperature();
	}
	resetEntrySleepTime();
}
void	incKeyPressedFunc(void){
	u8	vtOffset=0;
	u16   vtTemp16=0;
	if(!vPWMCh3Duty)
		vPWMCh3Duty=0.1;
	vPWMCh3Duty+=0.05;
	if(vPWMCh3Duty>1)
		vPWMCh3Duty=0.1;
	updatePWMChXValue(HT_PWM1,BUZZER_CHANNEL,vPWMCh3Duty*userPwmParaStruct.period);
	//正常画面
	if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
		if (((Object == displayParams.targetCharacter) && (0 != displayParams.validObjectIndex))
				|| ((Human == displayParams.targetCharacter) && (0 != displayParams.validHumanIndex))) {//当前模式有记录

			if ((Object == displayParams.targetCharacter))
				vtOffset = HALF_MAX_TEMP_INDEX;
			if (Object == displayParams.targetCharacter) {
				if (displayParams.memIndex  >= displayParams.validObjectIndex-1)
					displayParams.memIndex = 0;
				else
					displayParams.memIndex++;
			} else {
				if (displayParams.memIndex  >= displayParams.validHumanIndex-1)
					displayParams.memIndex = 0;
				else
					displayParams.memIndex++;
			}
			loadSignalRecord(&displayParams);
			if (pStorageParamsStruct->tempInfo.temperatureValue != 0xffff) {
				updateMemDataDisplay(&displayParams);
			}
		}
	}
	//设置界面
	else if (SettingDisplay == displayParams.DisplayModeIndex){
		//单位设定
		if(SettingUint==displayParams.arrowIndex){
			temperatureUnitSettingFunc();
		}
		//声效设定
		if(SettingVoice==displayParams.arrowIndex){
			displayParams.voiceSetting^=ON;
//					StoreData2Flash();
			writeEepromData(VOICE_ADDR,&displayParams.voiceSetting,1);
			pStorageParamsStruct->voiceSetting=displayParams.voiceSetting;
			updateVoiceDisplay(&displayParams);
		}
		//阈值设定
		if(SettingThreshold==displayParams.arrowIndex){
			if(Fahrenheit==displayParams.temperatureUnit)
					vtTemp16=HOT_THRESHOLD_MAX_F;
			else
					vtTemp16=HOT_THRESHOLD_MAX;
			if(displayParams.hotThreshold>=vtTemp16){
				displayParams.hotThreshold=HOT_THRESHOLD_MIN;
			}
			else
				displayParams.hotThreshold++;
//					StoreData2Flash();
			writeEepromData(THRESHOLD_ADDR,(u8 *)&displayParams.hotThreshold,2);
			pStorageParamsStruct->hotThreshold=displayParams.hotThreshold;
			updateThresholdDisplay(&displayParams);
		}
		//补偿设定
		if(SettingTolerance==displayParams.arrowIndex){
			if(displayParams.tolerance>=TOLERANCE_MAX){
				displayParams.tolerance=TOLERANCE_MIN;
			}
			else
				displayParams.tolerance++;
//					StoreData2Flash();
			writeEepromData(TOLERANCE_ADDR,(u8 *)&displayParams.tolerance,2);
			pStorageParamsStruct->tolerance=displayParams.tolerance;
			updateToleranceDisplay(&displayParams);
		}
	}


}
void	decKeyPressedFunc(void){
	u8	vtOffset=0;
	u16   vtTemp16=0;
	//正常画面
	if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
		if (((Object == displayParams.targetCharacter) && (0 != displayParams.validObjectIndex))
				|| ((Human == displayParams.targetCharacter) && (0 != displayParams.validHumanIndex))) {//当前模式有记录
			if ((Object == displayParams.targetCharacter))
				vtOffset = HALF_MAX_TEMP_INDEX;
			if (Object == displayParams.targetCharacter) {
				if (displayParams.memIndex == 0)
					displayParams.memIndex = displayParams.validObjectIndex-1 ;
				else
					displayParams.memIndex--;
			} else {
				if (displayParams.memIndex == 0)
					displayParams.memIndex = displayParams.validHumanIndex-1;
				else
					displayParams.memIndex--;
			}
			loadSignalRecord(&displayParams);
			if (pStorageParamsStruct->tempInfo.temperatureValue != 0xffff) {
				updateMemDataDisplay(&displayParams);
			}
		}
	}
	//设置界面
	else if (SettingDisplay == displayParams.DisplayModeIndex){
		//单位设定
		if(SettingUint==displayParams.arrowIndex){
			temperatureUnitSettingFunc();
		}
		//声效设定
		else if(SettingVoice==displayParams.arrowIndex){
			displayParams.voiceSetting^=ON;
//					StoreData2Flash();
			writeEepromData(VOICE_ADDR,&displayParams.voiceSetting,1);
			pStorageParamsStruct->voiceSetting=displayParams.voiceSetting;
			updateVoiceDisplay(&displayParams);
		}
		//阈值设定
		else if(SettingThreshold==displayParams.arrowIndex){
			if(Fahrenheit==displayParams.temperatureUnit)
					vtTemp16=HOT_THRESHOLD_MAX_F;
			else
					vtTemp16=HOT_THRESHOLD_MAX;
			if(displayParams.hotThreshold<=HOT_THRESHOLD_MIN){
				displayParams.hotThreshold=vtTemp16;
			}
			else
				displayParams.hotThreshold--;
			StoreData2Flash();
//					writeEepromData(THRESHOLD_ADDR,(u8 *)&displayParams.hotThreshold,2);
			readEepromData(TARGET_ADDR,vtTestArray,15);
			pStorageParamsStruct->hotThreshold=displayParams.hotThreshold;
			updateThresholdDisplay(&displayParams);
		}
		//补偿设定
		else if(SettingTolerance==displayParams.arrowIndex){
			if(displayParams.tolerance<=TOLERANCE_MIN){
				displayParams.tolerance=TOLERANCE_MAX;
			}
			else
				displayParams.tolerance--;
//					StoreData2Flash();
			writeEepromData(TOLERANCE_ADDR,(u8 *)&displayParams.tolerance,2);
			pStorageParamsStruct->tolerance=displayParams.tolerance;
			updateToleranceDisplay(&displayParams);
		}
	}

}

void settingKeyPressedFunc(void) {
	//Normal mode
	if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
		if (Object == displayParams.targetCharacter) {
			displayParams.targetCharacter = Human;
			if (displayParams.HumanRecordIndex)
				displayParams.memIndex = displayParams.HumanRecordIndex - 1;
			else
				displayParams.memIndex = displayParams.HumanRecordIndex;
			displayParams.memValue = pStorageParamsStruct->tempInfo.temperatureValue;
		} else {
			displayParams.targetCharacter = Object;
			if (displayParams.ObjectRecordIndex)
				displayParams.memIndex = displayParams.ObjectRecordIndex - 1;
			else
				displayParams.memIndex = displayParams.ObjectRecordIndex;
			displayParams.memValue = pStorageParamsStruct->tempInfo.temperatureValue;
		}
		loadSignalRecord(&displayParams);
		clear();
		pStorageParamsStruct->targetCharacter = displayParams.targetCharacter;
		writeEepromData(TARGET_ADDR, &displayParams.targetCharacter, 1);
		displaySystemMenu(&displayParams, DISABLE);
	}
	//设置界面
	else if (SettingDisplay == displayParams.DisplayModeIndex) {
		if (displayParams.arrowIndex++ >= Max_Arrow_Index) {
			displayParams.arrowIndex = Min_Arrow_Index;
		}
		updateArrowDisplay(&displayParams);
	}
}
void	settingKeyLongPressedFunc(void){
	if (vIsPowerKeyGod) {
		 	entryCalibrateMode();
	} else {
		if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
			displayParams.DisplayModeIndex = SettingDisplay;
		} else {
			displayParams.DisplayModeIndex = IdleIamgeDisplay;
		}
		clear(); /* Clear Screen */
		displaySystemMenu(&displayParams, DISABLE);
	}
}

void MenuFunction(void ){
	setGreenLEDToggle();
	genKeyTone();
	if (POWER_KEY_SHORT_PRESSED==vKeyValue) {
		powerKeyPressedFunc();
	}
	if (!fIsSystemOff) {
		u8	vtOffset=0;
		u16   vtTemp16=0;
		if (vKeyValue == POWER_KEY_LONG_PRESSED) {
			if (vIsSettingKeyGod) {
				entryCalibrateMode();
			}
		} else if (vKeyValue == INC_KEY_SHORT_PRESSED) {
			incKeyPressedFunc();
		} else if (vKeyValue == DEC_KEY_SHORT_PRESSED) {
			decKeyPressedFunc();
		} else if (vKeyValue == SETTING_KEY_SHORT_PRESSED) {
			settingKeyPressedFunc();
		} else if (vKeyValue == SETTING_KEY_LONG_PRESSED) {
			settingKeyLongPressedFunc();
		}
		if (!fIsSystemOff) {
			resetEntrySleepTime();
		}
	}
	vKeyValue = 0;
}
u8 	getResIndex(const u16 *pArr,u8 minIndex,u8 maxIndex,u16 vtRes){
		u8	i=0;
		u16 vtData=0;
		for(i=minIndex;i<=maxIndex;i++){
			vtData=*(resOfTempAmb+i);
			if(vtRes>=vtData){
				if(vtRes<=(vtData*1.025))
					return i;
				else
					return i-1;
			}
		}
		return 0xFF;
}

u8	getTemperatureOfAmbient(float vtRes){
		u16	vTResAmb=(u16)(vtRes*vResCoe*10);
		u8	i,len=0;
		len=sizeof(resOfTempAmb)/2;
		if((vTResAmb<=resOfTempAmb[Ramb_Degree0_Index])&&(vTResAmb>=resOfTempAmb[Ramb_Degree40_Index])){
			i=getResIndex(resOfTempAmb,Ramb_Degree0_Index,Ramb_Degree40_Index,vTResAmb);
			return	(i+Ramb_Start_Degree);
		}else if((vTResAmb<=resOfTempAmb[Ramb_Degree0_Index])&&(vTResAmb>=resOfTempAmb[0])){
			i=getResIndex(resOfTempAmb,0,Ramb_Degree0_Index,vTResAmb);
			return	(i+Ramb_Start_Degree);
		}else if((vTResAmb<=resOfTempAmb[Ramb_Max_inedex])&&(vTResAmb>=resOfTempAmb[Ramb_Degree40_Index])){
			i=getResIndex(resOfTempAmb,Ramb_Degree40_Index,Ramb_Max_inedex,vTResAmb);
			return	(i+Ramb_Start_Degree);
		}else{
			return 0xFF;
		}
}


float	getObjectTemperature(float vtVoltObj,u8 vtTempAmb){
	u8 i=0;
	u8	vButton=0;//,vTop=0;
	float vLow,vHi;
	for(;i<VOLT_OBJ_TABLE_COLUMN;i++){
		vHi=voltOfObj[vtTempAmb][i];
		if(vtVoltObj<=vHi){
			vLow=voltOfObj[vtTempAmb][i-1];
			if(vtVoltObj>=vLow){
				vButton=10*i-10;
				return (vtVoltObj-vLow)*10/(vHi-vLow)+vButton;
			}else{
				return	OBJECT_TEMP_ERROR;
			}
		}
	}
	return	OBJECT_TEMP_ERROR;
}


void sigmaResultExecute(sigmaAdTypeDef * pAdParas){
	UINT16 vtTemperatureU16=0;
	if(pAdParas->convertOverFlag) {
		if(pAdParas->convertOverFlag&VcmCoeConvertOver) {
			if(CalibrateMode==displayParams.DisplayModeIndex){
				displayFloat(32,0,vVcm_coe,3,' ');
				//convertAmbientTemperature();
				convertMcuTemperature();
			}
			pAdParas->convertOverFlag&=~VcmCoeConvertOver;
		}
		if(pAdParas->convertOverFlag&An0ChConvertOver) {
			convertObjectTemperature();
			if(CalibrateMode==displayParams.DisplayModeIndex){
				displayFloat(24,2,vResAmbient,1,'K');
			}
			pAdParas->convertOverFlag&=~An0ChConvertOver;
			vTempAmbient=getTemperatureOfAmbient(vResAmbient);
			if(CalibrateMode==displayParams.DisplayModeIndex){
				if(vResAmbient){
					OLED_ShowNum(96,2,vTempAmbient);
					OLED_ShowChar(112, 2, ICON_Degree_ADDRESS);
				}
			}
		}
//		if(pAdParas->convertOverFlag&VdacoChConvertOver){
//			pAdParas->convertOverFlag&=~VdacoChConvertOver;
//			if(fIsSensorTestMode){
//				if((vDacOut>0.010)||(vDacOut<=-0.010)){
//					convertDacOutput();
//				}else{
//					displayFloat(80,0,vDacOut,3,'m');
//					convertAmbientTemperature();
//				}
//			}
//		}
		if(pAdParas->convertOverFlag&VTSConvertOver){
			pAdParas->convertOverFlag&=~VTSConvertOver;
			if(CalibrateMode==displayParams.DisplayModeIndex){
				displayFloat(80,0,vVts,2,'m ');
				convertAmbientTemperature();
			}
		}
		if(pAdParas->convertOverFlag&An23ChConverting) {
			pAdParas->convertOverFlag&=~An23ChConverting;
			vVoltOfObj+=0.11;
			if (CalibrateMode==displayParams.DisplayModeIndex) {
				if (vVoltOfObj >= 0) {
					displayFloat(24, 4, vVoltOfObj, 3, 'm');
				} else {
					OLED_ShowChar(24, 4, '-');
					displayFloat(32, 4, vVoltOfObj, 2, 'm');
				}
			}
			vTempOfObj=getObjectTemperature(vVoltOfObj,vTempAmbient);
			if (CalibrateMode==displayParams.DisplayModeIndex) {
				displayFloat(32,6,vTempOfObj,1,ICON_Degree_ADDRESS);
			}else{
					vtTemperatureU16=vTempOfObj*10;
				if(vTempOfObj<0)
					vtTemperatureU16|=0x8000;
				irSensorDataProcess(vtTemperatureU16);
			}
		}
	}
}

float userRound(float data){
//	if(data>=0)
//		return ((float)(data+5))/100;
//	else
//		return ((float)(data+5))/100;
	return (data>=0)?((float)(data+5))/100: ((float)(data+5))/100;
}
/*****************************************************************
 *
 *温度计算
 *
 ******************************************************************/
//float calcTemperature(u16 u16TemperatureValue,displayUnitEnum unit){
//	float	vtTemp=0;
//	vtTemp=u16TemperatureValue*2.0-27315;
//	if(unit==Celsius){
//		vtTemp+=(displayParams.tolerance-DEFAULT_TOLERANCE)*10;
//		return	userRound(vtTemp);
//	}
//	else{
//		if(vtTemp>=0){
//			vtTemp=(vtTemp+5);
//		}else{
//			vtTemp= (vtTemp-5);
//		}
//		vtTemp=((float)((vtTemp*1.8+3200)+(displayParams.tolerance-DEFAULT_TOLERANCE)*10));		//转成华氏温度*100
//		return	userRound(vtTemp);
//	}
//}
float calcTemperature(u16 u16TemperatureValue,displayUnitEnum unit){
	float	vtTemp=0;
//	vtTemp=u16TemperatureValue*2.0-27315;
	vtTemp=(u16TemperatureValue&0x7FFF)*10.0;
	if(u16TemperatureValue&0x8000)
		vtTemp*=-1;
	if(unit==Celsius){
		vtTemp+=(displayParams.tolerance-DEFAULT_TOLERANCE)*10;
		return	userRound(vtTemp);
	}
	else{
		if(vtTemp>=0){
			vtTemp=(vtTemp+5);
		}else{
			vtTemp= (vtTemp-5);
		}
		vtTemp=((float)((vtTemp*1.8+3200)+(displayParams.tolerance-DEFAULT_TOLERANCE)*10));		//转成华氏温度*100
		return	userRound(vtTemp);
	}
}
/**********************************************************************
 *
 *
 *
 *******************************************************************/
void irSensorDataProcess(u16 originalIrData){
	//displayParams.humanStts
	float vtTemperature=0;
	vtTemperature=calcTemperature(originalIrData,Celsius);
	//测试体温
	if(Human==displayParams.targetCharacter){
		if((vtTemperature>=HUMAN_LOW_THRESHOLD)&&(vtTemperature<=HUMAN_HI_THRESHOLD)){
			genConfrimTone();
			vtTemperature=calcTemperature(originalIrData,displayParams.temperatureUnit);
			if(getHotThreshold()<=vtTemperature)
				displayParams.temperatureStts=StatusHot;
			else
				displayParams.temperatureStts=StatusNormal;
			displayParams.memValue=originalIrData;
			DDL_ZERO_STRUCT(pStorageParamsStruct->tempInfo);
//			pStorageParamsStruct->temperatureValueArray[displayParams.HumanRecordIndex]=originalIrData;
			//保存有效数据
			displayParams.memIndex=displayParams.HumanRecordIndex;
			if(displayParams.validHumanIndex<(HALF_MAX_TEMP_INDEX-1)){
				displayParams.validHumanIndex++;
			}
			if(displayParams.HumanRecordIndex==(HALF_MAX_TEMP_INDEX-1)){
				displayParams.HumanRecordIndex=0;
			}else{
				displayParams.HumanRecordIndex++;
			}
			pStorageParamsStruct->validHumanIndex=displayParams.validHumanIndex;
			pStorageParamsStruct->HumanRecordIndex=displayParams.HumanRecordIndex;
			StoreNewRecord(&displayParams);
			mainTemperatureDisplay(&displayParams,ENABLE);
			updateMemDataDisplay(&displayParams);
		}else {
			genErrorTone();
			if(HUMAN_LOW_THRESHOLD>vtTemperature)
				displayParams.temperatureStts=StatusOverflowLow;
			else
				displayParams.temperatureStts=StatusOverflowHi;
			mainTemperatureDisplay(&displayParams,ENABLE);
		}
	}
	//测试物体表面温度
	else if(Object==displayParams.targetCharacter){
		if((vtTemperature>=OBJECT_LOW_THRESHOLD)&&(vtTemperature<=OBJECT_HI_THRESHOLD)){
			genConfrimTone();
			displayParams.temperatureStts=StatusNormal;
			displayParams.memValue=originalIrData;
			DDL_ZERO_STRUCT(pStorageParamsStruct->tempInfo);
//			pStorageParamsStruct->temperatureValueArray[displayParams.ObjectRecordIndex+HALF_MAX_TEMP_INDEX]=originalIrData;			//保存有效数据
			displayParams.memIndex=displayParams.ObjectRecordIndex;
			if(displayParams.validObjectIndex<(HALF_MAX_TEMP_INDEX-1)){
				displayParams.validObjectIndex++;
			}
			if(displayParams.ObjectRecordIndex==(HALF_MAX_TEMP_INDEX-1)){
				displayParams.ObjectRecordIndex=0;
			}else{
				displayParams.ObjectRecordIndex++;
			}
			pStorageParamsStruct->validObjectIndex=displayParams.validObjectIndex;
			pStorageParamsStruct->ObjectRecordIndex=displayParams.ObjectRecordIndex;
			StoreNewRecord(&displayParams);
			mainTemperatureDisplay(&displayParams,ENABLE);
			updateMemDataDisplay(&displayParams);
		}else {
			genErrorTone();
			if(OBJECT_LOW_THRESHOLD>vtTemperature)
				displayParams.temperatureStts=StatusOverflowLow;
			else
				displayParams.temperatureStts=StatusOverflowHi;
			mainTemperatureDisplay(&displayParams,ENABLE);
		}
	}
}
