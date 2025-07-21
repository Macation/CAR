#include <LQ_PID.h>

pid_param_t Servo_Loc_PID;
pid_param_t Motor_Inc_PID;
pid_param_t Servo_Loc_PID_tmp;
float temp_servo_kp;
//600-800
float K_Dir_Table[10]={5,4,4,4};
int Look_Line_Table[10]={80,80,80,75};
float Servo_KP_Para_Table[10]={2.5,2.5,2.5,2.2};
float Servo_KI_Para_Table[10]={0  ,0  ,0  ,0  };
float Servo_KD_Para_Table[10]={4  ,4  ,4  ,4  };
float Servo_KP2_Para_Table[10]={0  ,0  ,0  ,0  };
float Servo_KD2_Para_Table[10]={0  ,0  ,0  ,0  };

//将Speed的值映射到数组的索引值
char Map_Value(int Speed)
{
    char index;
    index=(Speed-600)/50;
    if(index<0){index=0;}
    return index;
}

void Set_Para_Value(int Speed)
{
    K_Dir=K_Dir_Table[Map_Value(Speed)];
    Look_Line=Look_Line_Table[Map_Value(Speed)];

    Servo_Loc_PID.kp=Servo_KP_Para_Table[Map_Value(Speed)];
    Servo_Loc_PID.ki=Servo_KI_Para_Table[Map_Value(Speed)];
    Servo_Loc_PID.kd=Servo_KD_Para_Table[Map_Value(Speed)];
    Servo_Loc_PID.kp2=Servo_KP2_Para_Table[Map_Value(Speed)];
    Servo_Loc_PID.kd2=Servo_KD2_Para_Table[Map_Value(Speed)];
    Servo_Loc_PID.kp=Servo_KP_Para_Table[Map_Value(Speed)];
}


void Set_PID(void)
{
    PidInit(&Servo_Loc_PID);
    PidInit(&Motor_Inc_PID);
    Pid_Value();
//    E2PROM_Write_PID();
}

void Limit_PID(void)
{
    if(Servo_Loc_PID.kp<0){Servo_Loc_PID.kp=0;}
    if(Servo_Loc_PID.ki<0){Servo_Loc_PID.ki=0;}
    if(Servo_Loc_PID.kd<0){Servo_Loc_PID.kd=0;}
    if(Servo_Loc_PID.kp2<0){Servo_Loc_PID.kp2=0;}
    if(Servo_Loc_PID.kd2<0){Servo_Loc_PID.kd2=0;}

    if(Motor_Inc_PID.kp<0){Motor_Inc_PID.kp=0;}
    if(Motor_Inc_PID.ki<0){Motor_Inc_PID.ki=0;}
    if(Motor_Inc_PID.kd<0){Motor_Inc_PID.kd=0;}

    if(k_qz<0){k_qz=0;}else if(k_qz>=0.5){k_qz=0.5;}
    if(sp<1){sp=1;}else if(sp>=3){sp=3;}
    if(threshold<0){threshold=0;}else if(threshold>255){threshold=255;}
    if(K_Dir<0){K_Dir=0;}
}

void Pid_Value(void)
{
    //舵机转向环
    Servo_Loc_PID.kp=2.4;
    Servo_Loc_PID.ki=0;
    Servo_Loc_PID.kd=4.5;   //1
    Servo_Loc_PID.kp2=0;
    Servo_Loc_PID.kd2=0;

    //舵机临时转向环
    Servo_Loc_PID_tmp.kp=2.4;
    Servo_Loc_PID_tmp.ki=0;
    Servo_Loc_PID_tmp.kd=4;   //1
    Servo_Loc_PID_tmp.kp2=0;
    Servo_Loc_PID_tmp.kd2=0;


    //电机速度环
    Motor_Inc_PID.kp=11;
    Motor_Inc_PID.ki=0.41;
    Motor_Inc_PID.kd=1;

    temp_servo_kp=Servo_Loc_PID.kp;

    Look_Line=75;
    K_Dir=0;
    sp=0.0;
    k_qz=0;
    Refer_Speed=500;
    threshold=100;

    My_Speed_up.Straight_Speed_up=0;
    My_Speed_up.Small_Curve_Speed_up=0;
    My_Speed_up.Left_Circle_Speed_up=0;
    My_Speed_up.Curve_Speed_up=0;
    My_Speed_up.zuo_up=0;
}

/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：限幅函数
 *  参数说明：
  * @param    amt   ： 参数
  * @param    low   ： 最低值
  * @param    high  ： 最高值
 *  函数返回：无
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float constrain_float(float amt, float low, float high)
{
    return ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));
}

// pid参数初始化函数
void PidInit(pid_param_t * pid)
{
    pid->kp        = 0;
    pid->ki        = 0;
    pid->kd        = 0;
    pid->imax      = 0;
    pid->out_p     = 0;
    pid->out_i     = 0;
    pid->out_d     = 0;
    pid->out       = 0;
    pid->integrator= 0;
    pid->last_error= 0;
    pid->last_derivative   = 0;
    pid->last_t    = 0;
}


/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid位置式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果
 *  修改时间：2020年4月1日
 *  备    注：位置式舵机方向环
 *************************************************************************/
float PidLocCtrl(pid_param_t * pid, float error)
{
    /* 累积误差 */
    pid->integrator += error;

    /* 误差限幅 */
    constrain_float(pid->integrator, -pid->imax, pid->imax);

    pid->out_p = pid->kp * error;
    pid->out_i = pid->ki * pid->integrator;
    pid->out_d = pid->kd * (error - pid->last_error);

    pid->last_error = error;

    pid->out = pid->out_p + pid->out_i + pid->out_d;

    return pid->out;
}

float absfloat(float num)
{
    return num<0?(-num):num;
}


float Servo_PidLocCtrl(pid_param_t * pid, float error)
{
    pid->out_p = pid->kp * error;
    pid->out_d = pid->kd * (error - pid->last_error);

    pid->last_error = error;

    pid->out_p2 = absfloat(error)*error * pid->kp2;

    pid->out = pid->out_p + pid->out_d + pid->out_p2 ;
            //+ Yaw * pid->kd2

    return pid->out;
}


/*************************************************************************
 *  函数名称：float constrain_float(float amt, float low, float high)
 *  功能说明：pid增量式控制器输出
 *  参数说明：
  * @param    pid     pid参数
  * @param    error   pid输入误差
 *  函数返回：PID输出结果   注意输出结果已经包涵了上次结果
 *  修改时间：2020年4月1日
 *  备    注：
 *************************************************************************/
float PidIncCtrl(pid_param_t * pid, float error)
{
    pid->out_p = pid->kp * (error - pid->last_error);
    pid->out_i = pid->ki * error;
    pid->out_d = pid->kd * ((error - pid->last_error) - pid->last_derivative);

    pid->last_derivative = error - pid->last_error;
    pid->last_error = error;

    pid->out += pid->out_p + pid->out_i + pid->out_d;

    return pid->out;
}

