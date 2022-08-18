#include "DS1302.h"
#include "usart.h"
#include "led.h"
//延时函数

uchar time_buf[8] = {0x20,0x22,0x08,0x06,0x13,0x41,0x00,0x06};//初始时间
uchar readtime[21];//当前时间
uchar sec_buf=0;  //秒缓存
uchar sec_flag=0; //秒标志位

void Delay_xms(uint x)
{
  uint i,j;
  for(i=0;i<x;i++)
    for(j=0;j<112;j++);
}
//DS1302初始化函数
void ds1302_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	DS1302_PORT_RCC_ENABLE;

	GPIO_InitStructure.Pin=CLK_PIN | DAT_PIN | RST_PIN; 
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;  //开漏输出
	GPIO_InitStructure.Pull=GPIO_PULLUP;          //上拉
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//高速
	HAL_GPIO_Init(DS1302_PORT,&GPIO_InitStructure);

	
	RST=0;			//RST脚置低
	CLK=0;			//SCK脚置低
}
//向DS1302写入一字节数据
void ds1302_write_byte(uchar addr, uchar d) 
{
	uchar i;
	RST=1;					//启动DS1302总线	
	//写入目标地址：addr
	addr = addr & 0xFE;   //最低位置零，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DAT=1;
			}
		else {
			DAT=0;
			}
		CLK=1;      //产生时钟
		CLK=0;
		addr = addr >> 1;
		}	
	//写入数据：d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			DAT=1;
			}
		else {
			DAT=0;
			}
		CLK=1;    //产生时钟
		CLK=0;
		d = d >> 1;
		}
	RST=0;		//停止DS1302总线
}

//从DS1302读出一字节数据
uchar ds1302_read_byte(uchar addr) 
{

	uchar i,temp;	
	RST=1;					//启动DS1302总线
	//写入目标地址：addr
	addr = addr | 0x01;    //最低位置高，寄存器0位为0时写，为1时读
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DAT=1;
			}
		else {
			DAT=0;
			}
		CLK=1;
		CLK=0;
		addr = addr >> 1;
		}	
	//输出数据：temp
	for (i = 0; i < 8; i ++) {
		temp = temp >> 1;
		if (DATR) {
			temp |= 0x80;
			}
		else {
			temp &= 0x7F;
			}
		CLK=1;
		CLK=0;
		}	
	RST=0;					//停止DS1302总线
	return temp;
}
//向DS302写入时钟数据
void ds1302_write_time(void) 
{
	ds1302_write_byte(ds1302_control_add,0x00);			//关闭写保护 
	ds1302_write_byte(ds1302_sec_add,0x80);				//暂停时钟 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //涓流充电 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		//年 
	ds1302_write_byte(ds1302_month_add,time_buf[2]);	//月 
	ds1302_write_byte(ds1302_date_add,time_buf[3]);		//日 
	ds1302_write_byte(ds1302_hr_add,time_buf[4]);		//时 
	ds1302_write_byte(ds1302_min_add,time_buf[5]);		//分
	ds1302_write_byte(ds1302_sec_add,time_buf[6]);		//秒
	ds1302_write_byte(ds1302_day_add,time_buf[7]);		//周 
	ds1302_write_byte(ds1302_control_add,0x80);			//打开写保护     
}
//从DS302读出时钟数据
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//年 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//月 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//日 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//时 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//分 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//秒，屏蔽秒的第7位，避免超出59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//周 	
	

	if(sec_buf != time_buf[6])  //秒缓存
	{
		sec_buf = time_buf[6];
		LED1 = !LED1;
		sec_flag=1;
	}
}

void ds1302_send_time()
{
	readtime[0]=(time_buf[0]>>4)+'0';   //分离出年千位
  readtime[1]=(time_buf[0]&0x0F)+'0'; //分离出年百位 
  readtime[2]=(time_buf[1]>>4)+'0';   //分离出年十位
  readtime[3]=(time_buf[1]&0x0F)+'0'; //分离出年个位 
	readtime[4]='-';
  readtime[5]=(time_buf[2]>>4)+'0';   //分离出月十位
  readtime[6]=(time_buf[2]&0x0F)+'0'; //分离出月个位 
	readtime[7]='-';
  readtime[8]=(time_buf[3]>>4)+'0';   //分离出日十位
  readtime[9]=(time_buf[3]&0x0F)+'0'; //分离出日个位 
	readtime[10]=' ';
  readtime[11]=(time_buf[4]>>4)+'0';   //分离出小时十位
  readtime[12]=(time_buf[4]&0x0F)+'0'; //分离出小时个位 
	readtime[13]=':';
  readtime[14]=(time_buf[5]>>4)+'0';   //分离出分钟十位
  readtime[15]=(time_buf[5]&0x0F)+'0'; //分离出分钟个位
	readtime[16]=':';
  readtime[17]=(time_buf[6]>>4)+'0';   //分离出秒钟十位
  readtime[18]=(time_buf[6]&0x0F)+'0'; //分离出秒钟个位   
	readtime[20]=':';
	
	HAL_UART_Transmit(&UART1_Handler,readtime,21,1000);	
}
