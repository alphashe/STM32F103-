#include "DS1302.h"
#include "usart.h"
#include "led.h"
//��ʱ����

uchar time_buf[8] = {0x20,0x22,0x08,0x06,0x13,0x41,0x00,0x06};//��ʼʱ��
uchar readtime[21];//��ǰʱ��
uchar sec_buf=0;  //�뻺��
uchar sec_flag=0; //���־λ

void Delay_xms(uint x)
{
  uint i,j;
  for(i=0;i<x;i++)
    for(j=0;j<112;j++);
}
//DS1302��ʼ������
void ds1302_init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	DS1302_PORT_RCC_ENABLE;

	GPIO_InitStructure.Pin=CLK_PIN | DAT_PIN | RST_PIN; 
	GPIO_InitStructure.Mode=GPIO_MODE_OUTPUT_OD;  //��©���
	GPIO_InitStructure.Pull=GPIO_PULLUP;          //����
	GPIO_InitStructure.Speed=GPIO_SPEED_FREQ_HIGH;//����
	HAL_GPIO_Init(DS1302_PORT,&GPIO_InitStructure);

	
	RST=0;			//RST���õ�
	CLK=0;			//SCK���õ�
}
//��DS1302д��һ�ֽ�����
void ds1302_write_byte(uchar addr, uchar d) 
{
	uchar i;
	RST=1;					//����DS1302����	
	//д��Ŀ���ַ��addr
	addr = addr & 0xFE;   //���λ���㣬�Ĵ���0λΪ0ʱд��Ϊ1ʱ��
	for (i = 0; i < 8; i ++) {
		if (addr & 0x01) {
			DAT=1;
			}
		else {
			DAT=0;
			}
		CLK=1;      //����ʱ��
		CLK=0;
		addr = addr >> 1;
		}	
	//д�����ݣ�d
	for (i = 0; i < 8; i ++) {
		if (d & 0x01) {
			DAT=1;
			}
		else {
			DAT=0;
			}
		CLK=1;    //����ʱ��
		CLK=0;
		d = d >> 1;
		}
	RST=0;		//ֹͣDS1302����
}

//��DS1302����һ�ֽ�����
uchar ds1302_read_byte(uchar addr) 
{

	uchar i,temp;	
	RST=1;					//����DS1302����
	//д��Ŀ���ַ��addr
	addr = addr | 0x01;    //���λ�øߣ��Ĵ���0λΪ0ʱд��Ϊ1ʱ��
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
	//������ݣ�temp
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
	RST=0;					//ֹͣDS1302����
	return temp;
}
//��DS302д��ʱ������
void ds1302_write_time(void) 
{
	ds1302_write_byte(ds1302_control_add,0x00);			//�ر�д���� 
	ds1302_write_byte(ds1302_sec_add,0x80);				//��ͣʱ�� 
	//ds1302_write_byte(ds1302_charger_add,0xa9);	    //������ 
	ds1302_write_byte(ds1302_year_add,time_buf[1]);		//�� 
	ds1302_write_byte(ds1302_month_add,time_buf[2]);	//�� 
	ds1302_write_byte(ds1302_date_add,time_buf[3]);		//�� 
	ds1302_write_byte(ds1302_hr_add,time_buf[4]);		//ʱ 
	ds1302_write_byte(ds1302_min_add,time_buf[5]);		//��
	ds1302_write_byte(ds1302_sec_add,time_buf[6]);		//��
	ds1302_write_byte(ds1302_day_add,time_buf[7]);		//�� 
	ds1302_write_byte(ds1302_control_add,0x80);			//��д����     
}
//��DS302����ʱ������
void ds1302_read_time(void)  
{
	time_buf[1]=ds1302_read_byte(ds1302_year_add);		//�� 
	time_buf[2]=ds1302_read_byte(ds1302_month_add);		//�� 
	time_buf[3]=ds1302_read_byte(ds1302_date_add);		//�� 
	time_buf[4]=ds1302_read_byte(ds1302_hr_add);		//ʱ 
	time_buf[5]=ds1302_read_byte(ds1302_min_add);		//�� 
	time_buf[6]=(ds1302_read_byte(ds1302_sec_add))&0x7f;//�룬������ĵ�7λ�����ⳬ��59
	time_buf[7]=ds1302_read_byte(ds1302_day_add);		//�� 	
	

	if(sec_buf != time_buf[6])  //�뻺��
	{
		sec_buf = time_buf[6];
		LED1 = !LED1;
		sec_flag=1;
	}
}

void ds1302_send_time()
{
	readtime[0]=(time_buf[0]>>4)+'0';   //�������ǧλ
  readtime[1]=(time_buf[0]&0x0F)+'0'; //��������λ 
  readtime[2]=(time_buf[1]>>4)+'0';   //�������ʮλ
  readtime[3]=(time_buf[1]&0x0F)+'0'; //��������λ 
	readtime[4]='-';
  readtime[5]=(time_buf[2]>>4)+'0';   //�������ʮλ
  readtime[6]=(time_buf[2]&0x0F)+'0'; //������¸�λ 
	readtime[7]='-';
  readtime[8]=(time_buf[3]>>4)+'0';   //�������ʮλ
  readtime[9]=(time_buf[3]&0x0F)+'0'; //������ո�λ 
	readtime[10]=' ';
  readtime[11]=(time_buf[4]>>4)+'0';   //�����Сʱʮλ
  readtime[12]=(time_buf[4]&0x0F)+'0'; //�����Сʱ��λ 
	readtime[13]=':';
  readtime[14]=(time_buf[5]>>4)+'0';   //���������ʮλ
  readtime[15]=(time_buf[5]&0x0F)+'0'; //��������Ӹ�λ
	readtime[16]=':';
  readtime[17]=(time_buf[6]>>4)+'0';   //���������ʮλ
  readtime[18]=(time_buf[6]&0x0F)+'0'; //��������Ӹ�λ   
	readtime[20]=':';
	
	HAL_UART_Transmit(&UART1_Handler,readtime,21,1000);	
}
