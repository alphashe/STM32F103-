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
//�ܽŶ���
#define CLK_PIN 				GPIO_PIN_0
#define DAT_PIN 				GPIO_PIN_1
#define RST_PIN 				GPIO_PIN_2
#define DS1302_PORT 				GPIOA 
#define DS1302_PORT_RCC_ENABLE	__HAL_RCC_GPIOA_CLK_ENABLE()
#define CLK  PAout(0)
#define DAT  PAout(1)
#define DATR  PAin(1)
#define RST  PAout(2)
//�Ĵ�����ַ����
#define ds1302_sec_add			0x80		//�����ݵ�ַ
#define ds1302_min_add			0x82		//�����ݵ�ַ
#define ds1302_hr_add			0x84		//ʱ���ݵ�ַ
#define ds1302_date_add			0x86		//�����ݵ�ַ
#define ds1302_month_add		0x88		//�����ݵ�ַ
#define ds1302_day_add			0x8a		//�������ݵ�ַ
#define ds1302_year_add			0x8c		//�����ݵ�ַ
#define ds1302_control_add		0x8e		//�������ݵ�ַ
#define ds1302_charger_add		0x90 					 
#define ds1302_clkburst_add		0xbe

void Delay_xms(uint x);//��ʱ����
void ds1302_init(void);//DS1302��ʼ��
void ds1302_write_byte(uchar addr, uchar d); //��DS1302д��һ�ֽ�����
uchar ds1302_read_byte(uchar addr);//��DS1302����һ�ֽ�����
void ds1302_write_time(void); //��DS302д��ʱ������
void ds1302_read_time(void);  //��DS302����ʱ������
void ds1302_send_time(void);//�ô��ڷ���ʱ������
#endif

