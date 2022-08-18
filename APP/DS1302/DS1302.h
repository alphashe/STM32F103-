#ifndef _DS1302_H
#define _DS1302_H
/*********************************
*CLK: PA0
*DAT: PA1
*RST: PA2
*VCC: 3.3V
**********************************/
#define uchar unsigned char
#define uint  unsigned int
extern uchar sec_flag;
//管脚定义
#define CLK_PIN 				GPIO_PIN_0
#define DAT_PIN 				GPIO_PIN_1
#define RST_PIN 				GPIO_PIN_2
#define DS1302_PORT 				GPIOA 
#define DS1302_PORT_RCC_ENABLE	__HAL_RCC_GPIOA_CLK_ENABLE()
#define CLK  PAout(0)
#define DAT  PAout(1)
#define DATR  PAin(1)
#define RST  PAout(2)
//寄存器地址定义
#define ds1302_sec_add			0x80		//秒数据地址
#define ds1302_min_add			0x82		//分数据地址
#define ds1302_hr_add			0x84		//时数据地址
#define ds1302_date_add			0x86		//日数据地址
#define ds1302_month_add		0x88		//月数据地址
#define ds1302_day_add			0x8a		//星期数据地址
#define ds1302_year_add			0x8c		//年数据地址
#define ds1302_control_add		0x8e		//控制数据地址
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

void Delay_xms(uint x);//延时函数
void ds1302_init(void);//DS1302初始化
void ds1302_write_byte(uchar addr, uchar d); //向DS1302写入一字节数据
uchar ds1302_read_byte(uchar addr);//从DS1302读出一字节数据
void ds1302_write_time(void); //向DS302写入时钟数据
void ds1302_read_time(void);  //从DS302读出时钟数据
void ds1302_send_time(void);//用串口发送时钟数据
#endif

