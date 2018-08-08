#ifndef __KEY_H__
#define __KEY_H__

#include "include.h"

#define key1  gpio_get (PTB3)
#define key2  gpio_get (PTB7)
#define key3  gpio_get (PTB11)
#define key4  gpio_get (PTB17)

extern float kp_tiao;
extern float kp_tiao2;
extern float kd_tiao;

void KEY_Init(void);
void Can_Change(void);

void KeyAdd2(float *data);
void Int_KeyAdd2(int *data);

void KeyMin2(float *data);
void Int_KeyMin2(int *data);

void KeyAdd1(float *data);
void Int_KeyAdd1(int *data);

void Reset(float *data);
#endif