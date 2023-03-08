#include "led.h"
#include "delay.h"
#include "usart.h"
#include "control.h"
#include "sys.h"
#include "pwm.h"
#include "encoder.h"

 int main(void)
 {	
	u16 led0pwmval=0,cnt=0;    
	u8 dir=1;	
	delay_init();	    	 //延时函数初始化	  
	uart_init(115200);
	LED_Init();		  	//初始化与LED连接的硬件接口
	TIM1_PWM_Init(899,0);//不分频。PWM频率=72000/(899+1)=80Khz 
	TIM8_PWM_Init(899,0);
	Encoder_Init_TIM2();
	Encoder_Init_TIM3();
	Encoder_Init_TIM4();
	Encoder_Init_TIM5();	 
	 
  while(1)
	{
 		delay_ms(10);	 
		cnt++;
		if(dir)led0pwmval++;
		else led0pwmval--;	 
 		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	   					 
		TIM_SetCompare1(TIM1,led0pwmval);	  
		/*
		TIM_SetCompare1(TIM8,led0pwmval);	  
		TIM_SetCompare2(TIM8,led0pwmval);	  
		TIM_SetCompare3(TIM8,led0pwmval);	  
		TIM_SetCompare4(TIM8,led0pwmval);	  
		*/
		data_receive();
		Move_Ctrl();
		//send_msg();
		if(cnt>=50)
		{
			cnt=0;
			state_info();
		}
	} 
}

