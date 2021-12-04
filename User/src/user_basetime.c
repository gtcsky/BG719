
// 基本定时器TIMx,x[6,7]定时初始化函数

#include "user_basetime.h"
#include "const.h"
#include "ht32.h"
#include "ht32_board.h"
#include "user_menu.h"
uint32_t BASIC_TIMFLag = 0;
u16 vTime1msCnt;


void BASIC_TIM_Mode_Config(void) {
	NVIC_EnableIRQ(BFTM0_IRQn);
	BFTM_SetCompare(HT_BFTM0, SystemCoreClock / (1000/1));				//1ms
	BFTM_SetCounter(HT_BFTM0, 0);
	BFTM_IntConfig(HT_BFTM0, ENABLE);
	BFTM_EnaCmd(HT_BFTM0, ENABLE);
}

void BFTM0_IRQHandler(void) {							//1MS一次中断
//	     BFTM_EnaCmd(HT_BFTM1, ENABLE);
//	    while (BFTM_GetFlagStatus(HT_BFTM1) != SET);
//	    BFTM_ClearFlag(HT_BFTM1);
		BFTM_ClearFlag( HT_BFTM0);
		vTime1msCnt++;
			//set_userKey1_LEDToggle();
		//set_SettingKey_LEDToggle();
		if (vTime1msCnt >= 10000) {
			vTime1msCnt = 0;
		}
}

