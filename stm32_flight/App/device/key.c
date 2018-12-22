#include "key.h"
#include "delay.h"

void KEY_Init(void){
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOA,GPIOE时钟
 
    GPIO_InitStructure.GPIO_Pin = KEY_0 | KEY_1; //KEY0 KEY1对应引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE3,4
    
    GPIO_InitStructure.GPIO_Pin = KEY_UP;//WK_UP对应引脚PA0
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN ;//下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA0
}

uint8_t KEY_S(uint8_t mode){
    static uint8_t key_up = 1;//按键按松开标志
    if(mode)
        key_up = 1;  //支持连按       
    if(key_up && (!GPIO_ReadInputDataBit(GPIOE,KEY_0) || !GPIO_ReadInputDataBit(GPIOE,KEY_1) || GPIO_ReadInputDataBit(GPIOA,KEY_UP)))
    {
        delay_ms(10);//去抖动 
        key_up = 0;
        if(!GPIO_ReadInputDataBit(GPIOE,KEY_0))
            return 0;
        else if(!GPIO_ReadInputDataBit(GPIOE,KEY_1))
            return 1;
        else if(GPIO_ReadInputDataBit(GPIOA,KEY_UP))
            return 2;
    }
    else if(GPIO_ReadInputDataBit(GPIOE,KEY_0) && GPIO_ReadInputDataBit(GPIOE,KEY_1) && !GPIO_ReadInputDataBit(GPIOA,KEY_UP))
        key_up = 1;         
    return 3;// 无按键按下
}
