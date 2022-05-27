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
 u8 C,B=0,B1=0,kl[8]; //�жϴ���
 u8 have,ljks,slj3,slj30;
       
u8 sec=0,ms200=0,sec3,se30;
//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!

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

void WBZD_Init(void) // �ж����ų�ʼ��
{
	GPIO_InitTypeDef  GPIO_TimeBaseStructure;
	EXTI_InitTypeDef EXTI_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_TimeBaseStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʹ������
	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ������
    GPIO_TimeBaseStructure.GPIO_Pin = GPIO_Pin_1;   //LED  GPIO
 	GPIO_TimeBaseStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING ;//������������
	GPIO_TimeBaseStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA,&GPIO_TimeBaseStructure);
	
//	GPIO_EXTILineConfig (GPIO_PortSourceGPIOB,GPIO_PinSource15);//GPIO��������
	
	   GPIO_EXTILineConfig (GPIO_PortSourceGPIOA,GPIO_PinSource1);//GPIO��������
//	AFIO->EXTICR[1]&=0XFFFF0FFF;  //��������AP7��������
	
	EXTI_TimeBaseStructure.EXTI_Line = EXTI_Line1;   //�ж���
	EXTI_TimeBaseStructure.EXTI_Mode = EXTI_Mode_Interrupt;   //�ж�|�¼�
	EXTI_TimeBaseStructure.EXTI_Trigger = EXTI_Trigger_Falling; // ����|�½�
	EXTI_TimeBaseStructure.EXTI_LineCmd = ENABLE; //ʹ��
	EXTI_Init (&EXTI_TimeBaseStructure);
	
 	NVIC_TimeBaseStructure.NVIC_IRQChannel=EXTI1_IRQn;  //�ж���0
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//ʹ������
	
    GPIO_TimeBaseStructure.GPIO_Pin = GPIO_Pin_6;   //LED  GPIO
 	GPIO_TimeBaseStructure.GPIO_Mode  = GPIO_Mode_AF_PP ;//������������
	GPIO_TimeBaseStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA,&GPIO_TimeBaseStructure);

	TIM_DeInit (TIM3);
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =1; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState=TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_Pulse=psc;//ռ�ձ�
	TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig (TIM3,ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//****************************************************************************
void TIM4_init(u16 arr,u16 psc)
{

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx	
	
}

/***************************************************************************************
              �ⲿ�ж�ִ��
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
EXTI_ClearITPendingBit(EXTI_Line1); //�����־λ

}

/***************************************************************************************
              TIM4 �ж�ִ��
*************************************************************************************/
void TIM4_IRQHandler(void)
	
{
	static u8 secn;
	static u16 dianliu;
	static u8 js;

    if(TIM_GetITStatus(TIM4,TIM_IT_Update)!= RESET)
	{    //�ж��Ƿ���TIM4��update�ж�
//					 Made_Data();		//ADת��	
		//	if(dianliu>0x0FFF)dianliu=0x0FFF;
//			dianliu=(3135*(js/200.0))+788;
//				GP8302_Read(dianliu); //819
//			if (slj30==149)  js+=50;  else;
//			if (js>200)      js=0;  else;
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//ʹ���ж�
        ms200=1;
		secn++;
		if (secn>=5) {sec=1;secn=0;}
		slj30++;
		if (slj30>=150) 		  //30��  150
			{se30=1;slj30=0;}
		if (have) 
			{
				if(slj3>=3)     //���ý���ʱ��
					{sec3=1;slj3=0;}
				else
					slj3++;
			}	
	}
}
//��ȴ���ʱ30ms����������λ�õİ���
//llks=1ʱ�ۼ��ۼ�����=0ֹͣ
void Made_Data(void)
{
	int32_t a,b,c,d;
	u8 pp=0;
if(S_7705>=5){
 
	b=0; TM=0;
			if(TM7705_WaitDRDY())	// �ȴ��ڲ�������� --- ʱ��ϳ���Լ180ms //
			{
				TM7705_WriteByte(0x38);
				b=TM7705_Read3Byte();
			//	YS_WD(b);
			}
			else pp=1;

	a=0; TM=1;
		  if(pp==0)
		   {
			TM7705_WaitDRDY();	// �ȴ��ڲ�������� --- ʱ��ϳ���Լ180ms //
            TM7705_WriteByte(0x38);
				a=TM7705_Read3Byte();
//			TM7705_CalibSelf(1);
		    QingJiao_F();//���
			   delay_ms(7);
         a+=Ma_xz;  
				YS_YS(a);
				YS_YM(a);
				YS_WD(b);
			}
		    else ;
}else S_7705++;  //7707��ʼ����ʱ
		
}
//
void LJLL_Data(void)
{
	  JNS++;
	  N_A(JNS,JNW,8);
	  AT24CXX_Write(0x0200+NIAN,ljl,8);  //8λ
	  AT24CXX_Write(0x0200+NIAN+0x0008,JNW,8); //д���8λ  ����	
	  if(JNS>=1000000)
		  {
				N_A(NIAN/16+1,kl,4);  //
				NIAN+=16;
		        AT24CXX_Write(0x01f0,kl,4);         //�ǵڼ�λ
				AT24CXX_Write(0x0200+NIAN,ljl,8);  //д�ۼ���
			    JNS=0;
		        N_A(JNS,JNW,8);
				AT24CXX_Write(0x0200+NIAN+0X0008,JNW,8); //��ʱ��0
	        }
}     //200ms   150���ǰ���� 
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

// ����   

//******************************************  
void N_A(u32 n,u8 *a,u8 w){

  int i;	
  for(i=(w-1);i>0;i--)
	 {	
		 a[i]=(n%10)+48;
		 n/=10;
	 }	
 }








