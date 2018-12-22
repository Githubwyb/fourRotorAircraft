#include "keyboard.h"
#include "delay.h"

void KEYBOARD_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOE, ENABLE);  //使能GPIOB,GPIOD,GPIOE时钟
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                                //普通输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                          //100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                                //下拉
    GPIO_Init(GPIOD, &GPIO_InitStructure);                                      //初始化GPIOD0,2,4,6
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                               //普通输入模式

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //初始化GPIOB3,6,8

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //初始化GPIOE0

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStructure);                                      //初始化GPIOE0

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);                                      //初始化GPIOE0

    KEYBOAED_RESET_0;
    KEYBOAED_RESET_1;
    KEYBOAED_RESET_2;
    KEYBOAED_RESET_3;
}

uint8_t KEYBOARD_S(uint8_t mode)
{
    static uint8_t key_up = 1;//按键按松开标志
    if(mode)
        key_up = 1;  //支持连按

    KEYBOAED_RESET_0;
    KEYBOAED_RESET_1;
    KEYBOAED_RESET_2;
    KEYBOAED_RESET_3;

    if(key_up && (!KEYBOAED_IN_0 || !KEYBOAED_IN_1 || !KEYBOAED_IN_2 || !KEYBOAED_IN_3))
    {
        delay_ms(10);//去抖动 
        key_up = 0;

        KEYBOAED_RESET_0;
        KEYBOAED_SET_1;
        KEYBOAED_SET_2;
        KEYBOAED_SET_3;
		delay_ms(1);
        if(!KEYBOAED_IN_0)
            return 0;
        else if(!KEYBOAED_IN_1)
            return 1;
        else if(!KEYBOAED_IN_2)
            return 2;
        else if(!KEYBOAED_IN_3)
            return 3;

        KEYBOAED_RESET_1;
        KEYBOAED_SET_0;
        KEYBOAED_SET_2;
        KEYBOAED_SET_3;
		delay_ms(1);
        if(!KEYBOAED_IN_0)
            return 4;
        else if(!KEYBOAED_IN_1)
            return 5;
        else if(!KEYBOAED_IN_2)
            return 6;
        else if(!KEYBOAED_IN_3)
            return 7;

        KEYBOAED_RESET_2;
        KEYBOAED_SET_1;
        KEYBOAED_SET_0;
        KEYBOAED_SET_3;
		delay_ms(1);
        if(!KEYBOAED_IN_0)
            return 8;
        else if(!KEYBOAED_IN_1)
            return 9;
        else if(!KEYBOAED_IN_2)
            return 10;
        else if(!KEYBOAED_IN_3)
            return 11;

        KEYBOAED_RESET_3;
        KEYBOAED_SET_1;
        KEYBOAED_SET_2;
        KEYBOAED_SET_0;
		delay_ms(1);
        if(!KEYBOAED_IN_0)
            return 12;
        else if(!KEYBOAED_IN_1)
            return 13;
        else if(!KEYBOAED_IN_2)
            return 14;
        else if(!KEYBOAED_IN_3)
            return 15;
    }
    else if(KEYBOAED_IN_0 && KEYBOAED_IN_1 && KEYBOAED_IN_2 && KEYBOAED_IN_3)
        key_up = 1;         
    return 16;// 无按键按下
}

