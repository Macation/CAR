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

#include <LQ_DMA.h>
#include <LQ_GPIO.h>
#include <LQ_GPIO_LED.h>
#include <LQ_Inductor.h>
#include <LQ_MotorServo.h>

extern volatile uint8 Game_Over; // 小车完成全部任务，停车

/* GPIO外部中断 */
IFX_INTERRUPT(PIN_INT0_IRQHandler, PIN_INT0_VECTABNUM, PIN_INT0_PRIORITY);
IFX_INTERRUPT(PIN_INT1_IRQHandler, PIN_INT1_VECTABNUM, PIN_INT1_PRIORITY);
IFX_INTERRUPT(PIN_INT2_IRQHandler, PIN_INT2_VECTABNUM, PIN_INT2_PRIORITY);
IFX_INTERRUPT(PIN_INT3_IRQHandler, PIN_INT3_VECTABNUM, PIN_INT3_PRIORITY);

/** 外部中断CPU标号 */
const unsigned char PinIrqVectabNum[4] ={PIN_INT0_VECTABNUM, PIN_INT1_VECTABNUM, PIN_INT2_VECTABNUM, PIN_INT3_VECTABNUM};

/** 外部中断优先级 */
const unsigned char PinIrqPriority[4] = {PIN_INT0_PRIORITY, PIN_INT1_PRIORITY, PIN_INT2_PRIORITY, PIN_INT3_PRIORITY};

/** 外部中断服务函数地址 */
const void *PinIrqFuncPointer[4] = {&PIN_INT0_IRQHandler, &PIN_INT1_IRQHandler, &PIN_INT2_IRQHandler,&PIN_INT3_IRQHandler};

/*************************************************************************
 *  函数名称：PIN_INT0_IRQHandler中断服务函数
 *  功能说明：
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月30日
 *  备    注：外部中断0组管脚 使用的中断服务函数
 *************************************************************************/
void PIN_INT0_IRQHandler (void)
{
    /* �û����� */
    LED_Ctrl(LED1, RVS);        //��ƽ��ת,LED��˸
    Game_Over=1; // С�����ȫ������ͣ��
}

/*************************************************************************
 *  �������ƣ�PIN_INT1_IRQHandler�жϷ�����
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע���ⲿ�ж�1��ܽ� ʹ�õ��жϷ�����
 *************************************************************************/
void PIN_INT1_IRQHandler (void)
{
    /* �û����� */
    LED_Ctrl(LED1, RVS);        //��ƽ��ת,LED��˸
}

/*************************************************************************
 *  �������ƣ�PIN_INT2_IRQHandler�жϷ�����
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע���ⲿ�ж�2��ܽ� ʹ�õ��жϷ�����
 *************************************************************************/
void PIN_INT2_IRQHandler (void)
{
    // �û�����
    LED_Ctrl(LED1, RVS);        //��ƽ��ת,LED��˸
}

/*************************************************************************
 *  �������ƣ�PIN_INT3_IRQHandler�жϷ�����
 *  ����˵����
 *  ����˵������
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��30��
 *  ��    ע���ⲿ�ж�3��ܽ� ʹ�õ��жϷ�����
 *************************************************************************/
void PIN_INT3_IRQHandler (void)
{
    // �û�����
    DMA_CameraStart(PIN_INT2_PRIORITY);

}

/*************************************************************************
 *  �������ƣ�void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
 *  ����˵����GPIO��ʼ������
 *  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���
 *            mode     �� GPIO ģʽ   lq_gpio.h�еĺ궨����ѡ��
 *            output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ���� PIN_InitConfig(P00_0, PIN_MODE_OUTPUT, 1);  //P00_0��ʼ��������� �ߵ�ƽ
 *************************************************************************/
void PIN_InitConfig (GPIO_Name_t pin, IfxPort_Mode mode, unsigned char output)
{

    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* ���� GPIOģʽ */
    IfxPort_setPinMode(port, pinIndex, mode);

    IfxPort_setPinPadDriver(port, pinIndex, IfxPort_PadDriver_cmosAutomotiveSpeed2);

    /* GPIO���ģʽʱ ���״̬ */
    if (0 == output)
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
    }
}

/*************************************************************************
 *  �������ƣ�unsigned char PIN_Read(GPIO_Name_t pin)
 *  ����˵������ȡGPIO��ƽ����
 *  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���*
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����uint8_t pinState = PIN_Read(P00_0);  //��ȡP00_0 �ĵ�ƽ״̬
 *************************************************************************/
unsigned char PIN_Read (GPIO_Name_t pin)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);
    return IfxPort_getPinState(port, pinIndex);
}

/*************************************************************************
 *  �������ƣ�void PIN_Write(GPIO_Name_t pin, unsigned char output)
 *  ����˵��������GPIO���״̬
 *  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���*
 *            output   �� ���ģʽʱ�������ƽ 1���ߵ�ƽ  0���͵�ƽ
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����PIN_Write(P00_0, 1);//P00_0��Ϊ�ߵ�ƽ
 *************************************************************************/
void PIN_Write (GPIO_Name_t pin, unsigned char output)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* GPIO���ģʽʱ ���״̬ */
    if (0 == output)
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
    }
    else
    {
        IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
    }
}

/*************************************************************************
 *  �������ƣ�void PIN_Dir(GPIO_Name_t pin, unsigned char mode)
 *  ����˵��������GPIO �����������
 *  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���
 *            mode     �� GPIO ģʽ   lq_gpio.h�еĺ궨����ѡ��
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����PIN_Dir(P00_0, 1);//P00_0����Ϊ���
 *************************************************************************/
void PIN_Dir (GPIO_Name_t pin, unsigned char mode)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    if (0 == mode)
    {
        /* ���� GPIOģʽ */
        IfxPort_setPinMode(port, pinIndex, PIN_MODE_INPUT);
    }
    else
    {
        /* ���� GPIOģʽ */
        IfxPort_setPinMode(port, pinIndex, PIN_MODE_OUTPUT);
    }
}

/*************************************************************************
 *  �������ƣ�void PIN_Reverse(GPIO_Name_t pin)
 *  ����˵����GPIO��ƽ��ת������ʹ��ǰ���ȳ�ʼ��
 *  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���*
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����PIN_Reverse(P00_0); //��תP00_0�ܽŵĵ�ƽ״̬
 *************************************************************************/
void PIN_Reverse (GPIO_Name_t pin)
{
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    IfxPort_togglePin(port, pinIndex);
}

/*************************************************************************
 *  �������ƣ�void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode)
 *  ����˵����GPIO�ⲿ�жϳ�ʼ��
 *  ����˵����pin      �� �ܽű��    lq_gpio.h�ж���
 *            mode     �� GPIO ģʽ   lq_gpio.h�еĺ궨����ѡ��*
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����PIN_Exti(P20_0, PIN_IRQ_MODE_FALLING);//����P20_0 �½��ش����ж�
 *  ��    ע��TC264ֻ��15���̶���GPIO֧���ⲿ�ж� ��15���̶�GPIO��Ϊ4�飬ÿ��ֻ��ѡ������һ����Ϊ�ⲿ�жϹܽ�ʹ��
 *           0��P15_4  P33_7  P15_5                             1��P14_3  P15_8
 *           2��P10_2  P02_1  P00_4  P20_0  P11_10              3��P10_3  P14_1  P02_0  P20_9  P15_1
 *			�ⲿ�жϷ�������LQ_GPIO.c��  �ж����ȼ�������LQ_GPIO.h�� ���������޸�
 *************************************************************************/
void PIN_Exti (GPIO_Name_t pin, IfxPort_InputMode mode)
{
    int i, j;
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* �ر��ж� */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxScu_Req_In * reqPin = 0;

    /* ���ܽ� �Ƿ��� �̶���17���ܽ� */
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
            {

            }
            else if ((uint32) port == (uint32) (IfxScu_Req_In_pinTable[0][i][j]->pin.port)
                    && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
            {
                reqPin = IfxScu_Req_In_pinTable[0][i][j];
            }
        }
    }

    if (reqPin == 0)
    {
#pragma warning 557         // ���ξ���
        while (1)
            ; /* ��ѡ�����ⲿ�жϹ��ܵĹܽ� */
#pragma warning 557         // �򿪾���
    }

    /* �������������� */
    IfxScuEru_initReqPin(reqPin, mode);

    /* ������������ȷ������ͨ��EICRm */
    IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel) reqPin->channelId;

    /* �����ж�ģʽ */
    IfxScuEru_disableFallingEdgeDetection(inputChannel); /* EICRm.FENn */
    IfxScuEru_disableRisingEdgeDetection(inputChannel); /* EICRm.RENn */
    if (mode == IfxPort_InputMode_pullUp || mode == IfxPort_InputMode_noPullDevice)
    {
        IfxScuEru_enableFallingEdgeDetection(inputChannel); /* EICRm.FENn */
    }
    if (mode == IfxPort_InputMode_pullDown || mode == IfxPort_InputMode_noPullDevice)
    {
        IfxScuEru_enableRisingEdgeDetection(inputChannel); /* EICRm.RENn */
    }
    IfxScuEru_enableAutoClear(inputChannel);

    /* �źŵ�Ŀ�ĵ� */
    IfxScuEru_InputNodePointer triggerSelect = (IfxScuEru_InputNodePointer) inputChannel;
    IfxScuEru_OutputChannel outputChannel = (IfxScuEru_OutputChannel) inputChannel;

//	/* Connecting Matrix, Event Trigger Logic ETL block */
    IfxScuEru_enableTriggerPulse(inputChannel); /* EICRM.EIENN */
    IfxScuEru_connectTrigger(inputChannel, triggerSelect); /* EICRm.INPn */

    /* output channels, OutputGating Unit OGU block */
    IfxScuEru_setFlagPatternDetection(outputChannel, inputChannel, FALSE); /* IGCRm.IPENn */
    IfxScuEru_disablePatternDetectionTrigger(outputChannel); /* IGCRm.GEENn */
    IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive); /* IGCEm.IGPn */

    unsigned char vectabNum = PinIrqVectabNum[(int) inputChannel % 4];
    unsigned char Priority = PinIrqPriority[(int) inputChannel % 4];
    /* service request configuration */
    {
        volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int) outputChannel % 4];
        IfxSrc_init(src, vectabNum, Priority);
        IfxSrc_enable(src);
    }

    /* ��װ�����жϵ��жϷ��� */
    IfxCpu_Irq_installInterruptHandler((void *) PinIrqFuncPointer[(int) inputChannel % 4], Priority);

    /* �����ж� */
    IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
 *  �������ƣ�void PIN_ExtiEnable(GPIO_Name_t pin,  boolean bool)
 *  ����˵����GPIO�ⲿ�ж�ʹ��
 *  ����˵���� pin      �� �ܽű��    lq_gpio.h�ж���
 *            bool     �� �Ƿ����ж�
 *  �������أ���
 *  �޸�ʱ�䣺2020��3��10��
 *  ��    ����PIN_ExtiEnable(P20_0, FALSE);//�ر�P20_0 �ⲿ�ж�
 *  ��    ע��TC264ֻ��15���̶���GPIO֧���ⲿ�ж� ��15���̶�GPIO��Ϊ4�飬ÿ��ֻ��ѡ������һ����Ϊ�ⲿ�жϹܽ�ʹ��
 *           0��P15_4  P33_7  P15_5                             1��P14_3  P15_8
 *           2��P10_2  P02_1  P00_4  P20_0  P11_10              3��P10_3  P14_1  P02_0  P20_9  P15_1
 *			PIN_Exti()������ʼ����Ĭ��GPIO�����ⲿ�жϣ������Ҫ�رգ�����Ե���PIN_ExtiEnable()
 *************************************************************************/
void PIN_ExtiEnable (GPIO_Name_t pin, boolean bool)
{
    int i, j;
    Ifx_P *port = PIN_GetModule(pin);
    unsigned char pinIndex = PIN_GetIndex(pin);

    /* �ر��ж� */
    boolean interruptState = IfxCpu_disableInterrupts();

    IfxScu_Req_In * reqPin = 0;

    /* ���ܽ� �Ƿ��� �̶���17���ܽ� */
    for (i = 0; i < 8; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
            {

            }
            else if ((uint32) port == (uint32) (IfxScu_Req_In_pinTable[0][i][j]->pin.port)
                    && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
            {
                reqPin = IfxScu_Req_In_pinTable[0][i][j];
            }
        }
    }

    if (reqPin == 0)
    {
#pragma warning 557         // ���ξ���
        while (1)
            ; /* ��ѡ�����ⲿ�жϹ��ܵĹܽ� */
#pragma warning 557         // �򿪾���
    }

    /* ������������ȷ������ͨ��EICRm */
    IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel) reqPin->channelId;

    volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int) inputChannel % 4];

    IfxSrc_clearOverrun(src);
    IfxSrc_clearRequest(src);

    if (bool)
    {
        IfxSrc_enable(src);
    }
    else
    {
        IfxSrc_disable(src);
    }

    /* �����ж� */
    IfxCpu_restoreInterrupts(interruptState);
}

