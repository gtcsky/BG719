/*
 * user_spi.c
 *
 *  Created on: 2019年11月25日
 *      Author: Bough_004
 */

#include	"user_spi.h"
#include	"user_lptimer.h"
#include "ht32f5xxxx_spi.h"
#include "user_menu.h"
uint8_t  SPIFlagState;
uint8_t  SPI_Data;
uint8_t  MASTERFLAG;

 /**
  * @brief  使用SPI发送一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_SendByte(uint8_t byte)
 {
	/* 写入数据寄存器，把要写入的数据写入发送缓冲区 */
	SPI_SendData(HTCFG_MASTER_SPI, byte);
	while (!SPI_GetFlagStatus(HTCFG_MASTER_SPI, SPI_FLAG_TXE) && (vSystem5ms < 200)) ;
	return 0;
}

void WriteData(uint8_t *sendstr,uint8_t sendlen){
	    uint8_t i;
	    for(i=0;i<sendlen;i++)
	    {
		    SPI_SendByte(*(sendstr + i));
	    }
}
void ReadData(uint8_t *recvstr,uint8_t recvlen){

}

void spiIoInit(void) {
	 	 	 GPIO_InitTypeDef	GPIO_InitStruct={{0}};
			GPIO_InitStruct.GPIO_Mode = GPIO_DIR_OUT;							//output 模式
			GPIO_InitStruct.GPIO_OType = GPIO_DV_4MA;
			GPIO_InitStruct.GPIO_Pin = LcdResPin;									//Pin
			GPIO_InitStruct.GPIO_Af_Port_ID = LcdResPortID;						//AF设定时的 Port ID
			GPIO_Init(LcdResPort, &GPIO_InitStruct);								//GPIO Port
			set_LcdResPin;
			GPIO_InitStruct.GPIO_Pin = LcdDcPin;									//Pin
			GPIO_InitStruct.GPIO_Af_Port_ID = LcdDcPortID;							//AF设定时的 Port ID
			GPIO_Init(LcdDcPort, &GPIO_InitStruct);								//GPIO Port
			set_LcdDcPin;

			AFIO_GPxConfig(HTCFG_MASTER_SPI_SCK_GPIO_ID, HTCFG_MASTER_SPI_SCK_AFIO_PIN, AFIO_FUN_SPI);
			AFIO_GPxConfig(HTCFG_MASTER_SPI_SEL_GPIO_ID, HTCFG_MASTER_SPI_SEL_AFIO_PIN, AFIO_FUN_SPI);
	//  AFIO_GPxConfig(HTCFG_MASTER_SPI_MISO_GPIO_ID, HTCFG_MASTER_SPI_MISO_AFIO_PIN, AFIO_FUN_SPI);
			AFIO_GPxConfig(HTCFG_MASTER_SPI_MOSI_GPIO_ID, HTCFG_MASTER_SPI_MOSI_AFIO_PIN, AFIO_FUN_SPI);

}


void userSpiInit(void) {
	  SPI_InitTypeDef SPI_InitStructure;
	  spiIoInit();

//	  SPI_InitStructure.SPI_Mode = SPI_MASTER;
//	  SPI_InitStructure.SPI_FIFO = SPI_FIFO_ENABLE;
//	  SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
//	  SPI_InitStructure.SPI_SELMode = SPI_SEL_HARDWARE;
//	  SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
//	  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
//	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
//	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_FIRST;
//	  SPI_InitStructure.SPI_RxFIFOTriggerLevel = 1;
//	  SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
//	  SPI_InitStructure.SPI_ClockPrescaler = 16;
//	  SPI_Init(HTCFG_MASTER_SPI, &SPI_InitStructure);
//
//	  SPI_SELOutputCmd(HTCFG_MASTER_SPI, ENABLE);
//
//	  SPI_Cmd(HTCFG_MASTER_SPI, ENABLE);
////	  SPI_IntConfig(HTCFG_MASTER_SPI, SPI_INT_TXE, ENABLE);
//	  NVIC_EnableIRQ(HTCFG_MASTER_SPI_IRQ);

	  SPI_InitStructure.SPI_Mode = SPI_MASTER;
	  SPI_InitStructure.SPI_FIFO = SPI_FIFO_DISABLE;
	  SPI_InitStructure.SPI_DataLength = SPI_DATALENGTH_8;
	  SPI_InitStructure.SPI_SELMode = SPI_SEL_HARDWARE;
	  SPI_InitStructure.SPI_SELPolarity = SPI_SELPOLARITY_LOW;
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_FIRST;
	  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
	  SPI_InitStructure.SPI_RxFIFOTriggerLevel = 0;
	  SPI_InitStructure.SPI_TxFIFOTriggerLevel = 0;
	  SPI_InitStructure.SPI_ClockPrescaler = 2;
	  SPI_Init(HTCFG_MASTER_SPI, &SPI_InitStructure);

	  /* Slave SEL as output mode for slave select                                                                */
	  SPI_SELOutputCmd(HTCFG_MASTER_SPI, ENABLE);
	  SPI_Cmd(HTCFG_MASTER_SPI, ENABLE);
}

/**
  * @brief  This function handles SPI0COMB_IRQn interrupt request.
  * @param  None
  * @retval None
  */
/*********************************************************************************************************//**
 * @brief   This function handles SPI interrupt.
 * @retval  None
 ************************************************************************************************************/
void SPI0_IRQHandler(void)
//void SPI1_IRQHandler(void)
{
}
