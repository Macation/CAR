/*
 * newimage.c
 *
 *  Created on: 2024��5��14��
 *      Author: Macation
 */
#include "image1.h"
#define Image_downLine 115
#define Image_upLine 15
#define central_length Image_downLine-Image_upLine
int Image_centralLine[central_length];//��ű��ߵ�����
#define Array_Length MT9V034_IMAGEH/2+MT9V034_IMAGEW/2 //��������ĳ��� 154
int Image_Lefthand_Line[Array_Length][2];//�����ȡ��ı������������
int Image_Righthand_Line[Array_Length][2];
unsigned char Image_Line[120][160];//��ű��ߵ����飬0��1��ʾ
int Left_num=Array_Length;
int Right_num=Array_Length;
int block_size=7;
int error1=0;
//�洢��ǰ������ǰ����ǰ���ķ�������
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
    for (i = 0; i < LCDH; i ++)          //���۸� 120 / 2  = 60��
    // for (i = 0; i < IMAGEH; i += 3)       //OV7725�� 240 / 3  = 80��
    {
        for (j = 0; j <= LCDW; j ++)     //���ۿ�188 / 2  = 94��
        // for (j = 0; j <= IMAGEW; j += 3)  //OV7725��320 / 3  = 106��
        {
            show[row][line] = Image_Data[i][j+17];
            line++;
        }
        line = 0;
        row++;
    }
}
/*************************************************************************
*  �������ƣ�int Image_Threshold_Line(int Y)
*  ����˵�����Ե�������ֵ
*  �������أ���
*  �޸�ʱ�䣺2023��6��15��
*  ��    ע��
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
    float OmegaBack, OmegaFore, MicroBack, MicroFore, SigmaB, Sigma; // ��䷽��;
    int MinValue, MaxValue;
    int Threshold = 0;
    unsigned char HistoGram[256];              //

    for (j = 0; j < 256; j++)
        HistoGram[j] = 0; //��ʼ���Ҷ�ֱ��ͼ

    for(i=0;i<MT9V034_IMAGEW;i++)
    {
        HistoGram[Image_Data[Y][i]]++;
    }

    for (MinValue = 0; MinValue < 256 && HistoGram[MinValue] == 0; MinValue++)
        ;        //��ȡ��С�Ҷȵ�ֵ
    for (MaxValue = 255; MaxValue > MinValue && HistoGram[MaxValue] == 0;
            MaxValue--)
        ; //��ȡ���Ҷȵ�ֵ

    if (MaxValue == MinValue)
        return MaxValue;         // ͼ����ֻ��һ����ɫ
    if (MinValue + 1 == MaxValue)
        return MinValue;        // ͼ����ֻ�ж�����ɫ

    for (j = MinValue; j <= MaxValue; j++)
        Amount += HistoGram[j];        //  ��������

    Pixelshortegral = 0;
    for (j = MinValue; j <= MaxValue; j++) {
        Pixelshortegral += HistoGram[j] * j;        //�Ҷ�ֵ����
    }
    SigmaB = -1;
    for (j = MinValue; j < MaxValue; j++) {
        PixelBack = PixelBack + HistoGram[j];     //ǰ�����ص���
        PixelFore = Amount - PixelBack;           //�������ص���
        OmegaBack = (float) PixelBack / Amount;   //ǰ�����ذٷֱ�
        OmegaFore = (float) PixelFore / Amount;   //�������ذٷֱ�
        PixelshortegralBack += HistoGram[j] * j;  //ǰ���Ҷ�ֵ
        PixelshortegralFore = Pixelshortegral - PixelshortegralBack;  //�����Ҷ�ֵ
        MicroBack = (float) PixelshortegralBack / PixelBack;   //ǰ���ҶȰٷֱ�
        MicroFore = (float) PixelshortegralFore / PixelFore;   //�����ҶȰٷֱ�
        Sigma = OmegaBack * OmegaFore * (MicroBack - MicroFore)
                * (MicroBack - MicroFore);   //������䷽��
        if (Sigma > SigmaB)                    //����������䷽��g //�ҳ������䷽���Լ���Ӧ����ֵ
                {
            SigmaB = Sigma;
            Threshold = j;
        }
    }
    return Threshold;                        //���������ֵ;
}
/*************************************************************************
*  �������ƣ�void Image_Frameprocessing(void)
*  ����˵����
*  ����˵�������ڿ�
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
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
*  �������ƣ�int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value)
*  ����˵����
*  ����˵���������Թ�Ѳ��
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
*************************************************************************/
int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value)
{
    int half=block_size/2;//�����һ��
    int step = 0, dir = 0, turn = 0;//���ߵĲ�������ǰ����ת��Ĵ���
    while(step<num && half<X && X<LCDW-half-1 && half< Y && Y <LCDH-1 && turn<4)//ѭ����ֹ��������Խ����ת�����С��4����ֹԭ�ش�ת
    {
        int local_thres = 0; //����Ӧ��ֵ
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += show [Y+dy][X+dx];//��������ֵ�ۼ�
            }
        }
        local_thres /= block_size * block_size;//��ֵȡƽ��
        local_thres -= clip_value;//��ֵ���ü��������Լ���
        int front_value = show[Y+dir_front[dir][1]][X+dir_front[dir][0]];//��ǰ����ǰ���ĵ������ֵ��
        int frontleft_value = show[Y+dir_frontleft[dir][1]][X+dir_frontleft[dir][0]];//��ǰ������ǰ���ĵ������ֵ
        if(front_value<local_thres)//ǰ������Ϊ�ڣ�����ת��ת�����+1
        {
            dir=(dir + 1) % 4;
            turn++;
        }
        else if(frontleft_value<local_thres)//��ǰ������Ϊ�ڣ�ֱ�ߣ�ת�������0������+1
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
*  �������ƣ�int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value)
*  ����˵����
*  ����˵���������Թ�Ѳ��
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
*************************************************************************/
int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value)
{
    int half=block_size/2;//�����һ��
    int step = 0, dir = 0, turn = 0;//���ߵĲ�������ǰ����ת��Ĵ���
    while(step<num&& half<X && X<LCDW-half-1 && half< Y && Y <LCDH-1 && turn<4)//ѭ����ֹ��������Խ����ת�����С��4����ֹԭ�ش�ת)
    {
        int local_thres = 0; //����Ӧ��ֵ
        for(int dy=-half;dy<=half;dy++)
        {
            for(int dx=-half;dx<=half;dx++)
            {
                local_thres += show [Y+dy][X+dx];//��������ֵ�ۼ�
            }
        }
        local_thres /= block_size * block_size;//��ֵȡƽ��
        local_thres -= clip_value;//��ֵ���ü��������Լ���
        int front_value = show[Y+dir_front[dir][1]][X+dir_front[dir][0]];//��ǰ����ǰ���ĵ������ֵ��
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
*  �������ƣ�void Image_SeedGrowing(void)
*  ����˵���������������������ұ��ߺ���
*  �������أ���
*  �޸�ʱ�䣺2024��5��14��
*  ��    ע��
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
    for(;X1>0;X1--)//Ѱ�������
    {
        if(show[Y1][X1-1]<threshold)break;
    }
    Left_num=Array_Length;
    Left_num=Image_Findline_Lefthand(X1,Y1,Left_num,block_size,0);
    for(;X2<LCDW-1;X2++)//Ѱ�������
    {
        if(show[Y2][X2+1]<threshold)break;
    }
    Right_num=Array_Length;
    Right_num=Image_Findline_Righthand(X2,Y2,Right_num,block_size,0);
    Image_seedCentralline(X1,X2);//������
    error1=80-Image_centralLine[90-15];//�������
}
/*************************************************************************
*  �������ƣ�void Image_seedCentralline(int X,int Y)
*  ����˵���������������������ұ��ߺ���
*  �������أ���
*  �޸�ʱ�䣺2024��5��15��
*  ��    ע��
*
*************************************************************************/
void Image_seedCentralline(int X1,int X2)
{
    int  i,j;
    //ɨ�����ʱÿһ�е����
    int LeftStart=X1+5;
    int RightStart=X2-5;
    int Left[central_length]={2};
    int Right[central_length]={LCDW-4};
    for(i=Image_downLine-1;i>Image_upLine;i--)//������������ĵ�һ������Image_downLine��һ�У�����ȡ��ҲҪ����һ��
    {
        //��ֹ��㳬��ͼ��Χ
        if(RightStart>LCDW-4)
        {
            RightStart=LCDW-4;
        }
        if(LeftStart<2)
        {
            LeftStart=2;
        }
        //��ױ��֣�����ڸ���δ�ҵ�����㣬��ͬ��һ��ѭ���Ľ������һ��
//        if(i<Image_downLine-1)
//        {
//            Left[i-Image_upLine]=Left[i+1-Image_upLine];
//            Right[i-Image_upLine]=Right[i+1-Image_upLine];
//
//        }
        //ɨ���ߣ��������һ�е�����������Ӹ��������Բ����м�㿪ʼ������
        for(j=LeftStart;j > 0;j--)
        {
            if(Image_Line[i][j]==1)//�ҵ������
            {
                Left[i-Image_upLine]=j;

                break;
            }
        }
        //ɨ���ߣ�ͬ��������
        for(j=RightStart;j< LCDW-1;j++)
        {
            if(Image_Line[i][j]==1)//�ҵ������
            {
                Right[i-Image_upLine]=j;

                break;
            }
        }
        //�滮��һ�ε����
        LeftStart=Left[i-Image_upLine]+10;
        RightStart=Right[i-Image_upLine]-10;
    }
        for(i=0;i<central_length;i++)
        {
            Image_centralLine[i]=(Left[i]+Right[i])/2;
        }
    //��ͼ���л�������
    for(i=Image_downLine-1;i>Image_upLine-1;i--)
    {
        j=Image_centralLine[i-Image_upLine];
        //�Ӵ֣��������ص�
        Image_Line[i][j]=1;
        Image_Line[i][j-1]=1;

    }
}


