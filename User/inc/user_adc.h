/*
 * user_adc.h
 *
 *  Created on: 2020年3月7日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_ADC_H_
#define USER_INC_USER_ADC_H_
#include 	"const.h"
#include 	"adc_group.h"

/* Exported variables --------------------------------------------------------------------------------------*/
extern vu32 uADCGroupState;           // 0: Stop, 1: Group1, 2:Group2
extern u8 uADCGroupFinish;          // 0: Not finish, 2: Finished
extern	u16 uADCData[16];

extern	u16  userAdcChs;

typedef enum{
	UserChannel0=0x01,
	UserChannel1=0x02,
	UserChannel2=0x04,
	UserChannel3=0x08,
	UserChannel4=0x10,
	UserChannel5=0x20,
	UserChannel6=0x40,
	UserChannel7=0x80,
	UserChannel8=0x100,
	UserChannel9=0x200,
	UserChannel10=0x400,
	UserChannel11=0x800,
}userAdcChsEnum;
/* Settings ------------------------------------------------------------------------------------------------*/
#define ENABLE_DEBUG_PRINTF           		(1)
#define ENABLE_DEBUG_IO              		 	(1)

#define ADC_GROUP_TRIGGER_MODE        	(0)   /* 0: Trigger by software,  1: Trigger by Timer                 			*/
#define ADC_GROUP_DATA_MODE           	(0)   /* 0: Move ADC data by CPU, 1: Move ADC data by PDMA            	*/

#define ADC_GROUP_LIST_LENGTH        		(3)   // Same length of two group, the PDMA setting is more easy

#define ADC_GROUP1_LIST0              (ADC_CH_GND_VREF)
#define ADC_GROUP1_LIST1              (ADC_CH_VDD_VREF)
#define ADC_GROUP1_LIST2              (ADC_VCAP_CHL)
#define ADC_GROUP1_LIST3              (0)
//#define ADC_GROUP1_LIST4              (ADC_CH_4)
#define ADC_GROUP1_LIST4              (0)
#define ADC_GROUP1_LIST5              (0)
#define ADC_GROUP1_LIST6              (0)
#define ADC_GROUP1_LIST7              (0)

//#define ADC_GROUP2_LIST0              (ADC_CH_5)
#define ADC_GROUP2_LIST0              (ADC_CH_6)
#define ADC_GROUP2_LIST1              (ADC_CH_7)
#define ADC_GROUP2_LIST2              (ADC_CH_8)
#define ADC_GROUP2_LIST3              (ADC_CH_9)
#define ADC_GROUP2_LIST4              (0)
#define ADC_GROUP2_LIST5              (0)
#define ADC_GROUP2_LIST6              (0)
#define ADC_GROUP2_LIST7              (0)

extern	void ADC_Configuration(u16 chs);
extern	void ADC_GroupProcess(void);
extern	void ADC_IRQHandler(void);
extern	void ADC_Group1Config(void);
extern	void ADC_Test_MainRoutine(void);
extern	void ADC_Group_SoftwareStart(void);
extern	void ADC_Test_ShowResult(void);
extern	void ADC_TriggerConfig_Software(void);



#endif /* USER_INC_USER_ADC_H_ */
