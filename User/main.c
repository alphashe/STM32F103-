#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include "led.h"
#include "DS1302.h"
#include "stm32f1xx_hal.h"

	uint32_t alpha=0; 
	unsigned char ms[4]={0,0,'\r', '\n'};
	uint32_t alphashe = 0;
/*******************************************************************************
Uart配置：PA9：TXD，PA10：RXD
DS1302配置：
*******************************************************************************/
int main()
{

	HAL_Init();                     //初始化HAL库 
	SystemClock_Init(RCC_PLL_MUL9); //设置时钟,72M
	SysTick_Init(72);
	USART1_Init(9600);
	LED_Init();
	ds1302_init();
	
	while(1)
	{
		if(USART1_RX_STA&0x8000)
		{					   
//			len=USART1_RX_STA&0x3fff;//得到此次接收到的数据长度
//			HAL_UART_Transmit(&UART1_Handler,(uint8_t*)USART1_RX_BUF,len,1000);	//发送接收到的数据
//			while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//等待发送结束
			ds1302_read_time();
			alpha= HAL_GetTick();//uS -> mS i (int)
			ms[1] = ((alpha %100) / 10) + '0';
			ms[0] = (alpha/100) + '0';
			if(ms[0] >(9 +'0'))
			{				
				ms[0] = 9+'0';
			}
			ds1302_send_time();
			HAL_UART_Transmit(&UART1_Handler,ms,4,1000);//发送ms数组
			USART1_RX_STA=0;
		}
		ds1302_read_time();
		if(sec_flag == 1)
		{
			sec_flag = 0;
			HAL_WriteTick(0);
		}
	}
}
