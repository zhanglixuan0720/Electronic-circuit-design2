#ifndef LCD_H
#define LCD_H
#include "queue.h"
void Lcd_Port_Init(void);
void LcdInit(void);


void SendByte(uchar Dbyte);
uchar ReceiveByte(void);
void CheckBusy(void);

void WriteData(uchar WDLCM);
void WriteCommand(uchar Cbyte);

void LineClear(uchar l);
void LcdClear(void);

void WriteStr(uchar row, uchar col, uchar *puts);

void ImageShow(uchar *imagePtr);


void drawPoint2(uchar x_index,uchar y,unsigned char tempH,unsigned char tempL, uchar color);
void LcdClearHalf(uchar direction);

void ScreenShow(uchar *imagePtr,uchar *reference,uchar direction);
#endif