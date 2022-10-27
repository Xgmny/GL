#include "GP8302.h"

extern int32_t  SZ_LL_Z,SZ_LL_F;
extern u8 MNL;
int32_t lins;

void GP8302(int32_t ReadAddr)
{
	if(MNL==30)   //4-20mA   12mA=0
		{
				if(ReadAddr>500  &&  SZ_LL_Z!=0)
					{
					GP8302_Read((3135*((ReadAddr*10000)/SZ_LL_Z)/200000)+1567+788);	
					}
				else if(ReadAddr<-500  &&  SZ_LL_F!=0)
					{ 
						ReadAddr=(~ReadAddr)+1;
						//	lins=(((3135*(SZ_LL_F*10-ReadAddr))/SZ_LL_F/20)+788);
						lins=((SZ_LL_F*10-ReadAddr)*3135);
						lins/=SZ_LL_F;
						lins/=20;
						lins+=	789;
						GP8302_Read(lins);	
						}
				else GP8302_Read(1570+788);	
			}		
	else if(MNL==31)    //4-20mA   4mA=0
		{
       if(ReadAddr>500  &&  SZ_LL_Z!=0)
					{
					GP8302_Read((1852*((ReadAddr*10000)/SZ_LL_Z)/200000)+788);		
					}
				else if(ReadAddr<-500  &&  SZ_LL_F!=0)
					{ 
					ReadAddr=(~ReadAddr)+1;
					GP8302_Read((1852*((ReadAddr*10000)/SZ_LL_F)/200000)+788);	
					}
				else GP8302_Read(788);	
	   }
		else {MNL=30;}
}


u8 GP8302_Read(u16 ReadAddr)
{				  
	u8 temp=0;		  	    																 
	GP8802_Start();
	GP8802_Send_Byte(0XB0);	   //发送写命令
	GP8802_Ack();
	GP8802_Send_Byte(0X02);	   //发送写命令
	GP8802_Ack();
	GP8802_Send_Byte(ReadAddr<<4&0xFF);//发送低地址
	GP8802_Ack();
    GP8802_Send_Byte(ReadAddr>>4&0xFF);   //发送高地址
//	GP8802_Wait_Ack();   
    GP8802_Ack();	
    GP8802_Stop();//产生一个停止条件	
    
	return temp;
}


void GP8302_Init(void)
{					     
//	GPIO_InitTypeDef GPIO_InitStructure;   //39   40 脚
//	
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//使能GPIOB时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   //PB4
//	   
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; //3:DA  4时钟  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4); 	// 输出高
	
		GPIO_InitTypeDef GPIO_InitStructure;  //PA0  PA1   10脚  11脚
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//使能GPIOB时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   //PB4
	   
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //3:DA  4时钟  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1); 	// 输出高
}

void SDA8302_OUT(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void SDA8302_IN(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//使能GPIOA时钟
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;   //推挽输出

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

//产生IIC起始信号
void GP8802_Start(void)
{
	SDA8302_OUT();     //sda线输出
	SDA_8302=1;
delay_us(1);	
	SCL_8302=1;
	delay_us(5);
 	SDA_8302=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	SCL_8302=0;//钳住I2C总线，准备发送或接收数据 
	delay_us(5);
}	  
//产生IIC停止信号
void GP8802_Stop(void)
{
	SDA8302_OUT();//sda线输出
	SCL_8302=0;
    delay_us(5);
	SDA_8302=0;//STOP:when CLK is high DATA change form low to high
	delay_us(5);
	SCL_8302=1; 
	delay_us(5);
	SDA_8302=1;//发送I2C总线结束信号
							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 GP8802_Wait_Ack(void)
{
	u16 ucErrTime=0;
	SDA8302_IN();      //SDA设置为输入  
	//SDA_8302=1;//
	delay_us(1);	 
	
	SCL_8302=1;//delay_us(1); 
	
	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))   //f
		
//	while(READ_SDA)   //
	{
		ucErrTime++;
		if(ucErrTime>205)
		{
			GP8802_Stop();
			return 1;
		}
	}

	SCL_8302=0;//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void GP8802_Ack(void)
{
	SCL_8302=0;
	SDA8302_OUT();
	SDA_8302=0;
	delay_us(4);
	SCL_8302=1;
	delay_us(4);
	SCL_8302=0;
}
//不产生ACK应答		    
void GP8802_NAck(void)
{
	SCL_8302=0;
	SDA8302_OUT();
	SDA_8302=1;
	delay_us(4);
	SCL_8302=1;
	delay_us(4);
	SCL_8302=0;
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void GP8802_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA8302_OUT(); 	    
    SCL_8302=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {     
        SDA_8302=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(4);   //对TEA5767这三个延时都是必须的
		SCL_8302=1;
		delay_us(4); 
		SCL_8302=0;	
    }
} 
