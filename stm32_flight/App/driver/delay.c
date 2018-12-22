#include "delay.h"

//TIME_us最大为798915us
void delay_us(uint32_t TIME_us)
{
    uint32_t temp;
    SysTick->LOAD = TIME_us * 21; 							//时间加载	  		 
    SysTick->VAL = 0x00;        							//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; 				//开始倒数 	 
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp&(1 << 16)));					//等待时间到达   
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk; 				//关闭计数器
    SysTick->VAL = 0X00;       								//清空计数器 
}

//TIME_ms最大为798ms
void delay_ms(uint16_t TIME_ms)
{
    uint32_t temp;
    SysTick->LOAD = (uint32_t)TIME_ms * 21000;				//时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;           							//清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  			//开始倒数 
    do
    {
        temp = SysTick->CTRL;
    } while ((temp & 0x01) && !(temp&(1 << 16)));					//等待时间到达   
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  			//关闭计数器
    SysTick->VAL = 0X00;     		  						//清空计数器	  	    
}
