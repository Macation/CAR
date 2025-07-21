/*
 * Element_handle.c
 *
 *  Created on: 2024年6月3日
 *      Author: dduu
 */
#include <Element_handle.h>

Image_element Element1;
Image_Identify Identify1;
Pulse_Param_t ALLPULSE;
float Angle_in;//进圆环时的角度
float Angle_out;//角度变化值
int jishu=0;
int width[10];
float tmp_kp;
unsigned char Speed_flag;
int zcout=0;
unsigned int zhangaiflag=0;
int tmperr=0;
/*************************************************************************
*  函数名称：void Image_ElementInit(Image_element * Ele)
*  功能说明:   元素初始化
*  函数返回：无
*  修改时间：2023年4月5日
*  备    注：
*************************************************************************/
void Image_ElementInit(Image_element *Ele)
{
   Ele->Crossroad_flag=0;
   Ele->Circle_left=0;
   Ele->Obstruction=0;
   Ele->Circle_right=0;
   Ele->Zebra=0;
   Ele->Straightaway_flag=0;
   Ele->Small_Curve_flag=0;
   Ele->Curve_flag=0;
   Ele->zuo_flag=0;
}
/*************************************************************************
*  函数名称：void Image_IdentifyProcess(Image_Identify * Ide)
*  功能说明:   元素识别
*  函数返回：无
*  修改时间：2023年6月10日
*  备    注：
*************************************************************************/
void Image_IdentifyProcess(Image_Identify * Ide)
{
    if(ALLPULSE.Circle_Left_Pulse>5000){
        Element1.Circle_left=0;
        My_PulseKey(&ALLPULSE,1,0);
    }
    if(ALLPULSE.Circle_Right_Pulse>5000){
        Element1.Circle_right=0;
        My_PulseKey(&ALLPULSE,2,0);
    }
    if(ALLPULSE.Cross_Pulse>3000){
        Element1.Crossroad_flag=0;
        My_PulseKey(&ALLPULSE,0,0);
    }

//    Image_ZebraProcess(&Element1);
    //未加电磁，注释掉所有元素
    if(Ide->All_Identify==1)//全元素识别
    {
        Image_CrossProcess(&Element1);//十字处理
        Image_LeftCircleProcess(&Element1);//左圆环处理
        Image_RightCircleProcess(&Element1);//右圆环处理
        My_ObstructionProcess(&Element1);//路障处理
//        Image_ZebraProcess(&Element1);//斑马线处理
        Straightaway_Process(&Element1);
        Small_Curve_Process(&Element1);
//        Curve_Process(&Element1);
//        zuo_Process(&Element1);
    }
    else if(Ide->Zebra_Identify==1)
    {
        Image_ZebraProcess(&Element1);//斑马线处理
    }
    else if(Ide->Cross_Identify==1)
    {
        Image_CrossProcess(&Element1);//十字处理
    }
    else if(Ide->Circle_left_Identify==1)
    {
        Image_LeftCircleProcess(&Element1);//左圆环处理
    }
    else if(Ide->Circle_Right_Identify==1)
    {
        Image_RightCircleProcess(&Element1);//右圆环处理
    }
    else if(Ide->Obstruction_Identify==1)
    {
        My_ObstructionProcess(&Element1);//路障处理
    }
//    else if(Ide->Curve_Identify==1)
//    {
//        Curve_Process(&Element1); //大S弯处理
//    }
    else if(Ide->Straightaway_Identify==1)
    {
        Straightaway_Process(&Element1);  //直道处理
    }
    else if(Ide->Small_Curve_Identify==1)
    {
        Small_Curve_Process(&Element1); //小S弯处理
    }
//    else if(Ide->zuo_Identify==1)
//    {
//        zuo_Process(&Element1);
//    }
}
/*************************************************************************
*  函数名称：void My_ElementClean(Image_element* Ele,int i)
*  参数说明：当前识别成功的元素：0十字；1.左圆环；2.右圆环 3. 斑马线
*      功能说明：在成功识别一个元素之后，把其他元素标志位清零，防止误识别
*  函数返回：无
*  修改时间：2023年6月9日
*  备    注：
*************************************************************************/
void My_ElementClean(Image_element* Ele,int i)
{
    if(i==0)
    {
        Ele->Circle_left=0;
        Ele->Circle_right=0;
        Ele->Obstruction=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Small_Curve_flag=0;
        Ele->Curve_flag=0;
        Ele->zuo_flag=0;
    }
    else if(i==1)
    {
        Ele->Circle_right=0;
        Ele->Crossroad_flag=0;
        Ele->Obstruction=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Small_Curve_flag=0;
        Ele->Curve_flag=0;
        Ele->zuo_flag=0;
    }
    else if(i==2)
    {
        Ele->Crossroad_flag=0;
        Ele->Circle_left=0;
        Ele->Obstruction=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Small_Curve_flag=0;
        Ele->Curve_flag=0;
        Ele->zuo_flag=0;
    }
    else if(i==3)
    {
        Ele->Crossroad_flag=0;
        Ele->Circle_left=0;
        Ele->Circle_right=0;
        Ele->Obstruction=0;
        Ele->Zebra=0;
        Ele->Small_Curve_flag=0;
        Ele->Curve_flag=0;
        Ele->zuo_flag=0;
    }
    else if(i==4)
    {
        Ele->Crossroad_flag=0;
        Ele->Circle_left=0;
        Ele->Circle_right=0;
        Ele->Obstruction=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Curve_flag=0;
        Ele->zuo_flag=0;
    }
    else if(i==5)
    {
        Ele->Crossroad_flag=0;
        Ele->Circle_left=0;
        Ele->Circle_right=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Small_Curve_flag=0;
        Ele->Curve_flag=0;
        Ele->zuo_flag=0;
    }
    else if(i==6)
    {
        Ele->Crossroad_flag=0;
        Ele->Circle_left=0;
        Ele->Circle_right=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Small_Curve_flag=0;
        Ele->Obstruction=0;
        Ele->zuo_flag=0;
    }
    else if(i==7)
    {
        Ele->Crossroad_flag=0;
        Ele->Circle_left=0;
        Ele->Circle_right=0;
        Ele->Zebra=0;
        Ele->Straightaway_flag=0;
        Ele->Small_Curve_flag=0;
        Ele->Obstruction=0;
        Ele->Curve_flag=0;
    }
}
/*************************************************************************
*  函数名称：void Image_IdentifyInit(Image_Identify * Ide)
*  功能说明:   元素识别初始化
*  函数返回：无
*  修改时间：2023年6月10日
*  备    注：
*************************************************************************/
void Image_IdentifyInit(Image_Identify * Ide)
{
    //1代表开启元素识别
    Ide->Circle_left_Identify=1;
    Ide->Cross_Identify=1;
    Ide->Obstruction_Identify=1;
    Ide->Circle_Right_Identify=1;
    Ide->Zebra_Identify=1;
    Ide->All_Identify=1;//全元素识别
    Ide->Straightaway_Identify=1;
    Ide->Small_Curve_Identify=1;
    Ide->zuo_Identify=1;
}
/*************************************************************************
*  函数名称：void Image_CrossProcess(Image_element* Ele)
*  参数说明：
*  功能说明：十字处理
*  函数返回：无
*  修改时间：2023年6月3日
*  备    注：
*************************************************************************/
void Image_CrossProcess(Image_element* Ele)
{
    int x4 = guaidian1[3][0];
    int y4 = guaidian1[3][1];
    int X22 = guaidian2[3][0];
    int Y22 = guaidian2[3][1];
    if(abs(Y22-y4)>20)
    {
        x4=0;
        X22=0;
    }
    switch(Ele->Crossroad_flag)
    {
        case 0:
        {
            if(current_state1==3&&current_state2==3)
            {
                Ele->Crossroad_flag=7;
                My_PulseClean(&ALLPULSE,0);
                My_PulseKey(&ALLPULSE,0,1);

                //控制蜂鸣器
            }
        }
        break;
        case 7:
        {
            if(x4!=0&&X22!=0)
            {
                Ele->Crossroad_flag=1;
                My_PulseClean(&ALLPULSE,0);
                //控制蜂鸣器
//                Control_Beep(&Element1);
            }
        }
        break;
        case 1:
            if(x4!=0)draw_line(x4,y4,4,115);
            if(X22!=0)draw_line(X22,Y22,155,115);
            if(ALLPULSE.Cross_Pulse>800)
//            if(current_state1==0&&current_state2==0&&x4==0&&X22==0)
//            if(x4==0&&X22==0)
                {
                    Ele->Crossroad_flag=0;
                    My_PulseKey(&ALLPULSE,0,0);
                }
            break;
        case 2:
            if(current_state1==3&&current_state2==3&&x4!=0&&X22!=0)
            {
                Ele->Crossroad_flag=3;
            }
            break;
        case 3:
            if(x4!=0)draw_line(x4,y4,4,115);
            if(X22!=0)draw_line(X22,Y22,155,115);
            if(current_state1==0&&current_state2==0&&x4==0&&X22==0)
//            if(x4==0&&X22==0)
                {
                    Ele->Crossroad_flag=4;
                    My_PulseClean(&ALLPULSE,0);
                    My_PulseKey(&ALLPULSE,0,1);
                }
            break;
        case 4:
            if(ALLPULSE.Cross_Pulse>600)
            {
                Ele->Crossroad_flag=0;
                My_PulseKey(&ALLPULSE,0,0);
            }
            break;
    }
}
/*************************************************************************
*  函数名称：void Image_LeftCircleProcess(Image_element* Ele)
*  参数说明: 圆环处理函数
*  功能说明：
*  函数返回：无
*  修改时间：2023年5月27日
*  备    注：
*************************************************************************/
void Image_LeftCircleProcess(Image_element* Ele)
{
    int x1 = guaidian1[0][0];//下拐点
    int y1 = guaidian1[0][1];
    int x2 = guaidian1[1][0];//中拐点
    int y2 = guaidian1[1][1];
    int x3 = guaidian1[2][0];//上拐点
    int y3 = guaidian1[2][1];
    int X11 = guaidian2[0][0];//出弯拐点
    int Y11 = guaidian2[0][1];
    switch(Ele->Circle_left)
    {
        case 0:
            if(current_state2==0&&current_state1==3&&x1!=0)
//            if(current_state2==0&&current_state1==3&&x2!=0&&y2<60&&y2>20&&zuodianxian>30&&youdianxian<20&&qulv_value<50)
            {
                Ele->Circle_left=1;
                My_PulseKey(&ALLPULSE,1,1);
                My_PulseClean(&ALLPULSE,1);
            }
            break;
        case 1:
        {
            if(current_state2==0&&x2!=0&&ALLPULSE.Circle_Left_Pulse>200)
            {
                Ele->Circle_left=2;
                keep_beep=2;
                My_PulseClean(&ALLPULSE,1);

                //控制蜂鸣器
//                Control_Beep(&Element1);
            }
        }
            break;
           case 2:
           //确定是圆环，右边直道，找到中拐点，开始补线
               if(x2!=0)draw_line(4, 115, x2, y2);
               if(current_state1!=3&&current_state2==0&&ALLPULSE.Circle_Left_Pulse>200&&x2!=0)
               {
                   Ele->Circle_left=3;
                   My_PulseClean(&ALLPULSE,1);
//                   keep_beep=3;

               }

           break;
           case 3:
           //确定是圆环，右边直道，找到中拐点，开始补线
               if(x2!=0)draw_line(4, 115, x2, y2);
               if(current_state2==0&&ALLPULSE.Circle_Left_Pulse>450&&x3!=0&&x2==0)
               {
                   Ele->Circle_left=4;
                   My_PulseClean(&ALLPULSE,1);
                   Angle_Flag=1;
                   Angle_yaw=0;
               }
           break;
           case 4:
           //确定是圆环，右边直道，找到中拐点，开始补线
               if(x3!=0)draw_line(x3, y3, 155, 90);
               if(x3==0)draw_line(Image_Lefthand_Line[Left_num-1][0],Image_Lefthand_Line[Left_num-1][1],155,90);
               if(current_state2==1&&Angle_yaw>50&&x3==0)
               {
                   Ele->Circle_left=5;
                   My_PulseClean(&ALLPULSE,1);
                   My_PulseKey(&ALLPULSE,1,0);

               }
               break;
                   case 5:
                   {
                       if(Angle_yaw>250&&X11!=0)
                           {
                               Ele->Circle_left=6;
                               My_PulseKey(&ALLPULSE,1,1);
                           }
                   }
                   break;
                   case 6:
                   {
                       if(X11!=0)
                       {
                           draw_line(X11, Y11, 40, 30);
                       }
                       if(X11==0)draw_line(155,110,40,30);
                       if(Angle_yaw>330&&ALLPULSE.Circle_Left_Pulse>200)
                           {
                               Ele->Circle_left=7;
                               My_PulseClean(&ALLPULSE,1);
                           }
                   }
                   break;
                   case 7:
                   {
                       int x22;
                       for(int s=13;s<159;s++)
                       {
                           if(Image_Line[s][60]==1)
                           {
                               x22=s;
                               break;
                           }
                       }
                       draw_line(155,110,60,x22);
                       if(x3!=0&&ALLPULSE.Circle_Left_Pulse>200&&Angle_yaw>320)
                           {
                               Ele->Circle_left=8;
                               My_PulseClean(&ALLPULSE,1);
                           }
                   }
                   break;
                   case 8:
                   {
                       if(x3!=0)draw_line(x3,y3,4,115);
                       if(current_state1==0&&current_state2==0)
                           {
                               if(ALLPULSE.Circle_Left_Pulse>400)
                               {
                                   Ele->Circle_left=0;
                                   My_PulseKey(&ALLPULSE,1,0);
                                   Angle_Flag=0;
//                                   keep_beep=4;
                               }
                           }
                   }
                   break;
    }
}

/*************************************************************************
*  函数名称：void Image_RightCircleProcess(Image_element* Ele)
*  参数说明: 圆环处理函数
*  功能说明：
*  函数返回：无
*  修改时间：2023年6月10日
*  备    注：  此处入圆环第一步判断与十字误判率高，需要下一步区分
*************************************************************************/
void Image_RightCircleProcess(Image_element* Ele)
{
    int x1 = guaidian2[0][0];
    int y1 = guaidian2[0][1];//上拐点
    int x2 = guaidian2[1][0];
    int y2 = guaidian2[1][1];//中拐点
    int x3 = guaidian2[2][0];
    int y3 = guaidian2[2][1];//下拐点
    int X11 = guaidian1[1][0];
    int Y11 = guaidian1[1][1];
    if(ALLPULSE.Circle_Right_Pulse>10000)Ele->Circle_right=0;
    switch(Ele->Circle_right)
    {
        case 0:
            if(current_state1==0&&current_state2==3&&x1!=0)
//            if(current_state1==0&&current_state2==3&&x2!=0&&y2<60&&y2>20&&youdianxian>30&&zuodianxian<20&&qulv_value<50)
            {
                Ele->Circle_right=1;
                My_PulseKey(&ALLPULSE,2,1);
            }
            break;
        case 1:
            if(current_state1==0&&x2!=0&&ALLPULSE.Circle_Right_Pulse>200)
            {
                Ele->Circle_right=2;
                My_PulseClean(&ALLPULSE,2);

                //控制蜂鸣器
//                 Control_Beep(&Element1);
            }
            break;
           case 2:
           //确定是圆环，右边直道，找到中拐点，开始补线
               if(x2!=0)draw_line(155, 115, x2, y2);
               if(current_state2!=3&&current_state1==0&&ALLPULSE.Circle_Right_Pulse>200&&x2!=0)
               {
                   Ele->Circle_right=3;
                   keep_beep=2;
                   My_PulseClean(&ALLPULSE,2);
//                   keep_beep=3;
               }

           break;
           case 3:
           //确定是圆环，右边直道，找到中拐点，开始补线
               if(x2!=0)draw_line(155, 115, x2, y2);
               if(current_state1==0&&ALLPULSE.Circle_Right_Pulse>450&&x3!=0&&x2==0)
               {
                   Ele->Circle_right=4;
                   My_PulseClean(&ALLPULSE,2);
                   Angle_Flag=1;
                   Angle_yaw=0;
               }
           break;
           case 4:
           //确定是圆环，右边直道，找到中拐点，开始补线
               if(x3!=0)draw_line(x3, y3, 5, 90);
               if(x3==0)draw_line(Image_Righthand_Line[Right_num-1][0],Image_Righthand_Line[Right_num-1][1],5,90);
               if(current_state1==1&&Angle_yaw>50&&x3==0)
                   {
                       Ele->Circle_right=5;
                       My_PulseClean(&ALLPULSE,2);
                       My_PulseKey(&ALLPULSE,2,0);
                   }
           break;
                   case 5:
                   {
                       if(Angle_yaw>250&&X11!=0)
                       {
                           Ele->Circle_right=6;
                           My_PulseKey(&ALLPULSE,2,1);
                           My_PulseClean(&ALLPULSE,2);
                       }
                   }
                   break;
                   case 6:
                   {
                       if(X11!=0)
                       {
                           draw_line(X11, Y11, 120, 30);
                       }
                       if(X11==0)draw_line(4,110,120,30);
                       if(Angle_yaw>330&&ALLPULSE.Circle_Right_Pulse>200)
                           {
                               Ele->Circle_right=7;
                               My_PulseClean(&ALLPULSE,2);
                           }
                   }
                   break;
                   case 7:
                   {
                       int x22;
                       for(int s=13;s<159;s++)
                       {
                           if(Image_Line[s][100]==1)
                           {
                               x22=s;
                               break;
                           }
                       }
                       if(x22!=0)draw_line(4,110,100,x22);
                       if(x3!=0&&ALLPULSE.Circle_Right_Pulse>200&&Angle_yaw>320)
                           {
                               Ele->Circle_right=8;
                               My_PulseClean(&ALLPULSE,2);
                           }
                   }
                   break;
                   case 8:
                   {
                       if(x3!=0)draw_line(x3,y3,155,115);
                       if((current_state2==0||current_state2==1)&&(current_state1==0||current_state1==1))
                           {
                               if(ALLPULSE.Circle_Right_Pulse>400)
                               {
                                   Ele->Circle_right=0;
                                   My_PulseKey(&ALLPULSE,2,0);
                                   keep_beep=4;
                               }

                           }
                   }
                   break;
}
}
/*************************************************************************
*  函数名称：void My_ObstructionProcess(Image_element* Ele)
*  参数说明：
*  功能说明：路障处理
*  函数返回：无
*  修改时间：2023年6月3日
*  备    注：
*************************************************************************/
void My_ObstructionProcess(Image_element* Ele)
{
        int x1=guaidian1[4][0];//障碍物直角拐点
        int y1=guaidian1[4][1];
        int x2=guaidian1[5][0];
        int y2=guaidian1[5][1];//障碍物钝角拐点
        int x3=guaidian1[6][0];//障碍物锐角拐点
        int y3=guaidian1[6][1];
        int X1=guaidian2[4][0];//障碍物直角拐点
        int Y1=guaidian2[4][1];
        int X2=guaidian2[5][0];
        int Y2=guaidian2[5][1];//障碍物钝角拐点
        int X3=guaidian2[6][0];//障碍物锐角拐点
        int Y3=guaidian2[6][1];
        int leftstart,rightstart;
        int left[10],right[10];
        switch(Ele->Obstruction)
        {
            case 0:
                if(x1!=0&&x2!=0&&x3!=0&&y1<y2&&y1>y3)
                {
                    for(leftstart=0;leftstart<Left_num;leftstart++)
                    {
                        if(Image_Lefthand_Line[leftstart][1]==y1-5)
                        {
                            left[0]=leftstart;
                        }
                        if(Image_Lefthand_Line[leftstart][1]==y1)
                        {
                            left[1]=leftstart;
                        }
                        if(Image_Lefthand_Line[leftstart][1]==y1+5)
                        {
                            left[2]=leftstart;
                        }
                    }
                    for(rightstart=0;rightstart<Right_num;rightstart++)
                    {
                        if(Image_Righthand_Line[rightstart][1]==y1-5)
                        {
                            right[0]=rightstart;
                        }
                        if(Image_Righthand_Line[rightstart][1]==y1)
                        {
                            right[1]=rightstart;
                        }
                        if(Image_Righthand_Line[rightstart][1]==y1+5)
                        {
                            right[2]=rightstart;
                        }
                    }
                    width[0]=abs(Image_Lefthand_Line[left[0]][0]-Image_Righthand_Line[right[0]][0]);
                    width[1]=abs(Image_Lefthand_Line[left[1]][0]-Image_Righthand_Line[right[1]][0]);
                    width[2]=abs(Image_Lefthand_Line[left[2]][0]-Image_Righthand_Line[right[2]][0]);
                    if(width[2]-width[0]>10)
                    {
                            zcout++;
                            if(zcout>5)
                            {
                                Ele->Obstruction=1;
                                zhangaiflag=1;
                            }

                    }
                }
                if(X1!=0&&X2!=0&&X3!=0&&Y1<Y2&&Y1>Y3)
                {
                    for(leftstart=0;leftstart<Left_num;leftstart++)
                    {
                        if(Image_Lefthand_Line[leftstart][1]==Y1-5)
                        {
                            left[0]=leftstart;
                        }
                        if(Image_Lefthand_Line[leftstart][1]==Y1)
                        {
                            left[1]=leftstart;
                        }
                        if(Image_Lefthand_Line[leftstart][1]==Y1+5)
                        {
                            left[2]=leftstart;
                        }
                    }
                    for(rightstart=0;rightstart<Right_num;rightstart++)
                    {
                        if(Image_Righthand_Line[rightstart][1]==Y1-5)
                        {
                            right[0]=rightstart;
                        }
                        if(Image_Righthand_Line[rightstart][1]==Y1)
                        {
                            right[1]=rightstart;
                        }
                        if(Image_Righthand_Line[rightstart][1]==Y1+5)
                        {
                            right[2]=rightstart;
                        }
                    }
                    width[0]=abs(Image_Lefthand_Line[left[0]][0]-Image_Righthand_Line[right[0]][0]);
                    width[1]=abs(Image_Lefthand_Line[left[1]][0]-Image_Righthand_Line[right[1]][0]);
                    width[2]=abs(Image_Lefthand_Line[left[2]][0]-Image_Righthand_Line[right[2]][0]);
                    if(width[2]-width[0]>10)
                    {
                        zcout++;
                        if(zcout>5)
                        {
                            Ele->Obstruction=2;
                            zhangaiflag=2;
                        }
                    }
                }
                break;
            case 1:
                tmperr=guaidian1[6][1];
                if(x3==0)
                {
                    Ele->Obstruction=0;
                    zhangaiflag=0;
                    zcout=0;
                }
                break;
            case 2:
                tmperr=guaidian2[6][1];
                if(X3==0)
                {
                    Ele->Obstruction=0;
                    zhangaiflag=0;
                    zcout=0;
                }
                break;
        }
//    int x1=guaidian1[4][0];
//    int y1=guaidian1[4][1];
//    int X1=guaidian1[4][0];
//    int Y1=guaidian1[4][1];



//    if(current_state1==99&&current_state2==0&&X1!=0)
//    {
//        jishu++;
//        if(jishu>10)
//        {
////            Ele->Obstruction=1;
//            Beep_on;
//            jishu=0;
//        }
//
//    }

}

/*************************************************************************
*  函数名称：void Image_ZebraProcess(Image_element* Ele)
*  参数说明: 斑马线处理函数
*  功能说明：
*  函数返回：无
*  修改时间：2023年7月11日
*  备    注：
*************************************************************************/
void Image_ZebraProcess(Image_element* Ele)
{
    int leftstart = -1, rightstart = -1, start = 0, counter = 0, wid = 0;

    // 找到左边起始点
    for (int i = 0; i < Left_num; i++) {
        if (Image_Lefthand_Line[i][1] == 70) {
            leftstart = i;
            break;
        }
    }

    // 找到右边起始点
    for (int i = 0; i < Right_num; i++) {
        if (Image_Righthand_Line[i][1] == Image_Lefthand_Line[leftstart][1]) {
            rightstart = i;
            break;
        }
    }

    // 如果找到有效的左边和右边起始点
    if (leftstart != -1 && rightstart != -1) {
        for (int i = Image_Lefthand_Line[leftstart][0] + 2; i < Image_Righthand_Line[rightstart][0]; i++) {
            if (start == 0) {
                if (abs(Image_Use[70][i] - Image_Use[70][i - 1]) > 30) {
                    start = 1;
                }
            } else {
                wid++;
                if (abs(Image_Use[70][i] - Image_Use[70][i - 1]) > 30) {
                    if (wid > 2) counter++;
                    wid = 0;
                    start = 0;
                    if (counter > 3) {
                        Ele->Zebra = 1;
                        Motor_openFlag=0;
                        open_flag=0;
                        //控制蜂鸣器
//                        Control_Beep(&Element1);
                        break;
                    }
                }
            }
        }
    }

    // 如果检测到斑马线
    if (Ele->Zebra == 1) {
        Ele->Zebra = 0;
        close_flag=1;
        Motor_openFlag=0;
        ok_flag=0;
//        Servo_openFlag = 0;
//        MotorCtrl(0, 0);
//        ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);
        Set_NULLflag();

    }
}
/*************************************************************************
*  函数名称：void My_ObstructionProcess(Image_element* Ele)
*  参数说明：
*  功能说明：长直道加速
*  函数返回：无
*  修改时间：2023年6月3日
*  备    注：
*************************************************************************/
void Straightaway_Process(Image_element* Ele)
{
    switch(Ele->Straightaway_flag)
    {
        case 0:
            if(qvlv_quanju_right<20&&qvlv_quanju_left<20&&abs(qulv_value)<8&&current_state1==0&&current_state2==0)
            {
                Ele->Straightaway_flag=1;
//                keep_beep=5;
                if(Motor_openFlag)
                {
                    PID_flag_Direction=1;
                }
            }
            break;
        case 1:
            if(abs(qulv_value)>15)
                {
                    Ele->Straightaway_flag=0;
                }
            break;
    }
}
/*************************************************************************
*  函数名称：void Small_Curve_Process(Image_element* Ele)
*  参数说明：
*  功能说明：小S弯
*  函数返回：无
*  修改时间：2023年6月3日
*  备    注：
*************************************************************************/
void Small_Curve_Process(Image_element* Ele)
{
    switch(Ele->Small_Curve_flag)
    {
        case 0:
                if(qvlv_quanju_right>15&&qvlv_quanju_left>15&&abs(qulv_value)<15&&zuodianxian<30&&youdianxian<30&&(current_state1!=0||current_state2!=0)&&guaidian1[1][0]!=0&&guaidian2[1][0]!=0)
                {
                    Ele->Small_Curve_flag=1;
                }
            break;
        case 1:
             if((qvlv_quanju_right<12&&qvlv_quanju_left<12&&abs(qulv_value)<8&&current_state1==0&&current_state2==0&&guaidian1[1][0]==0&&guaidian2[1][0]==0)||(abs(qulv_value)>25))
             {
                Ele->Small_Curve_flag=0;
             }
            break;
    }
}
void Curve_Process(Image_element* Ele)
{
    switch(Ele->Curve_flag)
    {
        case 0:
            if(qvlv_quanju_right>20&&qvlv_quanju_left>20&&((zuodianxian>60&&youdianxian<10)||(zuodianxian<10&&youdianxian>40))&&(current_state1==1||current_state2==1))
            {
//                keep_beep=3;
                keep_beep=2;
                Ele->Curve_flag=1;
                cout=0;
            }
            break;
        case 1:
            if(current_state1==0&&current_state2==0)
            {
                cout++;
            }
            if(cout>5)
            {
//                keep_beep=4;
                Ele->Curve_flag=0;
                cout=0;
            }
            break;
    }
}
void zuo_Process(Image_element* Ele)
{
    switch(Ele->zuo_flag)
    {
        case 0:
            if(qulv_value>40&&Servo_Loc_error>15)
            {
                Ele->zuo_flag=1;
            }
            break;
        case 1:
            if(qulv_value<10||Servo_Loc_error<10)Ele->zuo_flag=0;
            break;
    }
}
void Set_NULLflag(void)
{
    Element1.Zebra=0;
    Element1.Circle_left=0;
    Element1.Circle_right=0;
    Element1.Crossroad_flag=0;
    Element1.Curve_flag=0;
    Element1.Rampway_flag=0;
    Element1.Straightaway_flag=0;
    Element1.Small_Curve_flag=0;
    Element1.Curve_flag=0;
    Element1.zuo_flag=0;
}


/*************************************************************************
*  函数名称：void Calcu_Pulse(void)
*  参数说明：
*  状态：
*  功能说明：计算全元素脉冲
*  函数返回：无
*  修改时间：2023年5月30日
*  备    注：
*************************************************************************/
void Calcu_Pulse(void)
{
    //100Pulse大约为0.78cm
    for(int i=0;i<6;i++)
        My_PulseCount(&ALLPULSE,i);//计数器计数
}


/*************************************************************************
*  函数名称：void My_PulseCount(Pulse_Param_t * PULSE,int element)
*  参数说明：元素：0十字;1左圆环;2右圆环;3路障;4弯道;5斑马线
*  状态：开或关
*  功能说明：脉冲计数器
*  函数返回：无
*  修改时间：2023年5月30日
*  备    注：
*************************************************************************/
void My_PulseCount(Pulse_Param_t * PULSE,int element)
{
    if(element==0)//十字
    {
        if(PULSE->Cross_Pulse_Key==1)
        {
            PULSE->Cross_Pulse+=(speedL1+speedR1)/2;
            PULSE->Cross_Pulse=(PULSE->Cross__Pulse_Last*9+PULSE->Cross_Pulse)/10;//滤波得到总计数
            PULSE->Cross__Pulse_Last=PULSE->Cross_Pulse;//记录上一级的值
        }
        else {
            PULSE->Cross_Pulse=0;
            PULSE->Cross__Pulse_Last=0;
        }
    }
    else if(element==1)//左圆环
    {
        if(PULSE->Circle_Left_Pulse_Key==1)
        {
            PULSE->Circle_Left_Pulse+=(speedL1+speedR1)/2;
            PULSE->Circle_Left_Pulse=(PULSE->Circle_Left_Pulse_Last*9 + PULSE->Circle_Left_Pulse)/10;//滤波得到总计数
            PULSE->Circle_Left_Pulse_Last=PULSE->Circle_Left_Pulse;
        }
        else {
            PULSE->Circle_Left_Pulse=0;
            PULSE->Circle_Left_Pulse_Last=0;
        }
    }
    else if(element==2)//右圆环
    {
        if(PULSE->Circle_Right_Pulse_Key==1)
        {
            PULSE->Circle_Right_Pulse+=(speedL1+speedR1)/2;
            PULSE->Circle_Right_Pulse=(PULSE->Circle_Right_Pulse_Last*9 + PULSE->Circle_Right_Pulse)/10;//滤波得到总计数
            PULSE->Circle_Right_Pulse_Last=PULSE->Circle_Right_Pulse;
        }
        else {
            PULSE->Circle_Right_Pulse=0;
            PULSE->Circle_Right_Pulse_Last=0;
        }
    }
    else if(element==3)//路障
    {
        if(PULSE->Obstruction_Pulse_Key==1)
        {
            PULSE->Obstruction_Pulse+=(speedL1+speedR1)/2;
            PULSE->Obstruction_Pulse=(PULSE->Obstruction_Pulse_Last*9 + PULSE->Obstruction_Pulse)/10;//滤波得到总计数
            PULSE->Obstruction_Pulse_Last=PULSE->Obstruction_Pulse;
        }
        else {
            PULSE->Obstruction_Pulse=0;
            PULSE->Obstruction_Pulse_Last=0;
        }
    }
    else if(element==4)//弯道
    {
        if(PULSE->Curve_Pulse_Key==1)
        {
            PULSE->Curve_Pulse+=(speedL1+speedR1)/2;
            PULSE->Curve_Pulse=(PULSE->Curve_Pulse_Last*9 + PULSE->Curve_Pulse)/10;//滤波得到总计数
            PULSE->Curve_Pulse_Last=PULSE->Curve_Pulse;
        }
        else {
            PULSE->Curve_Pulse=0;
            PULSE->Curve_Pulse_Last=0;
        }
    }
    else if(element==5)//斑马线
    {
        if(PULSE->Zebra_Pulse_Key==1)
        {
            PULSE->Zebra_Pulse+=(speedL1+speedR1)/2;
            PULSE->Zebra_Pulse=(PULSE->Zebra_Pulse_Last*9 + PULSE->Zebra_Pulse)/10;//滤波得到总计数
            PULSE->Zebra_Pulse_Last=PULSE->Zebra_Pulse;
        }
        else {
            PULSE->Zebra_Pulse=0;
            PULSE->Zebra_Pulse_Last=0;
        }
    }
}


/*************************************************************************
*  函数名称：void My_PulseClean(Pulse_Param_t * PULSE,int element)
*  参数说明：元素：0十字;1左圆环;2右圆环;3路障;4弯道;5斑马线
*  状态：开或关
*  功能说明：脉冲计数器清零
*  函数返回：无
*  修改时间：2023年5月30日
*  备    注：
*************************************************************************/
void My_PulseClean(Pulse_Param_t * PULSE,int element)
{
    if(element==0)//十字
    {
        PULSE->Cross_Pulse=0;
        PULSE->Cross__Pulse_Last=0;

    }
    else if(element==1)//左圆环
    {
        PULSE->Circle_Left_Pulse=0;
        PULSE->Circle_Left_Pulse_Last=0;
    }
    else if(element==2)//右圆环
    {
        PULSE->Circle_Right_Pulse=0;
        PULSE->Circle_Right_Pulse_Last=0;
    }
    else if(element==3)//路障
    {
        PULSE->Obstruction_Pulse=0;
        PULSE->Obstruction_Pulse_Last=0;
    }
    else if(element==4)//弯道
    {
        PULSE->Curve_Pulse=0;
        PULSE->Curve_Pulse_Last=0;
    }
    else if(element==5)//斑马线
    {
        PULSE->Zebra_Pulse=0;
        PULSE->Zebra_Pulse_Last=0;
    }
}

void Image_IdentifySet(Image_Identify * Ide, Image_element* Ele)
{
#if 1
    if(Ele->Zebra!=0)//路障优先级1
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=1;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        Ide->zuo_Identify=0;
//        My_ElementClean(Ele,4);
    }
    else if(Ele->Crossroad_flag!=0)//十字优先级1
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=1;
        Ide->Obstruction_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,0);
    }
    else if(Ele->Circle_left>2)//左圆环优先级4
    {
        Ide->Circle_left_Identify=1;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,1);
    }
    else if(Ele->Circle_right>2)//右圆环优先级5
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=1;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,2);
    }
    else if(Ele->Obstruction!=0)//右圆环优先级5
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=1;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,5);
    }
    else if(Ele->Straightaway_flag!=0)//直线优先级
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=1;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        Ide->zuo_Identify=0;

        My_ElementClean(Ele,3);
    }
    else if(Ele->Curve_flag!=0)//弯道优先级
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=1;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,6);
    }
    else if(Ele->Small_Curve_flag!=0)//小s弯
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=1;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,4);
    }
    else if(Ele->zuo_flag!=0)
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->zuo_Identify=0;
        My_ElementClean(Ele,7);
    }
    else {
        //一个元素都没识别，全开
        //1代表开启元素识别
        Ide->Circle_left_Identify=1;
        Ide->Circle_Right_Identify=1;
        Ide->Cross_Identify=1;
        Ide->Obstruction_Identify=1;
        Ide->Zebra_Identify=1;
        Ide->All_Identify=1;//全元素识别
        Ide->Straightaway_Identify=1;
        Ide->Small_Curve_Identify=1;
        Ide->Curve_Identify=1;
        Ide->zuo_Identify=1;
    }

#else
    if(Ele->Zebra!=0)//路障优先级1
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=1;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
//        My_ElementClean(Ele,4);
    }
    else if(Ele->Crossroad_flag!=0)//十字优先级1
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=1;
        Ide->Obstruction_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        My_ElementClean(Ele,0);
    }
    else if(Ele->Circle_left>2)//左圆环优先级4
    {
        Ide->Circle_left_Identify=1;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        My_ElementClean(Ele,1);
    }
    else if(Ele->Circle_right>2)//右圆环优先级5
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=1;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
        My_ElementClean(Ele,2);
    }
    else if(Ele->Obstruction!=0)//右圆环优先级5
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=1;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=0;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;
//        My_ElementClean(Ele,5);
    }
    else if(Ele->Straightaway_flag!=0)//直线优先级
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=1;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=0;

        My_ElementClean(Ele,3);
    }
    else if(Ele->Curve_flag!=0)//弯道优先级
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        Ide->Curve_Identify=1;
//        My_ElementClean(Ele,5);
    }
    else if(Ele->Small_Curve_flag!=0)//小s弯
    {
        Ide->Circle_left_Identify=0;
        Ide->Circle_Right_Identify=0;
        Ide->Cross_Identify=0;
        Ide->Obstruction_Identify=0;
        Ide->Zebra_Identify=0;
        Ide->All_Identify=1;//非全元素识别
        Ide->Straightaway_Identify=0;
        Ide->Small_Curve_Identify=0;
        My_ElementClean(Ele,4);
    }
    else {
        //一个元素都没识别，全开
        //1代表开启元素识别
        Ide->Circle_left_Identify=1;
        Ide->Circle_Right_Identify=1;
        Ide->Cross_Identify=1;
        Ide->Obstruction_Identify=1;
        Ide->Zebra_Identify=1;
        Ide->All_Identify=1;//全元素识别
        Ide->Straightaway_Identify=1;
        Ide->Small_Curve_Identify=1;
        Ide->Curve_Identify=1;
    }
#endif
}

/*************************************************************************
*  函数名称：void My_PulseKey(Pulse_Param_t * PULSE,int element,int state)
*  参数说明：元素：0十字;1左圆环;2右圆环;3路障;4弯道;5斑马线
*  状态：1开或0关
*  功能说明：脉冲计数器开关
*  函数返回：无
*  修改时间：2023年5月30日
*  备    注：
*************************************************************************/
void My_PulseKey(Pulse_Param_t * PULSE,int element,unsigned char state)
{
    if(element==0)//十字
    {
       PULSE->Cross_Pulse_Key=state;

    }
    else if(element==1)//左圆环
    {
       PULSE->Circle_Left_Pulse_Key=state;

    }
    else if(element==2)//右圆环
    {
       PULSE->Circle_Right_Pulse_Key=state;
    }
    else if(element==3)//路障
    {
       PULSE->Obstruction_Pulse_Key=state;
    }
    else if(element==4)//弯道
    {
       PULSE->Curve_Pulse_Key=state;
    }
    else if(element==5)//斑马线
    {
       PULSE->Zebra_Pulse_Key=state;
    }
}

char count_beep=0;
char keep_beep=0;
/*************************************************************************
*  函数名称：void Control_Beep(Image_element* Image_flag)
*  参数说明：元素判断标志结构体
*  状态：  斑马线：滴一声，障碍物：滴两声，十字路口：滴三声，左圆环：响1s，右圆环：响2s，蜂鸣器处理在CCU6中
*  功能说明：当识别到元素时，让蜂鸣器响或者滴不同的声音，用来判断识别到什么元素，有没有误判
*  函数返回：无
*  修改时间：2024年7月6日
*  备    注：
*************************************************************************/
void Control_Beep(Image_element* Image_flag)
{
    if(Image_flag->Zebra==1)
    {
        count_beep=1;
    }
    else if(Image_flag->Obstruction==1)
    {
        count_beep=2;
    }
    else if(Image_flag->Crossroad_flag==1)
    {
        count_beep=3;
    }
    else if(Image_flag->Circle_left==2)
    {
        keep_beep=1;
    }
    else if(Image_flag->Circle_right==2)
    {
        keep_beep=2;
    }
}

/*************************************************************************
*  函数名称：void Speed_Decision(Image_element* element)
*  参数说明：元素赛道状态信息
*  状态：   直道加速，弯道减速
*  功能说明：
*  函数返回：无
*  修改时间：2024年7月6日
*  备    注：转向决策函数，当不同元素时修改kp的值
*************************************************************************/
void Direction_Decision(Image_element* element)
{
#if 1
    PID_flag_Direction=0;
    if(element->Straightaway_flag==1)
    {
        PID_flag_Direction=1;
        Servo_Loc_PID_tmp.kp=1;
    }
    else if(element->Small_Curve_flag==1)
    {
        PID_flag_Direction=1;
        Servo_Loc_PID_tmp.kp=0.5;
    }
//    else if(element->Circle_left==1)
//    {
//        PID_flag_Direction=1;
//        Servo_Loc_PID_tmp.kp=3;
//    }
    else
    {
        PID_flag_Direction=0;
    }

//    else if(element->Circle_left==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
//    else if(element->Circle_right==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
//    else if(element->Crossroad_flag==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
//    else if(element->Curve_flag==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
#else
    Target_Speed=Refer_Speed;

#endif
}

/*************************************************************************
*  函数名称：void Speed_Decision(Image_element* element)
*  参数说明：元素赛道状态信息
*  状态：   直道小kp，弯道大kp
*  功能说明：
*  函数返回：无
*  修改时间：2024年7月6日
*  备    注：速度决策函数
*************************************************************************/
void Speed_Decision(Image_element* element)
{
#if 1
    if(element->Straightaway_flag==1)
    {
        Target_Speed=Refer_Speed+My_Speed_up.Straight_Speed_up;
    }
    else if(element->Small_Curve_flag==1)
    {
        if(Speed_up_flag){Target_Speed=Refer_Speed+My_Speed_up.Small_Curve_Speed_up;}
        else{Target_Speed=Refer_Speed;}
    }
    else if(element->Circle_left>2||element->Circle_right>2)
    {
        Target_Speed=Refer_Speed+My_Speed_up.Left_Circle_Speed_up;
    }
    else if(element->Curve_flag==1)
    {
        Target_Speed=Refer_Speed+My_Speed_up.Curve_Speed_up;
    }
    else if(element->zuo_flag==1)
    {
        Target_Speed=Refer_Speed+My_Speed_up.zuo_up;
    }
    else
    {
        Target_Speed=Refer_Speed;
    }


//    else if(element->Circle_left==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
//    else if(element->Circle_right==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
//    else if(element->Crossroad_flag==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
//    else if(element->Curve_flag==1)
//    {
//        Target_Speed=Refer_Speed-20;
//    }
    #else
        Target_Speed=Refer_Speed;

    #endif
}
