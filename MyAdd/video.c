#include "video.h"
#include "math.h"





uint8 shizi=0;//十字标志位
int16  zy_flag[60]={0};//60行的左右黑线检测情况
int16 area0[60]={0};         //计算高黑点数数组
int16 area2[80]={0};        //计算宽黑点数数组
int16  shizi_flag[60]={0};//全白标志位
uint8 Lflage=0;//左圆环出环口标志位
uint8 Rflage=0;//右圆环出环口标志位
uint8 chuHuan=0;//允许出环标志位

uint8 Lthan=0;//左环进环后还未出环的标志位
uint8 Rthan=0;//右环进环后还未出环的标志位
int16 L_Last_WhiteErr=0;//左进环时及在左环时上次左拐差值
int16 R_Last_WhiteErr=0;//右进环时及在右环时上次右拐差值

//60行的左边缘值
volatile   signed short int l_edge[60]={0};
//60行的右边缘值
volatile   signed short int r_edge[60]={80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};
//上次60行左边缘值
volatile  static signed short int pre_l_edge[60] ={0};
//上次60行右边缘值
volatile  static signed short int pre_r_edge[60]={80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80,80};

extern float sudu;
extern int Ltime;
extern int Rtime;
extern int real_line;
extern int16  M_err[60];
extern uint8 video[OV7725_EAGLE_H][OV7725_EAGLE_W];
/******************************************************************
功能：计算赛道黑线的中心线
参数：无
返回：中线差值
详解：计算出黑线的中心线，并返回与赛道中线的差值,差值为正值往左拐，负值往右拐
******************************************************************/
signed short int middle_line( unsigned char *bin_point,unsigned char CJ_everyline)
{
  
  
  volatile unsigned char i ;//for循环计数
  volatile unsigned char BL_flag = 0X00; //记录此次黑线查询状态	
  static signed short int mid_now = 40;		//缓冲中点值									
  volatile signed short int cha_zhi = 0;//差值
  //静态变量：
  //查询黑线位置的 开始查找点	
  volatile static signed short int find_p[60]  = {40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40,40};											
  
  //上次差值
  volatile static signed short int pre_cha[60] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  
  // 记录丟线前次的左右黑线查询状态
  volatile static unsigned char  LoseL_BL_flag[60] = {0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00};  
  
  volatile static unsigned char  out_loop[60] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//
  
  //定义初次左右黑线的位置
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
  
  //防止 查找点 出界
  if(find_p[CJ_everyline]<3)
	find_p[CJ_everyline]=3;	
  if(find_p[CJ_everyline]>76)
	find_p[CJ_everyline]=76;
  
  //非正常情况，图像中间为黑线
  if( bin_point[find_p[CJ_everyline]]==1 )
  {     
	//右拐时的左边线
	for(i = find_p[CJ_everyline]; i<77; i++)
	{
	  if((bin_point[i]==1)&&(bin_point[i+1]==0)&&(bin_point[i+2]==0)&&(bin_point[i+3]==0))//
	  {
		l_edge[CJ_everyline] = i+1;  
		BL_flag = BL_flag|0X10;//此处标注成找到左边界	   	
		break;
	  }
	}
	
	//左拐时的右边线
	for(i =find_p[CJ_everyline]; i>2; i--)
	{
	  if((bin_point[i]==1)&&(bin_point[i-1]==0)&&(bin_point[i-2]==0)&&(bin_point[i-3]==0))//
	  {
		r_edge[CJ_everyline] = i-1;
		BL_flag = BL_flag|0X01;//此处标注成找到右边界
		break;
	  }
	}
	
  } 
  else//正常情况，图像中间为白色
	
	//    if( bin_point[find_p[CJ_everyline]]==0 )      
	
  {   //判断左边黑线位置	  
	for(i = (unsigned char)find_p[CJ_everyline]; i>2; i--)
	{				
	  if( (bin_point[i]==0) && (bin_point[i-1]==1) &&(bin_point[i-2]==1)&&(bin_point[i-3]==1))//
	  {
		l_edge[CJ_everyline] = i-1;
		BL_flag = BL_flag|0X10;//此处标注成找到左边界
		break;
	  }						
	}
	//判断右边黑线位置
	for(i = (unsigned char)find_p[CJ_everyline]; i<77; i++)
	{
	  if( (bin_point[i]==0) && (bin_point[i+1]==1) && (bin_point[i+2]==1)&& (bin_point[i+3]==1))//
	  {
		r_edge[CJ_everyline] = i+1;
		BL_flag = BL_flag|0X01;//此处标注成找到右边界
		break;
	  }
	}
  }	
  
  //判断边沿值
  switch(BL_flag)
  {
  case 0x00: //两边都没黑线
	{	
	  
	  
	  if((bin_point[3]==1)&&(bin_point[76]==1))//全黑，两边都丢线
	  {	
		zy_flag[CJ_everyline]=4;
		shizi_flag[CJ_everyline]=0;
		
		return 0;
	  }
	  else if((bin_point[3]==0)&&(bin_point[46]==0)&&(bin_point[76]==0))//全白
	  {   
		zy_flag[CJ_everyline]=0;
		
		shizi_flag[CJ_everyline]=1;
		return 0;
	  }
	  
	}
  case 0x01://只有右边黑线
	{	
	  
	  
	  l_edge[CJ_everyline] = pre_l_edge[CJ_everyline] +(r_edge[CJ_everyline] - pre_r_edge[CJ_everyline]);//根据右边线特点求出左边线            
	  
	  
	  zy_flag[CJ_everyline]=3;
	  shizi_flag[CJ_everyline]=0;
	  break;
	}
  case 0x10: //只有左边黑线
	{  
	  r_edge[CJ_everyline] = pre_r_edge[CJ_everyline] + (l_edge[CJ_everyline] - pre_l_edge[CJ_everyline]);//根据左边线特点求出右边线
	  shizi_flag[CJ_everyline]=0;
	  zy_flag[CJ_everyline]=2;
	  break;
	}
  case 0x11: //两边黑线都有
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
  
  mid_now = (l_edge[CJ_everyline] + r_edge[CJ_everyline])/2; //求出中间值	
  
  
  cha_zhi = 40 - mid_now; //求出差值,给舵机             
  
  if( (BL_flag==0x01)||(BL_flag==0x10) )//更新丟线前的黑线状态	
  {
	LoseL_BL_flag[CJ_everyline] = BL_flag;
  }	
  
  find_p[CJ_everyline] = mid_now;//下次查找点
  pre_cha[CJ_everyline] = cha_zhi;//更新上次差值	
  pre_r_edge[CJ_everyline] = r_edge[CJ_everyline];//更新左边界限
  pre_l_edge[CJ_everyline] = l_edge[CJ_everyline];//更新右边界限
  return cha_zhi ;//返回当前的差值
  
}

/*******************************************************************************
函数名称：lvbo
函数功能: 滤波
参数：
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
功能：得到高应行黑点数
参数：y1 y起点，y2 y终点 ，x
返回：黑点数
详解：无
**************/
int16 getpoint_h(uint8_t x1,uint8_t x2, uint8_t y)
{
  uint8_t i=0;
  area0[y]=0;
  for(i=x1;i<=x2;i++)
  {
	area0[y]=area0[y]+video[y][i];//累加黑点数（相当于做加法运算）
  }
  return area0[y];
  
}

/*************
功能：得到宽对应黑点数
参数：x1 x起点，x2 x终点 ，y 
返回：黑点数
详解：无
**************/
int16 getpoint(uint8_t y1,uint8_t y2,uint8_t x)

{
  uint8_t i=0;
  area2[x]=0;
  for(i=y1;i<=y2;i++)
  {
	area2[x]=area2[x]+video[i][x];//累加黑点数（相当于做加法运算）
  }
  
  return area2[x];
  
}

/**************************************************************************************
跳变点检测

参数：
Type:                 0 行跳变点，1 列跳变点
Single：              Type=0时，为行数；Type=1时，为列数
Double1，Double2      Type=0时，为列数（Double1<Double2）；Type=1时，为行数（Double1<Double2）
返回：
TiaoPoint类型
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
		My_tiaoPoint.Type=1;//由白变黑
	  }
	  if(video[Single][i-1]==1&&video[Single][i]==1&&video[Single][i+1]==0&&video[Single][i+2]==0)
	  {
		My_tiaoPoint.Black_to_white_Number++;
		My_tiaoPoint.Type=2;//由黑变白
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
		My_tiaoPoint.Type=1;//由白变黑
	  }
	  if(video[i-1][Single]==1&&video[i][Single]==1&&video[i+1][Single]==0&&video[i+2][Single]==0)
	  {
		My_tiaoPoint.Black_to_white_Number++;
		My_tiaoPoint.Type=2;//由黑变白
	  }
	}
  }
  return My_tiaoPoint;
  
}

/******************************************************************
功能：十字识别及补线
参数：无
返回：无
详解：无
******************************************************************/

void crossroad(int16_t *zhongzhi) 
{
  
  //跳变点位于拐点前
  volatile static uint8_t TiaoPnt=0;//跳变点 的所在行数
  volatile static  uint8_t GuaiPnt=0;//拐点   的所在行数
  uint8 TiaoPnt_Flag = 0;//找到  跳变点 的标志
  uint8 GuaiPnt_Flag= 0; //找到  拐点   的标志
  uint8 i=0,m=0;	//用于计数	
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
左环岛



********************************************************************************/
void L_HuanDao(int16_t *zhongzhi)
{
  
  
  uint8 h=23;//左边线与中线的差值
  uint8 jing_tiao=0;
  
  uint8 L_flage=0;
  
  uint8 Pian_L1=0,Pian_L2=0;
  
  uint8 CHuan=0;
  uint8 C_h=0;
  uint8 Black_count=0,White_Count=0;
  
  uint8 i=0,m=0,k=0;
  uint8 Gui=0,Tiao=0;//有黑变白   有白变黑
  uint8 GPoint=0,TPoint=0;
  
  uint8 B_Guai=0,B_Tiao=0;
  uint8 B_GPoint=0,B_TPoint=0;
  
  if(Lflage==0)//当左圆环标志为0（未检测到左圆环时）
  {
	/******对左圆环进行检测**********/
	
	//第一种情况：车偏左的时候，检测第38行前都只有右边线（即出环岛口只有右边线）
	//                               38行后都只有左边线（即出环岛口之前的只有左边线）
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
  
  //第二种情况：出环岛口处都是只有左边线
  for(i=real_line-4;i<45;i++)
  {
	if(zy_flag[i-1]==3&&zy_flag[i]==3)
	{
	  L_flage=1;
	  k=i;
	  break;
	}
  }
  
  
  /******************符合第一、二种情况，开始判断跳变点*************************/
  if((Pian_L1==1&&Pian_L2==1)||L_flage==1||Lflage==1)
  {
	//由白变黑的跳变沿
	for(i=k;i>15;i--)
	{
	  if(zy_flag[i]==3&&zy_flag[i-1]==1&&zy_flag[i-2]==1)
	  {
		Tiao=1;
		TPoint=i-1;
		break;
	  }  
	  
	}
	
	//由黑变白的跳变沿
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
  
  
  
  /**************当车在出环岛口附近时*************************/
  if(Gui==1&&Tiao==1&&Lthan==0)
  {
	led(LED3,LED_ON);
	zhongzhi[real_line]=40-(r_edge[real_line]-30);//走赛道中间
	
	Ltime=0;
	Lflage=1;//开始计时的标志，进入环岛的标志
	
	
  }
  else if(Lflage==1&&(zy_flag[real_line]==1||Lthan==1))/***车将要进环及进环和进环之后***/
  {
	
	
	//检测是否出环
	  for(m=real_line;m<55;m++)
	  {
		if((zy_flag[m-1]==0||zy_flag[m-2]==0||zy_flag[m-3]==0||zy_flag[m-4]==0||zy_flag[m-5]==0)&&zy_flag[m]==3&&zy_flag[m+1]==3)
		{
		  C_h=1;//出环标志
		  led(LED2,LED_ON);
		  break;
		}
		  
	  }
	

	if(C_h==0)//当出环标志未成立时，检测是否是进环的的一种特殊状态或者另一种出环特征
	{
	  for(m=real_line;m>10;m--)//检测是否出环
	  {
		if(zy_flag[m]==0&&zy_flag[m+1]==0&&zy_flag[m-1]!=1)
		{
		  C_h=1;
		  break;
		}
		
	  }
	}
	
	
	if(C_h==0)//依然未检测到出环时，检测是否是第三种出环特征
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
	
	if(C_h==1)//出环特征确认
	{
	  
	  //出环

	  
	  	zhongzhi[real_line]+=15;//由于是左环，左打角

	}
	else//进环状态
	{
	  
	  //检测到检测行和进环口之间的距离并且进一步确认进环
	  for(m=real_line;m>7;m--)
	  {
		if(video[m][3]==1&&video[m-1][3]==0&&video[m-2][3]==0)
		{
		  i=real_line-m;
		  jing_tiao=1;
		  break;
		}
	  }
	  //如果检测到进环状态时50 Ltime后，进环状态清除
	  if(Ltime>50)
	  {
		jing_tiao=0;
	  }
	  
	  //根据检测行和进环口的距离，确认打角程度
	  if(jing_tiao==1)
	  {
		if(i>15)//如果离得还很远，车沿稍微靠左边行驶
		  h=22;
		else if(i>10&&i<=15)
		  h=21;
		else if(i>5&&i<=10)
		  h=20;
		else if(i>0&&i<=5)//车距离进环非常近了，开始使劲靠左行驶
		  h=19;
		zhongzhi[real_line]=40-(l_edge[real_line]+h);//计算差值
	  }
	  else//进环口之后处于进环口和出环口之间的左拐环岛，车正常行驶
	  {

		if(Ltime<30)//车还未进环时的误检测，确保车的靠左行驶，方便检测
		  h=22;
		else
		  h=30;
		zhongzhi[real_line]=40-(l_edge[real_line]+h);
	  }
	  
	}
	
	//记录上次进环时及其进环后左拐差值
	if(zhongzhi[real_line]>10)        
	  L_Last_WhiteErr=zhongzhi[real_line];
	chuHuan=1;//进环后允许出环，允许出环标志置1
	led(LED0,LED_ON);//车进环时LED0亮作为运行程序的标志
	Lthan=1;//进环后，保证程序持续的正常运行
  }
  
  
  
  /******************检测出环的标志******************************/
  for(m=20;m<40;m++)//如果从25--40行都是接近直道的赛道
  {
	if(zy_flag[m]==1&&(zy_flag[m+1]==1||zy_flag[m-1]==1))
	{
	  CHuan++;
	}
  }
  
  
  
  /*****************当进环60 Ltime后且检测到出环标志，关于左环岛的所有标志清0*****************************/
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
右环岛

原理同左环

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
	//由白变黑的跳变沿
	for(i=k;i>15;i--)
	{
	  if(zy_flag[i]==2&&zy_flag[i-1]==1&&zy_flag[i-2]==1)
	  {
		Tiao=1;
		TPoint=i-1;
		break;
	  }  
	  
	}
	
	//由黑变白的跳变沿
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
	Rflage=1;//开始计时的标志，进入环岛的标志
	
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
