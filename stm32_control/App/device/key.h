#ifndef _KEY_H_
#define _KEY_H_
#include <stm32f4xx.h>

#define KEY_0 GPIO_Pin_4
#define KEY_1 GPIO_Pin_3
#define KEY_UP GPIO_Pin_0

void KEY_Init(void);
uint8_t KEY_S(uint8_t mode);

#endif // !_LED_H_
