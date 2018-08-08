#include "key.h"

uint8 UI_flag=0;
float  yuzhi=60;     //����������ֵ
extern float  sudu2; 
extern int line;
/*****************************************************
���ܣ����������뿪�س�ʼ��
��������
���أ���
��⣺
*****************************************************/
void KEY_Init(void)
{
  gpio_init(PTB3, GPI, 0); port_init_NoALT(PTB3, PULLUP);//K1  
  gpio_init(PTB7, GPI, 0); port_init_NoALT(PTB7, PULLUP);//K2  
  gpio_init(PTB11, GPI, 0); port_init_NoALT(PTB11, PULLUP);//K3  
  gpio_init(PTB17, GPI, 0); port_init_NoALT(PTB17, PULLUP);//K4   
  
  //gpio_init(PTB18,GPI, 0); port_init_NoALT(PTB18,PULLUP);//BM7  
  //gpio_init(PTB17,GPI, 0); port_init_NoALT(PTB17,PULLUP);//BM6  
  //gpio_init(PTB16,GPI, 0); port_init_NoALT(PTB16,PULLUP);//BM5  
  //gpio_init(PTB11,GPI, 0); port_init_NoALT(PTB11,PULLUP);//BM4     
  
  //   gpio_init(PTB10,GPI, 0); port_init_NoALT(PTB10,PULLUP);//BM3  
  //   gpio_init(PTB9, GPI, 0); port_init_NoALT(PTB9, PULLUP);//BM2  
  //   gpio_init(PTB8, GPI, 0); port_init_NoALT(PTB8, PULLUP);//BM1  
  //   gpio_init(PTB2, GPI, 0); port_init_NoALT(PTB2, PULLUP);//BM0      
}






/*****************************************************
���ܣ�����K1(A6) ÿ��һ�μ�2
��������
���أ���
��⣺
*****************************************************/
void KeyAdd2(float *data)
{	
  
  if(!key1)   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key1)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key1);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		if(UI_flag==0)
		  *data = *data+2;
		else if(UI_flag==1)
		  *data = *data+10;
		
	  }
	}	
  }
}

void Int_KeyAdd2(int *data)
{	
  
  if(!key1)   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key1)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key1);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		if(UI_flag==0)
		  *data = *data+2;
		else if(UI_flag==1)
		  *data = *data+10;
		
	  }
	}	
  }
}
/*****************************************************
���ܣ�����K2(A5) ÿ��һ�μ�2
��������
���أ���
��⣺
*****************************************************/
void KeyMin2(float *data)
{	
  
  if(!key2)   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key2)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key2);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		if(UI_flag==0)
		  *data = *data-2;
		else if(UI_flag==1)
		  *data = *data-10;
		
	  }
	}	
  }
}

void Int_KeyMin2(int *data)
{	
  
  if(!key2)   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key2)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key2);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		if(UI_flag==0)
		  *data = *data-2;
		else if(UI_flag==1)
		  *data = *data-10;
		
	  }
	}	
  }
}



/*****************************************************
���ܣ�����K3(E27) ÿ��һ�μ�1
��������
���أ���
��⣺
*****************************************************/
void KeyAdd1(float *data)
{	
  
  if(!key3)   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key3)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key3);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		if(UI_flag==0)
		  *data = *data+1;
		else if(UI_flag==1)
		  *data = *data+5;
	  }
	}	
  }
}

void Int_KeyAdd1(int *data)
{	
  
  if(!key3)   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key3)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key3);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		if(UI_flag==0)
		  *data = *data+1;
		else if(UI_flag==1)
		  *data = *data+5;
	  }
	}	
  }
}

/*****************************************************
���ܣ�����K4  ÿ��һ�ΰѴ˱�����0
��������
���أ���
��⣺
*****************************************************/
void Reset(float *data)
{
  
  if(!key4 )   //�����⵽�͵�ƽ��˵����������
  {		
	systick_delay_ms(10); //��ʱȥ����һ��10-20ms
	if(!key4)     //�ٴ�ȷ�ϰ����Ƿ��£�û�а������˳�
	{
	  while(!key4);//���ȷ�ϰ��°����ȴ������ͷţ�û�����˳�
	  {
		*data=0;
	  }
	}	
  }
}

/***********************************************
���ܣ�ͨ������ ���ڲ���
��������
���أ���
��⣺��
***********************************************/
void Can_Change(void)
{
  float flag = 1;
  
  LCD_CLS(0); //����
  LCD_P6x8Str(0,4,(uint8 *)"sudu2:");		
  while(flag)
  {
	UI_flag=1;
	LCD_three(20,4,(int16)sudu2);      
	KeyAdd2(&sudu2);
	KeyMin2(&sudu2);
	KeyAdd1(&sudu2);      
	if(sudu2<0)
	  sudu2=0;
	Reset(&flag);//��flag��0 
	UI_flag=0;
  }
  
  LCD_CLS(0); //����
  
  flag =1;
  LCD_P6x8Str(0,4,(uint8 *)"line:");		
  while(flag)
  {
//	UI_flag=2;
	LCD_three(20,4,(int16)line);
	Int_KeyAdd2(&line);
	Int_KeyMin2(&line);
	Int_KeyAdd1(&line);   
	if(line<0)
	  line=0;
	Reset(&flag);//��flag��0          
  }
  
  LCD_CLS(0); //����
  
  flag =1;
  LCD_P6x8Str(0,4,(uint8 *)"kp:");		
  while(flag)
  {
	LCD_three(18,4,(int16)kp_tiao);      
	KeyAdd2(&kp_tiao);
	KeyMin2(&kp_tiao);
	KeyAdd1(&kp_tiao);      
	if(kp_tiao<0)
	  kp_tiao=0;
	Reset(&flag);//��flag��0          
  }
  
  LCD_CLS(0); //����
  
  flag = 1;	
  LCD_P6x8Str(0,4,(uint8 *)"kd:");		
  while(flag)
  {
	LCD_three(18,4,(int16)kd_tiao);      
	KeyAdd2(&kd_tiao);
	KeyMin2(&kd_tiao);
	KeyAdd1(&kd_tiao);
	if(kd_tiao<0)
	  kd_tiao=0;
	Reset(&flag);//��flag��0
  }
  LCD_CLS(0); //����   
  
  flag = 1;	
  LCD_P6x8Str(0,4,(uint8 *)"yuzhi:");		
  while(flag)
  {
	LCD_three(40,4,(int16)yuzhi);      
	KeyAdd2(&yuzhi);
	KeyMin2(&yuzhi);
	KeyAdd1(&yuzhi);
	SCCB_WriteByte(OV7725_CNST,(int16)yuzhi);
	if(yuzhi<0)
	  yuzhi=0;
	Reset(&flag);//��flag��0
  }
  LCD_CLS(0); //����    
}