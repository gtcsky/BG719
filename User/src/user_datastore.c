#include "user_datastore.h"
#include "user_menu.h"
#include "user_i2c.h"
#include "user_ddl.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"

u8	st1[20]={0};
u8	st2[20]={0};


StorageParamsStruct storageParamsStruct;
StorageParamsStruct *pStorageParamsStruct=&storageParamsStruct;


void StoreData2Flash(void){
	memcpy(pStorageParamsStruct,&displayParams,sizeof(StorageParamsStruct)-TIME_STAMP_SIZE-1-2);
	writeEepromData(MENU_SETTING_ADDR,(u8 *)pStorageParamsStruct,sizeof(StorageParamsStruct)-TIME_STAMP_SIZE-1-2);
}

void	StoreNewRecord(displayParamsStruct * disParams){
	memcpy(pStorageParamsStruct,&displayParams,sizeof(StorageParamsStruct)-TIME_STAMP_SIZE-1);
	writeEepromData(MENU_SETTING_ADDR,(u8 *)pStorageParamsStruct,sizeof(StorageParamsStruct)-TIME_STAMP_SIZE-1);
	if(Human==disParams->targetCharacter){
		if(disParams->HumanRecordIndex){
			disParams->memIndex=disParams->HumanRecordIndex-1;
			writeEepromData((disParams->HumanRecordIndex-1)<<4,(u8 *)&disParams->memValue,sizeof(TemperatureRecordStruct));
		}else{
			disParams->memIndex=0;
			writeEepromData(0x0000,(u8 *)&disParams->memValue,sizeof(TemperatureRecordStruct));
		}
	}else if(Object==disParams->targetCharacter){
		if(disParams->ObjectRecordIndex){
			disParams->memIndex=disParams->ObjectRecordIndex-1;
			writeEepromData((HALF_MAX_TEMP_INDEX+disParams->memIndex)<<4,(u8 *)&disParams->memValue,sizeof(TemperatureRecordStruct));
		}else{
			disParams->memIndex=0;
			writeEepromData((HALF_MAX_TEMP_INDEX+disParams->memIndex)<<4,(u8 *)&disParams->memValue,sizeof(TemperatureRecordStruct));
		}
	}
	memcpy(&pStorageParamsStruct->tempInfo,&disParams->memValue,sizeof(TemperatureRecordStruct)-1);
}
void TemperatureRecordInit(void) {
	displayParams.validHumanIndex = 0;
	displayParams.HumanRecordIndex = 0;
	displayParams.validObjectIndex = 0;
	displayParams.ObjectRecordIndex = 0;
	displayParams.targetCharacter = DEFAULT_TARGET;
	displayParams.temperatureUnit = DEFAULT_UINT;
	displayParams.voiceSetting = DEFAULT_VOICE;
	displayParams.hotThreshold = DEFAULT_THRESHOLD;
	displayParams.tolerance = DEFAULT_TOLERANCE;
	displayParams.flashNewFlag = NEW_FLASH_CODE;
	displayParams.memValue = 0xffff;
	DDL_ZERO_STRUCT(displayParams.timeStamp);
//	displayParams.validHumanIndex = 1;
//	displayParams.HumanRecordIndex = 2;
//	displayParams.validObjectIndex = 3;
//	displayParams.ObjectRecordIndex = 4;
//	displayParams.flashNewFlag = NEW_FLASH_CODE;
//	displayParams.targetCharacter = 5;
//	displayParams.temperatureUnit = 6;
//	displayParams.reserve = 0xf5;
//	displayParams.voiceSetting = 7;
//	displayParams.hotThreshold = 0x0809;
//	displayParams.tolerance = 0x0a0b;
//	displayParams.memValue = 0xffff;
//	memcpy(st1,&displayParams,20);
//	DDL_ZERO_STRUCT(displayParams.timeStamp);
	StoreData2Flash();
//	readEepromData(TARGET_ADDR,vtTestArray,20);
	writeEepromData((0) << 4, (u8 *) &displayParams.memValue, sizeof(TemperatureRecordStruct)-1);
	writeEepromData((HALF_MAX_TEMP_INDEX) << 4, (u8 *) &displayParams.memValue, sizeof(TemperatureRecordStruct)-1);
//	StoreNewRecord(&displayParams);
}

void	loadSignalRecord(displayParamsStruct * disParams){
	if(Human==disParams->targetCharacter){
//		if(disParams->HumanRecordIndex){
//			disParams->memIndex=disParams->HumanRecordIndex-1;
//			readEepromData((disParams->HumanRecordIndex-1)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}else{
//			disParams->memIndex=0;
//			readEepromData(0x0000,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}

			readEepromData(disParams->memIndex<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct)-1);
	}else if(Object==disParams->targetCharacter){
//		if(disParams->ObjectRecordIndex){
//			disParams->memIndex=disParams->ObjectRecordIndex-1;
//			readEepromData((HALF_MAX_TEMP_INDEX+disParams->memIndex)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}else{
//			disParams->memIndex=0;
//			readEepromData((HALF_MAX_TEMP_INDEX+disParams->memIndex)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}
			readEepromData((disParams->memIndex+HALF_MAX_TEMP_INDEX)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct)-1);
	}
//	memcpy(&displayParams,pStorageParamsStruct,sizeof(StorageParamsStruct)-1);
	memcpy(&displayParams.memValue,&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct)-1);
}

void	LoadSettingFromEeprom(displayParamsStruct * disParams){
	readEepromData(MENU_SETTING_ADDR,(u8 *)pStorageParamsStruct,sizeof(StorageParamsStruct)-TIME_STAMP_SIZE-1);
	memcpy(&displayParams,pStorageParamsStruct,sizeof(StorageParamsStruct)-TIME_STAMP_SIZE-1-2);
	loadSignalRecord(disParams);
//	if(Human==disParams->targetCharacter){
//		if(disParams->HumanRecordIndex){
//			disParams->memIndex=disParams->HumanRecordIndex-1;
//			readEepromData((disParams->HumanRecordIndex-1)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}else{
//			disParams->memIndex=0;
//			readEepromData(0x0000,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}
//	}else if(Object==disParams->targetCharacter){
//		if(disParams->ObjectRecordIndex){
//			disParams->memIndex=disParams->ObjectRecordIndex-1;
//			readEepromData((HALF_MAX_TEMP_INDEX+disParams->memIndex)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}else{
//			disParams->memIndex=0;
//			readEepromData((HALF_MAX_TEMP_INDEX+disParams->memIndex)<<4,(u8 *)&pStorageParamsStruct->tempInfo,sizeof(TemperatureRecordStruct));
//		}
//	}
//	memcpy(&displayParams,pStorageParamsStruct,sizeof(StorageParamsStruct)-1);
}


int FlashInit(void) {
	LoadSettingFromEeprom(&displayParams);
	if (NEW_FLASH_CODE != displayParams.flashNewFlag) {						//新Flash或者 Flash数据丢失
//		memcpy(st1,&displayParams,20);
		TemperatureRecordInit();											//重新初始化Flash
		LoadSettingFromEeprom(&displayParams);
	}
//	readEepromData(TARGET_ADDR,vtTestArray,20);
//	memcpy(st1,&displayParams,20);
//	memcpy(st2,pStorageParamsStruct,20);
	return PASSED;
}
