/*
 * user_i2c.c
 *
 *  Created on: 2020年4月1日
 *      Author: Bough_004
 */
#include "user_i2c.h"
#include "i2c_eeprom.h"
#include "user_lptimer.h"
#include "user_menu.h"
#include "string.h"
#include "stdlib.h"

u8 vtTestCnt2=0;

sigmaAdTypeDef	sigmaAdParas={0};

void user_I2C_EEPROM_Init(HT_I2C_TypeDef* I2Cx) {
	I2C_InitTypeDef I2C_InitStructure = { 0 };
	if(I2Cx==HT_I2C1){
		HT32F_DVB_GPxConfig(EEPROM_I2C_SCL_GPIO_ID, EEPROM_I2C_SCL_AFIO_PIN, EEPROM_I2C_SCL_AFIO_MODE);
		HT32F_DVB_GPxConfig(EEPROM_I2C_SDA_GPIO_ID, EEPROM_I2C_SDA_AFIO_PIN, EEPROM_I2C_SDA_AFIO_MODE);
	}else {
		HT32F_DVB_GPxConfig(I2C0_SCL_GPIO_ID, I2C0_SCL_AFIO_PIN, EEPROM_I2C_SCL_AFIO_MODE);
		HT32F_DVB_GPxConfig(I2C0_SDA_GPIO_ID, I2C0_SDA_AFIO_PIN, EEPROM_I2C_SDA_AFIO_MODE);
	}
	/* I2C configuration                                                                                      */
	I2C_InitStructure.I2C_GeneralCall = I2C_GENERALCALL_DISABLE;
	I2C_InitStructure.I2C_AddressingMode = I2C_ADDRESSING_7BIT;
	I2C_InitStructure.I2C_Acknowledge = I2C_ACK_DISABLE;
	I2C_InitStructure.I2C_OwnAddress = 0x00;
	I2C_InitStructure.I2C_Speed = I2C_EEPROM_SPEED;
	I2C_InitStructure.I2C_SpeedOffset = 0;
	I2C_Init(I2Cx, &I2C_InitStructure);

	  I2C_Cmd(I2Cx, ENABLE);
}

/***********************************************************************************************************
  * @brief  EEPROM acknowledge polling.
  * @retval None
  ***********************************************************************************************************/
returnStatus user_I2C_EEPROM_AckPolling(HT_I2C_TypeDef* I2Cx,u8 slaverAddr)
{
  u32 reg;
  u16	vtStartTimeStamp=0;
  vtStartTimeStamp= 0x100000 - vSystemLIRC5ms;

  /* wait if bus busy                                                                                       */
//  while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSBUSY));
    while (I2C_GetFlagStatus(I2Cx, I2C_FLAG_BUSBUSY)){
	    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
		    return	I2C_COMM_ERROR;
	    }
    }
    vtStartTimeStamp=0x100000 - vSystemLIRC5ms;
  while (1)
  {
    /* send slave address                                                                                   */
    I2C_TargetAddressConfig(I2Cx, slaverAddr, I2C_MASTER_WRITE);

    /* check status                                                                                         */
    while (1)
    {
      reg = I2Cx->SR;

	    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
		    return	I2C_COMM_ERROR;
	    }

      if (reg & I2C_FLAG_ADRS)
      {
        return	I2C_COMM_OK;
      }

      if (reg & I2C_FLAG_RXNACK)
      {
        I2C_ClearFlag(I2Cx, I2C_FLAG_RXNACK);
        break;
      }
    }
  }
}

/***********************************************************************************************************
  * @brief  Read numbers of sequential data from EEPROM.
  * @param  pBuffer : a pointer pointing to data destination buffer.
  * @param  ReadAddr : source EEPROM address.
  * @param  NumByteToRead : numbers of byte.
  * @retval None
  ***********************************************************************************************************/
void user_I2C_EEPROM_BufferRead(HT_I2C_TypeDef* I2Cx,u8 slaverAddr,u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{
  /* check parameters                                                                                       	*/
  Assert_Param(NumByteToRead > 0);

  /* acknowledge polling                                                                                    	*/
//  _I2C_EEPROM_AckPolling();
  user_I2C_EEPROM_AckPolling(I2Cx,(slaverAddr>>1));

  /* set EEPROM address                                                                                     	*/
  while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY));
  I2C_SendData(I2Cx, ReadAddr);

  /* send read command                                                                                      	*/
//  I2C_TargetAddressConfig(I2Cx, I2C_EEPROM_DEV_ADDR, I2C_MASTER_READ);
  I2C_TargetAddressConfig(I2Cx, (slaverAddr>>1), I2C_MASTER_READ);
  while (!I2C_CheckStatus(I2Cx, I2C_MASTER_SEND_START));
  while (!I2C_CheckStatus(I2Cx, I2C_MASTER_RECEIVER_MODE));

  /* enable master receiver ACK                                                                             */
  if (NumByteToRead > 1)
  {
    I2C_AckCmd(I2Cx, ENABLE);
  }

  /* sequential read                                                                                        */
  while (NumByteToRead)
  {
    while (!I2C_CheckStatus(I2Cx, I2C_MASTER_RX_NOT_EMPTY));
    *pBuffer = I2C_ReceiveData(I2Cx);

    pBuffer++;
    NumByteToRead--;

    if (NumByteToRead == 1)
    {
      I2C_AckCmd(I2Cx, DISABLE);
    }
  }

  /* end of read                                                                                            */
  I2C_GenerateSTOP(I2Cx);
}

/***********************************************************************************************************
  * @brief  EEPROM page write.
  * @param  pBuffer : a pointer pointing to data source buffer.
  * @param  WriteAddr : destination EEPROM address.
  * @param  NumByteToWrite : numbers of byte.
  * @retval None
  ***********************************************************************************************************/
void user_I2C_EEPROM_PageWrite(HT_I2C_TypeDef* I2Cx,u8 slaverAddr,u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  /* check parameters                                                                                       */
  Assert_Param((NumByteToWrite > 0) && ((NumByteToWrite <= (I2C_EEPROM_PAGE_SIZE - (WriteAddr & I2C_EEPROM_PAGE_MASK)))));

  /* acknowledge polling                                                                                    */
  user_I2C_EEPROM_AckPolling(I2Cx,(slaverAddr>>1));

  /* set EEPROM address                                                                                     */
  while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY));
  I2C_SendData(I2Cx, WriteAddr);

  /* page write                                                                                             		*/
  while (NumByteToWrite--)
  {
    while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY));
    I2C_SendData(I2Cx, *pBuffer);
    pBuffer++;
  }

  /* end of write                                                                                           		*/
  while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY));
  I2C_GenerateSTOP(I2Cx);
}


void genStopByteManual(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;
	GPIO_InitStruct.GPIO_Pin = I2C0_SCL_AFIO_PIN;
	GPIO_InitStruct.GPIO_Af_Port_ID = I2C0_SCL_GPIO_ID;
	GPIO_Init(HT_GPIOB, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = I2C0_SDA_AFIO_PIN;
	GPIO_InitStruct.GPIO_Af_Port_ID = I2C0_SDA_GPIO_ID;
	GPIO_Init(HT_GPIOB, &GPIO_InitStruct);
	GPIO_WriteOutBits(I2C0_SCL_GPIO, I2C0_SCL_AFIO_PIN, SET);
	GPIO_WriteOutBits(I2C0_SDA_GPIO, I2C0_SDA_AFIO_PIN, SET);
	user_I2C_EEPROM_Init(HT_I2C0);
}

returnStatus checkStts( HT_I2C_TypeDef* I2Cx,I2C_Result_Stts stts) {
//	returnStatus result = I2C_COMM_ERROR;
	u16 vtStartTimeStamp = 0;
	vtStartTimeStamp = 0x100000 - vSystemLIRC5ms;
	while (!I2C_CheckStatus(I2Cx, stts)){
		    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
			    I2C_GenerateSTOP(I2Cx);
			    genStopByteManual();
			    return	I2C_COMM_ERROR;
		    }
	}
	return I2C_COMM_OK;
}
/***********************************************************************************************************
  * @brief  EEPROM page write.
  * @param  pBuffer : a pointer pointing to data source buffer.
  * @param  WriteAddr : destination EEPROM address.
  * @param  NumByteToWrite : numbers of byte.
  * @retval None
  ***********************************************************************************************************/
returnStatus user_EEPROM_24C256_PageWrite(HT_I2C_TypeDef* I2Cx,u8 slaverAddr,u8* pBuffer, u8 WriteHiAddr,u8 WriteLowAddr, u16 NumByteToWrite)
 {

	returnStatus result = I2C_COMM_ERROR;
	/* check parameters                                                                                       */
	Assert_Param( (NumByteToWrite > 0) && ((NumByteToWrite <= (I2C_EEPROM_PAGE_SIZE - (WriteLowAddr & I2C_EEPROM_PAGE_MASK)))));

	/* acknowledge polling                                                                                    */
	result = user_I2C_EEPROM_AckPolling(I2Cx, (slaverAddr >> 1));
	if (result) {
		I2C_GenerateSTOP(I2Cx);
		return result;
	}
	/* set EEPROM address                                                                                     */
//	  vtStartTimeStamp= 0x100000 - vSystemLIRC5ms;
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY)){
//		    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
//			    I2C_GenerateSTOP(I2Cx);
//			    return	I2C_COMM_ERROR;
//		    }
//	}
	result = checkStts(I2Cx, I2C_MASTER_TX_EMPTY);
	if (result)
		return result;
	I2C_SendData(I2Cx, WriteHiAddr);
	/* set EEPROM address                                                                                     */
//	  vtStartTimeStamp= 0x100000 - vSystemLIRC5ms;
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY)){
//		    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
//			    I2C_GenerateSTOP(I2Cx);
//			    return	I2C_COMM_ERROR;
//		    }
//	}
	result = checkStts(I2Cx, I2C_MASTER_TX_EMPTY);
	if (result)
		return result;
	I2C_SendData(I2Cx, WriteLowAddr);
	/* page write                                                                                             		*/
//	  vtStartTimeStamp= 0x100000 - vSystemLIRC5ms;
	while (NumByteToWrite--) {
//		while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY)){
//		    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
//			    I2C_GenerateSTOP(I2Cx);
//			    return	I2C_COMM_ERROR;
//		    }
//		}
		result = checkStts(I2Cx, I2C_MASTER_TX_EMPTY);
		if (result)
			return result;
		I2C_SendData(I2Cx, *pBuffer);
		pBuffer++;
	}

	/* end of write                                                                                           		*/
//	  vtStartTimeStamp= 0x100000 - vSystemLIRC5ms;
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY)){
//		    if (((vtStartTimeStamp+vSystemLIRC5ms)&0xffff) > 10){
//			    I2C_GenerateSTOP(I2Cx);
//			    return	I2C_COMM_ERROR;
//		    }
//	}
	result = checkStts(I2Cx, I2C_MASTER_TX_EMPTY);
	if (result)
		return result;
	I2C_GenerateSTOP(I2Cx);
	return I2C_COMM_OK;
}
/***********************************************************************************************************
  * @brief  Read numbers of sequential data from EEPROM.
  * @param  pBuffer : a pointer pointing to data destination buffer.
  * @param  ReadAddr : source EEPROM address.
  * @param  NumByteToRead : numbers of byte.
  * @retval None
  ***********************************************************************************************************/
returnStatus user_EEPROM_24C256_BufferRead(HT_I2C_TypeDef* I2Cx,u8 slaverAddr,u8* pBuffer,u8 ReadHiAddr, u8 ReadLowAddr, u16 NumByteToRead)
 {
	returnStatus result = I2C_COMM_ERROR;
	/* check parameters                                                                                       	*/
	Assert_Param(NumByteToRead > 0);
	/* acknowledge polling                                                                                    	*/
//  _I2C_EEPROM_AckPolling();
	result = user_I2C_EEPROM_AckPolling(I2Cx, (slaverAddr >> 1));
	if (result) {
		I2C_GenerateSTOP(I2Cx);
		genStopByteManual();
		return result;
	}
	/* set EEPROM address                                                                                     	*/
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY)) {
//		if (((vtStartTimeStamp + vSystemLIRC5ms) & 0xffff) > 10) {
//			I2C_GenerateSTOP(I2Cx);
//			return I2C_COMM_ERROR;
//		}
//	}
	result = checkStts(I2Cx, I2C_MASTER_TX_EMPTY);
	if (result)
		return result;
	I2C_SendData(I2Cx, ReadHiAddr);
//	vtStartTimeStamp = 0x100000 - vSystemLIRC5ms;
	/* set EEPROM address                                                                                     	*/
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_TX_EMPTY)) {
//		if (((vtStartTimeStamp + vSystemLIRC5ms) & 0xffff) > 10) {
//			I2C_GenerateSTOP(I2Cx);
//			return I2C_COMM_ERROR;
//		}
//	}
	result = checkStts(I2Cx, I2C_MASTER_TX_EMPTY);
	if (result)
		return result;
	I2C_SendData(I2Cx, ReadLowAddr);

	/* send read command                                                                                      	*/
//  I2C_TargetAddressConfig(I2Cx, I2C_EEPROM_DEV_ADDR, I2C_MASTER_READ);
	I2C_TargetAddressConfig(I2Cx, (slaverAddr >> 1), I2C_MASTER_READ);
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_SEND_START)) {
//		if (((vtStartTimeStamp + vSystemLIRC5ms) & 0xffff) > 10) {
//			I2C_GenerateSTOP(I2Cx);
//			return I2C_COMM_ERROR;
//		}
//	}
	result = checkStts(I2Cx, I2C_MASTER_SEND_START);
	if (result)
		return result;
//	while (!I2C_CheckStatus(I2Cx, I2C_MASTER_RECEIVER_MODE)) {
//		if (((vtStartTimeStamp + vSystemLIRC5ms) & 0xffff) > 10) {
//			I2C_GenerateSTOP(I2Cx);
//			return I2C_COMM_ERROR;
//		}
//	}
	result = checkStts(I2Cx, I2C_MASTER_RECEIVER_MODE);
	if (result)
		return result;
	/* enable master receiver ACK                                                                             */
	if (NumByteToRead > 1) {
		I2C_AckCmd(I2Cx, ENABLE);
	}

	/* sequential read                                                                                        */
	while (NumByteToRead) {
//		while (!I2C_CheckStatus(I2Cx, I2C_MASTER_RX_NOT_EMPTY)) {
//			if (((vtStartTimeStamp + vSystemLIRC5ms) & 0xffff)
//					> 10) {
//				I2C_GenerateSTOP(I2Cx);
//				return I2C_COMM_ERROR;
//			}
//		}
		result = checkStts(I2Cx, I2C_MASTER_RX_NOT_EMPTY);
		if (result)
			return result;
		*pBuffer = I2C_ReceiveData(I2Cx);

		pBuffer++;
		NumByteToRead--;

		if (NumByteToRead == 1) {
			I2C_AckCmd(I2Cx, DISABLE);
		}
	}

	/* end of read                                                                                            */
	I2C_GenerateSTOP(I2Cx);
	return I2C_COMM_OK;
}

/***********************************************************************************************************
  * @brief  Write numbers of sequential data to EEPROM.
  * @param  pBuffer : a pointer pointing to data source buffer.
  * @param  WriteAddr : destination EEPROM address.
  * @param  NumByteToWrite : numbers of byte.
  * @retval None
  ***********************************************************************************************************/
void user_I2C_EEPROM_BufferWrite(HT_I2C_TypeDef* I2Cx,u8 slaverAddr,u8* pBuffer, u8 WriteAddr, u16 NumByteToWrite)
{
  u16 Byte2Wr, AvailableByte;

  /* check parameters                                                                                       		*/
  Assert_Param(NumByteToWrite <= I2C_EEPROM_CAPACITY);

  /* sequential write                                                                                       			*/
  while (NumByteToWrite)
  {
    AvailableByte = I2C_EEPROM_PAGE_SIZE - (WriteAddr & I2C_EEPROM_PAGE_MASK);
    Byte2Wr = (NumByteToWrite > AvailableByte)?(AvailableByte):(NumByteToWrite);
    user_I2C_EEPROM_PageWrite(I2Cx,slaverAddr,pBuffer, WriteAddr, Byte2Wr);
    pBuffer += Byte2Wr;
    WriteAddr = (WriteAddr + Byte2Wr) & I2C_EEPROM_ADDR_MASK;
    NumByteToWrite -= Byte2Wr;
  }
}

void	 	readSigmaCtrlRegister(u8 addr,u8* data){
//	u8  readTemp=0;
		user_I2C_EEPROM_BufferRead(HT_I2C0,SIGMAL_DEVICE_ADDR,data,addr,1);

}

void		writeSigmaCtrlRegister(u8 addr,u8* data){

		user_I2C_EEPROM_BufferWrite(HT_I2C0,SIGMAL_DEVICE_ADDR,data,addr,1);

}

//void		readEepromData(u8 addr,u8 *data, u16 len){
//
//		user_I2C_EEPROM_BufferRead(HT_I2C0,EEPROM_SLAVE_ADDR,data,addr,len);
//
//}
//void		writeEepromData(u8 addr,u8 *data, u16 len){
//
//		user_I2C_EEPROM_BufferWrite(HT_I2C0,EEPROM_SLAVE_ADDR,data,addr,len);
//
//}


/******************************************************************************************************
 *
 *	EEPROM: 24C256
 *		512 pages ,64Bytes/page
 *		MAX high address:127
 *		128*256Bytes
 *
 *******************************************************************************************************/
void		writeEepromData(u16 addr,u8* pBuff,u16 len){
		returnStatus result=I2C_COMM_ERROR;
		u8 times=0;
		do{
			result=user_EEPROM_24C256_PageWrite(HT_I2C0,EEPROM_SLAVE_ADDR,pBuff,(addr>>8)&0xff,addr&0xff,len);
		}while(result&&++times<3);
}
/******************************************************************************************************
 *
 *	EEPROM: 24C256
 *		512 pages ,64Bytes/page
 *		MAX high address:127
 *		128*256Bytes
 *
 *******************************************************************************************************/
void		readEepromData(u16 addr,u8* pBuff,u16 len){
		returnStatus result=I2C_COMM_ERROR;
		u8 times=0;
		do{
			result=user_EEPROM_24C256_BufferRead(HT_I2C0,EEPROM_SLAVE_ADDR,pBuff,(addr>>8)&0xff,addr&0xff,len);
		}while(result&&++times<3);
}


void eepromTest(void) {
	u8* vtString = (u8*) calloc(64, sizeof(char));
//	u8 * st = "127-0xC0:0123456789987654321000112233445566778899000111222333444555666777888999";
//	memcpy(vtString, st, strlen(st));
//	writeEepromData(127,0xC0,vtString,64);
//	free(vtString);
//	readEepromData(127,0x00,vtString,64);
//	readEepromData(127,0x40,vtString,64);
//	readEepromData(127,0x80,vtString,64);
//	readEepromData(127,0xC0,vtString,64);

}
void	 restartSigmaConvert(void){
	u8 	 vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADCDL_MASK|SIGMA_EOC_MASK);
	writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);

//	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADRST_MASK);
	vtReadTemp|=(SIGMA_AD_RESET_DISABLE);							//start converting
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
}

void	stopConvert(void){
	u8 vtReadTemp = 0;

	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADRST_MASK);
	vtReadTemp|=(SIGMA_AD_RESET_ENABLE);
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);

	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADCDL_MASK|SIGMA_EOC_MASK);
	writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
}


void	checkSigmaAdConvertOver(sigmaAdTypeDef * pAdParas){
	u8 vtReadTemp = 0;//,vtPWRC=0,vtPGAC0=0,vtPGAC1=0,vtPGACS=0,vtADCR0=0,vtADCR1=0,vtADCS=0;
//	u8 vtADCTE = 0,vtDAH=0,vtDAL=0,vtDACC=0,vtSIMC0=0,vtSIMTOC=0,vtHIRCC=0;
//	u8 vtTemp = 0;
	u32  vtSigmaAdResult=0;
	double vtVolt=0;
//	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
//	if(vtReadTemp&SIGMA_EOC_MASK){
	if(fIsSigmaDataReady){
	//	while(vSystem5ms<200);
		fIsSigmaDataReady=FALSE;
		readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
		vtReadTemp|=SIGMA_AD_RESULT_LOCK_ENABLE;
		writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
		readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);


		readSigmaCtrlRegister(SIGMA_ADRH, &vtReadTemp);
		vtSigmaAdResult=vtReadTemp<<16;
		readSigmaCtrlRegister(SIGMA_ADRM, &vtReadTemp);
		vtSigmaAdResult|=vtReadTemp<<8;
		readSigmaCtrlRegister(SIGMA_ADRL, &vtReadTemp);
		vtSigmaAdResult|=vtReadTemp;

		readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
		vtReadTemp&=~(SIGMA_EOC_MASK|SIGMA_ADCDL_MASK);
		writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
		readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	//	vtVolt*=(SIGMA_VCM);

//		sigmaAdParas.sigmaResultAcc=0;
//		sigmaAdParas.sigmaAdCovertCnt=0;
		if(++pAdParas->sigmaAdCovertCnt>3){
			pAdParas->sigmaResultAcc+=vtSigmaAdResult;
			if(pAdParas->sigmaAdCovertCnt==6){
				vtSigmaAdResult=pAdParas->sigmaResultAcc/3;

				if(vtSigmaAdResult&0x00800000){
					vtVolt=(float)(0x01000000-vtSigmaAdResult)/8838608;
					vtVolt*=-1;
				}else{
					vtVolt=(float)vtSigmaAdResult/8838608;
				}

				switch(pAdParas->convertingFlag){
				case VcmCoeConverting:
					vVcm_coe=vtVolt;
					break;
				case VdacoChConverting:
					vtVolt*=(SIGMA_VCM*1000/vVcm_coe);
					vDacOut=(float)vtVolt;
					break;
				case An23ChConverting:
					vtVolt*=(SIGMA_VCM/vVcm_coe);
					vtVolt=vtVolt*1000/pAdParas->totalGain;					// Units :mV
					vVoltOfObj=vtVolt;
					break;
				case VTSConverting:
					vtVolt*=(SIGMA_VCM/vVcm_coe);
					vtVolt=vtVolt*1000/pAdParas->totalGain;					// Units :mV
					vVts=vtVolt;
					break;
				case An0ChCoverting:
					vtVolt*=(SIGMA_VCM/vVcm_coe);
//					vtVolt+=SIGMA_VCM;
					vtVolt=0-vtVolt;
//					vResAmbient=(250-200*vtVolt)/vtVolt;
					vResAmbient=(250-RES_OF_AMBIENT*vtVolt)/vtVolt;
					vResAmbient*=vResCoe;
					break;
				}
				pAdParas->convertOverFlag=pAdParas->convertingFlag;
				stopConvert();
				pAdParas->convertingFlag=NonConverting;
				}
		}

		if(vtTestCnt2++>=2){
			vtTestCnt2=0;
		}
//		if(!vtSigmaAdResult){
//			vtReadTemp=0;
//		}else{
//			vtReadTemp=1;
//		}
	}
}

void	startSigmaAdConvert(void){
	u8 vtReadTemp = 0;
	//0
	readSigmaCtrlRegister(SIGMA_HIRCC, &vtReadTemp);
	vtReadTemp|=SIGMA_HIRCC_ENABLE;
	writeSigmaCtrlRegister(SIGMA_HIRCC, &vtReadTemp);
	do{
		vtReadTemp=0;
		readSigmaCtrlRegister(SIGMA_HIRCC, &vtReadTemp);
	}while(!(vtReadTemp&SIGMA_HIRCC_OK));

	vtReadTemp=0x00;
	writeSigmaCtrlRegister(SIGMA_DAH, &vtReadTemp);
	readSigmaCtrlRegister(SIGMA_DAH, &vtReadTemp);

	vtReadTemp=0xC0;
	writeSigmaCtrlRegister(SIGMA_DACC, &vtReadTemp);
	readSigmaCtrlRegister(SIGMA_DACC, &vtReadTemp);

	//1
	readSigmaCtrlRegister(SIGMA_PWRC, &vtReadTemp);
	vtReadTemp |= SIGMA_VCM_ENABLE;
	writeSigmaCtrlRegister(SIGMA_PWRC, &vtReadTemp);
	vtReadTemp = 0;
	readSigmaCtrlRegister(SIGMA_PWRC, &vtReadTemp);
	vSystem5ms=0;
	while(vSystem5ms<5);
	//2
	readSigmaCtrlRegister(SIGMA_PGAC0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_VREFGN_MASK|SIGMA_ADGN_MASK|SIGMA_PGAGN_MASK);
	vtReadTemp|=(SIGMA_VREFGN_DIV1|SIGMA_ADGN_1|SIGMA_PGAGN_1);
	writeSigmaCtrlRegister(SIGMA_PGAC0, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_PGAC0, &vtReadTemp);

	//3
	readSigmaCtrlRegister(SIGMA_PGAC1, &vtReadTemp);
	vtReadTemp&=~(SIGMA_INIS_MASK|SIGMA_INX_MASK|SIGMA_DCESET_MASK);
	vtReadTemp|=(SIGMA_INX_METHOD0|SIGMA_DCSET_0);
	writeSigmaCtrlRegister(SIGMA_PGAC1, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_PGAC1, &vtReadTemp);

	//4
	readSigmaCtrlRegister(SIGMA_ADCS, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADCK_MASK);
	vtReadTemp|=SIGMA_ADC_CLK;
	writeSigmaCtrlRegister(SIGMA_ADCS, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCS, &vtReadTemp);

	//5
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADOR_MASK|SIGMA_VREFS_MASK);
	vtReadTemp|=(SIGMA_OSR_8192|SIGMA_REFE_SELECT_VCM);
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);

	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp&=~(SIGMA_FLMS_MASK);
	vtReadTemp|=SIGMA_AD_PRESCALE_30;
//	vtReadTemp|=SIGMA_AD_PRESCALE_12;
	writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);

	//6
	readSigmaCtrlRegister(SIGMA_PGACS, &vtReadTemp);
	vtReadTemp&=~(SIGMA_IN2N_SELECT_MASK|SIGMA_IN2P_SELECT_MASK);
//	vtReadTemp|=(SIGMA_IN2N_AN3|SIGMA_IN2P_AN2);
	vtReadTemp|=(SIGMA_IN2N_AVSS|SIGMA_IN2P_VCM);
	writeSigmaCtrlRegister(SIGMA_PGACS, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_PGACS, &vtReadTemp);
	//7
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADSLP_MASK|SIGMA_ADOFF_MASK);
	vtReadTemp|=(SIGMA_AD_SLEEP_DISABLE|SIGMA_AD_PW_ENABLE);
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	//8
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
//	vtTemp=~(SIGMA_ADRST_MASK);
	vtReadTemp&=~(SIGMA_ADRST_MASK);
//	vtReadTemp&=vtTemp;
	vtReadTemp|=(SIGMA_AD_RESET_ENABLE);
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);

	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADCDL_MASK|SIGMA_EOC_MASK);
	writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
	vtReadTemp=0;
	readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);

	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADRST_MASK);
	vtReadTemp|=(SIGMA_AD_RESET_DISABLE);							//start converting
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
//	vtReadTemp=0;
//	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);

	//-------
//	readSigmaCtrlRegister(SIGMA_PWRC, &vtPWRC);
//	readSigmaCtrlRegister(SIGMA_PGAC0, &vtPGAC0);
//	readSigmaCtrlRegister(SIGMA_PGAC1, &vtPGAC1);
//	readSigmaCtrlRegister(SIGMA_PGACS, &vtPGACS);
//	readSigmaCtrlRegister(SIGMA_ADCR0, &vtADCR0);
//	readSigmaCtrlRegister(SIGMA_ADCR1, &vtADCR1);
//	readSigmaCtrlRegister(SIGMA_ADCS, &vtADCS);
//	readSigmaCtrlRegister(SIGMA_HIRCC, &vtHIRCC);

//	readSigmaCtrlRegister(SIGMA_ADRH, &vtReadTemp);
//	vtSigmaAdResult=vtReadTemp<<16;
//	readSigmaCtrlRegister(SIGMA_ADRM, &vtReadTemp);
//	vtSigmaAdResult|=vtReadTemp<<8;
//	readSigmaCtrlRegister(SIGMA_ADRL, &vtReadTemp);
//	vtSigmaAdResult|=vtReadTemp;
//	vtReadTemp=0;

	//-------
	sigmaAdParas.convertingFlag=VcmCoeConverting;
	sigmaAdParas.convertOverFlag=NonConverting;
	sigmaAdParas.sigmaResultAcc=0;
	sigmaAdParas.sigmaAdCovertCnt=0;
	sigmaAdParas.totalGain=1;
	checkSigmaAdConvertOver(&sigmaAdParas);
}

void restartSigmaAd(sigmaAdTypeDef * pAdParas,sigmaCovertTypeDef newCovType){
	u8 vtReadTemp = 0;
	sigmaAdParas.convertingFlag=newCovType;
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADRST_MASK);
	vtReadTemp|=(SIGMA_AD_RESET_ENABLE);
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp&=~(SIGMA_ADRST_MASK);
	vtReadTemp|=(SIGMA_AD_RESET_DISABLE);							//start converting
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	pAdParas->sigmaResultAcc=0;
	pAdParas->sigmaAdCovertCnt=0;
}


void	 updateAdChlAndStart(sigmaAdTypeDef * pAdParas,sigmaCovertTypeDef newCovType){
	u8 vtReadTemp = 0;
//	if(!pAdParas->convertingFlag||!pAdParas->convertOverFlag){
	if(!pAdParas->convertingFlag){
		sigmaAdParas.convertingFlag=newCovType;
		readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
		vtReadTemp&=~(SIGMA_ADRST_MASK);
		vtReadTemp|=(SIGMA_AD_RESET_ENABLE);
		writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
		readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);

		readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
		vtReadTemp&=~(SIGMA_ADCDL_MASK|SIGMA_EOC_MASK);
		writeSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);
		readSigmaCtrlRegister(SIGMA_ADCR1, &vtReadTemp);

		readSigmaCtrlRegister(SIGMA_PGAC0, &vtReadTemp);
		vtReadTemp&=~(SIGMA_VREFGN_MASK|SIGMA_ADGN_MASK|SIGMA_PGAGN_MASK);
		vtReadTemp|=(pAdParas->VrefGain|pAdParas->AdGain|pAdParas->PgGain);
		writeSigmaCtrlRegister(SIGMA_PGAC0, &vtReadTemp);
		//------------------------------------------------------------------
//		readSigmaCtrlRegister(SIGMA_PGAC1, &vtReadTemp);
//		vtReadTemp&=~(SIGMA_DCESET_MASK);
//		vtReadTemp|=(SIGMA_DCSET_075);
//		writeSigmaCtrlRegister(SIGMA_PGAC1, &vtReadTemp);
		//------------------------------------------------------------------
		readSigmaCtrlRegister(SIGMA_PGACS, &vtReadTemp);
		vtReadTemp&=~(SIGMA_IN2N_SELECT_MASK|SIGMA_IN2P_SELECT_MASK);
		vtReadTemp|=(pAdParas->in2N|pAdParas->in2P);
		writeSigmaCtrlRegister(SIGMA_PGACS, &vtReadTemp);

		readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
		vtReadTemp&=~(SIGMA_ADRST_MASK);
		vtReadTemp|=(SIGMA_AD_RESET_DISABLE);							//start converting
		writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
		pAdParas->sigmaResultAcc=0;
		pAdParas->sigmaAdCovertCnt=0;
	}
}


void sigmaAdTest(void) {
	u8 vtReadTemp = 0;
//	eepromTest();
//	while(1);

	readSigmaCtrlRegister(SIGMA_ADCTE, &vtReadTemp);
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp &= SIGMA_AD_PW_ENABLE;
	writeSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp = 0;
	readSigmaCtrlRegister(SIGMA_ADCR0, &vtReadTemp);
	vtReadTemp = 0;
	startSigmaAdConvert();
}

