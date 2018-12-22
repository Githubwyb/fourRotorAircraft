#ifndef _MOTOR_H_
#define _MOTOR_H_
#include <stm32f4xx.h>

//motor0方向控制：PC7
#define MOTOR0_Direction GPIO_Pin_7
//motor1方向控制：PD13
#define MOTOR1_Direction GPIO_Pin_13

void MOTOR_Init(uint16_t prescaler, uint16_t period);
void MOTOR0_Set(int32_t pwm);
void MOTOR1_Set(int32_t pwm);

#endif // !_LED_H_
