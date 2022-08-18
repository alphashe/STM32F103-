#ifndef _led_H
#define _led_H


#include "system.h"


/*  LED时钟端口、引脚定义 */
#define LED1_PORT 				GPIOC 
#define LED1_PIN 				GPIO_PIN_13
#define LED1_PORT_RCC_ENABLE	__HAL_RCC_GPIOC_CLK_ENABLE()




#define LED1 PCout(13)  	


void LED_Init(void);

#endif
