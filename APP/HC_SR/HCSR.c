#include "HCSR.h"

int hcsr_cd = 0;
void HC_SRInit()
{
		GPIO_InitTypeDef GPIO_InitStructure;
	
		HC_SR_PORT_RCC_ENABLE;
	
		GPIO_InitStructure.Pin=TRIG_PIN; 
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //����
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(HC_SR_PORT,&GPIO_InitStructure);
	
		GPIO_InitStructure.Pin=ECHO_PIN; 
    GPIO_InitStructure.Mode=GPIO_MODE_INPUT;  //�������
    GPIO_InitStructure.Pull=GPIO_NOPULL;          //����
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(HC_SR_PORT,&GPIO_InitStructure);
		
		TRIG = 0;
}

