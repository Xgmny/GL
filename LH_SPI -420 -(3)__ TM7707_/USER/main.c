#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "24c64.h"	
#include "wdg.h"
#include "timer.h"
#include "usart.h"
#include "usmart.h"
#include "oled.h"
#include "setcs.h"
#include "can.h"
#include "gui.h"
#include "pic.h"
#include "CS1239.h"
#include "TM7707.h"
#include "mport.h"
#include "IIC__.h"
#include "GP8302.h"
#include "mport.h"

  u8 Version[8]={0x32,0x32,0x2e,0x30,0x36,0x2e,0x32,0x32};
  u8 lwd[8]={0x2b,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 lwd_pa[8]={0x2b,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 lll[8]={0x2b,0x33,0x32,0x30,0x2e,0x30,0x32,0x30};
	u8 ljl[8]={0x2b,0x33,0x32,0x30,0x30,0x32,0x2e,0x30};
	u8 lsl[8]={0x2b,0x33,0x32,0x2e,0x30,0x32};
	u8 cyl[8]={0x20,0x30,0x30,0x30,0x2e,0x30,0x30,0x30};
	u8 cyl_0[8]={0x20,0x30,0x30,0x30,0x2e,0x30,0x30,0x30};
	u8 jdl[8]={0x2b,0x33,0x32,0x2e,0x30,0x32};
	
	u8 Error=0;  //ϵͳ����
	int32_t LJ;
	int32_t LL,CY,WenDu;
	u8 ms1000=0,K1=0,K2=0,K13=0;
	int8_t Gd,Gd_y;
	u16 A;
	u16 SZ_JZ_Z[10], SZ_JZ_F[10] ;                     //   %��У��
	u16 SZ_LD_Z, SZ_LD_F, SZ_QC_Z, SZ_QC_F ;       //   0��    �г�
	int16_t SZ_WD_B ,SZ_WD_O,SZ_WD_KZ,SZ_WD_KF;
	int32_t  SZ_LL_Z,SZ_LL_F;										//   ����������  
  int32_t  YuanMa ,  MANMA , Ma_xz=0;   //
   extern int32_t WD_Ohm,WD,WD_M;
   extern u32 NIAN;
   extern u8 key,se30,sec3,sec,ljks,slj3,slj30,have;
   extern u8 idata,date8[],error,JNW[];
   extern u16 QJs;
   extern u32 myid;
   u8 rxd_bz,canbuf_rxd[8],run;
	
 int main(void)
 {	
	int8_t gd;
	u8 cnt=0,old_key=0,hmqh,yei;
	u8 canbuf_txd[8]={0x00,0x00,0x32,0x30,0x30,0x30,0x2e,0x30};
  u8 OFF_error=1;
	delay_init();	    	 //��ʱ������ʼ��	
	OLED_Init();
	GUI_ShowCHinese(0,15,32,"�����Ǳ�",1);
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(9600);	 	//���ڳ�ʼ��Ϊ115200
	QingJiao_19200();
	uart_init(19200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//������ʼ��		
	AT24CXX_Init();			//IIC��ʼ�� 
	GP8302_Init();
	TM7707_init( );

	FIRST(); ljks=1;hmqh=0;
	TIM4_init(1999,7199);//��ʱ��4 200����

	CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,8,0);//CAN��ʼ,������250Kbps 
									//������  mode:0,��ͨģʽ;1,�ػ�ģʽ; //  
  // IWDG_Init(4,6250);      //��10s
	 run=1;      //����
//	LED0=1;        
 	while(1)
	{	

    while( ( Error && OFF_error) )  { 
					OLED_error(Error); 
					Made_Data();  //AD���
					delay_ms(800);
					if (4==KEY_Scan())OFF_error=0;else;
					if(Error==0){NVIC_SystemReset();}else;
		}//ϵͳ�����ж�   ����
		rxd_bz=Can_Receive_Msg(canbuf_rxd);//���յ�������	
		if (rxd_bz)   
		   {
		    	
			if (canbuf_rxd[0]=='O')
			{
				if((canbuf_rxd[1]=='U')&(canbuf_rxd[2]=='T'))
				      {BL_24c64();run=1;cnt=0;FIRST();
					     slj30=0;se30=0;ms200=0;}//����
      }
			else
			{
				if (canbuf_rxd[0]=='$')
				{	
					if (canbuf_rxd[1]=='Y')
						{	run=2;
							delay_ms(5);
							CAN_Answer();
						}
				}	
				else 
				CAN_SAVE();
			}
		}
		if (run<2)
		{
			if(ms200==1)			//200����
			{
				key=KEY_Scan();
				if (key==4) 
					{
						if(have==0)
								{have=1;sec3=0;slj3=0;}
							else
							{
								if(sec3) 
								{
									sec3=0;have=0;
									if(run==1)
									{
										run=0;   SET_IN();   //����
									}
									else 
									{
										BL_24c64();  //??????????
										run=1;  cnt=0; hmqh=0 ;FIRST(); }//�˳�����
									}
							}			
						}		
				else
					{
						if (have)
							have=0;
						if(run==1)
							{
								if((key==0)&(old_key==3))
								{
									Gd_y =0;
									if (hmqh>=1)
										{	hmqh=0;FIRST();}
									else
											{hmqh=1;FIRST_2();}
								}else;
								////////////
								if(hmqh && key==2) {if(Gd_y>30){Gd--;FIRST_2();gd=Gd;} else Gd_y=0;}
								else;
								if(hmqh && key==1) {if(Gd_y>30){Gd++;if(Gd>0)Gd=0;FIRST_2();gd=Gd;} else Gd_y++;}
								else;
							 if(ms1000==2)
							   {	
								  ms1000=0;
								if (!hmqh)	
								{	
//									GP8302_Read((3135*(js/200.0))+788);
														GUI_ShowString(34, 3,lll,8,16,1);					 // ����
									
										if(ljks)GUI_ShowString(34,24,ljl,8,16,1);					//ljl   lsl
										else    GUI_ShowString(34,24,ljl,8,16,0);					//ljl   lsl
									
										if(LL==0)GUI_ShowString(34,45,cyl_0,8,16,1);	  			//��ѹ
									  else     GUI_ShowString(34,45,cyl,8,16,1);
									
									if(key==1)K1++;  else K1=0; //K1
									if(key==6)K2++;  else K2=0; //K2
									if(key==5)K13++; else K13=0;
									
									if(K1==9 && Ma_xz!=-1)  CY_zero();
									if(K1==3 && Ma_xz==-1)  CY_zero_1();
//								if(K2==5 && Ma_xz==-1)  LJ_zero();
									if( K2==5 )             LJ_zero();
								}
								else{
									if(Gd_y<30)
									  {
//											u16 man=0,ling=0;
//											if(key==1)TM7705_LingDu(1);
//											ling=TM7705_Read2Byte();
//											if(key==2)TM7705_ManDu(1);	
//											man=TM7705_Read2Byte();
										GUI_ShowString(4, 16,lwd,5,16,1); 
				//						GUI_ShowNum(34,24,QJs,7,16,1); 
										if(Ma_xz==-1)
											{
												GUI_ShowString(69,16,lwd_pa ,5,8,1);	//	
												GUI_ShowNum(69,24,WD_M,5,8,1);	//ѹ���¶Ȳ���
											}
										else GUI_ShowString(69,16,lwd_pa ,5,16,1);	//
										
											GUI_ShowString(3,37,lll,8,8,1);	//����
											GUI_ShowString(68,37,cyl,8,8,1);	  			//��ѹ
									//	GUI_ShowString(4,33,jdl	,6,16,1);	//jdl���	jdl	
//										GUI_ShowNum(4,33,ling,6,16,1);
//										GUI_ShowNum(62,33,man,6,16,1);
										
									  }
								    else
									   {				
										if(gd<6 && gd>=0)GUI_ShowString(75,3+10*gd++,Version ,8,8,1);else gd++; // �汾��	 
										if(gd<6 && gd>=0)GUI_ShowString(75,3+10*gd++,lwd,5,8,1); 			else gd++;
										if(gd<6 && gd>=0)GUI_ShowString(75,3+10*gd++,lwd_pa ,5,8,1);		else gd++;
										if(gd<6 && gd>=0)GUI_ShowString(75,3+10*gd++,lsl,8,8,1);			else gd++;
										if(gd<6 && gd>=0)GUI_ShowNum_WD(81,3+10*gd++,WD_Ohm ,6,8,1); 		else gd++;
										if(gd<6 && gd>=0)GUI_ShowString(75,3+10*gd++,jdl,6,8,1);			else gd++;
								//	 	if(gd<6 && gd>=0)GUI_ShowNum(75,47,QJs,7,8,1); //���	jdl
										if(gd<6 && gd>=0)GUI_ShowNum(75,3+10*gd++,NIAN ,3,8,1); 			else gd++;
										if(gd<6 && gd>=0)GUI_ShowString(75,3+10*gd++,JNW,8,8,1); 			else gd++;
										if(gd<6 && gd>=0)GUI_ShowNum(75,3+10*gd++,error ,3,8,1);			else gd++;
										if(gd<6 && gd>=0)GUI_ShowNum(87,3+10*gd++,((*(u32*)0x40006418))&0x00000007 ,1,8,1);else gd++;//CAN_ESR BEW
										if(gd<6 && gd>=0)GUI_ShowNum(87,3+10*gd++,((*(u32*)0x40006418)>>4)&0x00000007 ,1,8,1);else gd++; // 
										if(gd<6 && gd>=0)GUI_ShowNum(75,3+10*gd++,((*(u32*)0x40006418)>>16)&0x000000FF ,3,8,1);else gd++; // 
										if(gd<6 && gd>=0)GUI_ShowNum(75,3+10*gd++,((*(u32*)0x40006418)>>24)&0x000000FF ,3,8,1);else gd++; //
										if(gd<6 && gd>=0)GUI_ShowNum(75,3+10*gd++,(CAN1->MCR)&0x000000FF ,3,8,1);else gd++; //
										if(gd<6 && gd>=0)GUI_ShowNum(87,3+10*gd++,(CAN1->RF0R)&0x00000003 ,1,8,1);else gd++; // 
										if(gd<6 && gd>=0)GUI_ShowNum(69,3+10*gd++,((*(u32*)0x1FFFF7E0))&0x0000FFFF, 8,8,1);else gd++; // 
										if(gd<6 && gd>=0)GUI_ShowNum(69,3+10*gd++,((*(u32*)0x1FFFF7E8))>>8 ,8,8,1);else gd++; //  
										if(gd<6 && gd>=0)GUI_ShowNum(69,3+10*gd++,((*(u32*)0x1FFFF7E8)) ,8,8,1);else gd++; // 	 
										
										   gd=Gd;						
									    }											
									}
							  }else ms1000++;
							
							old_key=key;
									
							}
							else
								SET_COME();	//����
						}
				  if (run==1)	
					{
						 Made_Data();		//ADת��	
						 if(se30)
						 {LJLL_Data();se30=0;}			//=1�ۼ�ʹ��  30��
						cnt++;
						if (cnt>=255) cnt=0;
						canbuf_txd[0]=cnt;

						canbuf_txd[2]=LL/100>>8;											//CAN����ʹ��
						canbuf_txd[3]=LL/100;
						canbuf_txd[4]=LJ>>24;
						canbuf_txd[5]=LJ>>16;
						canbuf_txd[6]=LJ>>8;
						canbuf_txd[7]=LJ;
						 
//						canbuf_txd[2]=CY>>8;												//�¶Ȳ���ʹ��
//						canbuf_txd[3]=CY;
//						canbuf_txd[4]=SZ_LD_Z>>8;
//						canbuf_txd[5]=SZ_LD_Z;
//						canbuf_txd[6]=WenDu>>8;
//						canbuf_txd[7]=WenDu;
						 
						 
						 
						Can_Send_Msg(myid,canbuf_txd,8);//����8���ֽ� 
						
					}
				ms200=0;  
			}
		}	IWDG->KR=0XAAAA;//reload
	}
	
}
