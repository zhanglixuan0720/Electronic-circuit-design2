#ifndef TIMER_H
#define TIMER_H
//系统时钟
void SYSCLK_Init(void); // 此程序初始化系统时钟 使用22.1184MHz晶体作为时钟源
//定时器
void Timer0_Init(void);       //初始化定时器0
void Timer0_ISR(void);        //定时器0中断服务程序
//void Timer1_Init(void);       //初始化定时器1
void Timer1_ISR(void);        //定时器1中断服务程序
//void Timer2_Init(void);       //初始化定时器2
//void Time2_ISR(void);         //定时器2中断服务程序
void Timer3_Init(int counts); //定时器3初始化
void Timer4_Init (int counts);
void Timer4_ISR (void);
//中断
void Interrupt_Init(); //中断初始化
void INT1_ISR(void);   //中断1服务程序
#endif