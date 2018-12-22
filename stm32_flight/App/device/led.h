#ifndef _LED_H_
#define _LED_H_
#include <stm32f4xx.h>

#define LED_0 GPIO_Pin_6
#define LED_1 GPIO_Pin_7
#define LED GPIO_Pin_6 | GPIO_Pin_7

void LED_Init(void);
void LED_Light(uint16_t led);
void LED_Change(uint16_t led);
void LED_Write(uint8_t led);
uint8_t LED_GetStatus(uint16_t led);

#endif // !_LED_H_
