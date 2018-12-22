#ifndef _SCOPE_H_
#define _SCOPE_H_
#include <stm32f4xx.h>

uint16_t CRC_CHECK(uint8_t *Buf, uint8_t CRC_CNT);

void VisualScope_Send(void);

#endif
