/*
 * user_lcd.c
 *
 *  Created on: 2019年10月11日
 *      Author: Bough_004
 */

#include "user_lcd.h"
#include "user_spi.h"
#include "user_adc.h"
#include "user_lptimer.h"
#include "user_character.h"
#include "user_menu.h"

u8	arrowIndex;

void	delay100us(UINT16 cnt){
	u16	vtSimpleLoop=200;//240;
	u16 i=0,l=0;
	for(;i<cnt;i++){
		for(l=0;l<vtSimpleLoop;l++);
	}
}

void lcdGpioInit(void) {
//	  AFIO_GPxConfig(HTCFG_MASTER_SPI_SCK_GPIO_ID, HTCFG_MASTER_SPI_SCK_AFIO_PIN, AFIO_FUN_SPI);
//	  AFIO_GPxConfig(HTCFG_MASTER_SPI_SEL_GPIO_ID, HTCFG_MASTER_SPI_SEL_AFIO_PIN, AFIO_FUN_SPI);
	//  AFIO_GPxConfig(HTCFG_MASTER_SPI_MISO_GPIO_ID, HTCFG_MASTER_SPI_MISO_AFIO_PIN, AFIO_FUN_SPI);
//	  AFIO_GPxConfig(HTCFG_MASTER_SPI_MOSI_GPIO_ID, HTCFG_MASTER_SPI_MOSI_AFIO_PIN, AFIO_FUN_SPI);
	  set_LcdResPin;
	  set_LcdDcPin;
}

void writeLcdCommand(u8 cmd){
	u8 vtTemp=cmd;
	clr_LcdDcPin;
	WriteData(&vtTemp,1);
	set_LcdDcPin;
}
void writeLcdData(u8 data){
	u8 vtTemp=data;
	set_LcdDcPin;
	//return;
	WriteData(&vtTemp,1);
}
/*********************************************************************
 *
 * 清除所有LCD显示
 *
 **********************************************************************/
void clear(void)
 {
	unsigned char x, y;
	writeLcdCommand(0x00);
	writeLcdCommand(0x10); /*set higher column address*/

	for (y = 0; y < 8; y++) {
		writeLcdCommand(0xB0 + y); /*set page address*/
		writeLcdCommand(0x00);
		writeLcdCommand(0x10);
		for (x = 0; x < 128; x++)
			writeLcdData(0x00);
		writeLcdData(0x00);
	}
}



///*********************************************************************
// *
// * LCD进入关机状态
// *
// **********************************************************************/
//void lcdEnterPowerDownMode(void){
//
//}

/*********************************************************************
 *
 * LCD进入休眠状态
 *
 **********************************************************************/
void  lcdEnterSleep(void){
	writeLcdCommand(0xAE); /*display off*/
	writeLcdCommand(0x8d); /*set charge pump disable*/
	writeLcdCommand(0x10);		//使用外置DC
	delay100us(5);
//	LcdPowerOff;
}
/*********************************************************************
 *
 * LCD退出休眠状态
 *
 **********************************************************************/
void  lcdExitSleep(void){
//	LcdPowerOn;
	vSystem5ms = 0;
	while (vSystem5ms <= LCD_POWER_ON_CONST)		;
	vSystem5ms = 0;
	writeLcdCommand(0xAE); /*display off*/
	writeLcdCommand(0x8d); /*set charge pump disable*/
	writeLcdCommand(0x14);		//使用内置DC
	writeLcdCommand(0xAF); /*display ON*/
	while (vSystem5ms <= LCD_POWER_ON_CONST)		;
	vSystem5ms = 0;
}

/**********
 *
 */
void batterDisplay(FunctionalState stts ) {
	if(ENABLE==stts){
		OLED_ShowCHinese(ICON_batt_X0, ICON_batt_Y, ICON_batt_lv0_Addr);
		OLED_ShowCHinese(ICON_batt_X1, ICON_batt_Y, ICON_batt_lv0_Addr + 1);
	}
	else{
		OLED_ShowString(ICON_batt_X0, ICON_batt_Y, "    ");
	}
}

///*****************************************************************************************
// *
// * 光标显示
// *
// ******************************************************************************************/
void 	updateArrowDisplay(displayParamsStruct * disParams){
		u8 i;
		for(i=0;i<=Max_Arrow_Index;i++){
			if(i==disParams->arrowIndex){
				OLED_ShowChar(ArrowArray[disParams->arrowIndex][0], ArrowArray[disParams->arrowIndex][1], ICON_Arrow_ADDRESS);
			}
			else{
				OLED_ShowChar(ArrowArray[i][0], ArrowArray[i][1], ' ');
			}
		}
}

/*****************************************************************************************
 *
 * 待测目标显示
 *
 ******************************************************************************************/
void	updateTargetCharacter(displayParamsStruct * disParams){
	OLED_ShowString(TARGET_CHARACTER_X+32,TARGET_CHARACTER_Y,"        ");
	if(disParams->targetCharacter%2==0){
		oledChineseString(TARGET_CHARACTER_X, TARGET_CHARACTER_Y, HUMEN_BODY, 2);
	}else  if(disParams->targetCharacter%2==1){
		oledChineseString(TARGET_CHARACTER_X, TARGET_CHARACTER_Y, OBJECT_TEMPERATURE, 4);
	}
}

float userAbs(float f1){
	if(f1>=0)
		return f1;
	else
		return 0-f1;
}
/*****************************************************************************************
 *
 * 显示记忆温度
 *
 ******************************************************************************************/
void updateMemDataDisplay(displayParamsStruct * disParams) {
	u8 vtIndex = 0;
	u8 vtValueX = 0;
	float vtTemperature = 0;
	float vtAbsTemperature = 0;
	if (disParams->memValue != 0xffff) {
		oledChineseString(ICON_Mem_X, ICON_ColorTemp_Y, MEM, 2);									//显示记忆
		//显示组号
		vtIndex = ((disParams->memIndex) % (MAX_TEMPERATURE_INDEX / 2)) + 1;
		OLED_ShowChar(Value_Mem_Index_X-8, Value_Mem_Index_Y,' ');
		if (vtIndex >= 10)
			OLED_ShowChar(Value_Mem_Index_X, Value_Mem_Index_Y, (vtIndex / 10 % 10) + '0');
		else
			OLED_ShowChar(Value_Mem_Index_X, Value_Mem_Index_Y, '0');
		OLED_ShowChar(Value_Mem_Index_X + 8, Value_Mem_Index_Y, (vtIndex % 10) + '0');
		//OLED_ShowChar(Value_Mem_Index_X + 16, Value_Mem_Index_Y,' ');
		OLED_ShowString(Value_Mem_Index_X + 16, Value_Mem_Index_Y, "     ");
		//显示记忆温度
		vtTemperature = calcTemperature(disParams->memValue, disParams->temperatureUnit);
		//	vtTemperature=-938.5;
		vtAbsTemperature = userAbs(vtTemperature);
		OLED_ShowString(Value_Mem_X, Value_Mem_Y, "   ");
		if (vtAbsTemperature >= 100)
			vtValueX = Value_Mem_X;
		else if(vtAbsTemperature >= 10)
			vtValueX = Value_Mem_X + 8;
		else
			vtValueX = Value_Mem_X + 16;
		if (vtTemperature < 0) {
			OLED_ShowChar(vtValueX, Value_Mem_Y, '-');
		}
		displayFloat(vtValueX + 10, Value_Mem_Y, vtAbsTemperature, 1, ICON_Degree_ADDRESS);
		if (disParams->temperatureUnit == Celsius) {
			OLED_ShowChar(Max_Column - 8, Value_Mem_Y, 'C');
		} else {
			OLED_ShowChar(Max_Column - 8, Value_Mem_Y, 'F');
		}
	}
}


/*****************************************************
 *
 * 清空中央显示区
 *
 ******************************************************/
void	clrMainTemperatureDisplay(void){
	u8 i=0;
	while(i<8){
		displayBigChar(i++*16,2,BIG_CHAR_EMPTY);
	}
}

void mainTemperatureDisplay(displayParamsStruct * disParams,FunctionalState stts){
	u8 vtStartAddr=0;
	float	vtTemperature=0;
	float	vtAbsTemperature=0;
	u16 vtIntTemperature=0;
	u8	totalIntLen=0;

	clrMainTemperatureDisplay();
	if(Human == disParams->targetCharacter)
		OLED_ShowString(HotSttsX, HotSttsY, "    ");
	if((DISABLE==stts)||(0xffff==disParams->memValue)){
		displayBigChar(32, 2, BIG_CHAR_NEG);
		displayBigChar(48, 2, BIG_CHAR_NEG);
		displayBigChar(64, 2, BIG_CHAR_NEG);
		displayBigChar(80, 2, BIG_CHAR_NEG);
	}else{
	//有效果数据
//	disParams->temperatureStts=StatusOverflowHi;
	if ((StatusNormal == disParams->temperatureStts) || (StatusHot == disParams->temperatureStts)) {
//	disParams->temperatureUnit=Fahrenheit;
		if(StatusHot== disParams->temperatureStts)
			//OLED_ShowString(HotSttsX, HotSttsY, "HOT");
			oledChineseString(HotSttsX, HotSttsY, SETTING_THRESHOLD, 2);		//发烧
		vtTemperature = calcTemperature(disParams->memValue, disParams->temperatureUnit);
//	vtTemperature=-128.5;
		vtAbsTemperature = userAbs(vtTemperature);
		vtIntTemperature = (u16) vtAbsTemperature;
		if (Human == disParams->targetCharacter) {
			if (Celsius == disParams->temperatureUnit) {
				vtStartAddr = BIT_DOT_COOR;
				totalIntLen = 2;
			} else if (Fahrenheit == disParams->temperatureUnit) {
				if (vtAbsTemperature >= 100) {
					vtStartAddr = BIT_DOT_COOR + 8;
					totalIntLen = 3;
				} else {
					vtStartAddr = BIT_DOT_COOR;
					totalIntLen = 2;
				}
			}
		} else if (Object == disParams->targetCharacter) {
			if (vtAbsTemperature >= 100) {
				vtStartAddr = BIT_DOT_COOR + 8;
				totalIntLen = 3;
			}else if (vtAbsTemperature >= 10) {
				vtStartAddr = BIT_DOT_COOR;
				totalIntLen = 2;
			}else {
				vtStartAddr = BIT_DOT_COOR;
				totalIntLen = 1;
			}
		}
		if (vtTemperature < 0) {
			if (totalIntLen > 2)
				displayBigChar(vtStartAddr - 64, 2, BIG_CHAR_NEG);
			else if(totalIntLen==2)
				displayBigChar(vtStartAddr - 48, 2, BIG_CHAR_NEG);
			else
				displayBigChar(vtStartAddr - 32, 2, BIG_CHAR_NEG);
		}
		if (totalIntLen >= 3) {
			displayBigChar(vtStartAddr - 48, 2, vtIntTemperature / 100);
		}
		if (totalIntLen >= 2) {
			displayBigChar(vtStartAddr - 32, 2, vtIntTemperature / 10 % 10);
		}
		displayBigChar(vtStartAddr - 16, 2, vtIntTemperature % 10);
		displayBigChar(vtStartAddr, 2, BIG_CHAR_DOT);
		displayBigChar(vtStartAddr + 8, 2, (u16) (vtAbsTemperature * 10) % 10);
		displayBigChar(vtStartAddr + 24, 2, BIG_CHAR_DEGREE);
		if (disParams->temperatureUnit == Celsius)
			displayBigChar(vtStartAddr + 32, 2, BIG_CHAR_C);
		else
			displayBigChar(vtStartAddr + 32, 2, BIG_CHAR_F);
	}
	//无效数据
	else{
		displayBigChinese(OverflowX,OverflowY,BIG_CHINESE_OVERFLOW);
		displayBigChinese(OverflowX+32,OverflowY,BIG_CHINESE_OVERFLOW+4);
		//OLED_ShowString(HotSttsX, HotSttsY, "HOT");
	}
	}
}
/************************************************************************
 *
 * 浮点小数显示
 *	x,y:起始坐标
 *	size: 小数点后显示几位
 *	per:显示单位
 */
void displayFloat(UINT8 x,UINT8 y,float fNum,UINT8 size,UINT8 per){
	UINT16 vtInt=0;
	UINT8 len=0,dotPos=0,i=0;
	vtInt=(UINT16)fNum;
//	OLED_ShowString(x,y,"    ");
	len=OLED_ShowNum(x,y,vtInt);
	dotPos=x+8*len;
	if(size){
		OLED_ShowChar(dotPos, y, '.');
		for(i=1;i<=size;i++){
			OLED_ShowChar(dotPos+5+(i-1)*8, y, ((u16) (fNum * oled_pow(10,i))) % 10 + '0');
		}
		OLED_ShowChar(dotPos +5+(size)*8, y, per);
	}else{
		OLED_ShowChar(dotPos , y, per);
		OLED_ShowChar(dotPos+8 , y, ' ');
	}
//	if(size<=2){
//		OLED_ShowString(dotPos +5+(size+1)*8, y, " ");
//	}
}


/************************************************************
 *
 * 显示大字符
 *
 *************************************************************/
void	displayBigChar(u8 x,u8 y,u8 data){
	if(data<=9){
		OLED_ShowCHinese(x, y, 15+2*data);
		OLED_ShowCHinese(x, y+2, 16+2*data);
	}else{
		OLED_ShowCHinese(x, y, data);
		OLED_ShowCHinese(x, y+2, data+1);
	}
}

void  oledDisplayBigChinesePub(u8 x,u8 y,u8 data){
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++) {
		writeLcdData(Hzk[2 * data][t]);
		adder += 1;
	}
	OLED_Set_Pos(x+16, y);
	for (t = 0; t < 16; t++) {
		writeLcdData(Hzk[2 * data + 1][t]);
		adder += 1;
	}
}

/************************************************************
 *
 * 显示大中文
 *
 *************************************************************/
void	displayBigChinese(u8 x,u8 y,u8 data){
	oledDisplayBigChinesePub(x,y,data);
	oledDisplayBigChinesePub(x,y+1,data+1);
	oledDisplayBigChinesePub(x,y+2,data+2);
	oledDisplayBigChinesePub(x,y+3,data+3);
}

/************************************************************************
 *
 * 单位显示
 *
 *************************************************************************/
void	updateUnitDisplay(displayParamsStruct * disParams){
		OLED_ShowChar(SettingUnitValueX,SettingUnitValueY, ICON_Degree_ADDRESS);
	if(Celsius==disParams->temperatureUnit){
//		oledChineseString(SettingUnitValueX, SettingUnitValueY, SETTING_UNIT, 2);		//单位
		OLED_ShowChar(SettingUnitValueX+8,SettingUnitValueY, 'C');
	}else{
		OLED_ShowChar(SettingUnitValueX+8,SettingUnitValueY, 'F');
	}
	OLED_ShowString(SettingUnitValueX + 16, SettingUnitValueY, "   ");
}
/************************************************************************
 *
 * 声效显示
 *
 *************************************************************************/
void	updateVoiceDisplay(displayParamsStruct * disParams){
	if(OFF==disParams->voiceSetting){
		OLED_ShowCHinese(SettingVoiceValueX,SettingVoiceValueY, VALUE_OFF_ADDR);
	}else{
		OLED_ShowCHinese(SettingVoiceValueX,SettingVoiceValueY, VALUE_ON_ADDR);
	}
	OLED_ShowString(SettingVoiceValueX + 16, SettingVoiceValueY, "       ");
}
/************************************************************************
 *
 * 阈值显示
 *
 *************************************************************************/
void	updateThresholdDisplay(displayParamsStruct * disParams){
	float vtTemperature=0;
	u16 vtbasic=0;
	if(disParams->temperatureUnit==Celsius)
		vtbasic=HOT_THRESH_CEL_START;
	else
		vtbasic=HOT_THRESH_FAL_START;
	vtTemperature=((float)(disParams->hotThreshold+vtbasic))/10;
	OLED_ShowChar(SettingThresholdValueX+38, SettingThresholdValueY, ' ');
	displayFloat(SettingThresholdValueX, SettingThresholdValueY, vtTemperature, 1, ICON_Degree_ADDRESS);
}
/************************************************************************
 *
 * 误差补偿显示
 *
 *************************************************************************/
void	updateToleranceDisplay(displayParamsStruct * disParams){
	float vtTemperature=0;
	vtTemperature=((float)(disParams->tolerance-DEFAULT_TOLERANCE))/10;
	if(vtTemperature>=0){
		displayFloat(SettingToleranceValueX, SettingToleranceValueY, vtTemperature, 1, ICON_Degree_ADDRESS);
		OLED_ShowChar(SettingToleranceValueX+30, SettingToleranceValueY, ' ');
		OLED_ShowString(SettingToleranceValueX + 32, SettingToleranceValueY, "     ");
	}else{
		OLED_ShowChar(SettingToleranceValueX, SettingToleranceValueY, '-');
		displayFloat(SettingToleranceValueX+8, SettingToleranceValueY, userAbs(vtTemperature), 1, ICON_Degree_ADDRESS);
		OLED_ShowString(SettingToleranceValueX + 38, SettingToleranceValueY, "     ");
	}
	OLED_ShowChar(Max_Column-8, SettingToleranceValueY, ' ');
}

/*****************************************************************************************
 *
 * 	待机画面显示
 * 	stts:是否显示中央大字符区
 *
 ******************************************************************************************/
void displaySystemMenu(displayParamsStruct * disParams, FunctionalState stts) {
//	clear(); 									/* Clear Screen */
//	disParams->DisplayModeIndex=SettingDisplay;
	if(IdleIamgeDisplay==disParams->DisplayModeIndex){
		mainTemperatureDisplay(disParams, stts);
		updateTargetCharacter(disParams);			// 待测目标显示
		updateMemDataDisplay(disParams);
	}else if(SettingDisplay==disParams->DisplayModeIndex){
//		disParams->arrowIndex=0;
		updateArrowDisplay(disParams);
		oledChineseString(SettingUnitX, SettingUnitY, SETTING_UNIT, 2);		//单位
		OLED_ShowChar(SettingUnitX+32,SettingUnitY, ':');
		updateUnitDisplay(disParams);

		oledChineseString(SettingVoiceX, SettingVoiceY, SETTING_VOICE, 2);		//声音
		OLED_ShowChar(SettingVoiceX+32,SettingVoiceY, ':');
		updateVoiceDisplay(disParams);

		oledChineseString(SettingThresholdX, SettingThresholdY, SETTING_THRESHOLD, 4);		//阈值
		OLED_ShowChar(SettingThresholdX+64,SettingThresholdY, ':');
		updateThresholdDisplay(disParams);

		oledChineseString(SettingToleranceX, SettingToleranceY, SETTING_COMP, 2);			//误差补偿
		OLED_ShowChar(SettingToleranceX+32,SettingToleranceY, ':');
		updateToleranceDisplay(disParams);
	}else if(CalibrateMode==disParams->DisplayModeIndex){
		OLED_ShowString(0,0,"Coe:");
		OLED_ShowString(0,2,"Ra:");
		OLED_ShowString(72,2,"Ta:");
		OLED_ShowString(0,4,"Vo:");
		OLED_ShowString(0,6,"To:");
	}
}
/*********************************************************************
 *
 * LCD上电初始化
 *
 **********************************************************************/

void powerOnLcdInit(void) {
	userSpiInit();
	lcdGpioInit();
	set_LcdResPin;
	delay100us(10);
	clr_LcdResPin;
	delay100us(10);
	set_LcdResPin;
	delay100us(10);
	writeLcdCommand(0xAE); /*display off*/

	writeLcdCommand(0x00); /*set lower column address*/
	writeLcdCommand(0x10); /*set higher column address*/
	writeLcdCommand(0x40); /*set display start line*/

	writeLcdCommand(0xB0); /*set page address*/
	writeLcdCommand(0x81); /*contract control*/
	writeLcdCommand(0xcf); /*128*/

	writeLcdCommand(0xA1); /*set segment remap*/

	writeLcdCommand(0xA6); /*normal / reverse*/

	writeLcdCommand(0xA8); /*multiplex ratio*/
	writeLcdCommand(0x3F); /*duty = 1/64*/

	writeLcdCommand(0xC8); /*Com scan direction*/

	writeLcdCommand(0xD3); /*set display offset*/
	writeLcdCommand(0x00);
	writeLcdCommand(0xD5); /*set osc division*/
	writeLcdCommand(0x80);
	writeLcdCommand(0xD9); /*set pre-charge period*/
	writeLcdCommand(0x22);

	writeLcdCommand(0xDA); /*set COM pins*/
	writeLcdCommand(0x12);

	writeLcdCommand(0xdb); /*set vcomh*/
	writeLcdCommand(0x30);
	writeLcdCommand(0x8d); /*set charge pump disable*/

	//writeLcdCommand(0x10);		//使用外置DC
	writeLcdCommand(0x14);		//使用内置DC

	clear(); /* Clear Screen */
	writeLcdCommand(0xAF); /*display ON*/
	vSystem5ms = 0;
	while (vSystem5ms <= LCD_POWER_ON_CONST);
	vSystem5ms = 0;
	if(CalibrateMode==displayParams.DisplayModeIndex){
		OLED_ShowString(0,0,"Coe:");
		OLED_ShowString(0,2,"Ra:");
		OLED_ShowString(72,2,"Ta:");
		OLED_ShowString(0,4,"Vo:");
		OLED_ShowString(0,6,"To:");
	}else{

//	if(!fIsSystemOff){
		displaySystemMenu(&displayParams,DISABLE);
//	}
	}



}
/************************************************************
 *
 * 定位OLED 显示坐标
 *
 *************************************************************/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	writeLcdCommand(0xb0 + y);
	writeLcdCommand(((x & 0xf0) >> 4) | 0x10);
	writeLcdCommand((x & 0x0f) | 0x01);
}


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OLED_ShowChar(u8 x,u8 y,u8 chr)
{
	unsigned char c=0,i=0;
		c=chr-' ';//得到偏移后的值
		if(x>Max_Column-1){x=0;y=y+2;}
			OLED_Set_Pos(x,y);
			for(i=0;i<8;i++)
			writeLcdData(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			writeLcdData(F8X16[c*16+i+8]);
}
/**********************************************************************
 *
 * 写单条Segment 数据(16条com线)数据
 *
 ***********************************************************************/
//void OLED_ShowOneSegment(u8 x,u8 y,u8  data1,u8 data2){
//	OLED_Set_Pos(x,y);
//	writeLcdData(data1);
//	OLED_Set_Pos(x,y+1);
//	writeLcdData(data2);
//}


//m^n函数
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}

u8 getNumLen(u32 num){
	u8 len=1;
	while (1) {
		if (num < 10)
			return	len;
		else
			num /= 10;
		len++;
	}
}
/***************************************************************
 *
 * 显示数字 并返回数字长度
 *
 ****************************************************************/
u8 OLED_ShowNum(u8 x, u8 y, u32 num) {
	u8 t, temp;
	u8 enshow = 0;
	u8 len = getNumLen(num);
	for (t = 0; t < len; t++) {
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1)) {
			if (temp == 0) {
				OLED_ShowChar(x + 8 * t, y, ' ');
				continue;
			} else
				enshow = 1;
		}
		OLED_ShowChar(x + 8 * t, y, temp + '0');
	}
	return len;
}




//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
 {
	unsigned char j = 0;
	while (chr[j] != '\0') {
		OLED_ShowChar(x, y, chr[j]);
		x += 8;
		if (x > 120) {
			x = 0;
			y += 2;
		}
		j++;
	}
}


//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
 {
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++) {
		writeLcdData(Hzk[2 * no][t]);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++) {
		writeLcdData(Hzk[2 * no + 1][t]);
		adder += 1;
	}
}


void HexDigitDis(u8 x,u8 y,u8 value){
	u8 vtTemp;
	vtTemp=(value>>4)&0x0F;
	if(vtTemp<=9)
		OLED_ShowChar(x, y,vtTemp+'0');
	else
		OLED_ShowChar(x, y,vtTemp+'A'-10);

	vtTemp=value&0x0F;
	if(vtTemp<=9)
		OLED_ShowChar(x+8, y,vtTemp+'0');
	else
		OLED_ShowChar(x+8, y,vtTemp+'A'-10);
}

/*************************************************************
 *
 * 显示中文字符串
 * x,y为起始坐标
 * str::待显示字符串的坐标
 * len:待显示字符串的长度
 *
 ************************************************************/
void	oledChineseString(u8 x,u8 y,const u8 *str,u8 len){
	u8 i=0;
	for(;i<len;i++){
		OLED_ShowCHinese(x+16*i,y,*(str+i));
	}
}


//void  versionDisplay(void){
//	clear();
//	OLED_ShowString(8,2,SOFTWARE_VERSION);
//	OLED_ShowString(8,4,HARDWARE_VERSION);
//}

//void temperatureDisplay(void) {
//	u8 vtTemperature_X = 0;
//	u8 vtTemperature_Y = 0;
//	u8 vtVolt_X = 0;
//	u8 vtVolt_Y = 0;
//	u8 vtCVolt_X = 0;
//	u8 vtCVolt_Y = 0;
//	fIsSystemTempGot=TRUE;
//	if (fIsSystemTempGot) {
//		fIsSystemTempGot = 0;
//		if (IdleIamgeDisplay == displayParams.DisplayModeIndex) {
//			vtTemperature_X = 50;
//			vtTemperature_Y = 0;
////			vtTemperature_X = 76;
////			vtTemperature_Y = 6;
//			vtVolt_X = 76;
//			vtVolt_Y = 2;
//			vtCVolt_X =vtVolt_X ;
//			vtCVolt_Y = 4;
//		} else {
//			vtTemperature_X = 76;
//			vtTemperature_Y =6;
//			vtVolt_X = 76;
//			vtVolt_Y = 2;
//			vtCVolt_X =vtVolt_X ;
//			vtCVolt_Y = 4;
//		}
//
//		displayFloat(vtTemperature_X,vtTemperature_Y,vVap_Volt,3,'V');
//		//---------------------------------------------------------------------
//		displayFloat(vtVolt_X,vtVolt_Y,vAdVoltOfBatt,3,'V');
//		//---------------------------------------------------------------------
//		displayFloat(vtCVolt_X,vtCVolt_Y,vTestCompBatt,3,'V');
//	}
//}

//void	 factoryModeTemperatureDisplay(void){
//	OLED_ShowChar(0, 0,vSystemTemperature/10+'0');
//	OLED_ShowChar(8, 0,vSystemTemperature%10+'0');
//	OLED_ShowChar(16, 0,ICON_Degree_ADDRESS);
//}


void	oledTestModeChineseString(u8 x,u8 y,const u8 *str,u8 len){
	u8 i=0;
	for(;i<len;i++){
		OLED_ShowCHinese(x+16*i,y,*(str+i));
	}
}


//void fullyChargeTest(void){
//	if(fIsSystemOff&&fIsCharging){
//		HexDigitDis(16,2,vChgResultForDisplay);
//		if(!vNearBattFullCnt){
//			OLED_ShowString(16,4,"    ");
//		}else{
//			if(!fIsBattFully)
//				OLED_ShowNum(16,4,(vNearBattFullCnt+vSystem1s)&0xffff);
//		}
//	}
//}

//void	displayFactoryMode(u8	item){
//	switch(item){
//	case Red_Current:
//		clear();
//		oledTestModeChineseString(16+16,4,RED_LED_CURRENT_STRING,4);
//		break;
//	case Green_Current:
//		oledTestModeChineseString(16+16,4,GREEN_LED_CURRENT_STRING,4);
//		break;
//	case Blue_Current:
//		oledTestModeChineseString(16+16,4,BLUE_LED_CURRENT_STRING,4);
//		break;
//	case Cw_Current:
//		oledTestModeChineseString(16+16,4,CW_LED_CURRENT_STRING,4);
//		break;
//	case Mw_Current:
//		oledTestModeChineseString(16+16,4,MW_LED_CURRENT_STRING,4);
//		break;
//	case Standby_Current:
//		clear();
//		oledTestModeChineseString(16+16,4,STANDBY_CURRENT_STRING,4);
//		break;
//	case Sleep_Current:
//		clear();
//		oledTestModeChineseString(16+16,4,SLEEP_CURRENT_STRING,4);
//		break;
//	case Press_Func_key:
//		clear();
//		oledTestModeChineseString(16,2,PRESS_FUNC_KEY,6);
//		oledTestModeChineseString(16,4,TEST,2);
//		oledTestModeChineseString(16+32,4,RED_LED_CURRENT_STRING,4);
//		oledTestModeChineseString(16,6,LEVEL,5);
//		OLED_ShowString(16+5*16,6,":A");
//		break;
//	case Press_Binc_key:
//		clear();
//		oledTestModeChineseString(16,2,PRESS_BDEC_KEY,7);
//		oledTestModeChineseString(16,4,TEST,2);
//		oledTestModeChineseString(16+32,4,STANDBY_CURRENT_STRING,4);
//		oledTestModeChineseString(16,6,LEVEL,5);
//		OLED_ShowString(16+5*16,6,":mA");
//		break;
//	case Press_Dec_key:
//		clear();
//		oledTestModeChineseString(16,2,PRESS_INC_KEY,7);
//		oledTestModeChineseString(16,4,TEST,2);
//		oledTestModeChineseString(16+32,4,SLEEP_CURRENT_STRING,4);
//		oledTestModeChineseString(16,6,LEVEL,5);
//		OLED_ShowString(16+5*16,6,":uA");
//		break;
//	case System_Volt_Display:
//		clear();
//		oledTestModeChineseString(16,2,PRESS_ANY_KEY,6);
//		oledTestModeChineseString(16,4,TEST,2);
//		oledTestModeChineseString(16+32,4,BATT_VOLT,4);
//		break;
//	}
//}
