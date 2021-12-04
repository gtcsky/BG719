/*
 * user_lptimer.c
 *
 *  Created on: 2020年3月18日
 *      Author: Bough_004
 */
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "io_define.h"
#include "user_lptimer.h"
#include "user_menu.h"
#include "ht32f5xxxx_rtc.h"

u16 vSystemLIRC5ms;
u16 vSystem5ms;
u8	vTimerCnt;
u8	fTime50ms;
u8	fTime100ms;
u8	fTime500ms;
u8	fTime1s;


/** @defgroup Enter_DeepSleep
  * @{
  */
void Enter_DeepSleep(void)
{
	/*
	 *配置AWK功能
	 */
//	AWK_Config();
//	NVIC_EnableIRQ(AWK_IRQn); 							/*打开ARM中断使能*/
//	LPTIMER_Cmd(DEBUG_LPTIMERx,DISABLE);				//关闭LPT 防止唤醒
	RTC_Cmd(DISABLE);									//关闭RTC防止唤醒
	SCB->SCR |= 0x04;
	__WFI();
}


/*********************************************************************************************************//**
 * @brief Configures the RTC.
 * @retval None
 * @details RTC configuration as following:
 *  - S/W reset backup domain.
 *  - Configure Low Speed External clock (LSE).
 *    - The LSE use fast startup mode.
 *      Shortly startup time but higher power consumption.
 *    - Enable the LSE.
 *    - Waits till LSE is ready.
 *  - Configure RTC.
 *    - Select LSE as RTC Clock Source.
 *    - Enable the RTC Second interrupt.
 *    - RTC prescaler = 32768 to generate 1 second interrupt.
 *    - Enable clear RTC counter by match function.
 *
 ************************************************************************************************************/
void LPTIMER_Config(void)			//user RTC as LPTIMER
{
  /* Reset Backup Domain                                                                                    */
  PWRCU_DeInit();

//  /* Configure Low Speed External clock (LSE)                                                               */
//  RTC_LSESMConfig(RTC_LSESM_NORMAL);
//  RTC_LSECmd(ENABLE);
  while (CKCU_GetClockReadyStatus(CKCU_FLAG_LSIRDY) == RESET);

  /* Configure the RTC                                                                                      */
  RTC_ClockSourceConfig(RTC_SRC_LSI);
  //RTC_IntConfig(RTC_INT_CSEC, ENABLE);
  RTC_IntConfig(RTC_INT_CM, ENABLE);
  //RTC_IntConfig(RTC_INT_OV, ENABLE);
  RTC_SetPrescaler(RTC_RPRE_1);
  RTC_SetCompare(TIMER_5MS_BY_32K);
  /* Restart counter when match event occurred                                                              */
  RTC_CMPCLRCmd(ENABLE);
  /* Enable NVIC RTC interrupt                                                                              */
  NVIC_EnableIRQ(RTC_IRQn);
  RTC_Cmd(ENABLE);
}



/*********************************************************************************************************//**
 * @brief   This function handles RTC interrupt.
 * @retval  None
 * @details In RTC interrupt service routine:
 *  - Reset RTC init time when Time is 23:59:59.
 *    - Set RTC Compare register as 86400.
 *    - Reset PWRCU_BAKREG_1 to 0.
 *  - Toggle LED1 each 1s.
 *  - Set gwTimeDisplay to 1 to enable time update.
 *
 ************************************************************************************************************/
void RTC_IRQHandler(void) {
	// extern vu32 gwTimeDisplay;
	u8 bFlags;

	bFlags = RTC_GetFlagStatus();
	if ((bFlags & 0x2) != 0x0) /* Match flag */
	{
		RTC_SetCompare(TIMER_5MS_BY_32K);
//		set_userKey1_LEDToggle();
		vSystemLIRC5ms++;
		vSystem5ms++;
		vTimerCnt++;
		if(vTimerCnt%10==0){
			fTime50ms=1;
		}
		if(vTimerCnt%20==0){
			fTime100ms=1;
		}
		if(vTimerCnt%100==0){
			fTime500ms=1;
		}
		if(vTimerCnt%200==0){
			fTime1s=1;
			vTimerCnt=0;
		}
		if(vSystem5ms>=USER_DOG_CONST){
			NVIC_SystemReset();
		}
	}
	/* Enable time update */
//  gwTimeDisplay = 1;
}
