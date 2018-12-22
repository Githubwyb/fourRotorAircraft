#include <motor.h>

void MOTOR_Init(uint16_t prescaler, uint16_t period)
{
    //pwm初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);         //TIM3时钟使能    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);       //使能PORTC时钟 
    
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM3);      //GPIOC6复用为定时器3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM3);      //GPIOC7复用为定时器3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM3);      //GPIOC8复用为定时器3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM3);      //GPIOC9复用为定时器3
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;      //6/7/8/9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;          //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //上拉
    GPIO_Init(GPIOC,&GPIO_InitStructure);                       //初始化PC6/PC7
      
    TIM_TimeBaseStructure.TIM_Prescaler = prescaler - 1;            //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = period - 1;                  //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);             //初始化定时器3
    
    //初始化TIM3 Channel1/2 PWM模式    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;           //选择定时器模式:TIM脉冲宽度调制模式2
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //输出极性:TIM输出比较极性高
    TIM_OC1Init(TIM3, &TIM_OCInitStructure);                   //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);                   //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);                   //根据T指定的参数初始化外设TIM3 OC2
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);                   //根据T指定的参数初始化外设TIM3 OC2

    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);          //使能TIM3在CCR1上的预装载寄存器
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);          //使能TIM3在CCR1上的预装载寄存器
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);          //使能TIM3在CCR1上的预装载寄存器
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);          //使能TIM3在CCR1上的预装载寄存器
    TIM_ARRPreloadConfig(TIM3,ENABLE);                         //ARPE使能 
    TIM_Cmd(TIM3, ENABLE);                                     //使能TIM3

    TIM_SetCompare1(TIM3, 1000);
    TIM_SetCompare2(TIM3, 1000);
    TIM_SetCompare3(TIM3, 1000);
    TIM_SetCompare4(TIM3, 1000);
}
