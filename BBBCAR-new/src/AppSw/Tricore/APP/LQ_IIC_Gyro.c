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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu_IntrinsicsTasking.h>
#include "LQ_IIC_Gyro.h"
#include <stdio.h>
#include "../Driver/LQ_SOFTI2C.h"
#include "../Driver/LQ_STM.h"
#include "LQ_OLED096.h"
//#include "../User/LQ_AnoScope.h"
#include "LQ_TFT18.h"
#include "LQ_ICM20602.h"
#include "LQ_MPU6050_DMP.h"
#include "LQ_UART.h"

#define OLED  0                 //ѡ����Ļ0��TFT��      1:OLED��
#define MPU6050_ADDR  0x68  //IICд��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

unsigned char IIC_ICM20602 = 0;
unsigned char IIC_ICM20689 = 0;
unsigned char IIC_MPU9250 = 0;
unsigned char IIC_ICM42605 = 0;
unsigned char IIC_MPU6050 = 0;
unsigned char IIC_errorid = 0;

unsigned char Gyro_ID = 0;



/**
  * @brief   ��ȡ�����ǵ��豸ID
  *
  * @param   void
  *
  * @retur   void
  *
  * @note
  *
  * @see      Gyro_Chose();
  *
  * @date     2021/7/12 ����һ
  */
unsigned char Gyro_Chose(void)
{
    Gyro_ID = MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);
    switch(Gyro_ID)
    {
        case 0x12:IIC_ICM20602 = 1;break;
        case 0x71:IIC_MPU9250 = 1;break;
        case 0x98:IIC_ICM20689 = 1;break;
        case 0x42:IIC_ICM42605 = 1;break;
        case 0x68:IIC_MPU6050 = 1;break;
        default :IIC_errorid = 1;return 0;
    }
    return Gyro_ID;
}


signed short  aacx,aacy,aacz;            //���ٶȴ�����ԭʼ����
signed short  gyrox,gyroy,gyroz;         //������ԭʼ����
short magx, magy, magz;
/**
  * @brief    ��ȡMPU6050����ICM20602ԭʼ���� ����
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @see      Test_MPU6050();
  *
  * @date     2019/6/12 ������
  */
void Test_IIC_Gyro(void)
{
    unsigned char res;
    char  txt[30];


    IIC_Init();                  //IIC��ʼ��//IIC��ʼ��
    delayms(100);
    res = Gyro_Chose();                //�ж��������ͺ�
    sprintf(txt,"ID:0X%2x",res);

#if OLED

    OLED_Init();                //LCD��ʼ��
    delayms(100);
    OLED_CLS();                 //LCD����
    OLED_P6x8Str(70,2,txt);     // �ַ�����ʾ
    if(IIC_ICM20602)
        OLED_P8x16Str(0,0,"LQ 20602 Test");// �ַ�����ʾ
    if(IIC_MPU6050)
        OLED_P8x16Str(0,0,"LQ 6050 Test");// �ַ�����ʾ
    if(IIC_ICM42605)
        OLED_P8x16Str(0,0,"LQ 42605 Test");// �ַ�����ʾ
    if(IIC_MPU9250)
        OLED_P8x16Str(0,0,"LQ 9250 Test");// �ַ�����ʾ
    if(IIC_ICM20689)
        OLED_P8x16Str(0,0,"LQ 20689 Test");// �ַ�����ʾ
    if(IIC_errorid)
    {
        OLED_P8x16Str(0,0,"Gyro Test fail");// �ַ�����ʾ
        while(1);
    }

#else
    TFTSPI_Init(1);         //��Ļ��ʼ��Ϊ����
    TFTSPI_CLS(u16BLACK);   // ����
    if(IIC_MPU9250){
        sprintf(txt,"0X%2x",res);
        TFTSPI_P8X16Str(10,8,"ID:",u16RED,u16BLACK);
        TFTSPI_P8X16Str(12,9,txt,u16RED,u16BLACK);// �ַ�����ʾ
    }else
        TFTSPI_P8X16Str(9,9,txt,u16RED,u16BLACK);// �ַ�����ʾ
    if (IIC_ICM20602)
        TFTSPI_P8X16Str(2,0,"LQ 20602 Test",u16RED,u16BLACK);// �ַ�����ʾ
    if (IIC_MPU6050)
        TFTSPI_P8X16Str(2,0,"LQ 6050 Test",u16RED,u16BLACK);// �ַ�����ʾ
    if (IIC_ICM42605)
        TFTSPI_P8X16Str(2,0,"LQ 42605 Test",u16RED,u16BLACK);// �ַ�����ʾ
    if (IIC_MPU9250)
        TFTSPI_P8X16Str(2,0,"LQ 9250 Test",u16RED,u16BLACK);// �ַ�����ʾ
    if (IIC_ICM20689)
        TFTSPI_P8X16Str(2,0,"LQ 20689 Test",u16RED,u16BLACK);// �ַ�����ʾ
    if (IIC_errorid)
    {
        TFTSPI_P8X16Str(1,0,"Gyro Test fail",u16RED,u16BLACK);// �ַ�����ʾ
        while(1);
    }
#endif
    MPU6050_Init();
    while(1)
    {
        MPU_Get_Raw_data(&aacx,&aacy,&aacz,&gyrox,&gyroy,&gyroz);   //�õ����ٶȴ���������
        if (IIC_MPU9250)
            MPU9250_Get_Magnetometer(&magx, &magy, &magz);

#if OLED
        sprintf((char*)txt,"ax:%06d",aacx);
        OLED_P6x8Str(0,2,txt);
        sprintf((char*)txt,"ay:%06d",aacy);
        OLED_P6x8Str(0,3,txt);
        sprintf((char*)txt,"az:%06d",aacz);
        OLED_P6x8Str(0,4,txt);
        sprintf((char*)txt,"gx:%06d",gyrox);
        OLED_P6x8Str(0,5,txt);
        sprintf((char*)txt,"gy:%06d",gyroy);
        OLED_P6x8Str(0,6,txt);
        sprintf((char*)txt,"gz:%06d",gyroz);
        OLED_P6x8Str(0,7,txt);
        //ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,gyroz,gyroz);

#else
        sprintf((char*)txt,"ax:%06d",aacx);
        TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);// �ַ�����ʾ
        sprintf((char*)txt,"ay:%06d",aacy);
        TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);// �ַ�����ʾ
        sprintf((char*)txt,"az:%06d",aacz);
        TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLACK);// �ַ�����ʾ
        sprintf((char*)txt,"gx:%06d",gyrox);
        TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLACK);// �ַ�����ʾ
        sprintf((char*)txt,"gy:%06d",gyroy);
        TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLACK);// �ַ�����ʾ
        sprintf((char*)txt,"gz:%06d",gyroz);
        TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLACK);// �ַ�����ʾ

        if (IIC_MPU9250)
        {
            sprintf((char*)txt,"mx:%06d",magx);
            TFTSPI_P8X16Str(0,7,txt,u16RED,u16BLACK);
            sprintf((char*)txt,"my:%06d",magy);
            TFTSPI_P8X16Str(0,8,txt,u16RED,u16BLACK);
            sprintf((char*)txt,"mz:%06d",magz);
            TFTSPI_P8X16Str(0,9,txt,u16RED,u16BLACK);
        }
#endif
        //ANO_DT_send_int16byte16(aacx,aacy,aacz,gyrox,gyroy,gyroz,gyroz,gyroz);
        delayms(100);
    }

}

/**
  * @brief    ����ȷ��ʱ
  *
  * @param    ��
  *
  * @return   ��
  *
  * @note     ��
  *
  * @see      delayms_mpu(100);
  *
  * @date     2019/4/22 ����һ
*/
void delayms_mpu(uint16 ms)
{
    while(ms--)
    {
        uint16  i = 300;
        while(i--)
        {
            NOP(50);
        }
    }
}



/**
  * @brief    ��ʼ��MPU6050 ���� ICM20602
  *
  * @param    ��
  *
  * @return   0����ʼ���ɹ�   1��ʧ��
  *
  * @note     ʹ��ǰ�ȳ�ʼ��IIC�ӿ�
  *
  * @see      MPU6050_Init();
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU6050_Init(void)
{
    int  res;
    if (IIC_ICM42605)
    {
//        MPU_Write_Byte(MPU6050_ADDR, device_config_reg,bit_soft_reset_chip_config);//chip soft reset
//        delayms(10);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x01);//Change to bank 1
//            MPU_Write_Byte(MPU6050_ADDR,intf_config4,0x03);//4 wire spi mode  ����SPI  Ĭ��ֵ0x03
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x00);
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config_reg,0x40);//Stream-to-FIFO Mode  ������Y���ݲ��˲�����Ƶ��ѡ��
//
//            res = MPU_Read_Byte(MPU6050_ADDR,int_source0_reg);
//            MPU_Write_Byte(MPU6050_ADDR,int_source0_reg,0x00);
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config2_reg,0x00);// watermark
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config3_reg,0x02);// watermark
//            MPU_Write_Byte(MPU6050_ADDR,int_source0_reg, (unsigned char)res);
//            MPU_Write_Byte(MPU6050_ADDR,fifo_config1_reg,0x63);// Enable the accel and gyro to the FIFO
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel,0x00);
//            MPU_Write_Byte(MPU6050_ADDR,int_config_reg,0x36);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = (MPU_Read_Byte(MPU6050_ADDR,int_source0_reg)|bit_int_fifo_ths_int1_en);
//            MPU_Write_Byte(MPU6050_ADDR,int_source0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,accel_config0_reg)&0x1F)|(bit_accel_ui_fs_sel_8g));//8g
//            MPU_Write_Byte(MPU6050_ADDR,accel_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,accel_config0_reg)&0xF0)|bit_accel_odr_50hz);
//            MPU_Write_Byte(MPU6050_ADDR,accel_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,gyro_config0_reg)&0x1F)|(bit_gyro_ui_fs_sel_1000dps));
//            MPU_Write_Byte(MPU6050_ADDR,gyro_config0_reg,(unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = ((MPU_Read_Byte(MPU6050_ADDR,gyro_config0_reg)&0xF0)|bit_gyro_odr_50hz);
//            MPU_Write_Byte(MPU6050_ADDR,gyro_config0_reg, (unsigned char)res);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = MPU_Read_Byte(MPU6050_ADDR,pwr_mgmt0_reg)|(bit_accel_mode_ln); // Accel on in LNM
//            MPU_Write_Byte(MPU6050_ADDR,pwr_mgmt0_reg, (unsigned char)res);
//            delayms(1);
//
//            MPU_Write_Byte(MPU6050_ADDR,reg_bank_sel, 0x00);
//            res = MPU_Read_Byte(MPU6050_ADDR,pwr_mgmt0_reg)|(bit_gyro_mode_ln); // Gyro on in LNM
//            MPU_Write_Byte(MPU6050_ADDR,pwr_mgmt0_reg,(unsigned char)res);
            delayms(1);
            return 0;
    }
    else
    {
        res = MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);           //��ȡMPU6050��ID
        if(res != Gyro_ID)                                 //����ID��ȷ
            return 1;

        res = 0;
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X80);//��λMPU6050
        delayms_mpu(100);  //��ʱ100ms
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X00);//����MPU6050
        res += MPU_Set_Gyro_Fsr(3);                                //�����Ǵ�����,��2000dps
        res += MPU_Set_Accel_Fsr(1);                               //���ٶȴ�����,��4g
        res += MPU_Set_Rate(1000);                                 //���ò�����1000Hz
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,0x02);      //�������ֵ�ͨ�˲���   98hz
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_INT_EN_REG,0X00);   //�ر������ж�
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_USER_CTRL_REG,0X00);//I2C��ģʽ�ر�
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT1_REG,0X01);//����CLKSEL,PLL X��Ϊ�ο�
        res += MPU_Write_Byte(MPU6050_ADDR,MPU_PWR_MGMT2_REG,0X00);//���ٶ��������Ƕ�����

        if(IIC_MPU9250)
        {
            MPU_Write_Byte(MPU6050_ADDR,MPU_FIFO_EN_REG,0X00);  //�ر�FIFO
            MPU_Write_Byte(MPU6050_ADDR,MPU_INTBP_CFG_REG,0X82);//INT���ŵ͵�ƽ��Ч������bypassģʽ
            res = MPU_Read_Byte(AK8963_ADDR,MPU_MAG_WIA);   //��ȡMPU9250��ID
            if(res == AK8963_ID)                                //����ID��ȷ
            {
                MPU_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL2_REG,0X01);//�������
                delayms_mpu(100);
                MPU_Write_Byte(AK8963_ADDR,MPU_MAG_CNTL1_REG,0X16);//����Ϊ����ģʽ�����16bit
            }
        }
        return 0;
    }
}

/**
  * @brief    ���������ǲ�����Χ
  *
  * @param    fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
  *
  * @return   0 �����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_Gyro_Fsr(3);        //�����Ǵ�����,��2000dps
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_Gyro_Fsr(unsigned char fsr)
{
    return MPU_Write_Byte(MPU6050_ADDR,MPU_GYRO_CFG_REG,fsr<<3);
}

/**
  * @brief    ���ü��ٶȼƲ�����Χ
  *
  * @param    fsr:0,��2g;1,��4g;2,��8g;3,��16g
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_Accel_Fsr(1);       //���ٶȴ�����,��4g
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_Accel_Fsr(unsigned char fsr)
{
    return MPU_Write_Byte(MPU6050_ADDR,MPU_ACCEL_CFG_REG,fsr<<3);
}

/**
  * @brief    �������ֵ�ͨ�˲�
  *
  * @param    lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_LPF(100);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_LPF(uint16 lpf)
{
    unsigned char  dat=0;
    if(lpf>=188)dat=1;
    else if(lpf>=98)dat=2;
    else if(lpf>=42)dat=3;
    else if(lpf>=20)dat=4;
    else if(lpf>=10)dat=5;
    else dat=6;
    return MPU_Write_Byte(MPU6050_ADDR,MPU_CFG_REG,dat);//�������ֵ�ͨ�˲���
}

/**
  * @brief    ���ò�����
  *
  * @param    rate:4~1000(Hz)
  *
  * @return   0�����óɹ�
  *
  * @note     ��
  *
  * @see      MPU_Set_Rate(1000);              //���ò�����1000Hz
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Set_Rate(uint16 rate)
{
    unsigned char  dat;
    if(rate>1000)rate=1000;
    if(rate<4)rate=4;
    dat=1000/rate-1;
    MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,dat);       //�������ֵ�ͨ�˲���
    return MPU_Set_LPF(rate/2);                                 //�Զ�����LPFΪ�����ʵ�һ��
}

/**
  * @brief    ��ȡ�¶�ֵ
  *
  * @param    ��
  *
  * @return   �¶�ֵ(������100��)
  *
  * @note     ��
  *
  * @see      signed short temp = MPU_Get_Temperature();
  *
  * @date     2019/6/12 ������
  */
short MPU_Get_Temperature(void)
{
    unsigned char  buf[2];
    short raw;
    float temp;
    MPU_Read_Len(MPU6050_ADDR,MPU_TEMP_OUTH_REG,2,buf);
    raw=((uint16)buf[0]<<8)|buf[1];
    temp=21+((double)raw)/333.87;
    return (short)temp*100;
}

/**
  * @brief    ��ȡ������ֵ
  *
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU_Get_Gyroscope(&data[0], &data[1], &data[2]);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Get_Gyroscope(signed short *gx,signed short *gy,signed short *gz)
{
    unsigned char  buf[6],res;
    res=MPU_Read_Len(MPU6050_ADDR,MPU_GYRO_XOUTH_REG,6,buf);
    if(res==0)
    {
        *gx=((uint16)buf[0]<<8)|buf[1];
        *gy=((uint16)buf[2]<<8)|buf[3];
        *gz=((uint16)buf[4]<<8)|buf[5];
    }
    return res;
}

/**
  * @brief    ��ȡ���ٶ�ֵ
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[3];
  * @see      MPU_Get_Accelerometer(&data[0], &data[1], &data[2]);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Get_Accelerometer(signed short *ax,signed short *ay,signed short *az)
{
    unsigned char  buf[6],res;
    res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,6,buf);
    if(res==0)
    {
        *ax=((uint16)buf[0]<<8)|buf[1];
        *ay=((uint16)buf[2]<<8)|buf[3];
        *az=((uint16)buf[4]<<8)|buf[5];
    }
    return res;
}

/**
  * @brief    ��ȡ ���ٶ�ֵ ���ٶ�ֵ
  *
  * @param    ax,ay,az:������x,y,z���ԭʼ����(������)
  * @param    gx,gy,gz:������x,y,z���ԭʼ����(������)
  *
  * @return   0����ȡ�ɹ�
  *
  * @note     ��
  *
  * @see      signed short data[6];
  * @see      ICM_Get_Raw_data(&data[0], &data[1], &data[2],&data[3], &data[4], &data[5]);
  *
  */
unsigned char MPU_Get_Raw_data(signed short *ax,signed short *ay,signed short *az,signed short *gx,signed short *gy,signed short *gz)
{
    if(IIC_ICM42605)
    {
        unsigned char  buf[14],res;
        res=MPU_Read_Len(MPU6050_ADDR,0X1F,12,buf);
        if(res==0)
        {
            *ax=((uint16)buf[0]<<8)|buf[1];
            *ay=((uint16)buf[2]<<8)|buf[3];
            *az=((uint16)buf[4]<<8)|buf[5];
            *gx=((uint16)buf[6]<<8)|buf[7];
            *gy=((uint16)buf[8]<<8)|buf[9];
            *gz=((uint16)buf[10]<<8)|buf[11];
        }
        return res;
    }
    else
    {
        unsigned char  buf[14],res;

        res=MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
        if(res==0)
        {
            *ax=((uint16)buf[0]<<8)|buf[1];
            *ay=((uint16)buf[2]<<8)|buf[3];
            *az=((uint16)buf[4]<<8)|buf[5];
            *gx=((uint16)buf[8]<<8)|buf[9];
            *gy=((uint16)buf[10]<<8)|buf[11];
            *gz=((uint16)buf[12]<<8)|buf[13];
        }
        return res;
    }
}

/**
* @brief    ��ȡ������ֵ
*
* @param    mx,my,mz:��������x,y,z���ԭʼ����(������)
*
* @return   0����ȡ�ɹ�
*
* @note     ��
*
* @see      signed short data[3];
* @see      MPU9250_Get_Magnetometer(&data[0], &data[1], &data[2]);
*
* @date     2020/12/15 ���ڶ�
*/
unsigned char MPU9250_Get_Magnetometer(short *mx,short *my,short *mz)
{

  unsigned char  buf[6],res;
  res=MPU_Read_Len(AK8963_ADDR,MPU_MAG_XOUTL_REG,6,buf);
  if(res==0)
  {
    *mx=((uint16)buf[1]<<8)|buf[0];
    *my=((uint16)buf[3]<<8)|buf[2];
    *mz=((uint16)buf[5]<<8)|buf[4];
  }
  MPU_Read_Byte(AK8963_ADDR,MPU_MAG_ST2_REG);
  return res;
}

/**
  * @brief    IIC ������
  *
  * @param    addr:������ַ
  * @param    reg :Ҫ��ȡ�ļĴ�����ַ
  * @param    len :Ҫ��ȡ�ĳ���
  * @param    buf :��ȡ�������ݴ洢��
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      unsigned char buf[14];
  * @see      MPU_Read_Len(MPU6050_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}

/**
  * @brief    IIC ����д
  *
  * @param    addr:������ַ
  * @param    reg :Ҫд��ļĴ�����ַ
  * @param    len :Ҫд��ĳ���
  * @param    buf :д�뵽�����ݴ洢��
  *
  * @return   0 ��д��ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      unsigned char buf[14];
  * @see      MPU9250_Write_Len(MPU9250_ADDR,MPU_ACCEL_XOUTH_REG,14,buf);
  *
  * @date     2021/7/1 ������
  */
unsigned char MPU_Write_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_WriteMultByteToSlave(addr<<1, reg, len, buf);
}

/**
  * @brief    IIC дһ���Ĵ���
  *
  * @param    addr  :������ַ
  * @param    reg   :�Ĵ�����ַ
  * @param    value :Ҫд���ֵ
  *
  * @return   0 ����ȡ�ɹ�
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      MPU_Write_Byte(MPU6050_ADDR,MPU_SAMPLE_RATE_REG,1);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);
}

/**
  * @brief    IIC ��һ���Ĵ���
  *
  * @param    addr  :������ַ
  * @param    reg   :�Ĵ�����ַ
  *
  * @return   ��ȡ��ֵ
  *
  * @note     �ײ����� ��ֲʱ��Ҫ�޸�
  *
  * @see      MPU_Read_Byte(MPU6050_ADDR,WHO_AM_I);
  *
  * @date     2019/6/12 ������
  */
unsigned char MPU_Read_Byte(unsigned char addr,unsigned char reg)
{
    unsigned char value[1];
    MPU_Read_Len(addr, reg, 1, value);
    return value[0];
}

void Test_LQ6050_DMP(void)
{
  char txt[30];
  IIC_Init();
  TFTSPI_Init(1);               // TFT2.0��ʼ��0:����  1������
  TFTSPI_CLS(u16BLACK);         // ����
  TFTSPI_P8X16Str(2,0,"LQ 6050 Test", u16RED, u16BLACK);
//  UART_PutStr(UART1, "LQ MPU6050 Test\r\n");
  LQ_DMP_Init();


  while(1)
  {

    LQ_DMP_Read();

    sprintf((char*)txt,"Pitch:%.02f", Pitch);
    TFTSPI_P8X16Str(0,1,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"Roll:%.02f", Roll);
    TFTSPI_P8X16Str(0,2,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"YAW:%.02f", Yaw);
    TFTSPI_P8X16Str(0,3,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"ax:%06d",accel[0]);
    TFTSPI_P8X16Str(0,4,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"ay:%06d",accel[1]);
    TFTSPI_P8X16Str(0,5,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"az:%06d",accel[2]);
    TFTSPI_P8X16Str(0,6,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"gx:%06d",gyro[0]);
    TFTSPI_P8X16Str(0,7,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"gy:%06d",gyro[1]);
    TFTSPI_P8X16Str(0,8,txt,u16RED,u16BLACK);
    sprintf((char*)txt,"gz:%06d",gyro[2]);
    TFTSPI_P8X16Str(0,9,txt,u16RED,u16BLACK);
  }
}




