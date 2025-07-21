
/*
 * My_Key.c
  *  功能: 通过定时器STM中断检测按键是否按键，支持连续按，但只会响应一次，定时周期为20ms，刚好消除去抖动，使用CCU6的CH1通道进行定时
 *  Created on: 2023年12月10日
 *      Author: dduu
 */

#include "My_Key.h"

//按键中断服务函数在LQ_CCU6中 定义在 void CCU60_CH0_IRQHandler(void)

key_t Key[5]; //存储Key的状态

//每20ms进入一次定时中断中，检测按键状态
void Key_Check(int i, key_t *Key, GPIO_Name_t pin)
{
    switch( Key[i].keyState )
    {
        case KEY_CHECK:
            // 读到低电平，进入按键确认状态
            if(PIN_Read(pin) == GPIO_LOW)
            {
                Key[i].keyState = KEY_COMFIRM;
            }
        break;

        case KEY_COMFIRM:
            if(PIN_Read(pin) == GPIO_LOW)
            {
                //读到低电平，按键确实按下，按键标志位置1，并进入按键释放状态
                Key[i].keyState = KEY_UNPRESSED;
                Key[i].keyFlag = 1;
            }
            //读到高电平，可能是干扰信号，返回初始状态
            else
            {
                Key[i].keyState = KEY_CHECK;
            }
        break;

        case KEY_UNPRESSED:
            if(PIN_Read(pin) == GPIO_HIGH)
            {
             // 读到高电平，说明按键释放，返回初始状态
             Key[i].keyState = KEY_CHECK;
            }
        break;
    }
}



