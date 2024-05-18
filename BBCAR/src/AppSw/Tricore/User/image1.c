/*
 * newimage.c
 *
 *  Created on: 2024年5月14日
 *      Author: Macation
 */
#include "image1.h"
#define Image_downLine 115
#define Image_upLine 15
#define central_length Image_downLine-Image_upLine
int Image_centralLine[central_length];//存放边线的数组
#define Array_Length MT9V034_IMAGEH/2+MT9V034_IMAGEW/2 //边线数组的长度 154
int Image_Lefthand_Line[Array_Length][2];//存放提取后的边线数组的坐标
int Image_Righthand_Line[Array_Length][2];
unsigned char Image_Line[120][160];//存放边线的数组，0，1表示
int Left_num=Array_Length;
int Right_num=Array_Length;
int block_size=7;
int error1=0;
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
unsigned char show[120][160];
void showing(void)
{
    short i = 0, j = 0, row = 0, line = 0;
    for (i = 0; i < LCDH; i ++)          //神眼高 120 / 2  = 60，
    // for (i = 0; i < IMAGEH; i += 3)       //OV7725高 240 / 3  = 80，
    {
        for (j = 0; j <= LCDW; j ++)     //神眼宽188 / 2  = 94，
        // for (j = 0; j <= IMAGEW; j += 3)  //OV7725宽320 / 3  = 106，
        {
            show[row][line] = Image_Data[i][j+17];
            line++;
        }
        line = 0;
        row++;
    }
}
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
        show[i][3]=0;
        show[i][4]=0;
        show[i][LCDW-4]=0;
        show[i][LCDW-3]=0;
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
                local_thres += show [Y+dy][X+dx];//方块内阈值累加
            }
        }
        local_thres /= block_size * block_size;//阈值取平均
        local_thres -= clip_value;//阈值做裁剪，可以自己调
        int front_value = show[Y+dir_front[dir][1]][X+dir_front[dir][0]];//当前像素前方的点的像素值；
        int frontleft_value = show[Y+dir_frontleft[dir][1]][X+dir_frontleft[dir][0]];//当前像素左前方的点的像素值
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
    while(step<num&& half<X && X<LCDW-half-1 && half< Y && Y <LCDH-1 && turn<4)//循环终止条件，不越界且转向次数小于4，防止原地打转)
    {
        int local_thres = 0; //自适应阈值
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += show [Y+dy][X+dx];//方块内阈值累加
            }
        }
        local_thres /= block_size * block_size;//阈值取平均
        local_thres -= clip_value;//阈值做裁剪，可以自己调
        int front_value = show[Y+dir_front[dir][1]][X+dir_front[dir][0]];//当前像素前方的点的像素值；
        int frontright_value=show[Y+dir_frontright[dir][1]][X+dir_frontright[dir][0]];
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

    int threshold=0;
    threshold=Image_Threshold_Line(115);
    int X1=LCDW/2;
    int Y1=Image_downLine;
    int X2=X1;
    int Y2=Y1;
    memset(Image_Line,0,sizeof(Image_Line));
    memset(Image_Lefthand_Line,0,sizeof(Image_Lefthand_Line));
    memset(Image_Righthand_Line,0,sizeof(Image_Righthand_Line));
    for(;X1>0;X1--)//寻找跳变点
    {
        if(show[Y1][X1-1]<threshold)break;
    }
    Left_num=Array_Length;
    Left_num=Image_Findline_Lefthand(X1,Y1,Left_num,block_size,0);
    for(;X2<LCDW-1;X2++)//寻找跳变点
    {
        if(show[Y2][X2+1]<threshold)break;
    }
    Right_num=Array_Length;
    Right_num=Image_Findline_Righthand(X2,Y2,Right_num,block_size,0);
    Image_seedCentralline(X1,X2);//画中线
    error1=80-Image_centralLine[90-15];//计算误差
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
        //规划下一次的起点
        LeftStart=Left[i-Image_upLine]+10;
        RightStart=Right[i-Image_upLine]-10;
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


