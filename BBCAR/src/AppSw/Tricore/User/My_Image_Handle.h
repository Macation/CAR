/*
 * My_Image_Handle.h
 *
 *  Created on: 2024年3月4日
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_IMAGE_HANDLE_H_
#define SRC_APPSW_TRICORE_USER_MY_IMAGE_HANDLE_H_

#include <stdlib.h>
#include <LQ_CAMERA.h>
#include <LQ_UART.h>

extern volatile int Left_Line[LCDH];           //左边界
extern volatile int Right_Line[LCDH];          //右边界
extern volatile int Middle_Line[LCDH];         //中边界

float Err_Sum(void);
float Cal_Err(void);
void Longest_White_Column(void);
void Straight_Detect(void);
void Curve_Detect(void);
void Left_Add_Line(int x1,int y1,int x2,int y2);
void K_Add_Boundry_Left(float k,int startX,int startY,int endY);
void K_Draw_Line(float k, int startX, int startY,int endY);
void Right_Add_Line(int x1,int y1,int x2,int y2);
void Draw_Line(int startX, int startY, int endX, int endY);
void Cross_Detect(void);
void Island_Detect(void);
void Find_Up_Point(int start,int end);
void Find_Down_Point(int start,int end);
void Lengthen_Left_Boundry(int start,int end);
void Lengthen_Right_Boundry(int start,int end); //向上找线
void Lengthen_Right_Boundry_Down(int start,int end); //向下找线
int Continuity_Change_Right(int start,int end);
int Monotonicity_Change_Right(int start,int end);
int Find_Right_Down_Point(int start,int end);
int Find_Right_Up_Point(int start,int end);

#endif /* SRC_APPSW_TRICORE_USER_MY_IMAGE_HANDLE_H_ */
