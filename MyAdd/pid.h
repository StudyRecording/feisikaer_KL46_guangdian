

/**
  ******************************************************************************
  * @file    pid.h
  * @author  HELONE
  * @version V1.0
  * @date    2015.1.21
  * @brief   www.beyondcore.net   http://upcmcu.taobao.com 
  * @note    ���ļ�Ϊ PID ģ��ͷ�ļ�
  ******************************************************************************
*/

#ifndef __PID_H__
#define __PID_H__
#include "include.h"

extern float  std_speed;
//���PID �ṹ������
typedef struct
{
  float pre_error;    //���ϴ�  ��ֵ
  float last_error;    //�ϴ�    ��ֵ
  float current_error; //����    ��ֵ
  
  float PID_Kp ; //���� ϵ��
  float PID_Kp3 ; //���� ϵ��
  float PID_Kd ; //΢�� ϵ��
  float PID_Kd3 ; //΢�� ϵ��
	
} PID_duoji_type; //���PID �ṹ��
extern PID_duoji_type       PID_duoji;      //������PID �ṹ�����

//���PID�ṹ������
typedef struct
{
	float  pre_error;    //���ϴ�  ��ֵ
	float  last_error;    //�ϴ�    ��ֵ
	float  current_error; //����    ��ֵ
	
	float PID_Kp ; //���� ϵ��
	float PID_Ki ; //���� ϵ��
	float PID_Kd ; //΢�� ϵ��
	
} PID_dianji_type; //���PID �ṹ��
extern PID_dianji_type      motor_PID;          //���� ��� ���PID �ṹ�����

void PID_init(void);
void PID_DuoJi(int16 x);
void PID_Motor(uint8_t ch);
#endif  //__PID_H__
