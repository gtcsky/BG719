/*
 * user_advtime.c
 *
 *  Created on: 2020年3月20日
 *      Author: Bough_004
 */

#include "user_advtime.h"
#include "io_define.h"
#include	"ht32f5xxxx_tm_type.h"
#include "user_menu.h"
//void ADVTIM_PWMMODE_Config(UserPwmInitTypeDef *pwmStruct ,ADVTIM_TypeDef* TIMx){
	u32  clock=0;
/* Private types -------------------------------------------------------------------------------------------*/
typedef struct
{
  u32 OverflowCounter;
  u32 StartValue;
  u32 CapturePulse;
  TM_CHP_Enum ChannelPolarity;
  bool DataValid;
  bool DataOverwrite;
} sPulseCaptureStructure;

/* Private variables ---------------------------------------------------------------------------------------*/
sPulseCaptureStructure CaptureCHx;

void ADVTIM_PWMMODE_Config(UserPwmInitTypeDef *pwmStruct ,HT_TM_TypeDef*  TIMx){
	  TM_TimeBaseInitTypeDef TM_TimeBaseInitStructure;
	  TM_OutputInitTypeDef TM_OutputInitStructure;
//	  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
//	  CKCUClock.Bit.AFIO                            = 1;
//	  HTCFG_OUTPUT_PWM_BY_SCTM_CLK(CKCUClock)       = 1;
//	  CKCU_PeripClockConfig(CKCUClock, ENABLE);

//	  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_8, AFIO_FUN_PWM) ;
	  AFIO_GPxConfig(BUZZER_PortID, BUZZER_PortPin, AFIO_FUN_PWM) ;
	  /* Init SCTM0 time-base for PWM                                                                           		*/
//	  TM_TimeBaseInitStructure.Prescaler = HTCFG_INPUT_CAPTURE_PRESCALER-1;
	  TM_TimeBaseInitStructure.Prescaler = 0;
	  clock=SystemCoreClock;
//	  TM_TimeBaseInitStructure.CounterReload = SystemCoreClock/(TM_TimeBaseInitStructure.Prescaler + 1)-1;
	  TM_TimeBaseInitStructure.CounterReload = pwmStruct->period;
	  TM_TimeBaseInitStructure.RepetitionCounter = 0;
	  TM_TimeBaseInitStructure.CounterMode = TM_CNT_MODE_UP;
	  TM_TimeBaseInitStructure.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
	  TM_TimeBaseInit(TIMx, &TM_TimeBaseInitStructure);

	  /* Clear Update Event Interrupt flag                                                                      		*/
	  TM_ClearFlag(TIMx, TM_FLAG_UEV);

	  TM_OutputInitStructure.OutputMode = TM_OM_PWM2;
	  TM_OutputInitStructure.Control = TM_CHCTL_ENABLE;
	  TM_OutputInitStructure.ControlN = TM_CHCTL_DISABLE;//TM_CHCTL_ENABLE;
	  TM_OutputInitStructure.Polarity = TM_CHP_INVERTED;
	  TM_OutputInitStructure.PolarityN = TM_CHP_NONINVERTED;
	  TM_OutputInitStructure.IdleState = MCTM_OIS_LOW;
	  TM_OutputInitStructure.IdleStateN = MCTM_OIS_HIGH;
	  if(pwmStruct->channels&userPWM_CH0){
		  TM_OutputInitStructure.Channel = TM_CH_0;
	  	  TM_OutputInitStructure.Compare =TM_TimeBaseInitStructure.CounterReload*pwmStruct->dutyArray[TM_CH_0];
	  	  TM_OutputInit(TIMx, &TM_OutputInitStructure);
	  }
	  if(pwmStruct->channels&userPWM_CH1){
		  TM_OutputInitStructure.Channel = TM_CH_1;
	  	  TM_OutputInitStructure.Compare =TM_TimeBaseInitStructure.CounterReload*pwmStruct->dutyArray[TM_CH_1];
	  	  TM_OutputInit(TIMx, &TM_OutputInitStructure);
	  }
	  if(pwmStruct->channels&userPWM_CH2){
		  TM_OutputInitStructure.Channel = TM_CH_2;
	  	  TM_OutputInitStructure.Compare =TM_TimeBaseInitStructure.CounterReload*pwmStruct->dutyArray[TM_CH_2];
	  	  TM_OutputInit(TIMx, &TM_OutputInitStructure);
	  }
	  if(pwmStruct->channels&userPWM_CH3){
		  TM_OutputInitStructure.Channel = TM_CH_3;
	  	  TM_OutputInitStructure.Compare =TM_TimeBaseInitStructure.CounterReload*pwmStruct->dutyArray[TM_CH_3];
	  	  TM_OutputInit(TIMx, &TM_OutputInitStructure);
	  }
	  /* SCTM0/PWM counter enable                                                                               		*/
	  if(pwmStruct->channels)
		  TM_Cmd(TIMx, ENABLE);
}

void	updatePWMChXValue(HT_TM_TypeDef* TMx,userPWMChEnum vtChx,u16 vtCompare ){
	float vtDuty=(float)vtCompare/userPwmParaStruct.period;
	if(vtCompare>(u16)TMx->CRR)
		vtCompare=TMx->CRR;
	if(vtChx==userPWM_CH0){
		TMx->CH0CCR=vtCompare;
		userPwmParaStruct.dutyArray[TM_CH_0]=vtDuty;
	}
	else if(vtChx==userPWM_CH1){
		TMx->CH1CCR=vtCompare;
		userPwmParaStruct.dutyArray[TM_CH_1]=vtDuty;
	}
	else if(vtChx==userPWM_CH2){

		userPwmParaStruct.dutyArray[TM_CH_2]=vtDuty;
		TMx->CH2CCR=vtCompare;
	}
	else if(vtChx==userPWM_CH3){
		userPwmParaStruct.dutyArray[TM_CH_3]=vtDuty;
		TMx->CH3CCR=vtCompare;
	}
}

void	updatePWMChlDuty(userPWMChEnum vtChx,float vtDuty,HT_TM_TypeDef* TMx){
	updatePWMChXValue(TMx,vtChx,vtDuty*userPwmParaStruct.period);
}
/*********************************************************************************************************//**
 * @brief   CaptureProcess function.
 * @retval  None
 ************************************************************************************************************/
void CaptureProcess(sPulseCaptureStructure* cap, u16 capture_value, bool isCapBeforeUpdate)
{
  if (cap->ChannelPolarity == TM_CHP_NONINVERTED)
  {
    /* Reset OverflowCounter and store capture value when rising edge occurred                              */
    if (isCapBeforeUpdate)
    {
      cap->OverflowCounter = 1;
    }
    else
    {
      cap->OverflowCounter = 0;
    }

    cap->StartValue = capture_value;

    /* Change channel polarity to capture when falling edge occur                                           */
    cap->ChannelPolarity = TM_CHP_INVERTED;
  }
  else
  {
    /* Compute pulse width in PCLK unit when falling edge occurred                                          */
    if (isCapBeforeUpdate)
      cap->OverflowCounter--;

    cap->CapturePulse = (cap->OverflowCounter << 16) + capture_value - cap->StartValue;

    if (cap->DataValid)
      cap->DataOverwrite = TRUE;
    else
      cap->DataValid = TRUE;

    /* Change channel polarity to capture when rising edge occur                                            */
    cap->ChannelPolarity = TM_CHP_NONINVERTED;
  }
}

/*********************************************************************************************************//**
 * @brief   This function handles General Purpose Timer interrupt.
 * @retval  None
 ************************************************************************************************************/
void GPTM0_IRQHandler(void)
{
  bool update_flag = FALSE;

  /* store and clear all interrupt flags                                                                    */
  u32 status = HTCFG_INPUT_CAPTURE_GPTM->INTSR;
  u32 cnt = HTCFG_INPUT_CAPTURE_GPTM->CNTR;
  if ((status & TM_INT_UEV) != (HTCFG_INPUT_CAPTURE_GPTM->INTSR & TM_INT_UEV))
  {
    status = HTCFG_INPUT_CAPTURE_GPTM->INTSR;
    cnt = HTCFG_INPUT_CAPTURE_GPTM->CNTR;
  }
  HTCFG_INPUT_CAPTURE_GPTM->INTSR = ~status;

  if (status & TM_INT_UEV)
  {
    update_flag = TRUE;
    /* The OverflowCounter will stop at max value 0xFFFF                                                    */
    if (CaptureCHx.OverflowCounter != 0xFFFF) CaptureCHx.OverflowCounter++;
  }

  if (status & HTCFG_INPUT_CAPTURE_GPTM_CCR)
  {
    u32 cap_value = TM_GetCaptureCompare1(HTCFG_INPUT_CAPTURE_GPTM);
    bool isCapBeforeUpdate = (update_flag && (cap_value > cnt))? TRUE : FALSE;
    CaptureProcess(&CaptureCHx, cap_value, isCapBeforeUpdate);
    TM_ChPolarityConfig(HTCFG_INPUT_CAPTURE_GPTM, HTCFG_INPUT_CAPTURE_GPTM_CHANNEL, CaptureCHx.ChannelPolarity);
  }
}
