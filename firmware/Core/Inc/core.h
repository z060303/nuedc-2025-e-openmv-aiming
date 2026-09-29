//
// Created by ZHENG on 26-3-11.
//

#ifndef CORE_H
#define CORE_H

#include <stdio.h>
#include "main.h"
#include "PID.h"
#include "tim.h"
#include"usart.h"
#include "Emm_v5.h"
void Tilt(int cur_x,int cur_y);
void yawbujin360_Trun(float *Cur_Angle,PID_t PID_y);
#endif //CORE_H
