#ifndef _ULTRASONIC_H_
#define _ULTRASONIC_H_
#include <stm32f4xx.h>

#define ULTR_TRIG GPIO_Pin_7
#define ULTR_ECHO GPIO_Pin_8

void ULTR_Init(void);
void ULTR_Start(void);
double Get_Height(void);

#endif
