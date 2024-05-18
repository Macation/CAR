#include <Main.h>

App_Cpu0 g_AppCpu0; // brief CPU 0 global data
IfxCpu_mutexLock mutexCpu0InitIsOk = 1;   // CPU0 初始化完成标志位
volatile char mutexCpu0TFTIsOk=0;         // CPU1 0占用/1释放 TFT

CircularBuffer bf;

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

    while (1)	//主循环
    {
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
        }
        if(Key_Value==5)
        {
            Key_Value=0;
            Motor_openFlag=!Motor_openFlag;
            Servo_openFlag=!Servo_openFlag;
            if(Motor_openFlag && Servo_openFlag)
//            if( Servo_openFlag)
                LED_Ctrl(LED2,ON);
            else
                LED_Ctrl(LED2,OFF);
        }
        //如果PID的数据发生改变,重新向E2PROM中写入数据
        if(data_change_flag==1)
        {
            data_change_flag=0;
            E2PROM_Write_PID();
        }

        char txt[70];
        sprintf(txt, "%.1f,%d,%d,%.1f,%.1f,%d,%d\n",Current_Speed,Current_Speed1,Current_Speed2,Target_Speed,Servo_LocPid,Motor_duty1,Motor_duty2);
        UART_PutStr(UART1,txt);
    }
}


void Init_System(void)
{
    MotorInit();

    ServoInit();  //舵机目前的频率为50Hz，不过看资料最高可以到300Hz，提高控制频率，后续可以试试看

    GPIO_LED_Init();

    GPIO_KEY_Init();

    TFTSPI_Init(0);               // TFT1.8初始化0:横屏  1：竖屏
    TFTSPI_CLS(u16BLACK);         // 清屏

    CAMERA_Init(50);                    //摄像头初始化

    ADC_InitConfig(ADC7, 80000);        //龙邱母板AD7连接着电池

    ENC_InitConfig(ENC2_InPut_P33_7, ENC2_Dir_P33_6); //左轮编码器
    ENC_InitConfig(ENC4_InPut_P02_8, ENC4_Dir_P33_5); //右轮编码器

//    UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
    UART_InitConfig(UART1_RX_P20_9,UART1_TX_P15_0, 115200); //串口初始化

    CCU6_InitConfig(CCU60, CCU6_Channel0, 2*1000);  //每10ms进入一次中断，处理电机舵机的事件
    CCU6_InitConfig(CCU60, CCU6_Channel1, 20*1000);  //每20ms进入一次定时中断中，检测按键状态以及ADC
//    CCU6_InitConfig(CCU61, CCU6_Channel0, 10*1000);  //每10ms进入一次中断，用于摄像头处理元素
//    CCU6_InitConfig(CCU61, CCU6_Channel1, 100*1000);  //每10ms进入一次中断，发送串口数据

    //初始化PID的值
    Set_PID();

    //读取E2PROM中的值
    E2PROM_Read_PID();
}

