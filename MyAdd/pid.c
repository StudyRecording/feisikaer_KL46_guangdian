#include "pid.h"

#define DUO_Rit 2450//����ұ߼��� 
#define DUO_Mdl 3350//����м�ֵ 
#define DUO_Lft 4250//�����߼���

float 	kp_tiao=0;    //��� ��������kp
float 	kp_tiao2=0;    //��� ��������kp��
float 	kd_tiao=0;   //��� ��������kd��
uint32 duoji_pwm = 0;  //���PWM
int32 dianji_pwm=0; //���ռ�ձ�	
PID_duoji_type       PID_duoji;      //���� ���PID �ṹ�����
PID_dianji_type      motor_PID;      //���� ���PID �ṹ�����

extern float std_speed;
extern uint32 pulse;
/******************************************************************************
���ܣ�PID������ʼ��
��������
���أ���
��⣺��
*******************************************************************************/
void PID_init(void)
{
  //��� PID������ʼ��
  PID_duoji.current_error = 0;
  PID_duoji.last_error = 0;
  PID_duoji.pre_error = 0;				
  PID_duoji.PID_Kp = kp_tiao;	
  PID_duoji.PID_Kd = kd_tiao; 
  PID_duoji.PID_Kp3 = kp_tiao2;	
  // PID_duoji.PID_Kd3 = kd_tiao3;
  //��� PID������ʼ��	
  motor_PID.current_error = 0;
  motor_PID.last_error = 0;
  motor_PID.pre_error = 0;
  motor_PID.PID_Kp = 300;//210    218 300
  motor_PID.PID_Ki = 30; //120    60  30
  motor_PID.PID_Kd = 80; //20     20  80
}

/*********************************************************************************************
���ܣ����PID���ƺ���
����������ͷ�ɼ�ͼ����ϲ��� ��ֵ
���أ���
��⣺���ݲ�ֵ���� PID �������ö��ִ��   
*********************************************************************************************/
void PID_DuoJi(int16 x)
{
  int32 P = 0,D = 0,duojiPWMtemp = 0; 
  duoji_pwm = 0;
  PID_duoji.last_error = PID_duoji.current_error; //�����ϴβ�ֵ
  x=0-x;
  PID_duoji.current_error = (float)x; //���±��β�ֵ
  PID_duoji.PID_Kp3 = kp_tiao2                                                                                                                  ; 
  PID_duoji.PID_Kp = kp_tiao;//����			
  PID_duoji.PID_Kd = kd_tiao; //΢��		
  P = (int32)( PID_duoji.PID_Kp * (float)PID_duoji.current_error+ PID_duoji.PID_Kp3 * (float)(PID_duoji.current_error*ABS(PID_duoji.current_error))); //���������	   
  D = (int32)( PID_duoji.PID_Kd * (float)( PID_duoji.current_error - PID_duoji.last_error ) ); //����΢����
  duojiPWMtemp = DUO_Mdl + (P + D); //�õ������ PWM ֵ ��ռ�ձȣ�
  if(duojiPWMtemp<0)
  {
	duojiPWMtemp = 0;
  } //��ֹ���ָ���
  duoji_pwm = (uint32)duojiPWMtemp;	
  
  if( duoji_pwm >(uint32)DUO_Lft ) //���ƶ�� ����ƫ��  
  {
	duoji_pwm = (uint32)DUO_Lft;
  } 	
  else if( duoji_pwm <(uint32)DUO_Rit )//���ƶ�� ����ƫ��
  {
	duoji_pwm =(uint32) DUO_Rit; 
  } 
  
  tpm_pwm_duty(TPM1, TPM_CH1,duoji_pwm);     //FTM_PWM_ChangeDuty( HW_FTM1, HW_FTM_CH1,duoji_pwm );//������	
  
}

void PID_Motor(uint8_t ch)
{
  int32 P= 0, I= 0,D = 0;
  int32 dianjiPWMtemp=0;     
  
  motor_PID.pre_error  = motor_PID.last_error; //�������ϴ����ֲ�ֵ
  motor_PID.last_error = motor_PID.current_error; //�����ϴ�����
  motor_PID.current_error = std_speed - pulse; //Ŀ���ٶ���ʵ���ٶȵĲ�
  
  if(ch==0)//ֹͣ
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
  
  
  
  P = (int32)(motor_PID.PID_Kp*(float)(motor_PID.current_error-motor_PID.last_error)); //���� P
  I = (int32)(motor_PID.PID_Ki*(float) motor_PID.current_error); //���� I
  D = (int32)(motor_PID.PID_Kd*(float)(motor_PID.current_error-2*motor_PID.last_error+motor_PID.pre_error)); //���� D	
  dianjiPWMtemp = P + I + D + (int32)dianji_pwm; 		
  if(dianjiPWMtemp<0)
  {
	dianjiPWMtemp = 0;
  }    
  dianji_pwm = (uint16)dianjiPWMtemp;	
  if(dianji_pwm>4000) //��������
  {
	dianji_pwm=4000; 
  }
  if(dianji_pwm<1) //�������� 
  {
	dianji_pwm=1; 
  }
  tpm_pwm_duty(TPM0,TPM_CH1,dianji_pwm);
  tpm_pwm_duty(TPM0,TPM_CH0,0);
}
