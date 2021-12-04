/*
 * user_exti.h
 *
 *  Created on: 2020年3月19日
 *      Author: Bough_004
 */

#ifndef USER_INC_USER_EXTI_H_
#define USER_INC_USER_EXTI_H_
#include "const.h"
#include "ht32f5xxxx_gpio.h"

#define	EXTI_CHANNEL_0			(0)
#define	EXTI_CHANNEL_1			(1)
#define	EXTI_CHANNEL_2			(2)
#define	EXTI_CHANNEL_3			(3)
#define	EXTI_CHANNEL_4			(4)
#define	EXTI_CHANNEL_5			(5)
#define	EXTI_CHANNEL_6			(6)
#define	EXTI_CHANNEL_7			(7)
#define	EXTI_CHANNEL_8			(8)
#define	EXTI_CHANNEL_9			(9)
#define	EXTI_CHANNEL_10		(10)
#define	EXTI_CHANNEL_11		(11)
#define	EXTI_CHANNEL_12		(12)
#define	EXTI_CHANNEL_13		(13)
#define	EXTI_CHANNEL_14		(14)
#define	EXTI_CHANNEL_15		(15)



/** @defgroup GPI0_IRQ_InitTypeDef
  * @{
  */
typedef struct
{
  uint8_t GPI0_IRQ_Pin_Enable;         /*!< Specifies the GPIO pins to be configured.
                                       This parameter can be any value of @ref GPI0_IRQ_Pin_Enable */

  uint8_t GPI0_IRQ_Pin_Clear;          /*!< Specifies the operating mode for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Clear */

  uint8_t GPI0_IRQ_Pin_Type;           /*!< Specifies the speed for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Type */

  uint8_t GPI0_IRQ_Pin_Polarity;       /*!< Specifies the operating output type for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Polarity */

  uint8_t GPI0_IRQ_Pin_Edge;           /*!< Specifies the operating Pull-up/Pull down for the selected pins.
                                       This parameter can be a value of @ref GPI0_IRQ_Pin_Edge */
  AFIO_EXTI_CH_Enum	GPIO_IRQ_Chl;
}GPI0_IRQ_InitTypeDef;


extern	void EXTI_Key_Config(void);

#endif /* USER_INC_USER_EXTI_H_ */
