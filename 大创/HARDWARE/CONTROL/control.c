#include "control.h"
#include "usart.h"
#include "encoder.h"

u8 i;
int Motor_pwm[4];
const float Kx[4] = {0,0,0,0}, Ky[4] = {0,0,0,0};
float target_wheel[4]={0,0,0,0}, real_wheel[4];
float target_Vx=0, target_Vy=0, target_W=0;
float target_Px=0, target_Py=0, target_yaw=0;
float x=0,y=0,yaw=1.57;

void Move_Ctrl(void)
{
	Forward();
	Inverse();
	PID(target_wheel,real_wheel,Motor_pwm);
	Set_Pwm(Motor_pwm);
}

void Forward(void)
{
	for(i=0;i<4;i++)
	target_wheel[i] = target_Vx*Kx[i] + target_Vy*Ky[i];
	
	printf("DEBUG-target_wheel: A %.2f, B %.2f, C %.2f, D %.2f\r\n",target_wheel[0],target_wheel[1],target_wheel[2],target_wheel[3]);
	return;
}

void Inverse(void)
{
	for(i=0;i<4;i++) 
	real_wheel[i] = Read_Encoder(i+2);
	
	return;
}
	
/*绝对值*/
float my_abs_f(float x)
{
	if(x >= 0) {return x;}
	else {return -x;}
}
int16_t my_abs_i(int x)
{
	if(x >= 0) {return x;}
	else {return -x;}
}

void PID(float *target, float *real, int *pwm)
{
	static float last_spd_target[4]={0,0,0,0};
	static float bias[4],bias_last[4],bias_integral[4] = {0,0,0,0};
	
	for(i=0;i<4;i++)
	{
		//积分清除
		if(target[i] != last_spd_target[i]) {bias_integral[i] = 0;}
		//获得偏差值
		bias[i] = target[i] - real[i];
		//变速积分
		if(my_abs_f(bias[i]) < I_low)
		{ bias_integral[i] += (bias[i] + bias_last[i])/2; }
		else if(my_abs_f(bias[i]) < I_up)
		{ bias_integral[i] += ((my_abs_f(bias[i]) - I_low)/(I_up - I_low))*((bias[i] +  bias_last[i])/2 ); }
		//PID计算电机输出PWM值
		pwm[i] = (int) ax_motor_kp*bias[i]*PID_SCALE + ax_motor_kd*(bias[i]-bias_last[i])*PID_SCALE + ax_motor_ki*bias_integral[i]*PID_SCALE;
		//记录上次偏差
		bias_last[i] = bias[i];
		//限制最大输出
		if(pwm[i]> 2000) pwm[i] = 2000;
		if(pwm[i] < -2000) pwm[i] = -2000;
	}
	return;
}

/*
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
*/
void Set_Pwm(int *pwm)
{
	if(pwm[0]<0)	AIN2=1,			AIN1=0;
	else 	        AIN2=0,			AIN1=1;
	PWMA=my_abs_i(pwm[0]);
	
	if(pwm[1]<0)	BIN1=0,			BIN2=1;
	else          BIN1=1,			BIN2=0;
	PWMB=my_abs_i(pwm[1]);	
	
	if(pwm[2]<0)	CIN2=1,			CIN1=0;
	else 	        CIN2=0,			CIN1=1;
	PWMA=my_abs_i(pwm[2]);
	
	if(pwm[3]<0)	DIN1=0,			DIN2=1;
	else          DIN1=1,			DIN2=0;
	PWMB=my_abs_i(pwm[3]);		
}

void sendmsg(void)
{
	char buf[20];

	int s1=f_to_u(x,1), s2=f_to_u(y,1), s3=f_to_u(yaw,3);
	buf[0]=POS_MSG+48;
	swrite(buf,s1,1);
	swrite(buf,s2,6);
	swrite(buf,s3,11);
	//sprintf(buf,"%u%d%d%d\r\n",POS_MSG,s1,s2,s3);

	printf("%s",buf);
	
	/*
	switch(info_req)
	{
		case 0:
		{
			break;
		}
		case WARN_MSG:
		{
			char str[4];
			sprintf(str,"%u",warn);
			DMA_USART1_Tx_Data("INFO:WARN= ",12);
			DMA_USART1_Tx_Data(str,4);
			break;
		}
		case SPD_MSG:
		{
			if(target_speed>0) {DMA_USART1_Tx_Data("Forward",8);}
			else if(target_speed<0) {DMA_USART1_Tx_Data("Back",6);}
			else if(target_omiga<0) {DMA_USART1_Tx_Data("TurnRight",10);}
			else {DMA_USART1_Tx_Data("TurnLeft",9);}
			break;
		}
		case POS_MSG:
		{
			break;
		}
		case PARA_MSG:
		{
			break;
		}
		case DES_MSG:
		{
			break;
		}
	}
	*/
	return;
}



