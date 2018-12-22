#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

extern void sendAngleData(float* angle, float height);
extern void sendAccelGyroData(float* accel,float* gyro);
extern void sendMotorData(short* val);

#endif /* PROTOCOL_H_ */
