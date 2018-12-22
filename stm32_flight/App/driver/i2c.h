#ifndef _I2C_H_
#define _I2C_H_
#include <stm32f4xx.h>

#define GPIO_Output 1
#define GPIO_Input 0

//I2C引脚定义
#define SDA_Pin GPIO_Pin_5
#define SCL_Pin GPIO_Pin_6
#define I2C_GPIO GPIOE

//设置引脚电平函数定义
#define SDA_Set GPIO_SetBits(I2C_GPIO, SDA_Pin)
#define SCL_Set GPIO_SetBits(I2C_GPIO, SCL_Pin)
#define SDA_Reset GPIO_ResetBits(I2C_GPIO, SDA_Pin)
#define SCL_Reset GPIO_ResetBits(I2C_GPIO, SCL_Pin)
#define SDA_Get GPIO_ReadInputDataBit(I2C_GPIO, SDA_Pin)

void I2C_S_Init(void);
void I2C_SDA_Init(uint8_t mode);
void I2C_SCL_Init(void);
uint8_t I2C_SendBytes(uint8_t Add,  uint8_t Reg, uint8_t* data, uint8_t n);
uint8_t I2C_SendByte(uint8_t Add,  uint8_t Reg, uint8_t data);
uint8_t I2C_RecieveBytes(uint8_t Add, uint8_t Reg, uint8_t*data, uint8_t n);
uint8_t I2C_RecieveByte(uint8_t Add, uint8_t Reg);
void I2C_WriteByte(uint8_t data);
uint8_t I2C_ReadByte(uint8_t ack);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Ack(void);
void I2C_nAck(void);
uint8_t I2C_rAck(void);

#endif
