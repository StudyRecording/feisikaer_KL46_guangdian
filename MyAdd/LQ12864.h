#ifndef __LQ12864_H__
#define __LQ12864_H__
#include "common.h"
#include  "MKL_gpio.h"          //IO口操作
//#include  "MKL_spi.h"           //SPI
#define HANG 60 
#define LIE 80 
#define X_WIDTH 128
#define Y_WIDTH 64
typedef unsigned char byte;
typedef unsigned int word;






//#define LCD_DC 	PT(E,0,PDOR)  //
//#define LCD_RST	PT(E,4,PDOR)  //E26
//#define LCD_SDA	PT(E,1,PDOR)  //E
//#define LCD_SCL	PT(E,2,PDOR)  //E29 




//硬件SPI
#define LCD_DC 	PT(E,6,PDOR)  //
#define LCD_RST	PT(E,4,PDOR)  //E26
#define SPI_Send_Byte(Data)  spi_mosi(SPI1,SPI_PCS1,&Data,NULL,1); 




void LCD_Init(void);
void LCD_CLS(unsigned char y1);
void LCD_Init(void);
void LCD_Fill(unsigned char bmp_dat);
void Draw_point(unsigned char x,unsigned char y);
void LCD_CLS(unsigned char y1);
void LCD_P6x8Str(unsigned char x,unsigned char y,unsigned char ch[]);
void LCD_P8x16Str(byte x,byte y,byte ch[]);
void LCD_P14x16Str(byte x,byte y,byte ch[]);
void LCD_Print(byte x, byte y, byte ch[]);
void LCD_one(unsigned char x,unsigned char y,int16 shuju);
void LCD_two(unsigned char x,unsigned char y,int16 shuju);
void LCD_three(unsigned char x,unsigned char y,int16 shuju);
void LCD_four(unsigned char x,unsigned char y,int16 shuju);
void OLED_CCD_bin(unsigned char x,unsigned char y,unsigned char *bin);
void OLED_CCD_wave(uint8_t y, uint8_t high, uint8_t *img, uint8_t maxval);
void ShowVideoBin1(unsigned char a[HANG][LIE]);
void zhongxian();
void LCD_CLS1();

#endif



//      LCD_one(0,0,1); //905us       //100us
//      LCD_two(0,1,11);//1.365ms     //157us
//      LCD_three(0,2,111);//1.845ms  //218.5us
//      LCD_four(0,3,1111);//2.315ms  //285.5us
//      LCD_P14x16Str(0,4,(byte *)"齐鲁工业大学"); //11.22ms  //1.225ms
//      OLED_CCD_bin(0,0,a[0]);//4.190ms  //450us
//      ShowVideoBin1(a);//28.85ms   //4.41ms
