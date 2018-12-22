#include "delay.h"

//TIME_us���Ϊ798915us
void delay_us(uint32_t TIME_us)
{
    uint32_t temp;
    SysTick->LOAD = TIME_us * 21; 							//ʱ�����	  		 
    SysTick->VAL = 0x00;        							//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 				//��ʼ���� 	 
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp&(1 << 16)));					//�ȴ�ʱ�䵽��   
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 				//�رռ�����
    SysTick->VAL = 0X00;       								//��ռ����� 
}

//TIME_ms���Ϊ798ms
void delay_ms(uint16_t TIME_ms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)TIME_ms * 21000;				//ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;           							//��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  			//��ʼ���� 
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp&(1 << 16)));					//�ȴ�ʱ�䵽��   
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  			//�رռ�����
    SysTick->VAL = 0X00;     		  						//��ռ�����	  	    
}
