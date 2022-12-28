#include <stdio.h>
#include <stdlib.h>
using namespace std;

#define u32 unsigned int
/**
 * @brief 调试函数，查看32位数的二进制存储形式
 * @param 32位数
 */
void debug_see(u32 t)
{
    printf("this is d: %d\r\n",t);
    printf("this is u: %u\r\n",t);
    for(int i=1;i<=32;i++)
    {
        if(t&0x80000000) printf("1");
        else printf("0");
        t = t<<1;
    }
    printf("\r\n");
    return;
}

/**
 * @brief 加法函数
 * @param ini_x 加数1
 * @param ini_y 加数2
 * @return 补码形式的结果
 */
u32 addition(u32 ini_x, u32 ini_y)
{
    //不需要转换为补码
    u32 x = ini_x, y = ini_y;
    //和，进位
    u32 ans,carry;
    ans = x ^ y;
    carry = (x & y) << 1;
    //不断获取进位与不考虑进位的和，迭代相加，直至进位为0
    while(carry != 0)
    {
        u32 i = ans, j = carry;
        ans = i ^ j;
        carry = (i & j) << 1;  
    }
    //DEBUG调试
    //printf("DEBUG: %d add %d = %d\r\n",ini_x,ini_y,ans);
    return ans;
}

/**
 * @brief “Get the complement” 获取补码
 * 
 * @param t 
 * @return u32 补码
 */
u32 get_com(u32 t)
{
    if(!(t&0x80000000)) {return t;}
    t = ~t;
    t |= 0x80000000;
    return addition(t,1);
}

/**
 * @brief 由补码获取原码
 * @param t 
 * @return u32 原码
 */
u32 back_com(u32 t)
{
    //debug_see(t);
    if(!(t&0x80000000)) {return t;}
    t = ~addition(t,-1);
    t |= 0x80000000;
    //debug_see(t);
    return t;
}

/**
 * @brief 减法
 * @param x 被减数
 * @param y 减数
 * @return u32 差
 */
u32 subtraction(u32 x, u32 y)
{
    return addition(x,-y);
}

/**
 * @brief 乘法
 * @param ini_x 
 * @param ini_y 
 * @return u32 积
 */
u32 multiplication(u32 ini_x, u32 ini_y)
{
    bool negative_flag=0;
    u32 ans=0, x=ini_x, y=ini_y;
    //判断积正负
    if( ( (x&0x80000000) && !(y&0x80000000) )||( !(x&0x80000000) && (y&0x80000000) ) ) {negative_flag=1;}
    //要转换为原码并去掉符号位
    x = back_com(x);
    y = back_com(y);
    x &= 0x7fffffff;
    y &= 0x7fffffff;
    //借鉴竖乘法
    for(int i=1;i<=31;i++)
    {
        if(y&0x01) {ans = addition(ans,x);}
        y = y>>1;
        x = x<<1;
        if(y == 0) {break;}
    }
    //加上正负号
    if (negative_flag) 
    {
        ans |= 0x80000000;//这个时候依然是原码，想直接用%d，要转换为补码
        ans = get_com(ans);
    }
    //DEBUG调试
    //printf("DEBUG: %d multiply %d = %d\r\n",ini_x,ini_y,ans);
    return ans;
}

/**
 * @brief 除法答案结构体
 * @param ans 商
 * @param mod 余数
 */
typedef struct __ans_division_t{
    u32 ans;
    u32 mod;
}ans_division_t;

/**
 * @brief 除法
 * @param ini_x 被除数
 * @param ini_y 除数
 * @return ans_division_t* 答案指针
 */
ans_division_t *division(u32 ini_x, u32 ini_y)
{
    bool negative_flag=0;
    u32 ans=0, carry=0, x=ini_x, y=ini_y;
    ans_division_t *ans_division;
    ans_division = (ans_division_t*)malloc(sizeof(ans_division_t));
    if( ( (x&0x80000000) && !(y&0x80000000) )||( !(x&0x80000000) && (y&0x80000000) ) ) {negative_flag=1;}
    //要转换为原码并去掉符号位
    x = back_com(x);
    y = back_com(y);
    x &= 0x7fffffff;
    y &= 0x7fffffff;
    //依然是模拟手除流程
    //除数不断进位直至比被除数大
    while(y < x) 
    {
        y = y<<1;
        carry = addition(carry,1);
    }
    //如果能除，商就加上1<<carry，除数再不断右移直至回到本来大小。
    while(carry)
    {
        if(x>=y)
        {
            ans = addition(ans,1<<carry);
            x = addition(x,-y);
        }
        y = y>>1;
        carry = addition(carry,-1);
    }
    if(x>=y) 
    {
        ans = addition(ans,1);
        x = addition(x,-y);
    }
    //赋值
    ans_division->ans = ans;
    ans_division->mod = x;
    return ans_division;
}

int main()
{
    u32 num1,num2,mode;
    int test;
    ans_division_t *ans_of_div;
    ans_of_div = (ans_division_t*)malloc(sizeof(ans_division_t));
    printf("Please choose the function and the number\r\n");
    printf("param1: mode(0:+; 1:-; 2:x; 3:/)\r\nparam2:num1;\r\nparam3:num2\r\n");
    
    while(1)
    {
        scanf("%u%u%u",&mode,&num1,&num2); 
        switch (mode)
        {
            case 0:
                printf("%d\r\n",addition(num1,num2));
                break;
            case 1:
                printf("%d\r\n",subtraction(num1,num2));
                break;
            case 2:
                printf("%d\r\n",multiplication(num1,num2));
                break;
            case 3:
                ans_of_div = division(num1,num2);
                printf("%d......%d\r\n",ans_of_div->ans,ans_of_div->mod);
                break;
        }
        printf("Wait for input......\r\n");
    }

    system("pause");
    return 0;
}
