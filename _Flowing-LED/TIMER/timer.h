#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

typedef struct __clock_t{
	u8 second;
	u8 minute;
	u8 hour;
	void (*p_update)(void);
}clock_t;

void TIM3_Int_Init(u16 arr,u16 psc); 
 

#endif

