#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	
#define MAX_RANGE 1000
#define MIN_RANGE 0

typedef struct __LED_t{
	bool direction;//1:+  ;  0:-
	int16_t luminance;//亮度，对应输出的PWM值	
	u8 change_speed;//变化速度
	u16 max_lum;//最大亮度
	u16 min_lum;//最小亮度
	void (*p_set_luminance)(int16_t luminance);//
}LED_t;

void LED_Init(void);//初始化
void blink(u8 mode);
								 
#endif
