/*
 * newimage.h
 *
 *  Created on: 2024年5月14日
 *      Author: Macation
 */

#ifndef SRC_APPSW_TRICORE_USER_NEWIMAGE_H_
#define SRC_APPSW_TRICORE_USER_NEWIMAGE_H_
//#define Array_Length MT9V034_IMAGEH/2+MT9V034_IMAGEW/2 //边线数组的长度 154

#define Array_Length 154 //边线数组的长度 154

#include "LQ_CAMERA.h"
#include <IfxCpu.h>
#include <IfxPort_reg.h>
#include <Platform_Types.h>
#include <stdlib.h>
#include <stdio.h>
#include <Element_handle.h>

#include "LQ_DMA.h"
#include "LQ_GPIO.h"
#include "LQ_UART.h"
#include "LQ_GPIO_LED.h"
#include "LQ_TFT18.h"
#include "string.h"

extern unsigned char Image_Line[120][160];
extern int Image_Lefthand_Line[Array_Length][2];
extern int Image_Righthand_Line[Array_Length][2];
extern int Servo_QZ_error;   //前瞻误差
extern int Servo_All_error;
extern float currentslope;
extern float currentslope2;
extern int crossflag;
extern int shizi;
extern int threshold;
extern int Left_num;
extern int Right_num;
extern int cout;
extern int qulv_value;
extern int qvlv_quanju_right;
extern int qvlv_quanju_left;
extern int current_state1;
extern int current_state2;
extern int guaidian1[Array_Length][2];
extern int guaidian2[Array_Length][2];
extern float a;
extern int Servo_Loc_error;
extern int image_error[];
extern uint8 image_flag;
extern int kpp;
extern int zuodianxian;
extern int youdianxian;
extern int Image_centralLine[95];//存放边线的数组
extern short OSTU;

int Image_Threshold_Line(int Y);
int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value);
void Image_SeedGrowing(void);
int Image_findSeedBegin(int thres);
void showing(void);
void Image_Frameprocessing(void);
int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value);
void Image_seedCentralline(int X1,int X2);
int Calc_ERROR(void);
float linearRegression(int path[][2], int start, int end);
float linearRegression2zhong(int path[], int start, int end);
void leftxielv(int leftstart,int rightstart);
float calculateCurvature(int path[][2], int idx, int path_length);
void blur(int ptsin[][2],int *num,int ptsout[][2],int he);
int find(float currentslope);
void qulv(void);
void draw_line(int x1, int y1, int x2, int y2);


#endif /* SRC_USER_IMAGE1_H_ */
