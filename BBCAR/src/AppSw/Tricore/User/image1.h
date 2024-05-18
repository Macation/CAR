/*
 * newimage.h
 *
 *  Created on: 2024Äê5ÔÂ14ÈÕ
 *      Author: Macation
 */

#ifndef SRC_APPSW_TRICORE_USER_NEWIMAGE_H_
#define SRC_APPSW_TRICORE_USER_NEWIMAGE_H_
#include "LQ_CAMERA.h"
#include <IfxCpu.h>
#include <IfxPort_reg.h>
#include <Platform_Types.h>
#include <stdlib.h>
#include <stdio.h>

#include "LQ_DMA.h"
#include "LQ_GPIO.h"
#include "LQ_UART.h"
#include "LQ_GPIO_LED.h"
#include "LQ_TFT18.h"
#include "string.h"
extern unsigned char Image_Line[120][160];
int Image_Threshold_Line(int Y);
int Image_Findline_Lefthand(int X,int Y,int num,int block_size,int clip_value);
void Image_SeedGrowing(void);
extern unsigned char show[120][160];
extern int error1;
void showing(void);
void Image_Frameprocessing(void);
int Image_Findline_Righthand(int X,int Y,int num,int block_size,int clip_value);
void Image_seedCentralline(int X1,int X2);
#endif /* SRC_APPSW_TRICORE_USER_NEWIMAGE_H_ */
