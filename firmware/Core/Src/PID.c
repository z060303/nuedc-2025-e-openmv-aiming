//
// Created by ZHENG on 25-12-29.
//

#include "PID.h"

void PID_Init(PID_t *p)
{
    p->Target = 0;                  //目标值
    p->Actual = 0;                  //当前值
    p->Out = 0;                     //输出值
    p->Error0 = 0;                  //当前值与目标值的误差
    p->Error1 = 0;                  //上一次当前值与目标值的误差差
    p->ErrorInt = 0;                //误差累计值
    p->LastError = 0;
}

void PID_Update(PID_t *p)
{
    p->Error1 = p->Error0;
    p->Error0 = p->Target - p->Actual;

    if (p->Ki != 0)
    {
        p->ErrorInt += p->Error0;
    }
    else
    {
        p->ErrorInt = 0;
    }

    p->Out = p->Kp * p->Error0
           + p->Ki * p->ErrorInt
           + p->Kd * (p->Error0 - p->LastError);

    // if (p->Out > p->OutMax) {p->Out = p->OutMax;}
    // if (p->Out < p->OutMin) {p->Out = p->OutMin;}
}
