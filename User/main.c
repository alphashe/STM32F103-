#include "system.h"
#include "SysTick.h"
#include "usart.h"
#include "led.h"
#include "DS1302.h"
#include "HCSR.h"
#include "stm32f1xx_hal.h"

	uint32_t alpha=0; 
	unsigned char ms[5]={0, 0, 0, '\r', '\n'};
	unsigned char ack[4]={'O', 'K', '\r', '\n'};
	uint32_t alphashe = 0;
	char hcflag=0;
	char pastflag=0;
/*******************************************************************************
Uart配置：PA9：TXD，PA10：RXD
DS1302配置：
*******************************************************************************/
void SendTimetoPC(void)	//将毫秒增至3位数
{
			ds1302_read_time();
			alpha= HAL_GetTick();//uS -> mS i (int)
			ms[2] = alpha%10+'0';
			ms[1] = ((alpha %100) / 10) + '0';
			ms[0] = (alpha/100) + '0';
			if(ms[0] >(9 +'0'))
			{				
				ms[0] = 9+'0';
			}
			ds1302_send_time();
			HAL_UART_Transmit(&UART1_Handler,ms,5,1000);//发送ms数组
}
int HCSR_TRG(void)
{
	if(hcsr_cd ==0)
		{
			TRIG = 1;
			delay_us(13);
			TRIG = 0;
			while(!ECHO);
			HAL_Delay(1);
			if(ECHO)
				hcflag=0;	//34cm内没有东西
			else
			{
				hcflag=1;	//34cm内有东西
			}
			hcsr_cd = 60;
		}
		return hcflag;
}
int main()
{

	HAL_Init();                     //初始化HAL库 
	SystemClock_Init(RCC_PLL_MUL9); //设置时钟,72M
	SysTick_Init(72);
	USART1_Init(9600);
	LED_Init();
	ds1302_init();
	HC_SRInit();
	TRIG  =0;
	while(1)
	{
		if(USART1_RX_STA&0x8000)
		{					   
			ds1302_write_time();
			HAL_UART_Transmit(&UART1_Handler,ack,4,1000);//发送ms数组
			USART1_RX_STA=0;
		}
		if(HCSR_TRG()&&(pastflag==0))	//防止检测到物体时，持续发送时间
		{
			SendTimetoPC();
			pastflag = 1;
		}
		pastflag = HCSR_TRG();
		ds1302_read_time();
		if(sec_flag == 1)
		{
			sec_flag = 0;
			HAL_WriteTick(0);
		}
		
	}
}
