#include <led.h>

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = LED;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, LED);
}

void LED_Light(uint16_t led) 
{
	GPIO_ResetBits(GPIOA, led);
}

void LED_Change(uint16_t led) {
	if(GPIO_ReadOutputDataBit(GPIOA,led))
		GPIO_ResetBits(GPIOA, led);
	else
		GPIO_SetBits(GPIOA, led);
}

void LED_Write(uint8_t led)
{
    if(led & 0x01)
    {
        GPIO_ResetBits(GPIOA, LED_0);
    }
    else
    {
        GPIO_SetBits(GPIOA, LED_0);
    }
    if(led & 0x02)
    {
        GPIO_ResetBits(GPIOA, LED_1);
    }
    else
    {
        GPIO_SetBits(GPIOA, LED_1);
    }
}

uint8_t LED_GetStatus(uint16_t led)
{
    return !GPIO_ReadOutputDataBit(GPIOF,led);
}
