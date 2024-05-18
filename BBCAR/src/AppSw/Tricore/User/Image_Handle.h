/*
 * Image_Handle.h
 *
 *  Created on: 2023骞�12鏈�10鏃�
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_IMAGE_HANDLE_H_
#define SRC_APPSW_TRICORE_USER_IMAGE_HANDLE_H_

#include <LQ_CAMERA.h>

extern int Servo_Loc_error;
extern uint8 StraightWay_flag;

int ABS(int A);
uint8 min(uint8 A,uint8 B);
void image_handle(void);
void Lost_Line_Left(uint8* count,uint8* loc);
void Lost_Line_Right(uint8* count,uint8* loc);
void Get_ABCD(void);
void GetImageSide(void);
void GetUpDownSide(void);
void Judge_CrossRoad(void);


#endif /* SRC_APPSW_TRICORE_USER_IMAGE_HANDLE_H_ */
