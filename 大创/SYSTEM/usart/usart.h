#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 

#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����

#define WARN_MSG 1 //warn
#define SPD_MSG  2 //speed
#define POS_MSG  3 //position
#define PARA_MSG 4 // param
#define DES_MSG  5 //destination
#define ACK_MSG  6
#define INFO_MSG 7

extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);
void data_receive();
int tr(char t);
float tr_s(u8 *s, int start, int num, int p);
int tr_head(u8 *s, int start, int num, int p);
int f_to_u(float data, u8 p);
void swrite(char *buf, int txt, u8 start);

#endif


