/*
 * user_adc.c
 *
 *  Created on: 2020年3月7日
 *      Author: Bough_004
 */
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "ht32f5xxxx_adc.h"

#include "adc_group.h"
#include "user_adc.h"
#include "io_define.h"


vu32 uADCGroupState = 1;    // 0: Stop, 1: Group 1, 2: Group 2.
u8 	uADCGroupFinish = 0;   // 0: Not finish, 2: Finished.

u16 uADCData[16];
u32 uTimeTick500ms = 4;

//void ADC_Test_MainRoutine_Softwre(void);
//void ADC_Test_MainRoutine_TM(void);

void ADC_Group1Config(void);
//void ADC_Group2Config(void);
void ADC_GroupProcess(void);
/*********************************************************************************************************//**
  * @brief  Configure the ADC
  * @retval None
  ***********************************************************************************************************/
void ADC_Configuration(u16 chs)
 {
	{
		CKCU_PeripClockConfig_TypeDef CKCUClock = { { 0 } };
		CKCUClock.Bit.AFIO = 1;
		CKCUClock.Bit.ADC = 1;
		CKCU_PeripClockConfig(CKCUClock, ENABLE);
	}

	/* Configure GPIO/AFIO mode as ADC function                                                               */
	if(chs&UserChannel0)
		AFIO_GPxConfig(HTCFG_ADCCH0_GPIO_ID, HTCFG_ADCCH0_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel1)
		AFIO_GPxConfig(HTCFG_ADCCH1_GPIO_ID, HTCFG_ADCCH1_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel2)
		AFIO_GPxConfig(HTCFG_ADCCH2_GPIO_ID, HTCFG_ADCCH2_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel3)
		AFIO_GPxConfig(HTCFG_ADCCH3_GPIO_ID, HTCFG_ADCCH3_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel4)
		AFIO_GPxConfig(HTCFG_ADCCH4_GPIO_ID, HTCFG_ADCCH4_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel5)
		AFIO_GPxConfig(HTCFG_ADCCH5_GPIO_ID, HTCFG_ADCCH5_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel6)
		AFIO_GPxConfig(HTCFG_ADCCH6_GPIO_ID, HTCFG_ADCCH6_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel7)
		AFIO_GPxConfig(HTCFG_ADCCH7_GPIO_ID, HTCFG_ADCCH7_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel8)
		AFIO_GPxConfig(HTCFG_ADCCH8_GPIO_ID, HTCFG_ADCCH8_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel9)
		AFIO_GPxConfig(HTCFG_ADCCH9_GPIO_ID, HTCFG_ADCCH9_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel10)
		AFIO_GPxConfig(HTCFG_ADCCH10_GPIO_ID, HTCFG_ADCCH10_AFIO_PIN, AFIO_FUN_ADC);
	if(chs&UserChannel11)
		AFIO_GPxConfig(HTCFG_ADCCH11_GPIO_ID, HTCFG_ADCCH11_AFIO_PIN, AFIO_FUN_ADC);
	/* ADCLK frequency is set to CK_AHB / 1                                                                  					 */

	CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV3);

	/* One Shot Mode                                                                                          						*/
	ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, ADC_GROUP_LIST_LENGTH, 0);

	ADC_Group1Config();

	/* Set sampling time as 1.5 + 36 ADCCLK, Conversion = 12.5 + 1.5 + 36 = 50 ADCCLK                         	*/
	ADC_SamplingTimeConfig(HT_ADC, 36);

	/* Enable ADC cycle end of conversion interrupt                                                           				*/
	ADC_IntConfig(HT_ADC, ADC_INT_CYCLE_EOC, ENABLE);

//#if (ENABLE_DEBUG_IO == 1)
//	ADC_IntConfig(HT_ADC, ADC_INT_SINGLE_EOC, ENABLE);
//#endif
	NVIC_EnableIRQ(ADC_IRQn);

#if (ADC_GROUP_DATA_MODE == ADC_GROUP_DATA_MODE_DMA)
	/* Issue ADC DMA request when cycle end of conversion occur                                              			 */
	ADC_PDMAConfig(HT_ADC, ADC_PDMA_REGULAR_CYCLE, ENABLE);
#endif

	ADC_Cmd(HT_ADC, ENABLE);
}



/*********************************************************************************************************//**
  * @brief  Software trigger of ADC Group
  * @retval None
  ***********************************************************************************************************/
void ADC_Group_SoftwareStart(void)
{
  uADCGroupFinish = 0;
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);
}

/*********************************************************************************************************//**
  * @brief  Configure ADC conversion list as group 1.
  * @retval None
  ***********************************************************************************************************/
void ADC_Group1Config(void)
{
  HT_ADC->LST[0] = (ADC_GROUP1_LIST0 << 0)  | \
                   (ADC_GROUP1_LIST1 << 8)  | \
                   (ADC_GROUP1_LIST2 << 16) | \
                   (ADC_GROUP1_LIST3 << 24);
  HT_ADC->LST[1] = (ADC_GROUP1_LIST4 << 0)  | \
                   (ADC_GROUP1_LIST5 << 8)  | \
                   (ADC_GROUP1_LIST6 << 16) | \
                   (ADC_GROUP1_LIST7 << 24);

  HT_ADC->CR &= ~(0x00070700);
  HT_ADC->CR |= ((u32)(0 - 1) << 16) | ((u32)(ADC_GROUP_LIST_LENGTH - 1) << 8);

//  #if 0
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST0, 0);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST1, 1);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST2, 2);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST3, 3);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST4, 4);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST5, 5);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST6, 6);
//  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP1_LIST7, 7);
//  ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, ADC_GROUP_LIST_LENGTH, 0);
//  #endif
}

/*********************************************************************************************************//**
  * @brief  Configure ADC conversion list as group 2.
  * @retval None
  ***********************************************************************************************************/
/*void ADC_Group2Config(void)
{
  HT_ADC->LST[0] = (ADC_GROUP2_LIST0 << 0)  | \
                   (ADC_GROUP2_LIST1 << 8)  | \
                   (ADC_GROUP2_LIST2 << 16) | \
                   (ADC_GROUP2_LIST3 << 24);
  HT_ADC->LST[1] = (ADC_GROUP2_LIST4 << 0)  | \
                   (ADC_GROUP2_LIST5 << 8)  | \
                   (ADC_GROUP2_LIST6 << 16) | \
                   (ADC_GROUP2_LIST7 << 24);

  HT_ADC->CR &= ~(0x00070700);
  HT_ADC->CR |= ((u32)(0 - 1) << 16) | ((u32)(ADC_GROUP_LIST_LENGTH - 1) << 8);

  #if 0
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST0, 0);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST1, 1);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST2, 2);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST3, 3);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST4, 4);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST5, 5);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST6, 6);
  ADC_RegularChannelConfig(HT_ADC, ADC_GROUP2_LIST7, 7);
  ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, ADC_GROUP_LIST_LENGTH, 0);
  #endif
}*/


///*********************************************************************************************************//**
//  * @brief  ADC group main routine
//  * @retval None
//  ***********************************************************************************************************/
//void ADC_Test_MainRoutine(void)
//{
//  #if (ADC_GROUP_TRIGGER_MODE == ADC_GROUP_TRIGGER_MODE_SW)
//  ADC_Test_MainRoutine_Softwre();
//  #endif
//
//  #if (ADC_GROUP_TRIGGER_MODE == ADC_GROUP_TRIGGER_MODE_TM)
//  ADC_Test_MainRoutine_TM();
//  #endif
//}
//
///*********************************************************************************************************//**
//  * @brief  ADC Group main routine - Software trigger every 2 second and show the result.
//  * @retval None
//  ***********************************************************************************************************/
//void ADC_Test_MainRoutine_Softwre(void)
//{
//  if (uTimeTick500ms >= 4)
//  {
//    uTimeTick500ms = 0;
//
//    ADC_Group_SoftwareStart();
//  }
//
//  if (uADCGroupFinish == ADC_GROUP_FINISH)
//  {
//    uADCGroupFinish = 0;
//    ADC_Test_ShowResult();
//  }
//}
//
///*********************************************************************************************************//**
//  * @brief  ADC Group main routine - Timer trigger every 2 second and show the result.
//  * @retval None
//  ***********************************************************************************************************/
//void ADC_Test_MainRoutine_TM(void)
//{
//  if (uADCGroupFinish == ADC_GROUP_FINISH)
//  {
//    uADCGroupFinish = 0;
//    ADC_Test_ShowResult();
//  }
//}

/*********************************************************************************************************//**
  * @brief  Show ADC Result.
  * @retval None
  ***********************************************************************************************************/
//void ADC_Test_ShowResult(void)
// {
//	uADCData[0] &= 0x00000FFF;
//	uADCData[1] &= 0x00000FFF;
//	uADCData[2] &= 0x00000FFF;
//	uADCData[3] &= 0x00000FFF;
//	uADCData[4] &= 0x00000FFF;
//	uADCData[5] &= 0x00000FFF;
//	uADCData[6] &= 0x00000FFF;
//	uADCData[7] &= 0x00000FFF;
//	uADCData[8] &= 0x00000FFF;
//	uADCData[9] &= 0x00000FFF;
//	uADCData[10] &= 0x00000FFF;
//	uADCData[11] &= 0x00000FFF;
//	uADCData[12] &= 0x00000FFF;
//	uADCData[13] &= 0x00000FFF;
//	uADCData[14] &= 0x00000FFF;
//	uADCData[15] &= 0x00000FFF;
//
//#if (ENABLE_DEBUG_PRINTF == 1)
//	printf("\r\nADC Result\r\n");
//	printf("ADC Data0 = %d\r\n", uADCData[0]);
//	printf("ADC Data1 = %d\r\n", uADCData[1]);
//	printf("ADC Data2 = %d\r\n", uADCData[2]);
//	printf("ADC Data3 = %d\r\n", uADCData[3]);
//	printf("ADC Data4 = %d\r\n", uADCData[4]);
//	printf("ADC Data5 = %d\r\n", uADCData[5]);
//	printf("ADC Data6 = %d\r\n", uADCData[6]);
//	printf("ADC Data7 = %d\r\n", uADCData[7]);
//	printf("ADC Data8 = %d\r\n", uADCData[8]);
//	printf("ADC Data9 = %d\r\n", uADCData[9]);
//	printf("ADC Data10 = %d\r\n", uADCData[10]);
//	printf("ADC Data11 = %d\r\n", uADCData[11]);
//	printf("ADC Data12 = %d\r\n", uADCData[12]);
//	printf("ADC Data13 = %d\r\n", uADCData[13]);
//	printf("ADC Data14 = %d\r\n", uADCData[14]);
//	printf("ADC Data15 = %d\r\n", uADCData[15]);
//#endif
//}

/*********************************************************************************************************//**
  * @brief  ADC Grpup process of Interrupt Handler (CPU reads ADC result).
  * @retval None
  ***********************************************************************************************************/
void ADC_GroupProcess(void)
{

  if (ADC_GetFlagStatus(HT_ADC, ADC_FLAG_CYCLE_EOC))
  {
    ADC_ClearIntPendingBit(HT_ADC, ADC_FLAG_CYCLE_EOC);
      uADCData[0] = HT_ADC->DR[0]&0x00000FFF;
      uADCData[1] = HT_ADC->DR[1]&0x00000FFF;
      uADCData[2] = HT_ADC->DR[2]&0x00000FFF;
      uADCData[3] = HT_ADC->DR[3]&0x00000FFF;
      uADCData[4] = HT_ADC->DR[4]&0x00000FFF;
      uADCData[5] = HT_ADC->DR[5]&0x00000FFF;
      uADCData[6] = HT_ADC->DR[6]&0x00000FFF;
      uADCData[7] = HT_ADC->DR[7]&0x00000FFF;
      uADCGroupFinish=TRUE;
    }
}

/*********************************************************************************************************//**
 * @brief   This function handles ADC interrupt.
 * @retval  None
 ************************************************************************************************************/
void ADC_IRQHandler(void)
{
  ADC_GroupProcess();
}


/*********************************************************************************************************//**
  * @brief  Configure ADC trigger source as software.
  * @retval None
  ***********************************************************************************************************/
void ADC_TriggerConfig_Software(void)
{
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);
}

/*********************************************************************************************************//**
  * @brief  Configure ADC trigger source as Timer.
  * @retval None
  ***********************************************************************************************************/
void ADC_TriggerConfig_TM(void)
{
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIGGER_TM_SOURCE);
  TM_Cmd(HT_GPTM0, ENABLE);
}

//float getBattVolt(void){
//	u16	vDaValue=0;
////	vDaValue=getSignalChlADC(ADC_Channel7_ENABLE);
//	return	(SYSTEM_VCAP*ADC_CONST/vDaValue);
//}
