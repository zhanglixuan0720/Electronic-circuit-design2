#include <C8051F020.h>
#include "port.h"

//---------------------------------------------
//端口初始化
//---------------------------------------------
void PORT_Init(void)
{
    XBR0 = 0X00; //设置XBR0
    XBR1 = 0X10; //设置XBR1
    XBR2 = 0X44; //打开交叉开关和弱上拉
    P74OUT &= 0xcc;
    P2MDOUT |= 0x7F;
    P0MDOUT |= 0x03;
    P1MDOUT = 0xFF; //端口1设置为推挽输出
    P2MDOUT = 0xFF; //端口2设置为推挽输出
    P3MDOUT = 0xFF; //端口3设置为推挽输出
}

void Port_IO_Init()
{
    //set P4[1] to push_pull model
    P74OUT |= 0x01;

    //enable crossbar
    XBR2 |= 0x40;

    //close the alam P4.1
    P4 &= 0xfd;

    //uart1,int1
    XBR1 |= 0x10;
    XBR2 |= 0x44;
}
