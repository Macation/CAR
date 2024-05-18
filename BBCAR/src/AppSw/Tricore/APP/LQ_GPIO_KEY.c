/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
銆愬钩    鍙般�戝寳浜緳閭辨櫤鑳界鎶�TC264DA鏍稿績鏉�
銆愮紪    鍐欍�慫YF/chiusir
銆怑-mail  銆慶hiusir@163.com
銆愯蒋浠剁増鏈�慥1.1 鐗堟潈鎵�鏈夛紝鍗曚綅浣跨敤璇峰厛鑱旂郴鎺堟潈
銆愭渶鍚庢洿鏂般��2020骞�10鏈�28鏃�
銆愮浉鍏充俊鎭弬鑰冧笅鍒楀湴鍧�銆�
銆愮綉    绔欍�慼ttp://www.lqist.cn
銆愭窐瀹濆簵閾恒�慼ttp://longqiu.taobao.com
------------------------------------------------
銆恉ev.env.銆慉URIX Development Studio1.2.2鍙婁互涓婄増鏈�
銆怲arget 銆� TC264DA/TC264D
銆怌rystal銆� 20.000Mhz
銆怱YS PLL銆� 200MHz
________________________________________________________________
鍩轰簬iLLD_1_0_1_11_0搴曞眰绋嬪簭,

浣跨敤渚嬬▼鐨勬椂鍊欙紝寤鸿閲囩敤娌℃湁绌烘牸鐨勮嫳鏂囪矾寰勶紝
闄や簡CIF涓篢C264DA鐙湁澶栵紝鍏跺畠鐨勪唬鐮佸吋瀹筎C264D
鏈簱榛樿鍒濆鍖栦簡EMEM锛�512K锛屽鏋滅敤鎴蜂娇鐢═C264D锛屾敞閲婃帀EMEM_InitConfig()鍒濆鍖栧嚱鏁般��
宸ョ▼涓媆Libraries\iLLD\TC26B\Tricore\Cpu\CStart\IfxCpu_CStart0.c绗�164琛屽乏鍙炽��
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_GPIO.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>

/*************************************************************************
*  鍑芥暟鍚嶇О锛歷oid GPIO_KEY_Init(void)
*  鍔熻兘璇存槑锛欸PIO鍒濆鍖栧嚱鏁�
*  鍙傛暟璇存槑锛氭棤
*  鍑芥暟杩斿洖锛氭棤
*  淇敼鏃堕棿锛�2020骞�3鏈�10鏃�
*  澶�    娉細
*************************************************************************/
void GPIO_KEY_Init(void)
{
	  // 鍒濆鍖�,杈撳叆鍙ｏ紝楂樼數骞�
	  PIN_InitConfig(KEY0p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(KEY1p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(KEY2p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(KEY3p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(KEY4p, PIN_MODE_INPUT, 1);

	  PIN_InitConfig(DSW0p, PIN_MODE_INPUT, 1);
	  PIN_InitConfig(DSW1p, PIN_MODE_INPUT, 1);
}
/*************************************************************************
*  鍑芥暟鍚嶇О锛歷oid Reed_Init(void)
*  鍔熻兘璇存槑锛氬共绨хGPIO鍙婁腑鏂垵濮嬪寲鍑芥暟
*  鍙傛暟璇存槑锛氭棤
*  鍑芥暟杩斿洖锛氭棤
*  淇敼鏃堕棿锛�2020骞�11鏈�21鏃�
*  澶�    娉細   涓柇鍦ㄩ偅涓牳鍒濆鍖栵紝涓柇VECTABNUM鍙峰繀椤诲搴旇cpu,鍚﹀垯涓嶄細璋冪敤涓柇鍑芥暟
*          涓柇鏈嶅姟鍑芥暟PIN_INT0_IRQHandler鍦↙Q_GPIO.c涓�
*************************************************************************/
void Reed_Init(void)
{
      // 鍒濆鍖�,杈撳叆鍙ｏ紝楂樼數骞�
      PIN_InitConfig(REEDp, PIN_MODE_INPUT, 1);
      // 浣胯兘鍦轰腑鏂� 锛屼笅闄嶆部瑙﹀彂
      PIN_Exti(REEDp, PIN_IRQ_MODE_FALLING);
}
#pragma warning 544         // 锟斤拷锟轿撅拷锟斤拷

/*************************************************************************
*  鍑芥暟鍚嶇О锛歶nsigned char KEY_Read(KEYn_e KEYno)
*  鍔熻兘璇存槑锛氳鍙栨寜閿姸鎬�
*  鍙傛暟璇存槑锛欿EYn_e KEYno鎸夐敭缂栧彿
*  鍑芥暟杩斿洖锛氭寜閿姸鎬侊紝0/1
*  淇敼鏃堕棿锛�2020骞�3鏈�10鏃�
*  澶�    娉細
*************************************************************************/
unsigned char KEY_Read(KEYn_e KEYno)
{
    switch(KEYno)
    {
      case KEY0:
        return PIN_Read(KEY0p);//母锟斤拷锟较帮拷锟斤拷0
      break;

      case KEY1:
        return PIN_Read(KEY1p);//母锟斤拷锟较帮拷锟斤拷1
      break;

      case KEY2:
        return PIN_Read(KEY2p);//母锟斤拷锟较帮拷锟斤拷2
      break;

      case KEY3:
        return PIN_Read(KEY3p);//母锟斤拷锟较帮拷锟斤拷1
      break;

      case KEY4:
        return PIN_Read(KEY4p);//母锟斤拷锟较帮拷锟斤拷2
      break;

      case DSW0:
        return PIN_Read(DSW0p);//姣嶆澘涓婃嫧鐮佸紑鍏�0
      break;

      case DSW1:
        return PIN_Read(DSW1p);//姣嶆澘涓婃嫧鐮佸紑鍏�1
      break;
      default:
        return 0XFF;
    }
    return 0;
}
#pragma warning default     // 锟津开撅拷锟斤拷


/*************************************************************************
*  锟斤拷锟斤拷锟斤拷锟狡ｏ拷unsigned char KEY_Read_All(void)
*  锟斤拷锟斤拷说锟斤拷锟斤拷锟斤拷取全锟斤拷锟斤拷锟斤拷状态
*  锟斤拷锟斤拷说锟斤拷锟斤拷锟斤拷
*  锟斤拷锟斤拷锟斤拷锟截ｏ拷锟斤拷锟斤拷锟斤拷锟阶刺拷锟�0--7锟斤拷锟斤拷状态
*  锟睫革拷时锟戒：2020锟斤拷3锟斤拷10锟斤拷
*  锟斤拷    注锟斤拷锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷状态锟斤拷锟斤拷锟斤拷锟斤拷霞锟绞癸拷锟�
*************************************************************************/
unsigned char KEY_Read_All(void)
{
   unsigned char tm=0;

   tm = (PIN_Read(KEY0p)|(PIN_Read(KEY1p)<<1)|(PIN_Read(KEY2p)<<2));//锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷状态锟斤拷锟斤拷锟斤拷
   if(tm==0x07)
	{
	   return 0;
	}
//   while(tm == (PIN_Read(KEY0p)|(PIN_Read(KEY1p)<<1)|(PIN_Read(KEY2p)<<2)));//锟饺达拷锟斤拷锟斤拷锟酵凤拷

   return  (~tm)&0X07;
}


/*************************************************************************
*  锟斤拷锟斤拷锟斤拷锟狡ｏ拷void Test_GPIO_KEY(void)
*  锟斤拷锟斤拷说锟斤拷锟斤拷锟斤拷锟皆筹拷锟斤拷
*  锟斤拷锟斤拷说锟斤拷锟斤拷锟斤拷
*  锟斤拷锟斤拷锟斤拷锟截ｏ拷锟斤拷
*  锟睫革拷时锟戒：2020锟斤拷3锟斤拷10锟斤拷
*  锟斤拷    注锟斤拷锟斤拷锟斤拷KEY0/1/2锟斤拷锟斤拷锟� LED全锟斤拷
*************************************************************************/
void Test_GPIO_KEY(void)
 {
	unsigned char k=0xFF;

	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read(KEY0)& KEY_Read(KEY1)& KEY_Read(KEY2);

 		if(k==0) LED_Ctrl(LEDALL, ON); //锟斤拷锟斤拷KEY0/1/2锟斤拷锟斤拷锟� LED全锟斤拷
 		else     LED_Ctrl(LEDALL, OFF);//锟缴匡拷KEY0/1/2锟斤拷锟斤拷锟� LED全锟斤拷

 		delayms(100);                //锟斤拷时锟饺达拷
 	}
 }


/*************************************************************************
*  锟斤拷锟斤拷锟斤拷锟狡ｏ拷void Test_ComKEY_Tft(void)
*  锟斤拷锟斤拷说锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷习锟斤拷锟斤拷锟斤拷诓锟斤拷锟斤拷锟绞�
*  锟斤拷锟斤拷说锟斤拷锟斤拷锟斤拷
*  锟斤拷锟斤拷锟斤拷锟截ｏ拷锟斤拷
*  锟睫革拷时锟戒：2020锟斤拷3锟斤拷10锟斤拷
*  锟斤拷    注锟斤拷锟斤拷锟斤拷状态锟斤拷锟斤拷锟斤拷示
*************************************************************************/
void Test_ComKEY_Tft(void)
 {
	unsigned char k=0xFF;

	TFTSPI_Init(0);        //LCD锟斤拷始锟斤拷  0:锟斤拷锟斤拷  1锟斤拷锟斤拷锟斤拷
	TFTSPI_CLS(u16BLUE);   //锟斤拷色锟斤拷幕
	TFTSPI_Show_Logo(0,37);//锟斤拷示锟斤拷锟斤拷LOGO
	TFTSPI_P16x16Str(0,0,(unsigned char*)"你好",u16RED,u16BLUE);		//锟街凤拷锟斤拷锟斤拷示
	TFTSPI_P8X16Str(0,1,"Long Qiu i.s.t.",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
	delayms(1000);              //锟斤拷时锟饺达拷
	TFTSPI_Init(1);        //LCD锟斤拷始锟斤拷  0:锟斤拷锟斤拷  1锟斤拷锟斤拷锟斤拷
	TFTSPI_CLS(u16BLACK);   //锟斤拷色锟斤拷幕
	TFTSPI_P8X16Str(0,1,"KEY Pressed:  ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示

	GPIO_KEY_Init();
 	while(1)
 	{
 		k= KEY_Read_All();
 		switch(k)
 		{
 		  case NOKEYDOWN:
 			 //TFTSPI_P8X16Str(35,3,"NO key!  ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY0DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0!    ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY1DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY1!    ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY2DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY2!    ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY01DOWN:
 			 TFTSPI_P8X16Str(35,3,"KEY0&1!  ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY02DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0&2!  ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY12DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY1&2!  ",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  case KEY012DOWN:
 		 	 TFTSPI_P8X16Str(35,3,"KEY0&1&2!",u16WHITE,u16BLACK);		//锟街凤拷锟斤拷锟斤拷示
 		  break;
 		  default:
 		  break;
 		 }
 		LED_Ctrl(LED0,RVS);        //锟侥革拷LED同时锟斤拷烁
 		delayms(100);              //锟斤拷时锟饺达拷
 	}
 }

void test_key(void)
{
    if(PIN_Read(KEY0p)==0)
    {
        delayms(20);
        while(PIN_Read(KEY0p)==0);
        LED_Ctrl(LED2,RVS);
    }
    if(PIN_Read(KEY1p)==0)
    {
        delayms(20);
        while(PIN_Read(KEY1p)==0);
        LED_Ctrl(LED2,RVS);
    }
    if(PIN_Read(KEY2p)==0)
    {
        delayms(20);
        while(PIN_Read(KEY2p)==0);
        LED_Ctrl(LED2,RVS);
    }
    if(PIN_Read(KEY3p)==0)
    {
        delayms(20);
        while(PIN_Read(KEY3p)==0);
        LED_Ctrl(LED2,RVS);
    }
    if(PIN_Read(KEY4p)==0)
    {
        delayms(20);
        while(PIN_Read(KEY4p)==0);
        LED_Ctrl(LED2,RVS);
    }
    if(PIN_Read(DSW1p)==1)
    {
        LED_Ctrl(LED2,1);
    }
    else
    {
        LED_Ctrl(LED2,0);
    }
    if(PIN_Read(DSW0p)==1)
    {
        LED_Ctrl(LED1,0);
    }
    else
    {
        LED_Ctrl(LED1,1);
    }
}


