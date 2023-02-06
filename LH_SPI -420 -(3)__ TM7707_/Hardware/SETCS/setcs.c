#include "key.h"
#include "timer.h"
#include "24c64.h"	
#include "gui.h"
#include "pic.h"
#include "setcs.h"
#include "can.h"
#include "delay.h"

extern u32 NIAN ,WD,LJ;
extern u32 myid;
extern u8 run,MNL;
extern u16 sto,sta;
extern  int32_t  YuanMa ,  MANMA , Ma_xz;   //
static u8 tem;		//光标(值)
static u8 ljl[8];
			u8 key;
			u8 ljks=1;
void CAN_Answer(void)	
{
	u8 shu[8],Ctxd[8];
	CAN_SET();
	AT24CXX_Read(0x01D0,shu,6);
	Ctxd[0]=0X5A;Ctxd[1]=0X53;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01D8,shu,6);
	Ctxd[0]=0X46;Ctxd[1]=0X53;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0110,shu,5);//L
	Ctxd[0]=0X5A;Ctxd[1]=0X4c;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];Ctxd[7]=0x20;
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0170,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X4c;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];Ctxd[7]=0x20;
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0118,shu,5);//Q
	Ctxd[0]=0X5A;Ctxd[1]=0X51;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0178,shu,6);
	Ctxd[0]=0X46;Ctxd[1]=0X51;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0120,shu,5);//0
	Ctxd[0]=0X5A;Ctxd[1]=0X30;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0180,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X30;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0128,shu,5);//1
	Ctxd[0]=0X5A;Ctxd[1]=0X31;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0188,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X31;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0130,shu,5);//2
	Ctxd[0]=0X5A;Ctxd[1]=0X32;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0190,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X32;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0138,shu,5);//3
	Ctxd[0]=0X5A;Ctxd[1]=0X33;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0198,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X33;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0140,shu,5);//4
	Ctxd[0]=0X5A;Ctxd[1]=0X34;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01A0,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X34;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0148,shu,5);//5
	Ctxd[0]=0X5A;Ctxd[1]=0X35;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01A8,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X35;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0150,shu,5);//6
	Ctxd[0]=0X5A;Ctxd[1]=0X36;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01B0,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X36;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0158,shu,5);//7
	Ctxd[0]=0X5A;Ctxd[1]=0X37;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01B8,shu,5);
	Ctxd[0]=0X46;Ctxd[1]=0X37;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0160,shu,5);//8
	Ctxd[0]=0X5A;Ctxd[1]=0X38;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01C0,shu,8);
	Ctxd[0]=0X46;Ctxd[1]=0X38;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);
	AT24CXX_Read(0x0168,shu,5);//9
	Ctxd[0]=0X5A;Ctxd[1]=0X39;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x01C8,shu,5);//10
	Ctxd[0]=0X46;Ctxd[1]=0X39;Ctxd[2]=shu[0];Ctxd[3]=shu[1];
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];
	Can_Send_Msg(myid,Ctxd,8);//发送8个字节 
	delay_ms(50);			
	AT24CXX_Read(0x0050,shu,4);//WK
	Ctxd[0]=0X57;Ctxd[1]=0X4b;Ctxd[2]=shu[0];Ctxd[3]=0x2e;
	Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[6]=shu[4];Ctxd[7]=0x30;Ctxd[7]=0x20;
	Can_Send_Msg(myid,Ctxd,8);
	delay_ms(50);			
	AT24CXX_Read(0x0058,shu,6);//WB
	Ctxd[0]=0X57;Ctxd[1]=0X42;Ctxd[2]=shu[0];Ctxd[5]=0x2e;
	Ctxd[3]=shu[1];Ctxd[4]=shu[2];Ctxd[6]=shu[4];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);
	delay_ms(50);			
	AT24CXX_Read(0x0060,shu,6);//WZ
	Ctxd[0]=0X5A;Ctxd[1]=0X57;Ctxd[2]=shu[0];Ctxd[6]=0x2e;
	Ctxd[3]=shu[1];Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);
	delay_ms(50);			
	AT24CXX_Read(0x0068,shu,6);//WF
	Ctxd[0]=0X46;Ctxd[1]=0X57;Ctxd[2]=shu[0];Ctxd[6]=0x2e;
	Ctxd[3]=shu[1];Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);
	delay_ms(50);			
	AT24CXX_Read(0x0070,shu,6);//ZJ
	Ctxd[0]=0X5A;Ctxd[1]=0X4A;Ctxd[2]=shu[0];Ctxd[6]=0x2e;
	Ctxd[3]=shu[1];Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);
	delay_ms(50);			
	AT24CXX_Read(0x0078,shu,6);//FJ
	Ctxd[0]=0X46;Ctxd[1]=0X4A;Ctxd[2]=shu[0];Ctxd[6]=0x2e;
	Ctxd[3]=shu[1];Ctxd[4]=shu[2];Ctxd[5]=shu[3];Ctxd[7]=shu[5];
	Can_Send_Msg(myid,Ctxd,8);
}
void CAN_SAVE(void)
{
	u8 n0,cc[8];
	for(n0=0;n0<7;n0++)	
		cc[n0]=canbuf_rxd[n0+2];
	if (run==1)
	{
		if((canbuf_rxd[0]=='S')&(canbuf_rxd[1]=='T'))	//
			{
				if (canbuf_rxd[2]=='O') 
				{ljks=0; sto++;}
				else if(canbuf_rxd[2]=='A')		
				{ljks=1; sta++;}
				else;
			}
	}
	else
	{
		if (run==2)
		{
			if(canbuf_rxd[0]=='Z')
			{
				switch (canbuf_rxd[1])
					{
						case 'L':{AT24CXX_Write(0x0110,cc,6);}break;
						case 'Q':{AT24CXX_Write(0x0118,cc,5);}break;
						case '0':{AT24CXX_Write(0x0120,cc,5);}break;
						case '1':{AT24CXX_Write(0x0128,cc,5);}break;
						case '2':{AT24CXX_Write(0x0130,cc,5);}break;
						case '3':{AT24CXX_Write(0x0138,cc,5);}break;
						case '4':{AT24CXX_Write(0x0140,cc,5);}break;
						case '5':{AT24CXX_Write(0x0148,cc,5);}break;
						case '6':{AT24CXX_Write(0x0150,cc,5);}break;
						case '7':{AT24CXX_Write(0x0158,cc,5);}break;
						case '8':{AT24CXX_Write(0x0160,cc,5);}break;
						case '9':{AT24CXX_Write(0x0168,cc,5);}break;
						case 'S':{AT24CXX_Write(0x01D0,cc,5);}break;
						case 'J':{AT24CXX_Write(0x0070,cc,6);}break;
						case 'W':{AT24CXX_Write(0x0060,cc,6);}break;
						default:{};break;
					}
			}		
			if(canbuf_rxd[0]=='F')
			{
				switch (canbuf_rxd[1])
					{
						case 'L':{AT24CXX_Write(0x0170,cc,6);}break;
						case 'Q':{AT24CXX_Write(0x0178,cc,5);}break;
						case '0':{AT24CXX_Write(0x0180,cc,5);}break;
						case '1':{AT24CXX_Write(0x0188,cc,5);}break;
						case '2':{AT24CXX_Write(0x0190,cc,5);}break;
						case '3':{AT24CXX_Write(0x0198,cc,5);}break;
						case '4':{AT24CXX_Write(0x01A0,cc,5);}break;
						case '5':{AT24CXX_Write(0x01A8,cc,5);}break;
						case '6':{AT24CXX_Write(0x01B0,cc,5);}break;
						case '7':{AT24CXX_Write(0x01B8,cc,5);}break;
						case '8':{AT24CXX_Write(0x01C0,cc,5);}break;
						case '9':{AT24CXX_Write(0x01C8,cc,5);}break;
						case 'S':{AT24CXX_Write(0x01D8,cc,5);}break;
						case 'J':{AT24CXX_Write(0x0078,cc,6);}break;
						case 'W':{AT24CXX_Write(0x0068,cc,6);}break;
						default:{};break;
						}		
			}
			if(canbuf_rxd[0]=='W')
			{
				switch (canbuf_rxd[1])
					{
						case 'K':{AT24CXX_Write(0x0050,cc,4);}break;  //
						case 'B':{AT24CXX_Write(0x0058,cc,5);}break;
						default:{};break;
					}
			}
		}
	}	
	if(canbuf_rxd[0]=='L')
			{
//				for(n0=0;n0<8;n0++)
//			    ljl[n0]=0x30;
//		        ljl[6]=0x2e;
//		AT24CXX_Write(0x0200+NIAN,ljl,8);
//		LJ=0;	
			LJ_zero();
		
	}
}
void SET_IN(void)	
{
	page=0;MENU();row=113;col=4;smode=1;wz=0;
}

void SET_COME(void)	
{   
	static u8 fx,xgbz,skey,lsk;   //xgbz=1 保存标志
	static u16 addr;
	static u8 ss[8],ld[8],qc[8],dd[8];
	u32 i=200000;
	u8  mm=0;
	
	skey=key;
	if(skey<4)
	{
		if(lsk==skey)
			{
				if (skey==3)
				skey=0;}
		else
			lsk=skey;
		if(skey==3)		//确定保存数据，翻页
		  {
			if (page==0)
			{
				switch (wz)
				{
					case 0x02:{	
								SIX();page=6;wz=0;addr=0x0050;  //page=6温度设置
									AT24CXX_Read(addr,ss,5);
									addr+=8;
									AT24CXX_Read(addr,ld,5);
									addr+=8;
									AT24CXX_Read(addr,qc,5);
									addr+=8;
									AT24CXX_Read(addr,dd,8);
								
									GUI_ShowString(79,0, ss,5,16,1);//显示温度数值
									GUI_ShowString(79,16,ld,5,16,1);
									GUI_ShowString(79,32,qc,5,16,1);
									GUI_ShowString(79,48,dd,5,16,1);

				      GUI_ShowString(0,48,dd+7,1,16,1);					//模拟量模式位		
						  MNL=dd[7];
													//模拟量标志符号
						          //模拟量标志符号


						
								    row=79;col=0;tem=ss[wz];smode=1;}break;
					case 0x03:{
								SEV();page=7;wz=2;
								AT24CXX_Read(0X0108,ld,3);
								GUI_ShowString(47,38,ld,3,16,1);
								row=63;col=38;tem=ld[wz];smode=1;
								}break;
					case 0x00:	{
									ONE();page=1;xgbz=0;
									fx=0;	GUI_ShowCHinese(0,0,16,"正",1);	
									addr=0x0110;wz=0;
									AT24CXX_Read(addr,ss,6);
									addr+=8;
									AT24CXX_Read(addr,ld,5);
									addr+=8;
									AT24CXX_Read(addr,qc,5);
									GUI_ShowString(47,16,ss,6,16,1);
									GUI_ShowString(47,32,ld,5,16,1);
									GUI_ShowString(47,48,qc,5,16,1);
									col=16;row=47;tem=ss[wz];
								}break;
					case 0x01:	{
									ONE();page=1;xgbz=0;
									fx=1;GUI_ShowCHinese(0,0,16,"反",1);
									addr=0x0170;wz=0;
									AT24CXX_Read(addr,ss,6);
									addr+=8;
									AT24CXX_Read(addr,ld,5);
									addr+=8;
									AT24CXX_Read(addr,qc,5);
									GUI_ShowString(47,16,ss,6,16,1);
									GUI_ShowString(47,32,ld,5,16,1);
									GUI_ShowString(47,48,qc,5,16,1);
									col=16;row=47;tem=ss[wz];
								}break;
					}
			}
			else
			{
				if(page==6) //保存??
				{
					if (xgbz==1)
						{
							if(col==0) ss[wz]=tem;
							if(col==16) ld[wz]=tem;
							if(col==32) qc[wz]=tem;
							if(col==48) dd[wz]=tem;
							addr=0x0050;
							AT24CXX_Write(addr,ss,5);
							addr+=8;AT24CXX_Write(addr,ld,6);
							addr+=8;AT24CXX_Write(addr,qc,6);
							addr+=8;AT24CXX_Write(addr,dd,8);
						}
						while(i){i--;
											if(KEY_Scan()==1) 
												mm=mm|1;
											if(KEY_Scan()==2) 
												mm=mm|2;
											if(KEY_Scan()==4) 
												mm=mm|4;
										}
					 if(mm==7){	
								QJHM();row=63;col=16;smode=1;wz=0;page=8;xgbz=0;   //设置第2页
		//						AT24CXX_Read(0x0070,ss,6);
								AT24CXX_Read(0x0090,ld,8);
								AT24CXX_Read(0x0080,qc,8);
								AT24CXX_Read(0x0088,dd,8);
						    GUI_ShowString(63,16,ld,8,16,1);
								GUI_ShowString(63,32,qc,8,16,1);
								GUI_ShowString(63,48,dd,8,16,1);
								col=16;row=63;tem=qc[wz];xgbz=0;tem=ld[0];
					     }
					else{	MENU();row=113;col=4;smode=0;wz=0;page=0;	}//跳回主页面
				}
				else
				{
					if (page==8)   //设置 2页
						{
							if (xgbz==1)
							{
								if(col==16) ld[wz]=tem;
				        if(col==32) qc[wz]=tem;
								if(col==48) dd[wz]=tem;
//								AT24CXX_Write(0x0070,ss,6);   角度
								AT24CXX_Write(0x0090,ld,8);
								AT24CXX_Write(0x0080,qc,8);
								AT24CXX_Write(0x0088,dd,8);
		            Ma_xz		= A_N_24C64(8,ld,0x0090);		//0点原码修正
								YuanMa	= A_N_24C64(8,qc,0x0080);		//原码  满-0
								MANMA		= A_N_24C64(8,dd,0x0088);		//满度压力 Pa   MANMA

							}
							MENU();row=113;col=4;smode=0;wz=0;page=0;	
						}
					if (page==7)
						{
							if (xgbz==1)
							{
				        ld[wz]=tem;
								AT24CXX_Write(0x0108,ld,3);
								myid=(ld[0]-0x30)*100+(ld[1]-0x30)*10+(ld[2]-0x30);
								if (myid>255) myid=255;
								myid=0x11940400+myid;  //16位 +10位
		
							}
							MENU();row=113;col=4;smode=0;wz=0;page=0;	
						}
					if (page==5)
						{
							if (xgbz==1)
							{
								ss[wz]=tem;
								if(fx==0) 
									addr=0x01D0;
									else 
										addr=0x01D8;
							AT24CXX_Write(addr,ss,6);
							}
							MENU();row=113;col=4;smode=0;wz=0;page=0;	
						}
					if ((page==1)|(page==2)|(page==3)|(page==4))
					{
						if (xgbz==1)
						{
							if(col==16) ss[wz]=tem;
							if(col==32) ld[wz]=tem;
							if(col==48) qc[wz]=tem;
							if (page==1)
								{if(fx==0) addr=0x0110;
									else addr=0x0170;}
							if (page==2)
								{if(fx==0) addr=0x0128;
									else addr=0x0188;}	
							if (page==3)
								{if(fx==0) addr=0x0140;
									else addr=0x01a0;}
							if (page==4)
								{if(fx==0) addr=0x0158;
									else addr=0x01b8;}	
						if(page==1)	AT24CXX_Write(addr,ss,6);// 零点
						else	AT24CXX_Write(addr,ss,5);
							addr+=8;AT24CXX_Write(addr,ld,5);
							addr+=8;AT24CXX_Write(addr,qc,5);
						}
						page++;wz=0;
						if (page==5)
						{	
							FIVE();	
							if(fx==0) 
									addr=0x01D0;
							else 
									addr=0x01D8;
							if(fx==0)
									GUI_ShowCHinese(5,16,16,"正",1);	
							else
									GUI_ShowCHinese(5,16,16,"反",1);
							AT24CXX_Read(addr,ss,6);
							GUI_ShowString(24,38,ss,6,16,1);
							col=38;row=24;tem=ss[wz];xgbz=0;
							}
						else
						{
							if (page==2)
								{	TWO();
									if(fx==0) addr=0x0128;
									else addr=0x0188;}
							if (page==3)
								{	THERR();
									if(fx==0) addr=0x0140;
									else addr=0x01a0;}
							if (page==4)
								{	FOUR();	if(fx==0) addr=0x0158;
									else addr=0x01b8;}				
							if(fx==0)
									GUI_ShowCHinese(0,0,16,"正",1);	
							else
								GUI_ShowCHinese(0,0,16,"反",1);
							AT24CXX_Read(addr,ss,5);
							addr+=8;AT24CXX_Read(addr,ld,5);
							addr+=8;AT24CXX_Read(addr,qc,5);
							GUI_ShowString(80,16,ss,5,16,1);
							GUI_ShowString(80,32,ld,5,16,1);
							GUI_ShowString(80,48,qc,5,16,1);
							col=16;row=80;tem=ss[wz];xgbz=0;
						}
					}
				}	
			xgbz=0;
			}
		}
	if(skey==2)//右
		{
 			if (page==0) 
				{   GUI_ShowChar(113,col,32,8,1);
					col+=16;wz++;smode=0;
					if(col>64) {col=4;wz=0;}
					GUI_ShowChar(113,col,32,8,0);
				}
			 if ((page==1)|(page==2)|(page==3)|(page==4))
				{	GUI_ShowChar(row,col,tem,16,1);
					if (col==16) ss[wz]=tem;
					if (col==32) ld[wz]=tem;
					if (col==48) qc[wz]=tem;
					row+=8;wz++;smode=1;
					if(page==1)
					{
						if(col==48)
						{	if(row==55) {row+=8;wz++;}}
						else
							{	if(row==63) {row+=8;wz++;}}
						if (row>=95&&col==16)	{row=47;col+=16;wz=0;	if(col>=64) col=16;}//第一行   换行	
						if (row>=87&&col!=16)	{row=47;col+=16;wz=0;	if(col>=64) col=16;}//第二三行   换行
						if (col==16) tem=ss[wz];
						if (col==32) tem=ld[wz];
						if (col==48) tem=qc[wz];
						GUI_ShowChar(row,col,tem,16,1);
					}						
					if ((page==2)|(page==3)|(page==4))
					{	
						
						if(row==88) {row+=8;wz++;}
						if (row>=120)
							{row=80;col+=16;wz=0;
							if(col>=64) col=16;}	
					}
					if (col==16) tem=ss[wz];
					if (col==32) tem=ld[wz];
					if (col==48) tem=qc[wz];
					GUI_ShowChar(row,col,tem,16,1);
				}	
			 if (page==5)
				{	GUI_ShowChar(row,col,tem,16,1);
					ss[wz]=tem;
					row+=8;wz++;smode=1;
					if(row==48) {row+=8;wz++;}
					tem=ss[wz];
					if (row>64)
							{row=24;wz=0;tem=ss[wz];}
					GUI_ShowChar(row,38,tem,16,1);
				}
			if (page==6) //温度设置光标
				{
					GUI_ShowChar(row,col,tem,16,1);
					if (col==0)  ss[wz]=tem;
					if (col==16) ld[wz]=tem;
					if (col==32) qc[wz]=tem;
					if (col==48) dd[wz]=tem;
					row+=8;wz++;smode=1;    //wz 行位   row光标显示位置
		
						if (col==0){
							if (wz==99)  {row+=8;wz++;}			else;   //跳过小数点
							if (wz>=5)	 {row=79;col+=16;wz=0;}	else;
						   }	
						if (col==16){
							if (wz==3)  {row+=8;wz++;}			else;//跳过小数点
							if (wz>=5)	{row=79;col+=16;wz=0;}	else;
						   }	
						
						if (col==32){
							if (wz==3)  {row+=8;wz++;}			else;
							if (wz>=5)	{row=0;col+=16;wz=7;}	else;  //4~20ma   row（x） 0位col(Y)   WZ（位)
							}	
						
						if (col==48){
							if (row==8)  {row=79;wz=0;}           else;
							if (wz==3)  {row+=8;wz++;}			else;
							if (wz==5)	{row=79;col+=16;wz=0;}	else;
							}	
						
						if(col>=64) col=0;
								
						if (col==0)  tem=ss[wz];//第1行
						if (col==16) tem=ld[wz];//第2行
						if (col==32) tem=qc[wz];//第3行
						if (col==48) tem=dd[wz];//第4行
						GUI_ShowChar(row,col,tem,16,1);//光标显示位置   第几行     
							
					 }
			if (page==7)
				{	GUI_ShowChar(row,col,tem,16,1);
					 ld[wz]=tem;
					row+=8;wz++;smode=1;
					if(wz==3) {row=47;wz=0;}
					tem=ld[wz];
					GUI_ShowChar(row,38,tem,16,1);
				}
			if (page==8)//设置2页
				{	
					GUI_ShowChar(row,col,tem,16,1);
					if (col==16) ld[wz]=tem;
					if (col==32) qc[wz]=tem;
					if (col==48) dd[wz]=tem;
					row+=8;wz++;smode=1;
	//				if (wz==4) {row+=8;wz++;}
					if (wz>=8) {row=63;col+=16;wz=0;}
					if (col>=64|wz>=8) col=16;
					if (col==16) tem=ld[wz];
					if (col==32) tem=qc[wz];
					if (col==48) tem=dd[wz];
					GUI_ShowChar(row,col,tem,16,1);
				 }
	    delay_ms(300);		//手感		
		}
		if(skey==1)//+
		{
 			if(page==0)
			{	GUI_ShowChar(113,col,32,8,1);
					col+=16;wz++;smode=0;
					if(col>64) {col=4;wz=0;}
					GUI_ShowChar(113,col,32,8,0);
			}
			else
				{
					xgbz=1;
					
					if( (((page==6))&((col==0)|(col==16)|(col==32)|(col==48))&((wz==0)|(wz==7))) | ( (page==8)&(col==16)&(wz==0) ) ) //"+"  ."-"号
						{
							if(wz==0){
									if (tem==0x20)
										tem=0x2d;   //是"-"
									else
										tem=0x20;   //" "   2b  "+"
							  }else;
							
							if(wz==7){       //4-20 ma切换
									if(tem==0x30)
										{tem=0x31;MNL=0x01;}
//										{tem=0x3c;MNL=0x01;} 3C<
									else
										{tem=0x30;MNL=0x00;}
										
										
								}else;
						}
					else
					{
						tem++;
						if (tem>=0x3a) tem=0x30;     //设置数字
						}
					smode=1;
					GUI_ShowChar(row,col,tem,16,1);
				}
		}
	}
	if (smode==1) 
			smode=0;
	else
			smode=1;
	
	if(page==0)
			GUI_ShowChar(113,col,32,8,smode);
	else{
			GUI_ShowChar(row,col,tem,16,smode);
   }      
}
