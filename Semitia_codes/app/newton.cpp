#include<iostream>
#include<cstdio>
#include<cmath>
using namespace std;

double function(double x)
{
    return x*x*x - 2*x*x - 4*x - 7;
}

int main()
{
    double t0,x0,x1;
    cin>>x0>>x1;
    while(abs(x1-x0)>0.001)
    {
        double t;
        t0 = x0;
        t = x1;
        x1 = x1 - (x1-x0)*function(x1)/(function(x1)-function(x0));
        x0 = t;
        printf("X0:%.10f, X1:%.10f\r\n",x0,x1);
        x1 = t0 - ((x0-t0)*(x0-t0))/(x1-2*x0+t0);
        printf("x1_:%.10f\r\n",x1);
    }

    system("pause");
    return 0;
}