
/* ***********************************************************************
				Ϋ���Ƽ�ѧԺ���������������ͷ�飨��Ӱ�ӣ�

//����ͷSDA A16  SCL A17   PCLK A7   HREF A5   VSYNC A6  E16-E23
//OLED  SCL E2	 SDA E1    RST E4    DC E6
//���뿪��B18  B17  B16  B11  B10  B9  B8  B2   ��--��   �����뿪�ؽ�B10  B9  B8  B2��
//����	  B3    B7    B11    B17     K1--K4
//����    UART0  9600   RXD A15   TXD A14  
//���    TPM0_CH1_PC2    TPM0_CH2_PC3
//����IO  C6  C7  C8  C9
//���	  TPM1_CH1_PA13
//������  TPM_CLKIN0_PIN  PTB16            
//������  A20(B0B1D6D7)

                                                2018-7-31
												������

************************************************************************ */


#include "common.h"
#include "include.h"
#include "math.h"

int real_line=38;//�ø��еĲ�ֵ��Ϊ�����ǳ̶ȵ��ж�

float  std_speed=0;  //�����ٶ�
float  sudu=0;      //��ֵ�������ٶȵı���
int line=38;		//�������ڼ����
float sudu2=0;		//���������ٶȣ���������sudu2----->sudu2��ֵ��sudu------>sudu��ֵ��std_speed��

int16 last_ErrAll=0;//��¼��һ�εĴ�ǲ�ֵ

uint8 mid=0;		//��¼�е���ֵ
uint8 imgbuff[CAMERA_SIZE];                             //����洢����ͼ�������
uint8 video[OV7725_EAGLE_H][OV7725_EAGLE_W];//����洢ͼ��Ķ�ά����
//ѭ���������
uint8 i=0;
uint8 j=0;

int16  M_err[60]={0};    //���ߵ������ֵ
int16  Err_All = 0;      
//��Ϻ�Ĳ�ֵ
uint32 pulse = 0;     //��¼��������������
int8   flag_stop=0;//ͣ����־λ
int Ltime=0;//������ƫ��տ�ʼ��ʱ
int Rtime=0;//�һ�����ƫ�ҹտ�ʼ��ʱ
uint8 Start_Line=0;//���������
uint8 Start_Time=0;//��⵽�����ߺ��ʱ��
uint8 Can_Stop=0;//���Լ��ֹͣ��


//����ÿ���֡�������淴ӳ���������ٶȣ�
int frame=0,painting_count=0,time_count=0;

//���ͣ����־�Ĵ���
uint8 Stop_flage=0;

extern uint8 Lflage;
extern uint8 Rflage;
extern int16  shizi_flag[60];
extern volatile   signed short int l_edge[60];
extern volatile   signed short int r_edge[60];
extern int16  zy_flag[60];
extern float kp_tiao;
extern float kp_tiao2;
extern float kd_tiao;
extern float yuzhi;
extern uint8 shizi;
extern uint8 chuHuan;
extern uint8 Lthan;
extern uint8 Rthan;

extern int16 L_Last_WhiteErr;
extern int16 R_Last_WhiteErr;

extern TiaoPoint;

//��������
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PIT_IRQHandler();

/* *******************************************************************************
*	������
*	2018-7-31
*	������
*	���ذ壺KL46
*	ע�⣺��ʮ������������ܳ��������Ӧ�ó���
**********************************************************************************/

void main(void)
{
  DisableInterrupts;//�ر����ж�
  
  //�����ʼ��
  tpm_pwm_init(TPM1, TPM_CH1,200,3350); //3330  ��ת ����   ��ת ��С   //��ʼ�� ��� PWM 
  
  //�����ʼ��
  tpm_pwm_init(TPM0, TPM_CH1,10*1000,0); //��ʼ�� ��� PWM     
  tpm_pwm_init(TPM0, TPM_CH0,10*1000,0);
  
  tpm_pulse_init(TPM2,TPM_CLKIN0,TPM_PS_1); //��ʼ�� TPM2 Ϊ�����ۼӣ�����ܽ�Ϊ TPM_CLKIN0_PIN ����Ƶϵ��Ϊ 1
  
  //OLED��ʼ��
  LCD_Init();
  
  //LED��ʼ��
  led_init(LED3);
  led_init(LED0);
  led_init(LED2);
  //���������뿪�س�ʼ��
  KEY_Init();
  //��ʼ������ͷ
  camera_init(imgbuff);
  
  //�����жϷ�����
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //����LPTMR���жϷ�����Ϊ PORTA_IRQHandler
  //��ʱ���ж�
  pit_init_ms(PIT0, 9);  //��ʼ��PIT0����ʱʱ��Ϊ�� 9ms
  pit_init_ms(PIT1, 3);  //��ʼ��PIT1����ʱʱ��Ϊ�� 2ms
  set_vector_handler(PIT_VECTORn ,PIT_IRQHandler); //����PIT���жϷ�����Ϊ PIT_IRQHandler  
  
  //���pid
  sudu2=160;
  kp_tiao=23;
  kd_tiao=80;
  kp_tiao2=0;
  yuzhi=60;
  
  //��ʼ��PID
  PID_init();
  //����
  LCD_CLS(0);//����
  Can_Change();
  
  systick_delay_ms(500);//��ʱ
  
  LCD_CLS(0);//����
  enable_irq(PIT_IRQn);//ʹ�ܶ�ʱ��
  EnableInterrupts;  //�������ж�
  
  while(1)
  {
    
    //��ȡͼ��
    camera_get_img();//����ͷ��ȡͼ��
    img_extract((uint8 *)video, (uint8 *)imgbuff,CAMERA_SIZE);//��ѹͼ��
    painting_count++;//��ʾͼ����
	
    //��������58��57��ȫ�ڣ�ͣ��
    if(getpoint_h(0,79,58)>68&&getpoint_h(0,79,57)>68)
    {
      flag_stop=1;
    }
    
    //ͣ����ͣ��
    if(Start_Line==0||Can_Stop==1)//δ���������ߣ�����Լ��ͣ����ʱ���иò��ֳ���
    {
      for(i=45;i<55;i++)//���45--55��
      {
        TiaoPoint stop_TiaoPoint=Jump_along(0,i,3,77);//�õ����������ͺʹ���
        if(stop_TiaoPoint.White_to_black_Number>5&&stop_TiaoPoint.Black_to_white_Number>5)//������дӰ׵��ںʹӺڵ��׵�����㶼����5��
        {
          Stop_flage++;//ͣ����־������1
          if(Stop_flage>4)//���ͣ����־��������4��
          {
            if(Start_Line==1)//����Ѿ�������������
              flag_stop=1;//�ô��Ǽ�⵽��ͣ���ߣ�ͣ��
            else
              Start_Line=1;//�ôμ�⵽��������
            break;
          }
        }
      }
    }
    
	//����0--59�еĲ�ֵ           
    for(i=0;i<60;i++)//1.970ms  1.945
    {
      M_err[i] = middle_line(video[i],i);//����60������������
    }
    
    //�˲�����ȥ�󲿷ֲ�����Ĳ�ֵ
    lvbo();
	
    //ʮ�ֳ���
    crossroad(M_err);
    
	//�󻷳���
    L_HuanDao(M_err);
	//��⵽��Բ��֮ǰ����ļ�⵽����Բ����֮ǰ��Բ����־λδ����������Բ����ر�־λ��������Բ����ر�־λ
    if(Lflage==1&&Rflage==1)
    {
      Rflage=0;
      Rtime=0;
      Rthan=0;
      chuHuan=0;
	  
    }
    
	//�һ�����
    R_HuanDao(M_err);
	//��⵽��Բ��֮ǰ����ļ�⵽����Բ����֮ǰ��Բ����־λδ����������Բ����ر�־λ��������Բ����ر�־λ
    if(Lflage==1&&Rflage==1)
    {
      Lflage=0;
      Ltime=0;
      Lthan=0;
      chuHuan=0;
	  
    }
    

	//��������Ϊȫ�ڣ��ò�����ȷ�Ĳ�ֵ
    if(zy_flag[real_line-1]==4)
    {
	  led(LED2,LED_OFF);
	  if(Lflage==1)//����Ѿ���⵽����Բ��
	  {
		Err_All=L_Last_WhiteErr+5;//���в�ֵ��Ϊ��һ�ν���Բ������ղ�ֵ
	  }
	  else if(Rflage==1)//����Ѿ���⵽����Բ��
	  {
		Err_All=R_Last_WhiteErr-5;//���в�ֵ��Ϊ��һ�ν���Բ������ղ�ֵ
	  }
	  else//�������������µõ�������ֵ
      	Err_All=last_ErrAll;//�ôβ�ֵ��Ϊ�ϴβ�ֵ
    }
    else//������������
    {
	  
      real_line=line;//�������Ϊ�������ڵ�����
      Err_All=M_err[real_line];//�õ�����еĲ�ֵ
      if(ABS(Err_All)>15)//�������еĲ�ֵ�ϴ�>15�������¼�˴β�ֵ
        last_ErrAll=Err_All;
	  
    }
    
    
    PID_DuoJi(Err_All);//������
    
    
    //�������
    if(flag_stop==1)//ͣ��
    {
      std_speed=0;
    }
    else
    {
		//�����ٶ�Ϊ�������ڵ��ٶ�
		sudu=sudu2;
      std_speed=sudu;
    }

    
	//����������߱����ʾ
    for(i=0;i<60;i++)
    {
      mid=40-M_err[i];
      video[i][mid]=1;
      video[i][mid+1]=1;
      video[i][mid-1]=1;
    }
	
    for(i=0;i<79;i++)
    {
	  //��ʾ�����
      video[real_line][i]=1;
      
    }
    
    //OLED��ʾ
    ShowVideoBin1(video);//4.410ms	��ʾ����ͼ��
    //OLED_CCD_bin(47,7,video[30]);//450us
    LCD_three(0,1,Err_All); //��ʾƫ��
//	LCD_three(0,2,l_edge[20]); 
//    LCD_three(0,3,r_edge[20]);
    LCD_three(0,4,l_edge[real_line]);
    LCD_three(0,5,r_edge[real_line]);
//	LCD_three(0,6,L_Radius);
	LCD_three(0,7,frame);//��ʾÿ�����֡
    //LCD_three(0,7,record);
  }
}

/*!
*  @brief      ��ֵ��ͼ���ѹ���ռ� �� ʱ�� ��ѹ��
*  @param      dst             ͼ���ѹĿ�ĵ�ַ
*  @param      src             ͼ���ѹԴ��ַ
*  @param      srclen          ��ֵ��ͼ���ռ�ÿռ��С
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 colour[2] = {0, 1}; //0 �� 1 �ֱ��Ӧ����ɫ
  //ע������ͷ 0 ��ʾ ��ɫ��1��ʾ ��ɫ
  uint8 tmpsrc;
  while(srclen --)
  {
    tmpsrc = *src++;
    *dst++ = colour[ (tmpsrc >> 7 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 6 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 5 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 4 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 3 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 2 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 1 ) & 0x01 ];
    *dst++ = colour[ (tmpsrc >> 0 ) & 0x01 ];
  }
}


/******************************************************************
���ܣ�PIT�ж�
��������
���أ���
��⣺��ȡLPTMR������
******************************************************************/
void PIT_IRQHandler(void)
{   
  
  if(PIT_TFLG(PIT0)==1) 
  { 
    PIT_Flag_Clear(PIT0); //���жϱ�־
    pulse=tpm_pulse_get(TPM2);//�õ���������������
    tpm_pulse_clean(TPM2);
	//�����⵽�󻷣���ʼ�������ʱ��
    if(Lflage==1)
    {
      Ltime++;
    }
	//�����⵽�һ�����ʼ�������ʱ��
    if(Rflage==1)
    {
      Rtime++;
    }
	//�����⵽�������Ҳ�δ������ͣ����ʱ����ʼ����ʱ��
    if(Can_Stop==0&&Start_Line==1)
    {
      Start_Time++;
    }
   
	
    if(Ltime>500)
      Ltime=500;
    
    if(Rtime>500)
      Rtime=500;
    
	//����⵽�����ߺ�30 Start_Timeʱ��������ͣ����
    if(Start_Time>30)
    {
      Can_Stop=1;
    }
    
    
  }
  
  else if(PIT_TFLG(PIT1)==1) 
  { 
    
    PIT_Flag_Clear(PIT1); //���жϱ�־

    time_count++;//ʱ������
	if(time_count==333)//��ʱ�����ӵ�1sʱ
	{
	  frame=painting_count;//�õ�֡��
	  //�Լ���֡�ı�־λ����
	  painting_count=0;
	  time_count=0;
	}
    
    if(flag_stop==1)//ͣ��ʱ
    {
      PID_Motor(0);//�������ֹͣ
    }
    else
      PID_Motor(1);   //��������£����������ʻ
  }
}


/*!
*  @brief      PORTA�жϷ�����
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
  uint8 n = 0;    //���ź�
  uint32 flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;                                   //���жϱ�־λ
  
  n = 6;                                              //���ж�
  if(flag & (1 << n))                                 //PTA6�����ж�
  {
    camera_vsync();
  }
}

/*!
*  @brief      DMA0�жϷ�����
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
  camera_dma();
}