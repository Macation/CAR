/*
 * My_menu.c
 *
 *  Created on: 2024年4月29日
 *      Author: dduu
 */
#include <My_menu.h>

//创建一个菜单
sMenu menu = {0,0,0,0};
extern float kp;
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
                case 1: TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)show); break;
                /* 三级菜单2，进入可运行App2 */
                case 2:  TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *)Image_Line);break;
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
                //三级菜单2APP
                case 2:
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
            }
            break;
    }
}

void main_menu(void)
{
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(0,0,(char*)"->My_Image",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,1,(char*)"  PID_Value",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,2,(char*)"  ENC_Value",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(0,0,(char*)"  My_Image",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,1,(char*)"->PID_Value",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,2,(char*)"  ENC_Value",u16WHITE,u16BLACK);
            break;
        case 3:
            TFTSPI_P8X16Str(0,0,(char*)"  My_Image",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,1,(char*)"  PID_Value",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(0,2,(char*)"->ENC_Value",u16WHITE,u16BLACK);
            break;
    }
}

void menu_ENC(void)
{
    switch(chooseBuf){
        case 1:
            TFTSPI_P8X16Str(2,0,(char*)"->ALLENC",u16WHITE,u16BLACK);
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
            TFTSPI_P8X16Str(2,1,(char*)"  Motor_Inc_PID",u16WHITE,u16BLACK);
            break;
        case 2:
            TFTSPI_P8X16Str(2,0,(char*)"  Servo_Loc_PID",u16WHITE,u16BLACK);
            TFTSPI_P8X16Str(2,1,(char*)"->Motor_Inc_PID",u16WHITE,u16BLACK);
            break;
    }
}

void Show_ENC(void)
{
    char txt[30];
    TFTSPI_P8X16Str(0, 0, (char *)"ENC:", u16WHITE, u16BLACK);
    sprintf(txt, "LPulse: %4d", LPulse);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "YPulse: %4d", YPulse);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Cur_Speed1: %d", Current_Speed1);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Cur_Speed2: %d", Current_Speed2);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
    sprintf(txt, "TarSpeed: %.1f",Target_Speed);
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Motor_openFlag: %d", Motor_openFlag);
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
}

void Show_ServoPid(void)
{
    char txt[30];
    sprintf(txt, "Select_PID=%1d",Select_PID);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "SP=%.2f",Servo_Loc_PID.kp);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "kp=%.2f",kp);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "SD=%.2f",Servo_Loc_PID.kd);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "KP_error=%.2f",Servo_Loc_PID.out_p);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "KD_error=%.2f ",Servo_Loc_PID.out_d);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Servo_error=%3d",(int)Servo_Loc_error);
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);
    sprintf(txt, "Servo_duty=%4d ",Servo_duty);              //1370~1700
    TFTSPI_P8X16Str(0, 7, txt, u16WHITE, u16BLACK);
}

void Show_MotorIncPid(void)
{
    char txt[30];
    sprintf(txt, "Select_PID=%1d",Select_PID);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "MP=%.2f",Motor_Inc_PID.kp);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "MI=%.2f",Motor_Inc_PID.ki);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
    sprintf(txt, "MD=%.2f",Motor_Inc_PID.kd);
    TFTSPI_P8X16Str(0, 3, txt, u16WHITE, u16BLACK);
    sprintf(txt, "M1_duty=%4d",Motor_duty1);
    TFTSPI_P8X16Str(0, 4, txt, u16WHITE, u16BLACK);
    sprintf(txt, "M2_duty=%4d",Motor_duty2);
    TFTSPI_P8X16Str(0, 5, txt, u16WHITE, u16BLACK);
    sprintf(txt, "M_IncPID: %6.2f", Motor_IncPID);
    TFTSPI_P8X16Str(0, 6, txt, u16WHITE, u16BLACK);
}

void Show_Motor(void)
{
    char txt[30];
    sprintf(txt, "speed=%.1f",Current_Speed);
    TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLACK);
    sprintf(txt, "speed1=%d",Current_Speed1);
    TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLACK);
    sprintf(txt, "speed2=%d",Current_Speed2);
    TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLACK);
}

