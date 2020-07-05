#include "def_com.h"
#include "adc.h"
#include "lcd.h"
#include "led.h"
#include "port.h"
#include "timer.h"
#include "queue.h"
#include "pid.h"
#include "string.h"
#include "status.h"
#include "menu.h"
extern long ADC0_result[2];
extern uchar ADC0_flag[2];

extern uint count_led;
extern uchar function_index;

Queue points;
PIDIncStruct PIDInc;
char temperature_reference = 50; //参考温度值

double SetPos = 0.5, ActPos, DeltaPos = 0;

uchar cure_data_flag;
uchar cure_data[2][1024];

uchar cure_enable = 0;

uchar inform_index = 0;


Menu menu0;
Menu menu01;
Menu menu02;
uchar code menu0_str[3][13]={"作者信息显示","温度检测预警","飞行稳定控制"}; 
uchar code menu01_str[4][13]= {"返回主界面  ","温度曲线显示","预警温度设定","返回上一级  "};
uchar code menu02_str[4][13]= {"返回主界面  ","控制曲线显示","电压曲线显示","返回上一级  "};

void InformShow(uchar index, uchar direction);
void InitDevice(); 
void draw_buffer(uchar *buffer, Queue *Q);
void Date_Show();




int main()
{
	// variable declaration
	int temp_int = 0, temp = 0, iter = 0, temperature0 = 0;
	double tempd;
	// init
	InitDevice(); 
	INT1 = 1;	 

	EX1 = 1;
	AD0EN = 1; 
	EA = 1;	  

	InintQueue(&points);
	InitMenu(&menu0,menu0_str,3);
	InitMenu(&menu01,menu01_str,4);
	InitMenu(&menu02,menu02_str,4);
	
	cure_data_flag = 0;
	//pid
	{
		PIDInc.Kp = 0.01;
		PIDInc.Ki = 0.001;
		PIDInc.Kd = 0.001;
	}
	LcdClear();
	MenuShow(&menu0);
	
	memset(cure_data[1], 0, 1024 * sizeof(uchar));
	WriteStr(0,0,menu0_str[0]);
	while (1)
	{
		if(function_index>=2&&function_index<=5)
		{
			if(ADC0_flag[0])
			{
				ADC0_flag[0]=0;
				temperature0= (int)(ADC0_result[0]-280)*(1.101/2);
				tempd=temperature0/40.0;
				temperature0=53.64*tempd-0.6166;//temperature calculate
				
				temp=tempd*32;
				temp=temp>63?63:temp;
				temp=temp<0?0:temp;
				EnQueue(&points,(uchar)temp);
			}
			if(points.base[(points.rear+129-1)%129]>temperature_reference && !(P4&0x02))
			{
				P4|=0x02;
				

			}
			else if(P4&0x02)
			{
				P4&=~0x02;
			}
			Led_display(2,temperature0);
		}
		else if(function_index>=6&&function_index<=9)
		{
			if (ADC0_flag[1])
			{
				ADC0_flag[1] = 0;

				ActPos = (ADC0_result[1] - 2000.0) / 240.0;
				
				DeltaPos+=PID_Inc(SetPos,ActPos,&PIDInc);
				DeltaPos = DeltaPos > 2.6? 2.6 : DeltaPos;
				DeltaPos = DeltaPos < 0.1 ? 0.1 : DeltaPos;
				if(function_index==9)
				{
					temp = ActPos * 64 / 3;
				}
				else
				{
					temp = DeltaPos * 64 / 3;
					
				}
				temp = temp > 63 ? 63 : temp;
				temp = temp < 0 ? 0 : temp;
				EnQueue(&points, (uchar)temp); //((iter++)%1024)/16);
			}
			Led_display(2,DeltaPos);

		}
		if (cure_enable)
		{
			memset(cure_data[cure_data_flag], 0, 1024 * sizeof(uchar));
			draw_buffer(cure_data[cure_data_flag], &points);
			ScreenShow(cure_data[cure_data_flag], cure_data[!cure_data_flag], 0);
			cure_data_flag = !cure_data_flag;
		}

		Date_Show();
		//Led_display(2,points.base[(points.rear+65-1)%65]);
		//WDTCN = 0xa5;
	}
	return 0;
}

void InitDevice()
{
	EA = 0;
	WDTCN = 0xde; 
	WDTCN = 0xad;
	EA = 1;
	SYSCLK_Init(); 
	PORT_Init();
	Port_IO_Init();
	Timer0_Init();
	//Timer1_Init();
	Timer3_Init(SYSCLK / SAMPLERATE0);
	DAC0_Init();
	Timer4_Init(SYSCLK / SAMPLE_RATE_DAC);
	EIE2 &=~0x04;//Close Timer4
	Interrupt_Init();
	ADC0_Init();

	LcdInit();
	LedInit();
}


void draw_buffer(uchar *buffer, Queue *Q)
{
	uchar num = Q->number, index = Q->rear, count = 0, temp;
	while (num-- != 0)
	{
		index = (index + 129 - 1) % 129;
		temp = (63 - Q->base[index]) * 8 + count >> 4;
		buffer[(63 - Q->base[index]) * 16 + (count >> 3)] |= 0x80 >> (count & 0x07);
		++count;
	}
}

void Date_Show()
{
	Led_display(0, 2020);
	digit_display(1, 0, 0);
	Led_display(1, 16.28);
}