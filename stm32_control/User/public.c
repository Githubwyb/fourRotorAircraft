#include <public.h>

void STM32_Init(void)
{
	LED_Init();
    USART1_Init(115200,1);
    USART6_Init(115200,0);
    KEY_Init();
    KEYBOARD_Init();
    MCF_OLED_Init();
    MCF_OLED_SetPeriod(50);
}

