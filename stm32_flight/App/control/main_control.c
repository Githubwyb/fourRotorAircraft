#include "main_control.h"

double gyro_n[3];
double acce_n[3];
double angle_n[3];
double angle_exp[3];
uint16_t motor_f = 1000;
uint16_t motor_pwm[4];
uint8_t flag_10ms = 0;
uint16_t pTime = 0;
extern double height;
extern uint8_t flag_height;
uint8_t stop_flag = 0;
uint8_t key = 3;
uint8_t Debug = 0;

pid_struct rollRatePid;
pid_struct pitchRatePid;
pid_struct yawRatePid;
pid_struct rollPid;
pid_struct pitchPid;

void Control_10ms(void)
{
    uint16_t i;
    double data[4];

    pTime++;

    Get_flight_param(gyro_n, acce_n, angle_n);

    data[0] = angle_exp[0];
    data[1] = angle_exp[1];
    data[2] = angle_exp[2];
    data[3] = motor_f;


    /*输入信号限幅*/
    if (data[0] > 30)
        data[0] = 30;
    else if (data[0] < -30)
        data[0] = -30;

    if (data[1] > 30)
        data[1] = 30;
    else if (data[1] < -30)
        data[1] = -30;

    if (data[2] > 100)
        data[2] = 100;
    else if (data[2] < -100)
        data[2] = -100;

    if (data[3] > 1900)
        data[3] = 1900;
    else if (data[3] < 1000)
        data[3] = 1000;

    /*油门低于1200时，进行积分清零*/
    if (data[3] < 1200)
    {
        rollPid.integ = 0;
        pitchPid.integ = 0;
        rollRatePid.integ = 0;
        pitchRatePid.integ = 0;
        yawRatePid.integ = 0;
    }
    else
    {
        /*角度环PID计算*/
        PID_Compulate(&rollPid, data[0], angle_n[0]);
        PID_Compulate(&pitchPid, data[1], angle_n[1]);

        /*角速度环PID计算，角度环PID输出作为角速度环的期望*/
        PID_Compulate(&rollRatePid, rollPid.output, gyro_n[0]);
        PID_Compulate(&pitchRatePid, pitchPid.output, gyro_n[1]);
        PID_Compulate(&yawRatePid, data[2], gyro_n[2]);
    }

    /*控制量分解*/
    motor_pwm[0] = data[3] - 0.7f * rollRatePid.output - 0.7f * pitchRatePid.output - yawRatePid.output;
    motor_pwm[1] = data[3] + 0.7f * rollRatePid.output - 0.7f * pitchRatePid.output + yawRatePid.output;
    motor_pwm[2] = data[3] + 0.7f * rollRatePid.output + 0.7f * pitchRatePid.output - yawRatePid.output;
    motor_pwm[3] = data[3] - 0.7f * rollRatePid.output + 0.7f * pitchRatePid.output + yawRatePid.output;

    /*输出信号限幅，限制在1000~2000us之间*/
    for (i = 0; i < 4; i++)
    {
        if (motor_pwm[i] > 2000)
        {
            motor_pwm[i] = 2000;
        }
        else if (motor_pwm[i] < 1000)
        {
            motor_pwm[i] = 1000;
        }
    }

    if(Debug)
    {
        motor_pwm[0] = 1000;
        motor_pwm[1] = 1000;
        motor_pwm[2] = 1000;
        motor_pwm[3] = 1000;
    }
    TIM_SetCompare1(TIM3, motor_pwm[0]);
    TIM_SetCompare2(TIM3, motor_pwm[1]);
    TIM_SetCompare3(TIM3, motor_pwm[2]);
    TIM_SetCompare4(TIM3, motor_pwm[3]);


    LED_Change(LED_0);
}

void Control_main(void)
{
    float data0[3], data1[3];
    int16_t data[4];
    if (pTime > 9)
    {
        pTime = 0;
        data0[0] = angle_n[0];
        data0[1] = angle_n[1];
        data0[2] = angle_n[2];
        sendAngleData(data0, height);

        data0[0] = acce_n[0];
        data0[1] = acce_n[1];
        data0[2] = acce_n[2];

        data1[0] = gyro_n[0];
        data1[1] = gyro_n[1];
        data1[2] = gyro_n[2];
        sendAccelGyroData(data0, data1);

        data[0] = motor_pwm[0];
        data[1] = motor_pwm[1];
        data[2] = motor_pwm[2];
        data[3] = motor_pwm[3];
        sendMotorData(data);
    }
    switch(key)
    {
        case 0:
            
            break;
        case 1:
            motor_f += 1;
            break;
        case 2:
            motor_f = 1000;
            break;
        case 3:
            Debug = 1;
            break;
        case 9:
            angle_exp[1] = 5;
            break;
        case 11:
            angle_exp[1] = -5;
            break;
        case 6:
            angle_exp[0] = 5;
            break;
        case 14:
            angle_exp[0] = -5;
            break;
        case 16:
            angle_exp[0] = 0;
            angle_exp[1] = 0;
            break;
		default:
			break;
    }
    key = 17;

    if(height >= 50)
        stop_flag = 1;

    if(height < 0)
        motor_f = 1000;

    if(motor_f > 1700)
        motor_f = 1700;
    else if(motor_f < 1000)
        motor_f = 1000;

    if(stop_flag)
        motor_f = 1000;
//     key = Remote_Scan();
//     switch (key)
//     {
//     case 0:
//         angle_exp[1] = 0;
//         angle_exp[0] = 0;
// //        str = "ERROR";
//         break;
//     case 162:
// //        str = "POWER";
//         break;
//     case 98:
//         angle_exp[1] = 5;
// //        str = "UP";
//         break;
//     case 2:
// //        str = "PLAY";
//         break;
//     case 226:
// //        str = "ALIENTEK";
//         break;
//     case 194:
//         angle_exp[0] = 5;
// //        str = "RIGHT";
//         break;
//     case 34:
//         angle_exp[0] = -5;
// //        str = "LEFT";
//         break;
//     case 224:
//         motor_f -= 100;
// //        str = "VOL-";
//         break;
//     case 168:
//         angle_exp[1] = -5;
// //        str = "DOWN";
//         break;
//     case 144:
//         motor_f += 100;
// //        str = "VOL+";
//         break;
//     case 104:
//         motor_f = 1100;
// //        str = "1";
//         break;
//     case 152:
//         motor_f = 1200;
// //        str = "2";
//         break;
//     case 176:
//         motor_f = 1300;
// //        str = "3";
//         break;
//     case 48:
//         motor_f = 1400;
// //        str = "4";
//         break;
//     case 24:
//         motor_f = 1500;
// //        str = "5";
//         break;
//     case 122:
//         motor_f = 1600;
// //        str = "6";
//         break;
//     case 16:
//         motor_f = 1700;
// //        str = "7";
//         break;
//     case 56:
// //        motor_f = 1800;
// //        str = "8";
//         break;
//     case 90:
//  //       motor_f = 1900;
// //        str = "9";
//         break;
//     case 66:
//         motor_f = 1000;
// //        str = "0";
//         break;
//     case 82:
// //        str = "DELETE";
//         break;
//     default:
//         break;
//     }
    if(flag_10ms)
    {
        flag_10ms = 0;
        Control_10ms();
        if(flag_height == 2)
        {
            ULTR_Start();
            flag_height = 1;
        }
        if(height >= 30)
            motor_f -= 2;
        else if(key ==0 && height <= 20)
            motor_f += 1;
    }

        // extern double Scope_Data[4];
        // Scope_Data[0] = height;
        // Scope_Data[1] = height;
        // Scope_Data[2] = height;
        // Scope_Data[3] = height;
        // VisualScope_Send();
}
