#include "pwm.h"

void TIM4_CH1_PWM_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStruct;
	TIM_OCInitTypeDef TIM_OCInitTypeStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	//初始化TIM4
	TIM_TimeBaseStruct.TIM_Period = arr;//重装载值 
	TIM_TimeBaseStruct.TIM_Prescaler = psc;//预分频值 
	TIM_TimeBaseStruct.TIM_ClockDivision = 0; //时钟分频1、2、4分频	
	TIM_TimeBaseStruct.TIM_CounterMode = TIM_CounterMode_Up;//设置计数模式
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStruct);
	
	//初始化输出比较参数
	TIM_OCInitTypeStruct.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式
	TIM_OCInitTypeStruct.TIM_OutputState = TIM_OutputState_Enable;//比较输出使能
	TIM_OCInitTypeStruct.TIM_OCPolarity = TIM_OCPolarity_High;//输出极性
	TIM_OC1Init(TIM4,&TIM_OCInitTypeStruct); //根据TIMX的参数设定初始化外设 TIMX OC2,TIMX OC3....
	TIM_OC2Init(TIM4,&TIM_OCInitTypeStruct);
	TIM_OC3Init(TIM4,&TIM_OCInitTypeStruct);
	TIM_OC4Init(TIM4,&TIM_OCInitTypeStruct);
	
	//使能预装载寄存器
	TIM_OC1PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);
	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);
	
	//使能定时器
	TIM_Cmd(TIM4,ENABLE);
	
}

