#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

/**
	*@brief  钟表结构体
	*@param1 直接读取TIM->CNT,精度0.0001s
	*@param2 秒
	*@param3 分钟
	*@param4 小时
	*@param5 更新函数
**/
typedef struct __clock_t{
	u16 CNT;
	u8 second;
	u8 minute;
	u8 hour;
	void (*p_update)(void);
}clock_t;

void TIM3_Int_Init(u16 arr,u16 psc); 
float get_interval(clock_t clock1, clock_t clock2);
clock_t get_time(void);
void reset_time(u8 *string_time,u8 len);
void wait(u8 s);
#endif

