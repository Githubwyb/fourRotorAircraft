#include "MPU6050.h"
#include "i2c.h"
#include "delay.h"
#include "math.h"

// int16_t GYRO_XYZrotate[3][3] = {0,0,0,0,0,0,0,0,0};
// int16_t ACCE_XYZacce[3][3] = {0,0,0,0,0,0,0,0,0};
// int16_t magn[3] = {0,0,0};

int16_t gyro_offset[3] = {0,0,0};
int16_t acce_offset[3] = {0,0,0};

uint8_t MPU6050_Init(void)
{
    uint8_t rev_data;
    I2C_S_Init();                                       //初始化IIC总线
    I2C_SendByte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X80);    //复位MPU6050
    delay_ms(100);
    // I2C_SendByte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X00);    //唤醒MPU6050 
    // MPU_Set_Gyro_Fsr(3);                                //陀螺仪传感器,±2000dps
    // MPU_Set_Accel_Fsr(0);                               //加速度传感器,±2g
    // MPU_Set_Rate(50);                                   //设置采样率50Hz
    // I2C_SendByte(MPU_ADDR, MPU_USER_CTRL_REG, 0X00);    //关闭I2C主模式
    // I2C_SendByte(MPU_ADDR, MPU_INTBP_CFG_REG, 0X02);    //开启bypass
    // I2C_SendByte(MPU_ADDR, MPU_INT_EN_REG, 0X00);       //关闭所有中断
    // I2C_SendByte(MPU_ADDR, MPU_FIFO_EN_REG, 0X00);      //关闭FIFO
    // I2C_SendByte(MPU_ADDR, MPU_INTBP_CFG_REG, 0X80);    //INT引脚低电平有效
    I2C_SendByte(MPU_ADDR,MPU_PWR_MGMT1_REG,0x01);
    I2C_SendByte(MPU_ADDR,0x19,0x07);//cyq：07 更新频率1khz
    I2C_SendByte(MPU_ADDR,0x1A,0x05);          //10Hz 滤波
    I2C_SendByte(MPU_ADDR,0x1B,0x00);
    I2C_SendByte(MPU_ADDR,0x1C,0x00);
    rev_data = I2C_RecieveByte(MPU_ADDR, MPU_DEVICE_ID_REG);
    if (rev_data == MPU_ADDR)                           //器件ID正确
    {
        // I2C_SendByte(MPU_ADDR, MPU_PWR_MGMT1_REG, 0X01);//设置CLKSEL,PLL X轴为参考
        // I2C_SendByte(MPU_ADDR, MPU_PWR_MGMT2_REG, 0X00);//加速度与陀螺仪都工作
        // MPU_Set_Rate(50);                               //设置采样率为50Hz
    }
    else
        return 1;
    Get_offset();
    return 0;
}

// void HMC5883L_Init(void)
// {
//     I2C_SendByte(0x1E, 0x00, 0x18);
//     I2C_SendByte(0x1E, 0x01, 0x20);
//     I2C_SendByte(0x1E, 0x02, 0x00);
// }


//设置MPU6050陀螺仪传感器满量程范围
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Gyro_Fsr(uint8_t fsr)
{
    return I2C_SendByte(MPU_ADDR, MPU_GYRO_CFG_REG, fsr << 3);//设置陀螺仪满量程范围  
}

//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Accel_Fsr(uint8_t fsr)
{
    return I2C_SendByte(MPU_ADDR, MPU_ACCEL_CFG_REG, fsr << 3);//设置加速度传感器满量程范围  
}

//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_LPF(uint16_t lpf)
{
    uint8_t data = 0;
    if (lpf >= 188)
        data = 1;
    else if (lpf >= 98)
        data = 2;
    else if (lpf >= 42)
        data = 3;
    else if (lpf >= 20)
        data = 4;
    else if (lpf >= 10)
        data = 5;
    else
        data = 6;
    return I2C_SendByte(MPU_ADDR, MPU_CFG_REG, data);//设置数字低通滤波器  
}

//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)
//返回值:0,设置成功
//    其他,设置失败 
uint8_t MPU_Set_Rate(uint16_t rate)
{
    uint8_t data;
    if (rate > 1000)
        rate = 1000;
    if (rate < 4)
        rate = 4;
    data = 1000 / rate - 1;
    I2C_SendByte(MPU_ADDR, MPU_SAMPLE_RATE_REG, data);  //设置数字低通滤波器
    return MPU_Set_LPF(rate / 2);                       //自动设置LPF为采样率的一半
}

//得到温度值
//返回值:温度值(扩大了100倍)
double MPU_Get_Temperature(void)
{
    uint8_t rev_data[2];
    int16_t raw;
    double temp;
    I2C_RecieveBytes(MPU_ADDR, MPU_TEMP_OUTH_REG, rev_data, 2);
    raw = ((uint16_t)rev_data[0] << 8) | rev_data[1];
    temp = 36.53 + ((double)raw) / 340;
    return temp;
}

//得到陀螺仪值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Gyroscope(int16_t* rotate)
{
    uint8_t rev_data[6], res;
    res = I2C_RecieveBytes(MPU_ADDR, MPU_GYRO_XOUTH_REG, rev_data, 6);
    if (res == 0)
    {
        rotate[0] = ((uint16_t)rev_data[0] << 8) | rev_data[1];
        rotate[1] = ((uint16_t)rev_data[2] << 8) | rev_data[3];
        rotate[2] = ((uint16_t)rev_data[4] << 8) | rev_data[5];
    }
    return res;
}

//得到加速度值(原始值)
//gx,gy,gz:陀螺仪x,y,z轴的原始读数(带符号)
//返回值:0,成功
//    其他,错误代码
uint8_t MPU_Get_Accelerometer(int16_t* acce)
{
    uint8_t rev_data[6], res;
    res = I2C_RecieveBytes(MPU_ADDR, MPU_ACCEL_XOUTH_REG, rev_data, 6);
    if (res == 0)
    {
        acce[0] = ((uint16_t)rev_data[0] << 8) | rev_data[1];
        acce[1] = ((uint16_t)rev_data[2] << 8) | rev_data[3];
        acce[2] = ((uint16_t)rev_data[4] << 8) | rev_data[5];
    }
    return res;
}

void MPU_Get_param(double* gyro, double* acce)
{
    int16_t data[3];
    MPU_Get_Gyroscope(data);
    gyro[0] = (data[0] - gyro_offset[0]) / 131.0;
    gyro[1] = (data[1] - gyro_offset[1]) / 131.0;
    gyro[2] = (data[2] - gyro_offset[2]) / 131.0;

    MPU_Get_Accelerometer(data);
    acce[0] = (data[0] - acce_offset[0]) / 16384.0 * 9.794;
    acce[1] = (data[1] - acce_offset[1]) / 16384.0 * 9.794;
    acce[2] = data[2] / 16384.0 * 9.794;
}

void Get_offset(void)
{
    int32_t sum[3] = {0,0,0};
    int16_t data[3];
    uint16_t i;

    for(i = 0;i < 1000;i++)
    {
        delay_ms(2);
        MPU_Get_Gyroscope(data);
        sum[0] += data[0];
        sum[1] += data[1];
        sum[2] += data[2];
    }

    gyro_offset[0] = sum[0] / 1000;
    gyro_offset[1] = sum[1] / 1000;
    gyro_offset[2] = sum[2] / 1000;

    sum[0] = 0;
    sum[1] = 0;
    sum[2] = 0;

    for(i = 0;i < 1000;i++)
    {
        delay_ms(2);
        MPU_Get_Accelerometer(data);
        sum[0] += data[0];
        sum[1] += data[1];
        sum[2] += data[2];
    }

    acce_offset[0] = sum[0] / 1000;
    acce_offset[1] = sum[1] / 1000;
    acce_offset[2] = sum[2] / 1000;
}

//得到偏转角
//返回值:0,成功
//    其他,错误代码
// double Get_angle(void)
// {
//     uint8_t rev_data[6], res;
//     double angle;
//     delay_ms(20);
//     res = I2C_RecieveBytes(0x1E, 0x03, rev_data, 6);
//     if (res == 0)
//     {
//         magn[0] = ((uint16_t)rev_data[0] << 8) | rev_data[1];
//         magn[1] = ((uint16_t)rev_data[2] << 8) | rev_data[3];
//         magn[2] = ((uint16_t)rev_data[4] << 8) | rev_data[5];
//     }
//     angle = atan2((double)magn[1],(double)magn[0]) * (180 / 3.14159265);
//     return angle;
// }

// void GYRO_Update_XYZ_rotate(double* rotate)
// {
//     GYRO_XYZrotate[2][0] = GYRO_XYZrotate[1][0];
//     GYRO_XYZrotate[1][0] = GYRO_XYZrotate[0][0];

//     GYRO_XYZrotate[2][1] = GYRO_XYZrotate[1][1];
//     GYRO_XYZrotate[1][1] = GYRO_XYZrotate[0][1];

//     GYRO_XYZrotate[2][2] = GYRO_XYZrotate[1][2];
//     GYRO_XYZrotate[1][2] = GYRO_XYZrotate[0][2];

//     MPU_Get_Gyroscope(GYRO_XYZrotate[0]);

//     rotate[0] = GYRO_XYZrotate[0][0] * 0.9 + GYRO_XYZrotate[1][0] * 0.07 + GYRO_XYZrotate[2][0] * 0.03;
//     rotate[1] = GYRO_XYZrotate[0][1] * 0.9 + GYRO_XYZrotate[1][1] * 0.07 + GYRO_XYZrotate[2][1] * 0.03;
//     rotate[2] = GYRO_XYZrotate[0][2] * 0.9 + GYRO_XYZrotate[1][2] * 0.07 + GYRO_XYZrotate[2][2] * 0.03;

//     rotate[0] = (rotate[0] - GYRO_X_ERR) / 16.4;
//     rotate[1] = (rotate[1] - GYRO_Y_ERR) / 16.4;
//     rotate[2] = (rotate[2] - GYRO_Z_ERR) / 16.4;



//     //int16_t rotate_0[3];
//     // MPU_Get_Gyroscope(rotate_0);

//     // rotate[0] = (rotate_0[0] - GYRO_X_ERR) / 16.4;
//     // rotate[1] = (rotate_0[1] - GYRO_Y_ERR) / 16.4;
//     // rotate[2] = (rotate_0[2] - GYRO_Z_ERR) / 16.4;
// }

// void ACCE_Update_XYZ_acce(double*acce)
// {
//     ACCE_XYZacce[2][0] = ACCE_XYZacce[1][0];
//     ACCE_XYZacce[1][0] = ACCE_XYZacce[0][0];
    
//     ACCE_XYZacce[2][1] = ACCE_XYZacce[1][1];
//     ACCE_XYZacce[1][1] = ACCE_XYZacce[0][1];
    
//     ACCE_XYZacce[2][2] = ACCE_XYZacce[1][2];
//     ACCE_XYZacce[1][2] = ACCE_XYZacce[0][2];
    
//     MPU_Get_Accelerometer(ACCE_XYZacce[0]);
    
//     acce[0] = ACCE_XYZacce[0][0] * 0.9 + ACCE_XYZacce[1][0] * 0.07 + ACCE_XYZacce[2][0] * 0.03;
//     acce[1] = ACCE_XYZacce[0][1] * 0.9 + ACCE_XYZacce[1][1] * 0.07 + ACCE_XYZacce[2][1] * 0.03;
//     acce[2] = ACCE_XYZacce[0][2] * 0.9 + ACCE_XYZacce[1][2] * 0.07 + ACCE_XYZacce[2][2] * 0.03;
//     acce[0] = acce[0] / 16384 * 100;
//     acce[1] = acce[1] / 16384 * 100;
//     acce[2] = acce[2] / 16384 * 100;
// }
