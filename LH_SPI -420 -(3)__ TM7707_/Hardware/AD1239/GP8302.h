#ifndef __GP8302_H__
#define __GP8302_H__
#include "delay.h"

#define SDA_8302  PBout(3)
#define SCL_8302  PBout(4)
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
void GP8302(int32_t ReadAddr);




#endif
