#ifndef MENU_H
#define MENU_H
#include "def_com.h"
typedef struct
{
    uchar menu_str[4][13];
    uchar menu_index;
    uchar max_index;
} Menu;
void InitMenu(Menu *menu, uchar *templat, uchar max_i);
void MenuUpate(Menu *menu, uchar direction);
void MenuShow(Menu *menu);
#endif