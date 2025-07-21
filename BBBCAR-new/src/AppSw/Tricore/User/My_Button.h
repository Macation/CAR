/*
 * My_Button.h
 *
 *  Created on: 2024年4月29日
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_MY_BUTTON_H_
#define SRC_APPSW_TRICORE_USER_MY_BUTTON_H_

#define BtnP    0    /* 左键 */
#define BtnN    1    /* 右键 */
#define BtnE    2    /* 确认键 */
#define BtnR    3    /* 返回键 */

#include <LQ_GPIO.h>
#include <LQ_TFT18.h>
#include <stdio.h>
#include <My_Key.h>
#include <My_menu.h>
#include <My_Control_Code.h>

extern int chooseBuf;    //左右选择时不立即进入该菜单
extern int switch_flag;    //页面更新标志

int isBtnP();
int isBtnN();
int isBtnE();
int isBtnR();
void Button_Scan(void);

#endif /* SRC_APPSW_TRICORE_USER_MY_BUTTON_H_ */
