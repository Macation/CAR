#ifndef SRC_APPSW_TRICORE_USER_LQ_PID_H_
#define SRC_APPSW_TRICORE_USER_LQ_PID_H_

#include <stdint.h>
#include <stdlib.h>
#include <LQ_CCU6.h>

typedef struct
{
    float                kp;         //P
    float                ki;         //I
    float                kd;         //D
    float                kp2;        //舵机二次项
    float                kd2;        //陀螺仪
    float                imax;       //积分限幅

    float                out_p;  //KP输出
    float                out_p2; //KP2输出
    float                out_i;  //KI输出
    float                out_d;  //KD输出
    float                out_d2; //KD2输出
    float                out;    //pid输出

    float                integrator; //< 积分值
    float                last_error; //< 上次误差
    float                last_derivative;//< 上次误差与上上次误差之差
    unsigned long        last_t;     //< 上次时间
}pid_param_t;

extern float groyz;
extern float temp_servo_kp;
extern pid_param_t Servo_Loc_PID;
extern pid_param_t Motor_Inc_PID;
extern pid_param_t Servo_Loc_PID_tmp;

extern float K_Dir_Table[];
extern int Look_Line_Table[];
extern float Servo_KP_Para_Table[];
extern float Servo_KI_Para_Table[];
extern float Servo_KD_Para_Table[];
extern float Servo_KP2_Para_Table[];
extern float Servo_KD2_Para_Table[];

void Set_PID(void);

char Map_Value(int Speed);

void Limit_PID(void);

void Set_Para_Value(int Speed);

void Pid_Value(void);

float absfloat(float num);

void PidInit(pid_param_t * pid);

float constrain_float(float amt, float low, float high);

float PidLocCtrl(pid_param_t * pid, float error);

float Servo_PidLocCtrl(pid_param_t * pid, float error);

float PidIncCtrl(pid_param_t * pid, float error);

#endif /* SRC_APPSW_TRICORE_USER_LQ_PID_H_ */
