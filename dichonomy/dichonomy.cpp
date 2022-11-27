#include<cstdio>
#include<cmath>
using namespace std;

#define MIN 0.3
#define MAX 0.4
#define precision 0.00001

double function(double x)
{
    return x*x*x*x - 3*x + 1;
}

int main()
{
    double left = MIN, right = MAX, mid;
    while(right - left > precision)
    {
        double ans;
        mid = (left+right)/2;
        ans = function(mid);
        if(ans < 0) right = mid;
        else left = mid; 
        printf("left: %.6f, right: %.6f, mid: %.6f, f(x): %.6f\r\n",left,right,mid,ans);
    }

    system("pause");
    return 0;

}