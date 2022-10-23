#include "sys.h"

extern u8 key_mode;
extern bool system_mode;

 int main(void)
 {	
	delay_init();	    	 //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	uart_init(9600);	 //串口初始化为9600
	LED_Init();		  	 //初始化与LED连接的硬件接口 
 	EXTIX_Init();		//外部中断初始化
	TIM4_CH1_PWM_Init(1800-1,100-1);

	while(1)
	{	    
		static u16 i=0;
		if(!system_mode) 
		{
			u8 time_mode;
			i++;
			i %= 900;
			time_mode = i/300;
			blink(time_mode);
		}
		else
		{blink(key_mode);}
	} 
}


