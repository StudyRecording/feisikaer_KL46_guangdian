#include "pid.h"

#define DUO_Rit 2450//舵机右边极限 
#define DUO_Mdl 3350//舵机中间值 
#define DUO_Lft 4250//舵机左边极限

float 	kp_tiao=0;    //舵机 按键调节kp
float 	kp_tiao2=0;    //舵机 按键调节kp，
float 	kd_tiao=0;   //舵机 按键调节kd，
uint32 duoji_pwm = 0;  //舵机PWM
int32 dianji_pwm=0; //电机占空比	
PID_duoji_type       PID_duoji;      //定义 舵机PID 结构体变量
PID_dianji_type      motor_PID;      //定义 电机PID 结构体变量

extern float std_speed;
extern uint32 pulse;
/******************************************************************************
功能：PID参数初始化
参数：无
返回：无
详解：无
*******************************************************************************/
void PID_init(void)
{
  //舵机 PID参数初始化
  PID_duoji.current_error = 0;
  PID_duoji.last_error = 0;
  PID_duoji.pre_error = 0;				
  PID_duoji.PID_Kp = kp_tiao;	
  PID_duoji.PID_Kd = kd_tiao; 
  PID_duoji.PID_Kp3 = kp_tiao2;	
  // PID_duoji.PID_Kd3 = kd_tiao3;
  //电机 PID参数初始化	
  motor_PID.current_error = 0;
  motor_PID.last_error = 0;
  motor_PID.pre_error = 0;
  motor_PID.PID_Kp = 300;//210    218 300
  motor_PID.PID_Ki = 30; //120    60  30
  motor_PID.PID_Kd = 80; //20     20  80
}

/*********************************************************************************************
功能：舵机PID控制函数
参数：摄像头采集图像拟合参数 差值
返回：无
详解：根据差值进行 PID 处理，并让舵机执行   
*********************************************************************************************/
void PID_DuoJi(int16 x)
{
  int32 P = 0,D = 0,duojiPWMtemp = 0; 
  duoji_pwm = 0;
  PID_duoji.last_error = PID_duoji.current_error; //更新上次差值
  x=0-x;
  PID_duoji.current_error = (float)x; //更新本次差值
  PID_duoji.PID_Kp3 = kp_tiao2                                                                                                                  ; 
  PID_duoji.PID_Kp = kp_tiao;//比例			
  PID_duoji.PID_Kd = kd_tiao; //微分		
  P = (int32)( PID_duoji.PID_Kp * (float)PID_duoji.current_error+ PID_duoji.PID_Kp3 * (float)(PID_duoji.current_error*ABS(PID_duoji.current_error))); //计算比例项	   
  D = (int32)( PID_duoji.PID_Kd * (float)( PID_duoji.current_error - PID_duoji.last_error ) ); //计算微分项
  duojiPWMtemp = DUO_Mdl + (P + D); //得到舵机的 PWM 值 （占空比）
  if(duojiPWMtemp<0)
  {
	duojiPWMtemp = 0;
  } //防止出现负数
  duoji_pwm = (uint32)duojiPWMtemp;	
  
  if( duoji_pwm >(uint32)DUO_Lft ) //限制舵机 过分偏左  
  {
	duoji_pwm = (uint32)DUO_Lft;
  } 	
  else if( duoji_pwm <(uint32)DUO_Rit )//限制舵机 过分偏右
  {
	duoji_pwm =(uint32) DUO_Rit; 
  } 
  
  tpm_pwm_duty(TPM1, TPM_CH1,duoji_pwm);     //FTM_PWM_ChangeDuty( HW_FTM1, HW_FTM_CH1,duoji_pwm );//舵机输出	
  
}

void PID_Motor(uint8_t ch)
{
  int32 P= 0, I= 0,D = 0;
  int32 dianjiPWMtemp=0;     
  
  motor_PID.pre_error  = motor_PID.last_error; //更新上上次左轮差值
  motor_PID.last_error = motor_PID.current_error; //更新上次左轮
  motor_PID.current_error = std_speed - pulse; //目标速度与实际速度的差
  
  if(ch==0)//停止
  {
	if(pulse>40)
	{
	  tpm_pwm_duty(TPM0,TPM_CH1,0);
	  tpm_pwm_duty(TPM0,TPM_CH0,1600);
	}
	else if(pulse<=40)
	{
	  tpm_pwm_duty(TPM0,TPM_CH1,0);
	  tpm_pwm_duty(TPM0,TPM_CH0,0);
	}
	return;	
	
  }
  
  
  
  P = (int32)(motor_PID.PID_Kp*(float)(motor_PID.current_error-motor_PID.last_error)); //计算 P
  I = (int32)(motor_PID.PID_Ki*(float) motor_PID.current_error); //计算 I
  D = (int32)(motor_PID.PID_Kd*(float)(motor_PID.current_error-2*motor_PID.last_error+motor_PID.pre_error)); //计算 D	
  dianjiPWMtemp = P + I + D + (int32)dianji_pwm; 		
  if(dianjiPWMtemp<0)
  {
	dianjiPWMtemp = 0;
  }    
  dianji_pwm = (uint16)dianjiPWMtemp;	
  if(dianji_pwm>4000) //极限限制
  {
	dianji_pwm=4000; 
  }
  if(dianji_pwm<1) //极限限制 
  {
	dianji_pwm=1; 
  }
  tpm_pwm_duty(TPM0,TPM_CH1,dianji_pwm);
  tpm_pwm_duty(TPM0,TPM_CH0,0);
}
