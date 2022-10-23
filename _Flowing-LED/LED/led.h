#ifndef __LED_H
#define __LED_H	 
#include "sys.h"

#define LED0 PAout(8)	// PA8
#define LED1 PDout(2)	// PD2	
#define MAX_RANGE 1000
#define MIN_RANGE 0

typedef struct __LED_t{
	bool direction;//1:+  ;  0:-
	int16_t luminance;//���ȣ���Ӧ�����PWMֵ	
	u8 change_speed;//�仯�ٶ�
	u16 max_lum;//�������
	u16 min_lum;//��С����
	void (*p_set_luminance)(int16_t luminance);//
}LED_t;

void LED_Init(void);//��ʼ��
void blink(u8 mode);
								 
#endif
