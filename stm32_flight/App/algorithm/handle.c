#include "handle.h"
#include "math.h"
#include "mpu6050.h"

#define PI 3.1415926535

#define Kp          2.0f
#define Ki          0.005f
#define halfT       0.005f      //100Hz,10ms
#define FACTOR      0.002f

double q0 = 1;
double q1 = 0;
double q2 = 0;
double q3 = 0;

void Get_flight_param(double* gyro, double* acce, double* angle)
{
    double gyro_g[3];
    MPU_Get_param(gyro, acce);

    gyro_g[0] = gyro[0] / 180 * PI; //角度换算成弧度
    gyro_g[1] = gyro[1] / 180 * PI; //角度换算成弧度
    gyro_g[2] = gyro[2] / 180 * PI; //角度换算成弧度

    Get_angle(gyro_g, acce, angle);
}

/**
*@breif:结算姿态角
*@param accel   :用于传入加速度数据，单位：m^2/s
*@param gyro    :用于传入角速度数据，单位：弧度/s
*@param angle   :用于返回解算出来的欧拉角数据，单位：度
*/
void Get_angle(double *gyro, double *acce, double *angle)
{
    double norm;
    double vx, vy, vz;
    double ex, ey, ez;

    double delta_2 = 0;

    double ax = acce[0];
    double ay = acce[1];
    double az = acce[2];

    double gx = gyro[0];
    double gy = gyro[1];
    double gz = gyro[2];
    // normalise the measurements
    norm = sqrt(ax * ax + ay * ay + az * az);
    ax = ax / norm;
    ay = ay / norm;
    az = az / norm;

    // estimated direction of gravity
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // error is sum of cross product between reference direction of field and direction measured by sensor
    ex = (ay * vz - az * vy);
    ey = (az * vx - ax * vz);
    ez = (ax * vy - ay * vx);

    gx = gx + ex * FACTOR / halfT;
    gy = gy + ey * FACTOR / halfT;
    gz = gz + ez * FACTOR / halfT;

    delta_2 = (2 * halfT*gx)*(2 * halfT*gx) + (2 * halfT*gy)*(2 * halfT*gy) + (2 * halfT*gz)*(2 * halfT*gz);

    q0 = (1 - delta_2 / 8) * q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
    q1 = (1 - delta_2 / 8) * q1 + (q0*gx + q2*gz - q3*gy)*halfT;
    q2 = (1 - delta_2 / 8) * q2 + (q0*gy - q1*gz + q3*gx)*halfT;
    q3 = (1 - delta_2 / 8) * q3 + (q0*gz + q1*gy - q2*gx)*halfT;

    // normalise quaternion
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    // roll
    angle[0] = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.295780f;
    // pitch
    angle[1] = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.295780f;
    // yaw
    angle[2] = -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1)* 57.295780f;
}


