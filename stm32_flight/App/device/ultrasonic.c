#include "ultrasonic.h"
#include "delay.h"

double height = 0;
uint8_t flag_height = 2;                            //1捕获上升沿 0捕获下降沿 2等待

void ULTR_Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);     //TIM2时钟使能    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);   //使能PORTB时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //GPIOB10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  //速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //下拉

    GPIO_Init(GPIOB,&GPIO_InitStructure); //初始化PB10

    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2); //PB10复用位定时器2
      
    TIM_TimeBaseStructure.TIM_Prescaler = 83;  //定时器分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = 0xffffffff;   //自动重装载值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
    
    TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
    
    //初始化TIM2输入捕获参数
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; 
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;         //上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;     //映射到TI上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;               //配置输入分频,不分频 
    TIM_ICInitStructure.TIM_ICFilter = 0x00;                            //IC1F=0000 配置输入滤波器 不滤波
    TIM_ICInit(TIM2, &TIM_ICInitStructure);
        
    TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC3,ENABLE);                 //允许更新中断 ,允许CC1IE捕获中断   
    TIM_Cmd(TIM2,ENABLE);                                               //使能定时器5
 
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 2;            //抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  //子优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                                     //根据指定的参数初始化NVIC寄存器
}

void ULTR_Start(void)
{
    GPIO_SetBits(GPIOB, GPIO_Pin_11);
    delay_us(15);
    GPIO_ResetBits(GPIOB, GPIO_Pin_11);
}

void TIM2_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        if(flag_height == 0)
        {
            height = -1;
            flag_height = 2;
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising);
        }
    }
    else if(TIM_GetITStatus(TIM2, TIM_IT_CC3) != RESET)
    {
		uint32_t time = 0;
        if(flag_height == 1)
        {
            flag_height = 0;
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Falling);
            TIM_Cmd(TIM2,DISABLE);                      //关闭定时器2
            TIM_SetCounter(TIM2,0);                     //清空计数器
            TIM_Cmd(TIM2,ENABLE);                       //使能定时器2
        }
        else if(flag_height == 0)
        {
            flag_height = 2;
            time = TIM_GetCapture3(TIM2);
            TIM_OC3PolarityConfig(TIM2,TIM_ICPolarity_Rising);
            height = time * 17 / 1000;
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC3|TIM_IT_Update); //清除中断标志位
}
