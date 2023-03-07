#include "sys.h"
#include "usart.h"	
#include "control.h"
#include "math.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif

//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	  
  
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART_RX_STA&0x8000)==0)//����δ���
			{
			if(USART_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
	
int tr(char t)
{ 
    return t-48; 
}

/**
 * @brief ���ַ�����startλ��ʼ��numλת��Ϊfloat��
 * @param s �ַ���
 * @param start ��ʼλ
 * @param num λ��
 * @param p �׸����� 10^p
 * @return float 
 */
float tr_s(u8 *s, int start, int num, int p)
{
	float ans=0;
	short negative=1;
	if(s[start++] == '-') {negative=-1;}
	num--;
	while(num>0)
	{
			ans+= tr(s[start++])*pow(10,p--);
			num--;
	}
	ans*=negative;
	return ans;
}

int tr_head(u8 *s, int start, int num, int p)
{
	int ans=0;
	while(num>0)
	{
		ans+= tr(s[start++])*pow(10,p--);
		num--;
	}
	return ans;
}

/*
	p ���Կ����Ǳ�����С��λ��
*/
int f_to_u(float data, u8 p)
{
	int t = (int)floor(data*pow(10,p));
	return t;
}

void swrite(char *buf, int txt, u8 start)
{
	u8 i;
	if(txt<0) {buf[start]='-';txt*=-1;}
	else 			{buf[start]='+';}
	start++;
	for(i=0;i<4;i++)
	{
		u16 f=(u16)(pow(10,3-i));
		buf[start+i]=txt/f + 48;
		txt %= f;
	}
	//buf[++start]=0;
	return;
}

const u8 for_bac=1, le_ri=2, turn=3;
void data_receive(void)
{
	u8 *buf;
	u8 msg_type;
//#if USART1_DMA
//	if(!USART1_RX_FLAG)
//	{info_req=0;return;}
//	USART1_RX_FLAG = 0;
//	buf = usart1_rxbuf;
//#else
	if(!(USART_RX_STA&0x80))
	{return;}
	buf = USART_RX_BUF;
//#endif
	
	msg_type = buf[0]-'0';
	switch(msg_type)
	{
		/*
		case WARN_MSG:
		{
			warn = tr_s(buf,1,3,2);//100*(rx_buf[1]-'0') + 10*(rx_buf[2]-'0') + (rx_buf[3]-'0');
			//info_req = WARN_MSG;
			break;
		}
		*/
		case SPD_MSG:
		{
			u8 mov_mod = tr(buf[1]);
			switch(mov_mod)
			{
				case for_bac:
				{
					target_Vx = tr_s(buf,2,5,1);
					target_Vy = 0;
					target_W  = 0;
					break;
				}
				case le_ri:
				{
					target_Vx = 0;
					target_Vy = tr_s(buf,2,5,1);
					target_W  = 0;
					break;
				}
				case turn:
				{
					target_Vx = 0;
					target_Vy = 0;
					target_W  = tr_s(buf,2,5,1);
					break;
				}
			}
			//printf("speed:%.2f, ",target_speed);
			//info_req = SPD_MSG;
			break;
		}
		
		case POS_MSG://Ҫǰ��������
		{
			target_Px = tr_s(buf,1,4,2);//100*(rx_buf[1]-'0') + 10*(rx_buf[2]-'0') + (rx_buf[3]-'0') + 0.1*(rx_buf[4]-'0');
			target_Py = tr_s(buf,5,4,2);//100*(rx_buf[5]-'0') + 10*(rx_buf[6]-'0') + (rx_buf[7]-'0') + 0.1*(rx_buf[8]-'0');
			target_yaw = tr_s(buf,9,4,2);
			//info_req = POS_MSG;
			break;
		}
		
		case PARA_MSG:
		{
			u8 para_type = tr(buf[1]);
			switch(para_type)
			{
				case 1:
				{
					break;
				}
				case 2:
				{
					break;
				}
				case 3:
				{
					break;
				}
				case 7:
				{
					//Turn_KP = tr_s(buf)
					break;
				}
				case 8:
				{
					break;
				}
			}
			break;
		}
		case ACK_MSG:
		{
			//ACK=1;
			break;
		}
		
		
	}
	
//#if !USART1_DMA
//USART_RX_STA=0;
//#endif
	return;
}

#endif	

