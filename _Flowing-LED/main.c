#include "sys.h"

extern u8 key_mode;
extern bool system_mode;

 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	uart_init(9600);	 //���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 //��ʼ����LED���ӵ�Ӳ���ӿ� 
 	EXTIX_Init();		//�ⲿ�жϳ�ʼ��
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


