/*
 * My_Button.c
 *
 *  Created on: 2024年4月29日
 *      Author: dduu
 */
#include <My_Button.h>

int chooseBuf = 1;    //左右选择时不立即进入该菜单
int switch_flag=0;    //页面更新标志

//上一个
int isBtnP()
{
    if(Key_Value==1)
    {
        Key_Value=0;
        return 1;
    }
    return 0;

}

//下一个
int isBtnN()
{
    if(Key_Value==2)
    {
        Key_Value=0;
        return 1;
    }
    return 0;
}

//确认键
int isBtnE()
{
    if(Key_Value==3)
    {
        Key_Value=0;
        return 1;
    }
    return 0;
}

//返回键
int isBtnR()
{
    if(Key_Value==4)
    {
        Key_Value=0;
        return 1;
    }
    return 0;
}

/* 按键扫描 */
void Button_Scan(void)
{
    switch(menu.mode1)
    {
        //主菜单
        case 0:
            if(isBtnE()){          //确认键
                switch_flag=1;
                menu.mode1 = chooseBuf; chooseBuf = 1;
            }else if(isBtnR()){    //返回键（主菜单不用）
            }else if(isBtnP()){    //左键
                switch_flag=1;
                if(chooseBuf>1)
                    chooseBuf--;
                else
                    chooseBuf=1;
            }else if(isBtnN()){    //右键
                switch_flag=1;
                if(chooseBuf<3)
                    chooseBuf++;
                else
                    chooseBuf=3;
            }
            break;
        /*
         * 二级菜单1摄像头
         */
        case 1:
            switch(menu.mode2)
            {
                /*
                 * 二级菜单选择 摄像头
                 */
                case 0:
                    if(isBtnE()){        //确认键
                        switch_flag=1;
                        menu.mode2 = chooseBuf; chooseBuf = 1;
                    }else if(isBtnR()){  //返回键
                        switch_flag=1;
                        chooseBuf = 1; menu.mode2 = 0; menu.mode1 = 0;
                    }else if(isBtnP()){    //左键
                        switch_flag=1;
                        if(chooseBuf>1)
                            chooseBuf--;
                        else
                            chooseBuf=1;
                    }else if(isBtnN()){  //右键
                        switch_flag=1;
                        if(chooseBuf<2) chooseBuf++;
                        else    chooseBuf=2;
                    }
                    break;
                /*
                 * 三级菜单1  App1
                 */
                case 1:
                    if(isBtnE()){         //确认键（App1中不需要确认键）
                        switch_flag=1;
                    }else if(isBtnR()){   //返回键
                        switch_flag=1;
                        menu.mode2 = 0;
                    }else if(isBtnP()){   //左键（App1中不需要左键）
                    }else if(isBtnN()){   //右键（App1中不需要右键）
                    }
                    break;
                /*
                 * 三级菜单2  App2
                 */
                case 2:
                    if(isBtnE()){         //确认键（App1中不需要确认键）
                        switch_flag=1;
                    }else if(isBtnR()){   //返回键
                        switch_flag=1;
                        menu.mode2 = 0;
                    }else if(isBtnP()){   //左键（App1中不需要左键）
                    }else if(isBtnN()){   //右键（App1中不需要右键）
                    }
                    break;
            }
            break;
        /*
         * 二级菜单2    PID
         */
        case 2:
            switch(menu.mode2)
            {
                /*
                 * 二级菜单选择
                 */
                case 0:
                    if(isBtnE()){        //确认键
                        switch_flag=1;
                        menu.mode2 = chooseBuf; chooseBuf = 1;
                    }else if(isBtnR()){  //返回键
                        switch_flag=1;
                        chooseBuf = 1; menu.mode2 = 0; menu.mode1 = 0;
                    }else if(isBtnP()){    //左键
                        switch_flag=1;
                        if(chooseBuf>1)
                            chooseBuf--;
                        else
                            chooseBuf=1;
                    }else if(isBtnN()){   //右键
                        switch_flag=1;
                        if(chooseBuf<2) chooseBuf++;
                        else chooseBuf=2;
                    }
                    break;
                /*
                 * 三级菜单1  App1
                 */
                case 1:
                    if(isBtnE()){         //确认键（App1中不需要确认键）
                    }else if(isBtnR()){   //返回键
                        switch_flag=1;
                        menu.mode2 = 0;
                    }else if(isBtnP()){   //左键（App1中不需要左键）
                    }else if(isBtnN()){   //右键（App1中不需要右键）
                    }
                    break;
                /*
                 * 三级菜单2  App2
                 */
                case 2:
                    if(isBtnE()){         //确认键（App1中不需要确认键）
                    }else if(isBtnR()){   //返回键
                        switch_flag=1;
                        menu.mode2 = 0;
                    }else if(isBtnP()){   //左键（App1中不需要左键）
                    }else if(isBtnN()){   //右键（App1中不需要右键）
                    }
                    break;
            }
            break;
            /*
         * 二级菜单3    ENC
         */
        case 3:
            switch(menu.mode2)
            {
                /*
                 * 二级菜单选择
                 */
                case 0:
                    if(isBtnE()){        //确认键
                        switch_flag=1;
                        menu.mode2 = chooseBuf; chooseBuf = 1;
                    }else if(isBtnR()){  //返回键
                        switch_flag=1;
                        chooseBuf = 1; menu.mode2 = 0; menu.mode1 = 0;
                    }else if(isBtnP()){  //左键
                        switch_flag=1;
                        if(chooseBuf>1)
                            chooseBuf--;
                        else
                            chooseBuf=1;
                    }else if(isBtnN()){  //右键
                        switch_flag=1;
                        if(chooseBuf<1) chooseBuf++;
                        else chooseBuf=1;
                    }
                    break;
                /*
                 * 三级菜单1  App1
                 */
                case 1:
                    if(isBtnE()){         //确认键（App1中不需要确认键）
                    }else if(isBtnR()){   //返回键
                        switch_flag=1;
                        menu.mode2 = 0;
                    }else if(isBtnP()){   //左键（App1中不需要左键）
                    }else if(isBtnN()){   //右键（App1中不需要右键）
                    }
                    break;
            }
            break;
    }
}



