#include "menu.h"
#include "lcd.h"
#include "string.h"
uchar code dir[] = {"  <="};

void InitMenu(Menu *menu, uchar *templat, uchar max_i)
{
    int i, j;
    for (i = 0; i < max_i; ++i)
    {
        for (j = 0; j < 13; ++j)
        {
            menu->menu_str[i][j] = *templat++;
        }
    }
    menu->max_index = max_i;
    menu->menu_index = 0;
}

// up:1,down 0
void MenuUpate(Menu *menu, uchar direction)
{
    uchar str[17];
    memset(str, '\0', sizeof(str));
    direction = direction == 0 ? 0 : direction / direction;
    LineClear(menu->menu_index);
    WriteStr(menu->menu_index, 0, menu->menu_str[menu->menu_index]);

    if (direction)
    {

        menu->menu_index = (menu->menu_index + 1) % menu->max_index;
    }
    else
    {
        menu->menu_index = (menu->menu_index + menu->max_index - 1) % menu->max_index;
    }
    strcpy(str, menu->menu_str[menu->menu_index]);
    WriteStr(menu->menu_index, 0, strcat(str, dir)); //"  <="
}
void MenuShow(Menu *menu)
{
    uchar i;
    uchar str[17];
    menu->menu_index = 0;
    LcdClear();
    for (i = 0; i < menu->max_index; ++i)
    {
        WriteStr(i, 0, menu->menu_str[i]);
    }
    memset(str, '\0', sizeof(str));
    strcpy(str, menu->menu_str[menu->menu_index]);
    WriteStr(menu->menu_index, 0, strcat(str, dir)); //"  <="
}
