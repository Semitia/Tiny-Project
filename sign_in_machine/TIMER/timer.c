#include "timer.h"

clock_t p_clock,fix_clock,reset_clock;//从开始计时的时钟；修正时刻；重置的时刻；
bool count_flag=0;										//wait()计时标志
u8 count_ack=0;												//计时回馈

/**
	*@brief 更新时间
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
	*@brief 时钟初始化
	*@param 时间
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
	*@brief 获取两个时间点之间的时间间隔（单位秒）
	*@param 两个时间点的时间
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
	*@brief 获取当前时间，TIM3->CNT可精确到0.0001秒
**/
clock_t get_time()
{
	u16 count = TIM3->CNT;
	p_clock.CNT = count;
	return p_clock;
}

/**
	*@brief 判断字符
	*@param 字符
**/
static u8 get_value(u8 x)
{
	if( x>='0' && x<='9') return (x-'0');
	else return 10;
}

/**
	*@brief  重置时间
	*@param1 字符串
	*@param2 长度
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
	*@brief 延时
	*@param 秒
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
	*@brief 定时器TIM3和p_clock的初始化函数
	*@param 自动重装载值 ，预分频值
**/
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 计数到5000为500ms
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  10Khz的计数频率  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(  //使能或者失能指定的TIM中断
		TIM3, //TIM2
		TIM_IT_Update ,
		ENABLE  //使能
		);
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx外设
		
	p_clock = newclock(0,0,0,0);
}

/**
	*@brief 定时器中断函数，每秒触发一次，更新p_clock
	*@param void
**/
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //检查指定的TIM中断发生与否:TIM 中断源 
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx的中断待处理位:TIM 中断源 
			//update
			time_update();
			LED0 = !LED0;
			if(count_flag) {count_ack++;}
		}
}

