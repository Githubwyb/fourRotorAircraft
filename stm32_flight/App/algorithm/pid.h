#ifndef _PID_H_
#define _PID_H_

typedef struct 
{
    double error;
    double lastError;
    double integ;
    double deriv;            
    double iLimit;           /*integ limit*/
    double kp;
    double ki;
    double kd;
    double output;
    double oLimit;           /*output limit*/
} pid_struct;

void PID_param_Init(void);
double PID_Compulate(pid_struct* pid, double desired, double measured);
void PID_param_Reset(pid_struct* pid);

#endif
