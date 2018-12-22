#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "stm32f4xx.h"

#define KEYBOAED_IN_0 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)
#define KEYBOAED_IN_1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define KEYBOAED_IN_2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)
#define KEYBOAED_IN_3 GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)

#define KEYBOAED_SET_0 GPIO_SetBits(GPIOD, GPIO_Pin_1)
#define KEYBOAED_SET_1 GPIO_SetBits(GPIOD, GPIO_Pin_3)
#define KEYBOAED_SET_2 GPIO_SetBits(GPIOD, GPIO_Pin_5)
#define KEYBOAED_SET_3 GPIO_SetBits(GPIOD, GPIO_Pin_7)

#define KEYBOAED_RESET_0 GPIO_ResetBits(GPIOD, GPIO_Pin_1)
#define KEYBOAED_RESET_1 GPIO_ResetBits(GPIOD, GPIO_Pin_3)
#define KEYBOAED_RESET_2 GPIO_ResetBits(GPIOD, GPIO_Pin_5)
#define KEYBOAED_RESET_3 GPIO_ResetBits(GPIOD, GPIO_Pin_7)

void KEYBOARD_Init(void);
uint8_t KEYBOARD_S(uint8_t mode);

#endif
