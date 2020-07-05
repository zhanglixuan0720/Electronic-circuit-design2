#ifndef LED_H
#define LED_H
void digit_display(uchar x, uchar y, char num); //数码管显示 左上为0,0 加点额外给高四位置1
void Led_display(uchar row, float num);         //数码管数字显示 可以智能显示正负小数
void Led_Port_Init();                           //数码管接口初始化
void LedInit();                                 //数码管初始化
#endif