#include "delay.h"
#include "sys.h"
#include "oled.h"
#include "bmp.h"
#include "led.h"
#include "timer.h"

extern u32 LED_state;
char str1[] = "LED State: ON";
char str2[] = "LED_State: OFF";
char str3[] = "Counts: ";


int main(void)
{
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	LED_Init();		  	 //初始化与LED连接的硬件接口 
	TIM3_Int_Init(5000-1,7200-1); 
	OLED_Init();
	OLED_ColorTurn(0);//0正常显示，1 反色显示
  OLED_DisplayTurn(0);//0正常显示 1 屏幕翻转显示
	LED_ON;
	while(1)
	{
		//OLED_Clear();
		OLED_ShowString(15,10,str3,8,1);
		OLED_ShowNum(15,20,LED_state,4,8,1);
		
		if(LED_state & 0x00000001) 
		{
			OLED_ShowString(15,30,"LED State: ON ",8,1);
		}
		else 
		{
			OLED_ShowString(15,30,"LED State: OFF",8,1);
		}
		
		OLED_Refresh();
		//delay_ms(500);
	}
	return 0;
}

