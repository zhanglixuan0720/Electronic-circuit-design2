#ifndef DEF_COM_H
#define DEF_COM_H
#include <C8051F020.h> // SFR声明
#include <stdio.h>
//-----------------------------------------------------------------------------
// C8051F02X的16位SFR定义
//-----------------------------------------------------------------------------

sfr16 DP = 0x82;     // 数据指针
sfr16 TMR3RL = 0x92; // 定时器3重装值
sfr16 TMR3 = 0x94;   // 定时器3计数器
sfr16 ADC0 = 0xbe;   // ADC0数据
sfr16 ADC0GT = 0xc4; // ADC0大于窗口
sfr16 ADC0LT = 0xc6; // ADC0小于窗口
sfr16 RCAP2 = 0xca;  // 定时器2捕捉/重装
sfr16 T2 = 0xcc;     // 定时器2
sfr16 RCAP4 = 0xe4;  // 定时器4捕捉/重装
sfr16 TMR4 = 0xf4;     // 定时器4
sfr16 DAC0 = 0xd2;   // DAC0数据


//#define SYSCLK 22118400   // 系统时钟频率 Hz
#define SAMPLERATE0 50000 // ADC0采样频率 Hz
#define uchar unsigned char
#define uint unsigned int

#define SYSCLK 11059200 // 系统时钟频率 Hz

#define TIMER_PRESCALER 12 // Based on Timer CKCON settings

#define LED_TOGGLE_RATE 100 // LED toggle rate in milliseconds    1ms \
                            // if LED_TOGGLE_RATE = 1, the LED will   \
                            // be on for 1 millisecond and off for    \
                            // 1 millisecond

// There are SYSCLK/TIMER_PRESCALER timer ticks per second, so
// SYSCLK/TIMER_PRESCALER/1000 timer ticks per millisecond.
#define TIMER_TICKS_PER_MS SYSCLK / TIMER_PRESCALER / 1000

// Note: LED_TOGGLE_RATE*TIMER_TICKS_PER_MS should not exceed 65535 (0xFFFF)
// for the 16-bit timer

#define AUX1 TIMER_TICKS_PER_MS *LED_TOGGLE_RATE
#define AUX2 -AUX1
#define AUX3 AUX2 & 0x00FF
#define AUX4 ((AUX2 & 0xFF00) >> 8)

#define TIMER0_RELOAD_HIGH AUX4 // Reload value for Timer0 high byte
#define TIMER0_RELOAD_LOW AUX3  // Reload value for Timer0 low byte

#define SAMPLE_RATE_DAC 5000 //0L        // DAC sampling rate in Hz

sbit INT1 = P0 ^ 2; //0的时候按键没用

//#define DataPort P2

sfr16 DAC1 = 0xd5; // DAC1数据



void FunctionCallback(uchar index,uchar button);
void MainInterfaceShow();
#endif