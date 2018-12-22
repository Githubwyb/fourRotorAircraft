#include "i2c.h"
#include "delay.h"

void I2C_S_Init(void)
{
	I2C_SDA_Init(GPIO_Output);
    SDA_Set;
    I2C_SCL_Init();
}

void I2C_SDA_Init(uint8_t mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SDA_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;

    if(mode)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    else
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;

    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
}

void I2C_SCL_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = SCL_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;

    GPIO_Init(I2C_GPIO, &GPIO_InitStructure);
    SCL_Set;
}

uint8_t I2C_SendBytes(uint8_t Add,  uint8_t Reg, uint8_t* data, uint8_t n)
{
	uint8_t i;
	I2C_Start();
	I2C_WriteByte((Add << 1) | 0);
    if(I2C_rAck())
    {
	   I2C_Stop();
       return 1;
    }
	I2C_WriteByte(Reg);
	I2C_rAck();
	for(i = 0;i < n;i++)
	{
		I2C_WriteByte(data[i]);
		if(I2C_rAck())
        {
           I2C_Stop();
           return 1;
        }
	}
	I2C_Stop();
    return 0;
}

uint8_t I2C_SendByte(uint8_t Add,  uint8_t Reg, uint8_t data)
{
    I2C_Start();
    I2C_WriteByte((Add << 1) | 0);
    if(I2C_rAck())
    {
       I2C_Stop();
       return 1;
    }
    I2C_WriteByte(Reg);
    I2C_rAck();
    I2C_WriteByte(data);
    if(I2C_rAck())
    {
       I2C_Stop();
       return 1;
    }
    I2C_Stop();
    return 0;
}

uint8_t I2C_RecieveBytes(uint8_t Add, uint8_t Reg, uint8_t*data, uint8_t n)
{
	uint8_t i;
	I2C_Start();
	I2C_WriteByte((Add << 1) | 0);
	if(I2C_rAck())
    {
       I2C_Stop();
       return 1;
    }
	I2C_WriteByte(Reg);
	I2C_rAck();
	
	I2C_Start();
	I2C_WriteByte((Add << 1) | 1);
	I2C_rAck();
	for(i = 0;i < n - 1;i++)
	{
		data[i] = I2C_ReadByte(1);
	}
	data[n - 1] = I2C_ReadByte(0);
	I2C_Stop();
    return 0;
}

uint8_t I2C_RecieveByte(uint8_t Add, uint8_t Reg)
{
    uint8_t rev_data;
    I2C_Start();
    I2C_WriteByte((Add << 1) | 0);
    I2C_rAck();
    I2C_WriteByte(Reg);
    I2C_rAck();
    
    I2C_Start();
    I2C_WriteByte((Add << 1) | 1);
    I2C_rAck();
    rev_data = I2C_ReadByte(0);
    I2C_Stop();
    return rev_data;
}

void I2C_WriteByte(uint8_t data)
{
	uint8_t i = 0;
	I2C_SDA_Init(GPIO_Output);
    SCL_Reset;
	for(i = 0;i < 8; i++)
	{
        if((data & 0x80))
            SDA_Set;
        else
            SDA_Reset;
		data <<= 1;
		delay_us(2);
		SCL_Set;
		delay_us(2);
		SCL_Reset;
		delay_us(2);
	}
}

uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t i = 0;
	uint8_t rev_data = 0;
    I2C_SDA_Init(GPIO_Input);
	for(i = 0;i < 8; i++)
	{
		SCL_Reset;
		delay_us(2);
        SCL_Set;
		rev_data <<= 1;
		rev_data |= SDA_Get;
		delay_us(1);
	}
    if(ack)
        I2C_Ack();
    else
        I2C_nAck();
    return rev_data;
}

void I2C_Start(void)
{
	I2C_SDA_Init(GPIO_Output);
    SDA_Set;
	SCL_Set;
	delay_us(4);
	SDA_Reset;
	delay_us(4);
	SCL_Reset;
}

void I2C_Stop(void)
{
	I2C_SDA_Init(GPIO_Output);
    SCL_Reset;
    SDA_Reset;
    delay_us(4);
	SCL_Set;
	SDA_Set;
	delay_us(4);	
}

void I2C_Ack(void)
{
    SCL_Reset;
	I2C_SDA_Init(GPIO_Output);
    SDA_Reset;
	delay_us(2);
	SCL_Set;
	delay_us(2);
	SCL_Reset;
}

void I2C_nAck(void)
{
    SCL_Reset;
	I2C_SDA_Init(GPIO_Output);
    SDA_Set;
	delay_us(2);
	SCL_Set;
	delay_us(2);
	SCL_Reset;
}

uint8_t I2C_rAck(void)
{
	uint8_t i = 0;
	I2C_SDA_Init(GPIO_Input);
	delay_us(1);
	SCL_Set;
	delay_us(1);
	while (SDA_Get)
	{
		i++;
        if(i > 250)
        {
            I2C_Stop();
            return 1;
        }
	}
	SCL_Reset;
	return 0;
}
