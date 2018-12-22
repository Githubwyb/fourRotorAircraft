#ifndef _PUBLIC_H
#define _PUBLIC_H
#include <stm32f4xx.h>

//driver
#include "delay.h"
#include "usart.h"
//#include "tim.h"

//device
#include "led.h"
#include "key.h"
#include "keyboard.h"
//#include "scope.h"
#include "oled.h"
//#include "remote.h"

void STM32_Init(void);

#endif // _PUBLIC_H
