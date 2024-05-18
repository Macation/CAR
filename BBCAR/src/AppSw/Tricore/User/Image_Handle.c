/*
 * Image_Handle.c
 *  功能：摄像头常规巡线
 *  Created on: 2023年12月10日
 *      Author: dduu
 */
#include "image_Handle.h"

extern unsigned char Bin_Image[LCDH][LCDW];

extern volatile uint8 CrossWay_Flag;

typedef struct{
        uint8 X; //横坐标0~119
        uint8 Y; //纵坐标0~159
}Point;

Point A;
Point B;
Point C;
Point D;

uint8 ZUO[120];
uint8 YOU[120];
uint8 ZHONGJIAN[120];

uint8 Lost_Left_Count=0;
uint8 Lost_Right_Count=0;
uint8 Lost_Left_Loc=50;
uint8 Lost_Right_Loc=50;

//求车身偏差
uint8 QIANZHAN=15;
uint8 YUAN,ZHONG,JIN;

uint8 Servo_QZ_ERR=0; //前瞻误差
int Servo_Loc_error=0; //车身横向误差

uint8 StraightWay_flag=0; //0为弯道，1为直道

uint8 ImageSide_LR[2][100];  //左右边线
uint8 ImageSide_UD[2][160];  //上下边线

int ABS(int A)
{
    if(A<=0)
    {
       A=-A;
    }
    return A;
}

uint8 min(uint8 A,uint8 B)
{
    return (A>=B)?B:A;
}

void GetUpDownSide(void)   //当前设置长为100，宽为160
{
    uint8 last = 50;
    uint8 i = 0, j = 0;
    //初始化,将上下边线初始化为边界值
    for(uint8 i=0;i<160;i++)
    {
        ImageSide_UD[0][i]=0;
        ImageSide_UD[1][i]=99;
    }

    /* 用中线比较近的行 判断是否丢线 */
    for(i = last; i > 0; i--)
    {
        if(Bin_Image[i][80]==0)  //找到黑点
        {
            ImageSide_UD[0][80] = i;
            break;
        }
    }

    for(i = last; i < 100; i++)
    {
        if(Bin_Image[i][80]==0)
        {
            ImageSide_UD[1][80] = i;
            break;
        }
    }

    for(i=80-1;i>0;i--)  //搜索左半边线
    {
        for(j=ImageSide_UD[0][i+1]+10;(j>0 && j<100);j--)   //搜索左上边线
        {
            if(Bin_Image[j][i]==0)
            {
                ImageSide_UD[0][i]=j;
                break;
            }
        }
        for(j=ImageSide_UD[1][i+1]-10;(j>0 && j<100);j++)  //搜索左下边线
        {
            if(Bin_Image[j][i]==0)
            {
                ImageSide_UD[1][i]=j;
                break;
            }
        }
    }

    for(i=80+1;i<160;i++)  //搜索右半边线
    {
        for(j=ImageSide_UD[0][i-1]+10;(j>0 && j<100);j--)   //搜索右上边线
        {
            if(Bin_Image[j][i]==0)
            {
                ImageSide_UD[0][i]=j;
                break;
            }
        }
        for(j=ImageSide_UD[1][i+1]-10;(j>0 && j<100);j++)  //搜索右下边线
        {
            if(Bin_Image[j][i]==0)
            {
                ImageSide_UD[1][i]=j;
                break;
            }
        }
    }

}

//在for循环的时候，如果设置变量类型为uint8，即0~255，0在-的话为255，如果循环条件为>0，那么循环无法结束,因此在边界的时候边界数组为初始值0/159
void GetImageSide(void)  //舍弃掉下边20行，剩余100行搜左右边线BinImage[100][160]
{
    //          找中线
   //初始化,将左右中线初始化为边界值
   for(uint8 i=0;i<100;i++)
   {
       ImageSide_LR[0][i]=0;
       ImageSide_LR[1][i]=159;
   }
   //摄像头在屏幕上的显示为下边为99，从下往上索引逐渐递减
   for(uint8 i=99;i>0;i--)
   {
       //从中间向左边进行巡线
       for(uint8 j=80;j>=1;j--)
       {
           if((Bin_Image[i][j-1]==0) && (Bin_Image[i][j]==0) && (Bin_Image[i][j+1]==1)) //
           {
               ImageSide_LR[0][i] = j ;
               break;
           }
       }
       //从中间向右边进行巡线
       for(uint8 j=80;j<=158;j++)
       {
           if((Bin_Image[i][j-1]==1) && (Bin_Image[i][j]==0) && (Bin_Image[i][j+1]==0))
           {
               ImageSide_LR[1][i] = j;
               break;
           }
       }
   }
}

void Get_ABCD(void)  //获取特殊点ABCD从右往左巡线根据线的连续性
{
    A.X=100;
    A.Y=2;
    B.X=100;
    B.Y=157;  //初始化
    for(uint8 i=80;i>0;i--)
    {
        if(Bin_Image[A.X][i]==0)
        {
            A.Y=i;
            break;
        }
    }
    for(uint8 i=80;i<159;i++)
    {
        if(Bin_Image[B.X][i]==0)
        {
            B.Y=i;
            break;
        }
    }

    C.X=A.X-1;
    C.Y=A.Y;
    for(uint8 i=C.X;i>1;i--)
    {
        C.X=i;
        for(uint8 j=C.Y-1;j<80;j++)
        {
            if(Bin_Image[i][j]==1)  //从侧面向右找白点
            {
                C.Y=j-1;  //变为黑点
                break;
            }
        }
        if(Bin_Image[i-1][C.Y]==1 && Bin_Image[i-2][C.Y]==1)  //说明找到拐点C
        {
            C.X=i;
            break;
        }
    }
    D.X=B.X-1;
    D.Y=B.Y;
    for(uint8 i=D.X;i>1;i--)
    {
        D.X=i;
        for(uint8 j=D.Y+1;j>80;j--)
        {
            if(Bin_Image[i][j]==1)  //从侧面向右找白点
            {
                D.Y=j+1;  //变为黑点
                break;
            }
        }
        if(Bin_Image[i-1][D.Y]==1 && Bin_Image[i-2][D.Y]==1)  //说明找到拐点D
        {
            D.X=i;
            break;
        }
    }

    //初级判断十字路口
    if(ABS(C.X-D.X)<10 && C.X>30 && D.X>30)
    {
        uint8 X=min(C.X,D.X)-1;   //上一行
        uint8 black=0;
        for(uint8 i=X;i>X-10;i--)
        {
            for(uint8 j=20;j<80;j+=5)
            {
                if(Bin_Image[i][j]==0)
                {
                    black++;
                }
            }
        }
        if(black<10)  //判断出十字路口，进行补线
        {
            CrossWay_Flag=1;
        }
    }

    if(CrossWay_Flag)
    {
        float K;
        K=(float)(A.X-C.X)/(C.Y-A.Y);
        for(uint8 i=C.X-1;i>C.X-20;i--)
        {
            Bin_Image[i][(uint8)((C.X-i)/K+C.Y)]=0;
            Bin_Image[i][(uint8)((C.X-i)/K+C.Y+1)]=0;
        }
        K=(float)(B.X-D.X)/(B.Y-D.Y);
        for(uint8 i=D.X-1;i>D.X-20;i--)
        {
            Bin_Image[i][(uint8)((i-D.X)/K+D.Y)]=0;
            Bin_Image[i][(uint8)((i-D.X)/K+D.Y-1)]=0;
        }
        if(ABS(C.X-D.X)>10 || C.X<=30 || D.X<=30)
        {
            CrossWay_Flag=0;
        }
    }
}


//黑为0，白为1
void image_handle(void)
{
    //          找中线
    //初始化,将左右中线初始化为边界值
    for(uint8 i=0;i<120;i++)
    {
        ZHONGJIAN[i]=79;
        ZUO[i]=0;
        YOU[i]=159;
    }
    //摄像头在屏幕上的显示为下边为99，从下往上索引逐渐递减
    for(uint8 i=119;i>0;i--)
    {
        //从中间向左边进行巡线
        for(uint8 j=ZHONGJIAN[i];j>=1;j--)
        {
            if((Bin_Image[i][j-1]==0) && (Bin_Image[i][j]==0) && (Bin_Image[i][j+1]==1)) //
            {
                ZUO[i] =j ;
                break;
            }
        }
        //从中间向右边进行巡线
        for(uint8 j=ZHONGJIAN[i];j<=158;j++)
        {
            if((Bin_Image[i][j-1]==1) && (Bin_Image[i][j]==0) && (Bin_Image[i][j+1]==0))
            {
                YOU[i] = j;
                break;
            }
        }

        ZHONGJIAN[i] = (ZUO[i] + YOU[i]) /2;

        //将搜索到的中线用黑线涂黑
        Bin_Image[i][ZHONGJIAN[i]]=0;
    }
    //前瞻  求车身偏差
    JIN=ZHONGJIAN[110];
    ZHONG=ZHONGJIAN[110-QIANZHAN];
    YUAN=ZHONGJIAN[110-QIANZHAN*2];

    if(JIN<ZHONG && ZHONG<YUAN)
    {
        Servo_QZ_ERR=((YUAN-ZHONG)+(ZHONG-JIN))/2;
    }
    else if(JIN<ZHONG && ZHONG>YUAN)
    {
        Servo_QZ_ERR=(ZHONG-JIN)/2;
    }
    else if(JIN>ZHONG && ZHONG>YUAN)
    {
        Servo_QZ_ERR=((JIN-ZHONG)+(ZHONG-YUAN))/2;
    }
    else if(JIN>ZHONG && ZHONG<YUAN)
    {
        Servo_QZ_ERR=(JIN-ZHONG)/2;
    }

    Servo_Loc_error=80-ZHONGJIAN[100];  //横向误差

}

void Lost_Line_Left(uint8* count,uint8* loc)  //计算左边线丢线行数
{
    uint8 flag=0;
    *count=0;
    *loc=50;  //设置默认值
    for(uint8 i=LCDH-1-5;i>0;i--) //舍弃掉下面5行可能由于摄像头广角导致的丢线现象
    {
        if(ImageSide_LR[0][i]==0)
        {
            (*count)++;
            if((ImageSide_LR[0][i+1]-ImageSide_LR[0][i]>25) && flag==0)  //记录下左边线数组第一次发生突变的位置
            {
                *loc=i;
                flag=1;
            }
        }
    }
}

void Lost_Line_Right(uint8* count,uint8* loc)  //计算右边线丢线行数
{
    uint8 flag=0;
    *count=0;
    *loc=50;  //设置默认值
    for(uint8 i=LCDH-1-5;i>0;i--) //舍弃掉下面5行可能由于摄像头广角导致的丢线现象
    {
        if(ImageSide_LR[1][i]==159)
        {
            (*count)++;
            if((ImageSide_LR[1][i]-ImageSide_LR[1][i+1]>25) && flag==0)  //记录下右边线数组第一次发生突变的位置
            {
                *loc=i;
                flag=1;
            }
        }
    }
}


void Judge_CrossRoad(void)
{
    Lost_Line_Left(&Lost_Left_Count,&Lost_Left_Loc);
    Lost_Line_Right(&Lost_Right_Count,&Lost_Right_Loc);

    if(Lost_Left_Count>=20 && Lost_Right_Count>=20)            //左右丢线较多，初步判断十字路口
    {
        uint8 X=min(Lost_Left_Loc,Lost_Right_Loc);   //上一行
        uint8 black=0;
        for(uint8 i=X;i>X-10;i--)
        {
            for(uint8 j=20;j<140;j+=5)
            {
                if(Bin_Image[i][j]==0)
                {
                    black++;
                }
            }
        }
        if(black<15)  //最终判断出十字路口，进行补线
        {
            CrossWay_Flag=1;
        }
    }
}



uint8 UpSide[160];


Point Up_Left;     //左上拐点
Point Up_Right;    //右上拐点
Point Down_Left;   //左下拐点
Point Down_Right;  //右下拐点

void Camera_Handle(void)
{
    //上拐点  从左往右向下找突变
    for(uint8 i=0;i<160;i++)
    {
        for(uint8 j=0;j<120;j++)
        {
            if(Bin_Image[j][i]==0 || j==119)  // ?
            {
                UpSide[i]=j;
            }
        }
    }

    for(uint8 i=80;i>0;i--)
    {
        if(UpSide[i-1]<UpSide[i] && UpSide[i]>UpSide[i+1])
        {
            Up_Left.X=UpSide[i];
            Up_Left.Y=i;
        }
    }

    for(uint8 i=80;i<159;i++)
    {
        if(UpSide[i-1]<UpSide[i] && UpSide[i]>UpSide[i+1])
        {
            Up_Right.X=UpSide[i];
            Up_Right.Y=i;
        }
    }

    //下拐点  左右边界数组找突变
    for(uint8 i=118;i>0;i--)
    {
        if(ZUO[i-1]<ZUO[i] && ZUO[i]>ZUO[i+1])
        {
            Down_Left.X=i;
            Down_Left.Y=ZUO[i];
            break;
        }
    }
    for(uint8 i=118;i>0;i--)
    {
        if(YOU[i+1]>YOU[i] && YOU[i]<YOU[i-1])
        {
            Down_Right.X=i;
            Down_Right.Y=YOU[i];
            break;
        }
    }
}




