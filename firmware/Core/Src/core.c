//
// Created by ZHENG on 26-3-11.
//

#include "core.h"

#include <tgmath.h>

#define Cen_x 160
#define Cen_y 120
#define CenArea 20
#define DEAD_ZONE 2        // 像素死区（防抖）
#define MAX_SPEED 200     // 最大速度（根据你的电机调整）
#define MIN_SPEED 20      // 最小启动速度
#define FILTER_K 0.7f      // 平滑系数（0~1，越大越平滑）

extern float bujin_angle1;
extern float Servo_angle2;
extern PID_t x_PID;
extern PID_t y_PID;
extern int flag;
int Dir = 0;

void Tilt(int cur_x,int cur_y) {

    //检测到目标
    if (flag) {
        //x_PID参数
        x_PID.Target = Cen_x;
        x_PID.Actual = cur_x;
        x_PID.LastError = x_PID.Error0;

        //y_PID参数
        y_PID.Target = Cen_y;
        y_PID.Actual = cur_y;
        y_PID.LastError = y_PID.Error0;

        //PID计算
        PID_Update(&x_PID);
        PID_Update(&y_PID);
        printf("%d %d\r\n",(int)(x_PID.Out * 100),(int)(y_PID.Out * 100));

        //对当前角度进行限幅
        if (Servo_angle2 < 0)Servo_angle2 = 0;
        if (Servo_angle2 > 270)Servo_angle2 = 270;

        //舵机转动
        PitchServo270_Turn(&Servo_angle2,y_PID);

        //步进电机转动
        yawbujin360_Trun(&bujin_angle1,x_PID);
    }
    //未检测到目标
    else {
        x_PID.Error0 = 0;
        x_PID.ErrorInt = 0;  // 死区内清零积分，避免残留
        x_PID.Out = 0;
        y_PID.Error0 = 0;
        y_PID.ErrorInt = 0;  // 死区内清零积分，避免残留
        y_PID.Out = 0;
        y_PID.Actual = Cen_y;
        x_PID.Actual = Cen_x;
        Servo270_Stop(&bujin_angle1,&Servo_angle2);
    }

}

//步进电机控制
void yawbujin360_Trun(float *Cur_Angle, PID_t x_PID)
{
    static float speed_filter = 0;   // 低通滤波后的速度

    float error = x_PID.Target - x_PID.Actual;

    // ===== 1. 死区处理（防止抖动）=====
    if (error < DEAD_ZONE && error > -DEAD_ZONE)
    {
        Emm_V5_Vel_Control(1, 0, 0, 10, 0); // 停止
        return;
    }

    // ===== 2. PID输出作为速度 =====
    float speed = x_PID.Out;

    // ===== 3. 限幅 =====
    if (speed > MAX_SPEED) speed = MAX_SPEED;
    if (speed < -MAX_SPEED) speed = -MAX_SPEED;

    // ===== 4. 最小速度补偿（避免卡住）=====
    if (speed > 0 && speed < MIN_SPEED) speed = MIN_SPEED;
    if (speed < 0 && speed > -MIN_SPEED) speed = -MIN_SPEED;

    // ===== 5. 低通滤波（防抖+平滑）=====
    speed_filter = FILTER_K * speed_filter + (1 - FILTER_K) * speed;

    // ===== 6. 方向判断 =====
    uint8_t dir;
    if (speed_filter > 0)
        dir = 0;   // CW
    else
        dir = 1;   // CCW

    // ===== 7. 取绝对值作为速度 =====
    uint16_t send_speed = (uint16_t)(fabs(speed_filter)) /10;

    // ===== 8. 发送控制命令 =====
    Emm_V5_Vel_Control(1, dir, send_speed, 10, 0);
}

