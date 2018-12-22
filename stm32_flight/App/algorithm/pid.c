#include "pid.h"

extern pid_struct rollRatePid;
extern pid_struct pitchRatePid;
extern pid_struct yawRatePid;
extern pid_struct rollPid;
extern pid_struct pitchPid;

void PID_param_Init(void)
{
    PID_param_Reset(&rollRatePid);
    PID_param_Reset(&pitchRatePid);
    PID_param_Reset(&yawRatePid);
    PID_param_Reset(&rollPid);
    PID_param_Reset(&pitchPid);

    rollPid.kp = 7;
    rollPid.ki = 0;
    rollPid.kd = 0;
    rollPid.iLimit = 20;
    rollPid.oLimit = 300;

    pitchPid.kp = 7;
    pitchPid.ki = 0;
    pitchPid.kd = 0;
    pitchPid.iLimit = 20;
    pitchPid.oLimit = 300;

    rollRatePid.kp = 1.2;
    rollRatePid.ki = 2.0;
    rollRatePid.kd = 0.06;
    rollRatePid.iLimit = 20;
    rollRatePid.oLimit = 200;

    pitchRatePid.kp = 1.2;
    pitchRatePid.ki = 2.0;
    pitchRatePid.kd = 0.06;
    pitchRatePid.iLimit = 20;
    pitchRatePid.oLimit = 200;

    yawRatePid.kp = 2.8;
    yawRatePid.ki = 1.5;
    yawRatePid.kd = 0.06;
    yawRatePid.iLimit = 20;
    yawRatePid.oLimit = 200;
}

/**
*@breif:pid计算
*@param desired:期望
*@param measured:实际测量值
*/
double PID_Compulate(pid_struct* pid, double desired, double measured)
{
    pid->error = desired - measured;
    pid->integ += (pid->error * 0.01f);                 /*integ update*/

    if (pid->integ > pid->iLimit)                            /*limit the integ*/
        pid->integ = pid->iLimit;
    else if (pid->integ < -pid->iLimit)
        pid->integ = -pid->iLimit;

    pid->deriv = (pid->error - pid->lastError) * 100;

    pid->output = pid->kp * pid->error + pid->ki * pid->integ + pid->kd * pid->deriv;

    if (pid->output > pid->oLimit)                           /*limit the output*/
        pid->output = pid->oLimit;
    else if (pid->output < -pid->oLimit)
        pid->output = -pid->oLimit;

    pid->lastError = pid->error;

    return pid->output;
}


/*****************************************************************************/
/**
*@breif:复位pid参数，将pid参数全部置为0
*/
void PID_param_Reset(pid_struct* pid)
{
    pid->error = 0;
    pid->lastError = 0;
    pid->integ = 0;
    pid->deriv = 0;
    pid->iLimit = 0;
    pid->kp = 0;
    pid->ki = 0;
    pid->kd = 0;
    pid->output = 0;
    pid->oLimit = 0;
}


