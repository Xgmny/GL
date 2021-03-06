#include "TM7707.h"
#include "led.h"
#include "gui.h"

u8 kan,S_7705=5,error;
u8 TM;  //7705_1  TM=1    7705_2  TM=0 
// Target : TM7707



enum
{
	/* 寄存器选择  RS2 RS1 RS0  */
	REG_COMM	= 0x00,	/* 通信寄存器 */
	REG_SETUP	= 0x10,	/* 设置寄存器 */
	REG_CLOCK_H	= 0x20,	/* 滤波器低寄存器 高 */
	REG_CLOCK_L	= 0x50,	/* 滤波器低寄存器 低 */
	REG_DATA	= 0x30,	/* 数据寄存器 */
	REG_ZERO_CH1	= 0x60,	/* CH1 偏移寄存器 */
	REG_FULL_CH1	= 0x70,	/* CH1 满量程寄存器 */
	REG_ZERO_CH2	= 0x61,	/* CH2 偏移寄存器 */
	REG_FULL_CH2	= 0x71,	/* CH2 满量程寄存器 */

	/* 读写操作 */
	WRITE 		= 0x00,	/* 写操作 */
	READ 		= 0x08,	/* 读操作 */

	/* 通道 */
	CH_1		= 0,	/* AIN1+  AIN1- */
	CH_2		= 1,	/* AIN2+  AIN2- */
	CH_3		= 2,	/* AIN1-  AIN1- */
	CH_4		= 3		/* AIN1-  AIN2- */
};
/* 设置寄存器bit定义 */
enum
{
	MD_NORMAL		= (0 << 6),	/* 正常模式 */
	MD_CAL_SELF		= (1 << 6),	/* 自校准模式 */
	MD_CAL_ZERO		= (2 << 6),	/* 校准0刻度模式 */
	MD_CAL_FULL		= (3 << 6),	/* 校准满刻度模式 */

	GAIN_1			= (0 << 3),	/* 增益 */
	GAIN_2			= (1 << 3),	/* 增益 */
	GAIN_4			= (2 << 3),	/* 增益 */
	GAIN_8			= (3 << 3),	/* 增益 */
	GAIN_16			= (4 << 3),	/* 增益 */
	GAIN_32			= (5 << 3),	/* 增益 */
	GAIN_64			= (6 << 3),	/* 增益 */
	GAIN_128		= (7 << 3),	/* 增益 */

	/* 无论双极性还是单极性都不改变任何输入信号的状态，它只改变输出数据的代码和转换函数上的校准点 */
	BIPOLAR			= (0 << 2),	/* 双极性输入 */
	UNIPOLAR		= (1 << 2),	/* 单极性输入 */

	BUF_NO			= (0 << 1),	/* 输入无缓冲（内部缓冲器不启用) */
	BUF_EN			= (1 << 1),	/* 输入有缓冲 (启用内部缓冲器) */

	FSYNC_0			= 0,
	FSYNC_1			= 1		/* 不启用 */
};

/* 滤波器低寄存器高 */
enum
{
	CLKDIS_0	= 0x00,		/* 时钟输出使能 （当外接晶振时，必须使能才能振荡） */
	CLKDIS_1	= 0x10,		/* 时钟禁止 （当外部提供时钟时，设置该位可以禁止MCK_OUT引脚输出时钟以省电 */
       BST_1      = 0x20,     //大电流

	/*
		2.4576MHz（CLKDIV=0 ）或为 4.9152MHz （CLKDIV=1 ），CLK 应置 “0”。
		1MHz （CLKDIV=0 ）或 2MHz   （CLKDIV=1 ），CLK 该位应置  “1”
	*/
	CLK_4_9152M = 0x08,
	CLK_2_4576M = 0x00,
	CLK_1M 		= 0x04,
	CLK_2M 		= 0x0C,

	FS_50HZ		= 0x00,
	FS_60HZ		= 0x01,
	FS_250HZ	= 0x02,
	FS_500HZ	= 0x04,

	/*
		四十九、电子秤应用中提高TM7705 精度的方法
			当使用主时钟为 2.4576MHz 时，强烈建议将时钟寄存器设为 84H,此时数据输出更新率为10Hz,即每0.1S 输出一个新数据。
			当使用主时钟为 1MHz 时，强烈建议将时钟寄存器设为80H, 此时数据输出更新率为4Hz, 即每0.25S 输出一个新数据
	*/
	ZERO_0		= 0x00,
	ZERO_1		= 0x80
};
/* 滤波器低寄存器高 */
enum
{
FS7		=(7 << 1),
FS6		=(6 << 1),
FS5		=(5 << 1),
FS4		=(4 << 1),
FS3		=(3 << 1),
FS2		=(2 << 1),	
FS1		=(1 << 1),
FS0		=(1 << 0)	
};

//u8 *intp;
//u8 idata,date8[180];
//void write_byte1(u8 date) //写一字节数据
//{
// u8 i;
// CS_1=0;    //CS 拉低，

//	for(i=0;i<8;i++)
//	{
//		SCLK=0; delay_us(2); //SCLK 拉低准备写数据
//		
//		if(date&0x80)  DIO=1;
//		else           DIO=0;
//		
//		SCLK=1; delay_us(2); // 拉高，写入一位数据
//		date<<=1;
//	}
//	CS=1; //写完一个字节后CS 拉高。
//}


//u8 read_byte1 () //读一字节数据
//{
//	u8 j,b1;
//	CS=0;
//	for(j=0;j<8;j++)
//	{
//		SCLK=0; delay_us(2);
//		b1=(b1<<1)|DII;
//		SCLK=1; delay_us(2);
//	}
//		return b1;
//}


//void save_data(void)
//{
//	u8 i0,i1;
//	intp=date8;
//  for(i0=0;i0<60;i0++)
//     {
//		while(DRDY); //等待DRDT 为低时,可读出数据
//		write_byte1(0x38); //向通信寄存器写入数据，选择通道1 作为有效，将下一

//		for(i1=0;i1<3;i1++) //连续存储一个数据。
//		{
//		 *intp=read_byte1();
//		  intp++;
//		}
//		 CS_1=1;
//      }
//}


void TM7707_init(void) //初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_15;	
	                                // 10 CS_1   11 CS_2	 12 REST;    13 SCLK      15 DIN;
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 	//	GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_14;
	                                //  8 DRDY_1;  9 DRDY_2;   14 DOUT    
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING ;		
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
    GPIO_SetBits (GPIOB,GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13);   //CS, REST上拉  
 

//write_byte1(0X10); //向通信寄存器写数据,选择通道1 作为有效通道，将下一个操作设为对设置寄存器进行写操作。
//write_byte1(0X20); //对设置寄存器写操作，选择16 倍增益,无BUF 缓冲器,双极性。
//write_byte1(0X50); //向通信寄存器写数据，选择通道1 作为有效通道,将下一个操作设为对滤波低寄存器进行写操作。
//write_byte1(0X00); //向滤波低寄存器写入0X00。
//write_byte1(0X20); //向通信寄存器写数据，选择通道1 作为有效通道,将下一个操作设为对滤波高寄存器进行写操作。
//write_byte1(0X0F); //向滤波高寄存器写入0X0F。
 //	TM7705_WriteByte(0x10);    TM7705_WriteByte(0x20);//ain1
//	TM7705_WriteByte(0x20);    TM7705_WriteByte(0x2f);//ain1
//	TM7705_WriteByte(0x50);    TM7705_WriteByte(0x50);//ain1  
_7707_REST();

GUI_Draw_sin(7,1);
//delay_ms(1600);
//	  write_byte1(0X20);  write_byte1(0X25);  //滤波器高寄存器   大电流
	
}

void DIO_IN( ) //初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_14;		
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_IN_FLOATING; 		
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void DIO_OUT( ) //初始化
{
	GPIO_InitTypeDef  GPIO_InitStructure;					
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	

	GPIO_InitStructure.GPIO_Pin   =  GPIO_Pin_14;		
	GPIO_InitStructure.GPIO_Mode  =  GPIO_Mode_Out_PP; 		
	GPIO_InitStructure.GPIO_Speed =  GPIO_Speed_50MHz;		
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Delay
*	功能说明: CLK之间的延迟，时序延迟. 用于STM32F407  168M主频
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Delay(void)
{
	uint16_t i;

	for (i = 0; i < 5; i++);
}

/*


*********************************************************************************************************
*	函 数 名: TM7705_SyncSPI
*	功能说明: 同步TM7705芯片SPI接口时序
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_SyncSPI(void)
{
	/* AD7705串行接口失步后将其复位。复位后要延时500us再访问 */
	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);
	TM7705_Send8Bit(0xFF);	
	CS1_1();
	CS2_1();
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Send8Bit
*	功能说明: 向SPI总线发送8个bit数据。 不带CS控制。
*	形    参: _data : 数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Send8Bit(uint8_t _data)
{
	uint8_t i;

	for(i = 0; i < 8; i++)
	{
		if (_data & 0x80)
		{
			DI_1();
		}
		else
		{
			DI_0();
		}
		SCK_0();
		_data <<= 1;
		TM7705_Delay();
		SCK_1();	
		TM7705_Delay();	
	
	}

}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Recive8Bit
*	功能说明: 从SPI总线接收8个bit数据。 不带CS控制。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static uint8_t TM7705_Recive8Bit(void)
{
	uint8_t i;
	uint8_t read = 0;
	for (i = 0; i < 8; i++)
	{
		SCK_0();
		TM7705_Delay();		
		read = read<<1;
		if (DO_IS_HIGH())
		{
			read++;
		}
		SCK_1();		
		TM7705_Delay();
	}

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WriteByte
*	功能说明: 写入1个字节。带CS控制
*	形    参: _data ：将要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
 void TM7705_WriteByte(uint8_t _data)
{
	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	
	TM7705_Send8Bit(_data);
	
	CS2_1();
	CS1_1();
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Write3Byte
*	功能说明: 写入3个字节。带CS控制
*	形    参: _data ：将要写入的数据
*	返 回 值: 无
*********************************************************************************************************
*/
static void TM7705_Write3Byte(uint32_t _data)
{
	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	TM7705_Send8Bit((_data >> 16) & 0xFF);
	TM7705_Send8Bit((_data >> 8) & 0xFF);
	TM7705_Send8Bit(_data);
	CS1_1();
	CS2_1();
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ReadByte
*	功能说明: 从AD芯片读取一个字（16位）
*	形    参: 无
*	返 回 值: 读取的字（16位）
*********************************************************************************************************
*/
static uint8_t TM7705_ReadByte(void)
{
	uint8_t read;

	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	read = TM7705_Recive8Bit();
	CS1_1();
	CS2_1();
	
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Read2Byte
*	功能说明: 读2字节数据
*	形    参: 无
*	返 回 值: 读取的数据（16位）
*********************************************************************************************************
*/
static uint16_t TM7705_Read2Byte(void)
{
	uint16_t read;

	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	CS1_1();
	CS2_1();

	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_Read3Byte
*	功能说明: 读3字节数据
*	形    参: 无
*	返 回 值: 读取到的数据（24bit) 高8位固定为0.
*********************************************************************************************************
*/
 uint32_t TM7705_Read3Byte(void)
{
	uint32_t read;

	if(TM)	CS1_0(); 
    else    CS2_0(); 	
	delay_us(1);
	read = TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	read <<= 8;
	read += TM7705_Recive8Bit();
	CS1_1();
	CS2_1();
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WaitDRDY
*	功能说明: 等待内部操作完成。 自校准时间较长，需要等待。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
u8 TM7705_WaitDRDY(void)
{
	uint32_t i;

	for (i = 0; i < 70000; i++)
	{
		if(TM)
		  {  if (DRDY1_IS_LOW()) {return 1;}	else; }	
		else
		  {  if (DRDY2_IS_LOW()) {return 1;}	else; }
	}
	if(i>=70000){
		
		S_7705=0;
		
		_7707_REST();
		error++;
		return 0;
		
	  }  
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_WriteReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。 对于8位的寄存器，取32位形参的低8bit
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_WriteReg(uint8_t _RegID, uint32_t _RegValue)
{
	uint8_t bits;

	switch (_RegID)
	{
		case REG_COMM:		/* 通信寄存器 */		
		case REG_SETUP:		/* 设置寄存器 8bit */
		case REG_CLOCK_H:		/* 时钟寄存器 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 偏移寄存器 24bit */
		case REG_FULL_CH1:	/* CH1 满量程寄存器 24bit */
		case REG_ZERO_CH2:	/* CH2 偏移寄存器 24bit */
		case REG_FULL_CH2:	/* CH2 满量程寄存器 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* 数据寄存器 16bit */
		default:
			return;
	}

	TM7705_WriteByte(_RegID | WRITE);	/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */

	if (bits == 8)
	{
		TM7705_WriteByte((uint8_t)_RegValue);
	}
	else	/* 24bit */
	{
		TM7705_Write3Byte(_RegValue);
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ReadReg
*	功能说明: 写指定的寄存器
*	形    参:  _RegID : 寄存器ID
*			  _RegValue : 寄存器值。 对于8位的寄存器，取32位形参的低8bit
*	返 回 值: 读到的寄存器值。 对于8位的寄存器，取32位形参的低8bit
*********************************************************************************************************
*/
uint32_t TM7705_ReadReg(uint8_t _RegID)
{
	uint8_t bits;
	uint32_t read;

	switch (_RegID)
	{
		case REG_COMM:		/* 通信寄存器 */
		case REG_SETUP:		/* 设置寄存器 8bit */
		case REG_CLOCK_H:		/* 时钟寄存器 8bit */
			bits = 8;
			break;

		case REG_ZERO_CH1:	/* CH1 偏移寄存器 24bit */
		case REG_FULL_CH1:	/* CH1 满量程寄存器 24bit */
		case REG_ZERO_CH2:	/* CH2 偏移寄存器 24bit */
		case REG_FULL_CH2:	/* CH2 满量程寄存器 24bit*/
			bits = 24;
			break;

		case REG_DATA:		/* 数据寄存器 16bit */
		default:
			return 0xFFFFFFFF;
	}

	TM7705_WriteByte(_RegID | READ);	/* 写通信寄存器, 指定下一步是写操作，并指定写哪个寄存器 */

	if (bits == 16)
	{
		read = TM7705_Read2Byte();
	}
	else if (bits == 8)
	{
		read = TM7705_ReadByte();
	}
	else	/* 24bit */
	{
		read = TM7705_Read3Byte();
	}
	return read;
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_CalibSelf
*	功能说明: 启动自校准. 内部自动短接AIN+ AIN-校准0位，内部短接到Vref 校准满位。此函数执行过程较长，
*			  实测约 180ms
*	形    参:  _ch : ADC通道，1或2
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_CalibSelf(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* 自校准CH1 */
	TM7705_WriteByte(0x10);    TM7705_WriteByte(0x20);//ain1
	TM7705_WriteByte(0x20);    TM7705_WriteByte(0x2f);//ain1
	TM7705_WriteByte(0x50);    TM7705_WriteByte(0x10);//ain1
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */		
		TM7705_WriteByte(MD_CAL_SELF | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* 启动自校准 */

	}
	else if (_ch == 2)
	{
		/* 自校准CH2 */
	TM7705_WriteByte(0x10);    TM7705_WriteByte(0x20);//ain1
	TM7705_WriteByte(0x20);    TM7705_WriteByte(0x2f);//ain1//
	TM7705_WriteByte(0x50);    TM7705_WriteByte(0x10);//ain1
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道2 */
		TM7705_WriteByte(MD_CAL_SELF | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* 启动自校准 */


		
		//		TM7705_WaitDRDY();	/* 等待内部操作完成  --- 时间较长，约180ms */
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_SytemCalibZero
*	功能说明: 启动系统校准零位. 请将AIN+ AIN-短接后，执行该函数。校准应该由主程序控制并保存校准参数。
*			 执行完毕后。可以通过 TM7705_ReadReg(REG_ZERO_CH1) 和  TM7705_ReadReg(REG_ZERO_CH2) 读取校准参数。
*	形    参: _ch : ADC通道，1或2
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_SytemCalibZero(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* 校准CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
	else if (_ch == 2)
	{
		/* 校准CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_ZERO | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* 启动自校准 */
		TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_SytemCalibFull
*	功能说明: 启动系统校准满位. 请将AIN+ AIN-接最大输入电压源，执行该函数。校准应该由主程序控制并保存校准参数。
*			 执行完毕后。可以通过 TM7705_ReadReg(REG_FULL_CH1) 和  TM7705_ReadReg(REG_FULL_CH2) 读取校准参数。
*	形    参:  _ch : ADC通道，1或2
*	返 回 值: 无
*********************************************************************************************************
*/
void TM7705_SytemCalibFull(uint8_t _ch)
{
	if (_ch == 1)
	{
		/* 校准CH1 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_1);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH1_GAIN_BIPOLAR_BUF | FSYNC_0);/* 启动自校准 */
	//	TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
	else if (_ch == 2)
	{
		/* 校准CH2 */
		TM7705_WriteByte(REG_SETUP | WRITE | CH_2);	/* 写通信寄存器，下一步是写设置寄存器，通道1 */
		TM7705_WriteByte(MD_CAL_FULL | __CH2_GAIN_BIPOLAR_BUF | FSYNC_0);	/* 启动自校准 */
	//	TM7705_WaitDRDY();	/* 等待内部操作完成 */
	}
}

/*
*********************************************************************************************************
*	函 数 名: TM7705_ReadAdc1
*	功能说明: 读通道1或2的ADC数据
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
uint32_t TM7705_ReadAdc(uint8_t _ch)
{
	uint8_t i;
	uint32_t read = 0;
	
	/* 为了避免通道切换造成读数失效，读2次 */

	for (i = 0; i < 1; i++)
	{
		TM7705_WaitDRDY();		/* 等待DRDY口线为0 */		

		if (_ch == 1)
		{
			TM7705_WriteByte(0x38);
		}
		else if (_ch == 2)
		{
			TM7705_WriteByte(0x39);
		}

		read = TM7705_Read3Byte();
	}
	return read;	
}

void _7707_REST(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	delay_ms(500);
	GPIO_SetBits (GPIOB,GPIO_Pin_12);
	delay_ms(500);
    TM=1;
	 TM7705_CalibSelf(1);
	TM=0;
	 TM7705_CalibSelf(2);
}	

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/

