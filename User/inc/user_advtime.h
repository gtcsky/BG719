/*
 * user_advtime.h
 *
 *  Created on: 2020年3月20日
 *      Author: Bough_004
 */
#include "const.h"
//#include "ht32f5xxxx_tm.h"




extern	void 	ADVTIM_PWMMODE_Config(UserPwmInitTypeDef *pwmStruct ,HT_TM_TypeDef*  TIMx);
extern	void		updatePWMChXValue(HT_TM_TypeDef* TMx,userPWMChEnum vtChx,u16 vtCompare );
extern	void		updatePWMChlDuty(userPWMChEnum vtChx,float vtDuty,HT_TM_TypeDef* TMx);
