/*
 * user_key.c
 *
 *  Created on: 2020年3月19日
 *      Author: Bough_004
 */

#include "user_key.h"
#include "user_menu.h"
#include "user_lptimer.h"
#include "io_define.h"
extern	u16 vSystemLIRC5ms;
u8	vKeyValue;
u8 	vIsKeyPressed=0;
u8 	vIsFuncKeyGod;
u8 	vIsClosedKeyGod;
u8	vDoubleClickCountDown;
u16		vFuncKeyTimer;

u8 	vIsPowerKeyGod;
u16	vPowerKeyTimer;

u16	vIncKeyTimer;
u8 	vIsIncKeyGod;

u16	vDecKeyTimer;
u8 	vIsDecKeyGod;

//u16	vSettingKeyTimer;
//u8 	vIsSettingKeyGod;
u16	vSettingKeyTimer;
u8 	vIsSettingKeyGod;

//u16	vBrightnessDownKeyTimer;
//u8 	vIsBrightnessDownKeyGod;
void closedReleasedFunc(void){
//	vIsKeyPressed&=~CLOSED_KEY_PRESSED;
//	vClosedKeyTimer=0;
//	if(!fIsCharging){
//		if(!fIsDischarging){
//			turnOffBlueLED();
//			motorFunctionOff();
//		}else {						//discharging
//			if(fBlueLEDStart)
//				turnOffBlueLED();
//			set_BlueLED;
//		}
//
//	}
//	vIsClosedKeyGod&=~(CLOSED_KEY_SHOT_PRRESS_GOD|CLOSED_KEY_LONG_PRRESS_GOD);
}


void	 funcPowerKeyReleased(void){
	clrPowerKeyPressed;
	vPowerKeyTimer=0;
	vIsPowerKeyGod&=~(POWER_KEY_SHOT_PRRESS_GOD|POWER_KEY_LONG_PRRESS_GOD);
}
void  funcIncKeyReleased(void){
	clrIncKeyPressed;
	vIncKeyTimer=0;
	vIsIncKeyGod&=~(INC_KEY_SHOT_PRRESS_GOD|INC_KEY_LONG_PRRESS_GOD);
}

void  funcDecKeyReleased(void){
	clrDecKeyPressed;
	vDecKeyTimer=0;
	vIsDecKeyGod&=~(DEC_KEY_SHOT_PRRESS_GOD|DEC_KEY_LONG_PRRESS_GOD);
}
void  funcSettingKeyReleased(void){
	clrSettingKeyPressed;
	vSettingKeyTimer=0;
	vIsSettingKeyGod&=~(SETTING_KEY_SHOT_PRRESS_GOD|SETTING_KEY_LONG_PRRESS_GOD);
}

//void	funcBrightnessDownKeyReleased(){
//	clrBrightnessDownKeyPressed;
//	vBrightnessDownKeyTimer=0;
//	vIsBrightnessDownKeyGod&=~(BRIGHTNESS_DOWN_KEY_SHOT_PRRESS_GOD|BRIGHTNESS_DOWN_KEY_LONG_PRRESS_GOD);
//}

void funcKeyTask(void) {

	// power Key 释放
	if (checkPowerKeyPressed&& GET_POWER_KEY_PIN_STTS  ){
		funcPowerKeyReleased();
	}
	// Inc Key 释放
	if (checkIncKeyPressed && GET_INC_KEY_PIN_STTS ){
		funcIncKeyReleased();
	}
	// Dec Key 释放
	if (checkDecKeyPressed && GET_DEC_KEY_PIN_STTS ){
		funcDecKeyReleased();
	}
	// Setting key 释放
	if (checkSettingKeyPressed && GET_SETTING_KEY_PIN_STTS ){
		if((vIsSettingKeyGod&SETTING_KEY_SHOT_PRRESS_GOD)&&!(vIsSettingKeyGod&SETTING_KEY_LONG_PRRESS_GOD)){
					//vKeyValue = POWER_KEY_SHORT_PRESSED;
					vKeyValue = SETTING_KEY_SHORT_PRESSED;								//
		}
		funcSettingKeyReleased();
	}
//	// Brightness Down Key 释放
//	if (checkBrightnessDownKeyPressed && GET_BRIGHTNESS_DOWN_KEY_PIN_STTS ){
//		funcBrightnessDownKeyReleased();
//	}


	// power Key 检测
	if(!GET_POWER_KEY_PIN_STTS){
		if (checkPowerKeyPressed ) {
			if (!(vIsPowerKeyGod&POWER_KEY_SHOT_PRRESS_GOD)) {
				if (((vPowerKeyTimer+vSystemLIRC5ms)&0xffff) > SHORT_PRESSED_TIME) {
					vIsPowerKeyGod|=POWER_KEY_SHOT_PRRESS_GOD;
					vKeyValue = POWER_KEY_SHORT_PRESSED;
				}
			} else {
				if (!(vIsPowerKeyGod&POWER_KEY_LONG_PRRESS_GOD)) {
					if (((vPowerKeyTimer+vSystemLIRC5ms)&0xffff) > LONG_PRESSED_2S) {
						vKeyValue = POWER_KEY_LONG_PRESSED;
						vIsPowerKeyGod|=POWER_KEY_LONG_PRRESS_GOD;
						vPowerKeyTimer = 0;	//stop cnt
					}
				}
			}
		}else{
			setPowerKeyPressedFlag();
		}
	}


	// Inc Key 检测
	if(!GET_INC_KEY_PIN_STTS){
		if (checkIncKeyPressed ) {
			if (!(vIsIncKeyGod&INC_KEY_SHOT_PRRESS_GOD)) {
				if (((vIncKeyTimer+vSystemLIRC5ms)&0xffff) > SHORT_PRESSED_TIME) {
					vIsIncKeyGod|=INC_KEY_SHOT_PRRESS_GOD;
					vKeyValue = INC_KEY_SHORT_PRESSED;
				}
			} else {
				if (!(vIsIncKeyGod&INC_KEY_LONG_PRRESS_GOD)) {
					if (((vIncKeyTimer+vSystemLIRC5ms)&0xffff) > LONG_PRESSED_0S5) {
						vKeyValue = INC_KEY_LONG_PRESSED;
						vIsIncKeyGod|=INC_KEY_LONG_PRRESS_GOD;
						vIncKeyTimer = 0;	//stop cnt
					}
				}
			}
		}else{
			setIncKeyPressedFlag();
		}

	}
	// Dec Key 检测
	if(!GET_DEC_KEY_PIN_STTS){
		if (checkDecKeyPressed ) {
			if (!(vIsDecKeyGod&DEC_KEY_SHOT_PRRESS_GOD)) {
				if (((vDecKeyTimer+vSystemLIRC5ms)&0xffff) > SHORT_PRESSED_TIME) {
					vIsDecKeyGod|=DEC_KEY_SHOT_PRRESS_GOD;
					vKeyValue = DEC_KEY_SHORT_PRESSED;
				}
			} else {
				if (!(vIsDecKeyGod&DEC_KEY_LONG_PRRESS_GOD)) {
					if (((vDecKeyTimer+vSystemLIRC5ms)&0xffff) > LONG_PRESSED_0S5) {
						vKeyValue = DEC_KEY_LONG_PRESSED;
						vIsDecKeyGod|=DEC_KEY_LONG_PRRESS_GOD;
						vDecKeyTimer = 0;	//stop cnt
					}
				}
			}
		}else{
			setDecKeyPressedFlag();
		}
	}

	// Setting Key 检测
	if(!GET_SETTING_KEY_PIN_STTS){
		if (checkSettingKeyPressed ) {
			if (!(vIsSettingKeyGod&SETTING_KEY_SHOT_PRRESS_GOD)) {
				if (((vSettingKeyTimer+vSystemLIRC5ms)&0xffff) > SHORT_PRESSED_TIME) {
					vIsSettingKeyGod|=SETTING_KEY_SHOT_PRRESS_GOD;
//					vKeyValue = SETTING_KEY_SHORT_PRESSED;
				}
			} else {
				if (!(vIsSettingKeyGod&SETTING_KEY_LONG_PRRESS_GOD)) {
					if (((vSettingKeyTimer+vSystemLIRC5ms)&0xffff) > LONG_PRESSED_1S) {
						vKeyValue = SETTING_KEY_LONG_PRESSED;
						vIsSettingKeyGod|=SETTING_KEY_LONG_PRRESS_GOD;
					}
				}
			}
		}else{
			setSettingKeyPressedFlag();
		}
	}
}

void setPowerKeyPressedFlag(void) {
	vPowerKeyTimer = 0x10000 - vSystemLIRC5ms;
	vIsKeyPressed |= Power_KEY_PRESSED;
}
void setIncKeyPressedFlag(void) {
	funcIncKeyReleased();
	vIncKeyTimer = 0x10000 - vSystemLIRC5ms;
	vIsKeyPressed |= Inc_KEY_PRESSED;
}
void setDecKeyPressedFlag(void) {
	funcDecKeyReleased();
	vDecKeyTimer = 0x10000 - vSystemLIRC5ms;
	vIsKeyPressed |= Dec_KEY_PRESSED;
}
void setSettingKeyPressedFlag(void) {
	funcSettingKeyReleased();
	vSettingKeyTimer = 0x10000 - vSystemLIRC5ms;
	vIsKeyPressed |= Setting_KEY_PRESSED;
}
//void setBrightnessDownKeyPressedFlag(void) {
//	funcBrightnessDownKeyReleased();
//	vBrightnessDownKeyTimer = 0x10000 - vSystemLIRC5ms;
//	vIsKeyPressed |= BrightnessDown_KEY_PRESSED;
//}
void holdingKeyFunc(void) {
	if (vIsIncKeyGod&INC_KEY_LONG_PRRESS_GOD) {
		vKeyValue = INC_KEY_SHORT_PRESSED;					//长按时每0.1秒触发一个按键
	}
	if (vIsDecKeyGod&DEC_KEY_LONG_PRRESS_GOD) {
		vKeyValue = DEC_KEY_SHORT_PRESSED;					//长按时每0.1秒触发一个按键
	}
//	if (vIsSettingKeyGod&BRIGHTNESS_UP_KEY_LONG_PRRESS_GOD) {
//		vKeyValue = BRIGHTNESS_UP_KEY_SHORT_PRESSED;		//长按时每0.05秒触发一个按键
//	}
//	if (vIsBrightnessDownKeyGod&BRIGHTNESS_DOWN_KEY_LONG_PRRESS_GOD) {
//		vKeyValue = BRIGHTNESS_DOWN_KEY_SHORT_PRESSED;		//长按时每0.05秒触发一个按键
//	}
}
