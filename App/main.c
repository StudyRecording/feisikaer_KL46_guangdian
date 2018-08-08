
/* ***********************************************************************
				潍坊科技学院——广电四轮摄像头组（幻影队）

//摄像头SDA A16  SCL A17   PCLK A7   HREF A5   VSYNC A6  E16-E23
//OLED  SCL E2	 SDA E1    RST E4    DC E6
//拨码开关B18  B17  B16  B11  B10  B9  B8  B2   高--低   （拨码开关仅B10  B9  B8  B2）
//按键	  B3    B7    B11    B17     K1--K4
//串口    UART0  9600   RXD A15   TXD A14  
//电机    TPM0_CH1_PC2    TPM0_CH2_PC3
//备用IO  C6  C7  C8  C9
//舵机	  TPM1_CH1_PA13
//编码器  TPM_CLKIN0_PIN  PTB16            
//蜂鸣器  A20(B0B1D6D7)

                                                2018-7-31
												胡鹏成

************************************************************************ */


#include "common.h"
#include "include.h"
#include "math.h"

int real_line=38;//用改行的差值作为舵机打角程度的判断

float  std_speed=0;  //期望速度
float  sudu=0;      //赋值给期望速度的变量
int line=38;		//按键调节检测行
float sudu2=0;		//按键调节速度（按键调节sudu2----->sudu2赋值给sudu------>sudu赋值给std_speed）

int16 last_ErrAll=0;//记录上一次的打角差值

uint8 mid=0;		//记录行的中值
uint8 imgbuff[CAMERA_SIZE];                             //定义存储接收图像的数组
uint8 video[OV7725_EAGLE_H][OV7725_EAGLE_W];//定义存储图像的二维数组
//循环所需变量
uint8 i=0;
uint8 j=0;

int16  M_err[60]={0};    //中线的数组差值
int16  Err_All = 0;      
//拟合后的差值
uint32 pulse = 0;     //记录编码器的脉冲数
int8   flag_stop=0;//停车标志位
int Ltime=0;//左环向左偏左拐开始计时
int Rtime=0;//右环向右偏右拐开始计时
uint8 Start_Line=0;//检测起跑线
uint8 Start_Time=0;//检测到起跑线后的时间
uint8 Can_Stop=0;//可以检测停止线


//计算每秒的帧数（侧面反映程序运行速度）
int frame=0,painting_count=0,time_count=0;

//检测停车标志的次数
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

//函数声明
void img_extract(uint8 *dst, uint8 *src, uint32 srclen);
void PORTA_IRQHandler();
void DMA0_IRQHandler();
void PIT_IRQHandler();

/* *******************************************************************************
*	主函数
*	2018-7-31
*	胡鹏成
*	主控板：KL46
*	注意：第十三届恩智浦智能车广电四轮应用程序
**********************************************************************************/

void main(void)
{
  DisableInterrupts;//关闭总中断
  
  //舵机初始化
  tpm_pwm_init(TPM1, TPM_CH1,200,3350); //3330  右转 增大   左转 减小   //初始化 舵机 PWM 
  
  //电机初始化
  tpm_pwm_init(TPM0, TPM_CH1,10*1000,0); //初始化 电机 PWM     
  tpm_pwm_init(TPM0, TPM_CH0,10*1000,0);
  
  tpm_pulse_init(TPM2,TPM_CLKIN0,TPM_PS_1); //初始化 TPM2 为脉冲累加，输入管脚为 TPM_CLKIN0_PIN ，分频系数为 1
  
  //OLED初始化
  LCD_Init();
  
  //LED初始化
  led_init(LED3);
  led_init(LED0);
  led_init(LED2);
  //按键及拨码开关初始化
  KEY_Init();
  //初始化摄像头
  camera_init(imgbuff);
  
  //配置中断服务函数
  set_vector_handler(PORTA_VECTORn ,PORTA_IRQHandler);    //设置LPTMR的中断服务函数为 PORTA_IRQHandler
  set_vector_handler(DMA0_VECTORn ,DMA0_IRQHandler);      //设置LPTMR的中断服务函数为 PORTA_IRQHandler
  //计时器中断
  pit_init_ms(PIT0, 9);  //初始化PIT0，定时时间为： 9ms
  pit_init_ms(PIT1, 3);  //初始化PIT1，定时时间为： 2ms
  set_vector_handler(PIT_VECTORn ,PIT_IRQHandler); //设置PIT的中断服务函数为 PIT_IRQHandler  
  
  //舵机pid
  sudu2=160;
  kp_tiao=23;
  kd_tiao=80;
  kp_tiao2=0;
  yuzhi=60;
  
  //初始化PID
  PID_init();
  //按键
  LCD_CLS(0);//清屏
  Can_Change();
  
  systick_delay_ms(500);//延时
  
  LCD_CLS(0);//清屏
  enable_irq(PIT_IRQn);//使能定时器
  EnableInterrupts;  //开启总中断
  
  while(1)
  {
    
    //获取图像
    camera_get_img();//摄像头获取图像
    img_extract((uint8 *)video, (uint8 *)imgbuff,CAMERA_SIZE);//解压图像
    painting_count++;//显示图像数
	
    //出赛道，58、57行全黑，停车
    if(getpoint_h(0,79,58)>68&&getpoint_h(0,79,57)>68)
    {
      flag_stop=1;
    }
    
    //停车道停车
    if(Start_Line==0||Can_Stop==1)//未经过起跑线，或可以检测停车线时运行该部分程序
    {
      for(i=45;i<55;i++)//检测45--55行
      {
        TiaoPoint stop_TiaoPoint=Jump_along(0,i,3,77);//得到跳变点的类型和次数
        if(stop_TiaoPoint.White_to_black_Number>5&&stop_TiaoPoint.Black_to_white_Number>5)//如果改行从白到黑和从黑到白的跳变点都大于5次
        {
          Stop_flage++;//停车标志次数加1
          if(Stop_flage>4)//如果停车标志次数大于4次
          {
            if(Start_Line==1)//如果已经经过了起跑线
              flag_stop=1;//该次是检测到了停车线，停车
            else
              Start_Line=1;//该次检测到是起跑线
            break;
          }
        }
      }
    }
    
	//计算0--59行的差值           
    for(i=0;i<60;i++)//1.970ms  1.945
    {
      M_err[i] = middle_line(video[i],i);//计算60行赛道中心线
    }
    
    //滤波，滤去大部分不合理的差值
    lvbo();
	
    //十字程序
    crossroad(M_err);
    
	//左环程序
    L_HuanDao(M_err);
	//检测到左圆环之前错误的检测到了右圆环或之前右圆环标志位未清除，清除右圆环相关标志位，保留左圆环相关标志位
    if(Lflage==1&&Rflage==1)
    {
      Rflage=0;
      Rtime=0;
      Rthan=0;
      chuHuan=0;
	  
    }
    
	//右环程序
    R_HuanDao(M_err);
	//检测到右圆环之前错误的检测到了左圆环或之前左圆环标志位未清除，清除左圆环相关标志位，保留右圆环相关标志位
    if(Lflage==1&&Rflage==1)
    {
      Lflage=0;
      Ltime=0;
      Lthan=0;
      chuHuan=0;
	  
    }
    

	//如果检测行为全黑，得不到正确的差值
    if(zy_flag[real_line-1]==4)
    {
	  led(LED2,LED_OFF);
	  if(Lflage==1)//如果已经检测到了左圆环
	  {
		Err_All=L_Last_WhiteErr+5;//该行差值变为上一次进左圆环的左拐差值
	  }
	  else if(Rflage==1)//如果已经检测到了右圆环
	  {
		Err_All=R_Last_WhiteErr-5;//该行差值变为上一次进右圆环的左拐差值
	  }
	  else//如果其他情况导致得到错误中值
      	Err_All=last_ErrAll;//该次差值变为上次差值
    }
    else//如果检测行正常
    {
	  
      real_line=line;//检测行数为按键调节的行数
      Err_All=M_err[real_line];//得到检测行的差值
      if(ABS(Err_All)>15)//如果检测行的差值较大（>15），则记录此次差值
        last_ErrAll=Err_All;
	  
    }
    
    
    PID_DuoJi(Err_All);//舵机输出
    
    
    //电机调速
    if(flag_stop==1)//停车
    {
      std_speed=0;
    }
    else
    {
		//期望速度为按键调节的速度
		sudu=sudu2;
      std_speed=sudu;
    }

    
	//计算出的中线变黑显示
    for(i=0;i<60;i++)
    {
      mid=40-M_err[i];
      video[i][mid]=1;
      video[i][mid+1]=1;
      video[i][mid-1]=1;
    }
	
    for(i=0;i<79;i++)
    {
	  //显示检测行
      video[real_line][i]=1;
      
    }
    
    //OLED显示
    ShowVideoBin1(video);//4.410ms	显示赛道图像
    //OLED_CCD_bin(47,7,video[30]);//450us
    LCD_three(0,1,Err_All); //显示偏差
//	LCD_three(0,2,l_edge[20]); 
//    LCD_three(0,3,r_edge[20]);
    LCD_three(0,4,l_edge[real_line]);
    LCD_three(0,5,r_edge[real_line]);
//	LCD_three(0,6,L_Radius);
	LCD_three(0,7,frame);//显示每秒多少帧
    //LCD_three(0,7,record);
  }
}

/*!
*  @brief      二值化图像解压（空间 换 时间 解压）
*  @param      dst             图像解压目的地址
*  @param      src             图像解压源地址
*  @param      srclen          二值化图像的占用空间大小
*  @since      v5.0            img_extract(img, imgbuff,CAMERA_SIZE);
*  Sample usage:
*/
void img_extract(uint8 *dst, uint8 *src, uint32 srclen)
{
  uint8 colour[2] = {0, 1}; //0 和 1 分别对应的颜色
  //注：摄像头 0 表示 白色，1表示 黑色
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
功能：PIT中断
参数：无
返回：无
详解：读取LPTMR脉冲数
******************************************************************/
void PIT_IRQHandler(void)
{   
  
  if(PIT_TFLG(PIT0)==1) 
  { 
    PIT_Flag_Clear(PIT0); //清中断标志
    pulse=tpm_pulse_get(TPM2);//得到编码器的脉冲数
    tpm_pulse_clean(TPM2);
	//如果检测到左环，开始计算进环时间
    if(Lflage==1)
    {
      Ltime++;
    }
	//如果检测到右环，开始计算进环时间
    if(Rflage==1)
    {
      Rtime++;
    }
	//如果检测到起跑线且并未允许检测停车线时，开始计算时间
    if(Can_Stop==0&&Start_Line==1)
    {
      Start_Time++;
    }
   
	
    if(Ltime>500)
      Ltime=500;
    
    if(Rtime>500)
      Rtime=500;
    
	//当检测到起跑线后30 Start_Time时，允许检测停车线
    if(Start_Time>30)
    {
      Can_Stop=1;
    }
    
    
  }
  
  else if(PIT_TFLG(PIT1)==1) 
  { 
    
    PIT_Flag_Clear(PIT1); //清中断标志

    time_count++;//时间增加
	if(time_count==333)//当时间增加到1s时
	{
	  frame=painting_count;//得到帧数
	  //对计算帧的标志位清零
	  painting_count=0;
	  time_count=0;
	}
    
    if(flag_stop==1)//停车时
    {
      PID_Motor(0);//电机快速停止
    }
    else
      PID_Motor(1);   //其它情况下，电机正常行驶
  }
}


/*!
*  @brief      PORTA中断服务函数
*  @since      v5.0
*/
void PORTA_IRQHandler()
{
  uint8 n = 0;    //引脚号
  uint32 flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;                                   //清中断标志位
  
  n = 6;                                              //场中断
  if(flag & (1 << n))                                 //PTA6触发中断
  {
    camera_vsync();
  }
}

/*!
*  @brief      DMA0中断服务函数
*  @since      v5.0
*/
void DMA0_IRQHandler()
{
  camera_dma();
}