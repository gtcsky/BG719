/*
 * user_i2c.h
 *
 *  Created on: 2020年4月1日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_I2C_H_
#define USER_INC_USER_I2C_H_

#include "const.h"

#define	SIGMA_VCM		1.25

#define	COMM_TIMEOUT_CONST	10//		10*5MS

//--------------------------------------------------------------------
#define	SIGMA_PWRC		0x00
#define	SIGMA_VCM_ENABLE			0X80
#define	SIGMA_VCM_DISABLE			0X00
//#define	SIGMA_VCM_DISABLE			(~SIGMA_VCM_ENABLE);

#define	SIGMA_PWRC_FLMS_MASK			(0X7F)		//Bit 6~0 D6~D0：特性优化位
#define	SIGMA_PWRC_FLMS_30				(0X28)		//010_1000B：ADCR1[FLMS2~0] = 000B (fADCK = fMCLK/30)
#define	SIGMA_PWRC_FLMS_12				(0X2C)		//010_1100B：ADCR1[FLMS2~0] = 010B (fADCK = fMCLK/12)

//--------------------------------------------------------------------
#define	SIGMA_PGAC0		0x01
#define	SIGMA_VREFGN_MASK	0X60		//Bit 6~5 VGS1~VGS0：REFP/REFN 差分参考电压增益选择位
//#define	SIGMA_VREFGN_STEP1	0X00		//00：VREFGN = 1
//#define	SIGMA_VREFGN_STEP2	0X20		//01：VREFGN = 1/2
//#define	SIGMA_VREFGN_STEP3	0X40		//10：VREFGN = 1/4
typedef enum{
	SIGMA_VREFGN_DIV1=0X00,		//00：VREFGN = 1
	SIGMA_VREFGN_DIV2=0X20,		//01：VREFGN = 1/2
	SIGMA_VREFGN_DIV4=0X40		//10：VREFGN = 1/4
}sigmaVrefGainTypeDef;


#define	SIGMA_ADGN_MASK		0X18		//Bit 4~3 AGS1~AGS0：ADC PGAOP/PGAON 差分输入信号增益选择位
//#define	SIGMA_ADGN_1	0X00			//00：ADGN = 1
//#define	SIGMA_ADGN_2	0X08			//01：ADGN = 2 ( 用于 Gain = 128 = PGAGN × ADGN = 64 × 2 )
typedef enum{
	SIGMA_ADGN_1=0X00,				//00：ADGN = 1
	SIGMA_ADGN_2=0X08				//01：ADGN = 2 ( 用于 Gain = 128 = PGAGN × ADGN = 64 × 2 )
}sigmaAdGainTypeDef;

#define	SIGMA_PGAGN_MASK		0X07		//Bit 2~0 PGS2~PGS0：PGA DI+/DI- 差分通道输入增益选择位
//#define	SIGMA_PGAGN_1			0X00
//#define	SIGMA_PGAGN_2			0X01
//#define	SIGMA_PGAGN_4			0X02
//#define	SIGMA_PGAGN_8			0X03
//#define	SIGMA_PGAGN_16		0X04
//#define	SIGMA_PGAGN_32		0X05
//#define	SIGMA_PGAGN_64		0X06
typedef enum{
	SIGMA_PGAGN_1=0X00,
	SIGMA_PGAGN_2=0X01,
	SIGMA_PGAGN_4=0X02,
	SIGMA_PGAGN_8=0X03,
	SIGMA_PGAGN_16=0X04,
	SIGMA_PGAGN_32=0X05,
	SIGMA_PGAGN_64	=0X06
}sigmaPgGainTypeDef;
//--------------------------------------------------------------------
#define	SIGMA_PGAC1		0x02
#define	SIGMA_INIS_MASK				0X40		//Bit 6 INIS：选择输入端 IN1/IN2 内部连接控制位
#define	SIGMA_INIS_CONNECTED			0X40
#define	SIGMA_INIS_DISCONNECTED		0X00
//#define	SIGMA_INIS_DISCONNECTED		(~SIGMA_INIS_CONNECTED)

#define	SIGMA_INX_MASK				0X30		//Bit 5~4 INX1~INX0：选择输入端 IN1/IN2 以及 PGA 差分输入端 DI+/DI- 连接控制位
#define	SIGMA_INX_METHOD0			0X00		//
#define	SIGMA_INX_METHOD1			0X10		//
#define	SIGMA_INX_METHOD2			0X20		//
#define	SIGMA_INX_METHOD3			0X30		//

#define	SIGMA_DCESET_MASK				0X0E		//Bit 3~1 DCSET2~DCSET0：差分输入信号 PGAOP/PGAON 偏置选择位
#define	SIGMA_DCSET_0					0X00
#define	SIGMA_DCSET_025				0X02		//001：DCSET = +0.25 × ΔVR_I
#define	SIGMA_DCSET_050				0X04
#define	SIGMA_DCSET_075				0X06
#define	SIGMA_DCSET_025N				0X0A		//101：DCSET = -0.25 × ΔVR_I
#define	SIGMA_DCSET_050N				0X0C
#define	SIGMA_DCSET_075N				0X0E		//111：DCSET = -0.75 × ΔVR_I
//--------------------------------------------------------------------
#define	SIGMA_PGACS		0x03
#define	SIGMA_IN2N_SELECT_MASK				0X38		//Bit 5~3 CHSN2~CHSN0：PGA 反相输入端 IN2 选择位
typedef enum{
	SIGMA_IN2N_AN1=0X00,
	SIGMA_IN2N_AN3=0X08,
	SIGMA_IN2N_VDACO=0X20,
	SIGMA_IN2N_AVSS=0X28,
	SIGMA_IN2N_VCM=0X30,
	SIGMA_IN2N_VTSN=0X38
}sigmaIn2nTypeDef;

#define	SIGMA_IN2P_SELECT_MASK				0X07		//Bit 2~0 CHSP2~CHSP0：正相输入端 IN1 选择位
typedef enum{
	SIGMA_IN2P_AN0=0X00,
	SIGMA_IN2P_AN2=0X01,
	SIGMA_IN2P_VDACO=0X04,
	SIGMA_IN2P_VCM=0X06,
	SIGMA_IN2P_VTSP=0X07
}sigmaIn2pTypeDef;
//--------------------------------------------------------------------
#define	SIGMA_ADRL			0x04
//--------------------------------------------------------------------
#define	SIGMA_ADRM		0x05
//--------------------------------------------------------------------
#define	SIGMA_ADRH		0x06
//--------------------------------------------------------------------
#define	SIGMA_ADCR0		0x07
#define	SIGMA_ADRST_MASK		(0X80)	//Bit 7 ADRST：A/D 转换器软件复位控制位
#define	SIGMA_AD_RESET_DISABLE		(0X00)
#define	SIGMA_AD_RESET_ENABLE			(0X80)

#define	SIGMA_ADSLP_MASK		(0X40)	//Bit 6 ADSLP：A/D 转换器休眠模式控制位
#define	SIGMA_AD_SLEEP_DISABLE		(0X00)
#define	SIGMA_AD_SLEEP_ENABLE		(0X40)

#define	SIGMA_ADOFF_MASK		(0X20)	//Bit 5 	A/D 转换器模块电源开 / 关控制位
#define	SIGMA_AD_PW_DISABLE		(0X20)
#define	SIGMA_AD_PW_ENABLE		(0X00)
//#define	SIGMA_AD_PW_ENABLE		(~SIGMA_AD_PW_DISABLE)

#define	SIGMA_ADOR_MASK		(0X1E)	//Bit 4~1	：A/D 转换器过采样率选择位
#define	SIGMA_OSR_32768		(0X00)//(0X00<<1)
#define	SIGMA_OSR_16384		(0X02)//(0X01<<1)
#define	SIGMA_OSR_8192			(0X04)//(0X02<<1)
#define	SIGMA_OSR_4096			(0X06)//(0X03<<1)
#define	SIGMA_OSR_2048			(0X08)//(0X04<<1)
#define	SIGMA_OSR_1024			(0X0A)//(0X05<<1)
#define	SIGMA_OSR_512			(0X0C)//(0X06<<1)
#define	SIGMA_OSR_256			(0X0E)//(0X07<<1)
#define	SIGMA_OSR_128			(0X10)//(0X08<<1)

#define	SIGMA_VREFS_MASK		(0X01)	//Bit 0	A/D 转换器参考电压对选择位
#define	SIGMA_REFE_SELECT_VCM		(0X00)	//Vref=Vcm&AVss
#define	SIGMA_REFE_SELECT_VREF		(0X01)	//Vref=VrefP&VrefN
//--------------------------------------------------------------------
#define	SIGMA_ADCR1		0x08
#define	SIGMA_FLMS_MASK			(0XE0)	//Bit 7~5	A/D 转换器时钟分频比选择
#define	SIGMA_AD_PRESCALE_30		(0X00)	//	prescale=30	000：fADCK = fMCLK/30，N = 30
#define	SIGMA_AD_PRESCALE_12		(0X40)	//	prescale=12	010：fADCK = fMCLK/12，N = 12

#define	SIGMA_VRBUFN_MASK		(0X10)	//Bit 4	A/D 转换器反相参考电压输入 (VRN) 缓存控制位
#define	SIGMA_VRN_BUFF_DISABLE		(0X00)	//0：除能输入缓存，使能旁路功能
#define	SIGMA_VRN_BUFF_ENABLE		(0X10)	//1：使能输入缓存，除能旁路功能

#define	SIGMA_VRBUFP_MASK		(0X08)	//Bit 3	A/D 转换器正相参考电压输入 (VRP) 缓存控制位
#define	SIGMA_VRP_BUFF_DISABLE		(0X00)	//0：除能输入缓存，使能旁路功能
#define	SIGMA_VRP_BUFF_ENABLE			(0X08)	//1：使能输入缓存，除能旁路功能

#define	SIGMA_ADCDL_MASK			(0X04)	//Bit 2	A/D 转换器数据锁存功能控制位
#define	SIGMA_AD_RESULT_LOCK_DISABLE			(0X00)	//0：A/D 转换数据更新
#define	SIGMA_AD_RESULT_LOCK_ENABLE			(0X04)	//1：A/D 转换数据不更新

#define	SIGMA_EOC_MASK			(0X02)	//Bit 1	A/D 转换结束标志
#define	SIGMA_AD_CONVERTING			(0X00)	//0：A/D 转换中
#define	SIGMA_AD_CONVERT_OVER		(0X02)	//1：A/D 转换结束


//--------------------------------------------------------------------
#define	SIGMA_ADCS			0x09
#define	SIGMA_ADCK_MASK			(0X1F)		//ADCK4~ADCK0：24-bit A/D 转换器时钟源 fMCLK 分频率选择位
#define	SIGMA_ADC_CLK				(0X1F)		//11111：fMCLK = fADC_CLK	(4.9152 MHz)
#define	SIGMA_ADC_CLK_DIV2		(0X00)		//00000~11110：fMCLK = fADC_CLK/2/(ADCK[4:0] + 1)
#define	SIGMA_ADC_CLK_DIV4		(0X01)		//00000~11110：fMCLK = fADC_CLK/2/(ADCK[4:0] + 1)
#define	SIGMA_ADC_CLK_DIV6		(0X02)		//00000~11110：fMCLK = fADC_CLK/2/(ADCK[4:0] + 1)
#define	SIGMA_ADC_CLK_DIV8		(0X03)		//00000~11110：fMCLK = fADC_CLK/2/(ADCK[4:0] + 1)
//--------------------------------------------------------------------
#define	SIGMA_ADCTE		0x0A
//--------------------------------------------------------------------
#define	SIGMA_DAH			0x0B
//Bit 7~0 D11~D4：DAC 输出控制码
//--------------------------------------------------------------------
#define	SIGMA_DAL			0x0C
#define	SIGMA_DAL_MASK			(0x0F)
//Bit 3~0 D3~D0：DAC 输出控制码
//注：对该寄存器写值只会写入到影子缓存器中，直到对 DAH 寄存器写值，才会将实际值写入 DAL 寄存器
//--------------------------------------------------------------------
#define	SIGMA_DACC			0x0D
#define	SIGMA_DACEN_MASK			(0x80)		//Bit 7 DACEN：DAC 使能或除能控制位
#define	SIGMA_DACEN_DISABLE			(0x00)
#define	SIGMA_DACEN_ENABLE			(0x80)

#define	SIGMA_DACVRS_MASK			(0x40)		//Bit 6 DACVRS：DAC 参考电压选择位
#define	SIGMA_DACVRS_AVDD			(0x00)		//0：DAC 参考电压来自 AVDD
#define	SIGMA_DACVRS_AVCM			(0x40)		//1：DAC 参考电压来自 VCM
//--------------------------------------------------------------------
#define	SIGMA_SIMC0		0x0E
//--------------------------------------------------------------------
#define	SIGMA_SIMTOC		0x10
//--------------------------------------------------------------------
#define	SIGMA_HIRCC		0x11

#define	SIGMA_HIRCC_OK_MASK		((u8)~0X02)
#define	SIGMA_HIRCC_OK				0X02

#define	SIGMA_HIRCC_MASK			((u8)~0X01)
#define	SIGMA_HIRCC_ENABLE		0X01
#define	SIGMA_HIRCC_DISABLE		0X00



//--------------------------------------------------------------------
typedef enum{
	NonConverting=0,
	VcmCoeConverting=0x01,
	VcmCoeConvertOver=0x01,
	VdacoChConverting=0x02,
	VdacoChConvertOver=0x02,
	An23ChConverting=0x04,
	An23ChConvertOver=0x04,
	An0ChCoverting=0x08,
	An0ChConvertOver=0x08,
	VTSConverting=0x10,
	VTSConvertOver=0x10
}sigmaCovertTypeDef;

typedef struct{
	sigmaIn2nTypeDef			in2N;
	sigmaIn2pTypeDef			in2P;
	sigmaVrefGainTypeDef		VrefGain;
	sigmaAdGainTypeDef		AdGain;
	sigmaPgGainTypeDef		PgGain;
	sigmaCovertTypeDef		convertingFlag;
	sigmaCovertTypeDef		convertOverFlag;
	u8						sigmaAdCovertCnt;
	u32						sigmaResultAcc;
	u16						totalGain;
}sigmaAdTypeDef;

#define	SIGMAL_DEVICE_ADDR	0XD0
#define	EEPROM_SLAVE_ADDR	0XA0

extern	sigmaAdTypeDef	sigmaAdParas;

extern	void		writeEepromData(u16 addr,u8* pBuff,u16 len);
extern	void		readEepromData(u16 addr,u8* pBuff,u16 len);
extern	void		eepromTest(void);

extern	void	 	restartSigmaConvert(void);
extern	void	 	updateAdChlAndStart(sigmaAdTypeDef * pAdParas,sigmaCovertTypeDef newCovType);
extern	void 	restartSigmaAd(sigmaAdTypeDef * pAdParas,sigmaCovertTypeDef newCovType);
extern	void		checkSigmaAdConvertOver(sigmaAdTypeDef * pAdParas);
extern	void 	sigmaAdTest(void);
extern	void	 	readSigmaCtrlRegister(u8 addr,u8* data);
extern	void	 	writeSigmaCtrlRegister(u8 addr,u8* data);
extern	void 	user_I2C_EEPROM_Init(HT_I2C_TypeDef* I2Cx) ;
extern	void 	user_I2C_EEPROM_BufferRead(HT_I2C_TypeDef* I2Cx,u8 slaverAddr,u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
#endif /* USER_INC_USER_I2C_H_ */
