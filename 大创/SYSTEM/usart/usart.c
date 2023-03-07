#include "sys.h"
#include "usart.h"	
#include "control.h"
#include "math.h"

#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

 
 
#if EN_USART1_RX   //如果使能了接收
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA=0;       //接收状态标记	  
  
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
		{
		Res =USART_ReceiveData(USART1);	//读取接收到的数据
		
		if((USART_RX_STA&0x8000)==0)//接收未完成
			{
			if(USART_RX_STA&0x4000)//接收到了0x0d
				{
				if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
				else USART_RX_STA|=0x8000;	//接收完成了 
				}
			else //还没收到0X0D
				{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
					{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//接收数据错误,重新开始接收	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
	OSIntExit();  											 
#endif
} 
	
int tr(char t)
{ 
    return t-48; 
}

/**
 * @brief 将字符串从start位开始的num位转换为float数
 * @param s 字符串
 * @param start 起始位
 * @param num 位数
 * @param p 首个数乘 10^p
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
	p 可以看做是保留的小数位数
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
		
		case POS_MSG://要前往的坐标
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

