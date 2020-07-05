#ifndef PID_H
#define PID_H
typedef struct
{
    float Kp; //比例系数Proportional
    float Ki; //积分系数Integral
    float Kd; //微分系数Derivative

    float Ek;  //当前误差
    float Ek1; //前一次误差 e(k-1)
    float Ek2; //再前一次误差 e(k-2)
} PIDIncStruct;

float PID_Inc(float SetValue, float ActualValue, PIDIncStruct *PID);
#endif