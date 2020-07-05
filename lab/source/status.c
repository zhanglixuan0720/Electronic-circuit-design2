#include "status.h"
#include "menu.h"
#include "lcd.h"
#include "image.h"
#include "queue.h"

extern Queue points;

extern Menu menu0;
extern Menu menu01;
extern Menu menu02;
extern uchar function_index;
extern char temperature_reference;
extern double DeltaPos;

extern uchar cure_enable;
uchar temperature_str[] = {"预警温度值:050℃"};


void FunctionCallback(uchar index, uchar button)
{
	switch (index)
	{
	case 0:
		Menu0Callback(button);
		break;
	case 1:
		AuthorInformCallback(button);
		break;
	case 2:
		Menu01Callback(button);
		break;
	case 3:
		MainInterfaceCallback(button);
		break;
	case 4:
		CureCallback(button);
		break;
	case 5:
		TemperatureCallback(button);
	    break;
	case 6:
		Menu02Callback(button);
		break;
	case 7:
		MainInterfaceCallback(button);
		break;
	case 8:
	case 9:
		CureCallback(button);
		break;
	default:
		return;
	}
}

void Menu0Callback(uchar button)
{
	switch (button)
	{
	case 0:
		Function0Ensure();
		break;
	case 1:
		MenuUpate(&menu0, 0);
		break;
	case 2:
		MenuUpate(&menu0, 1);
		break;
	default:
		return;
	}
}
void AuthorInformShow()
{
	LcdClear();
	WriteStr(0, 0, "NAME：张立璇");
	WriteStr(1, 0, "CLASS:自动化74");
	WriteStr(2, 0, "ID:  2176112682");
	WriteStr(3, 0, "TEL: 15399257039");
}
void Function0Ensure()
{
	switch (menu0.menu_index)
	{
	case 0:
		AuthorInformShow();
		function_index = 1;
		break;
	case 1:
		//main interface of function 01
		MainInterface1Show();
		function_index = 3;
		break;
	case 2:
		//main interface of function 02
		MainInterface2Show();
		function_index = 7;
		EIE2 |=0x04;
		break;
		//??????
	default:
		return;
	}
}

void AuthorInformCallback(uchar button)
{
	if (button == 0)
	{
		LcdClear();
		MenuShow(&menu0);
		function_index = 0;
	}
}

void Menu01Callback(uchar button)
{
	switch (button)
	{
	case 0:
		Function01Ensure();
		break;
	case 1:
		MenuUpate(&menu01, 0);
		break;
	case 2:
		MenuUpate(&menu01, 1);
		break;
	default:
		return;
	}
}

void Function01Ensure()
{
	switch (menu01.menu_index)
	{
	case 0:
		//main interface of function 01
		MainInterface1Show();
		function_index = 3;
		break;
	case 1:
		LcdClear();
		ClearQueue(&points);
		cure_enable = 1;
		function_index = 4;
		break;
	case 2:
		TemperatureSet();
		function_index = 5;
		break;
	case 3:
		MenuShow(&menu0);
		function_index = 0;
	default:
		return;
	}
}
void TemperatureSet()
{
	//LcdClear();
	WriteStr(0, 0, "为了您的安全，请");
	WriteStr(1, 0, "谨慎选择预警温度");
	WriteStr(2, 0, temperature_str);
	WriteStr(3, 0, "****************");
}

void Menu02Callback(uchar button)
{
	switch (button)
	{
	case 0:
		Function02Ensure();
		break;
	case 1:
		MenuUpate(&menu02, 0);
		break;
	case 2:
		MenuUpate(&menu02, 1);
		break;
	default:
		return;
	}
}

void Function02Ensure()
{
	switch (menu02.menu_index)
	{
	case 0:
		//main interface of function 02
		MainInterface2Show();
		function_index = 7;
		break;
	case 1:
		LcdClear();
		ClearQueue(&points);
		cure_enable = 2;
		function_index = 8;
		break;
	case 2:
		LcdClear();
		ClearQueue(&points);
		cure_enable = 3;
		function_index = 9;
		break;
	case 3:
		EIE2 &=~0x04;
		MenuShow(&menu0);
		function_index = 0;
		DeltaPos=0;
	default:
		return;
	}
}

void MainInterfaceCallback(uchar button)
{
	LcdClearHalf(1);
	if (function_index == 3)
	{
		MenuShow(&menu01);
		function_index = 2;
	}
	else if (function_index == 7)
	{
		MenuShow(&menu02);
		function_index = 6;
	}
}

void CureCallback(uchar button)
{
	if (button == 0)
	{
		cure_enable = 0;
		LcdClearHalf(0);
		LcdClearHalf(1);
		switch (function_index)
		{
		case 4:
			function_index = 2;
			MenuShow(&menu01);
			break;
		case 8:
		case 9:
			function_index = 6;
			MenuShow(&menu02);
			break;
		default:
			return;
		}
	}
}

void TemperatureCallback(uchar button)
{
	switch (button)
	{
	case 0:
		MenuShow(&menu01);
		function_index = 2;
		break;
	case 1:
		++temperature_reference;
		TemperatureFresh();
		break;
	case 2:
		--temperature_reference;
		TemperatureFresh();
		break;
	default:
		return;
	}
}

void TemperatureFresh()
{
	uchar temp = temperature_reference;
	temperature_str[11] = temp / 100 + '0';
	temp = temp - (temperature_str[11] - '0') * 100;
	temperature_str[12] = temp / 10 + '0';
	temp = temp - (temperature_str[12] - '0') * 10;
	temperature_str[13] = temp + '0';
	WriteStr(2, 0, temperature_str);
}

void MainInterface1Show()
{
	LcdClear();
	WriteStr(0, 0, "欢迎使用");
	WriteStr(1, 0, "X-ZLX001");
	WriteStr(2, 0, "温度检测");
	WriteStr(3, 0, "预警系统");
	ImageShow(gImage_temperature);
}
void MainInterface2Show()
{
	LcdClear();
	WriteStr(0, 0, "欢迎使用");
	WriteStr(1, 0, "X-ZLX000");
	WriteStr(2, 0, "飞行稳定");
	WriteStr(3, 0, "控制系统");
	ImageShow(gImage_fly);
}
