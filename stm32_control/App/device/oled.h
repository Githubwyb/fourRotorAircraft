#ifndef _OLED_H_
#define _OLED_H_

#include <stm32f4xx.h>

#define OLED_CHAR_WIDTH 6 //每个字符点宽度
#define IMAGE_NUM 128//每行取点，可修改
#define IMAGE_ROW 64//行数，可修改
#define DTIM_CHANNEL 0//使用的外部时钟输入通道，也就是晶振接入的引脚

#define ODD 0//奇数
#define EVEN 1//偶数

#define ON  1//开启
#define OFF 0//关闭

#define   PORT_DD  PDD

#define   PIN_7  7

#define   OLED_SCLK_PORT        GPIOE
#define   OLED_SCLK_PIN			GPIO_Pin_12
#define   OLED_SDIN_PORT        GPIOE
#define   OLED_SDIN_PIN			GPIO_Pin_13
#define   OLED_DC_PORT          GPIOE
#define   OLED_DC_PIN           GPIO_Pin_14
#define   OLED_RST_PORT         GPIOE
#define   OLED_RST_PIN          GPIO_Pin_15

#define   LED_IMAGE_WHITE                    1
#define   LED_IMAGE_BLACK                    0
#define   LED_MAX_ROW_NUM                   64
#define   LED_MAX_COLUMN_NUM               128

#define   TWINK_PERIOD                      50 // per ms

#define   false                              0
#define   E_SYS                              1

//获得液晶的显示更新频率
uint16_t MCF_OLED_GetPeriod(void);

void MCF_OLED_Show_Int(uint8_t uc_posx, uint8_t uc_posy, int32_t num);
void MCF_OLED_Show_Uint(uint8_t uc_posx, uint8_t uc_posy, uint32_t num);

/*---------------------------------------------------------------------------------------------*
*        函数名：MCF_OLED_SetPeriod(uint16_t peri)
*        参数  ：液晶的显示频率
*        返回值：无
*        功能  ：设置液晶的显示频率
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_SetPeriod(uint16_t peri);

/*---------------------------------------------------------------------------------------------*
*        函数名：oled_init(void)
*        参数  ：无
*        返回值：无
*        功能  ：LED初始化
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_Init(void);

/*---------------------------------------------------------------------------------------------*
*        函数名：oled_fill(char uc_data)
*        参数  ：uc_data
*        返回值：无
*        功能  ：全屏写入同一个字符uc_data 清屏
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_Fill(char uc_data);

/*---------------------------------------------------------------------------------------------*
*        函数名：oled_display_6x8char(uint8_t uc_posx, uint8_t uc_posy, char uc_data)
*        参数  ：显示的横坐标(0~122)，行范围(0～7)，显示的字符
*        返回值：无
*        功能  ：显示一个字符
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_6x8char(uint8_t uc_posx, uint8_t uc_posy, char uc_data);

/*---------------------------------------------------------------------------------------------*
*        函数名：oled_display_6x8str(uint8_t uc_posx, uint8_t uc_posy, char uc_dataStr[])
*        参数  ：显示的横坐标(0~122)，行范围(0～7)，显示的字符
*        返回值：无
*        功能  ：显示一串字符
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_6x8str(uint8_t uc_posx, uint8_t uc_posy, char uc_dataStr[]);

/*---------------------------------------------------------------------------------------------*
*        函数名：oled_print_char(uint8_t uc_posx, uint8_t uc_posy, char c_data)
*        参数  ：显示的横坐标(0~122)，行范围(0～7)，需要转化显示的数值(-128~127)
*        返回值：无
*        功能  ：将一个char型数转换成3位数进行显示
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_char(uint8_t uc_posx, uint8_t uc_posy, int8_t c_data);
void MCF_OLED_uint8_t(uint8_t uc_posx, uint8_t uc_posy, uint8_t c_data);

/*---------------------------------------------------------------------------------------------*
*        函数名：oled_print_short(uint8_t uc_posx, uint8_t uc_posy, int16_t s_data)
*        参数  ：显示的横坐标(0~122)，行范围(0～7)，需要转化显示的数值
*        返回值：无
*        功能  ：将一个int16_t型数转换成5位数进行显示
*----------------------------------------------------------------------------------------------*/
void MCF_OLED_short(uint8_t uc_posx, uint8_t uc_posy, int16_t s_data);

//控制液晶显示数字
void OledControl(void);


/**
*  @ brief   oled_set_pos
*
*  @ param   uc_posx   列
*            uc_posy   页
*
*	@ note    设置坐标
*
*	@ return  none
*/
void oled_set_pos(uint8_t uc_posx, uint8_t uc_posy);

/**
*  @ brief   oled_gap
*
*  @ param   none
*
*	@ note    display gap between dates
*
*	@ return  none
*/
void  oled_gap(void);

/**
*  @ brief   oled_twink
*
*  @ param   none
*
*	@ note    twink, to remind us the oled is working
*
*	@ return  none
*/
void  oled_twink(void);

/**
*  @ brief   oled_clear
*
*  @ param   none
*
*	@ note    clear some space
*
*	@ return  none
*/
void  oled_clear(uint8_t start_x, uint8_t page, uint8_t clear_size);

/**
*  @ brief   oled_gap
*
*  @ param   none
*
*	@ note    display gap between dates
*
*	@ return  none
*/
void  oled_one_group(uint8_t my_x, uint8_t my_y, uint16_t my_data);

void oled_my_show(uint8_t my_x, uint8_t my_y, int16_t mydata);

//
void oled_write_data(uint8_t uc_data);

void oled_pin_init(void);

/**
*    @brief   写一个字节命令
*
*    @param   uint8_t uc_cmd
*/
void  oled_write_command(uint8_t uc_cmd);

/**
*  @ brief   SetStartColumn
*
*  @ param   a     起始列地址
*            b     起始行地址
*
*	@ note    对于 page Addressing Mode  设定开始的列
*	          每一页有128列，开始列=16*Highchar Column + Lowchar Column
*
*	@ return  none
*/
void SetStartColumn(uint8_t uc_data);

/**
*  @ brief   SetColumnAddress
*
*  @ param   a     起始列地址
*            b     起始行地址
*
*	@ note    对于Horizontal Addressing Mode 设置起始列地址与结束列地址
*	          Set Column Address
*	          Default => 0x00 (Column Start Address)
*	          Default => 0x7F (Column End Address)
*
*	@ return  none
*/
void SetColumnAddress(uint8_t a, uint8_t b);

/**
*  @ brief   SetPageAddress
*
*  @ param   a     起始页地址
*            b     起始页地址
*
*	@ note    对于Vchartical Addressing Mode 设置起始页地址与结束页地址
*	          Set Page Address
*	          Default => 0xB0 (0x00)
*	          Default => 0x07 (Page End Address)
*
*	@ return  none
*/
void SetPageAddress(uint8_t a, uint8_t b);

/**
*  @ brief   SetInvcharseDisplay
*
*  @ param   uc_data
*
*	@ note    Set Invcharse Display On/Off
*	          Default => 0xA6
*	          0xA6 (0x00) => Normal Display
*	          0xA7 (0x01) => Invcharse Display On
*
*	@ return  none
*/
void SetInvcharseDisplay(uint8_t uc_data);

/**
*  @ brief   SetEntireDisplay
*
*  @ param   uc_data
*
*	@ note    Set Entire Display On / Off
*	          Default => 0xA4
*	          0xA4 (0x00) => Normal Display
*	          0xA5 (0x01) => Entire Display On
*
*	@ return  none
*/
void SetEntireDisplay(uint8_t uc_data);

/**
*  @ brief   SetVCOMH
*
*  @ param   uc_data
*
*	@ note    Set VCOMH Deselect Level
*	          Default => 0x20 (0.77*VCC)
*
*	@ return  none
*/
void SetVCOMH(uint8_t uc_data);

/**
*  @ brief   SetPrechargePchariod
*
*  @ param   uc_data
*
*	@ note    Set Pre-Charge Pchariod
*	          Default => 0x22 (2 Display Clocks [Phase 2] / 2 Display Clocks [Phase 1])
*	          D[3:0] => Phase 1 Pchariod in 1~15 Display Clocks
*	          D[7:4] => Phase 2 Pchariod in 1~15 Display Clocks
*
*	@ return  none
*/
void SetPrechargePchariod(uint8_t uc_data);

/**
*  @ brief   SetContrastControl
*
*  @ param   uc_data
*
*	@ note    Set Contrast Control
*	          Default => 0x7F
*
*	@ return  none
*/
void SetContrastControl(uint8_t uc_data);

/**
*  @ brief   SetCommonConfig
*
*  @ param   uc_data
*
*	@ note    Set COM Pins Hardware Configuration
*	          Default => 0x12 (0x10)
*	          Altcharnative COM Pin Configuration
*	          Disable COM Left/Right Re-Map
*
*	@ return  none
*/
void SetCommonConfig(uint8_t uc_data);

/**
*  @ brief   SetCommonRemap
*
*  @ param   uc_data
*
*	@ note    Set COM Output Scan Direction
*	          Default => 0xC0
*	          0xC0 (0x00) => Scan from COM0  to 63
*	          0xC8 (0x08) => Scan from COM63 to 0
*
*	@ return  none
*/
void SetCommonRemap(uint8_t uc_data);

/**
*  @ brief   SetSegmentRemap
*
*  @ param   uc_data
*
*	@ note    Set Segment Re-Map
*	          Default => 0xA0
*	          0xA0 (0x00) => Column Address 0 Mapped to SEG0
*	          0xA1 (0x01) => Column Address 0 Mapped to SEG127
*
*	@ return  none
*/
void SetSegmentRemap(uint8_t uc_data);

/**
*  @ brief   SetAddressingMode
*
*  @ param   uc_data
*
*	@ note    Set Memory Addressing Mode
*	          0x00 => Horizontal Addressing Mode
*	          0x01 => Vchartical Addressing Mode
*	          0x02 => Page Addressing Mode
*
*	@ return  none
*/
void SetAddressingMode(uint8_t uc_data);

/**
*  @ brief   SetChargePump
*
*  @ param   uc_data
*
*	@ note    Set Charge Pump
*	          0x10 (0x00) => Disable Charge Pump
*	          0x14 (0x04) => Enable Charge Pump
*
*	@ return  none
*/
void SetChargePump(uint8_t uc_data);

/**
*  @ brief   SetStartLine
*
*  @ param   uc_data
*
*	@ note    Set Display Start Line
*	          Default => 0x40 (0x00)
*
*	@ return  none
*/
void SetStartLine(uint8_t uc_data);

/**
*  @ brief   SetDisplayOffset
*
*  @ param   uc_data
*
*	@ note    Set Display Offset
*	          Default => 0x00
*
*	@ return  none
*/
void SetDisplayOffset(uint8_t uc_data);

/**
*  @ brief   SetMultiplexRatio
*
*  @ param   uc_data
*
*	@ note    Set Multiplex Ratio
*	          Default => 0x3F (1/64 Duty)
*
*	@ return  none
*/
void SetMultiplexRatio(uint8_t uc_data);

/**
*  @ brief   SetDisplayClock
*
*  @ param   uc_data
*
*	@ note    Set Display Clock Divchare Ratio / Oscillator Frequency
*	          D[3:0] => Display Clock Divcharchar
D[7:4] => Oscillator Frequency
*
*	@ return  none
*/
void SetDisplayClock(uint8_t uc_data);

/**
*  @ brief   SetDisplayOnOff
*
*  @ param   uc_data
*
*	@ note    Set Display On/Off
*	          0xAE (0x00) => Display Off  Default
0xAF (0x01) => Display On
*
*	@ return  none
*/
void SetDisplayOnOff(uint8_t uc_data);

void MCF_OLED_6x8char_hl(uint8_t uc_posx, uint8_t uc_posy, char uc_data);

void MCF_OLED_6x8str_hl(uint8_t uc_posx, uint8_t uc_posy, char uc_dataStr[]);

void MCF_OLED_char_hl(uint8_t uc_posx, uint8_t uc_posy, int8_t c_data);

void oled_display_16x8char(uint8_t uc_posx, uint8_t uc_posy, char uc_data);

void oled_display_16x8str(uint8_t uc_posx, uint8_t uc_posy, char uc_dataStr[]);

void oled_print_16x8char(uint8_t uc_posx, uint8_t uc_posy, int8_t c_data);

void oled_print_16x8short(uint8_t uc_posx, uint8_t uc_posy, int16_t s_data);

void oled_display_16x8char_hl(uint8_t uc_posx, uint8_t uc_posy, char uc_data);

void oled_display_16x8str_hl(uint8_t uc_posx, uint8_t uc_posy, char uc_dataStr[]);

void oled_print_16x8short_hl(uint8_t uc_posx, uint8_t uc_posy, int16_t s_data, int8_t bits_num);

void oled_print_3_short(uint8_t uc_posx, uint8_t uc_posy, int16_t s_data);

void oled_print_image(volatile uint8_t usPicture[][IMAGE_NUM], uint16_t usRowNum, uint16_t usColumnNum);

long Limiter(long s_data, long min, long max);//将输入的数范围限制在min和max之间，超出的记做max或min 

#endif /* OLED_H_ */
