#include "timer.h"
#include "delay.h"
#include "24c64.h"
#include "led.h"
#include "gui.h"
#include "usart.h"
#include "TM7707.h"
#include "protocol.h"
#include "mport.h"
#include "GP8302.h"

	extern u8 lll[];
	extern u8 ljl[];
	extern u8 lsl[];
	extern u8 cyl[];
	
extern u32 JNS, Ma_xz;
extern u16 NIAN;
extern u8 JNW[], TM, S_7705;
extern int32_t LJ;

static uint8_t buf[3];
//static int32_t a,b;
 u8 C,B=0,B1=0,kl[8]; //中断次数
 u8 have,ljks,slj3,slj30;
       
u8 sec=0,ms200=0,sec3,se30;
//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!

void PVD_init(void)
{
	EXTI_InitTypeDef EXTI_InitStructure; 
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = PVD_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	EXTI_DeInit();
  EXTI_StructInit(&EXTI_InitStructure);  
  EXTI_InitStructure.EXTI_Line = EXTI_Line16; 
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE; //
  EXTI_Init(&EXTI_InitStructure); 
	//		EXTI_InitStructure.EXTI_Trigger= EXTI_Trigger_Rising; 
		
}

void WBZD_Init(void) // 中断引脚初始化
{
	GPIO_InitTypeDef  GPIO_TimeBaseStructure;
	EXTI_InitTypeDef EXTI_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能引脚
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能引脚
    GPIO_TimeBaseStructure.GPIO_Pin = GPIO_Pin_1;   //LED  GPIO
 	GPIO_TimeBaseStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;//设置上拉输入
	GPIO_TimeBaseStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA,&GPIO_TimeBaseStructure);
	
//	GPIO_EXTILineConfig (GPIO_PortSourceGPIOB,GPIO_PinSource15);//GPIO与线链接
	
	   GPIO_EXTILineConfig (GPIO_PortSourceGPIOA,GPIO_PinSource1);//GPIO与线链接
//	AFIO->EXTICR[1]&=0XFFFF0FFF;  //定义引用AP7与线连接
	
	EXTI_TimeBaseStructure.EXTI_Line = EXTI_Line1;   //中断线
	EXTI_TimeBaseStructure.EXTI_Mode = EXTI_Mode_Interrupt;   //中断|事件
	EXTI_TimeBaseStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 上升|下降
	EXTI_TimeBaseStructure.EXTI_LineCmd = ENABLE; //使能
	EXTI_Init (&EXTI_TimeBaseStructure);
	
 	NVIC_TimeBaseStructure.NVIC_IRQChannel=EXTI1_IRQn;  //中断线0
	NVIC_TimeBaseStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_TimeBaseStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_TimeBaseStructure.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_TimeBaseStructure);
	
}



//****************************************************************************
void TIM3_PWM_init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef  			GPIO_TimeBaseStructure;
    TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef    		TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能引脚
	
    GPIO_TimeBaseStructure.GPIO_Pin = GPIO_Pin_6;   //LED  GPIO
 	GPIO_TimeBaseStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;//设置上拉输入
	GPIO_TimeBaseStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA,&GPIO_TimeBaseStructure);

	TIM_DeInit (TIM3);
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =1; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_Pulse=psc;//占空比
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig (TIM3,ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//****************************************************************************
void TIM4_init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
	TIM_Cmd(TIM4, ENABLE);  //使能TIMx	
	
}

/***************************************************************************************
              外部中断执行
*************************************************************************************/
void EXTI1_IRQHandler(void){

	
//	EXTI->PR =EXTI->PR;
EXTI->IMR&=0XFFFFFF7F;
  while(1){
	GPIO_ResetBits	(GPIOC,GPIO_Pin_13); //LED
	delay_ms(3000);
	GPIO_SetBits	(GPIOC,GPIO_Pin_13); //LED
     delay_ms(3000);
  }
EXTI->IMR|=0X80;
EXTI_ClearITPendingBit(EXTI_Line1); //清除标志位

}

/***************************************************************************************
              TIM4 中断执行
*************************************************************************************/
void TIM4_IRQHandler(void)
	
{
	static u8 secn;
	static u16 dianliu;
	static u8 js;

    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!= RESET)
	{    //判断是否是TIM4的update中断
//					 Made_Data();		//AD转换	
		//	if(dianliu>0x0FFF)dianliu=0x0FFF;
//			dianliu=(3135*(js/200.0))+788;
//				GP8302_Read(dianliu); //819
//			if (slj30==149)  js+=50;  else;
//			if (js>200)      js=0;  else;
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//使能中断
        ms200=1;
		secn++;
		if (secn>=5) {sec=1;secn=0;}
		slj30++;
		if (slj30>=150) 		  //30秒  150
			{se30=1;slj30=0;}
		if (have) 
			{
				if(slj3>=3)     //设置进入时间
					{sec3=1;slj3=0;}
				else
					slj3++;
			}	
	}
}
//最长等待延时30ms，看看计算位置的安放
//llks=1时累加累计流量=0停止
void Made_Data(void)
{
	int32_t a,b,c,d;
	u8 pp=0;
if(S_7705>=5){
 
	b=0; TM=0;
			if(TM7705_WaitDRDY())	// 等待内部操作完成 --- 时间较长，约180ms //
			{
				TM7705_WriteByte(0x38);
				b=TM7705_Read3Byte();
			//	YS_WD(b);
			}
			else pp=1;

	a=0; TM=1;
		  if(pp==0)
		   {
			TM7705_WaitDRDY();	// 等待内部操作完成 --- 时间较长，约180ms //
            TM7705_WriteByte(0x38);
				a=TM7705_Read3Byte();
//			TM7705_CalibSelf(1);
		    QingJiao_F();//倾角
			   delay_ms(7);
         a+=Ma_xz;  
				YS_YS(a);
				YS_YM(a);
				YS_WD(b);
			}
		    else ;
}else S_7705++;  //7707初始化延时
		
}
//
void LJLL_Data(void)
{
	  JNS++;
	  N_A(JNS,JNW,8);
	  AT24CXX_Write(0x0200+NIAN,ljl,8);  //8位
	  AT24CXX_Write(0x0200+NIAN+0x0008,JNW,8); //写入后8位  记数	
	  if(JNS>=1000000)
		  {
				N_A(NIAN/16+1,kl,4);  //
				NIAN+=16;
		        AT24CXX_Write(0x01f0,kl,4);         //记第几位
				AT24CXX_Write(0x0200+NIAN,ljl,8);  //写累计量
			    JNS=0;
		        N_A(JNS,JNW,8);
				AT24CXX_Write(0x0200+NIAN+0X0008,JNW,8); //计时清0
	        }
}     //200ms   150次是半分钟 
//*************************************************

void PVD_IRQHandler(void)
{
if(EXTI_GetITStatus(EXTI_Line16) != RESET)
	{
	while(1){	
		      GPIO_SetBits(GPIOA,GPIO_Pin_1);
			  AT24CXX_Write(0x01e0,ljl,8);
	GPIO_ResetBits	(GPIOC,GPIO_Pin_13); //LED
	delay_ms(3000);
	GPIO_SetBits	(GPIOC,GPIO_Pin_13); //LED
    delay_ms(3000);
		
	         }  
	EXTI_ClearITPendingBit(EXTI_Line16);
	}
}
//****************************************** 

// 数据   

//******************************************  
void N_A(u32 n,u8 *a,u8 w){

  int i;	
  for(i=(w-1);i>0;i--)
	 {	
		 a[i]=(n%10)+48;
		 n/=10;
	 }	
 }








