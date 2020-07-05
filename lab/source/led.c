#include "def_com.h"
#include "led.h"
//0,1,2,3,4,5,6,7,8,9,-
uchar code digitron_num[] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf};

void digit_display(uchar x, uchar y, char num)
{
    uchar digit_num = digitron_num[num & 0x0f];
    P5 = 0xff;
    P6 = 0xff;
    switch (x) //确定行列
    {
    case 0:
        P5 &= ~(1 << y);
        break;
    case 1:
        P5 &= ~(1 << (y + 4));
        break;
    case 2:
        P6 &= ~(1 << y);
        break;
    default:
        return;
    }
    if (num & 0xf0) //确定数字
    {
        digit_num &= 0x7f;
    }
    P7 = digit_num;
}


void Led_display(uchar row, float num)
{
    uchar sign = 0, digits = 4, digitbit = 0;
    int i = 1000, integer = 0;
    float decimal = 0;
    if (num < 0) //符号判断
    {
        sign = 1;
        num *= -1;
    }
    integer = (int)num;
    decimal = num - integer; //小数部分提取
    if (integer)             //判断位数
    {
        while (integer / i == 0)
        {
            digits -= 1;
            i /= 10;
        }
    }
    else
    {
        digits = 1;
    }
		
    if (sign) //溢出退回，否则显示符号
    {
        if (digits > 3)
        {
            return;
        }
        digit_display(row, digitbit, 10);
        ++digitbit;
    }
    else
    {
        if (digits > 4)
        {
            return;
        }
    }
    while (digits-- != 0) //整数部分显示
    {
        if (i == 1)
        {
            digit_display(row, digitbit, (integer / i) | 0xf0);
        }
        else
        {
            digit_display(row, digitbit, integer / i);
        }
				integer = integer % i;
        i /= 10;
        ++digitbit;
    }
		
    i = 1;
    while (digitbit != 4) //小数部分显示
    {
        i *= 10;
        digit_display(row, digitbit, (int)(decimal * 10));
		decimal = decimal * 10 - (int)(decimal * 10);
        ++digitbit;
    }
}

void Led_Port_Init()
{
    //set to push_pull model
    P74OUT &= 0xff;

    //enable crossbar
    XBR2 |= 0x40;
}

void LedInit()
{
    Led_Port_Init();
    P5 |= 0xff;
    P6 |= 0x0f;
    P7 = 0xff;
}