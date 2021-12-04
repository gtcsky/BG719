/*
 * user_lptimer.h
 *
 *  Created on: 2020年3月18日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_LPTIMER_H_
#define USER_INC_USER_LPTIMER_H_
#include "const.h"


#define	TIMER_5MS_BY_32K		160// 0.005s/(1/32000)

extern	u16 	vSystemLIRC5ms;
extern	u16 	vSystem5ms;
extern	u8	vTimerCnt;
extern	u8	fTime50ms;
extern	u8	fTime100ms;
extern	u8	fTime500ms;
extern	u8	fTime1s;
void LPTIMER_Config(void);
void Enter_DeepSleep(void);
void Enter_Sleep(void);
#endif /* USER_INC_USER_LPTIMER_H_ */
