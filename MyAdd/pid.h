

/**
  ******************************************************************************
  * @file    pid.h
  * @author  HELONE
  * @version V1.0
  * @date    2015.1.21
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    此文件为 PID 模块头文件
  ******************************************************************************
*/

#ifndef __PID_H__
#define __PID_H__
#include "include.h"

extern float  std_speed;
//舵机PID 结构体声明
typedef struct
{
  float pre_error;    //上上次  差值
  float last_error;    //上次    差值
  float current_error; //本次    差值
  
  float PID_Kp ; //比列 系数
  float PID_Kp3 ; //比列 系数
  float PID_Kd ; //微分 系数
  float PID_Kd3 ; //微分 系数
	
} PID_duoji_type; //舵机PID 结构体
extern PID_duoji_type       PID_duoji;      //定义舵机PID 结构体变量

//电机PID结构体声明
typedef struct
{
	float  pre_error;    //上上次  差值
	float  last_error;    //上次    差值
	float  current_error; //本次    差值
	
	float PID_Kp ; //比列 系数
	float PID_Ki ; //积分 系数
	float PID_Kd ; //微分 系数
	
} PID_dianji_type; //电机PID 结构体
extern PID_dianji_type      motor_PID;          //定义 左边 电机PID 结构体变量

void PID_init(void);
void PID_DuoJi(int16 x);
void PID_Motor(uint8_t ch);
#endif  //__PID_H__
