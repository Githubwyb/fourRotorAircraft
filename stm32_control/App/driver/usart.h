#ifndef _USART_H_
#define _USART_H_
#include <stm32f4xx.h>

void USART1_Init(uint32_t bound, uint8_t nvic);
void USART6_Init(uint32_t bound, uint8_t nvic);
uint8_t USART_Receive(USART_TypeDef* usart);
void USART_Send(USART_TypeDef* usart, uint8_t* data, uint8_t n);

#endif
