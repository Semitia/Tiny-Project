#include "sys.h"

/**
	*@brief  打卡记录链表
	*@param1 时序
	*@param2 打卡时间
	*@param3 指向下一条记录
**/
typedef struct __record_t{
	u8 number;
	clock_t record_time;
	
	struct __record *next;
}record_t;

/**
	*@brief  人物链表
	*@param1 名字
	*@param2 密码（通行证）
	*@param3 打卡记录
	*@param4 指向下一个人
**/
typedef struct __person_t{
	u8 *name;
	u8 *password;
	clock_t sign_up_time;
	record_t *own_record;
	
	struct __person_t *next;
}person_t;


extern u8 WK_UP_flag;  //0:没有按 1:按了 
extern float interval; //两次按键间隔
u8 lenth;//数据长度
person_t *person_link; //人物链表

/**
	*@brief 等待输入
**/
void wait_for_input(void)
{
	while(!(USART_RX_STA&0x8000));//没有接收到数据就一直等待	
	lenth=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
	return;
}

/**
	*@brief  人物注册
	*@param1 字符串
	*@param2 长度
**/
void sign_up(u8 *strings, u8 len)
{
	person_t *p;
	u8 *tem_name,*tem_password;
	u8 i=0,j=0;
	tem_name = (u8*)malloc(10);
	tem_password = (u8*)malloc(20);
	
	//解析字符串
	while(strings[i] != ' ')
	{
		tem_name[i] = strings[i];
		i++;
	}
	i++;
	while(i<len)
	{
		tem_password[j] = strings[i];
		i++;
		j++;
	}
	
	//初始化人物信息
	p = (person_t *)malloc(sizeof(person_t));
	p->name = tem_name;
	p->password = tem_password;
	p->next = person_link->next;
	person_link->next = p;
	p->sign_up_time = get_time();
	free(tem_name),free(tem_password);
	return;
}

/**
	*@brief  登记
	*@param1 字符串
	*@param2 长度
**/
void sign_in(u8 *strings, u8 len)
{
	u8 *tem_name;
	u8 i=0;
	tem_name = (u8*)malloc(10);
	while(i<len)
	{
		tem_name[i] = strings[i];
		i++;
	}
	
}

/**
	*@brief  查询打卡记录 
	*@param1 字符串
	*@param2 长度
**/
void inquire(u8 *strings, u8 len)
{
	
}

/**
	*@brief  人物链表初始化
**/
person_t *person_init(void)
{
	person_t *p;
	p = (person_t *)malloc(sizeof(person_t));
	p->name = NULL;
	p->next = NULL;
	p->own_record = NULL;
	p->password = NULL;
	p->sign_up_time = get_time();//实际上为0也可以
	return p;
}

 int main(void)
 {	
	delay_init();	    															  //延时函数初始化	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// 设置中断优先级分组2
	uart_init(9600);	 																//串口初始化为9600
	LED_Init();		  	 																//初始化与LED连接的硬件接口 
 	EXTIX_Init();																			//外部中断初始化
	TIM3_Int_Init(10000,7200-1); 											//1s
	person_link = person_init();											//人物链表初始化
	
	
	while(1)
	{	
		printf("Please press and hold to initialize\r\n");
		while(!WK_UP_flag) {}														//等待长按初始化
		if(interval > 3)																//是长按
		{
			printf("Please reset the time of system\r\n");
			wait_for_input();
			reset_time(USART_RX_BUF,lenth);								//重置时间
			USART_RX_STA=0;
			WK_UP_flag=0;
			break;
		}
		WK_UP_flag=0;																		//清除标志
	}
	
	while(1)
	{
		printf("Please press and hold to sign up\r\nPlease press once to sign in\r\nPlease press twice to inquire your record\r\n");
		while(!WK_UP_flag) {}
		if(interval > 3)																//长按
		{
			printf("sign up\r\n");
			printf("user: , passport: \r\n");
			wait_for_input();
			sign_up(USART_RX_BUF,lenth);									//注册
			USART_RX_STA=0;
			WK_UP_flag=0;
		}
		else
		{
			//delay_ms(3000);
			wait(1);																			//延时1s
			if(WK_UP_flag == 1) 													//按一次
			{
				printf("sign in\r\n");
				printf("user: \r\n");
				wait_for_input();
				sign_in(USART_RX_BUF,lenth);								//记录
				USART_RX_STA=0;
				WK_UP_flag=0;
				
			}
			else if(WK_UP_flag == 2)											//按两次
			{
				printf("inquire\r\n");
				printf("user: , passport: \r\n");
				wait_for_input();
				inquire(USART_RX_BUF,lenth);								//查询记录
				USART_RX_STA=0;
				WK_UP_flag=0;
				
			}
		}
		
	}
	
}


