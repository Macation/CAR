/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
��ƽ    ̨�������������ܿƼ�TC264DA���İ�
����    д��ZYF/chiusir
��E-mail  ��chiusir@163.com
������汾��V1.1 ��Ȩ���У���λʹ��������ϵ��Ȩ
�������¡�2020��10��28��
�������Ϣ�ο����е�ַ��
����    վ��http://www.lqist.cn
���Ա����̡�http://longqiu.taobao.com
------------------------------------------------
��dev.env.��AURIX Development Studio1.2.2�����ϰ汾
��Target �� TC264DA/TC264D
��Crystal�� 20.000Mhz
��SYS PLL�� 200MHz
________________________________________________________________
����iLLD_1_0_1_11_0�ײ����,

ʹ�����̵�ʱ�򣬽������û�пո��Ӣ��·����
����CIFΪTC264DA�����⣬�����Ĵ������TC264D
����Ĭ�ϳ�ʼ����EMEM��512K������û�ʹ��TC264D��ע�͵�EMEM_InitConfig()��ʼ��������
������\Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c��164�����ҡ�
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

������8��ת��������ͬ��ת����ÿ��ת��8��ͨ��������Ϊ8/10/12λ��Ĭ��Ϊ��߾���12λ��
ADC�˿�AN������Ϊ����ڣ�������Ϊ����ڣ���ARM��Ƭ����ͬ��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_ADC.h>

/**
 * ADC�Ĵ���
 */
IfxVadc_Adc vadc;

/**
 * ADCת���ṹ��
 */
Ifx_VADC_RES conversionResult;

/**
 * ADCת����
 */
IfxVadc_Adc_Group g_AdcGroup[3];

/**
 * ADCת��ͨ�����ýṹ��
 */
IfxVadc_Adc_Channel g_AdcChannel[28];


/*************************************************************************
*  �������ƣ�void ADC_init(void)
*  ����˵����ADC��ʼ������
*  ����˵����channel  ADCͨ��
*  ����˵����Freq_Hz  ADCͨ������Ƶ��     ע�� ADC������ģ�� ͨ��0-13  16-25  35-49�ֱ�����������ͨ��  ����ʹ��ɨ�������ʽ��������һ��ADCģ��ͬʱ��ʼ����N��ͨ������ADCʵ�ʲ����� = Freq_Hz / N
*  �������أ���
*  �޸�ʱ�䣺2020��5��10��
*  ��    ע��     ע�� adc���������1000000
*************************************************************************/
void ADC_InitConfig(ADC_Channel_t channel, unsigned long Freq_Hz)
{
	//�½�����
	IfxVadc_Adc_Config adcConfig;
	IfxVadc_Adc_initModuleConfig(&adcConfig, &MODULE_VADC);

	adcConfig.supplyVoltage = IfxVadc_LowSupplyVoltageSelect_3V;

	//��ʼ��ģ��
	IfxVadc_Adc_initModule(&vadc, &adcConfig);

	//�½�ADC������
	IfxVadc_Adc_GroupConfig adcGroupConfig;
	IfxVadc_Adc_initGroupConfig(&adcGroupConfig, &vadc);

	if(Freq_Hz > 1000000)
	{
		Freq_Hz = 1000000;
	}

	/* ���ò���ʱ�� */
	adcGroupConfig.inputClass[0].sampleTime = 1.0f/Freq_Hz;
	adcGroupConfig.inputClass[1].sampleTime = 1.0f/Freq_Hz;

	/* ���÷ֱ��� 12λ */
	adcGroupConfig.inputClass[0].resolution = IfxVadc_ChannelResolution_12bit;
	adcGroupConfig.inputClass[1].resolution = IfxVadc_ChannelResolution_12bit;

#pragma warning 547         // ���ξ���
	//�޸���
	adcGroupConfig.groupId = ADC_GetGroup(channel);
#pragma warning default     // �򿪾���

	//��Ҫ˵��:ͨ����ͬһ������Ϊmaster!
	adcGroupConfig.master = adcGroupConfig.groupId;

	//����ȫ���ٲ�����Դ��ÿ������Դ����˳�򴥷�1��4����8·ת��
	adcGroupConfig.arbiter.requestSlotQueueEnabled          = FALSE;

	//��������ģʽ����������3������Դ�е�һ��
	adcGroupConfig.arbiter.requestSlotScanEnabled           = TRUE;

	//����ɨ��ģʽ
	adcGroupConfig.arbiter.requestSlotBackgroundScanEnabled = FALSE;

	//���ñ���ɨ��
	//ʹ�����е���Ϊ "always" ģʽ (�ޱ��ؼ��)
	adcGroupConfig.queueRequest.triggerConfig.gatingMode          = IfxVadc_GatingMode_disabled;
	adcGroupConfig.scanRequest.triggerConfig.gatingMode           = IfxVadc_GatingMode_always;
	adcGroupConfig.backgroundScanRequest.triggerConfig.gatingMode = IfxVadc_GatingMode_disabled;
	//�����Զ�ɨ��ģʽ
	adcGroupConfig.scanRequest.autoscanEnabled = TRUE;

	//��ʼ����
	IfxVadc_Adc_initGroup(&g_AdcGroup[ADC_GetGroup(channel)], &adcGroupConfig);

	//�½�ͨ������
	IfxVadc_Adc_ChannelConfig adcChannelConfig;

	IfxVadc_Adc_initChannelConfig(&adcChannelConfig, &g_AdcGroup[ADC_GetGroup(channel)]);//��ʼ��
	adcChannelConfig.channelId = (IfxVadc_ChannelId)(ADC_GetChannel(channel));           //ͨ��IDѡ��
	adcChannelConfig.resultRegister = (IfxVadc_ChannelResult)(ADC_GetChannel(channel));  //��Ӧͨ��ת����������ѡ��

	//ʹ��ר�ý���Ĵ�����ʼ��ͨ��
	IfxVadc_Adc_initChannel(&g_AdcChannel[ADC_GetIndex(channel)], &adcChannelConfig);
	//�����Զ�ɨ��
	unsigned channels = (1 << adcChannelConfig.channelId);
	unsigned mask = channels;
	IfxVadc_Adc_setScan(&g_AdcGroup[ADC_GetGroup(channel)], channels, mask);

	//��ʼ�Զ�ɨ��
	IfxVadc_Adc_startScan(&g_AdcGroup[ADC_GetGroup(channel)]);

}

/*************************************************************************
*  �������ƣ�unsigned short ADC_Read(ADC_Channel_t  chn)
*  ����˵����ADC�ɼ�
*  ����˵����chn    : ADCͨ��
*  �������أ�ADC��ֵ
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned short ADC_Read(ADC_Channel_t  chn)
{
	//�ȴ�ת�����
	do{
		conversionResult = IfxVadc_Adc_getResult(&g_AdcChannel[ADC_GetIndex(chn)]);
	} while( !conversionResult.B.VF );

	return 	conversionResult.B.RESULT;//����ת�����
}

/*************************************************************************
*  �������ƣ�unsigned short ADC_Read(ADC_Channel_t  chn)
*  ����˵����ADC��βɼ�ȡƽ��ֵ
*  ����˵����chn    : ADCͨ��
*  ����˵����count  : �ɼ�����
*  �������أ�ADC��ֵ
*  �޸�ʱ�䣺2020��3��10��
*  ��    ע��
*************************************************************************/
unsigned short ADC_ReadAverage(ADC_Channel_t  chn, unsigned char count)
{
	unsigned short i = 0;
	unsigned long sum = 0;
	for(i = 0; i < count; i++)
	{
		sum += ADC_Read(chn);
	}
	return 	(unsigned short)(sum/count);//����ת�����
}
/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////��/////////////////////////////////////////////////

