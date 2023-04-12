#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

#define C1 PBin(4)
#define C2 PBin(3)
#define C3 PAin(5)
#define C4 PAin(4)

#define SPEED_Y 40 //����(ǰ��)����趨�ٶ�
#define SPEED_Z 100//ƫ��(����)����趨�ٶ� 
#define ACCX_FIX 100
#define ACCY_FIX -200
#define ACCZ_FIX 16000
#define PI 3.14159265

#define I_up 80
#define I_low 55
#define PID_SCALE  0.1f  //PID����ϵ��

#define PWMA   TIM8->CCR1  //PC6
#define AIN2   PCout(4)
#define AIN1   PCout(5)
#define PWMB   TIM8->CCR4  //PC7
#define BIN1   PCout(2)
#define BIN2   PCout(3)
#define PWMC   TIM8->CCR1  //PC8
#define CIN2   PCout(0)
#define CIN1   PCout(1)
#define PWMD   TIM8->CCR4  //PC9
#define DIN1   PAout(11)
#define DIN2   PAout(12)

void Move_Ctrl(void);
void Forward(void);
void Inverse(void);
void PID(float *target, float *real, int *pwm);
void Set_Pwm(int *pwm);

void data_receive(void);
void send_msg(void);
void state_info(void);

extern float target_wheel[4];
extern int Motor_pwm[4];
extern float target_Vx, target_Vy, target_W;
extern float target_Px, target_Py, target_yaw;
#endif
