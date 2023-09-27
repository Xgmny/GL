#ifndef __GP8302_H__
#define __GP8302_H__
#include "delay.h"

#define SDA_8302  PAout(0)
#define SCL_8302  PAout(1)

#define MAX_mA 0x7fff	
#define	mA20 26908
#define	mA4	mA20/5
#define mA16 (mA20-mA4)	
#define mA8 	mA16/2
#define uA1 mA20/20000
//≥ı ºªØIIC


void GP8302_Init(void);
void GP8802_Start(void);
void GP8802_Stop(void);
u8 GP8802_Wait_Ack(void);
void GP8802_Ack(void);
void GP8802_Send_Byte(u8 txd);
void SDA8302_OUT(void);
void SDA8302_IN(void);

u8 GP8302_Read(u16 ReadAddr);
u8 GP8312_Read(u16 ReadAddr);
void GP8302(int32_t ReadAddr);
void GP8312(int32_t ReadAddr);



#endif
