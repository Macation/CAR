/*
 * My_Image_Handle.c
 *
 *  Created on: 2024年3月4日
 *      Author: dduu
 */
#include "My_Image_Handle.h"

#define QianZhan 15

char data_txt[30];

extern unsigned char Bin_Image[LCDH][LCDW];
extern int Servo_Loc_error;

volatile uint8 Straight_Flag;        //直道标志
volatile uint8 Curve_Flag;           //弯道标志
volatile uint8 CrossWay_Flag;          //十字路口标志
volatile uint8 Island_Flag;            //环岛标志
volatile int White_Column[LCDW];//每列白列长度
volatile int Longest_White_Column_Left[2];  //存储左边最长白列的数量和位置
volatile int Longest_White_Column_Right[2]; //存储右边最长白列的数量和位置
volatile int Search_Stop_Line;          //搜索截止列，最长白列所在行数
volatile int Road_Wide[LCDH];           //赛道宽度
volatile int Left_Line[LCDH];           //左边界
volatile int Right_Line[LCDH];          //右边界
volatile int Middle_Line[LCDH];         //中边界
volatile int Left_Lost_Flag[LCDH];      //左丢线数组，0为不丢线，1为丢线
volatile int Right_Lost_Flag[LCDH];     //右丢线数组，0为不丢线，1为丢线
volatile uint8 Right_Island_Flag=0;     //右环岛标志
volatile uint8 Left_Island_Flag=0;      //左环岛标志
volatile uint8 Island_State=0;          //环岛状态数
volatile int Right_Lost_Time=0;        //右丢线行数
volatile int Left_Lost_Time=0;         //左丢线行数
volatile int Boundry_Start_Left=0;     //左边第一个非丢线点
volatile int Boundry_Start_Right=0;    //右边第一个非丢线点
volatile int Both_Lost_Time=0;         //两边同时丢线数
volatile int Right_Down_Find;          //右下拐点行数
volatile int Left_Down_Find;           //左下拐点行数
volatile int Left_Up_Find;             //左上拐点行数
volatile int Right_Up_Find;            //右上拐点行数
volatile int Mon_change_Right_Line;     //右单调变化点
volatile int monotonicity_change_line[2];  //单调变化坐标点
volatile int Right_Up_Guai[2];
volatile int island_state_3_up[2];
volatile float k;
//75 20 38

//双最长白列巡线,寻找图像左右边界（采取的是最长白列左右寻找边界的方法)
void Longest_White_Column(void)
{
    int i,j;
    int start_column=20;//最长白列的搜索区间
    int end_column=LCDW-20;
    //相关数据使用前清零
    Longest_White_Column_Left[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Left[1] = 20;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Right[0] = 0;//最长白列,[0]是最长白列的长度，[1】是第某列
    Longest_White_Column_Right[1] = 140;//最长白列,[0]是最长白列的长度，[1】是第某列

    Right_Lost_Time = 0;        //边界丢线数
    Left_Lost_Time  = 0;
    Boundry_Start_Left  = 0;    //第一个非丢线点,常规边界起始点
    Boundry_Start_Right = 0;
    Both_Lost_Time = 0;//两边同时丢线数

    for (i = 0; i <=LCDH-1; i++)//数据清零
    {
        Right_Lost_Flag[i] = 0;
        Left_Lost_Flag[i] = 0;
        Left_Line[i] = 0;
        Right_Line[i] = LCDW-1;
    }
    for(i=0;i<=LCDW-1;i++)
    {
        White_Column[i] = 0;
    }
//
////    //环岛需要对最长白列范围进行限定
////    //环岛3状态需要改变最长白列寻找范围
////    if(Right_Island_Flag==1)//右环
////    {
////        if(Island_State==3)
////        {
////            start_column=40;
////            end_column=LCDW-20;
////        }
////    }
////    else if(Left_Island_Flag==1)//左环
////    {
////        if(Island_State==3)
////        {
////            start_column=20;
////            end_column=LCDW-40;
////        }
////    }
//
//    //获取每列的白色点数
    for(i=start_column;i<=end_column;i++)
    {
        for(j=LCDH-1;j>=0;j--)
        {
            if(Bin_Image[j][i]==0)
            {
                break;
            }
            else
            {
                White_Column[i]++;
            }
        }
    }
    //从左到右找左边最长白列
    Longest_White_Column_Left[0] = 0;
    for(i=start_column;i<=end_column;i++)
    {
        if (Longest_White_Column_Left[0] < White_Column[i])//找最长的那一列
        {
            Longest_White_Column_Left[0] = White_Column[i];//【0】是白列长度
            Longest_White_Column_Left[1] = i;              //【1】是下标，第j列
        }
    }
//    //从右到左找右左边最长白列
    Longest_White_Column_Right[0] = 0;      //【0】是白列长度
    for(i=end_column;i>=start_column;i--)//从右往左，注意条件，找到左边最长白列位置就可以停了
    {
        if (Longest_White_Column_Right[0] < White_Column[i])//找最长的那一列
        {
            Longest_White_Column_Right[0] = White_Column[i];//【0】是白列长度
            Longest_White_Column_Right[1] = i;              //【1】是下标，第j列
        }
    }

    Search_Stop_Line = Longest_White_Column_Left[0];//搜索截止列选取左或者右区别不大，他们两个理论上是一样的
//
//    //异常检测下面for循环Search_Stop_Line-1的目的是防止上述在最低行就检测到零行导致程序死循环，
//    //如果Search_Stop_Line=0的话会卡死，需要-1(图像全黑时)
//    //bug:在做下面一行的时候就检测到零行
    for (i = LCDH - 1; i >= LCDH-Search_Stop_Line-1; i--)//常规巡线
    {
        //bug
        for (j = Longest_White_Column_Left[1]; j >= 0 + 2; j--)//往左边扫描
        {//从最下面一行，访问到我的有效是视野行
            if (Bin_Image[i][j] ==1 && Bin_Image[i][j - 1] == 0 && Bin_Image[i][j - 2] == 0)//黑黑白认为到达左边界
            {
                Left_Line[i] = j;
                Left_Lost_Flag[i] = 0; //左丢线数组，丢线置1，不丢线置0
                break;
            }
            else if(j<=0+2)
            {
                Left_Line[i] = j;//找到头都没找到边，就把屏幕最左右当做边界
                Left_Lost_Flag[i] = 1; //左丢线数组，丢线置1，不丢线置0
                break;
            }
        }
        //从最下面一行，访问到我的有效是视野行
        for (j = Longest_White_Column_Right[1]; j <= LCDW-1-2; j++)
        {
            if (Bin_Image[i][j] ==1 && Bin_Image[i][j + 1] == 0 && Bin_Image[i][j + 2] == 0)//白黑黑，找到右边界
            {
                Right_Line[i] = j;
                Right_Lost_Flag[i] = 0; //右丢线数组，丢线置1，不丢线置0
                break;
             }
             else if(j>=LCDW-1-2)//没找到右边界，把屏幕最右赋值给右边界
             {
                 Right_Line[i] = j;
                 Right_Lost_Flag[i] = 1; //右丢线数组，丢线置1，不丢线置0
                 break;
             }
         }
     }

    for (i = LCDH - 1; i >= 0; i--)//赛道数据初步分析
    {
        if (Left_Lost_Flag[i] == 1)//单边丢线数
            Left_Lost_Time++;
        if (Right_Lost_Flag[i] == 1)
            Right_Lost_Time++;
        if (Left_Lost_Flag[i] == 1 && Right_Lost_Flag[i] == 1)//双边丢线数
            Both_Lost_Time++;
        if (Boundry_Start_Left ==  0 && Left_Lost_Flag[i]  != 1)//记录第一个非丢线点，边界起始点
            Boundry_Start_Left = i;
        if (Boundry_Start_Right == 0 && Right_Lost_Flag[i] != 1)
            Boundry_Start_Right = i;
        Road_Wide[i]=Right_Line[i]-Left_Line[i];
    }

////    //环岛3状态改变边界，看情况而定，我认为理论上的最优情况是不需要这些处理的
////    if(Island_State==3||Island_State==4)
////    {
////        if(Right_Island_Flag==1)//右环
////        {
////            for (i = LCDH - 1; i >= 0; i--)//右边直接写在边上
////            {
////                Right_Line[i]=LCDW-1;
////            }
////        }
////        else if(Left_Island_Flag==1)//左环
////        {
////            for (i = LCDH - 1; i >= 0; i--)//左边直接写在边上
////            {
////                Left_Line[i]=0;      //右边线线数组
////            }
////        }
////    }

}

//计算实际中线与理论中线的误差，用于舵机打角
float Err_Sum(void)
{
    int i;
    float Err=0;
    //常规误差
     for(i=95;i<=105;i++)
     {
         Err+=(LCDW/2-((Left_Line[i]+Right_Line[i])/2));//右移1位，等效除2
     }
     Err = Err/10.0;
    return Err;
}

float Cal_Err(void)
{
    //前瞻  求车身偏差
    float Err=0;
    int Near=Middle_Line[110];
    int Mid=Middle_Line[110-QianZhan];
    int Far=Middle_Line[110-QianZhan*2];

    if(Near<Mid && Mid<Far)
    {
        Err=(Near+Mid+Far)/3.0;
        Err=(LCDW/2-Err);
    }
    else if(Near<Mid && Mid>Far)
    {
        Err=(Near+Mid)/2.0;
        Err=(LCDW/2-Err);
    }
    else if(Near>Mid && Mid>Far)
    {
        Err=(Near+Mid+Far)/3.0;
        Err=(LCDW/2-Err);
    }
    else if(Near>Mid && Mid<Far)
    {
        Err=(Near+Mid)/2.0;
        Err=(LCDW/2-Err);
    }
    else
    {
        Err=(Near+Mid+Far)/3.0;
        Err=(LCDW/2-Err);
    }

    return Err;
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     直道检测
  @param     null
  @return    null
  Sample     Straight_Detect()；
  @note      利用最长白列，边界起始点，中线起始点，
-------------------------------------------------------------------------------------------------------------------*/
//void Straight_Detect(void)
//{
//    Straight_Flag=0;
//    if(Search_Stop_Line>=65)//截止行很远
//    {
//        if(Boundry_Start_Left>=68 && Boundry_Start_Right>=65)//起始点靠下
//        {
//            if(-5<=Servo_Loc_error && Servo_Loc_error<=5)//误差很小
//            {
//                Straight_Flag=1;//认为是直道
//            }
//        }
//    }
//    else
//        Straight_Flag=0;
//}

//直道检测
void Straight_Detect(void)
{


}

//弯道检测
void Curve_Detect(void)
{
    Curve_Flag=0;
    if(Search_Stop_Line>=65)//截止行很远
    {
        if(Boundry_Start_Left>=68 && Boundry_Start_Right>=65)//起始点靠下
        {
            if(-5<=Servo_Loc_error && Servo_Loc_error<=5)//误差很小
            {
                Curve_Flag=1;//认为是弯道
            }
        }
    }
    else
        Curve_Flag=0;
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     找下面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Right_Down_Find=0;
             Left_Down_Find=0;
  Sample     Find_Down_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Down_Point(int start,int end)
{
    int i,t;
    Right_Down_Find=0;
    Left_Down_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=LCDH-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=LCDH-1-5;
    if(end<=LCDH-Search_Stop_Line)
        end=LCDH-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(Left_Down_Find==0&&//只找第一个符合条件的点
            ABS(Left_Line[i]-Left_Line[i+1])<=5&&//角点的阈值可以更改
            ABS(Left_Line[i+1]-Left_Line[i+2])<=5&&
            ABS(Left_Line[i+2]-Left_Line[i+3])<=5&&
              (Left_Line[i]-Left_Line[i-2])>=8&&
              (Left_Line[i]-Left_Line[i-3])>=15&&
              (Left_Line[i]-Left_Line[i-4])>=15)
        {
            Left_Down_Find=i;//获取行数即可
        }
        if(Right_Down_Find==0&&//只找第一个符合条件的点
           ABS(Right_Line[i]-Right_Line[i+1])<=5&&//角点的阈值可以更改
           ABS(Right_Line[i+1]-Right_Line[i+2])<=5&&
           ABS(Right_Line[i+2]-Right_Line[i+3])<=5&&
              (Right_Line[i]-Right_Line[i-2])<=-8&&
              (Right_Line[i]-Right_Line[i-3])<=-15&&
              (Right_Line[i]-Right_Line[i-4])<=-15)
        {
            Right_Down_Find=i;
        }
        if(Left_Down_Find!=0&&Right_Down_Find!=0)//两个找到就退出
        {
            break;
        }
    }
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     找上面的两个拐点，供十字使用
  @param     搜索的范围起点，终点
  @return    修改两个全局变量
             Left_Up_Find=0;
             Right_Up_Find=0;
  Sample     Find_Up_Point(int start,int end)
  @note      运行完之后查看对应的变量，注意，没找到时对应变量将是0
-------------------------------------------------------------------------------------------------------------------*/
void Find_Up_Point(int start,int end)
{
    int i,t;
    Left_Up_Find=0;
    Right_Up_Find=0;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(end<=LCDH-Search_Stop_Line)
        end=LCDH-Search_Stop_Line;
    if(end<=5)//及时最长白列非常长，也要舍弃部分点，防止数组越界
        end=5;
    if(start>=LCDH-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=LCDH-1-5;
    for(i=start;i>=end;i--)
    {
        if(Left_Up_Find==0&&//只找第一个符合条件的点
            ABS(Left_Line[i]-Left_Line[i-1])<=5&&
            ABS(Left_Line[i-1]-Left_Line[i-2])<=5&&
            ABS(Left_Line[i-2]-Left_Line[i-3])<=5&&
              (Left_Line[i]-Left_Line[i+2])>=8&&
              (Left_Line[i]-Left_Line[i+3])>=15&&
              (Left_Line[i]-Left_Line[i+4])>=15)
        {
            Left_Up_Find=i;//获取行数即可
        }
        if(Right_Up_Find==0&&//只找第一个符合条件的点
            ABS(Right_Line[i]-Right_Line[i-1])<=5&&//下面两行位置差不多
            ABS(Right_Line[i-1]-Right_Line[i-2])<=5&&
            ABS(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            Right_Up_Find=i;//获取行数即可
        }
        if(Left_Up_Find!=0&&Right_Up_Find!=0)//下面两个找到就出去
        {
            break;
        }
    }
    if(abs(Right_Up_Find-Left_Up_Find)>=30)//纵向撕裂过大，视为误判
    {
        Right_Up_Find=0;
        Left_Up_Find=0;
    }
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     左补线
  @param     补线的起点，终点
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的,不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Left_Add_Line(int x1,int y1,int x2,int y2)     //左补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=LCDW-1)//起始点位置校正，排除数组越界的可能
       x1=LCDW-1;
    else if(x1<=0)
       x1=0;
    if(y1>=LCDH-1)
       y1=LCDH-1;
    else if(y1<=0)
       y1=0;
    if(x2>=LCDW-1)
       x2=LCDW-1;
    else if(x2<=0)
       x2=0;
    if(y2>=LCDH-1)
       y2=LCDH-1;
    else if(y2<=0)
       y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    //这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
    //建议进行判断，根据a1和a2的大小关系，决定++或者--访问
    //这里修改各位自行操作
    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=LCDW)
            hx=LCDW;
        else if(hx<=0)
            hx=0;
        Left_Line[i]=hx;
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右补线
  @param     补线的起点，终点
  @return    null
  Sample     Left_Add_Line(int x1,int y1,int x2,int y2);
  @note      补的直接是边界，点最好是可信度高的,不要乱补
-------------------------------------------------------------------------------------------------------------------*/
void Right_Add_Line(int x1,int y1,int x2,int y2)     //右补线,补的是边界
{
    int i,max,a1,a2;
    int hx;
    if(x1>=LCDW-1)//起始点位置校正，排除数组越界的可能
       x1=LCDW-1;
    else if(x1<=0)
       x1=0;
    if(y1>=LCDH-1)
       y1=LCDH-1;
    else if(y1<=0)
       y1=0;
    if(x2>=LCDW-1)
       x2=LCDW-1;
    else if(x2<=0)
       x2=0;
    if(y2>=LCDH-1)
       y2=LCDH-1;
    else if(y2<=0)
       y2=0;
    a1=y1;
    a2=y2;
    if(a1>a2)//坐标互换
    {
        max=a1;
        a1=a2;
        a2=max;
    }
    //这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
    //建议进行判断，根据a1和a2的大小关系，决定++或者--访问
    //这里修改各位自行操作
    for(i=a1;i<=a2;i++)//根据斜率补线即可
    {
        hx=(i-y1)*(x2-x1)/(y2-y1)+x1;
        if(hx>=LCDW)
            hx=LCDW;
        else if(hx<=0)
            hx=0;
        Right_Line[i]=hx;
    }
}

int count=0;

void Cross_Detect(void)
{
    count++;
    int down_search_start=0;//下角点搜索开始行
    CrossWay_Flag=0;
    if(Island_Flag==0)//与环岛互斥开
    {
        Left_Up_Find=0;
        Right_Up_Find=0;
        if(Both_Lost_Time>=10)//十字必定有双边丢线，在有双边丢线的情况下再开始找角点
        {
            Find_Up_Point( LCDH-1, 0 );
            if(Left_Up_Find==0 && Right_Up_Find==0)//只要没有同时找到两个上点，直接结束
            {
                return;
            }
        }
        if(Left_Up_Find!=0 && Right_Up_Find!=0)//找到两个上点，就认为找到十字了
        {
            CrossWay_Flag=1;//确定对应标志位，便于各元素互斥掉
            down_search_start=Left_Up_Find>Right_Up_Find?Left_Up_Find:Right_Up_Find;//用两个上拐点坐标靠下者作为下点的搜索上限
            Find_Down_Point(LCDH-5,down_search_start+2);//在上拐点下2行作为下角点的截止行
            if(Left_Down_Find<=Left_Up_Find)
            {
                Left_Down_Find=0;//下点不可能比上点还靠上
            }
            if(Right_Down_Find<=Right_Up_Find)
            {
                Right_Down_Find=0;//下点不可能比上点还靠上
            }
            if(Left_Down_Find!=0&&Right_Down_Find!=0)
            {//四个点都在，无脑连线，这种情况显然很少
                Left_Add_Line (Left_Line[Left_Up_Find],Left_Up_Find ,Left_Line[Left_Down_Find] ,Left_Down_Find);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find==0&&Right_Down_Find!=0)//11//这里使用的是斜率补线
            {//三个点                                     //01
                Lengthen_Left_Boundry(Left_Up_Find-1,LCDH-1);
                Right_Add_Line(Right_Line[Right_Up_Find],Right_Up_Find,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else if(Left_Down_Find!=0&&Right_Down_Find==0)//11
            {//三个点                                      //10
                Left_Add_Line (Left_Line [Left_Up_Find ],Left_Up_Find ,Left_Line [Left_Down_Find ] ,Left_Down_Find);
                Lengthen_Right_Boundry(Right_Up_Find-1,LCDH-1);
            }
            else if(Left_Down_Find==0 && Right_Down_Find==0)//11
            {//就俩上点                                    //00
                Lengthen_Left_Boundry (Left_Up_Find-1,LCDH-1);
                Lengthen_Right_Boundry(Right_Up_Find-1,LCDH-1);
            }
        }
        else
        {
            CrossWay_Flag=0;
        }
    }

}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     右左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Lengthen_Right_Boundry(int start,int end)；
  @note      从起始点向上找3个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Left_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=LCDH-1)//起始点位置校正，排除数组越界的可能
        start=LCDH-1;
    else if(start<=0)
        start=0;
    if(end>=LCDH-1)
        end=LCDH-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }
    //这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
    //建议进行判断，根据a1和a2的大小关系，决定++或者--访问
    //这里修改各位自行操作
    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Left_Add_Line(Left_Line[start],start,Left_Line[end],end);
    }
    else
    {
        k=(float)(Left_Line[start]-Left_Line[start-4])/5.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Left_Line[i]=(int)(i-start)*k+Left_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Left_Line[i]>=LCDW-1)
            {
                Left_Line[i]=LCDW-1;
            }
            else if(Left_Line[i]<=0)
            {
                Left_Line[i]=0;
            }
        }
    }
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     右左边界延长
  @param     延长起始行数，延长到某行
  @return    null
  Sample     Lengthen_Right_Boundry(int start,int end)；
  @note      从起始点向上找3个点，算出斜率，向下延长，直至结束点
-------------------------------------------------------------------------------------------------------------------*/
void Lengthen_Right_Boundry(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=LCDH-1)//起始点位置校正，排除数组越界的可能
        start=LCDH-1;
    else if(start<=0)
        start=0;
    if(end>=LCDH-1)
        end=LCDH-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }
    //这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
    //建议进行判断，根据a1和a2的大小关系，决定++或者--访问
    //这里修改各位自行操作
    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(Right_Line[start],start,Right_Line[end],end);
    }
    else
    {
        k=(float)(Right_Line[start]-Right_Line[start-4])/5.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Right_Line[i]=(int)(i-start)*k+Right_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Right_Line[i]>=LCDW-1)
            {
                Right_Line[i]=LCDW-1;
            }
            else if(Right_Line[i]<=0)
            {
                Right_Line[i]=0;
            }
        }
    }
}

void Lengthen_Right_Boundry_Down(int start,int end)
{
    int i,t;
    float k=0;
    if(start>=LCDH-1)//起始点位置校正，排除数组越界的可能
        start=LCDH-1;
    else if(start<=0)
        start=0;
    if(end>=LCDH-1)
        end=LCDH-1;
    else if(end<=0)
        end=0;
    if(end<start)//++访问，坐标互换
    {
        t=end;
        end=start;
        start=t;
    }
    //这里有bug，下方循环++循环，只进行y的互换，但是没有进行x的互换
    //建议进行判断，根据a1和a2的大小关系，决定++或者--访问
    //这里修改各位自行操作
    if(start<=5)//因为需要在开始点向上找3个点，对于起始点过于靠上，不能做延长，只能直接连线
    {
        Right_Add_Line(Right_Line[start],start,Right_Line[end],end);
    }
    else  //小问题:如果开始行过低的话，向下搜三行会搜到丢线行
    {
        k=(float)(Right_Line[start]-Right_Line[start+4])/4.0;//这里的k是1/斜率
        for(i=start;i<=end;i++)
        {
            Right_Line[i]=(int)(i-start)*k+Right_Line[start];//(x=(y-y1)*k+x1),点斜式变形
            if(Right_Line[i]>=LCDW-1)
            {
                Right_Line[i]=LCDW-1;
            }
            else if(Right_Line[i]<=0)
            {
                Right_Line[i]=0;
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右下角点检测
  @param     起始点，终止点
  @return    返回角点所在的行数，找不到返回0
  Sample     Find_Right_Down_Point(int start,int end);
  @note      角点检测阈值可根据实际值更改
-------------------------------------------------------------------------------------------------------------------*/
int Find_Right_Down_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int right_down_line=0;
    if(Right_Lost_Time>=0.9*LCDH)//大部分都丢线，没有拐点判断的意义
        return right_down_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=LCDH-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=LCDH-1-5;
    if(end<=LCDH-Search_Stop_Line)
        end=LCDH-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(right_down_line==0 && //只找第一个符合条件的点
           ABS(Right_Line[i]-Right_Line[i+1])<=5 &&  //角点的阈值可以更改
           ABS(Right_Line[i+1]-Right_Line[i+2])<=5 &&
           ABS(Right_Line[i+2]-Right_Line[i+3])<=5 &&
              (Right_Line[i]-Right_Line[i-2])<=-5 &&
              (Right_Line[i]-Right_Line[i-3])<=-10 &&
              (Right_Line[i]-Right_Line[i-4])<=-10)
        {
            right_down_line=i;//获取行数即可
            break;
        }
    }
    return right_down_line;
}


int Find_Right_Up_Point(int start,int end)//找四个角点，返回值是角点所在的行数
{
    int i,t;
    int right_up_line=0;
    if(Right_Lost_Time>=0.9*LCDH)//大部分都丢线，没有拐点判断的意义
        return right_up_line;
    if(start<end)
    {
        t=start;
        start=end;
        end=t;
    }
    if(start>=LCDH-1-5)//下面5行数据不稳定，不能作为边界点来判断，舍弃
        start=LCDH-1-5;
    if(end<=LCDH-Search_Stop_Line)
        end=LCDH-Search_Stop_Line;
    if(end<=5)
       end=5;
    for(i=start;i>=end;i--)
    {
        if(right_up_line==0 &&//只找第一个符合条件的点
            ABS(Right_Line[i]-Right_Line[i-1])<=5&&//下面两行位置差不多
            ABS(Right_Line[i-1]-Right_Line[i-2])<=5&&
            ABS(Right_Line[i-2]-Right_Line[i-3])<=5&&
              (Right_Line[i]-Right_Line[i+2])<=-8&&
              (Right_Line[i]-Right_Line[i+3])<=-15&&
              (Right_Line[i]-Right_Line[i+4])<=-15)
        {
            right_up_line=i;//获取行数即可
            break;
        }
    }
    return right_up_line;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     右赛道连续性检测
  @param     起始点，终止点
  @return    连续返回0，不连续返回断线出行数
  Sample     continuity_change_flag=Continuity_Change_Right(int start,int end)
  @note      连续性的阈值设置为5，可更改
-------------------------------------------------------------------------------------------------------------------*/
int Continuity_Change_Right(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Right_Lost_Time>=0.9*LCDH)//大部分都丢线，没必要判断了
       return 1;
    if(start>=LCDH-5)//数组越界保护
        start=LCDH-5;
    if(end<=5)
       end=5;
    if(start<end)//都是从下往上计算的，反了就互换一下
    {
       t=start;
       start=end;
       end=t;
    }

    for(i=start;i>=end;i--)
    {
        if(ABS(Right_Line[i]-Right_Line[i-1])>=5)//连续性阈值是5，可更改
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

int Continuity_Change_Left(int start,int end)
{
    int i;
    int t;
    int continuity_change_flag=0;
    if(Left_Lost_Time>=0.9*LCDH)//大部分都丢线，没必要判断了
       return 1;
    if(start>=LCDH-5)//数组越界保护
        start=LCDH-5;
    if(end<=5)
       end=5;
    if(start<end)//都是从下往上计算的，反了就互换一下
    {
       t=start;
       start=end;
       end=t;
    }

    for(i=start;i>=end;i--)
    {
        if(ABS(Left_Line[i]-Left_Line[i-1])>=5)//连续性阈值是5，可更改
       {
            continuity_change_flag=i;
            break;
       }
    }
    return continuity_change_flag;
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     单调性突变检测
  @param     起始点，终止行
  @return    点所在的行数，找不到返回0
  Sample     Find_Right_Up_Point(int start,int end);
  @note      前5后5它最大（最小），那他就是角点
-------------------------------------------------------------------------------------------------------------------*/
int Monotonicity_Change_Right(int start,int end)//单调性改变，返回值是单调性改变点所在的行数
{
    int i;
    int monotonicity_change_line=0;

    if(Right_Lost_Time>=0.9*LCDH)//大部分都丢线，没有单调性判断的意义
       return monotonicity_change_line;
    if(start>=LCDH-1-5)//数组越界保护
       start=LCDH-1-5;
    if(end<=5)
       end=5;

    if(start<end)//都是从下往上计算的，反了就互换一下
    {
        int t;
        t=start;
        start=end;
        end=t;
    }

    for(i=start;i>=end;i--)//会读取前5后5数据，所以前面对输入范围有要求
    {
        if(Right_Line[i]==Right_Line[i+5]&&Right_Line[i]==Right_Line[i-5]&&
        Right_Line[i]==Right_Line[i+4]&&Right_Line[i]==Right_Line[i-4]&&
        Right_Line[i]==Right_Line[i+3]&&Right_Line[i]==Right_Line[i-3]&&
        Right_Line[i]==Right_Line[i+2]&&Right_Line[i]==Right_Line[i-2]&&
        Right_Line[i]==Right_Line[i+1]&&Right_Line[i]==Right_Line[i-1])
        {//一堆数据一样，显然不能作为单调转折点
            continue;
        }
        else if(Right_Line[i] <Right_Line[i+5]&&Right_Line[i] <Right_Line[i-5]&&
        Right_Line[i] <Right_Line[i+4]&&Right_Line[i] <Right_Line[i-4]&&
        Right_Line[i]<=Right_Line[i+3]&&Right_Line[i]<=Right_Line[i-3]&&
        Right_Line[i]<=Right_Line[i+2]&&Right_Line[i]<=Right_Line[i-2]&&
        Right_Line[i]<=Right_Line[i+1]&&Right_Line[i]<=Right_Line[i-1])
        {//就很暴力，这个数据是在前5，后5中最大的，那就是单调突变点
            monotonicity_change_line=i;
            break;
        }
    }
    return monotonicity_change_line;
}


/*-------------------------------------------------------------------------------------------------------------------
  @brief     通过斜率，定点补线--
  @param     k       输入斜率
             startY  输入起始点纵坐标
             endY    结束点纵坐标
  @return    null
  Sample     K_Add_Boundry_Left(float k,int startY,int endY);
  @note      补得线直接贴在边线上
-------------------------------------------------------------------------------------------------------------------*/
void K_Add_Boundry_Left(float k,int startX,int startY,int endY)
{
    int i,t;
    if(startY>=LCDH-1)
        startY=LCDH-1;
    else if(startY<=0)
        startY=0;
    if(endY>=LCDH-1)
        endY=LCDH-1;
    else if(endY<=0)
        endY=0;
    if(startY<endY)//--操作，start需要大
    {
        t=startY;
        startY=endY;
        endY=t;
    }
    //这里有bug，下方循环--循环，需要start要大，只进行y的互换，但是没有进行x的互换
    //建议进行判断，如果start更小，那就进行++访问
    //这里修改各位自行操作
    for(i=startY;i>=endY;i--)
    {
        Left_Line[i]=(int)((i-startY)/k+startX);//(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
        if(Left_Line[i]>=LCDW-1)
        {
            Left_Line[i]=LCDW-1;
        }
        else if(Left_Line[i]<=0)
        {
            Left_Line[i]=0;
        }
    }
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     根据斜率划线
  @param     输入斜率，定点，画一条黑线
  @return    null
  Sample     K_Draw_Line(k, 20,MT9V03X_H-1 ,0)
  @note      补的就是一条线，需要重新扫线
-------------------------------------------------------------------------------------------------------------------*/
void K_Draw_Line(float k, int startX, int startY,int endY)
{
    int endX=0;

    if(startX>=LCDW-1)//限幅处理
        startX=LCDW-1;
    else if(startX<=0)
        startX=0;
    if(startY>=LCDH-1)
        startY=LCDH-1;
    else if(startY<=0)
        startY=0;
    if(endY>=LCDH-1)
        endY=LCDH-1;
    else if(endY<=0)
        endY=0;

    endX=(int)((endY-startY)/k+startX);//(y-y1)=k(x-x1)变形，x=(y-y1)/k+x1
    Draw_Line(startX,startY,endX,endY);
}

/*-------------------------------------------------------------------------------------------------------------------
  @brief     画线
  @param     输入起始点，终点坐标，补一条宽度为2的黑线
  @return    null
  Sample     Draw_Line(0, 0,MT9V03X_W-1,MT9V03X_H-1);
             Draw_Line(MT9V03X_W-1, 0,0,MT9V03X_H-1);
                                    画一个大×
  @note     补的就是一条线，需要重新扫线
-------------------------------------------------------------------------------------------------------------------*/
void Draw_Line(int startX, int startY, int endX, int endY)
{
    int i,x,y;
    int start=0,end=0;
    if(startX>=LCDW-1)//限幅处理
        startX=LCDW-1;
    else if(startX<=0)
        startX=0;
    if(startY>=LCDH-1)
        startY=LCDH-1;
    else if(startY<=0)
        startY=0;
    if(endX>=LCDW-1)
        endX=LCDW-1;
    else if(endX<=0)
        endX=0;
    if(endY>=LCDH-1)
        endY=LCDH-1;
    else if(endY<=0)
        endY=0;

    if(startX==endX)//一条竖线
    {
        if (startY > endY)//互换
        {
            start=endY;
            end=startY;
        }
        for (i = start; i <= end; i++)
        {
            if(startX<=1)
                startX=1;
            Bin_Image[i][startX]=0;
            Bin_Image[i][startX-1]=0;
        }
    }
    else if(startY == endY)//补一条横线
    {
        if (startX > endX)//互换
        {
            start=endX;
            end=startX;
        }
        for (i = start; i <= end; i++)
        {
            if(startY<=1)
                startY=1;
            Bin_Image[startY][i]=0;
            Bin_Image[startY-1][i]=0;
        }
    }
    else //上面两个是水平，竖直特殊情况，下面是常见情况
    {
        if(startY>endY)//起始点矫正
        {
            start=endY;
            end=startY;
        }
        else
        {
            start=startY;
            end=endY;
        }
        for (i = start; i <= end; i++)//纵向补线，保证每一行都有黑点
        {
            x =(int)(startX+(endX-startX)*(i-startY)/(endY-startY));//两点式变形
            if(x>=LCDW-1)
                x=LCDW-1;
            else if (x<=1)
                x=1;
            Bin_Image[i][x] = 0;
            Bin_Image[i][x-1] = 0;
        }
        if(startX>endX)
        {
            start=endX;
            end=startX;
        }
        else
        {
            start=startX;
            end=endX;
        }
        for (i = start; i <= end; i++)//横向补线，保证每一列都有黑点
        {
            y =(int)(startY+(endY-startY)*(i-startX)/(endX-startX));//两点式变形
            if(y>=LCDH-1)
                y=LCDH-1;
            else if (y<=0)
                y=0;
            Bin_Image[y][i] = 0;
        }
    }
}

void Island_Detect(void)
{
    Island_Flag=0;
    if(CrossWay_Flag==0)
    {
        if(Island_State==0)
        {
            Right_Down_Find=Find_Right_Down_Point(LCDH-5,50);
            if(Right_Lost_Time>=20 && Right_Lost_Time-Left_Lost_Time>=10 && Search_Stop_Line>=80 &&
                    Boundry_Start_Left>=80 && Boundry_Start_Right>=80 && Right_Down_Find!=0)
            {
                 Island_State=1;
            }
        }
        else if(Island_State==1)
        {
            Right_Down_Find=Find_Right_Down_Point(LCDH-5,50);
            Mon_change_Right_Line=Monotonicity_Change_Right(Right_Down_Find-5,0);
            if(Mon_change_Right_Line!=0)
            {
                //补线
                Right_Add_Line(Right_Line[Mon_change_Right_Line],Mon_change_Right_Line,Right_Line[Right_Down_Find],Right_Down_Find);
            }
            else
            {
                Lengthen_Right_Boundry_Down(Right_Down_Find+1,Right_Down_Find+30);
            }

            if(Boundry_Start_Right<=60 && Right_Down_Find==0 && Mon_change_Right_Line!=0 && Right_Lost_Time>=60)
            {
                Island_State=2;
            }
        }
        else if(Island_State==2)
        {
            if(Mon_change_Right_Line!=0)
            {
                monotonicity_change_line[0]=Monotonicity_Change_Right(70,5);//单调性改变
                monotonicity_change_line[1]=Right_Line[monotonicity_change_line[0]];
                Right_Add_Line((int)(LCDW-1-(monotonicity_change_line[1]*0.15)),LCDH-1,monotonicity_change_line[1],monotonicity_change_line[0]);
                if((Boundry_Start_Right>=LCDH-5||monotonicity_change_line[0]>50) && monotonicity_change_line[0]==0)//右下角再不丢线进3
                {
                    Island_State=3;//下方丢线，说明大弧线已经下来了
                }

            }
        }
        else if(Island_State==3)
        {
//            Find_Up_Point(70,5);
//            if(Right_Up_Find!=0)
//            {
//
//            }
            if(k!=0)//已经找到点了，画一条死线
            {
                K_Draw_Line(k,30,LCDH-1,0);
                Longest_White_Column();//刷新最长白列
            }
            else//还在找点
            {
                Right_Up_Guai[0]=Find_Right_Up_Point(40,10);//找右上拐点
                Right_Up_Guai[1]=Right_Line[Right_Up_Guai[0]];
                if(Right_Up_Guai[0]<10)//角点位置不对，退出环岛
                {
                    Island_State=0;
                    Right_Island_Flag=0;
                }
                if(k==0&&(15<=Right_Up_Guai[0]&&Right_Up_Guai[0]<50)&&(70<Right_Up_Guai[1]&&Right_Up_Guai[1]<150))
                {//当角点出现在给定范围内
                    island_state_3_up[0]= Right_Up_Guai[0];
                    island_state_3_up[1]= Right_Up_Guai[1];
                    k=(float)((float)(LCDH-island_state_3_up[0])/(float)(20-island_state_3_up[1]));
                    K_Draw_Line(k,30,LCDH-1,0);
                    Longest_White_Column();//刷新赛道数据
                }
            }
//            if((Island_State==3)&&(abs(FJ_Angle)>=60))//只依靠陀螺仪积分
//            {
//                k=0;//斜率清零
//                Island_State=4;
//                Longest_White_Column();//最长白列
//            }
        }
    }

}

