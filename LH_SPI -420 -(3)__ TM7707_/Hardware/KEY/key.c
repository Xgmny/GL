#include "key.h"
#include "sys.h" 
#include "delay.h"

extern u8 key;


void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTbʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0���ó����룬Ĭ������	  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOb.0

}

//u8 KEY_Scan(void)
//{	 
//	u8 aj;
//	if((KEYA==0||KEYR==0||KEYY==0))
//		{
//			 delay_ms(20);//ȥ���� 
//			if(KEYA==0)//+
//				{
//				
//				if(KEYR==0)  //+������
//					aj=4;
//		           else 
//					   aj=1;
//				}	
//			  else
//				{
//					if	(KEYR==0)
//						aj=2;
//					else
//					{
//					if  (KEYY==0)
//							aj=3;
//						else
//							aj=5;}
//				}
//		   }
//	else
//		aj=0;
//return aj;
//}

u8 KEY_Scan(void)
{	 
	u8 aj=0,x=0;
	if((KEYA==0||KEYR==0||KEYY==0))//K1,K2,K3
		{
			 delay_ms(20);//ȥ���� 
			if(KEYA==0) x|=1;else x&=~1;
			if(KEYR==0) x|=2;else x&=~2;
			if(KEYY==0) x|=4;else x&=~4;
				
			  switch(x)
			{
				case 1 : aj=1; break;
				case 2 : aj=2; break;
				case 4 : aj=3; break;
				case 3 : aj=4; break;
				case 5 : aj=5; break;
				case 6 : aj=6; break;
				case 7 : aj=0; break;
			}
	  }
return aj;

}


