#include "pid.h"

float PID_Inc(float SetValue, float ActualValue, PIDIncStruct *PID)
{
    float pid_inc; //增量

    PID->Ek = SetValue - ActualValue;
    pid_inc = (PID->Kp * PID->Ek) + (PID->Ki * PID->Ek1) + (PID->Kd * PID->Ek2);

    PID->Ek2 = PID->Ek1;
    PID->Ek1 = PID->Ek;
    return pid_inc;
}