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
	GP8802_Send_Byte(0XB0);	   //����д����
	GP8802_Ack();
	GP8802_Send_Byte(0X02);	   //����д����
	GP8802_Ack();
	GP8802_Send_Byte(ReadAddr<<4&0xFF);//���͵͵�ַ
	GP8802_Ack();
    GP8802_Send_Byte(ReadAddr>>4&0xFF);   //���͸ߵ�ַ
//	GP8802_Wait_Ack();   
    GP8802_Ack();	
    GP8802_Stop();//����һ��ֹͣ����	
    
	return temp;
}


void GP8302_Init(void)
{					     
//	GPIO_InitTypeDef GPIO_InitStructure;   //39   40 ��
//	
//	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );	//ʹ��GPIOBʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   //PB4
//	   
//GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4; //3:DA  4ʱ��  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	GPIO_SetBits(GPIOB,GPIO_Pin_3|GPIO_Pin_4); 	// �����
	
		GPIO_InitTypeDef GPIO_InitStructure;  //PA0  PA1   10��  11��
	
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//ʹ��GPIOBʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   //PB4
	   
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //3:DA  4ʱ��  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1); 	// �����
}

void SDA8302_OUT(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}
void SDA8302_IN(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );	//ʹ��GPIOAʱ��
	   
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;   //�������

	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

//����IIC��ʼ�ź�
void GP8802_Start(void)
{
	SDA8302_OUT();     //sda�����
	SDA_8302=1;
delay_us(1);	
	SCL_8302=1;
	delay_us(5);
 	SDA_8302=0;//START:when CLK is high,DATA change form high to low 
	delay_us(5);
	SCL_8302=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	delay_us(5);
}	  
//����IICֹͣ�ź�
void GP8802_Stop(void)
{
	SDA8302_OUT();//sda�����
	SCL_8302=0;
    delay_us(5);
	SDA_8302=0;//STOP:when CLK is high DATA change form low to high
	delay_us(5);
	SCL_8302=1; 
	delay_us(5);
	SDA_8302=1;//����I2C���߽����ź�
							   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 GP8802_Wait_Ack(void)
{
	u16 ucErrTime=0;
	SDA8302_IN();      //SDA����Ϊ����  
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

	SCL_8302=0;//ʱ�����0 	   
	return 0;  
} 
//����ACKӦ��
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
//������ACKӦ��		    
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
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void GP8802_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA8302_OUT(); 	    
    SCL_8302=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {     
        SDA_8302=(txd&0x80)>>7;
        txd<<=1; 	  
		delay_us(4);   //��TEA5767��������ʱ���Ǳ����
		SCL_8302=1;
		delay_us(4); 
		SCL_8302=0;	
    }
} 
