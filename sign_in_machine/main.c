#include "sys.h"

/**
	*@brief  �򿨼�¼����
	*@param1 ʱ��
	*@param2 ��ʱ��
	*@param3 ָ����һ����¼
**/
typedef struct __record_t{
	u8 number;
	clock_t record_time;
	
	struct __record *next;
}record_t;

/**
	*@brief  ��������
	*@param1 ����
	*@param2 ���루ͨ��֤��
	*@param3 �򿨼�¼
	*@param4 ָ����һ����
**/
typedef struct __person_t{
	u8 *name;
	u8 *password;
	clock_t sign_up_time;
	record_t *own_record;
	
	struct __person_t *next;
}person_t;


extern u8 WK_UP_flag;  //0:û�а� 1:���� 
extern float interval; //���ΰ������
u8 lenth;//���ݳ���
person_t *person_link; //��������

/**
	*@brief �ȴ�����
**/
void wait_for_input(void)
{
	while(!(USART_RX_STA&0x8000));//û�н��յ����ݾ�һֱ�ȴ�	
	lenth=USART_RX_STA&0x3fff;//�õ��˴ν��յ������ݳ���
	return;
}

/**
	*@brief  ����ע��
	*@param1 �ַ���
	*@param2 ����
**/
void sign_up(u8 *strings, u8 len)
{
	person_t *p;
	u8 *tem_name,*tem_password;
	u8 i=0,j=0;
	tem_name = (u8*)malloc(10);
	tem_password = (u8*)malloc(20);
	
	//�����ַ���
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
	
	//��ʼ��������Ϣ
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
	*@brief  �Ǽ�
	*@param1 �ַ���
	*@param2 ����
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
	*@brief  ��ѯ�򿨼�¼ 
	*@param1 �ַ���
	*@param2 ����
**/
void inquire(u8 *strings, u8 len)
{
	
}

/**
	*@brief  ���������ʼ��
**/
person_t *person_init(void)
{
	person_t *p;
	p = (person_t *)malloc(sizeof(person_t));
	p->name = NULL;
	p->next = NULL;
	p->own_record = NULL;
	p->password = NULL;
	p->sign_up_time = get_time();//ʵ����Ϊ0Ҳ����
	return p;
}

 int main(void)
 {	
	delay_init();	    															  //��ʱ������ʼ��	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		// �����ж����ȼ�����2
	uart_init(9600);	 																//���ڳ�ʼ��Ϊ9600
	LED_Init();		  	 																//��ʼ����LED���ӵ�Ӳ���ӿ� 
 	EXTIX_Init();																			//�ⲿ�жϳ�ʼ��
	TIM3_Int_Init(10000,7200-1); 											//1s
	person_link = person_init();											//���������ʼ��
	
	
	while(1)
	{	
		printf("Please press and hold to initialize\r\n");
		while(!WK_UP_flag) {}														//�ȴ�������ʼ��
		if(interval > 3)																//�ǳ���
		{
			printf("Please reset the time of system\r\n");
			wait_for_input();
			reset_time(USART_RX_BUF,lenth);								//����ʱ��
			USART_RX_STA=0;
			WK_UP_flag=0;
			break;
		}
		WK_UP_flag=0;																		//�����־
	}
	
	while(1)
	{
		printf("Please press and hold to sign up\r\nPlease press once to sign in\r\nPlease press twice to inquire your record\r\n");
		while(!WK_UP_flag) {}
		if(interval > 3)																//����
		{
			printf("sign up\r\n");
			printf("user: , passport: \r\n");
			wait_for_input();
			sign_up(USART_RX_BUF,lenth);									//ע��
			USART_RX_STA=0;
			WK_UP_flag=0;
		}
		else
		{
			//delay_ms(3000);
			wait(1);																			//��ʱ1s
			if(WK_UP_flag == 1) 													//��һ��
			{
				printf("sign in\r\n");
				printf("user: \r\n");
				wait_for_input();
				sign_in(USART_RX_BUF,lenth);								//��¼
				USART_RX_STA=0;
				WK_UP_flag=0;
				
			}
			else if(WK_UP_flag == 2)											//������
			{
				printf("inquire\r\n");
				printf("user: , passport: \r\n");
				wait_for_input();
				inquire(USART_RX_BUF,lenth);								//��ѯ��¼
				USART_RX_STA=0;
				WK_UP_flag=0;
				
			}
		}
		
	}
	
}


