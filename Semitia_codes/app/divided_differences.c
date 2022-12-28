#include <math.h>
#include <stdio.h>

#define num 8

float fx(float x)
{
    return 7*pow(x,7) + 6*pow(x,5) + 5*pow(x,3) + 4*x*x + 3*x + 1;
}

int main()
{
    float x[num] = {0,1,2,3,4,5,6,7};
    float F[num][num];//F[i][j] 表示从xi开始的j阶差商
    for(int i=0;i<num;i++)
    {   F[i][0] = fx(x[i]); 
        printf("%.0f, ",F[i][0]);}

    printf("\r\n");
    for(int i=1;i<num;i++)
    {
        for(int j=0;j<num-i;j++)
        {
            F[j][i] = (F[j][i-1] - F[j+1][i-1])/(x[j] - x[j+i]);
            printf("F[%.0f...%.0f]:%.2f, ",x[j],x[j+i],F[j][i]);
        }
        printf("\r\n");
    }

    system("pause");
    return 0;
}