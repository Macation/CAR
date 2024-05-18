/*
 * My_Control_Code.h
 * 存放原本存储在CCU6中的代码
 *  Created on: 2024年4月21日
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_CONTROL_CODE_H_
#define SRC_APPSW_TRICORE_USER_MY_CONTROL_CODE_H_

#include <My_Key.h>
#include <LQ_PID.h>
#include <LQ_CAMERA.h>
#include <LQ_MotorServo.h>
#include <Platform_Types.h>
#include <image_Handle.h>
#include <My_UART.h>
#include "image1.h"

extern uint8 Key_Value;
extern uint8 Select_PID;
extern float Motor_IncPID;
extern float Motor_DirPId;                   //
extern int Current_Speed1;        //速度单位m/s
extern int Current_Speed2;
extern float Current_Speed;
extern volatile sint16 LPulse;                 //
extern volatile sint16 YPulse;                 //
extern int Servo_duty;
extern uint8 Motor_openFlag;         //电机启动标志
extern uint8 Servo_openFlag;         //舵机启动标志
extern short Motor_duty1;
extern short Motor_duty2;
extern float Target_Speed;

void Modify_PID(void);
void Key_Control(void);
void Motor_Control(void);
void Servo_Control(void);
void Camera_Control(void);
int Low_Pass_Filter(int encinput,int* Feedback_enc,long int* Feedback_enc_tmp);
void System_Control(void);
extern float Servo_LocPid;
#endif /* SRC_APPSW_TRICORE_USER_MY_CONTROL_CODE_H_ */
