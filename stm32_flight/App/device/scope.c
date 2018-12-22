//************************串口示波器握手程序******************
#include "scope.h"
#include "usart.h"

double Scope_Data[4];

uint16_t CRC_CHECK(uint8_t *Buf, uint8_t CRC_CNT)
{
	uint16_t CRC_Temp;
	uint8_t i, j;
	CRC_Temp = 0xffff;

	for (i = 0; i<CRC_CNT; i++)
	{
		CRC_Temp ^= Buf[i];

		for (j = 0; j<8; j++)
		{
			if (CRC_Temp & 0x01)
			{
				CRC_Temp = (uint16_t)((CRC_Temp >> 1) ^ 0xa001);
			}
			else
			{
				CRC_Temp = (uint16_t)(CRC_Temp >> 1);
			}
		}
	}

	return CRC_Temp;
}

//还没写与摄像头有关的优化
void VisualScope_Send(void)
{
	int16_t temp[4] = { 0 };
	uint16_t temp1[4] = { 0 };
	uint8_t databuf[10] = { 0 };
	uint8_t i = 0;
	uint16_t CRC16 = 0;

	


	//保证图像处理优先级
/*	if (g_carInfo.CameraInfo.Is_Image_Ok == 1)
	{
		return;
	}*/



	for (i = 0; i<4; i++)
	{
		temp[i] = (int16_t)Scope_Data[i];
		temp1[i] = (uint16_t)temp[i];
	}

	for (i = 0; i<4; i++)
	{
		databuf[i * 2] = (uint8_t)(temp1[i] % 256);
		databuf[i * 2 + 1] = (uint8_t)(temp1[i] / 256);
	}

	CRC16 = CRC_CHECK(databuf, 8);
	
	
	databuf[8] = (uint8_t)(CRC16 % 256);
	databuf[9] = (uint8_t)(CRC16 / 256);
//	databuf[8] = (uint8)32;
//	databuf[9] = (uint8)16;

	USART_Send(USART1, databuf, 10);
}
