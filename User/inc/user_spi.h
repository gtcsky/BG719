/*
 * user_spi.h
 *
 *  Created on: 2019-11-25
 *      Author: Bough_004
 */

#ifndef SOURCE_INC_USER_SPI_H_
#define SOURCE_INC_USER_SPI_H_

#include "stdint.h"
#include "const.h"
//#include "spi.h"
#include	"io_define.h"
#define HTCFG_MASTER_SPI                     			(HT_SPI0)
#define HTCFG_MASTER_SPI_IRQ                 		(SPI0_IRQn)

#define HTCFG_MASTER_SPI_SCK_GPIO_ID         	(GPIO_PA)
#define HTCFG_MASTER_SPI_SCK_AFIO_PIN        	(AFIO_PIN_4)

#define HTCFG_MASTER_SPI_MOSI_GPIO_ID        	(GPIO_PA)
#define HTCFG_MASTER_SPI_MOSI_AFIO_PIN       	(AFIO_PIN_5)

#define HTCFG_MASTER_SPI_MISO_GPIO_ID        	(GPIO_PA)
#define HTCFG_MASTER_SPI_MISO_AFIO_PIN       	(AFIO_PIN_6)

#define HTCFG_MASTER_SPI_SEL_GPIO_ID         	(GPIO_PA)
#define HTCFG_MASTER_SPI_SEL_AFIO_PIN        	(GPIO_PIN_7)

#define 	SpiCsPortID                                  			(GPIO_PA)
#define 	SpiCsPort                                 			(HT_GPIOA)
#define 	SpiCsPin                                  			(GPIO_PIN_7)
#define 	SpiCsCLK(CK)                                    		(CK.Bit.PA)
#define	SpiCsInactive							GPIO_WriteOutBits(SpiCsPort, SpiCsPin, SET)
#define	SpiCsActive							GPIO_WriteOutBits(SpiCsPort, SpiCsPin, RESET)


extern	void WriteData(uint8_t *sendstr,uint8_t sendlen);
extern	void ReadData(uint8_t *recvstr,uint8_t recvlen);
extern	void userSpiInit(void);
#endif /* SOURCE_INC_USER_SPI_H_ */
