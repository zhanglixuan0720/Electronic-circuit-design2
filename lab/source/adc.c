#include "adc.h"
#include <C8051F020.h>
#include "def_com.h"

//int temperature_raw;
long ADC0_result[2]={0,0};	//AD转换缓冲
uchar ADC0_flag[2]={0,0};		//AD数据有效指示			
unsigned char channel = 0;
//--------------------------------------------
//ADC0初始化
//--------------------------------------------
void ADC0_Init(void)
{
    ADC0CN = 0xC4;       //关闭ADC;低能追踪模式
    REF0CN = 0x03;        //打开温度传感器, on-chip VREF 
    AMX0SL=0x01;     // ADC0使用"0"通道	changale channels
    ADC0CF=31<<3;         // ADC转换时钟= 2.5MHz
    ADC0CF&=0xf8;          // PGA gain = 1
    EIE2|=0x02;        // 打开ADC0中断
}
////--------------------------------------------
////ADC0中断服务
////--------------------------------------------
void ADC0_ISR(void) interrupt 15
{
    // ADC mux 通道 (0-1)
    AD0INT = 0; // ??ADC??????
    //temperature_raw = ADC_Value(0);
	ADC0_result[channel]=ADC_Value(0);      // ?ADC?
	ADC0_flag[channel]=1;
	channel=(channel+1)%2;
	AMX0SL = channel;              // ??mux?????
}

//--------------------------------------------
//计算ADC数值
//--------------------------------------------
unsigned int ADC_Value(unsigned char Flag)
{
    unsigned int data temp, temp0, temp1;
    if (Flag) //ADC1
        temp0 = ADC1;
    else //ADC0
    {
        temp0 = ADC0L;
        temp1 = ADC0H;
    }
		//temp = temp0&0x3f;
	if(channel)
	{temp = temp1 * 256 + temp0;//(temp1&0x03) * 256 + temp0;
    }
	else
	{
		temp = (temp1&0x03) * 256 + temp0;
    }
    return temp; //取回ADC转换数据
}

//---------------------------------------------
//DAC0初始化
//---------------------------------------------
void DAC0_Init(void)
{
	DAC0CN = 0x94;                      // Enable DAC0 in left-justified mode
									   // managed by Timer4 overflows
	REF0CN |= 0x03;                     // Enable the internal VREF (2.4v) and
									   // the Bias Generator
/*
    REF0CN = 0x03; // 使用内部VREF发生器
    DAC0CN = 0x98;
    DAC0L = 0x00;
    DAC0H = 0x00; // 打开DAC0，右对齐模式
*/
}
