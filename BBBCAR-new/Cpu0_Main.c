#include <Main.h>

App_Cpu0 g_AppCpu0; // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0占用/1释放 TFT

sint16 VBAT;
uint8 rx_data;


/* 母板修改
 * 按键从上往下为1~5，按键1：22.3，按键2：22.1，按键3：22.2，按键4：22.0，按键5：33.12
 * 拨码开关，左边：33.8，右边：33.9
 * 舵机，左边舵机2：33.10，右边舵机1：33.13
 * 编码器2,2A:02.8,2B:33.5
 * 电机，MT1 N:32.4,MT1 P:23.1,MT2 N:21.3,MT2 P:21.2
 * */
int core0_main (void)
{
    // 关闭CPU总中断
    IfxCpu_disableInterrupts();

    // 关闭看门狗，如果不设置看门狗喂狗需要关闭
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    // 读取总线频率
    g_AppCpu0.info.pllFreq = IfxScuCcu_getPllFrequency();
    g_AppCpu0.info.cpuFreq = IfxScuCcu_getCpuFrequency(IfxCpu_getCoreIndex());
    g_AppCpu0.info.sysFreq = IfxScuCcu_getSpbFrequency();
    g_AppCpu0.info.stmFreq = IfxStm_getFrequency(&MODULE_STM0);

    //初始化外设
    Init_System();
    // 开启CPU总中断
    IfxCpu_enableInterrupts();

    // 通知CPU1，CPU0初始化完成
    IfxCpu_releaseMutex(&mutexCpu0InitIsOk);
    // 切记CPU0,CPU1...不可以同时开启屏幕显示，否则冲突不显示
    mutexCpu0TFTIsOk=0;         // CPU1： 0占用/1释放 TFT

    int flag=0; //页面切换标志
    while (1)   //主循环
    {
        Key5_Handle();

        //跑车模式
        if(Motor_openFlag==1 && Servo_openFlag==1)
        {
            LED_Ctrl(LED2,ON);
            LED_Ctrl(LED3,ON);
        }
        else
        {
            LED_Ctrl(LED2,OFF);
            LED_Ctrl(LED3,OFF);
        }

        //拨码开关向上为0（ON），向下为1（OFF）
        if(KEY_Read(DSW0)==0 && KEY_Read(DSW1)==0)   //菜单模式
        {
            if(!flag){switch_flag=1;flag=1;}
            Button_Scan();
            Menu_Scan();
        }
        else if(KEY_Read(DSW0)==0 && KEY_Read(DSW1)==1)  //按键调参模式
        {
            if(flag){switch_flag=1;flag=0;}
            Modify_PID();
            Limit_PID();
        }
        //如果PID的数据发生改变,重新向E2PROM中写入数据
        if(data_change_flag==1)
        {
            data_change_flag=0;
            if(sp_change!=1)
            {
                E2PROM_Write_PID();
            }
            else
            {
                sp_change=0;
            }
        }
        //光电开关，当距离较近时，关闭电机舵机
//         if(PIN_Read(P15_8)==0)
//         {
//             Motor_openFlag=0;
//             Servo_openFlag=0;
//             MotorCtrl(0,0);
//             ATOM_PWM_InitConfig(ATOMSERVO1, Servo_Center_Mid, 100);
//         }

//            UART_Send_Code();
//            //图像到来时，将图像发送至上位机
//            if(UART_SendFlag)
//            {
//                UART_SendFlag=0;
//                sendimg_binary(Image_Line,160,120,0);
//            }

    }
}


