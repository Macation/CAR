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
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>

/*************************************************************************
*  鍑芥暟鍚嶇О锛歷oid LED_Init(void)
*  鍔熻兘璇存槑锛欸PIO鍒濆鍖栧嚱鏁� LED鐏墍鐢≒10.6銆丳10.5銆丳15.4鍜孭15.6鍒濆鍖�
*  鍙傛暟璇存槑锛氭棤
*  鍑芥暟杩斿洖锛氭棤
*  淇敼鏃堕棿锛�2020骞�3鏈�10鏃�
*  澶�    娉細
*************************************************************************/
void GPIO_LED_Init(void)
{
    // 鍒濆鍖�,杈撳叆鍙ｏ紝楂樼數骞�
    PIN_InitConfig(LED0p, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(LED1p, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(LED2p, PIN_MODE_OUTPUT, 1);
    PIN_InitConfig(LED3p, PIN_MODE_OUTPUT, 1);
}


/*************************************************************************
*  鍑芥暟鍚嶇О锛歷oid LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  鍔熻兘璇存槑锛歀ED鎺у埗
*  鍙傛暟璇存槑锛歀EDn_e LEDno缂栧彿,LEDs_e sta鐘舵�佷寒/鐏�/缈昏浆
*  鍑芥暟杩斿洖锛氭寜閿姸鎬侊紝0/1
*  淇敼鏃堕棿锛�2020骞�3鏈�10鏃�
*  澶�    娉細
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
    switch(LEDno)
    {
    case LED0:
      if(sta==ON)        PIN_Write(LED0p,0);
      else if(sta==OFF) PIN_Write(LED0p,1);
      else if(sta==RVS) PIN_Reverse(LED0p);
    break;

    case LED1:
      if(sta==ON)        PIN_Write(LED1p,0);
      else if(sta==OFF) PIN_Write(LED1p,1);
      else if(sta==RVS) PIN_Reverse(LED1p);
    break;

    case LED2:
      if(sta==ON)        PIN_Write(LED2p,0);
      else if(sta==OFF) PIN_Write(LED2p,1);
      else if(sta==RVS) PIN_Reverse(LED2p);
    break;

    case LED3:
      if(sta==ON)        PIN_Write(LED3p,0);
      else if(sta==OFF) PIN_Write(LED3p,1);
      else if(sta==RVS) PIN_Reverse(LED3p);
    break;
    case LEDALL:
      if(sta==ON)
      {
    	  PIN_Write(LED0p,0);
    	  PIN_Write(LED1p,0);
    	  PIN_Write(LED2p,0);
    	  PIN_Write(LED3p,0);
      }
      else if(sta==OFF)
      {
    	  PIN_Write(LED0p,1);
    	  PIN_Write(LED1p,1);
    	  PIN_Write(LED2p,1);
    	  PIN_Write(LED3p,1);
      }
      else if(sta==RVS)
      {
    	  PIN_Reverse(LED0p);
    	  PIN_Reverse(LED1p);
    	  PIN_Reverse(LED2p);
    	  PIN_Reverse(LED3p);
      }
    break;
    default:
    break;
    }
}
/*************************************************************************
*  鍑芥暟鍚嶇О锛歷oid Test_GPIO_LED(void)
*  鍔熻兘璇存槑锛氭祴璇曠▼搴�
*  鍙傛暟璇存槑锛氭棤
*  鍑芥暟杩斿洖锛氭棤
*  淇敼鏃堕棿锛�2020骞�3鏈�10鏃�
*  澶�    娉細鏍稿績鏉夸笂鐨凩ED鐏棯鐑�--鍥涗釜LED鍚屾椂闂儊
*************************************************************************/
void Test_GPIO_LED(void)
 {
	GPIO_LED_Init();
 	while(1)
 	{
 	    LED_Ctrl(LED2,ON);
 	    LED_Ctrl(LED3,OFF);

 		delayms(1000);                //锟斤拷时锟饺达拷
        LED_Ctrl(LED2,OFF);
        LED_Ctrl(LED3,ON);
 	}
 }
