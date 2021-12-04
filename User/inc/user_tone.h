/*
 * user_tone.h
 *
 *  Created on: 2020年3月7日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_TONE_H_
#define USER_INC_USER_TONE_H_

#include	"const.h"

#define	TONE_DUTY	0.3


typedef struct{
//	toneIdEnum 	toneID;
	u8	toneProcessIndex;
	u8	toneMaxIndex;
	u8 	tongProcessCnt;
	switchSttsEnum 	tongStts;
	const 	UINT8 *	timeArray;
	const	UINT8 *	sttsArray;
	func_ptr_t  pfnToneFuncCallBack;
}voiceParamStruct;


extern	void 	turnOffTone(void) ;
extern	void 	voiceFunc(void);
extern	void 	genEndTone(void);
extern	void 	genKeyTone(void);
extern	void 	genConfrimTone(void);
extern	void	 	genErrorTone(void);
#endif /* USER_INC_USER_TONE_H_ */
