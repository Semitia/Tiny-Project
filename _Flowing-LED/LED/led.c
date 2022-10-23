#include "led.h"
//�ĸ�LED
LED_t *p_led0,*p_led1,*p_led2,*p_led3;

/**
	*@brief ÿ��LED��ʼ������
	*@param �������ȣ��ٶȣ����ȷ�Χ
**/
static LED_t *new_LED(bool dir,int16_t lum,u8 speed,u16 mx_lum,u16 mn_lum)
{	
	LED_t *p_newLED = NULL;
	p_newLED=(LED_t*)malloc(sizeof(LED_t));
	if(p_newLED==NULL) {return NULL;}
	p_newLED->direction = dir;
	p_newLED->luminance = lum;
	p_newLED->change_speed = speed;
	p_newLED->max_lum = mx_lum;
	p_newLED->min_lum = mn_lum;
	return p_newLED;
}

/**
	*@brief �޷�������ʹ��MAX_RANGE��MIN_RANGE���Ǹ��Ե�max_luminance��min_luminance
	*@param ������initial luminance
**/
static u16 limit(int16_t initial_data)
{
	u16 data;
	data = initial_data>MAX_RANGE?MAX_RANGE:initial_data;
	data = initial_data<MIN_RANGE?MIN_RANGE:initial_data;
	return data;
}

/**
	*@brief ���ø���LED���ȣ���ӦTIM4��4��ͨ��
	*@param ����
**/
static void set_luminance_LED0(int16_t pwm)
{
	TIM_SetCompare1(TIM4,limit(pwm) );
}
static void set_luminance_LED1(int16_t pwm)
{
	TIM_SetCompare2(TIM4,limit(pwm) );
}
static void set_luminance_LED2(int16_t pwm)
{
	TIM_SetCompare3(TIM4,limit(pwm) );
}
static void set_luminance_LED3(int16_t pwm)
{
	TIM_SetCompare4(TIM4,limit(pwm) );
}

/**
	*@brief �仯�ٶȳ�ʼ��
	*@param ���ٶ�ֵ
**/
static void speed_init(u8 speed0,u8 speed1,u8 speed2,u8 speed3)
{
	p_led0->change_speed=speed0;
	p_led1->change_speed=speed1;
	p_led2->change_speed=speed2;
	p_led3->change_speed=speed3;
}

/**
	*@brief ���ȳ�ʼ��
	*@param ��������ֵ
**/
static void luminance_init(int16_t lum0,int16_t lum1,int16_t lum2,int16_t lum3)
{
	p_led0->luminance=lum0;
	p_led1->luminance=lum1;
	p_led2->luminance=lum2;
	p_led3->luminance=lum3;
}


/**
	*@brief LED��ʼ������
	*@param void
**/
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��PA,PD�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 		 //LED0-->PA.8 �˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 		 //�����趨������ʼ��GPIOA.8
	GPIO_SetBits(GPIOA,GPIO_Pin_8);											 //PA.8 �����

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	    		 	 //LED1-->PD.2 �˿�����, �������
	GPIO_Init(GPIOD, &GPIO_InitStructure);	  				 	 //������� ��IO���ٶ�Ϊ50MHz
	GPIO_SetBits(GPIOD,GPIO_Pin_2); 						 				 //PD.2 ����� 		
	
	//����LED��ʼ��
	p_led0 = new_LED(1,0,5,260,0);
	p_led0->p_set_luminance = set_luminance_LED0;
	p_led1 = new_LED(1,-100,5,260,0);	
	p_led1->p_set_luminance = set_luminance_LED1;
	p_led2 = new_LED(1,-200,5,260,0);	
	p_led2->p_set_luminance = set_luminance_LED2;
	p_led3 = new_LED(1,-300,5,260,0);
	p_led3->p_set_luminance = set_luminance_LED3;	
}

/**
	*@brief led��˸���Ƴ���
	*@param ��˸ģʽ
**/
void blink(u8 mode)
{
	static u8 last_mode = 0;
	switch (mode)
	{
		//����ģʽ
		case 0:
		{
			if(last_mode==1) {speed_init(5,5,5,5);}
			else if(last_mode==2) {speed_init(5,5,5,5);luminance_init(0,-100,-200,-300);}
			last_mode = 0;
			if(p_led0->direction) {p_led0->luminance+=p_led0->change_speed;}
			else {p_led0->luminance-=p_led0->change_speed;}
			if(p_led0->luminance>=p_led0->max_lum) {p_led0->direction=0;}
			if(p_led0->luminance<=p_led0->min_lum) {p_led0->direction=1;}
			p_led0->p_set_luminance(p_led0->luminance);
			
			if(p_led1->direction) {p_led1->luminance+=p_led1->change_speed;}
			else {p_led1->luminance-=p_led1->change_speed;}
			if(p_led1->luminance>=p_led1->max_lum) {p_led1->direction=0;}
			if(p_led1->luminance<=p_led1->min_lum) {p_led1->direction=1;}
			p_led1->p_set_luminance(p_led1->luminance);
			
			if(p_led2->direction) {p_led2->luminance+=p_led2->change_speed;}
			else {p_led2->luminance-=p_led2->change_speed;}
			if(p_led2->luminance>=p_led2->max_lum) {p_led2->direction=0;}
			if(p_led2->luminance<=p_led2->min_lum) {p_led2->direction=1;}
			p_led2->p_set_luminance(p_led2->luminance);
			
			if(p_led3->direction) {p_led3->luminance+=p_led3->change_speed;}
			else {p_led3->luminance-=p_led3->change_speed;}
			if(p_led3->luminance>=p_led3->max_lum) {p_led3->direction=0;}
			if(p_led3->luminance<=p_led3->min_lum) {p_led3->direction=1;}
			p_led3->p_set_luminance(p_led3->luminance);
			
			delay_ms(15);
			break;
		}
		//����ģʽ
		case 1:
		{
			if(last_mode==0) {speed_init(10,10,10,10);}
			else if(last_mode==2) {speed_init(10,10,10,10);luminance_init(0,-100,-200,-300);}
			last_mode = 1;
			if(p_led0->direction) {p_led0->luminance+=p_led0->change_speed;}
			else {p_led0->luminance-=p_led0->change_speed;}
			if(p_led0->luminance>=p_led0->max_lum) {p_led0->direction=0;}
			if(p_led0->luminance<=p_led0->min_lum) {p_led0->direction=1;}
			p_led0->p_set_luminance(p_led0->luminance);
			
			if(p_led1->direction) {p_led1->luminance+=p_led1->change_speed;}
			else {p_led1->luminance-=p_led1->change_speed;}
			if(p_led1->luminance>=p_led1->max_lum) {p_led1->direction=0;}
			if(p_led1->luminance<=p_led1->min_lum) {p_led1->direction=1;}
			p_led1->p_set_luminance(p_led1->luminance);
			
			if(p_led2->direction) {p_led2->luminance+=p_led2->change_speed;}
			else {p_led2->luminance-=p_led2->change_speed;}
			if(p_led2->luminance>=p_led2->max_lum) {p_led2->direction=0;}
			if(p_led2->luminance<=p_led2->min_lum) {p_led2->direction=1;}
			p_led2->p_set_luminance(p_led2->luminance);
			
			if(p_led3->direction) {p_led3->luminance+=p_led3->change_speed;}
			else {p_led3->luminance-=p_led3->change_speed;}
			if(p_led3->luminance>=p_led3->max_lum) {p_led3->direction=0;}
			if(p_led3->luminance<=p_led3->min_lum) {p_led3->direction=1;}
			p_led3->p_set_luminance(p_led3->luminance);
			
			delay_ms(15);
			break;
		}
		//����
		case 2:
		{
			last_mode = 2;
			if(p_led0->luminance>0)
			{p_led0->luminance-=p_led0->change_speed;}
			p_led0->p_set_luminance(p_led0->luminance);
			
			if(p_led1->luminance>0)
			{p_led1->luminance-=p_led1->change_speed;}
			p_led1->p_set_luminance(p_led1->luminance);
			
			if(p_led2->luminance>0)
			{p_led2->luminance-=p_led2->change_speed;}
			p_led2->p_set_luminance(p_led2->luminance);
			
			if(p_led3->luminance>0)
			{p_led3->luminance-=p_led3->change_speed;}
			p_led3->p_set_luminance(p_led3->luminance);
			delay_ms(15);
			break;
		}
	}
	return;
}


