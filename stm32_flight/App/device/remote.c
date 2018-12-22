#include "remote.h"

void Remote_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef  TIM1_ICInitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//TIM1时钟使能 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//GPIOA8  复用功能,上拉
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1); //GPIOA8复用为TIM1

    TIM_TimeBaseStructure.TIM_Prescaler = 167;  ////预分频器,1M的计数频率,1us加1.   
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseStructure.TIM_Period = 10000;   //设定计数器自动重装值 最大10ms溢出  
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    //初始化TIM2输入捕获参数
    TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     选择输入端 IC1映射到TI1上
    TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;  //上升沿捕获
    TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;     //配置输入分频,不分频 
    TIM1_ICInitStructure.TIM_ICFilter = 0x03;//IC1F=0003 8个定时器时钟周期滤波
    TIM_ICInit(TIM1, &TIM1_ICInitStructure);//初始化定时器2输入捕获通道

    TIM_ITConfig(TIM1, TIM_IT_Update | TIM_IT_CC1, ENABLE);//允许更新中断 ,允许CC1IE捕获中断    
    TIM_Cmd(TIM1, ENABLE);        //使能定时器1

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;       //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;       //子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //初始化NVIC寄存器
}

//遥控器接收状态
//[7]:收到了引导码标志
//[6]:得到了一个按键的所有信息
//[5]:保留    
//[4]:标记上升沿是否已经被捕获                                 
//[3:0]:溢出计时器
uint8_t  RmtSta = 0;
uint16_t Dval;       //下降沿时计数器的值
uint32_t RmtRec = 0;   //红外接收到的数据              
uint8_t  RmtCnt = 0;   //按键按下的次数    
                       //定时器1溢出中断
void TIM1_UP_TIM10_IRQHandler(void)
{

    if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET) //溢出中断
    {
        if (RmtSta & 0x80)//上次有数据被接收到了
        {
            RmtSta &= ~0X10;                      //取消上升沿已经被捕获标记
            if ((RmtSta & 0X0F) == 0X00)
                RmtSta |= 1 << 6;//标记已经完成一次按键的键值信息采集
            if ((RmtSta & 0X0F)<14)
                RmtSta++;
            else
            {
                RmtSta &= ~(1 << 7);//清空引导标识
                RmtSta &= 0XF0;   //清空计数器 
            }
        }
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);  //清除中断标志位 
}

//定时器1输入捕获中断服务程序     
void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET) //处理捕获(CC1IE)中断
    {
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8))//上升沿捕获
        {
            TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Falling);     //CC1P=1 设置为下降沿捕获
            TIM_SetCounter(TIM1, 0);     //清空定时器值
            RmtSta |= 0X10;                   //标记上升沿已经被捕获
        }
        else //下降沿捕获
        {
            Dval = TIM_GetCapture1(TIM1);//读取CCR1也可以清CC1IF标志位
            TIM_OC1PolarityConfig(TIM1, TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
            if (RmtSta & 0X10)                 //完成一次高电平捕获 
            {
                if (RmtSta & 0X80)//接收到了引导码
                {

                    if (Dval>300 && Dval<800)          //560为标准值,560us
                    {
                        RmtRec <<= 1; //左移一位.
                        RmtRec |= 0;  //接收到0     
                    }
                    else if (Dval>1400 && Dval<1800)  //1680为标准值,1680us
                    {
                        RmtRec <<= 1; //左移一位.
                        RmtRec |= 1;  //接收到1
                    }
                    else if (Dval>2200 && Dval<2600)  //得到按键键值增加的信息 2500为标准值2.5ms
                    {
                        RmtCnt++;       //按键次数增加1次
                        RmtSta &= 0XF0;   //清空计时器     
                    }
                }
                else if (Dval>4200 && Dval<4700)      //4500为标准值4.5ms
                {
                    RmtSta |= 1 << 7;   //标记成功接收到了引导码
                    RmtCnt = 0;       //清除按键次数计数器
                }
            }
            RmtSta &= ~(1 << 4);
        }
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_CC1);  //清除中断标志位 
}

//处理红外键盘
//返回值:
//   0,没有任何按键按下
//其他,按下的按键键值.
uint8_t Remote_Scan(void)
{
    uint8_t sta = 0;
    uint8_t t1, t2;
    if (RmtSta&(1 << 6))//得到一个按键的所有信息了
    {
        t1 = RmtRec >> 24;          //得到地址码
        t2 = (RmtRec >> 16) & 0xff;   //得到地址反码 
        if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID)//检验遥控识别码(ID)及地址 
        {
            t1 = RmtRec >> 8;
            t2 = RmtRec;
            if (t1 == (uint8_t)~t2)sta = t1;//键值正确     
        }
        if ((sta == 0) || ((RmtSta & 0X80) == 0))//按键数据错误/遥控已经没有按下了
        {
            RmtSta &= ~(1 << 6);//清除接收到有效按键标识
            RmtCnt = 0;       //清除按键次数计数器
        }
    }
    return sta;
}
