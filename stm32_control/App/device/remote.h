#ifndef _REMOTE_H_
#define _REMOTE_H_
#include <stm32f4xx.h>

//红外遥控识别码(ID),每款遥控器的该值基本都不一样,但也有一样的.
//我们选用的遥控器识别码为0
#define REMOTE_ID 0                

extern uint8_t RmtCnt;   //按键按下的次数

void Remote_Init(void);
uint8_t Remote_Scan(void);

#endif
