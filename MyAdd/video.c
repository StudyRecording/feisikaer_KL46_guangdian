#include "video.h"
#include "math.h"





uint8 shizi=0;//ʮ�ֱ�־λ
int16  zy_flag[60]={0};//60�е����Һ��߼�����
int16 area0[60]={0};         //����ߺڵ�������
int16 area2[80]={0};        //�����ڵ�������
int16  shizi_flag[60]={0};//ȫ�ױ�־λ
uint8 Lflage=0;//��Բ�������ڱ�־λ
uint8 Rflage=0;//��Բ�������ڱ�־λ
uint8 chuHuan=0;//���������־λ

uint8 Lthan=0;//�󻷽�����δ�����ı�־λ
uint8 Rthan=0;//�һ�������δ�����ı�־λ
int16 L_Last_WhiteErr=0;//�����ʱ������ʱ�ϴ���ղ�ֵ
int16 R_Last_WhiteErr=0;//�ҽ���ʱ�����һ�ʱ�ϴ��ҹղ�ֵ

//60�е����Եֵ
volatile   signed short int l_edge[60]={0};
//60�е��ұ�Եֵ
volatile   signed short int r_edge[60]={80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};
//�ϴ�60�����Եֵ
volatile  static signed short int pre_l_edge[60] ={0};
//�ϴ�60���ұ�Եֵ
volatile  static signed short int pre_r_edge[60]={80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};

extern float sudu;
extern int Ltime;
extern int Rtime;
extern int real_line;
extern int16  M_err[60];
extern uint8 video[OV7725_EAGLE_H][OV7725_EAGLE_W];
/******************************************************************
���ܣ������������ߵ�������
��������
���أ����߲�ֵ
��⣺��������ߵ������ߣ����������������ߵĲ�ֵ,��ֵΪ��ֵ����գ���ֵ���ҹ�
******************************************************************/
signed short int middle_line( unsigned char *bin_point,unsigned char CJ_everyline)
{
  
  
  volatile unsigned char i ;//forѭ������
  volatile unsigned char BL_flag = 0X00; //��¼�˴κ��߲�ѯ״̬	
  static signed short int mid_now = 40;		//�����е�ֵ									
  volatile signed short int cha_zhi = 0;//��ֵ
  //��̬������
  //��ѯ����λ�õ� ��ʼ���ҵ�	
  volatile static signed short int find_p[60]  = {40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40};											
  
  //�ϴβ�ֵ
  volatile static signed short int pre_cha[60] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  // ��¼�G��ǰ�ε����Һ��߲�ѯ״̬
  volatile static unsigned char  LoseL_BL_flag[60] = {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};  
  
  volatile static unsigned char  out_loop[60] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//
  
  //����������Һ��ߵ�λ��
  l_edge[CJ_everyline]=0;
  r_edge[CJ_everyline]=80;
  
//      if(pre_cha[12] >10 )// 10
//        Find_Flag = 1;
//      else if( pre_cha[12] < (-10) )// -10
//        Find_Flag = 2;
//      if(Find_Flag == 1)
//      {	
//        find_p[0] = 3;
//        find_p[1] = 3;
//        find_p[2] = 3;
//        find_p[3] = 3;
//        find_p[4] = 3;
//        find_p[5] = 3;
//        find_p[6] = 3;
//      }
//      else if(Find_Flag==2)
//      {
//        find_p[0] = 77;
//        find_p[1] = 77;
//        find_p[2] = 77;
//        find_p[3] = 77;
//        find_p[4] = 77;
//        find_p[5] = 77;
//        find_p[6] = 77;
//      }
//  
  
  //��ֹ ���ҵ� ����
  if(find_p[CJ_everyline]<3)
	find_p[CJ_everyline]=3;	
  if(find_p[CJ_everyline]>76)
	find_p[CJ_everyline]=76;
  
  //�����������ͼ���м�Ϊ����
  if( bin_point[find_p[CJ_everyline]]==1 )
  {     
	//�ҹ�ʱ�������
	for(i = find_p[CJ_everyline]; i<77; i++)
	{
	  if((bin_point[i]==1)&&(bin_point[i+1]==0)&&(bin_point[i+2]==0)&&(bin_point[i+3]==0))//
	  {
		l_edge[CJ_everyline] = i+1;  
		BL_flag = BL_flag|0X10;//�˴���ע���ҵ���߽�	   	
		break;
	  }
	}
	
	//���ʱ���ұ���
	for(i =find_p[CJ_everyline]; i>2; i--)
	{
	  if((bin_point[i]==1)&&(bin_point[i-1]==0)&&(bin_point[i-2]==0)&&(bin_point[i-3]==0))//
	  {
		r_edge[CJ_everyline] = i-1;
		BL_flag = BL_flag|0X01;//�˴���ע���ҵ��ұ߽�
		break;
	  }
	}
	
  } 
  else//���������ͼ���м�Ϊ��ɫ
	
	//    if( bin_point[find_p[CJ_everyline]]==0 )      
	
  {   //�ж���ߺ���λ��	  
	for(i = (unsigned char)find_p[CJ_everyline]; i>2; i--)
	{				
	  if( (bin_point[i]==0) && (bin_point[i-1]==1) &&(bin_point[i-2]==1)&&(bin_point[i-3]==1))//
	  {
		l_edge[CJ_everyline] = i-1;
		BL_flag = BL_flag|0X10;//�˴���ע���ҵ���߽�
		break;
	  }						
	}
	//�ж��ұߺ���λ��
	for(i = (unsigned char)find_p[CJ_everyline]; i<77; i++)
	{
	  if( (bin_point[i]==0) && (bin_point[i+1]==1) && (bin_point[i+2]==1)&& (bin_point[i+3]==1))//
	  {
		r_edge[CJ_everyline] = i+1;
		BL_flag = BL_flag|0X01;//�˴���ע���ҵ��ұ߽�
		break;
	  }
	}
  }	
  
  //�жϱ���ֵ
  switch(BL_flag)
  {
  case 0x00: //���߶�û����
	{	
	  
	  
	  if((bin_point[3]==1)&&(bin_point[76]==1))//ȫ�ڣ����߶�����
	  {	
		zy_flag[CJ_everyline]=4;
		shizi_flag[CJ_everyline]=0;
		
		return 0;
	  }
	  else if((bin_point[3]==0)&&(bin_point[46]==0)&&(bin_point[76]==0))//ȫ��
	  {   
		zy_flag[CJ_everyline]=0;
		
		shizi_flag[CJ_everyline]=1;
		return 0;
	  }
	  
	}
  case 0x01://ֻ���ұߺ���
	{	
	  
	  
	  l_edge[CJ_everyline] = pre_l_edge[CJ_everyline] +(r_edge[CJ_everyline] - pre_r_edge[CJ_everyline]);//�����ұ����ص���������            
	  
	  
	  zy_flag[CJ_everyline]=3;
	  shizi_flag[CJ_everyline]=0;
	  break;
	}
  case 0x10: //ֻ����ߺ���
	{  
	  r_edge[CJ_everyline] = pre_r_edge[CJ_everyline] + (l_edge[CJ_everyline] - pre_l_edge[CJ_everyline]);//����������ص�����ұ���
	  shizi_flag[CJ_everyline]=0;
	  zy_flag[CJ_everyline]=2;
	  break;
	}
  case 0x11: //���ߺ��߶���
	{
	  
	  zy_flag[CJ_everyline]=1;
	  shizi_flag[CJ_everyline]=0;
	  break;
	}
  default : 
	{
	  
	  break;
	}
  }
  
  mid_now = (l_edge[CJ_everyline] + r_edge[CJ_everyline])/2; //����м�ֵ	
  
  
  cha_zhi = 40 - mid_now; //�����ֵ,�����             
  
  if( (BL_flag==0x01)||(BL_flag==0x10) )//���G��ǰ�ĺ���״̬	
  {
	LoseL_BL_flag[CJ_everyline] = BL_flag;
  }	
  
  find_p[CJ_everyline] = mid_now;//�´β��ҵ�
  pre_cha[CJ_everyline] = cha_zhi;//�����ϴβ�ֵ	
  pre_r_edge[CJ_everyline] = r_edge[CJ_everyline];//������߽���
  pre_l_edge[CJ_everyline] = l_edge[CJ_everyline];//�����ұ߽���
  return cha_zhi ;//���ص�ǰ�Ĳ�ֵ
  
}

/*******************************************************************************
�������ƣ�lvbo
��������: �˲�
������
*******************************************************************************/
void lvbo(void)
{
  uint8 line;
  for(line=5;line<55;line++)
  {
	
	if((M_err[line]>M_err[line-2])&&(M_err[line]>M_err[line+2]))
	{
	  M_err[line]=(M_err[line-2]+M_err[line+2])/2;
	  
	}
	else if((M_err[line]<M_err[line-2])&&(M_err[line]<M_err[line+2]))
	{
	  M_err[line]=(M_err[line-2]+M_err[line+2])/2;
	  
	}
  }
}

/*************
���ܣ��õ���Ӧ�кڵ���
������y1 y��㣬y2 y�յ� ��x
���أ��ڵ���
��⣺��
**************/
int16 getpoint_h(uint8_t x1,uint8_t x2, uint8_t y)
{
  uint8_t i=0;
  area0[y]=0;
  for(i=x1;i<=x2;i++)
  {
	area0[y]=area0[y]+video[y][i];//�ۼӺڵ������൱�����ӷ����㣩
  }
  return area0[y];
  
}

/*************
���ܣ��õ����Ӧ�ڵ���
������x1 x��㣬x2 x�յ� ��y 
���أ��ڵ���
��⣺��
**************/
int16 getpoint(uint8_t y1,uint8_t y2,uint8_t x)

{
  uint8_t i=0;
  area2[x]=0;
  for(i=y1;i<=y2;i++)
  {
	area2[x]=area2[x]+video[i][x];//�ۼӺڵ������൱�����ӷ����㣩
  }
  
  return area2[x];
  
}

/**************************************************************************************
�������

������
Type:                 0 ������㣬1 �������
Single��              Type=0ʱ��Ϊ������Type=1ʱ��Ϊ����
Double1��Double2      Type=0ʱ��Ϊ������Double1<Double2����Type=1ʱ��Ϊ������Double1<Double2��
���أ�
TiaoPoint����
*****************************************************************************************/
TiaoPoint Jump_along(uint8 Type,uint8_t Single,uint8_t Double1,uint8_t Double2)
{
  TiaoPoint My_tiaoPoint;
  int i=0;
  if(Type==0)
  {
	for(i=Double1;i<=Double2;i++)
	{
	  if(video[Single][i-1]==0&&video[Single][i]==0&&video[Single][i+1]==1&&video[Single][i+2]==1)
	  {
		My_tiaoPoint.White_to_black_Number++;
		My_tiaoPoint.Type=1;//�ɰױ��
	  }
	  if(video[Single][i-1]==1&&video[Single][i]==1&&video[Single][i+1]==0&&video[Single][i+2]==0)
	  {
		My_tiaoPoint.Black_to_white_Number++;
		My_tiaoPoint.Type=2;//�ɺڱ��
	  }
	}
  }
  else if(Type==1)
  {
	for(i=Double1;i<=Double2;i++)
	{
	  if(video[i-1][Single]==0&&video[i][Single]==0&&video[i+1][Single]==1&&video[i+2][Single]==1)
	  {
		My_tiaoPoint.White_to_black_Number++;
		My_tiaoPoint.Type=1;//�ɰױ��
	  }
	  if(video[i-1][Single]==1&&video[i][Single]==1&&video[i+1][Single]==0&&video[i+2][Single]==0)
	  {
		My_tiaoPoint.Black_to_white_Number++;
		My_tiaoPoint.Type=2;//�ɺڱ��
	  }
	}
  }
  return My_tiaoPoint;
  
}

/******************************************************************
���ܣ�ʮ��ʶ�𼰲���
��������
���أ���
��⣺��
******************************************************************/

void crossroad(int16_t *zhongzhi) 
{
  
  //�����λ�ڹյ�ǰ
  volatile static uint8_t TiaoPnt=0;//����� ����������
  volatile static  uint8_t GuaiPnt=0;//�յ�   ����������
  uint8 TiaoPnt_Flag = 0;//�ҵ�  ����� �ı�־
  uint8 GuaiPnt_Flag= 0; //�ҵ�  �յ�   �ı�־
  uint8 i=0,m=0;	//���ڼ���	
  int16 Guai_Xielv=0,Tiao_Xielv=0;
  
  if((shizi_flag[real_line]==1)||(shizi_flag[real_line+1]==1)||(shizi_flag[real_line-1]==1)||(shizi_flag[real_line-2]==1)||(shizi_flag[real_line+2]==1))
  {
	
	for(i=real_line;i>real_line-15;i--)
	{
	  if((shizi_flag[i]!=1)&&(shizi_flag[i-1]!=1))
	  {
		TiaoPnt_Flag=1;
		TiaoPnt=i-2;
		break;    
	  }    
	}
	for(i=real_line+1;i<real_line+15;i++)
	{
	  if((shizi_flag[i]!=1)&&(shizi_flag[i+1]!=1))
	  {
		GuaiPnt_Flag=1;
		GuaiPnt=i+2;
		break;    
	  }    
	}  
	if((TiaoPnt_Flag==1)&&( GuaiPnt_Flag==1))
	{
	  
	  for(m=TiaoPnt+1; m<GuaiPnt; m++) 
	  {
		zhongzhi[m] = (int)( 1.0*((float)zhongzhi[TiaoPnt] -( ((float)(m-TiaoPnt)/(float)(GuaiPnt-TiaoPnt)) * (float)(zhongzhi[TiaoPnt]-zhongzhi[GuaiPnt]) )) );
		l_edge[m] = (int)( 1.0*((float)l_edge[TiaoPnt] -( ((float)(m-TiaoPnt)/(float)(GuaiPnt-TiaoPnt)) * (float)(l_edge[TiaoPnt]-l_edge[GuaiPnt]) )) );
		r_edge[m] = (int)( 1.0*((float)r_edge[TiaoPnt] -( ((float)(m-TiaoPnt)/(float)(GuaiPnt-TiaoPnt)) * (float)(r_edge[TiaoPnt]-r_edge[GuaiPnt]) )) );
	  }
	  shizi=1;
	  
	  
	}
	else if((TiaoPnt_Flag==1)&&( GuaiPnt_Flag==0))
	{
	  
	  
	  Tiao_Xielv = ( zhongzhi[TiaoPnt+1] - zhongzhi[TiaoPnt-1])/2;
	  for(m=TiaoPnt+1; m<real_line; m++) 
	  {
		
		zhongzhi[m] = (int)( (float)(zhongzhi[TiaoPnt] + (m-TiaoPnt)*Tiao_Xielv )*1.0 );
		l_edge[m] = (int)( (float)(l_edge[TiaoPnt] + (m-TiaoPnt)*Tiao_Xielv )*1.0 );
		r_edge[m] = (int)( (float)(r_edge[TiaoPnt] + (m-TiaoPnt)*Tiao_Xielv )*1.0 );
		
	  }
	  shizi=1;
	  
	}
	else if((TiaoPnt_Flag==0)&&( GuaiPnt_Flag==1))
	{
	  
	  Guai_Xielv = ( zhongzhi[GuaiPnt] - zhongzhi[GuaiPnt+2] )/2;
	  for(m=real_line-15; m<GuaiPnt-1; m++) 
	  {
		zhongzhi[m] = (int)( (float)(zhongzhi[GuaiPnt] +(GuaiPnt-m)*Guai_Xielv )*1.0 );
		l_edge[m] = (int)( (float)(l_edge[GuaiPnt] +(GuaiPnt-m)*Guai_Xielv )*1.0 );
		r_edge[m] = (int)( (float)(r_edge[GuaiPnt] +(GuaiPnt-m)*Guai_Xielv )*1.0 );        
	  }  
	  shizi=1;
	 
	}
	else
	{
	  shizi=0;
	  return;
	}
	
	
  }
  else
	shizi=0;
  
}




/*******************************************************************************
�󻷵�



********************************************************************************/
void L_HuanDao(int16_t *zhongzhi)
{
  
  
  uint8 h=23;//����������ߵĲ�ֵ
  uint8 jing_tiao=0;
  
  uint8 L_flage=0;
  
  uint8 Pian_L1=0,Pian_L2=0;
  
  uint8 CHuan=0;
  uint8 C_h=0;
  uint8 Black_count=0,White_Count=0;
  
  uint8 i=0,m=0,k=0;
  uint8 Gui=0,Tiao=0;//�кڱ��   �аױ��
  uint8 GPoint=0,TPoint=0;
  
  uint8 B_Guai=0,B_Tiao=0;
  uint8 B_GPoint=0,B_TPoint=0;
  
  if(Lflage==0)//����Բ����־Ϊ0��δ��⵽��Բ��ʱ��
  {
	/******����Բ�����м��**********/
	
	//��һ���������ƫ���ʱ�򣬼���38��ǰ��ֻ���ұ��ߣ�����������ֻ���ұ��ߣ�
	//                               38�к�ֻ������ߣ�����������֮ǰ��ֻ������ߣ�
	for(i=real_line;i>16;i--)
	{
	  if(zy_flag[i]==2&&zy_flag[i-1]==3&&zy_flag[i-2]==3)
	  {
		Pian_L1=1;
		k=i-1;
		break;
	  }
	}
	
	for(i=k;i<55;i++)
	{
	  if(zy_flag[i]==3&&zy_flag[i+1]==2&&zy_flag[i+2]==2)
	  {
		Pian_L2=1;
		break;
	  }
	}
	
  }
  
  //�ڶ���������������ڴ�����ֻ�������
  for(i=real_line-4;i<45;i++)
  {
	if(zy_flag[i-1]==3&&zy_flag[i]==3)
	{
	  L_flage=1;
	  k=i;
	  break;
	}
  }
  
  
  /******************���ϵ�һ�������������ʼ�ж������*************************/
  if((Pian_L1==1&&Pian_L2==1)||L_flage==1||Lflage==1)
  {
	//�ɰױ�ڵ�������
	for(i=k;i>15;i--)
	{
	  if(zy_flag[i]==3&&zy_flag[i-1]==1&&zy_flag[i-2]==1)
	  {
		Tiao=1;
		TPoint=i-1;
		break;
	  }  
	  
	}
	
	//�ɺڱ�׵�������
	for(i=k;i<56;i++)
	{
	  if(zy_flag[i]==3&&zy_flag[i+1]==1&&zy_flag[i+2]==1)
	  {
		
		Gui=1;
		GPoint=i+1;
		break;
	  }
	  
	}
	
  }
  
  
  
  /**************�����ڳ������ڸ���ʱ*************************/
  if(Gui==1&&Tiao==1&&Lthan==0)
  {
	led(LED3,LED_ON);
	zhongzhi[real_line]=40-(r_edge[real_line]-30);//�������м�
	
	Ltime=0;
	Lflage=1;//��ʼ��ʱ�ı�־�����뻷���ı�־
	
	
  }
  else if(Lflage==1&&(zy_flag[real_line]==1||Lthan==1))/***����Ҫ�����������ͽ���֮��***/
  {
	
	
	//����Ƿ����
	  for(m=real_line;m<55;m++)
	  {
		if((zy_flag[m-1]==0||zy_flag[m-2]==0||zy_flag[m-3]==0||zy_flag[m-4]==0||zy_flag[m-5]==0)&&zy_flag[m]==3&&zy_flag[m+1]==3)
		{
		  C_h=1;//������־
		  led(LED2,LED_ON);
		  break;
		}
		  
	  }
	

	if(C_h==0)//��������־δ����ʱ������Ƿ��ǽ����ĵ�һ������״̬������һ�ֳ�������
	{
	  for(m=real_line;m>10;m--)//����Ƿ����
	  {
		if(zy_flag[m]==0&&zy_flag[m+1]==0&&zy_flag[m-1]!=1)
		{
		  C_h=1;
		  break;
		}
		
	  }
	}
	
	
	if(C_h==0)//��Ȼδ��⵽����ʱ������Ƿ��ǵ����ֳ�������
	{
	  for(m=9;m<real_line-1;m++)
	  {
		if(zy_flag[m]==4)
		  Black_count++;
		if(Black_count>2)
		{
		  Black_count=3;
		  break;
		}
			
	  }
	  
	  for(m=real_line;m<55;m++)
	  {
		if(zy_flag[m]==0)
		  White_Count++;
		if(White_Count>4)
		{
		  White_Count=5;
		  break;
		}
			
	  }
	  
	  if(Black_count>2&&White_Count>4)
	  {
		C_h=1;
		led(LED2,LED_ON);
	  }
		
	  
	}
	
	if(C_h==1)//��������ȷ��
	{
	  
	  //����

	  
	  	zhongzhi[real_line]+=15;//�������󻷣�����

	}
	else//����״̬
	{
	  
	  //��⵽����кͽ�����֮��ľ��벢�ҽ�һ��ȷ�Ͻ���
	  for(m=real_line;m>7;m--)
	  {
		if(video[m][3]==1&&video[m-1][3]==0&&video[m-2][3]==0)
		{
		  i=real_line-m;
		  jing_tiao=1;
		  break;
		}
	  }
	  //�����⵽����״̬ʱ50 Ltime�󣬽���״̬���
	  if(Ltime>50)
	  {
		jing_tiao=0;
	  }
	  
	  //���ݼ���кͽ����ڵľ��룬ȷ�ϴ�ǳ̶�
	  if(jing_tiao==1)
	  {
		if(i>15)//�����û���Զ��������΢�������ʻ
		  h=22;
		else if(i>10&&i<=15)
		  h=21;
		else if(i>5&&i<=10)
		  h=20;
		else if(i>0&&i<=5)//����������ǳ����ˣ���ʼʹ��������ʻ
		  h=19;
		zhongzhi[real_line]=40-(l_edge[real_line]+h);//�����ֵ
	  }
	  else//������֮���ڽ����ںͳ�����֮�����ջ�������������ʻ
	  {

		if(Ltime<30)//����δ����ʱ�����⣬ȷ�����Ŀ�����ʻ��������
		  h=22;
		else
		  h=30;
		zhongzhi[real_line]=40-(l_edge[real_line]+h);
	  }
	  
	}
	
	//��¼�ϴν���ʱ�����������ղ�ֵ
	if(zhongzhi[real_line]>10)        
	  L_Last_WhiteErr=zhongzhi[real_line];
	chuHuan=1;//������������������������־��1
	led(LED0,LED_ON);//������ʱLED0����Ϊ���г���ı�־
	Lthan=1;//�����󣬱�֤�����������������
  }
  
  
  
  /******************�������ı�־******************************/
  for(m=20;m<40;m++)//�����25--40�ж��ǽӽ�ֱ��������
  {
	if(zy_flag[m]==1&&(zy_flag[m+1]==1||zy_flag[m-1]==1))
	{
	  CHuan++;
	}
  }
  
  
  
  /*****************������60 Ltime���Ҽ�⵽������־�������󻷵������б�־��0*****************************/
  if(Ltime>60&&CHuan>10&&Lflage==1)
  {
	led(LED0,LED_OFF);
	led(LED3,LED_OFF);
//	led(LED2,LED_OFF);
	Lflage=0;
	Ltime=0;
	Lthan=0;
	chuHuan=0;
	
  }
  
  
}



/*******************************************************************************
�һ���

ԭ��ͬ��

********************************************************************************/
void R_HuanDao(int16_t *zhongzhi)
{  
  
  uint8 h=23;
  uint8 jing_tiao=0;
  
  uint8 R_flage=0;
  
  uint8 Pian_R1=0,Pian_R2=0;
  
  uint8 CHuan=0;
  uint8 C_h=0;
  uint8 Black_count=0,White_Count=0;
  
  uint8 i=0,m=0,k=0;
  uint8 Gui=0,Tiao=0;
  uint8 GPoint=0,TPoint=0;
  
  uint8 B_Guai=0,B_Tiao=0;
  uint8 B_GPoint=0,B_TPoint=0;
  
  if(Rflage==0)
  {
	
	for(i=real_line;i>20;i--)
	{
	  if(zy_flag[i]==3&&zy_flag[i-1]==2&&zy_flag[i-2]==2)
	  {
		Pian_R1=1;
		k=i;
		break;
	  }
	}
	
	for(i=k;i<55;i++)
	{
	  if(zy_flag[i]==2&&zy_flag[i+1]==3&&zy_flag[i+2]==3)
	  {
		Pian_R2=1;
		break;
	  }
	}
	
  }
  
  
  for(i=real_line-4;i<45;i++)
  {
	if(zy_flag[i+1]==2&&zy_flag[i]==2)
	{
	  R_flage=1;
	  k=i;
	  break;
	}
  }
 
  if((Pian_R1==1&&Pian_R2==1)||R_flage==1||Rflage==1)
  {
	//�ɰױ�ڵ�������
	for(i=k;i>15;i--)
	{
	  if(zy_flag[i]==2&&zy_flag[i-1]==1&&zy_flag[i-2]==1)
	  {
		Tiao=1;
		TPoint=i-1;
		break;
	  }  
	  
	}
	
	//�ɺڱ�׵�������
	for(i=k;i<56;i++)
	{
	  if(zy_flag[i]==2&&zy_flag[i+1]==1&&zy_flag[i+2]==1)
	  {
		
		Gui=1;
		GPoint=i+1;
		break;
	  }
	  
	}
	
  }
  
  
  
  if(Gui==1&&Tiao==1&&Rthan==0)
  {
	led(LED3,LED_ON);
	zhongzhi[real_line]=40-(l_edge[real_line]+30);
	
	
	Rtime=0;
	Rflage=1;//��ʼ��ʱ�ı�־�����뻷���ı�־
	
  }
  else if(Rflage==1&&(zy_flag[real_line]==1||Rthan==1))
  {
	
	

	  for(m=real_line;m<55;m++)
	  {
		if((zy_flag[m-1]==0||zy_flag[m-2]==0||zy_flag[m-3]==0||zy_flag[m-4]==0||zy_flag[m-5]==0)&&zy_flag[m]==2&&zy_flag[m+2]==2)
		{
		  C_h=1;
		  
		  break;
		}
	  }

	
	if(C_h==0)
	{
	  for(m=real_line;m>10;m--)
	  {
		if(zy_flag[m]==0&&zy_flag[m+1]==0&&zy_flag[m-1]!=1)
		{
		  C_h=1;
		  break;
		}
	  }
	}
	  
	if(C_h==0)
	{
	  for(m=9;m<real_line-1;m++)
	  {
		if(zy_flag[m]==4)
		  Black_count++;
		if(Black_count>2)
		{
		  Black_count=3;
		  break;
		}
	  }
	  
	  for(m=real_line;m<55;m++)
	  {
		if(zy_flag[m]==0)
		  White_Count++;
		if(White_Count>4)
		{
		  White_Count=5;
		  break;
		}
			
	  }
	  
	  if(Black_count>2&&White_Count>4)
	  {
		C_h=1;
		led(LED2,LED_ON);
	  }
		
	  
	}
	
	if(C_h==1) 
	{
	  

	  
	  	zhongzhi[real_line]-=15;
	}
	else
	{
	  
	  for(m=real_line;m>10;m--)
	  {
		if(video[m][77]==1&&video[m-1][77]==0&&video[m-2][77]==0)
		{
		  i=real_line-m;
		  jing_tiao=1;
		  break;
		}
	  }
	 
	  if(Rtime>50)
	  {
		jing_tiao=0;
	  }
	  
	  
	  if(jing_tiao==1)
	  {
		if(i>15)
		  h=22;
		else if(i>10&&i<=15)
		  h=21;
		else if(i>5&&i<=10)
		  h=20;
		else if(i>0&&i<=5)
		  h=19;
		zhongzhi[real_line]=40-(r_edge[real_line]-h);
	  }
	  else
	  {

		if(Rtime<30)
		  h=22;
		else
		  h=30;
		zhongzhi[real_line]=40-(r_edge[real_line]-h);
	  }
	}
	if(zhongzhi[real_line]<-10)
	  R_Last_WhiteErr=zhongzhi[real_line];
	chuHuan=1;                              
	led(LED0,LED_ON);
	Rthan=1;
  }
  
  
  
  for(m=20;m<40;m++)
  {
	if(zy_flag[m]==1&&(zy_flag[m+1]==1||zy_flag[m-1]==1))
	{
	  CHuan++;
	}
  }
  
  if(Rtime>60&&CHuan>10&&Rflage==1)
  {
	led(LED0,LED_OFF);
	led(LED3,LED_OFF);
	//	led(LED2,LED_OFF);
	Rflage=0;
	Rtime=0;
	Rthan=0;
	chuHuan=0;
	
  }
  
  
}
