/*
 * user_datastore.h
 *
 *  Created on: 2020年5月7日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_DATASTORE_H_
#define USER_INC_USER_DATASTORE_H_

#include	"const.h"
#define	NEW_FLASH_CODE			0XA5A5


#define	MENU_SETTING_ADDR		(127<<6)				//PAGE 127     (127<<6)|0x00~ (127<<6)|0x1F				32bytes
#define	TARGET_ADDR				(MENU_SETTING_ADDR+6)
#define	UNIT_ADDR					(MENU_SETTING_ADDR+7)
#define	VOICE_ADDR					(MENU_SETTING_ADDR+8)
#define	THRESHOLD_ADDR			(MENU_SETTING_ADDR+10)
#define	TOLERANCE_ADDR			(MENU_SETTING_ADDR+12)
#define	CALIBRATE_DATA_ADDR		((127<<6)|0x20)		//PAGE 127

//every temperature record store as 16bytes in EEPROM .
typedef	struct{
	u16	temperatureValue;
	u8 	timeStamp[TIME_STAMP_SIZE];
}TemperatureRecordStruct;

typedef struct{
	UINT8	validHumanIndex;
	UINT8	HumanRecordIndex;
	UINT8	validObjectIndex;
	UINT8	ObjectRecordIndex;

	UINT16	flashNewFlag;
	UINT8	targetCharacter;
	UINT8	temperatureUnit;
	UINT8	voiceSetting;

	UINT8	reserve;
	UINT16	hotThreshold;
	UINT16	tolerance;
	TemperatureRecordStruct	tempInfo;
}StorageParamsStruct;

extern	StorageParamsStruct storageParamsStruct;
extern	StorageParamsStruct *pStorageParamsStruct;

extern	int FlashInit(void);
extern	void StoreData2Flash(void);
extern	void storeData(u16 storeAddr, u8 len, ...) ;
extern	void	loadSignalRecord(displayParamsStruct * disParams);
extern	void	StoreNewRecord(displayParamsStruct * disParams);
#endif /* USER_INC_USER_DATASTORE_H_ */
