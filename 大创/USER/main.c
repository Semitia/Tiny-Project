#include "led.h"
#include "delay.h"
#include "usart.h"
#include "control.h"
#include "sys.h"
#include "pwm.h"

 int main(void)
 {	
	u16 led0pwmval=0;    
	u8 dir=1;	
	delay_init();	    	 //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM1_PWM_Init(899,0);//����Ƶ��PWMƵ��=72000/(899+1)=80Khz 
	TIM2_PWM_Init(899,0);
   	while(1)
	{
 		delay_ms(10);	 
		if(dir)led0pwmval++;
		else led0pwmval--;	 
 		if(led0pwmval>300)dir=0;
		if(led0pwmval==0)dir=1;	   					 
		TIM_SetCompare1(TIM1,led0pwmval);	  
		
		TIM_SetCompare1(TIM2,led0pwmval);	  
		TIM_SetCompare2(TIM2,led0pwmval);	  
		TIM_SetCompare3(TIM2,led0pwmval);	  
		TIM_SetCompare4(TIM2,led0pwmval);	  
		
		data_receive();
		Move_Ctrl();
		
		
	} 
}

