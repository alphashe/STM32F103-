#include "led.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	LED1_PORT_RCC_ENABLE;

	
	GPIO_InitStructure.Pin=LED1_PIN; 
    GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_InitStructure.Pull=GPIO_PULLUP;          //����
    GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//����
    HAL_GPIO_Init(LED1_PORT,&GPIO_InitStructure);

	
	HAL_GPIO_WritePin(LED1_PORT,LED1_PIN,GPIO_PIN_SET);	

} 
