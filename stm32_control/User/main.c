#include <public.h>

extern uint8_t data[255];
extern uint8_t n;
extern uint8_t n_resv;
extern uint8_t n_send;
uint8_t keyboard = 16;

int main()
{
	STM32_Init();
	while(1)
	{
        keyboard = KEYBOARD_S(1);
        USART_Send(USART1, &keyboard, 1);
        if(n > 10)
        {
            USART_Send(USART6, &data[n_send], 10);
            n -= 10;
            n_send += 10;
            if(n_send == 250)
                n_send = 0;
        }
	}
}
