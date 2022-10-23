#include "timer.h"

clock_t p_clock,fix_clock,reset_clock;//�ӿ�ʼ��ʱ��ʱ�ӣ�����ʱ�̣����õ�ʱ�̣�
bool count_flag=0;										//wait()��ʱ��־
u8 count_ack=0;												//��ʱ����

/**
	*@brief ����ʱ��
	*@param void
**/
static void time_update(void)
{
	p_clock.second++;
	if(p_clock.second>=60) {p_clock.minute++;p_clock.second-=60;}
	if(p_clock.minute>=60) {p_clock.hour++;p_clock.minute-=60;}
	printf("minute:%d second:%d\r\n",p_clock.minute,p_clock.second);
	return;
}

/**
	*@brief ʱ�ӳ�ʼ��
	*@param ʱ��
**/
static clock_t newclock(u8 hour,u8 minute,u8 second,u16 CNT)
{
	clock_t new_clock;
	new_clock.hour=hour;
	new_clock.minute=minute;
	new_clock.second=second;
	new_clock.CNT=CNT;
	new_clock.p_update = time_update;
	
	return new_clock;
}

/**
	*@brief ��ȡ����ʱ���֮���ʱ��������λ�룩
	*@param ����ʱ����ʱ��
**/
float get_interval(clock_t clock1, clock_t clock2)
{
	int8_t delta_hour,delta_minute,delta_second;
	int16_t delta_CNT;
	float delta_time;
	delta_hour = clock2.hour - clock1.hour;
	delta_minute = clock2.minute - clock1.minute;
	delta_second = clock2.second - clock1.second;
	delta_CNT = clock2.CNT - clock1.CNT;
	//printf("clock_1:%d %d %d\r\n",clock1.hour,clock1.minute,clock1.second);
	//printf("clock_2:%d %d %d\r\n",clock2.hour,clock2.minute,clock2.second);
	//printf("h:%d min:%d s:%d CNT:%d\r\n",delta_hour,delta_minute,delta_second,delta_CNT);
	delta_time = (float)3600*delta_hour+60*delta_minute+delta_second+0.0001*delta_CNT;
	return delta_time;
}

/**
	*@brief ��ȡ��ǰʱ�䣬TIM3->CNT�ɾ�ȷ��0.0001��
**/
clock_t get_time()
{
	u16 count = TIM3->CNT;
	p_clock.CNT = count;
	return p_clock;
}

/**
	*@brief �ж��ַ�
	*@param �ַ�
**/
static u8 get_value(u8 x)
{
	if( x>='0' && x<='9') return (x-'0');
	else return 10;
}

/**
	*@brief  ����ʱ��
	*@param1 �ַ���
	*@param2 ����
**/
void reset_time(u8 *string_time,u8 len)
{
	//printf("%d %s",len,string_time);
	u8 i,update_flag=0;
	u8 tem_hour=0,tem_minute=0,tem_second=0;
	for(i=0;i<len;i++)
	{
		u8 tem_value = get_value(string_time[i]);
		if(tem_value == 10) {update_flag++;continue;}
		else 
		{
			switch(update_flag)
			{
				case 0:
					tem_hour = tem_hour*10 + tem_value;
					break;
				case 1:
					tem_minute = tem_minute*10 + tem_value;
					break;
				case 2:
					tem_second = tem_second*10 + tem_value;
					break;
				default:
					printf("wrong!\r\n");
					break;
			}
		}
	}
	fix_clock = newclock(tem_hour,tem_minute,tem_second,0);
	reset_clock = get_time();
	return;
}

/**
	*@brief ��ʱ
	*@param ��
**/
void wait(u8 s)
{
	u16 initial_cnt = TIM3->CNT;
	count_flag=1;
	while(count_ack<s || TIM3->CNT<initial_cnt) {}
	count_flag = 0;
	count_ack = 0;
	return;
}


/**
	*@brief ��ʱ��TIM3��p_clock�ĳ�ʼ������
	*@param �Զ���װ��ֵ ��Ԥ��Ƶֵ
**/
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 ������5000Ϊ500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(  //ʹ�ܻ���ʧ��ָ����TIM�ж�
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //ʹ��
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx����
		
	p_clock = newclock(0,0,0,0);
}

/**
	*@brief ��ʱ���жϺ�����ÿ�봥��һ�Σ�����p_clock
	*@param void
**/
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx���жϴ�����λ:TIM �ж�Դ 
			//update
			time_update();
			LED0 = !LED0;
			if(count_flag) {count_ack++;}
		}
}

