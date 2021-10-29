#include "oled.h"
#include "gui.h"
#include "pic.h"
 extern int8_t Gd,Gd_y;


void CAN_SET(void)
{
	u8 X=0;
	OLED_Clear(0); 
	do
	{
		GUI_ShowString(X,10,"*",1,16,1);
		X+=8;
	}
	while(X<128);
	GUI_ShowString(0,30,"*",1,16,1);
	GUI_ShowCHinese(16,30,16,"正在设置参数",1);
	GUI_ShowString(120,30,"*",1,16,1);
	X=0;
	do
	{
		GUI_ShowString(X,50,"*",1,16,1);
		X+=8;
	}
	while(X<128);
}	
void ONE(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(16,0,16,"向修正参数：",1);
	GUI_ShowCHinese(3,16,16,"零点：",1);
	GUI_ShowCHinese(3,32,16,"切除：",1);
	GUI_ShowString(96,16,"KPa",3,16,1);
//	GUI_ShowString(105,19,"3",1,8,1);
	GUI_ShowString(96,32,"m /h",4,16,1);
	GUI_ShowString(105,35,"3",1,8,1);
	GUI_ShowString(2,48,"<05%:",5,16,1);
	
}	
void TWO(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(16,0,16,"向修正参数：",1);
	GUI_ShowString(4,16,"05%~10%:",8,16,1);
	GUI_ShowString(4,32,"10%~15%:",8,16,1);
	GUI_ShowString(4,48,"15%~20%:",8,16,1);
	
}
void THERR(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(16,0,16,"向修正参数：",1);
	GUI_ShowString(4,16,"20%~30%:",8,16,1);
	GUI_ShowString(4,32,"30%~40%:",8,16,1);
	GUI_ShowString(4,48,"40%~50%:",8,16,1);
	
}
void FOUR(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(16,0,16,"向修正参数：",1);
	GUI_ShowString(4,16,"50%~60%:",8,16,1);
	GUI_ShowString(4,32,"60%~70%:",8,16,1);
	GUI_ShowString(4,48,"70%~80%:",8,16,1);
	
}
void MENU(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(10,0,16,"设置正向参数",1);
	GUI_ShowCHinese(10,16,16,"设置反向参数",1);
	GUI_ShowCHinese(10,32,16,"设置修正参数",1);
	GUI_ShowCHinese(10,48,16,"设置通讯地址",1);
	GUI_ShowChar(113,4,32,8,0);
}
void FIVE(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(21,16,16,"向流量量程：",1);
	GUI_ShowString(88,38,"m /h",4,16,1);
	GUI_ShowString(97,41,"3",1,8,1);
	
}
void SIX(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(0,0,16,"零点温度：",1);
	GUI_ShowCHinese(0,16,16,"温度系数：",1);
	GUI_ShowCHinese(16,32,16,"度以上：",1);
	GUI_ShowCHinese(16,48,16,"度以下：",1);
	GUI_ShowString(0,32,"25",2,16,1);
	//GUI_ShowString(32,32,"C",1,16,1);
	GUI_ShowString(0,48,"25",2,16,1);
	//GUI_ShowString(32,48,"C",1,16,1);
}
void SEV(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(21,15,16,"通讯地址：",1);
}
void QJHM(void)
{	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(0,0,16,"倾角",1);
	GUI_ShowCHinese(24,32,16,"度：",1);
	GUI_ShowString(0,32,"+30",3,16,1);
	GUI_ShowString(0,48,"-30",3,16,1);
	GUI_ShowCHinese(24,48,16,"度：",1);
}
void FIRST(void)
{
#if XS	
	OLED_Clear(0);             //清屏（全黑）
	GUI_ShowCHinese(0,0,16,"流量",1);
	GUI_ShowCHinese(0,16,16,"累计",1);
	GUI_ShowCHinese(0,32,16,"流速",1);
	GUI_ShowCHinese(0,48,16,"差压",1);
	GUI_ShowString(100,0,"m/h",3,16,1);
	GUI_ShowString(108,3,"3",1,8,1);
	GUI_ShowString(112,19,"m",1,16,1);
	GUI_ShowString(120,19,"3",1,8,1);
	GUI_ShowString(104,32,"m/s",3,16,1);
	GUI_ShowString(104,48,"KPa",3,16,1);
	
#else
		OLED_Clear(0);             //清屏（全黑）
//	GUI_DrawLine(0,0,WIDTH-1,0,1);
//	GUI_DrawLine(0,22,WIDTH-1,22,1);
//	GUI_DrawLine(0,42,WIDTH-1,42,1);
//	GUI_DrawLine(1,63,WIDTH-1,63,1);
	
	GUI_ShowCHinese(0,3,16,"流量",1);
	GUI_ShowCHinese(0,24,16,"累计",1);
	GUI_ShowCHinese(0,45,16,"差压",1);
	GUI_ShowString(100,3,"m/h",3,16,1);
	GUI_ShowString(108,3,"3",1,8,1);
	GUI_ShowString(112,24,"m",1,16,1);
	GUI_ShowString(120,24,"3",1,8,1);
	GUI_ShowString(104,45,"KPa",3,16,1);
						
#endif	
}
void FIRST_2(void)
{	
	u8 i,a;
	int8_t gd;
		OLED_Clear(0);             //??(??)
if(Gd_y<30)
{
GUI_DrawRectangle(0, 5, WIDTH-1, HEIGHT-1-5,1);
GUI_DrawLine(WIDTH/2-1,5,WIDTH/2-1,HEIGHT-6,1);
GUI_DrawLine(0,HEIGHT/2,WIDTH-1,HEIGHT/2,1);	
	GUI_ShowCHinese(16,0,16,"温度",1);
	GUI_ShowCHinese(78,0,16,"温差",1);
	GUI_ShowCHinese(16,48,16,"角度",1);
	GUI_ShowCHinese(78,48,16,"角差",1);
	GUI_ShowString(46,14,"o",1,8,1);
	GUI_ShowString(52,16,"C",1,16,1);
	GUI_ShowString(107,16,"Pa",2,16,1);
}
else
{   
	GUI_DrawRectangle(0, 0, WIDTH-1, HEIGHT-1,1);
//	GUI_DrawLine(WIDTH/2-1,5,WIDTH/2-1,HEIGHT-6,1);
//	for(a=1;a<7;a++)	GUI_DrawLine(0,a*i,WIDTH-1,a*i,1);
	gd=Gd;
	
	if(gd<6 && gd>=0)GUI_ShowString(2,3+10*gd,  "Temperature:",12,8,1);	else ;
	if(gd<6 && gd>=0)GUI_ShowString(110,3+10*gd++,"$",1,8,1);			else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(8,3+10*gd, "Correction:",11,8,1);	else ;
    if(gd<6 && gd>=0)GUI_ShowString(110,3+10*gd++,"Pa",2,8,1);			else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(20,3+10*gd++,"Sampling:",9,8,1);	    else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(8,3+10*gd++,"Resistance:",11,8,1);	else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(26,3+10*gd++,"[]Angle:",8,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(8,3+10*gd++,"EEPROM Bit:",11,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(8,3+10*gd++, "EEPROM Num:",11,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(38,3+10*gd++,"Error:",6,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(14,3+10*gd++,"ESR B,E,W:",10,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(26,3+10*gd++,"ESR LEC:",8,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(26,3+10*gd++,"ESR TEC:",8,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(26,3+10*gd++,"ESR REC:",8,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(26,3+10*gd++,"MCR 0-7:",8,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(20,3+10*gd++,"RF0R FMP:",9,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(32,3+10*gd++,"F_SIZE:",7,8,1);		else gd++;
	if(gd<6 && gd>=0)GUI_ShowString(44,3+10*gd++,"F_ID:",5,8,1);		else gd++;
}	

}

