#ifndef STATUS_H
#define STATUS_H
#include "def_com.h"

void FunctionCallback(uchar index, uchar button);

void Menu0Callback(uchar button); // status 0 callback;
void AuthorInformShow();
void Function0Ensure();
void AuthorInformCallback(uchar button); // status 1 callback;

void Menu01Callback(uchar button); // status 2 callback;
void Function01Ensure();
void TemperatureSet();

void Menu02Callback(uchar button); // status 6 callback;
void Function02Ensure();

void MainInterfaceCallback(uchar button); // status 3 7 callback;
void CureCallback(uchar button);          // status 4 8 9 callback;

void TemperatureCallback(uchar button); //satus 5 callback;
void TemperatureFresh();

// MainInterface
void MainInterface1Show();
void MainInterface2Show();
#endif