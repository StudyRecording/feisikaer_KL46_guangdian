#ifndef __VIDEO_H__
#define __VIDEO_H__
#include "include.h"

typedef struct //TiaoPoint
{
  uint8 Black_to_white_Number;//黑到白的跳变点数
  uint8 White_to_black_Number;//白到黑的跳变点数
  //uint8 Tiao_Point=0;          //0 行跳变点，1 列跳变点
  uint8 Type;//1 由白变黑  2 由黑变白
}TiaoPoint;


signed short int middle_line( unsigned char *bin_point,unsigned char CJ_everyline);
void lvbo(void);
int16 getpoint_h(uint8_t x1,uint8_t x2, uint8_t y);
int16 getpoint(uint8_t y1,uint8_t y2,uint8_t x);

TiaoPoint Jump_along(uint8 Type,uint8_t Single,uint8_t Double1,uint8_t Double2);
void crossroad(int16_t *zhongzhi);
void L_HuanDao(int16_t *zhongzhi);
void R_HuanDao(int16_t *zhongzhi);
#endif 