/*
 * FuzzyPID.c
 *
 *  Created on: 2024年5月27日
 *      Author: dduu
 */
#include "FuzzyPID.h"

Fuzzy_param_t Fpid1;

const int  num_area = 8; //划分区域个数
float e_membership_values[7] = {-3,-2,-1,0,1,2,3}; //输入e的隶属值
float ec_membership_values[7] = { -3,-2,-1,0,1,2,3 };//输入de/dt的隶属值
float kp_menbership_values[7] = { -3,-2,-1,0,1,2,3 };//输出增量kp的隶属值
float ki_menbership_values[7] = { -3,-2,-1,0,1,2,3 }; //输出增量ki的隶属值
float kd_menbership_values[7] = { -3,-2,-1,0,1,2,3 };  //输出增量kd的隶属值
//float fuzzyoutput_menbership_values[7] = { -3,-2,-1,0,1,2,3 };

int  Kp_rule_list[7][7] = { {PB,PB,PB,PB,PB,PB,PB},     //kp规则表
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PB,PB,PB,PB,PB,PB,PB} };

int  Ki_rule_list[7][7] = { {PB,PB,PB,PB,PB,PB,PB},     //ki规则表
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PB,PB,PB,PB,PB,PB,PB} };

int  Kd_rule_list[7][7] = { {ZO,ZO,ZO,ZO,ZO,ZO,ZO},     //kd规则表
                            {PS,PS,PS,PS,PS,PS,PS},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PB,PB,PB,PB,PB,PB,PB},
                            {PM,PM,PM,PM,PM,PM,PM},
                            {PS,PS,PS,PS,PS,PS,PS},
                            {ZO,ZO,ZO,ZO,ZO,ZO,ZO} };


/*************************************************************************
*  函数名称：void FuzzyPID_Init(Fuzzy_param_t * pid)
*  功能说明：模糊PID初始化
*  参数说明：
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
void FuzzyPID_Init(Fuzzy_param_t * pid)
{
    pid->kp=0;                       //PID参数kp
    pid->ki=0;                       //PID参数ki
    pid->kd=0;                       //PID参数kd
    pid->imax= 100;                   //PID积分最大值
    pid->integrator=0;                //PID积分器

    pid->qdetail_kp=0;               //增量kp对应论域中的值
    pid->qdetail_ki=0;               //增量ki对应论域中的值
    pid->qdetail_kd=0;               //增量kd对应论域中的值

    pid->detail_kp=0;                //输出增量kp
    pid->detail_ki=0;                //输出增量ki
    pid->detail_kd=0;                //输出增量kd
    pid->qerro=0;                    //输入e对应论域中的值
    pid->qerro_c=0;                  //输入de/dt对应论域中的值
    pid->output=0;                   //模糊PID输出
}

/*************************************************************************
*  函数名称：void Fuzzy_Get_grad_membership(Fuzzy_param_t * pid,float erro,float erro_c)
*  功能说明：输入e与de/dt隶属度计算函数
*  参数说明：
* @param    pid     模糊pid参数
* @param    erro   pid输入误差
* @param    erro_c   pid输入误差导数
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
void Fuzzy_Get_grad_membership(Fuzzy_param_t * pid,float erro,float erro_c)
{     int i=0;
        //误差的隶属度
        if(erro > e_membership_values[0] && erro < e_membership_values[6])//在论域范围内
        {
            for(i = 0; i <num_area-2; i++)
            {
                if(erro >= e_membership_values[i] && erro <= e_membership_values[i+1])//在论域中找到隶属的区间
                {
                    //计算隶属度
                    pid->e_gradmembership[0] = -(erro-e_membership_values[i+1])/(e_membership_values[i + 1] - e_membership_values[i]);
                    pid->e_gradmembership[1] = 1+(erro-e_membership_values[i+1])/(e_membership_values[i + 1] - e_membership_values[i]);
                    pid->e_grad_index[0] = i;
                    pid->e_grad_index[1] = i+1;
                    break;
                }
            }
        }
        else
        {       //如果误差小于论域范围或者超过论域范围
             if (erro <= e_membership_values[0])
            {
                pid->e_gradmembership[0] = 1;
                pid->e_gradmembership[1] = 0;
                pid->e_grad_index[0] = 0;
                pid->e_grad_index[1] = -1;
            }
            else if (erro >= e_membership_values[6])
            {
                pid->e_gradmembership[0] = 1;
                pid->e_gradmembership[1] = 0;
                pid->e_grad_index[0] = 6;
                pid->e_grad_index[1] = -1;
            }
        }
        //误差导数的隶属度
        if (erro_c > ec_membership_values[0] && erro_c < ec_membership_values[6])
    {
        for (i = 0; i < num_area - 2; i++)
        {
            if (erro_c >= ec_membership_values[i] && erro_c <= ec_membership_values[i + 1])
            {
                pid->ec_gradmembership[0] = -(erro_c - ec_membership_values[i + 1]) / (ec_membership_values[i + 1] - ec_membership_values[i]);
                pid->ec_gradmembership[1] = 1 + (erro_c - ec_membership_values[i + 1]) / (ec_membership_values[i + 1] - ec_membership_values[i]);
                pid->ec_grad_index[0] = i;
                pid->ec_grad_index[1] = i + 1;
                break;
            }
        }
    }
    else
    {
            //如果误差小于论域范围或者超过论域范围
        if (erro_c <= ec_membership_values[0])
        {
            pid->ec_gradmembership[0] = 1;
            pid->ec_gradmembership[1] = 0;
            pid->ec_grad_index[0] = 0;
            pid->ec_grad_index[1] = -1;
        }
        else if (erro_c >= ec_membership_values[6])
        {
            pid->ec_gradmembership[0] = 1;
            pid->ec_gradmembership[1] = 0;
            pid->ec_grad_index[0] = 6;
            pid->ec_grad_index[1] = -1;
        }
    }
}
/*************************************************************************
*  函数名称：void Fuzzy_GetSumGrad(Fuzzy_param_t * pid)
*  功能说明：获取输出增量kp,ki,kd的总隶属度
*  参数说明：
* @param    pid     模糊pid参数
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
void Fuzzy_GetSumGrad(Fuzzy_param_t * pid)
{
    int i=0;
    int j=0;
    //初始化PID三个参数的总隶属度
    for(i=0; i <7 ; i++)
    {
        pid->KpgradSums[i]=0;
        pid->KigradSums[i]=0;
        pid->KdgradSums[i]=0;
    }
    for(i=0;i<2;i++)
    {
        if(pid->e_grad_index[i] == -1)
        {
            continue;
        }
        for(j=0; j<2;j++)
        {
            if(pid->ec_grad_index[j] != -1)
            {
              int indexKp = Kp_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
              int indexKi = Ki_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;
              int indexKd = Kd_rule_list[pid->e_grad_index[i]][pid->ec_grad_index[j]] + 3;  //+3的目的是将【-3，+3】的区间偏移到数组【0，6】

              pid->KpgradSums[indexKp] = pid->KpgradSums[indexKp] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
              pid->KigradSums[indexKi] = pid->KigradSums[indexKi] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);
              pid->KdgradSums[indexKd] = pid->KdgradSums[indexKd] + (pid->e_gradmembership[i] * pid->ec_gradmembership[j]);  //这里+=的原因是因为可能有多少频率相同对应同一个P,在规则表中对应同一个
             }
             else
             {
              continue;
             }
        }
     }
}
/*************************************************************************
*  函数名称：void Fuzzy_GetOUT(Fuzzy_param_t * pid)
*  功能说明：计算输出增量kp,kd,ki对应论域值
*  参数说明：
* @param    pid     模糊pid参数
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
void Fuzzy_GetOUT(Fuzzy_param_t * pid)
{
      for (int i = 0; i < num_area - 1; i++)
    {
        pid->qdetail_kp += kp_menbership_values[i] * pid->KpgradSums[i];
        pid->qdetail_ki += ki_menbership_values[i] * pid->KigradSums[i];
        pid->qdetail_kd += kd_menbership_values[i] * pid->KdgradSums[i];
    }

}
/*************************************************************************
*  函数名称：float Quantization(float maximum,float minimum,float x)
*  功能说明：区间映射函数
*  参数说明：
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
float Quantization(float maximum,float minimum,float x)
{
    float qvalues= 6.0 *(x-minimum)/(maximum - minimum)-3;
    return qvalues;
}


/*************************************************************************
*  函数名称：float Inverse_quantization(float maximum, float minimum, float qvalues)
*  功能说明：反区间映射函数
*  参数说明：
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
float Inverse_quantization(float maximum, float minimum, float qvalues)
{
    float x = (maximum - minimum) *(qvalues + 3)/6 + minimum;
    return x;
}

/*************************************************************************
*  函数名称：float Fuzzy_Controller(Fuzzy_param_t * pid,float e_max, float e_min,
                            float ec_max, float ec_min, float kp_max, float kp_min, float erro,
                            float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,
                            float erro_pre,float errp_ppre,int type)
*  功能说明：模糊PID控制实现函数
*  参数说明：
* @param    pid     模糊pid参数
* @param    e_max   误差最大值
* @param    e_min   误差最小值
* @param    ec_max  误差导数最大值
* @param    ec_min  误差导数最小值
* @param    kp_max  kp最大值
* @param    kp_min  kp最小值
* @param    erro    误差
* @param    erro_c  误差导数
* @param    ki_max  ki最大值
* @param    ki_min  ki最小值
* @param    kd_max  kd最大值
* @param    kd_min  kd最小值
* @param    erro_pre    上一个误差
* @param    errp_ppre  上上一个误差
* @param    type       初值
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
float Fuzzy_Controller(Fuzzy_param_t * pid,float e_max, float e_min, float ec_max, float ec_min, float kp_max, float kp_min, float erro, float erro_c,float ki_max,float ki_min,float kd_max,float kd_min,float erro_pre,float errp_ppre,int type)
{
    int num=0;
    int num1=0;
    //模糊化
    pid->qerro = Quantization(e_max,e_min,erro);                //映射到论域
    pid->qerro_c = Quantization(ec_max,ec_min,erro_c);          //映射到论域
    Fuzzy_Get_grad_membership(pid,pid->qerro,pid->qerro_c);     //计算隶属度
    Fuzzy_GetSumGrad(pid);                                      //计算PID各个参数总的隶属度
    //模糊推理
    Fuzzy_GetOUT(pid);                                          //获取论域中PID增量
    //反模糊化
    pid->detail_kp = Inverse_quantization(kp_max,kp_min,pid->qdetail_kp);
    pid->detail_ki = Inverse_quantization(ki_max,ki_min,pid->qdetail_ki);
    pid->detail_kd = Inverse_quantization(kd_max,kd_min,pid->qdetail_kd);   //获取各自域下PID增量
    pid->qdetail_kp = 0;
    pid->qdetail_ki = 0;
    pid->qdetail_kd = 0;

    //获取初值
    num=Fuzzy_get_KP_Initial(0);
    num1=Fuzzy_get_KD_Initial(0);
    //计算pid参数
    pid->kp =num+pid->detail_kp;
    pid->ki =pid->detail_ki;
    pid->kd =num1+pid->detail_kd;
    //符号
    if (pid->kp < 0)
        pid->kp = 0;
    if (pid->ki < 0)
        pid->ki = 0;
    if (pid->kd < 0)
        pid->kd = 0;
    pid->detail_kp=0;
    pid->detail_ki=0;
    pid->detail_kd=0;
    pid->integrator+=erro;//积分累加
    //积分限幅
    if(pid->integrator>100)pid->integrator=100;
    else if(pid->integrator<-100)pid->integrator=-100;

    //纯P控制
//    pid->kd=0;

    //pd控制
    pid->kp_error = pid->kp * erro;
    pid->kd_error = pid->kd * erro_c;
    pid->output = pid->kp_error+pid->kd_error;//pd控制
    return pid->output;

}
/*************************************************************************
*  函数名称：int Fuzzy_get_KP_Initial(int type)
*  功能说明：模糊PID获得KP初值函数
*  参数说明：
* @param    type     图像误差或者电磁误差
*  函数返回：无
*  修改时间：2023年2月7日
*  备    注：
*************************************************************************/
int Fuzzy_get_KP_Initial(int type)
{   int num=0;
    if(type==0)
    {
        num=1;
    }
    else {
        num=0;
    }
    return num;
}

/*************************************************************************
*  函数名称：int Fuzzy_get_KD_Initial(int type)
*  功能说明：模糊PID获得Kd初值函数
*  参数说明：
* @param    type     图像误差或者电磁误差
*  函数返回：无
*  修改时间：2023年6月14日
*  备    注：
*************************************************************************/
int Fuzzy_get_KD_Initial(int type)
{
    int num=0;
    if(type==0)
    {
        num=1;
    }
    else {
        num=10;
    }
    return num;
}



