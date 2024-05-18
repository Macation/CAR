/*
 * My_Control_Code.c
 *  Created on: 2024
 *      Author: dduu
*/
#include "My_Control_Code.h"

uint8 Key_Value=0;
uint8 Select_SorM=0;
uint8 Select_PID=0;
float Motor_IncPID=0;
float Motor_DirPId=0;                   //
float Current_Speed=0;
int Current_Speed1=0;        //速度单位m/s
long int Current_Tmp_Speed1=0;        //速度单位m/s
int Current_Speed2=0;
long int Current_Tmp_Speed2=0;        //速度单位m/s
volatile sint16 LPulse;                 //
volatile sint16 YPulse;                 //
int Servo_duty=Servo_Center_Mid;
uint8 Motor_openFlag=0;         //电机启动标志
uint8 Servo_openFlag=0;         //舵机启动标志
short Motor_duty1 = 0;
short Motor_duty2 = 0;
float kp=0;
float Target_Speed=0; //目标速度m/s
float Servo_LocPid;  //方向环误差PID
void System_Control(void)
{
    static long int ms=0;
    //控制舵机
    if(ms%5==0)
    {
        Servo_Loc_error=error1;
        Servo_LocPid=PidLocCtrl(&Servo_Loc_PID,(float)Servo_Loc_error);   //使用P小的PID
        //Servo_LocPid=PidLocCtrl(&Servo_Loc_PID,(float)Servo_Loc_error)+EKP*Servo_QZ_ERR;

        Servo_duty=(int)(Servo_Center_Mid + Servo_LocPid);

        if (Servo_duty >= Servo_Left_Max)                  //限制幅值
            Servo_duty = Servo_Left_Max;
        else if (Servo_duty <= Servo_Right_Min)            //限制幅值
            Servo_duty = Servo_Right_Min;

        if(Servo_openFlag==1)
        {
            ATOM_PWM_InitConfig(ATOMSERVO1, Servo_duty, 100);  //舵机频率为100HZ，初始值为1.5ms中值
        }
        else
        {
            ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);  //舵机频率为100HZ，初始值为1.5ms中值
        }
        Motor_DirPId=kp*Servo_LocPid;
        Motor_duty1=(int)(Motor_IncPID-Motor_DirPId)*0.94;  //0.94
        Motor_duty2=(int)(Motor_IncPID+Motor_DirPId)*1.06;  //1.06
        if(Motor_duty1 > 4000)Motor_duty1 = 4000;else if(Motor_duty1 < 0)Motor_duty1 = 0;
        if(Motor_duty2 > 4000)Motor_duty2 = 4000;else if(Motor_duty2 < 0)Motor_duty2 = 0;
        MotorCtrl(Motor_duty2,Motor_duty1);
    }
    if(ms%10==0)
    {
        LPulse = -ENC_GetCounter(ENC4_InPut_P02_8); // 左电机，小车前进为负值
         YPulse = ENC_GetCounter(ENC2_InPut_P33_7);  // 右电机，小车前进为正值

         //控制电机同样是5ms
         /* 获取编码器值 */

     //        Current_Speed1=LPulse*0.87;        //编译器旋转一圈，小车移动30/50*4.95*3.14=9.3258cm  速度*100   原本为0.0087，目前100的速度相当于1m/s
     //        Current_Speed2=YPulse*0.87;      //YPulse/1024.0*12.0576/100.0/0.01/1.357 编码器的值*100
         Low_Pass_Filter((int)LPulse*0.87, &Current_Speed1, &Current_Tmp_Speed1);
         Low_Pass_Filter((int)YPulse*0.87, &Current_Speed2, &Current_Tmp_Speed2);
         Current_Speed=(Current_Speed1+Current_Speed2)/2.0;

         //转向环PID


         //没加差速


         //速度环PID
         Motor_IncPID=PidIncCtrl(&Motor_Inc_PID,(float)(Target_Speed-Current_Speed));

         //电机限幅
         if(Motor_IncPID > 4000)Motor_IncPID = 4000;else if(Motor_IncPID < 0)Motor_IncPID = 0;
         if(Motor_Inc_PID.out > 4000)Motor_Inc_PID.out = 4000;else if(Motor_Inc_PID.out < 0)Motor_Inc_PID.out = 0;

         //转向和电机驱动融合并限幅
//         Motor_IncPID=2000;

         //给电机PWM信号
 //该函数第一个形参是右轮M2，第二个形参是左轮M1
         if(Motor_openFlag==1)
         {
             Target_Speed=150;
         }
         else if(Motor_openFlag==0)
         {
     //            Motor_Inc_PID.out=0;
     //            Motor_Inc_PID.last_derivative=0;
     //            Motor_Inc_PID.last_error=0;
     //            Current_Tmp_Speed1=0;  //低通滤波积分项清零
     //            Current_Tmp_Speed2=0;
             Target_Speed=0;
         }

    }
    ms++;
    if(ms>50000)
    {
       ms=0;
    }

}

void Motor_Control(void)
{
        LPulse = -ENC_GetCounter(ENC4_InPut_P02_8); // 左电机，小车前进为负值
        YPulse = ENC_GetCounter(ENC2_InPut_P33_7);  // 右电机，小车前进为正值

        //控制电机同样是5ms
        /* 获取编码器值 */

//        Current_Speed1=LPulse*0.87;        //编译器旋转一圈，小车移动30/50*4.95*3.14=9.3258cm  速度*100   原本为0.0087，目前100的速度相当于1m/s
//        Current_Speed2=YPulse*0.87;      //YPulse/1024.0*12.0576/100.0/0.01/1.357 编码器的值*100
        Low_Pass_Filter((int)LPulse*0.87, &Current_Speed1, &Current_Tmp_Speed1);
        Low_Pass_Filter((int)YPulse*0.87, &Current_Speed2, &Current_Tmp_Speed2);
        Current_Speed=(Current_Speed1+Current_Speed2)/2.0;

        //转向环PID
//        if(StraightWay_flag)
//        {
//           Motor_DirPId=(float)10*Servo_Loc_error;
//        }
//        else
//        {
//           Motor_DirPId=(float)0.4*Servo_Loc_error;
//        }

//        Motor_DirPId=(float)0.4*Servo_Loc_error;
        //没加差速
        Motor_DirPId=0.0;

        //速度环PID
        Motor_IncPID=PidIncCtrl(&Motor_Inc_PID,(float)(Target_Speed-Current_Speed));

        //电机限幅
        if(Motor_IncPID > 4000)Motor_IncPID = 4000;else if(Motor_IncPID < 0)Motor_IncPID = 0;
        if(Motor_Inc_PID.out > 4000)Motor_Inc_PID.out = 4000;else if(Motor_Inc_PID.out < 0)Motor_Inc_PID.out = 0;

        //转向和电机驱动融合并限幅
//        Motor_IncPID=2500;
        Motor_duty1=(int)(Motor_IncPID+Motor_DirPId)*0.94;  //0.94
        Motor_duty2=(int)(Motor_IncPID-Motor_DirPId)*1.06;  //1.06
        if(Motor_duty1 > 4000)Motor_duty1 = 4000;else if(Motor_duty1 < 0)Motor_duty1 = 0;
        if(Motor_duty2 > 4000)Motor_duty2 = 4000;else if(Motor_duty2 < 0)Motor_duty2 = 0;

        //给电机PWM信号
        MotorCtrl(Motor_duty2,Motor_duty1);  //该函数第一个形参是右轮M2，第二个形参是左轮M1
        if(Motor_openFlag==1)
        {
            Target_Speed=150;
        }
        else if(Motor_openFlag==0)
        {
//            Motor_Inc_PID.out=0;
//            Motor_Inc_PID.last_derivative=0;
//            Motor_Inc_PID.last_error=0;
//            Current_Tmp_Speed1=0;  //低通滤波积分项清零
//            Current_Tmp_Speed2=0;
            Target_Speed=0;
        }

    MotorCtrl(Motor_duty2,Motor_duty1);
}

void Servo_Control(void)
{
    //控制舵机
    float Servo_LocPid;  //方向环误差PID

//    Servo_Loc_error=(int)Err_Sum();

//    if(Servo_Loc_error>=10 && Servo_Loc_error<=-10)
//    {
//        Servo_Loc_PID.kp=40;
//    }
//    else
//    {
//        Servo_Loc_PID.kp=20;
//    }

    Servo_LocPid=PidLocCtrl(&Servo_Loc_PID,(float)Servo_Loc_error);   //使用P小的PID
    //Servo_LocPid=PidLocCtrl(&Servo_Loc_PID,(float)Servo_Loc_error)+EKP*Servo_QZ_ERR;

    Servo_duty=(int)(Servo_Center_Mid + Servo_LocPid);

    if (Servo_duty >= Servo_Left_Max)                  //限制幅值
        Servo_duty = Servo_Left_Max;
    else if (Servo_duty <= Servo_Right_Min)            //限制幅值
        Servo_duty = Servo_Right_Min;

    if(Servo_openFlag==1)
    {
        ATOM_PWM_InitConfig(ATOMSERVO1, Servo_duty, 100);  //舵机频率为100HZ，初始值为1.5ms中值
    }
    else
    {
        ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);  //舵机频率为100HZ，初始值为1.5ms中值
    }


}

void Camera_Control(void)
{
//    if(++LED1_Time == 50)   //10*50 = 500ms
//    {
//        LED1_Time = 0;
//        LED_Ctrl(LED1,RVS);     // LED
//    }

//        Cross_Detect();
    //    Right_Down_Find=Find_Right_Down_Point(LCDH-1,0);
    //    if(Right_Down_Find!=0)
    //    {
    //        Mon_change_Right_Line=Monotonicity_Change_Right(Right_Down_Find-5,0);
    //    }
    //    else
    //    {
    //        Mon_change_Right_Line=Monotonicity_Change_Right(LCDH-1,0);
    //    }

//        Island_Detect();

        for(int i=LCDH-1;i>=0;i--)
        {
            Middle_Line[i]=(Right_Line[i]+Left_Line[i])/2;
//            Bin_Image[i][Left_Line[i]]=0;
//            Bin_Image[i][Middle_Line[i]]=0;
//            Bin_Image[i][Right_Line[i]]=0;
        }
}

void Key_Control(void)
{
    Key_Check(0,Key,KEY0_Pin);
    Key_Check(1,Key,KEY1_Pin);
    Key_Check(2,Key,KEY2_Pin);
    Key_Check(3,Key,KEY3_Pin);
    Key_Check(4,Key,KEY4_Pin);

    if(Key[0].keyFlag==1)
    {
        Key[0].keyFlag=0;
        Key_Value=1;
    }
    else if(Key[1].keyFlag==1)
    {
        Key[1].keyFlag=0;
        Key_Value=2;
    }
    else if(Key[2].keyFlag==1)
    {
        Key[2].keyFlag=0;
        Key_Value=3;
    }
    else if(Key[3].keyFlag==1)
    {
        Key[3].keyFlag=0;
        Key_Value=4;
    }
    else if(Key[4].keyFlag==1)
    {
        Key[4].keyFlag=0;
        Key_Value=5;
    }
}

void Modify_PID(void)
{
    if(Key_Value==1)
    {
        Key_Value=0;
        data_change_flag=1; //修改了参数，需要写进E2PROM
        if(Select_SorM==0)  //选择Servo
        {
            switch(Select_PID)
            {
                case 0:  //P
                    Servo_Loc_PID.kp+=1;
                    break;
                case 1:  //I
                    kp+=0.2;
                    break;
                case 2:  //D
                    Servo_Loc_PID.kd+=1;
                    break;
            }
        }
        else if(Select_SorM==1) //选择Motor
        {
            switch(Select_PID)
            {
                case 0:  //P
                    Motor_Inc_PID.kp+=0.5;
                    break;
                case 1:  //I
                    Motor_Inc_PID.ki+=0.1;
                    break;
                case 2:  //D
                    Motor_Inc_PID.kd+=1;
                    break;
            }
        }
    }
    else if(Key_Value==2)
    {
        Key_Value=0;
        data_change_flag=1;
        if(Select_SorM==0)  //选择Servo
        {
            switch(Select_PID)
            {
                case 0:  //P
                    Servo_Loc_PID.kp-=0.5;
                    break;
                case 1:  //I
//                    Servo_Loc_PID.ki-=0.1;
                    kp-=0.2;
                    break;
                case 2:  //D
                    Servo_Loc_PID.kd-=1;
                    break;
            }
        }
        else if(Select_SorM==1) //选择Motor
        {
            switch(Select_PID)
            {
                case 0:  //P
                    Motor_Inc_PID.kp-=0.5;
                    break;
                case 1:  //I
                    Motor_Inc_PID.ki-=0.1;
                    break;
                case 2:  //D
                    Motor_Inc_PID.kd-=1;
                    break;
            }
        }
    }
    else if(Key_Value==3)  //选择调P还是I还是D
    {
        Key_Value=0;
        Select_PID++;
        Select_PID%=3;
    }
    else if(Key_Value==4)  //选择调Servo还是Motor
    {
        Key_Value=0;
        Select_PID=0;
        switch_flag=1; //页面更新标志
        Select_SorM++;
        Select_SorM%=3;
    }

    if(switch_flag==1){switch_flag=0;TFTSPI_CLS(u16BLACK);}  //更新页面

    if(Select_SorM==0){Show_ServoPid();}
    if(Select_SorM==1){Show_MotorIncPid();}    //显示调试信息
    if(Select_SorM==2){Show_Motor();}

}


int Low_Pass_Filter(int encinput,int* Feedback_enc,long int* Feedback_enc_tmp)
{
    int Feedback_enc_Now = 0;
    int tmp = 5;
    int tmp2=tmp*0.5;
    int tmp7;
    Feedback_enc_Now=encinput;                     //编码器当前值
    *Feedback_enc_tmp+=Feedback_enc_Now- *Feedback_enc; //编码器读数差求积
    Feedback_enc_Now=*Feedback_enc_tmp/tmp;                 //取1/20使编码器数值平滑
    tmp7=*Feedback_enc_tmp%tmp;
    if(tmp7>=tmp2)  *Feedback_enc = Feedback_enc_Now + 1;                  //速度值四舍五入
    if(tmp7<=-tmp2) *Feedback_enc = Feedback_enc_Now - 1;
    return *Feedback_enc;                                 //反馈滤波后的速度
}

