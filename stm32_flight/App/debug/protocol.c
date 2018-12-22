#include "protocol.h"
#include "usart.h"

#define BUF_SIZE		    128
#define BYTE0(temp)			(temp & 0x000000ff)
#define BYTE1(temp)			(temp & 0x0000ff00)>>8
#define BYTE2(temp)			(temp & 0x00ff0000)>>16
#define BYTE3(temp)			(temp & 0xff000000)>>24


static unsigned char txBuf[BUF_SIZE];

void sendAngleData(float* angle, float height)
{
	unsigned char cnt = 0;
	volatile unsigned int temp;
	
	unsigned char i;
	unsigned char sum = 0x00;
	
	txBuf[cnt++] = 0xAA;
	txBuf[cnt++] = 0xAA;
	txBuf[cnt++] = 0x01;
	txBuf[cnt++] = 0;

	temp = (short)(angle[0]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	temp = (short)(angle[1]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	temp = (short)(angle[2]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	//2字节超声波高度
    temp = (short)(height);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);
	//4字节气压高度
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	//1字节解锁标志
	txBuf[cnt++] = 0xA1;
	//写入帧长度
	txBuf[3] = cnt - 4;
	//校验和计算

	for(i=0;i<cnt;i++)
		sum += txBuf[i];

	txBuf[cnt++] = sum;

	USART_Send(USART1, txBuf, cnt);
}

void sendAccelGyroData(float* accel,float* gyro)
{
	unsigned char cnt = 0;
	volatile unsigned int temp;
	unsigned char i;
	unsigned char sum = 0x00;
	
	txBuf[cnt++] = 0xAA;
	txBuf[cnt++] = 0xAA;
	txBuf[cnt++] = 0x02;
	txBuf[cnt++] = 0;

	//6字节加速度信息
	temp = (short)(accel[0]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	temp = (short)(accel[1]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	temp = (short)(accel[2]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	//6字节角速度信息
	temp = (short)(gyro[0]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	temp = (short)(gyro[1]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);

	temp = (short)(gyro[2]*100);
	txBuf[cnt++] = BYTE1(temp);
	txBuf[cnt++] = BYTE0(temp);


	//6字节空白帧
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	txBuf[cnt++] = 0;
	//写入帧长度
	txBuf[3] = cnt - 4;
	//校验和计算

	for(i=0;i<cnt;i++)
		sum += txBuf[i];

	txBuf[cnt++] = sum;

	USART_Send(USART1, txBuf,cnt);
}

//void echoRcData(short *val)
//{
//	unsigned char cnt = 0;
//	volatile unsigned int temp;

//	txBuf[cnt++] = 0xAA;
//	txBuf[cnt++] = 0xAA;
//	txBuf[cnt++] = 0x03;		//功能字
//	txBuf[cnt++] = 20;			//帧长

//	temp = rcData[THR];			//THR
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	temp = rcData[RUD];			//RUD
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	temp = rcData[AIL];			//AIL
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	//6字节角速度信息
//	temp = rcData[ELE];			//ELE
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	//AUX1
//	txBuf[cnt++] = 0x00;
//	txBuf[cnt++] = 0x00;
//	//AUX2
//	txBuf[cnt++] = 0x00;
//	txBuf[cnt++] = 0x00;
//	//AUX3
//	txBuf[cnt++] = 0x00;
//	txBuf[cnt++] = 0x00;
//	//AUX4
//	txBuf[cnt++] = 0x00;
//	txBuf[cnt++] = 0x00;
//	//AUX5
//	txBuf[cnt++] = 0x00;
//	txBuf[cnt++] = 0x00;
//	//AUX6
//	txBuf[cnt++] = 0x00;
//	txBuf[cnt++] = 0x00;

//	txBuf[3] = cnt - 4;
//	//校验和计算
//	unsigned char i;
//	unsigned char sum = 0x00;
//	for(i=0;i<cnt;i++)
//		sum += txBuf[i];

//	txBuf[cnt++] = sum;

//	usartSendBytes(txBuf,cnt);
//}

void sendMotorData(short* val)
{
	unsigned char cnt = 0;

	unsigned char i;
	unsigned char sum = 0x00;
	//将1000~2000量化到0~1000，上位机的柱条显示范围0~1000

	txBuf[cnt++] = 0xAA;
	txBuf[cnt++] = 0xAA;
	txBuf[cnt++] = 0x06;
	txBuf[cnt++] = 0;

	txBuf[cnt++] = BYTE1(val[0]-1000);
	txBuf[cnt++] = BYTE0(val[0]-1000);
	txBuf[cnt++] = BYTE1(val[1]-1000);
	txBuf[cnt++] = BYTE0(val[1]-1000);
	txBuf[cnt++] = BYTE1(val[2]-1000);
	txBuf[cnt++] = BYTE0(val[2]-1000);
	txBuf[cnt++] = BYTE1(val[3]-1000);
	txBuf[cnt++] = BYTE0(val[3]-1000);
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;
	txBuf[cnt++] = 0x00;

	txBuf[3] = cnt-4;

	//校验和计算

	for(i=0;i<cnt;i++)
		sum += txBuf[i];

	txBuf[cnt++] = sum;

	USART_Send(USART1, txBuf, cnt);
}

//void echoGyroPidData(pidStruct* pid1,pidStruct* pid2,pidStruct* pid3)
//{
//	unsigned char cnt = 0;
//	volatile unsigned int temp;

//	txBuf[cnt++] = 0xAA;
//	txBuf[cnt++] = 0xAA;
//	txBuf[cnt++] = 0x10;
//	txBuf[cnt++] = 0;

//	temp = pid1->kp * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid1->ki * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid1->kd * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	temp = pid2->kp * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid2->ki * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid2->kd * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	temp = pid3->kp * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid3->ki * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid3->kd * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	//写入帧长度
//	txBuf[3] = cnt - 4;
//	//校验和计算
//	unsigned char i;
//	unsigned char sum = 0x00;
//	for(i=0;i<cnt;i++)
//		sum += txBuf[i];

//	txBuf[cnt++] = sum;

//	trace(txBuf,cnt);
//}
//void echoPidData(pidStruct* pid1,pidStruct* pid2,pidStruct* pid3)
//{
//	unsigned char cnt = 0;
//	volatile unsigned int temp;

//	txBuf[cnt++] = 0xAA;
//	txBuf[cnt++] = 0xAA;
//	txBuf[cnt++] = 0x11;
//	txBuf[cnt++] = 0;

//	temp = pid1->kp * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid1->ki * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid1->kd * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	temp = pid2->kp * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid2->ki * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid2->kd * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	temp = pid3->kp * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid3->ki * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);
//	temp = pid3->kd * 100;
//	txBuf[cnt++] = BYTE1(temp);
//	txBuf[cnt++] = BYTE0(temp);

//	//写入帧长度
//	txBuf[3] = cnt - 4;
//	//校验和计算
//	unsigned char i;
//	unsigned char sum = 0x00;
//	for(i=0;i<cnt;i++)
//		sum += txBuf[i];

//	txBuf[cnt++] = sum;

//	trace(txBuf,cnt);
//}
