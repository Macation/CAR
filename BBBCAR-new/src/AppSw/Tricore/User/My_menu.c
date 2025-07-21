/*
 * My_menu.c
 *
 *  Created on: 2024年4月29日
 *      Author: dduu
 */
#include <My_menu.h>

//创建一个菜单
sMenu menu = {0,0,0,0};

uint8 change_threshold;

Element_speed_up My_Speed_up;

void Speed_up_Init(void)
{
    My_Speed_up.Curve_Speed_up=0;
    My_Speed_up.Left_Circle_Speed_up=0;
    My_Speed_up.Right_Circle_Speed_up=0;
    My_Speed_up.Small_Curve_Speed_up=0;
    My_Speed_up.Straight_Speed_up=0;
    My_Speed_up.zuo_up=0;
}

//放在主循环中扫描
void Menu_Scan(void)
{
    if(switch_flag==1){
        switch_flag=0;
        TFTSPI_CLS(u16BLACK);         // 清屏
    }
    switch(menu.mode1)
    {
        /* 主菜单，选择进入对应二级菜单 */
        //进入主菜单时显示“主菜单”，并播报一遍
        case 0: main_menu(); break;
        /* 二级菜单1    各个图像数据*/
        case 1:
            switch(menu.mode2)
            {
                /* 二级菜单，选择进入对应三级菜单  */
                    //进入二级菜单时显示“二级菜单1”，并播报一遍
                case 0:menu_image();break;
                /* 三级菜单1，进入可运行App1 */
                case 1: TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
                        TFTSPI_Draw_Line(0, (uint8)Look_Line, 159, (uint8)Look_Line, u16YELLOW);
                        break;
                /* 三级菜单2，进入可运行App2 */
                case 2:  TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *)Image_Line);
                         TFTSPI_Draw_Line(0, (uint8)Look_Line, 159, (uint8)Look_Line, u16YELLOW);
                         char txt[30];
                         sprintf(txt, "%3d",threshold);              //1350~1550
                         TFTSPI_P8X16Str(16, 0, txt, u16WHITE, u16BLACK);
                         break;
            }
            break;
            /* 二级菜单2    PID主菜单*/
        case 2:
            switch(menu.mode2)
            {
                case 0:
                    //PID主菜单
                    menu_pid();
                    break;
                //三级菜单1APP
                case 1:
                    Show_ServoPid();
                    break;
                case 2:
                    Show_ServoFuzzy();
                    break;
                //三级菜单2APP
                case 3:
                    Show_MotorIncPid();
                    break;

            }
            break;
        case 3:
            switch(menu.mode2)
            {
                case 0:
                    //ENC编码器主菜单
                    menu_ENC();
                    break;
                //三级菜单1APP
                case 1:
                    Show_ENC();
                    break;
                case 2:
                    Show_Pulse();
                    break;
            }
            break;
    }
}

void main_menu(void)
{
    char txt[20];
    sprintf(txt, "%d.%02dV ", VBAT / 100, VBAT % 100);
    TFTSPI_P8X16Str(0, 0, txt, u16PURPLE, u16BLACK);
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(0,1,(char*)"->My_Image",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,2,(char*)"  PID_Value",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,3,(char*)"  ENC_Value",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(0,1,(char*)"  My_Image",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,2,(char*)"->PID_Value",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,3,(char*)"  ENC_Value",u16WHITE,u16BLACK);
            break;
        case 3:
            TFTSPI_P8X16Str(0,1,(char*)"  My_Image",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,2,(char*)"  PID_Value",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,3,(char*)"->ENC_Value",u16WHITE,u16BLACK);
            break;
    }
}

void menu_ENC(void)
{
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(2,0,(char*)"->ALLENC",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"  PULSE",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(2,0,(char*)"  ALLENC",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"->PULSE",u16WHITE,u16BLACK);
            break;
    }
}

void menu_ServoMotor(void)
{
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(2,0,(char*)"->open",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"  close",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(2,0,(char*)"  open",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"->close",u16WHITE,u16BLACK);
            break;
    }
}

void menu_image(void)
{
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(2,0,(char*)"->No_Handler",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"  Do_Handler",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(2,0,(char*)"  No_Handler",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"->Do_Handler",u16WHITE,u16BLACK);
            break;
    }
}

void menu_pid(void)
{
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(2,0,(char*)"->Servo_Loc_PID",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"  Servo_Loc_Fuzzy",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,2,(char*)"  Motor_Inc_PID",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(2,0,(char*)"  Servo_Loc_PID",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"->Servo_Loc_Fuzzy",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,2,(char*)"  Motor_Inc_PID",u16WHITE,u16BLACK);
            break;
        case 3:
            TFTSPI_P8X16Str(2,0,(char*)"  Servo_Loc_PID",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"  Servo_Loc_Fuzzy",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,2,(char*)"->Motor_Inc_PID",u16WHITE,u16BLACK);
            break;
    }
}

void Show_Pulse(void)
{
    char txt[30];
    TFTSPI_P8X16Str(0, 0, (char *)"Pulse:", u16WHITE, u16BLACK);
    sprintf(txt, "Circle_LKey: %1d", ALLPULSE.Circle_Left_Pulse_Key);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Circle_L: %4d", ALLPULSE.Circle_Left_Pulse);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
}

void Show_ENC(void)
{
    char txt[30];
    TFTSPI_P8X16Str(0, 0, (char *)"ENC:", u16WHITE, u16BLACK);
    sprintf(txt, "speedL: %4d", speedL);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "speedR: %4d", speedR);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "speedL1: %4d", speedL1);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "speedR1: %4d", speedR1);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
    sprintf(txt, "TarSpeed: %3d",Target_Speed);
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Motor_openFlag: %d", Motor_openFlag);
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
}

void Show_ServoFuzzy(void)
{
    char txt[30];
    sprintf(txt, "E%03d",Servo_Loc_error);
    TFTSPI_P8X16Str(14, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "S%1d",Select_Mode);
    TFTSPI_P8X16Str(17, 0, txt, u16WHITE, u16BLACK);

    sprintf(txt, "0:kp=%3.1f",Image_kp);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:kd=%3.1f",Image_kd);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:Look=%3d",Look_Line);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "3:Dir=%3.1f",K_Dir);              //1350~1550
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "4:sp=%3.1f",sp);              //1350~1550
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "5:threshold=%d",threshold);              //1350~1550
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);

    sprintf(txt, "FzkP=%3.1f Fzkd=%3.1f",Fpid1.kp,Fpid1.kd);
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);

    sprintf(txt, "Servo_duty=%4d ",Servo_duty);              //1350~1550
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
}

void Show_Speed_up(void)
{
    char txt[30];

    sprintf(txt, "S%1d",Select_Mode);
    TFTSPI_P8X16Str(17, 0, txt, u16WHITE, u16BLACK);

    sprintf(txt, "0:Staright=%04d",My_Speed_up.Straight_Speed_up);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:Small_Curve=%04d",My_Speed_up.Small_Curve_Speed_up);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:Circle=%04d",My_Speed_up.Left_Circle_Speed_up);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "3:Curve=%04d",My_Speed_up.Curve_Speed_up);              //1350~1550
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "4:zuo=%04d",My_Speed_up.zuo_up);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "4:zuo=%04d",gyroz);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
}

void Show_Test(void)
{
    char txt[30];
    sprintf(txt, "L%03d  Q%03d  A%03d",Servo_Loc_error,Servo_QZ_error,Servo_All_error);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "S%1d",Select_Mode);
    TFTSPI_P8X16Str(17, 5, txt, u16WHITE, u16BLACK);

    sprintf(txt, "c1=%.2f c2=%.2f",currentslope,currentslope2);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "cro=%hu lef=%hu rig=%hu",Element1.Crossroad_flag,Element1.Circle_left,Element1.Circle_right);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
//    sprintf(txt, "newYAW=%4.1f",current_yaw);
    sprintf(txt, "w1=%3d w2=%3d w3=%3d",width[0],width[1],width[2]);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);

//    sprintf(txt, "Dis=%5d ",Distance);              //1350~1550
//    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);
    sprintf(txt, "qulv: %2d %2d %2d",qulv_value,qvlv_quanju_left,qvlv_quanju_right);              //1350~1550
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);

    sprintf(txt, "zuo=%d ", Element1.zuo_flag);              //1350~1550
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Angle_yaw=%.2f ", Angle_yaw);              //1350~1550
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);



}

void Show_ServoPid(void)
{
    char txt[30];
    sprintf(txt, "E%03d",Servo_Loc_error);
    TFTSPI_P8X16Str(14, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "S%1d",Select_Mode);
    TFTSPI_P8X16Str(17, 1, txt, u16WHITE, u16BLACK);

    sprintf(txt, "0:SP1=%4.2f",Servo_Loc_PID.kp);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:SD1=%4.2f",Servo_Loc_PID.kd);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:Look=%3d",Look_Line);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "3:Dir=%3.1f",K_Dir);              //1350~1550
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "4:sp=%3.1f",sp);              //1350~1550
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "5:threshold=%d",threshold);              //1350~1550
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);

    sprintf(txt, "perr=%4.1f derr=%4.1f ",Servo_Loc_PID.out_p,Servo_Loc_PID.out_d);
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Servo_duty=%4d ",Servo_duty);              //1350~1550
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);

}

void Show_MotorIncPid(void)
{
    char txt[30];
    sprintf(txt, "%1d",Select_Mode);
    TFTSPI_P8X16Str(18, 0, txt, u16WHITE, u16BLACK);

    sprintf(txt, "MP=%.2f",Motor_Inc_PID.kp);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "MI=%.2f",Motor_Inc_PID.ki);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "MD=%.2f",Motor_Inc_PID.kd);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);

    sprintf(txt, "M1_duty=%4d",Motor_duty1);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "M2_duty=%4d",Motor_duty2);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "M_IncPID: %6.2f", Motor_IncPID);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);

}


void Show_Main(void)
{
    char txt[30];
    sprintf(txt, "S%1d",Select_Mode);
    TFTSPI_P8X16Str(17, 0, txt, u16WHITE, u16BLACK);

    sprintf(txt, "0:Speed=%4d",Target_Speed);              //1350~1550
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:Mode=%1d",Run_Mode);              //1350~1550
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:OSTU=%3d",OSTU);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "cor=%d lft=%d rig=%d",Element1.Crossroad_flag,Element1.Circle_left,Element1.Circle_right);              //1350~1550
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "ob=%d st=%d s=%d S=%d",Element1.Obstruction,Element1.Straightaway_flag,Element1.Small_Curve_flag,Element1.Curve_flag);              //1350~1550
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);

}

void Show_Identify(void)
{

    char txt[30];
    sprintf(txt, "S%1d",Select_Mode);
    TFTSPI_P8X16Str(17, 1, txt, u16WHITE, u16BLACK);

    sprintf(txt, "0:Straight=%d",Identify1.Straightaway_Identify);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "1:obstruct=%d",Identify1.Obstruction_Identify);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "2:Small_Curve=%d",Identify1.Small_Curve_Identify);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "3:Curve=%d",Identify1.Curve_Identify);                //1350~1550
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "4:CrossRoad=%d",Identify1.Cross_Identify);                           //1350~1550
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "5:Circle_left=%d",Identify1.Circle_left_Identify);                      //1350~1550
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
    sprintf(txt, "6:Circle_right=%d",Identify1.Circle_Right_Identify);                      //1350~1550
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);

}

