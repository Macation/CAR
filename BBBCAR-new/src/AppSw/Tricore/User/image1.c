/*
 * newimage.c
 *
 *  Created on: 2024年5月14日
 *      Author: Macation
 */
#include "image1.h"

#define Image_downLine 110
#define Image_upLine 15
#define central_length Image_downLine-Image_upLine
short OSTU=0;
int Image_centralLine[central_length];//存放边线的数组
float a=0;
int cout=0;
int qulv_value;
int Image_Lefthand_Line[Array_Length][2];//存放提取后的边线数组的坐标
int Image_Righthand_Line[Array_Length][2];
int newImage_Lefthand_Line[Array_Length][2];
int newImage_Righthand_Line[Array_Length][2];
unsigned char Image_Line[120][160];//存放边线的数组，0，1表示
int Left_num=Array_Length;
int Right_num=Array_Length;
int block_size=7;
int Servo_Loc_error;  //横向误差
int Servo_QZ_error;   //前瞻误差
int Servo_All_error;  //合成偏差
int image_error[6];
uint8 image_flag;
float currentslope=0;
float currentslope2=0;
double previous_slope = 0, current_slope = 0,sum=0;
int current_state1=0;
int current_state2=0;
int guaidian1[Array_Length][2];
int guaidian2[Array_Length][2];
int guaidiannum1=0;
int shiziflag=0;
int threshold=100;
int qvlv_value=0;
int qulv_jin;
int qulv_yuan;
int qvlv_jin_left;
int qvlv_jin_right;
int qvlv_yuan_left;
int qvlv_yuan_right;
int qvlv_quanju_right,qvlv_quanju_left=0;
//存储了前方，左前，右前方的方向数组
int dir_front[4][2]=   {{0,-1},
                        {1,0},
                        {0,1},
                        {-1,0}};
int dir_frontleft[4][2]=   {{-1,-1},
                            {1,-1},
                            {1,1},
                            {-1,1}};
int dir_frontright[4][2]=   {{1,-1},
                            {1,1},
                            {-1,1},
                            {-1,-1}};
int crossflag=0;
int shizi=0;
int flag=0;
int zuodianxian,youdianxian=0;
/*************************************************************************
*  函数名称：int Image_Threshold_Line(int Y)
*  功能说明：对单行找阈值
*  函数返回：无
*  修改时间：2023年6月15日
*  备    注：
*************************************************************************/
int Image_Threshold_Line(int Y)
{
    signed int i, j;
    unsigned long Amount = 0;
    unsigned long PixelBack = 0;
    unsigned long PixelshortegralBack = 0;
    unsigned long Pixelshortegral = 0;
    signed long PixelshortegralFore = 0;
    signed long PixelFore = 0;
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // 类间方差;
    int MinValue, MaxValue;
    int Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //初始化灰度直方图

    for(i=0;i<MT9V034_IMAGEW;i++)
    {
        HistoGram[Image_Data[Y][i]]++;
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ;        //获取最小灰度的值
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0;
            MaxValue--)
        ; //获取最大灰度的值

    if (MaxValue == MinValue)
        return MaxValue;         // 图像中只有一个颜色
    if (MinValue + 1 == MaxValue)
        return MinValue;        // 图像中只有二个颜色

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  像素总数

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++) {
        Pixelshortegral += HistoGram[j] * j;        //灰度值总数
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++) {
        PixelBack = PixelBack + HistoGram[j];     //前景像素点数
        PixelFore = Amount - PixelBack;           //背景像素点数
        OmegaBack = (float) PixelBack / Amount;   //前景像素百分比
        OmegaFore = (float) PixelFore / Amount;   //背景像素百分比
        PixelshortegralBack += HistoGram[j] * j;  //前景灰度值
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //背景灰度值
        MicroBack = (float) PixelshortegralBack / PixelBack;   //前景灰度百分比
        MicroFore = (float) PixelshortegralFore / PixelFore;   //背景灰度百分比
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore)
                * (MicroBack - MicroFore);   //计算类间方差
        if (Sigma > SigmaB)                    //遍历最大的类间方差g //找出最大类间方差以及对应的阈值
                {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //返回最佳阈值;
}
/*************************************************************************
*  函数名称：void Image_Frameprocessing(void)
*  参数说明：
*  功能说明：画黑框
*  函数返回：无
*  修改时间：2024年5月14日
*  备    注：
*************************************************************************/
void Image_Frameprocessing(void)
{
    for(int i=0;i<LCDH;i++)
    {
        Image_Use[i][3]=0;
        Image_Use[i][LCDW-3]=0;
    }
}

/*************************************************************************
*  函数名称：int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value)
*  参数说明：
*  功能说明：左手迷宫巡线
*  函数返回：无
*  修改时间：2024年5月14日
*  备    注：
*************************************************************************/
int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value)
{
    int half=block_size/2;//方块的一半
    int step = 0, dir = 0, turn = 0;//已走的步数，当前方向，转向的次数
    while(step<num && half<X && X<LCDW-half-1 && half< Y && Y <LCDH-1 && turn<4)//循环终止条件，不越界且转向次数小于4，防止原地打转
    {
        int local_thres = 0; //自适应阈值
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += Image_Use [Y+dy][X+dx];//方块内阈值累加
            }
        }
        local_thres /= block_size * block_size;//阈值取平均
        local_thres -= clip_value;//阈值做裁剪，可以自己调
        int front_value = Image_Use[Y+dir_front[dir][1]][X+dir_front[dir][0]];//当前像素前方的点的像素值；
        int frontleft_value = Image_Use[Y+dir_frontleft[dir][1]][X+dir_frontleft[dir][0]];//当前像素左前方的点的像素值
        if(front_value<local_thres)//前方像素为黑，向右转，转向次数+1
        {
            dir=(dir + 1) % 4;
            turn++;
        }
        else if(frontleft_value<local_thres)//左前方像素为黑，直走，转向次数清0，步数+1
        {
            Y+=dir_front[dir][1];
            X+=dir_front[dir][0];
            Image_Lefthand_Line[step][0]=X;
            Image_Lefthand_Line[step][1]=Y;
            if(X==4)zuodianxian++;
            Image_Line[Y][X]=1;
            step++;
            turn=0;
        }
        else
        {
            Y += dir_frontleft[dir][1];
            X += dir_frontleft[dir][0];
            Image_Lefthand_Line[step][0]=X;
            Image_Lefthand_Line[step][1]=Y;
            if(X==4)zuodianxian++;
            Image_Line[Y][X]=1;
            dir=(dir + 3) % 4;
            step++;
            turn=0;
        }
    }
    return step;
}
/*************************************************************************
*  函数名称：int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value)
*  参数说明：
*  功能说明：右手迷宫巡线
*  函数返回：无
*  修改时间：2024年5月14日
*  备    注：
*************************************************************************/
int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value)
{
    int half=block_size/2;//方块的一半
    int step = 0, dir = 0, turn = 0;//已走的步数，当前方向，转向的次数
    while(step<num&& half<X && X<LCDW-half && half< Y && Y <LCDH-1 && turn<4)//循环终止条件，不越界且转向次数小于4，防止原地打转)
    {
        int local_thres = 0; //自适应阈值
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += Image_Use [Y+dy][X+dx];//方块内阈值累加
            }
        }
        local_thres /= block_size * block_size;//阈值取平均
        local_thres -= clip_value;//阈值做裁剪，可以自己调
        int front_value = Image_Use[Y+dir_front[dir][1]][X+dir_front[dir][0]];//当前像素前方的点的像素值；
        int frontright_value=Image_Use[Y+dir_frontright[dir][1]][X+dir_frontright[dir][0]];
        if(front_value<local_thres)
        {
            dir = (dir + 3) % 4;
            turn++;
        }
        else if(frontright_value < local_thres)
        {
            Y+=dir_front[dir][1];
            X+=dir_front[dir][0];
            Image_Righthand_Line[step][0]=X;
            Image_Righthand_Line[step][1]=Y;
            if(X==156)youdianxian++;
            Image_Line[Y][X]=1;
            step++;
            turn=0;
        }
        else
        {
            Y += dir_frontright[dir][1];
            X += dir_frontright[dir][0];
            Image_Righthand_Line[step][0]=X;
            Image_Righthand_Line[step][1]=Y;
            if(X==156)youdianxian++;
            Image_Line[Y][X]=1;
            dir=(dir + 1) % 4;
            step++;
            turn=0;
        }
    }
    return step;
}
/*************************************************************************
*  函数名称：void Image_SeedGrowing(void)
*  功能说明：用区域生长法找左右边线函数
*  函数返回：无
*  修改时间：2024年5月14日
*  备    注：
*
*************************************************************************/
void Image_SeedGrowing(void)
{
    if(thresholdflag)
    {
        OSTU=GetOSTU(Image_Use);
    }
    else
    {
        OSTU=0;

    }
//    threshold=OSTU;
    cout=0;
    zuodianxian=0;
    youdianxian=0;
    int X1=Image_findSeedBegin(100);
    int Y1=Image_downLine;
    int X2=X1;
    int Y2=Y1;
    memset(Image_Line,0,sizeof(Image_Line));
    memset(Image_Lefthand_Line,0,sizeof(Image_Lefthand_Line));
    memset(Image_Righthand_Line,0,sizeof(Image_Righthand_Line));
    memset(guaidian1,0,sizeof(guaidian1));
    memset(guaidian2,0,sizeof(guaidian2));
    for(;X1>0;X1--)//寻找跳变点
    {
        if(Image_Use[Y1][X1-1]<threshold)break;
    }
    Left_num=Array_Length;
    Left_num=Image_Findline_Lefthand(X1,Y1,Left_num,block_size,0);
    for(;X2<LCDW-1;X2++)//寻找跳变点
    {
        if(Image_Use[Y2][X2+1]<threshold)break;
    }
    Right_num=Array_Length;
    Right_num=Image_Findline_Righthand(X2,Y2,Right_num,block_size,0);
    int leftstart;
    for(leftstart=0;leftstart<Left_num;leftstart++)
    {
        if(Image_Lefthand_Line[leftstart][1]==60)break;
    }
    if(Left_num>60)
    {
        for(int j=0;j<79;j++)
        {
            Image_Line[Image_Lefthand_Line[leftstart][1]][j]=1;
            currentslope=linearRegression(Image_Lefthand_Line, leftstart-10, leftstart+10);
//            currentslope=calculateCurvature(Image_Lefthand_Line,leftstart,Left_num);
        }
    }
    int rightstart;
    for(rightstart=0;rightstart<Right_num;rightstart++)
    {
        if(Image_Righthand_Line[rightstart][1]==Image_Lefthand_Line[leftstart][1])break;
    }
    if(Right_num-rightstart>20&&rightstart>10)
    {
        for(int s=159;s>80;s--)
        {
            Image_Line[Image_Righthand_Line[rightstart][1]][s]=1;
            currentslope2=linearRegression(Image_Righthand_Line, rightstart-10, rightstart+10);
//            currentslope2=calculateCurvature(Image_Righthand_Line,rightstart,Right_num);
        }
    }
    current_state1=find(currentslope);
    current_state2=find(currentslope2);
    flag=0;
    leftxielv(leftstart,rightstart);
        Image_IdentifyProcess(&Identify1);//识别处理
        Image_IdentifySet(&Identify1, &Element1);//设置识别优先级
    Image_seedCentralline(X1,X2);//画中线
    qulv();

//    if(current_state1==1&&current_state2==1)
//    {
//         Beep_on;
//    }
}
/*************************************************************************
*  函数名称：int Image_findSeedBegin(Image_element* Ele, int thres)
*  功能说明：找种子的起始点函数
*  函数返回：无
*  修改时间：2023年4月23日
*  备    注：
*************************************************************************/
int Image_findSeedBegin(int thres)
{
    int i=0;
    for(i=79;i>2;i--)
    {
        if(Image_Use[Image_downLine][i-1]>thres && Image_Use[Image_downLine][i]>thres
                && Image_Use[Image_downLine][i+1]>thres)
        {
            return i;
        }
    }
    for(i=81;i<MT9V034_IMAGEW -2;i++)
    {
        if(Image_Use[Image_downLine][i-1]>thres && Image_Use[Image_downLine][i]>thres
                && Image_Use[Image_downLine][i+1]>thres)
        {
            return i;
        }
    }

    return LCDW/2;
}
/*************************************************************************
*  函数名称：void Image_seedCentralline(int X,int Y)
*  功能说明：用区域生长法找左右边线函数
*  函数返回：无
*  修改时间：2024年5月15日
*  备    注：
*
*************************************************************************/
void Image_seedCentralline(int X1,int X2)
{
    int  i,j;
    //扫描边线时每一行的起点
    int LeftStart=X1+5;
    int RightStart=X2-5;
    int Left[central_length]={2};
    int Right[central_length]={LCDW-4};
    int qidian;
    for(i=Image_downLine-1;i>Image_upLine;i--)//由于生长法存的第一个点在Image_downLine上一行，所以取点也要在上一行
    {
        //防止起点超过图像范围
        if(RightStart>LCDW-4)
        {
            RightStart=LCDW-4;
        }
        if(LeftStart<2)
        {
            LeftStart=2;
        }
        //零阶保持，如果在该行未找到跳变点，则同上一个循环的结果保持一致
//        if(i<Image_downLine-1)
//        {
//            Left[i-Image_upLine]=Left[i+1-Image_upLine];
//            Right[i-Image_upLine]=Right[i+1-Image_upLine];
//
//        }
        //扫左线，由于最后一行的起点在左种子附近，所以不从中间点开始找跳变
        for(j=LeftStart;j > 0;j--)
        {
            if(Image_Line[i][j]==1)//找到跳变点
            {
                Left[i-Image_upLine]=j;

                break;
            }
        }
        //扫右线，同左线相似
        for(j=RightStart;j< LCDW-1;j++)
        {
            if(Image_Line[i][j]==1)//找到跳变点
            {
                Right[i-Image_upLine]=j;

                break;
            }
        }
//        //规划下一次的起点
        qidian=(Left[i-Image_upLine]+Right[i-Image_upLine])/2;
        LeftStart=qidian+10;
        RightStart=qidian-10;
//        if(Element1.Crossroad_flag!=0)
//        {
//            LeftStart=80;
//            RightStart=80;
//        }
    }

        for(i=0;i<central_length;i++)
        {
            Image_centralLine[i]=(Left[i]+Right[i])/2;
        }

    //在图像中画出中线
    for(i=Image_downLine-1;i>Image_upLine-1;i--)
    {
        j=Image_centralLine[i-Image_upLine];
        //加粗，两个像素点
        Image_Line[i][j]=1;
        Image_Line[i][j-1]=1;

    }
}
/*************************************************************************
*  函数名称：void Image_seedCentralline(int X,int Y)
*  功能说明：计算直线段斜率
*  函数返回：无
*  修改时间：2024年5月20日
*  备    注：
*
*************************************************************************/
float linearRegression(int path[][2], int start, int end) {
    int n = end - start + 1;
    float slope;
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (int i = start; i <= end; ++i) {
        int x = path[i][0];
        int y = path[i][1];
        if(path[i][0]==4&&path[i-1][0]==4&&path[i+1][0]==4)return 0;
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }

    float denominator = (float)(n * sum_x2 - sum_x * sum_x);
    if (denominator == 0) {
        slope = 0;
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denominator;
    }
    return slope;
}
float linearRegression2zhong(int path[], int start, int end) {
    int n = end - start + 1;
    float slope;
    float sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (int i = start; i <= end; ++i) {
        int x = path[i];
        int y = i;  // 纵坐标为索引值
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }

    float denominator = (float)(n * sum_x2 - sum_x * sum_x);
    if (denominator == 0) {
        slope = 0;
    } else {
        slope = (n * sum_xy - sum_x * sum_y) / denominator;
    }
    return slope;
}
/*************************************************************************
*  函数名称：void leftxielv()
*  功能说明：拐点检测函数
*  函数返回：无
*  修改时间：
*  备    注：
*
*************************************************************************/
void leftxielv(int leftstart,int rightstart)
{
    for(int i=0;i<Left_num-10;i++)
    {
        int x1,y1,x2,y2,x3,y3=0;
        x1=Image_Lefthand_Line[i][0];
        y1=Image_Lefthand_Line[i][1];
        x2=Image_Lefthand_Line[i+5][0];
        y2=Image_Lefthand_Line[i+5][1];
        x3=Image_Lefthand_Line[i+10][0];
        y3=Image_Lefthand_Line[i+10][1];
        if(y2<=20)break;
        if(x1<x2 && y1>y2 && x3<x2 && y3>y2)//下拐点
        {
            guaidian1[0][0]=x2;
            guaidian1[0][1]=y2;
        }
        if(x1<x2 && y1>y2 && x3<x2 && y3<y2)//中拐点
        {
            guaidian1[1][0]=x2;
            guaidian1[1][1]=y2;
        }
        if(x1<x2 && y1<y2 && x3>x2 && y3<y2)//上拐点
        {
            guaidian1[2][0]=x2;
            guaidian1[2][1]=y2;
//            if(guaidian1[2][0]!=0&&guaidian1[2][0]<30)
//            {
//                guaidian1[2][0]=0;
//                guaidian1[2][1]=0;
//            }
        }
        if(x1<x2 && x3>x2 && abs(y1-y2)<5 && abs(y2-y3)>3)
        {
            guaidian1[3][0]=x2;//十字上拐点
            guaidian1[3][1]=y2;
        }
        if(x1<x2 && y2>y3 && abs(x2-x3)<5 && abs(y1-y2)<2 )
        {
            guaidian1[4][0]=x2;//障碍物直角拐点
            guaidian1[4][1]=y2;
        }
        if(x1<x2 && y1>y2 && x3>x2 && abs(y3-y2)<2 )
        {
            guaidian1[5][0]=x2;//障碍物钝角拐点
            guaidian1[5][1]=y2;
        }
        if(guaidian1[4][1]!=0&&guaidian1[4][1]>guaidian1[5][1])
        {
            guaidian1[4][0]=0;
            guaidian1[4][1]=0;
        }
        if(x1>x2 && abs(y1-y2)<3 && x3>x2 && y3<y2 )
        {
            guaidian1[6][0]=x2;//障碍物锐角拐点
            guaidian1[6][1]=y2;
        }

        if(guaidian1[2][1]!=0&&guaidian1[2][1]>guaidian1[1][1])
        {
            guaidian1[1][0]=0;
            guaidian1[1][1]=0;
        }
        if(guaidian1[2][1]!=0&&guaidian1[2][1]>=guaidian1[0][1])
        {
            guaidian1[0][0]=0;
            guaidian1[0][1]=0;
        }
//        if(flag==1)
//        {
//            if(guaidian1[0][0]==0&&guaidian1[0][1]==0)
//            {
//                guaidian1[0][0]=4;
//                guaidian1[0][1]=115;
//            }
//        }
//        Image_Line[Image_Lefthand_Line[i][1]][Image_Lefthand_Line[i][0]]=1;
    }
    for(int i=0;i<Right_num-10;i++)
    {
        int X1,Y1,X2,Y2,X3,Y3=0;
        X1=Image_Righthand_Line[i][0];
        Y1=Image_Righthand_Line[i][1];
        X2=Image_Righthand_Line[i+5][0];
        Y2=Image_Righthand_Line[i+5][1];
        X3=Image_Righthand_Line[i+10][0];
        Y3=Image_Righthand_Line[i+10][1];
        if(Y2<=20)break;
        if(X1>X2 && Y1>Y2 && X3>X2 && Y3<Y2)
        {
            guaidian2[0][0]=X2;
            guaidian2[0][1]=Y2;
        }
        if(X1>X2 && Y1>Y2 && X3>X2 && Y3<Y2)
        {
            guaidian2[1][0]=X2;
            guaidian2[1][1]=Y2;
        }
        if(X1>X2 && Y1<Y2 && X3<X2 && Y3<Y2)
        {
            guaidian2[2][0]=X2;
            guaidian2[2][1]=Y2;
//            if(guaidian2[2][0]!=0&&159-guaidian2[2][0]<20)
//            {
//                guaidian2[2][0]=0;
//                guaidian2[2][1]=0;
//            }
        }
        if(X1>X2 && abs(Y1-Y2)<5 && X3<X2 && abs(Y2-Y3)>3)
        {
            guaidian2[3][0]=X2;//十字上拐点
            guaidian2[3][1]=Y2;
        }
        if(X1>X2 && Y2>Y3 && abs(X2-X3)<5 && abs(Y1-Y2)<2 )
        {
            guaidian2[4][0]=X2;//障碍物直角拐点
            guaidian2[4][1]=Y2;
        }
        if(X1>X2 && Y1>Y2 && X3<X2 && abs(Y3-Y2)<2 )
        {
            guaidian2[5][0]=X2;//障碍物钝角拐点
            guaidian2[5][1]=Y2;
        }
        if(guaidian2[4][1]!=0&&guaidian2[4][1]>guaidian2[5][1])
        {
            guaidian2[4][0]=0;
            guaidian2[4][1]=0;
        }
        if(X1<X2 && abs(Y1-Y2)<3 && X3<X2 && Y3<Y2 )
        {
            guaidian2[6][0]=X2;//障碍物锐角拐点
            guaidian2[6][1]=Y2;
        }


        if(guaidian2[2][1]!=0&&guaidian2[2][1]>guaidian2[1][1])
        {
            guaidian2[1][0]=0;
            guaidian2[1][1]=0;
        }
        if(guaidian2[2][1]!=0&&guaidian2[2][1]>=guaidian2[0][1])
        {
            guaidian2[0][0]=0;
            guaidian2[0][1]=0;
        }
//        Image_Line[Image_Righthand_Line[i][1]][Image_Righthand_Line[i][0]]=1;
    }
}
/*************************************************************************
*  函数名称：float calculateCurvature(int path[][2], int idx, int path_length)();
*  参数说明：
*  功能说明：曲率计算函数
*  函数返回：
*  修改时间：
*  备    注：
*************************************************************************/
float calculateCurvature(int path[][2], int idx, int path_length) {
    // 确保 idx 合法，避免数组越界
    if (idx < 12 || idx + 12 >= path_length) {
        return 9999.0f; // 返回 0 表示无法计算曲率
    }

    // 获取三个点的坐标
    int X1 = path[idx - 12][0];
    int Y1 = path[idx - 12][1];
    int X2 = path[idx][0];
    int Y2 = path[idx][1];
    int X3 = path[idx + 12][0];
    int Y3 = path[idx + 12][1];

    // 计算三角形ABC的面积
    float S_of_ABC = ((float)(X2 - X1) * (Y3 - Y1) - (float)(X3 - X1) * (Y2 - Y1)) / 2.0f;
    // 面积的符号表示方向

    // 计算三边长度的平方
    float AB = sqrtf((float)((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1)));
    float BC = sqrtf((float)((X3 - X2) * (X3 - X2) + (Y3 - Y2) * (Y3 - Y2)));
    float AC = sqrtf((float)((X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1)));

    // 判断是否有边长为0的情况
    if (AB == 0.0f || BC == 0.0f || AC == 0.0f) {
        return -0.1f;
    }

    // 计算曲率
    float K = (4.0f * fabsf(S_of_ABC)) / (AB * BC * AC);

    return K;
}
//
//float calculateCurvature(int path[][2], int idx) {
//    int X1 = path[idx - 15][0];
//    int Y1 = path[idx - 15][1];
//    int X2 = path[idx][0];
//    int Y2 = path[idx][1];
//    int X3 = path[idx + 15][0];
//    int Y3 = path[idx + 15][1];
//    float K;
//    int S_of_ABC = ((X2 - X1) * (Y3 - Y1) - (X3 - X1) * (Y2 - Y1)) / 2;
//    //面积的符号表示方向
//    float q1 = (float)((X2 - X1) * (X2 - X1) + (Y2 - Y1) * (Y2 - Y1));
//    float AB = sqrtf(q1);
//    q1 = (float)((X3 - X2) * (X3 - X2) + (Y3 - Y2) * (Y3 - Y2));
//    float BC = sqrtf(q1);
//    q1 = (float)((X3 - X1) * (X3 - X1) + (Y3 - Y1) * (Y3 - Y1));
//    float AC =  sqrtf(q1);
//    if (AB * BC * AC == 0)
//      {
//          K = 0;
//      }
//    else
//       K = (float)4  * S_of_ABC / (AB * BC * AC);
//   return K;
//
////    double denominator1 = (double)(x1 - x2);
////    double denominator2 = (double)(x2 - x3);
////
////    // 检查分母是否为零，防止除零错误
////    if (denominator1 == 0 || denominator2 == 0) {
////        return -0.1; // 在这种情况下曲率非常小，可以认为是直线
////    }
////
////    double k1 = 0.5 * (x1 * x1 + y1 * y1 - x2 * x2 - y2 * y2) / denominator1;
////    double k2 = 0.5 * (x2 * x2 + y2 * y2 - x3 * x3 - y3 * y3) / denominator2;
////
////    double centerX = (k1 - k2) / (y1 - y2 - y2 + y3);
////    double centerY = k1 - centerX * (y1 - y2) / denominator1;
////
////    double radius = sqrt((x1 - centerX) * (x1 - centerX) + (y1 - centerY) * (y1 - centerY));
////
////    // 检查半径是否为零，防止除零错误
////    if (radius == 0) {
////        return 9999; // 在这种情况下曲率非常大，可以认为是拐点
////    }
////
////    return 1 / radius;
//}
/*************************************************************************
*  函数名称：void blur();
*  参数说明：
*  功能说明：边线点集三角滤波
*  函数返回：
*  修改时间：
*  备    注：
*************************************************************************/
void blur(int ptsin[][2],int *num,int ptsout[][2],int he)
{
    if(he%2==1)
    {
       int half=he/2;
       for(int i=0;i<*num;i++)
       {
           ptsout[i][0]=ptsout[i][1]=0;
           for(int j= -half;j<=half;j++)
           {
               int index = i + j;
               if (index < 0) {
                   index = 0; // 将索引值限制在 0 到 num - 1 之间
               } else if (index >= *num) {
                   index = *num - 1;
               }
               ptsout[i][0] += ptsin[index][0] * (half + 1 - abs(j));
               ptsout[i][1] += ptsin[index][1] * (half + 1 - abs(j));
           }
           ptsout[i][0] /= (2 * half + 2) * (half + 1) / 2;
           ptsout[i][1] /= (2 * half + 2) * (half + 1) / 2;
       }
    }
}
/*************************************************************************
*  函数名称：void find();
*  参数说明：
*  功能说明：判断当前边线状态
*  函数返回：
*  修改时间：
*  备    注：
*************************************************************************/
int find(float currentslope)
{
    if(fabsf(currentslope)<2.2&&fabsf(currentslope)>=1.0)return 0;
    if(fabsf(currentslope)<1.0&&fabsf(currentslope)>0.2)return 1;
    if(fabsf(currentslope)<=0.2&&fabsf(currentslope)>0)return 2;
    if(fabsf(currentslope)==0)return 3;
    return 99;
}
/*************************************************************************
*  函数名称：void draw_line();
*  参数说明：
*  功能说明：补线函数，两点直线
*  函数返回：
*  修改时间：
*  备    注：
*************************************************************************/
void draw_line(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;
    float x = (float)x1, y = (float)y1;
    for (int i = 0; i <= steps; i++) {
        Image_Line[(int)y][(int)x] = 1;
        x += xIncrement;
        y += yIncrement;
    }
}
int Calc_ERROR(void)
{
    int Err=0;
    if(zhangaiflag==0)
    {
        for(int i=0;i<5;i++)
        {
            int error=75-Image_centralLine[Look_Line-i-Image_upLine];
            image_error[i]=error;
            Err+=error;
        }
        Err = Err/5;
        image_error[5]=Err;
    }
    if(zhangaiflag==1)
    {
        Err=75-Image_centralLine[tmperr+5-Image_upLine];
    }
    if(zhangaiflag==2)
    {
        Err=75-Image_centralLine[tmperr+5-Image_upLine];
    }
    //常规误差
    return Err;
}
/*************************************************************************
*  函数名称：void qulv();
*  参数说明：
*  功能说明：曲率计算
*  函数返回：
*  修改时间：
*  备    注：
*************************************************************************/
void qulv()
{
    qulv_value=0;
    qulv_jin=0;
    qulv_yuan=0;
    qvlv_quanju_right=0;
    qvlv_quanju_left=0;
    qvlv_jin_left=0;
    qvlv_jin_right=0;
    qvlv_yuan_left=0;
    qvlv_yuan_right=0;
    for(int i=central_length;i>0;i--)
    {
        if((Image_centralLine[i]-Image_centralLine[i-1])>0)
         {
           if(i>45)
           {
               qvlv_jin_right++;
           }
           if(i<50)
           {
               qvlv_yuan_right++;
           }
           qvlv_quanju_right++;
         }
        else if((Image_centralLine[i]-Image_centralLine[i-1])<0)
         {
            if(i>45)
            {
                qvlv_jin_left++;
            }
            if(i<50)
            {
                qvlv_yuan_left++;
            }
           qvlv_quanju_left++;
         }
    }
    qulv_value=qvlv_quanju_right-qvlv_quanju_left;
    qulv_jin=qvlv_jin_right-qvlv_jin_left;
    qulv_yuan=qvlv_yuan_right-qvlv_yuan_left;
//    if(qvlv_quanju_right>15&&qvlv_quanju_left>15&&qulv_value<10&&current_state1!=20&&current_state2!=3)
//    {
//        Beep_on;
//    }
}
