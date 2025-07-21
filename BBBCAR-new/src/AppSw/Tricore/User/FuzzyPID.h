/*
 * FuzzyPID.h
 *
 *  Created on: 2024年5月27日
 *      Author: dduu
 */

#ifndef SRC_APPSW_TRICORE_USER_FUZZYPID_H_
#define SRC_APPSW_TRICORE_USER_FUZZYPID_H_

#include "include.h"

typedef struct
{
    float kp;                       //PID参数kp
    float ki;                       //PID参数ki
    float kd;                       //PID参数kd
    float imax;                     //PID参数积分最大值
    float integrator;               //pid积分器
    float qdetail_kp;               //增量kp对应论域中的值
    float qdetail_ki;               //增量ki对应论域中的值
    float qdetail_kd;               //增量kd对应论域中的值

    float detail_kp;                //输出增量kp
    float detail_ki;                //输出增量ki
    float detail_kd;                //输出增量kd
    float qerro;                    //输入e对应论域中的值
    float qerro_c;                  //输入de/dt对应论域中的值
    float e_gradmembership[2];      //输入e的隶属度
    float ec_gradmembership[2];     //输入de/dt的隶属度
    int e_grad_index[2];            //输入e隶属度在规则表的索引
    int ec_grad_index[2];           //输入de/dt隶属度在规则表的索引

    float gradSums[7];
    float KpgradSums[7];   //输出增量kp总的隶属度
    float KigradSums[7];   //输出增量ki总的隶属度
    float KdgradSums[7];   //输出增量kd总的隶属度
    float output;          //模糊PID输出
    float kp_error;
    float kd_error;
}Fuzzy_param_t;


//论域隶属值
#define NB -3
#define NM -2
#define NS -1
#define ZO 0
#define PS 1
#define PM 2
#define PB 3

extern Fuzzy_param_t Fpid1;
extern const int num_area;
extern float e_membership_values[7];
extern float ec_membership_values[7];
extern float kp_menbership_values[7];
extern float ki_menbership_values[7];
extern float kd_menbership_values[7];
extern int  Kp_rule_list[7][7];
extern int Ki_rule_list[7][7];
extern int  Kd_rule_list[7][7];
//extern int  Fuzzy_rule_list[7][7];
//extern float fuzzyoutput_menbership_values[7];


void FuzzyPID_Init(Fuzzy_param_t * pid);//模糊PID初始化
void Fuzzy_Get_grad_membership(Fuzzy_param_t * pid,float erro,float erro_c);//输入e与de/dt隶属度计算函数
void Fuzzy_GetSumGrad(Fuzzy_param_t * pid);//获取输出增量kp,ki,kd的总隶属度
void Fuzzy_GetOUT(Fuzzy_param_t * pid);//计算输出增量kp,kd,ki对应论域值
float Quantization(float maximum,float minimum,float x);//区间映射函数
float Inverse_quantization(float maximum, float minimum, float qvalues);//反区间映射函数
float Fuzzy_Controller(Fuzzy_param_t * pid,float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, float erro, float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,float erro_pre,float errp_ppre,int type);//模糊PID控制实现函数
int Fuzzy_get_KP_Initial(int type);//得初值函数
int Fuzzy_get_KD_Initial(int type);



#endif /* SRC_APPSW_TRICORE_USER_FUZZYPID_H_ */
