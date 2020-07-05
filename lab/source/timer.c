#include <C8051F020.h>
#include "def_com.h"
#include "timer.h"
#include "queue.h"
#include "led.h"
#include "lcd.h"

extern uchar inform_index;
extern uchar cure_enable;
extern char temperature_reference;
extern double DeltaPos;
extern double SetPos;
uint count_led = 0;
uchar image_index=0;
uchar function_index = 0;
uchar k=0;
int dac0_data;

//系统时钟设定
/*-----------------------------*/
void SYSCLK_Init(void)
{
    int i;         // 延时计数器
    OSCXCN = 0x67; // 起动外部振荡器晶体为 22.1184MHz
    for (i = 0; i < 256; i++)
        ; // 等待振荡器启动(>1ms)
    while (!(OSCXCN & 0x80))
        ;          // 等待晶体振荡器稳定
    OSCICN = 0x88; // 选择外部振荡器作为系统时钟源并使能丢失时钟检测器
}

//定时器
/*-----------------------------*/
//TR0 = 0; // 开始读秒
void Timer0_Init(void)
{
    TH0 = TIMER0_RELOAD_HIGH; // Reinit Timer0 High register
    TL0 = TIMER0_RELOAD_LOW;
    ET0 = 1;     // Timer0 interrupt enabled
    TMOD = 0x01; // 16-bit Mode Timer0
    //TCON |= 0x10;                        // Timer0 ON
}

void Timer0_ISR(void) interrupt 1
{
    TH0 = TIMER0_RELOAD_HIGH; // Reinit Timer0 High register
    TL0 = TIMER0_RELOAD_LOW;

    /*
	
        * coding here
        * coding here
    */
	//button up
	P7 = 0xff;
	P5 = 0xf8;
	if (INT1)
	{
		// Timer0 off
		TCON &= ~0x10;
		//INT1 on
		EX1 = 1;
	}
	//button down
	else
	{
	}
	P5 = 0xff;
	//temperture_fake=(temperture_fake+1)%600;
}

////定时器1初始化
//void Timer1_Init(void)
//{
//    CKCON &= 0x68;
//    TMOD = 0x10;
//    TH1 = 0xFc;
//    TL1 = 0x18;
//    ET1 = 1;
//    TR1 = 1;
//}

////定时器1 中断服务程序
//void Timer1_ISR(void) interrupt 3
//{
//    TH1 |= 0xfc;
//    TL1 |= 0x18;
    //*
    //    * coding here
    //    * coding here
    //
//    
//}

////定时器2初始化，DAC0使用
//void Timer2_Init(void)
//{
//    RCAP2H = 0x05;
//    RCAP2L = 0x00; //自动重装载
//    TH2 = 0x05;
//    TL2 = 0x00;
//    T2CON = 0x04;
//    IE |= 0x20;
//}
////定时器2 中断服务程序
//void Time2_ISR(void) interrupt 5
//{
//    unsigned int xdata temp;
//    TF2 = 0;
//    temp &= 0x00ff;
//    DAC0L = temp;
//    temp >>= 8;
//    DAC0H = temp;
//}

//定时器3初始化
void Timer3_Init(int counts)
{
    TMR3CN = 0x02;     // 停止Timer3; 清除TF3;
    TMR3RLL = -counts; // 初始化装载值
    TMR3 = 0xffff;     // 设置立即装载
    EIE2 &= 0xFE;      // 禁止定时器Timer3中断
    TMR3CN |= 0x04;    // 开启Timer3
}

/*定时器4的配置及初始化*/
void Timer4_Init (int counts)
{   
   T4CON = 0x00;                       // Stop Timer4; Clear overflow flag (TF4);
                                       // Set to Auto-Reload Mode

   
   CKCON |= 0x40;                      // Timer4 counts SYSCLKs

   RCAP4 = -counts;                    // Set reload value
   TMR4 = RCAP4;                       // Initialzie Timer4 to reload value

   EIE2 |= 0x04;                       // Enable Timer4 interrupts
   T4CON |= 0x04;                      // Start Timer4

}

/*定时器4的中断服务程序*/
void Timer4_ISR (void) interrupt 16
{
	T4CON &= ~0x80;                     // Clear Timer4 overflow flag
	if(!(function_index>=6&&function_index<=9))
	{
		DeltaPos=0;
	}
		
	dac0_data=(DeltaPos-2.73)*100000/8;//34125;//0x7fff;///((++k)%100);
	DAC0=dac0_data ^ 0x8000;

}

//中断
/*-----------------------------*/
//中断初始化
void Interrupt_Init()
{
    //INT1
    //low level triggle
    IT1 = 0;
    //enable INT1
    EX1 = 1;
    //enable all interrupt
    EA = 1;
}

//中断1服务程序
void INT1_ISR(void) interrupt 2
{
	uchar button=0;
//	if(!function_index)
//	{
//		LcdClearHalf(1);
//		FunctionCallback(0,0);
//		function_index=1;
//		return;
//	}
	switch (P5)
    {
    //right
    case 0xfe:
		button=2;
//		EIE2 &=~0x04;					   // Close Timer4 interrupts
//		SetPos-=0.5;
//		SetPos=SetPos<0?0:SetPos;
		break;
    //middle
    case 0xfd:
		button=1;
//		SetPos+=0.5;
//		SetPos=SetPos>5?5:SetPos;
		break;
    //left
    case 0xfb:
//		EIE2 |=0x04;
		button=0;
		break;
    default:
        return;
    }
	//button= ((~P5)&0x07)/2;
	FunctionCallback(function_index,button);
	/*
    switch (P5)
    {
    //right
    case 0xfe:
		++temperature_reference;
		break;
    //middle
    case 0xfd:
		--temperature_reference;
		break;
    //left
    case 0xfb:
		if(function_index)
		{
			cure_enable=0;
			MainInterfaceShow();
		}
		else
		{
			LcdClear();
			cure_enable=1;
		}
		function_index=!function_index;
		break;
    default:
        return;
    }
	*/	
	

    TCON |= 0x10; // Timer0 on
    EX1 = 0;      //INT1 off
    IE1 = 0;      //clear interrupt flag
}