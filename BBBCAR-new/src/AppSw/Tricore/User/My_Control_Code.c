/*
 * My_Control_Code.c
 *  Created on: 2024
 *      Author: dduu
*/
#include "My_Control_Code.h"

//#define Test_Motor_Speed
#define DOUBLE_ERROR
#define Compete_Mode

int OSTU_threshold;
uint8 close_flag;
uint8 open_flag;
float test_out;
uint8 add_kpFlag;
float sp=1; //左转p系数
uint8 sp_change=0;
char Run_Mode=0;  //该值选择使用传统PID还是模糊PID
uint8 Key_Value=0;
uint8 Select_SorM=0;
uint8 Select_Mode=0;
float Motor_IncPID=0;
float Motor_IncPID1=0;
float Motor_IncPID2=0;
int Servo_duty=Servo_Center_Mid;
uint8 Motor_openFlag=0;         //电机启动标志
uint8 Servo_openFlag=0;         //舵机启动标志
uint8 thresholdflag=0;
short Motor_duty1 = 0;
short Motor_duty2 = 0;
int Look_Line=95;
int Target_Speed=0; //目标速度m/s,100即为1m/s
int Refer_Speed=0;  //参考速度
int Last_err=0;//上一次图像误差
int Last_last_err=0;//上上一次图像误差
float Image_kp=20;
float Image_kd=5;
short Dir_out;
float K_Dir=0;

int Test_err;
int Test_err_d;
int PID_flag_Direction;
uint8 Speed_up_flag;

volatile sint16 LPulse = 0;          // 速度全局变量
volatile sint16 YPulse = 0;          // 速度全局变量

void Motor_Control(void)
{
    //mini车 YPulse/1024.0*30/58*4.95*3.14(cm)/100(m)/0.1(t:ms)  单位m/s  在乘100扩大100倍，相当于Target=100即为1m/s  0.0911
    speedALL=(speedL1+speedR1)/2.0;

    Speed_Decision(&Element1);  //Motor_openFlag在其中

    //一个速度PID同时控制两个电机
    if(Motor_openFlag==0)
    {
        Motor_IncPID=PidIncCtrl(&Motor_Inc_PID,(float)(0 - speedALL));
    }
    else
    {
        Motor_IncPID=PidIncCtrl(&Motor_Inc_PID,(float)(Target_Speed - speedALL));
    }

    //电机限幅
    if(Motor_IncPID > 6000)Motor_IncPID = 6000;else if(Motor_IncPID < 0)Motor_IncPID = 0;
    if(Motor_Inc_PID.out > 6000)Motor_Inc_PID.out = 6000;else if(Motor_Inc_PID.out < 0)Motor_Inc_PID.out = 0;
}

float Tmp_FuzzyOut; //该变量没用，只是为了在串口中观看变量值

void Servo_Control_Fuzzy(void)
{
    Servo_Loc_error=Calc_ERROR();  //横向偏差

    //控制舵机
    float Fuzzy_out;

    int err=0;
    int err_d=0;
    float e_max=30;
    float e_min=-30;
    float ec_max=4;
    float ec_min=-4;
    float kp_max =Image_kp;
    float kp_min =-Image_kp;
    float ki_max = 0;
    float ki_min = 0;
    float kd_max = Image_kd;
    float kd_min = -Image_kd;

    err=Servo_Loc_error;
    Test_err=err;
    if(err>=e_max)
        err=e_max;
    else if(err<=e_min)
        err=e_min;

    err_d=err-Last_err;
    Test_err_d=err_d;

    if(err_d>=ec_max)
        err=ec_max;
    else if(err_d<=ec_min)
        err=ec_min;

    //模糊pid处理
    Fuzzy_out=Fuzzy_Controller(&Fpid1,e_max,e_min,ec_max,ec_min,kp_max,kp_min,(float)err,(float)err_d,ki_max,ki_min,kd_max,kd_min,(float)Last_err,(float)Last_last_err,0);

    Servo_duty=(int)(Servo_Center_Mid + Fuzzy_out);

    //限制幅值
    if (Servo_duty >= Servo_Left_Max)
        Servo_duty = Servo_Left_Max;
    else if (Servo_duty <= Servo_Right_Min)
        Servo_duty = Servo_Right_Min;

    if(Servo_openFlag==1)
    {
        ATOM_PWM_InitConfig(ATOMSERVO1, Servo_duty, 100);  //舵机频率为100HZ，初始值为1.5ms中值
    }
    else
    {
        ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);  //舵机频率为100HZ，初始值为1.5ms中值
    }

    Last_last_err=Last_err;
    Last_err=err;

    //当一副图像到来时,如果需要差速,在方向环中控制会更加灵活
    //速度控制部分
    //差速
    Tmp_FuzzyOut=Fuzzy_out;  //该变量没用，只是为了在串口中观看变量值
    Dir_out=(int)K_Dir*Fuzzy_out;

#ifdef DOUBLE_ERROR

    //两轮差速
    Motor_duty1=(int)(Motor_IncPID-Dir_out);           //大于1.8m/s:0.91758  小于1.8m/s:0.8634
    Motor_duty2=(int)(Motor_IncPID+Dir_out);           //大于1.8m/s:1.12656  小于1.8m/s:1.20244
    if(Motor_duty1 > 6000)Motor_duty1 = 6000;else if(Motor_duty1 < 0)Motor_duty1 = 0;
    if(Motor_duty2 > 6000)Motor_duty2 = 6000;else if(Motor_duty2 < 0)Motor_duty2 = 0;

#else

    //单轮差速
    if(Dir_out>=0)
    {
        Motor_duty1=(int)(Motor_IncPID);
        Motor_duty2=(int)(Motor_IncPID+Dir_out);
    }
    else
    {
        Motor_duty1=(int)(Motor_IncPID-Dir_out);
        Motor_duty2=(int)(Motor_IncPID);
    }

#endif

    if(Motor_duty1 > 6000)Motor_duty1 = 6000;else if(Motor_duty1 < 0)Motor_duty1 = 0;
    if(Motor_duty2 > 6000)Motor_duty2 = 6000;else if(Motor_duty2 < 0)Motor_duty2 = 0;

    //给电机PWM信号
    if(Motor_openFlag==1)
    {
        MotorCtrl(Motor_duty2,Motor_duty1);  //该函数第一个形参是右轮M2，第二个形参是左轮M1
    }
    else
    {
        Motor_Inc_PID.out=0;
        Motor_Inc_PID.last_derivative=0;
        Motor_Inc_PID.last_error=0;
        Motor_IncPID=0;
        Motor_duty2=0;
        Motor_duty1=0;
        Feedback_SpeedL=0;
        Feedback_SpeedR=0;
        Feedback_Speed_tmpL=0;  //低通滤波积分项清零
        Feedback_Speed_tmpR=0;
        MotorCtrl(0,0);
    }
}

float k_qz=0.1;

void Servo_Control_PD(void)
{
#if 1
    Servo_Loc_error=Calc_ERROR();  //横向偏差
    Servo_QZ_error=qulv_value;     //前瞻偏差

    Servo_All_error=Servo_Loc_error*(1-k_qz)+Servo_QZ_error*k_qz;

    //采取二次项的方式,改进PD控制2
    Direction_Decision(&Element1);

    if(PID_flag_Direction==0)
    {
//        if(Servo_Loc_error>15){Servo_Loc_PID.kp=temp_servo_kp*sp;}
//         else{Servo_Loc_PID.kp=temp_servo_kp;} /*左转达不到最大值，代码上手动加大Kp*/

        Servo_PidLocCtrl(&Servo_Loc_PID,(float)Servo_All_error);
        Servo_duty=(int)(Servo_Center_Mid + Servo_Loc_PID.out);
    }
    else if(PID_flag_Direction==1)
    {
        Servo_PidLocCtrl(&Servo_Loc_PID_tmp,(float)Servo_All_error);
        Servo_duty=(int)(Servo_Center_Mid + Servo_Loc_PID_tmp.out);
    }

#else
    Servo_Loc_error=Calc_ERROR();  //横向偏差

    Direction_Decision(&Element1);

    if(PID_flag_Direction==0)
    {
        if(Servo_Loc_error>20){Servo_Loc_PID.kp=temp_servo_kp*sp;}
         else{Servo_Loc_PID.kp=temp_servo_kp;} /*左转达不到最大值，代码上手动加大Kp*/

        Servo_PidLocCtrl(&Servo_Loc_PID,(float)Servo_Loc_error);
        Servo_duty=(int)(Servo_Center_Mid + Servo_Loc_PID.out);
    }
    else if(PID_flag_Direction==1)
    {
        Servo_PidLocCtrl(&Servo_Loc_PID_tmp,(float)Servo_Loc_error);
        Servo_duty=(int)(Servo_Center_Mid + Servo_Loc_PID_tmp.out);
    }
#endif

    //舵机限幅
    if (Servo_duty >= Servo_Left_Max)
        Servo_duty = Servo_Left_Max;
    else if (Servo_duty <= Servo_Right_Min)
        Servo_duty = Servo_Right_Min;

    if(ok_flag==0)   //启动时计时1s
    {
        ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);
    }
    else
    {
        if(Servo_openFlag==1)
        {
            ATOM_PWM_InitConfig(ATOMSERVO1, Servo_duty, 100);  //舵机频率为100HZ，初始值为1.5ms中值
        }
        else
        {
            ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);  //舵机频率为100HZ，初始值为1.5ms中值
        }

    }

    //当一副图像到来时,如果需要差速,在方向环中控制会更加灵活
    //速度控制部分
    //差速
    if(PID_flag_Direction==0)
    {
        test_out=Servo_Loc_PID.out;
        Dir_out=(int)K_Dir*Servo_Loc_PID.out;
    }

#ifdef DOUBLE_ERROR


    //两轮差速
    if(Dir_out>0)
    {
        Motor_duty1=(int)(Motor_IncPID-Dir_out);           //大于1.8m/s:0.91758  小于1.8m/s:0.8634
        Motor_duty2=(int)(Motor_IncPID+Dir_out);           //大于1.8m/s:1.12656  小于1.8m/s:1.20244
    }
    else
    {
        Motor_duty1=(int)(Motor_IncPID);
        Motor_duty2=(int)(Motor_IncPID);
    }
    if(Motor_duty1 > 6000)Motor_duty1 = 6000;else if(Motor_duty1 < 0)Motor_duty1 = 0;
    if(Motor_duty2 > 6000)Motor_duty2 = 6000;else if(Motor_duty2 < 0)Motor_duty2 = 0;

#else

    //单轮差速
    if(Dir_out>=0)
    {
        Motor_duty1=(int)(Motor_IncPID);
        Motor_duty2=(int)(Motor_IncPID+Dir_out);
    }
    else
    {
        Motor_duty1=(int)(Motor_IncPID);
        Motor_duty2=(int)(Motor_IncPID);
    }

#endif

    if(Motor_duty1 > 6000)Motor_duty1 = 6000;else if(Motor_duty1 < 0)Motor_duty1 = 0;
    if(Motor_duty2 > 6000)Motor_duty2 = 6000;else if(Motor_duty2 < 0)Motor_duty2 = 0;

    //给电机PWM信号
    if(Motor_openFlag==1)
    {
        MotorCtrl(Motor_duty2,Motor_duty1);  //该函数第一个形参是右轮M2，第二个形参是左轮M1
    }
    else
    {
        Motor_Inc_PID.out=0;
        Motor_Inc_PID.last_derivative=0;
        Motor_Inc_PID.last_error=0;
        Motor_IncPID=0;
        Motor_duty2=0;
        Motor_duty1=0;
        Feedback_SpeedL=0;
        Feedback_SpeedR=0;
        Feedback_Speed_tmpL=0;  //低通滤波积分项清零
        Feedback_Speed_tmpR=0;
        MotorCtrl(0,0);
        Set_NULLflag();
    }
}


int change_count=0;//大幅度变化次数

int max_pix=0;
int min_pix=0;


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

void Key5_Handle(void)
{
    if(Key_Value==5)
    {
        Key_Value=0;
        Servo_openFlag=!Servo_openFlag;
        Motor_openFlag=!Motor_openFlag;
        if(Servo_openFlag==1 && Motor_openFlag==1 && open_flag==0)
        {
            open_flag=1;
        }
    }
}



void Modify_PID(void)
{
    //屏幕显示参数
    if(switch_flag==1){switch_flag=0;TFTSPI_CLS(u16BLACK);}  //更新页面

    if(Select_SorM==0){Show_Main();}
    else if(Select_SorM==1)
    {
        if(Run_Mode==0){Show_ServoPid();}
        else if(Run_Mode==1){Show_ServoFuzzy();}
    }
    else if(Select_SorM==2){Show_Speed_up();}

//    else if(Select_SorM==3){Show_Identify();}
    else if(Select_SorM==3){Show_Test();}

#ifdef Test_Motor_Speed
    else if(Select_SorM==3){Show_MotorIncPid();}
#endif

    if(Key_Value==1)
    {
        Key_Value=0;
        data_change_flag=1; //修改了参数，需要写进E2PROM
        if(Select_SorM==0)
        {
            switch(Select_Mode)
            {
                case 0:  //P
                    Refer_Speed+=50;
#ifndef Compete_Mode
                    Set_Para_Value(Target_Speed);
#endif
                    break;
                case 1:  //I
                    Run_Mode++;
                    if(Run_Mode>1){Run_Mode=1;}
                    break;
                case 2:
                    thresholdflag=!thresholdflag;
                    break;
                case 3:

                    break;
            }
            if(Refer_Speed>=1500)
                Refer_Speed=1500;
        }
        else if(Select_SorM==1)  //Servo
        {
            if(Run_Mode==0)      //Servo--PD
            {
                switch(Select_Mode)
                {
                    case 0:  //P
                        temp_servo_kp+=0.1;
                        Servo_Loc_PID.kp=temp_servo_kp;
                        break;
                    case 1:  //I
                        Servo_Loc_PID.kd+=0.1;
                        break;
                    case 2:  //D
                        Look_Line++;
                        break;
                    case 3:
                        K_Dir+=0.5;
                        break;
                    case 4:
                        sp+=0.1;
                        sp_change=1;
                        break;
                    case 5:
                        threshold+=1;
                        break;
                }
            }
            else if(Run_Mode==1)  //Servo-Fuzzy
            {
                switch(Select_Mode)
                {
                    case 0:  //P
                        Image_kp+=1;
                        break;
                    case 1:  //I
                        Image_kd+=1;
                        break;
                    case 2:
                        Look_Line++;
                        break;
                    case 3:
                        K_Dir+=0.5;
                        break;
                    case 4:
                        sp+=0.1;
                        break;
                    case 5:
                        threshold+=1;
                        break;
                }
            }
        }
        else if(Select_SorM==2) //Speed-up
        {
            switch(Select_Mode)
             {
                 case 0:  //P
                     My_Speed_up.Straight_Speed_up+=50;
                     break;
                 case 1:  //I
                     My_Speed_up.Small_Curve_Speed_up+=50;
                     break;
                 case 2:
                     My_Speed_up.Left_Circle_Speed_up=My_Speed_up.Right_Circle_Speed_up+=50;
                     break;
                 case 3:
                     My_Speed_up.Curve_Speed_up+=50;
                     break;
                 case 4:
                     My_Speed_up.zuo_up+=50;
                     break;
             }
        }
//        else if(Select_SorM==3) //identify
//        {
//            switch(Select_Mode)
//             {
//                 case 0:  //P
//                     Identify1.Straightaway_Identify=1;
//                     break;
//                 case 1:  //I
//                     Identify1.Obstruction_Identify=1;
//                     break;
//                 case 2:
//                     Identify1.Small_Curve_Identify=1;
//                     break;
//                 case 3:
//                     Identify1.Curve_Identify=1;
//                     break;
//                 case 4:
//                     Identify1.Cross_Identify=1;
//                     break;
//                 case 5:
//                     Identify1.Circle_left_Identify=1;
//                     break;
//                 case 6:
//                     Identify1.Circle_Right_Identify=1;
//                     break;
//             }
//        }
        else if(Select_SorM==3) //Test
        {
            switch(Select_Mode)
            {
                case 0:
                    Servo_Loc_error++;
                    break;
                case 1:

                    break;
            }
        }
#ifdef Test_Motor_Speed
        else if(Select_SorM==3) //选择Motor
        {
            switch(Select_Mode)
            {
                case 0:  //P
                    Motor_Inc_PID.kp+=0.1;
                    break;
                case 1:  //I
                    Motor_Inc_PID.ki+=0.01;
                    break;
                case 2:  //D
                    Motor_Inc_PID.kd+=0.01;
                    break;
                case 3:

                    break;
            }
        }
#endif
    }
    else if(Key_Value==2)
    {
        Key_Value=0;
        data_change_flag=1;
        if(Select_SorM==0)
        {
            switch(Select_Mode)
            {
                case 0:  //P
                    Refer_Speed-=50;
#ifndef Compete_Mode
                    Set_Para_Value(Target_Speed);
#endif
                    break;
                case 1:  //I
                    Run_Mode--;
                    if(Run_Mode<0){Run_Mode=0;}
                    break;
                case 2:
                    break;
            }
            if(Refer_Speed<=0)
                Refer_Speed=0;
        }
        else if(Select_SorM==1)  //Servo
        {
            if(Run_Mode==0)      //Servo--PD
            {
                switch(Select_Mode)
                {
                    case 0:  //P
                        temp_servo_kp-=0.1;
                        Servo_Loc_PID.kp=temp_servo_kp;
                        break;
                    case 1:  //I
                        Servo_Loc_PID.kd-=0.1;
                        break;
                    case 2:  //D
                        Look_Line--;
                        break;
                    case 3:
                        K_Dir-=0.5;
                        break;
                    case 4:
                        sp-=0.1;
                        sp_change=1;
                        break;
                    case 5:
                        threshold-=1;
                }
            }
            else if(Run_Mode==1)  //Servo-Fuzzy
            {
                switch(Select_Mode)
                {
                    case 0:  //P
                        Image_kp-=1;
                        break;
                    case 1:  //I
                        Image_kd-=1;
                        break;
                    case 2:
                        Look_Line--;
                        break;
                    case 3:
                        K_Dir-=0.5;
                        break;
                    case 4:
                        sp-=0.1;
                        break;
                    case 5:
                        threshold-=1;
                        break;
                }
            }
        }
        else if(Select_SorM==2) //Speed_up
        {
            switch(Select_Mode)
             {
                 case 0:  //P
                     My_Speed_up.Straight_Speed_up-=50;
                     break;
                 case 1:  //I
                     My_Speed_up.Small_Curve_Speed_up-=50;
                     break;
                 case 2:
                     My_Speed_up.Left_Circle_Speed_up=My_Speed_up.Right_Circle_Speed_up-=50;
                     break;
                 case 3:
                     My_Speed_up.Curve_Speed_up-=50;
                     break;
                 case 4:
                     My_Speed_up.zuo_up-=50;
                     break;
             }
        }
//        else if(Select_SorM==3) //identify
//        {
//            switch(Select_Mode)
//             {
//                 case 0:  //P
//                     Identify1.Straightaway_Identify=0;
//                     break;
//                 case 1:  //I
//                     Identify1.Obstruction_Identify=0;
//                     break;
//                 case 2:
//                     Identify1.Small_Curve_Identify=0;
//                     break;
//                 case 3:
//                     Identify1.Curve_Identify=0;
//                     break;
//                 case 4:
//                     Identify1.Cross_Identify=0;
//                     break;
//                 case 5:
//                     Identify1.Circle_left_Identify=0;
//                     break;
//                 case 6:
//                     Identify1.Circle_Right_Identify=0;
//                     break;
//             }
//        }
        else if(Select_SorM==3) //Test
        {
            switch(Select_Mode)
            {
                case 0:
                    Servo_Loc_error--;
                    break;
                case 1:
                    break;
            }
        }

#ifdef Test_Motor_Speed
        else if(Select_SorM==3) //选择Motor1
        {
            switch(Select_Mode)
            {
                case 0:  //P
                    Motor_Inc_PID.kp-=0.1;
                    break;
                case 1:  //I
                    Motor_Inc_PID.ki-=0.01;
                    break;
                case 2:  //D
                    Motor_Inc_PID.kd-=0.01;
                    break;
                case 3:

                    break;
            }
        }
#endif
    }
    else if(Key_Value==3)  //选择调P还是I还是D
    {
        Key_Value=0;
        Select_Mode++;
        Select_Mode%=6;
    }
    else if(Key_Value==4)  //选择调Servo还是Motor
    {
        Key_Value=0;
        Select_Mode=0;
        switch_flag=1; //页面更新标志
        Select_SorM++;
        Select_SorM%=4;
    }

}


/***************低通滤波******************/
int Low_Pass_Filter(short int speedLR,int Feedback_Speed,long int Feedback_Speed_tmp,char s)
{
    int Feedback_Speed_Now;
    int tmp=5;
    int tmp2=tmp/2;
    int tmp7;
    Feedback_Speed_Now=speedLR*225*125/32/58;                     //电机当前转速mini speedLR*375*15/58/16
    Feedback_Speed_tmp+=Feedback_Speed_Now-Feedback_Speed; //速度差求积
    Feedback_Speed=Feedback_Speed_tmp/tmp;                 //取1/10使速度平滑
    tmp7=Feedback_Speed_tmp%tmp;
    if(tmp7>=tmp2)  Feedback_Speed+=1;                         //速度值四舍五入
    if(tmp7<=-tmp2) Feedback_Speed-=1;

    if(s) {Feedback_SpeedL=Feedback_Speed; Feedback_Speed_tmpL=Feedback_Speed_tmp;}
    else  {Feedback_SpeedR=Feedback_Speed; Feedback_Speed_tmpR=Feedback_Speed_tmp;}

    return Feedback_Speed;                                 //反馈滤波后的速度
}


void Init_System(void)
{
    TFTSPI_Init(0);               // TFT1.8初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLACK);         // 清屏

    MotorInit();

//    TestServo();

    //舵机目前的频率为50Hz，不过看资料最高可以到300Hz，提高控制频率，后续可以试试看
    ServoInit();

    //无刷初始化
    //ATOM_PWM_InitConfig(ATOMSERVO2, 0, 50);

    GPIO_LED_Init();

    GPIO_Beep_Init();

    GPIO_KEY_Init();

    //陀螺仪初始化,MPU6050上电后需要延时一段时间才能初始化
    IIC_Init();

    delayms(100);

    Gyro_ID = Gyro_Chose();

    MPU6050_Init();

    //摄像头初始化
    CAMERA_Init(50);

    //龙邱母板AD7连接着电池
    ADC_InitConfig(ADC7, 80000);

    ENC_InitConfig(ENC2_InPut_P33_7, ENC2_Dir_P33_6); //左轮编码器
    ENC_InitConfig(ENC4_InPut_P02_8, ENC4_Dir_P33_5); //右轮编码器

    UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);

    CCU6_InitConfig(CCU60, CCU6_Channel0, 2*1000);  //每2ms进入一次中断，处理电机舵机的事件

    //初始化PID的值
    Set_PID();

    //初始化元素标志
    Set_NULLflag();

    //读取E2PROM中的值
//    E2PROM_Read_PID();

    Speed_up_Init();

    //上电之后读取一次电压值
    VBAT=ADC_Read(ADC7);
    VBAT = VBAT * 0.46;  // x/4095*3.3*100*5.7
}

