/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
 【平    台】北京龙邱智能科技TC264DA核心板
 【编    写】ZYF/chiusir
 【E-mail  】chiusir@163.com
 【软件版本】V1.1 版权所有，单位使用请先联系授权
 【最后更新】2020年4月10日
 【相关信息参考下列地址】
 【网    站】http://www.lqist.cn
 【淘宝店铺】http://longqiu.taobao.com
 ------------------------------------------------
 【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
 【Target 】 TC264DA
 【Crystal】 20.000Mhz
 【SYS PLL】 200MHz
 ________________________________________________________________

 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ
 *  备    注：TC264只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
 *           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
 *           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
 *          外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
 *          中断在那个核初始化，中断VECTABNUM号必须对应该cpu,否则不会调用中断函数
 QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef SRC_APPSW_TRICORE_MAIN_MAIN_H_
#define SRC_APPSW_TRICORE_MAIN_MAIN_H_

#include <include.h>
#include <stdio.h>
#include <stdint.h>
#include <LQ_ADC.h>
#include <IfxCpu.h>
#include <IfxStm.h>
#include <LQ_UART.h>
#include <LQ_CCU6.h>
#include <LQ_TFT18.h>
#include <LQ_CAMERA.h>
#include <IfxScuCcu.h>
#include <IfxScuWdt.h>
#include <LQ_SOFTI2C.h>
#include <IfxStm_reg.h>
#include <LQ_IIC_Gyro.h>
#include <LQ_I2C_VL53.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_MotorServo.h>
#include <Platform_Types.h>
#include "../User/CircularBuffer.h"

#include "image1.h"
#include "My_Key.h"
#include "My_menu.h"
#include "My_UART.h"
#include "Image_Handle.h"
#include "LQ_ImageProcess.h"
#include "My_Image_Handle.h"


extern App_Cpu0 g_AppCpu0; // brief CPU 0 global data
extern IfxCpu_mutexLock mutexCpu0InitIsOk;   /** CPU0 锟斤拷始锟斤拷锟斤拷杀锟街疚�  */
extern volatile char mutexCpu0TFTIsOk;       /** TFT18使锟矫憋拷志位  */
/**
 * 锟斤拷时锟斤拷 5ms锟斤拷50ms锟斤拷志位
 */
extern volatile uint8 cpu1Flage5ms;
extern volatile uint8 cpu1Flage50ms;

/* 锟斤拷锟斤拷锟劫讹拷  */
extern volatile sint16 targetSpeed;

/* 锟斤拷锟较憋拷志位 */
extern volatile uint8 evadibleFlage;

void Init_System(void);
int core0_main(void);

#endif /* SRC_APPSW_TRICORE_MAIN_MAIN_H_ */
