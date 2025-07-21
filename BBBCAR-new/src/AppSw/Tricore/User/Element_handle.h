/*
 * Element_handle.h
 *
 *  Created on: 2024年6月3日
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_ELEMENT_HANDLE_H_
#define SRC_APPSW_TRICORE_USER_ELEMENT_HANDLE_H_

#include <Platform_Types.h>
#include <LQ_MotorServo.h>
#include <LQ_CAMERA.h>
#include "My_Control_Code.h"

typedef struct{
    uint8 Curve_flag;               //弯道
    uint8 Rampway_flag;             //坡道
    uint8 Zebra;                    //斑马线
    uint8 Crossroad_flag;           //十字路口
    uint8 Straightaway_flag;        //直道
    uint8 Circle_left;              //左圆环
    uint8 Circle_right;             //右圆环
    uint8 Obstruction;              //障碍物
    uint8 Small_Curve_flag;         //小s弯
    uint8 zuo_flag;
}Image_element;

//这个结构体控制是否需要识别元素，自定义元素优先级顺序为：1.路障 ；2. 第一行丢线的断路； 3. 左圆环； 4. 右圆环； 5. 断路;  6.十字
//图像元素结构体
typedef struct
{
    //十字标志位
    unsigned char Cross_Identify;
    //圆环标志位
    unsigned char Circle_left_Identify;
    unsigned char Circle_Right_Identify;
    //路障标志位
    unsigned char Obstruction_Identify;
    unsigned char Straightaway_Identify;
    //斑马线标志位
    unsigned char Zebra_Identify;
    //路障标志位
    unsigned char All_Identify;
    //路障标志位
    unsigned char Small_Curve_Identify;
    unsigned char Curve_Identify;
    unsigned char zuo_Identify;
}Image_Identify;

//计数器结构体
typedef struct
{
    sint16 Cross_Pulse;          // 十字编码器总计数器
    sint16 Cross__Pulse_Last;          // 十字编码器计数器

    sint16 Circle_Left_Pulse;          // 左圆环编码器总计数器
    sint16 Circle_Left_Pulse_Last;          // 左圆环编码器计数器

    sint16 Circle_Right_Pulse;          // 右圆环编码器总计数器
    sint16 Circle_Right_Pulse_Last;          // 右圆环编码器计数器

    sint16 Obstruction_Pulse;          // 障碍物编码器总计数器
    sint16 Obstruction_Pulse_Last;          // 障碍物编码器计数器

    sint16 Curve_Pulse;          // 弯道编码器总计数器
    sint16 Curve_Pulse_Last;          //弯道编码器计数器

    sint16 Zebra_Pulse;         //斑马线计数器
    sint16 Zebra_Pulse_Last;     //斑马线计数器

    unsigned char Cross_Pulse_Key;            //十字计数器开关
    unsigned char Circle_Left_Pulse_Key;      //左圆环计数器开关
    unsigned char Circle_Right_Pulse_Key;     //右圆环计数器开关
    unsigned char Obstruction_Pulse_Key;      //路障计数器开关
    unsigned char Curve_Pulse_Key;            //弯道计数器开关
    unsigned char Zebra_Pulse_Key;            //弯道计数器开关
}Pulse_Param_t;

extern char keep_beep;
extern char count_beep;
extern Image_element Element1;
extern Image_Identify Identify1;
extern Pulse_Param_t ALLPULSE;
extern uint8 angle_flag;
extern float Angle_in;//进圆环时的角度
extern float Angle_delta;//角度变化值
extern int width[10];
extern unsigned int zhangaiflag;
extern int tmperr;

void Set_NULLflag(void);
void Calcu_Pulse(void);
void My_PulseClean(Pulse_Param_t * PULSE,int element);
void My_PulseCount(Pulse_Param_t * PULSE,int element);
void Image_IdentifyProcess(Image_Identify * Ide);
void Image_CrossProcess(Image_element* Ele);
void Image_LeftCircleProcess(Image_element* Ele);
void Image_RightCircleProcess(Image_element* Ele);
void My_ObstructionProcess(Image_element* Ele);
void Image_ZebraProcess(Image_element* Ele);
void Control_Beep(Image_element* Image_flag);
void My_PulseKey(Pulse_Param_t * PULSE,int element,unsigned char state);
void Image_IdentifySet(Image_Identify * Ide, Image_element* Ele);
void Speed_Decision(Image_element* element);
void Direction_Decision(Image_element* element);
void Straightaway_Process(Image_element* Ele);
void Small_Curve_Process(Image_element* Ele);
void Curve_Process(Image_element* Ele);
void My_ElementClean(Image_element* Ele,int i);
void zuo_Process(Image_element* Ele);
#endif /* SRC_APPSW_TRICORE_USER_ELEMENT_HANDLE_H_ */
