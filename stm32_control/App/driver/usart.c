#include "usart.h"
#include "led.h"
#include "key.h"

uint8_t data[255];
uint8_t n = 0;
uint8_t n_resv = 0;
uint8_t n_send = 0;

void USART1_Init(uint32_t bound, uint8_t nvic)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);       //使能GPIOA时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);      //使能USART1时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   //GPIOA9复用为USART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  //GPIOA10复用为USART1

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;     //GPIOA9与GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	        //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);                      //初始化PA9，PA10

    //USART1 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                 //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
    USART_Init(USART1, &USART_InitStructure);                   //初始化串口1

    USART_Cmd(USART1, ENABLE);                                  //使能串口1 

    if (nvic) {
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //开启相关中断

        //Usart1 NVIC 配置
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //串口1中断通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);	                        //根据指定的参数初始化NVIC寄存器
    }
    else {
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);         //开启相关中断
    }
}

void USART6_Init(uint32_t bound, uint8_t nvic)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);       //使能GPIOC时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);      //使能USART6时钟

    //串口1对应引脚复用映射
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);   //GPIOC6复用为USART6
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);  //GPIOC7复用为USART6

    //USART1端口配置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;     //GPIOC6与GPIOC7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //复用功能
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //速度50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //推挽复用输出
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                      //初始化PC6，PC7

    //USART6 初始化设置
    USART_InitStructure.USART_BaudRate = bound;                 //波特率设置
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;         //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式
    USART_Init(USART6, &USART_InitStructure);                   //初始化串口6

    USART_Cmd(USART6, ENABLE);                                  //使能串口6 

    if (nvic) {
        USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);          //开启相关中断

        //Usart1 NVIC 配置
        NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;       //串口6中断通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
        NVIC_Init(&NVIC_InitStructure);                         //根据指定的参数初始化NVIC寄存器
    }
    else {
        USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);         //开启相关中断
    }
}

uint8_t USART_Receive(USART_TypeDef* usart)
{
    while (USART_GetFlagStatus(usart, USART_FLAG_RXNE != SET)) {}
    return USART_ReceiveData(usart);
}

void USART_Send(USART_TypeDef* usart, uint8_t* data, uint8_t n)
{
    uint8_t i;
    uint8_t* point;
    if (n)
    {
        for (i = 0; i < n; i++)
        {
            USART_SendData(usart, data[i]);
            while (USART_GetFlagStatus(usart, USART_FLAG_TC) != SET) {}
        }
    }
    else
    {
        point = data;
        while (*point != '\0')
        {
            USART_SendData(usart, *point);
            while (USART_GetFlagStatus(usart, USART_FLAG_TC) != SET) {}
            point++;
        }
    }
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == SET)
    {
        data[n_resv] = USART_ReceiveData(USART1);
        n++;
        n_resv++;
        if(n_resv == 250)
            n_resv = 0;
//        USART_Send(USART6, &data, 1);
		LED_Change(LED_0);
    }
}

void USART6_IRQHandler(void)                    //串口6中断服务程序
{
    uint8_t data1;
    if (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET)
    {
        data1 = USART_ReceiveData(USART6);
		USART_Send(USART6, &data1, 1);
        LED_Change(LED_0);
    }
}
