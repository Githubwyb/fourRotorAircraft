#include "usart.h"
#include "led.h"
#include "key.h"

uint8_t data[255];
uint8_t n = 0;
uint8_t n_resv = 0;
uint8_t n_send = 0;

void USART1_Init(uint32_t bound, uint8_t nvic)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);       //ʹ��GPIOAʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);      //ʹ��USART1ʱ��

    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);   //GPIOA9����ΪUSART1
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);  //GPIOA10����ΪUSART1

    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;     //GPIOA9��GPIOA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	        //�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //����
    GPIO_Init(GPIOA, &GPIO_InitStructure);                      //��ʼ��PA9��PA10

    //USART1 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;                 //����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);                   //��ʼ������1

    USART_Cmd(USART1, ENABLE);                                  //ʹ�ܴ���1 

    if (nvic) {
        USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);          //��������ж�

        //Usart1 NVIC ����
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;       //����1�ж�ͨ��
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
        NVIC_Init(&NVIC_InitStructure);	                        //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
    }
    else {
        USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);         //��������ж�
    }
}

void USART6_Init(uint32_t bound, uint8_t nvic)
{
    //GPIO�˿�����
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);       //ʹ��GPIOCʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);      //ʹ��USART6ʱ��

    //����1��Ӧ���Ÿ���ӳ��
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);   //GPIOC6����ΪUSART6
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);  //GPIOC7����ΪUSART6

    //USART1�˿�����
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;     //GPIOC6��GPIOC7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                //���ù���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           //�ٶ�50MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;              //���츴�����
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                //����
    GPIO_Init(GPIOC, &GPIO_InitStructure);                      //��ʼ��PC6��PC7

    //USART6 ��ʼ������
    USART_InitStructure.USART_BaudRate = bound;                 //����������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //�ֳ�Ϊ8λ���ݸ�ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //һ��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;         //����żУ��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //�շ�ģʽ
    USART_Init(USART6, &USART_InitStructure);                   //��ʼ������6

    USART_Cmd(USART6, ENABLE);                                  //ʹ�ܴ���6 

    if (nvic) {
        USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);          //��������ж�

        //Usart1 NVIC ����
        NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;       //����6�ж�ͨ��
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //�����ȼ�3
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQͨ��ʹ��
        NVIC_Init(&NVIC_InitStructure);                         //����ָ���Ĳ�����ʼ��NVIC�Ĵ���
    }
    else {
        USART_ITConfig(USART6, USART_IT_RXNE, DISABLE);         //��������ж�
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


void USART1_IRQHandler(void)                	//����1�жϷ������
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

void USART6_IRQHandler(void)                    //����6�жϷ������
{
    uint8_t data1;
    if (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == SET)
    {
        data1 = USART_ReceiveData(USART6);
		USART_Send(USART6, &data1, 1);
        LED_Change(LED_0);
    }
}
