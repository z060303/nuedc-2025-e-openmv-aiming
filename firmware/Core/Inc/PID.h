//
// Created by ZHENG on 25-12-29.
//

#ifndef __PID_H
#define __PID_H

typedef struct {
    int Target;               //目标值
    int Actual;               //当前值
    float Out;                  //输出值

    float Kp;                   //比例项
    float Ki;                   //积分项
    float Kd;                   //微分项

    int LastError;            //上次误差
    int Error0;               //当前误差
    int Error1;               //当前误差减去上次误差（微分误差）
    int ErrorInt;             //累计误差（积分误差）
    //输出限幅
    float OutMax;               //最大输出值
    float OutMin;               //最小输出值
} PID_t;

void PID_Init(PID_t *p);
void PID_Update(PID_t *p);

#endif

