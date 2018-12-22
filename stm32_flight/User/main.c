#include <public.h>

int main(){
	STM32_Init();
    while(1)
    {
//        MCF_OLED_6x8str(0,0,"OK");
        Control_main();
    }
}
