#include <Main.h>

unsigned char UART_SendFlag;

// 定时器 5ms和50ms标志位
volatile uint8 cpu1Flage5ms = 0;
volatile uint8 cpu1Flage50ms = 0;

// 期望速度
volatile sint16 targetSpeed = 10;

// 避障标志位
volatile uint8 evadibleFlage = 0;

int core1_main (void)
{
    // 开启CPU总中断
    IfxCpu_enableInterrupts();

    // 关闭看门狗
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword ());

    // 等待CPU0 初始化完成
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));

    // 所有含有中断的测试都默认在CPU0中执行，如果需要用CPU1请参考龙邱B站视频。
    // 程序配套视频地址：https://space.bilibili.com/95313236
    while(1)//主循环
    {
        if(Camera_Flag == 2)   //图像处理时间大约为2ms
        {

            UART_SendFlag=1;
            Get_Use_Image();             /* 提取部分使用的数据 */
            Image_Frameprocessing();
            Image_SeedGrowing();        //区域生长爬线
            Camera_Flag = 0;             /* 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 */

        }
    }
}
