/*
 * My_UART.c
 *
 *  Created on: 2024锟斤拷1锟斤拷15锟斤拷
 *      Author: dduu
 */
#include "My_UART.h"

#define USER_UART UART1

char Txt=0;

uint8 Data_Get[50];
uint8 data_buf[100];
uint8 data_change_flag=0;
unsigned char UART_flag=0;

uint8 gp=10;
uint8 gi=50;
uint8 gd=100;

int uart_flag=0;
uint8 _sta=0;
int kasi=0;

uint16 dlen;
uint16 dcnt;

void UART_ReceiveData(uint8 data)
{
    static uint16 data_len=0; //鏁版嵁闀垮害
    static uint16 data_cnt=0; //宸茬粡鎺ユ敹鍒扮殑鏁版嵁闀垮害
    static uint16 count=0;

    if(_sta==0)
    {
        Data_Get[0]=data;
        if(data==0xAB)
        {
            _sta=1;
            data_len=0;
            data_cnt=0;
        }
    }
    else if(_sta==1)
    {
        _sta=2;
        Data_Get[1]=data;
    }
    else if(_sta==2)
    {
        _sta=3;
        Data_Get[2]=data;
    }
    else if(_sta==3)
    {
        _sta=4;
        Data_Get[3]=data;
    }
    else if(_sta==4)
    {
        Data_Get[4+count++]=data;
        if(count>=2)
        {
            _sta=5;
            count=0;
            data_len=Data_Get[4]+(Data_Get[5]<<8);
            if(data_len>10)
            {

                _sta=0;
            }
            dlen=data_len;
        }
    }
    else if(_sta==5)
    {
        Data_Get[6+data_cnt++]=data;
        dcnt=data_cnt;
        if(data_cnt>=data_len)
        {
            kasi=1;
            _sta=6;
        }
    }
    else if(_sta==6)
    {
        _sta=7;
        Data_Get[6+data_cnt++]=data;
    }
    else if(_sta==7)
    {
        _sta=0;
        kasi=0;
        uart_flag=1;
        Data_Get[6+data_cnt++]=data;
        LED_Ctrl(LED2,RVS);
        Data_Analyse();
    }
}

void Data_Analyse(void)
{
    uint8 sumcheck = 0;
    uint8 addcheck = 0;
    uint16 flen = data_buf[4] + data_buf[5] * 256;
    for(uint16 i=0; i < (flen+6); i++)
    {
        sumcheck += data_buf[i];
        addcheck += sumcheck;
    }

    if(sumcheck != data_buf[flen+6] || addcheck != data_buf[flen+7])
        return;                     //

    if(Data_Get[3]==0xE0)           //
    {
        uint8 cmd=Data_Get[6];
        uint16 id=Data_Get[7]+(Data_Get[8]<<8);
        uint8 sum_check=Data_Get[9];
        uint8 add_check=Data_Get[10];       //9.10

        if(cmd==0x00){              //
            UART_SendPar_Cmd00('X');
        }else if(cmd==0x01){        //
            UART_SendPar_Cmd01(6);  //参数个数
        }else if(cmd==0x02){        //
            switch(id){
                case 0x0000:
                    UART_SendPar_Cmd02(id,Servo_Loc_PID.kp);
                    break;
                case 0x0001:
                    UART_SendPar_Cmd02(id,Servo_Loc_PID.ki);
                    break;
                case 0x0002:
                    UART_SendPar_Cmd02(id,Servo_Loc_PID.kd);
                    break;
                case 0x0003:
                    UART_SendPar_Cmd02(id,Motor_Inc_PID.kp);
                    break;
                case 0x0004:
                    UART_SendPar_Cmd02(id,Motor_Inc_PID.ki);
                    break;
                case 0x0005:
                    UART_SendPar_Cmd02(id,Motor_Inc_PID.kd);
                    break;
            }
        }else if(cmd==0x03){        //
            switch(id){
                case 0x0000:
                    UART_SendPar_Cmd03(id,8,"SEP");
                    break;
                case 0x0001:
                    UART_SendPar_Cmd03(id,8,"SEI");
                    break;
                case 0x0002:
                    UART_SendPar_Cmd03(id,8,"SED");
                    break;
                case 0x0003:
                    UART_SendPar_Cmd03(id,8,"LMP");
                    break;
                case 0x0004:
                    UART_SendPar_Cmd03(id,8,"LMI");
                    break;
                case 0x0005:
                    UART_SendPar_Cmd03(id,8,"LMD");
                    break;
            }
        }else if(cmd==0x10){
            UART_SaveReturn(sum_check,add_check);
            data_change_flag=1;
        }
    }
    else if(Data_Get[3]==0xE1)      //
    {
        uint16 id=Data_Get[6]+(Data_Get[7]<<8);

        unsigned char byteArray[sizeof(float)];
        byteArray[0]=Data_Get[8];
        byteArray[1]=Data_Get[9];
        byteArray[2]=Data_Get[10];
        byteArray[3]=Data_Get[11];
        float val;
        memcpy(&val, byteArray, sizeof(float));

        uint8 sum_check=Data_Get[12];
        uint8 add_check=Data_Get[13];       //

        switch(id)
        {
            case 0x0000:
                Servo_Loc_PID.kp=val;
                UART_WriteReturn(sum_check,add_check);
                break;
            case 0x0001:
                Servo_Loc_PID.ki=val;
                UART_WriteReturn(sum_check,add_check);
                break;
            case 0x0002:
                Servo_Loc_PID.kd=val;
                UART_WriteReturn(sum_check,add_check);
                break;
            case 0x0003:
                Motor_Inc_PID.kp=val;
                UART_WriteReturn(sum_check,add_check);
                break;
            case 0x0004:
                Motor_Inc_PID.ki=val;
                UART_WriteReturn(sum_check,add_check);
                break;
            case 0x0005:
                Motor_Inc_PID.kd=val;
                UART_WriteReturn(sum_check,add_check);
                break;
        }
    }

}


void UART_WriteReturn(uint8 sum,uint8 add)
{
    unsigned char  _cnt=0;

    data_buf[_cnt++] = 0xAB;
    data_buf[_cnt++] = 0xFF;
    data_buf[_cnt++] = 0xFE;
    data_buf[_cnt++] = 0x00;

    data_buf[_cnt++] = 0x03;
    data_buf[_cnt++] = 0x00;

    data_buf[_cnt++]=0xE1;

    data_buf[_cnt++]=sum;

    data_buf[_cnt++]=add;

    uint8 sumcheck = 0;
    uint8 addcheck = 0;

    uint16 flen = data_buf[4] + data_buf[5] * 256;
    for(uint16 i=0; i < (flen+6); i++)
    {
        sumcheck += data_buf[i];
        addcheck += sumcheck;
    }

    data_buf[_cnt++] = sumcheck;
    data_buf[_cnt++] = addcheck;

    UART_PutBuff(USER_UART, data_buf, _cnt);
}

void UART_SendData(float data1, float data2, float data3)
{
  unsigned char  _cnt=0;

  data_buf[_cnt++] = 0xAB;
  data_buf[_cnt++] = 0xFF;
  data_buf[_cnt++] = 0xFE;
  data_buf[_cnt++] = 0xF1;

  data_buf[_cnt++] = 0x0B;
  data_buf[_cnt++] = 0x00;

  unsigned char byteArray1[sizeof(float)];
  memcpy(byteArray1, &data1, sizeof(float));
  unsigned char byteArray2[sizeof(float)];
  memcpy(byteArray2, &data2, sizeof(float));
  unsigned char byteArray3[sizeof(float)];
  memcpy(byteArray3, &data3, sizeof(float));

  data_buf[_cnt++]= byteArray1[0];
  data_buf[_cnt++]= byteArray1[1];
  data_buf[_cnt++]= byteArray1[2];
  data_buf[_cnt++]= byteArray1[3];

  data_buf[_cnt++]= byteArray2[0];
  data_buf[_cnt++]= byteArray2[1];
  data_buf[_cnt++]= byteArray2[2];
  data_buf[_cnt++]= byteArray2[3];

  data_buf[_cnt++]= byteArray3[0];
  data_buf[_cnt++]= byteArray3[1];
  data_buf[_cnt++]= byteArray3[2];
  data_buf[_cnt++]= byteArray3[3];

  uint8 sumcheck = 0;
  uint8 addcheck = 0;

  uint16 flen = data_buf[4] + data_buf[5] * 256;
  for(uint16 i=0; i < (flen+6); i++)
  {
      sumcheck += data_buf[i];
      addcheck += sumcheck;
  }

  //
  data_buf[_cnt++] = sumcheck;
  data_buf[_cnt++] = addcheck;

  UART_PutBuff(USER_UART, data_buf, _cnt);
}


void UART_SaveReturn(uint8 sum,uint8 add)
{
  unsigned char  _cnt=0;

  data_buf[_cnt++] = 0xAB;
  data_buf[_cnt++] = 0xFF;
  data_buf[_cnt++] = 0xFE;
  data_buf[_cnt++] = 0x00;

  data_buf[_cnt++] = 0x03;
  data_buf[_cnt++] = 0x00;
  data_buf[_cnt++] = 0xE0;

  data_buf[_cnt++]=sum;

  data_buf[_cnt++]=add;

  uint8 sumcheck = 0;
  uint8 addcheck = 0;

  uint16 flen = data_buf[4] + data_buf[5] * 256;
  for(uint16 i=0; i < (flen+6); i++)
  {
      sumcheck += data_buf[i];
      addcheck += sumcheck;
  }
    //

  data_buf[_cnt++] = sumcheck;
  data_buf[_cnt++] = addcheck;

  UART_PutBuff(USER_UART, data_buf, _cnt);
}

void UART_SendPar_Cmd00(char name)  //杩斿洖璁惧淇℃伅甯�
{
    unsigned char  _cnt=0;

    data_buf[_cnt++] = 0xAB;
    data_buf[_cnt++] = 0xFF;
    data_buf[_cnt++] = 0xFE;
    data_buf[_cnt++] = 0xE3;       //锟斤拷锟斤拷锟借备帧

    data_buf[_cnt++] = 0x0A;
    data_buf[_cnt++] = 0x00;      //锟斤拷锟斤拷

    data_buf[_cnt++]= 0xFF;       //DEV_ID

    data_buf[_cnt++]= 0x00;
    data_buf[_cnt++]= 0x00;       //HW_VER

    data_buf[_cnt++]= 0x00;
    data_buf[_cnt++]= 0x00;       //SW_VER

    data_buf[_cnt++]= 0x00;
    data_buf[_cnt++]= 0x00;       //BL_VER

    data_buf[_cnt++]= 0x00;
    data_buf[_cnt++]= 0x00;       //PT_VER

    data_buf[_cnt++]= name;        //DEV_NAME

    uint8 sumcheck = 0;
    uint8 addcheck = 0;

    uint16 flen = data_buf[4] + data_buf[5] * 256;
    for(uint16 i=0; i < (flen+6); i++)
    {
        sumcheck += data_buf[i];
        addcheck += sumcheck;
    }

    data_buf[_cnt++] = sumcheck;
    data_buf[_cnt++] = addcheck;

    UART_PutBuff(USER_UART, data_buf, _cnt);
}

void UART_SendPar_Cmd03(uint16 id, uint8 type, char name[4])  //杩斿洖cmd03甯�
{
    unsigned char  _cnt=0;

    data_buf[_cnt++] = 0xAB;
    data_buf[_cnt++] = 0xFF;
    data_buf[_cnt++] = 0xFE;
    data_buf[_cnt++] = 0xE2;

    data_buf[_cnt++] = 0x17;      //涓嶇畻info鐨勮瘽锛�23涓瓧鑺�
    data_buf[_cnt++] = 0x00;

    data_buf[_cnt++]= (uint8)id;
    data_buf[_cnt++]= (uint8)(id>>8);       //PAR_ID

    data_buf[_cnt++]= (uint8)type;          //PAR_TYPE

    data_buf[_cnt++]= name[0];
    data_buf[_cnt++]= name[1];
    data_buf[_cnt++]= name[2];

    for(int i=3;i<20;i++){
        data_buf[_cnt++]= 0x00;              //PAR_NAME
    }

    uint8 sumcheck = 0;
    uint8 addcheck = 0;

    uint16 flen = data_buf[4] + data_buf[5] * 256;
    for(uint16 i=0; i < (flen+6); i++)
    {
        sumcheck += data_buf[i];
        addcheck += sumcheck;
    }

    data_buf[_cnt++] = sumcheck;
    data_buf[_cnt++] = addcheck;

    UART_PutBuff(USER_UART, data_buf, _cnt);
}

void UART_SendPar_Cmd02(uint16 id , float val)  //杩斿洖cmd02甯�
{
    unsigned char  _cnt=0;

    data_buf[_cnt++] = 0xAB;
    data_buf[_cnt++] = 0xFF;
    data_buf[_cnt++] = 0xFE;
    data_buf[_cnt++] = 0xE1;

    data_buf[_cnt++] = 0x06;  //float绫诲瀷
    data_buf[_cnt++] = 0x00;

    data_buf[_cnt++]= (uint8)id;
    data_buf[_cnt++]= (uint8)(id>>8);

    unsigned char byteArray[sizeof(float)];
    memcpy(byteArray, &val, sizeof(float));

    data_buf[_cnt++]= byteArray[0];
    data_buf[_cnt++]= byteArray[1];
    data_buf[_cnt++]= byteArray[2];
    data_buf[_cnt++]= byteArray[3];

    uint8 sumcheck = 0;
    uint8 addcheck = 0;

    uint16 flen = data_buf[4] + data_buf[5] * 256;
    for(uint16 i=0; i < (flen+6); i++)
    {
        sumcheck += data_buf[i];
        addcheck += sumcheck;
    }
//
    data_buf[_cnt++] = sumcheck;
    data_buf[_cnt++] = addcheck;

    UART_PutBuff(USER_UART, data_buf, _cnt);
}

void UART_SendPar_Cmd01(uint16 val)   //杩斿洖cmd01甯�
{
    unsigned char  _cnt=0;

    data_buf[_cnt++] = 0xAB;
    data_buf[_cnt++] = 0xFF;
    data_buf[_cnt++] = 0xFE;
    data_buf[_cnt++] = 0xE0;
    //帧

    data_buf[_cnt++] = 0x03;
    data_buf[_cnt++] = 0x00;

    data_buf[_cnt++]= 0x01;
    data_buf[_cnt++]=(uint8)(val);
    data_buf[_cnt++]=(uint8)(val>>8);   //鍙傛暟涓暟

    uint8 sumcheck = 0;
    uint8 addcheck = 0;

    uint16 flen = data_buf[4] + data_buf[5] * 256;
    for(uint16 i=0; i < (flen+6); i++)
    {
        sumcheck += data_buf[i];
        addcheck += sumcheck;
    }

//
    data_buf[_cnt++] = sumcheck;
    data_buf[_cnt++] = addcheck;

    UART_PutBuff(USER_UART, data_buf, _cnt);
}
