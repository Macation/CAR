/*
 * My_Key.h
 *
 *  Created on: 2023年12月10日
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_KEY_H_
#define SRC_APPSW_TRICORE_USER_MY_KEY_H_

#include <LQ_STM.h>
#include <LQ_GPIO.h>
#include <LQ_CCU6.h>
#include "My_Button.h"

#define KEY0_Pin P22_3   //K1
#define KEY1_Pin P22_1   //K2
#define KEY2_Pin P22_2   //k3
#define KEY3_Pin P22_0   //K4
#define KEY4_Pin P33_12  //k5

#define GPIO_LOW 0
#define GPIO_HIGH 1
//全是低电平点亮

typedef enum
{
    KEY_CHECK = 0,  //按键检测状态
    KEY_COMFIRM,    //按键确认状态
    KEY_UNPRESSED     //按键释放状态
}keyState_e;        //状态枚举变量

typedef struct
{
    keyState_e keyState; //按键状态
    uint8 keyFlag;       //按键按下标志
}key_t;                //按键状态结构体

extern key_t Key[5];

void Key_Check(int i, key_t *Key, GPIO_Name_t pin);


#endif /* SRC_APPSW_TRICORE_USER_MY_KEY_H_ */
