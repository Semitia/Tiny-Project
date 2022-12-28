#include <stdio.h>
#include <math.h>

#define H 0.2
#define H_test 0.01

/**
 * @brief dy/dx,即微分方程
 * 
 * @param x 
 * @param y 
 * @return 一阶导数值
 */
float dy_dx(float x, float y)
{
    return y - 2*x/y;
}

int main()
{
    FILE *fp = fopen("ODE_Euler.xls","w");
    float y = 1, x = 0;
    fprintf(fp,"x\ty\n");
    fprintf(fp,"%.2f\t%.2f\n",x,y);
    //普通Euler法
    while(x<=1)
    {
        y += H*dy_dx(x,y);
        x += H;
        fprintf(fp,"%.2f\t%.2f\n",x,y);
    }

    //提高采样频率
    y=1, x=0;
    fprintf(fp,"x\ty_high\n");
    fprintf(fp,"%.2f\t%.2f\n",x,y);
    while(x<=1)
    {
        y += H_test*dy_dx(x,y);
        x += H_test;
        fprintf(fp,"%.2f\t%.2f\n",x,y);
    }
    
    //改进Euler法
    y=1, x=0;
    fprintf(fp,"x\ty_plus\n");
    fprintf(fp,"%.2f\t%.2f\n",x,y);
    while(x<=1)
    {
        float pre_y;
        pre_y = y + H*dy_dx(x,y);
        y += H*(dy_dx(x,y) + dy_dx(x+H,pre_y))/2;
        x += H;
        fprintf(fp,"%.2f\t%.2f\n",x,y);
    }
    fclose(fp);
    system("pause");
    return 0;
}