#ifndef _HCSR_H
#define	_HCSR_H

#include "system.h"
#include "SysTick.h"
//trig: PB5
//echo: PB6

#define TRIG PBout(5)
#define ECHO PBin(6)
#define HC_SR_PORT 				GPIOB
#define TRIG_PIN					GPIO_PIN_5
#define ECHO_PIN					GPIO_PIN_6
#define HC_SR_PORT_RCC_ENABLE	__HAL_RCC_GPIOB_CLK_ENABLE()

extern int hcsr_cd;
void HC_SRInit(void);


#endif

