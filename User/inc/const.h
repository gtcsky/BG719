/*
 * const.h
 *
 *  Created on: 2020年3月16日
 *      Author: Bough_004
 */

#ifndef USER_INC_CONST_H_
#define USER_INC_CONST_H_
#include "ht32.h"
#include "ht32_board.h"
#include "ht32_board_config.h"
#include "ht32f5xxxx_gpio.h"

typedef signed char 			i8;
typedef unsigned char 		u8;
typedef unsigned char 		UINT8;
typedef short	 				i16;
typedef unsigned short 		u16;
typedef unsigned short 		UINT16;
typedef int 					i32;
typedef unsigned 				u32;
typedef unsigned 				UINT32;
typedef long long 			i64;
typedef unsigned long long 	u64;
typedef unsigned long long 	UINT64;
//#define TRUE        ((uint8_t) 1u)
//#define FALSE       ((uint8_t) 0u)
typedef void         (*func_ptr_t)(void);

//typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
//#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))
//
//typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define	USER_DOG_CONST		1600				//1600*5ms=8s

#define	NORMAL_MODE_TIMEOUT				600*20			//60s
#define	POWER_DOWN_MODE_TIMEOUT		4				//10*50ms=500ms
//#define	POWER_DONW_LCD_TIMEOUT			60				//3S
#define	POWER_DONW_LCD_TIMEOUT			3				//150ms


#define	ADC_CONST			(4096)
//#define 	vA2D_Coe			(ADC_CONST/SYSTEM_VDD)			//模数转换系数	2.0v*vA2D_Coe=2.0v对应的AD值
//#define 	vD2A_Coe			(SYSTEM_VDD/ADC_CONST)			//数模转换系数	0x0136*vD2A_Coe=0x0136对应的电压
#define	SYSTEM_VCAP		(1.5)


#define	RES_OF_AMBIENT		200		//200kR
#define	Ramb_Max_inedex	120
#define	Ramb_Degree0_Index		20
#define	Ramb_Degree40_Index	60
#define	Ramb_Start_Degree		(-20)



#define	PWM_FREQ_2K			9999//PWM频率=系统频率20M/(vPWMPeriod+1);
#define	PWM_FREQ_1K			19999//PWM频率=系统频率20M/(vPWMPeriod+1);
#define	PWM_FREQ_20K			999//PWM频率=系统频率20M/(vPWMPeriod+1);
#define	PWM_FREQ_10K			1999//PWM频率=系统频率20M/(vPWMPeriod+1);
#define	ADVICE_TIM_PERIOD		PWM_FREQ_2K


typedef enum{
	IdleIamgeDisplay=0,
	SettingDisplay,
	CalibrateMode,
}displayModeEnum;

typedef enum{
	SettingUint=0,
	SettingVoice=1,
	SettingThreshold=2,
	SettingTolerance=3,
}settingIndexEnum;

typedef enum{
	Human=0,
	Object=1
}displayTargetEnum;

typedef enum{
	Celsius=0,		//摄氏
	Fahrenheit=1		//华氏
}displayUnitEnum;

typedef enum{
	StatusNormal=0,			//正常体温
	StatusHot=1,				//发热状态
	StatusOverflowLow=2,		//超过下限
	StatusOverflowHi=3		//超过上限
}TemperatureSttsEnum;

#define	HOT_THRESHOLD_MAX		20
#define	HOT_THRESHOLD_MAX_F		40
#define	HOT_THRESHOLD_MIN		0
#define	HOT_THRESH_CEL_START		370			//37.0F
#define	HOT_THRESH_FAL_START		985			//98.5F
#define	TOLERANCE_MAX				60
#define	TOLERANCE_MIN				0

#define	DEFAULT_ARROR_INDEX		0	//3
#define	DEFAULT_TARGET			Human
#define	DEFAULT_UINT				Celsius
#define	DEFAULT_VOICE				ON
//#define	DEFAULT_THRESHOLD		0X3C8F			//=37.0
#define	DEFAULT_THRESHOLD		0				//
#define	DEFAULT_TOLERANCE			30				//(DEFAULT_TOLERANCE-30)/10=0.0
#define	MAX_CD_TIMER				100
#define	Min_Arrow_Index				0
#define	Max_Arrow_Index				3

#define	MAX_TEMPERATURE_INDEX	128
#define	HALF_MAX_TEMP_INDEX	64
#define	TIME_STAMP_SIZE			7

#define	HUMAN_HI_THRESHOLD		42.0				//上限温度
//#define	HUMAN_HOT_THRESHOLD	37.5				//发烧温度
#define	HUMAN_LOW_THRESHOLD	30.0				//下限温度

#define	OBJECT_HI_THRESHOLD		115
#define	OBJECT_LOW_THRESHOLD		-40

typedef struct{
	UINT8	validHumanIndex;					//	 体温最大有效编号
	UINT8	HumanRecordIndex;				//	 体温最后记录编号
	UINT8	validObjectIndex;					//	 物体温度最大有效编号
	UINT8	ObjectRecordIndex;				//	 物体温度最后记录编号

	UINT16	flashNewFlag;						//	空Flash标志
	displayTargetEnum	targetCharacter;		//	待测量目标性质
	displayUnitEnum	temperatureUnit;			//	温度单位
	UINT8	voiceSetting;						//	声音开关

	UINT8	reserve;
	UINT16	hotThreshold;						//	发烧阈值
	UINT16	tolerance;						//	误差设定		//(tolerance-30)/10 对应误差补偿

	UINT16	memValue;						//	记忆值
	UINT8	timeStamp[TIME_STAMP_SIZE];		//
	UINT8	DisplayModeIndex;				//	参考值: displayModeEnum
	UINT8	arrowIndex;						//	光标序号	(0~4)		参考值:settingIndexEnum
	UINT8	memIndex;						//	当前显示记忆标号
	TemperatureSttsEnum	temperatureStts;		//	温度状态
}displayParamsStruct;





typedef enum{
	userPWM_CH0=1,
	userPWM_CH1=2,
	userPWM_CH2=4,
	userPWM_CH3=8
}userPWMChEnum;


typedef enum {
	OFF = 0,
	ON = 1
} switchSttsEnum;

typedef enum{
	I2C_READ_DATA_ERROT=-2,
	I2C_COMM_ERROR=-1,
	I2C_COMM_OK=0
}returnStatus;

typedef enum
{
	FAILED = 0,
	PASSED = !FAILED
} TestStatus;

typedef struct
{
  u16 period;					//PWM频率=系统频率/(vPWMPeriod+1);
  u8 channels;					//channel可以是	BUZZER_CHANNEL|MOTOR_CHANNEL|DISCHARGE_CHANNEL|CHARGE_CHANNEL以及他们的子集
  float dutyArray[4];			//dutyArray[0~3]存储对应的4个PWM channel的duty
} UserPwmInitTypeDef;
#endif /* USER_INC_CONST_H_ */
