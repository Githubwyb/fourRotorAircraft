#include <public.h>

void STM32_Init(void)
{
    USART1_Init(115200, ENABLE);
    LED_Init();
    KEY_Init();
    MOTOR_Init(84,4000);
    MPU6050_Init();
    PID_param_Init();
    ULTR_Init();
    TIM_10ms_Init();
    // MCF_OLED_Init();
    // MCF_OLED_SetPeriod(50);
 //   Remote_Init();
//    HMC5883L_Init();
}
