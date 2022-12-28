# 位运算实现四则运算

## 加法

### 算法思路

​	在平时手算的时候我们经常会**列竖式**，先不考虑进位，只看**和**和**相加的两个数**同位的数，把进位数写小一点记在横线上方。**计算下一位的时候再加上上一位的进位。**

​	所以在程序中也是模拟这样一个思路：

- 先不看进位，得到一个和；记下进位
- 进位在与和相加，不看进位得到一个新的和；同时也得到新的进位
- 不断迭代递推，直到不再产生新的进位

### 代码实现

```cPP
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
```

​	可以发现，

- 不看进位得到的和其实就是**两数异或**，即`ans = x ^ y;`；
- 进位即**两数相与再左移一位**，即`    carry = (x & y) << 1;`

## 减法

​	**a-b** 即 **a+(-b)**

## 乘法

	### 算法思路

​	可以将被乘数不断加**乘数次数**的自己。但是这样效率不高。同样是模拟手算的思想，从**乘数的最低位**到**最高位**，在**第i位**，即给最后乘积贡献的值为

==被乘数 * 乘数[i] * (进制数)^(i-1)^==

​	在这个算法下不再能直接用补码参与计算，所以要换成原码并去除符号位。在开始的时候根据两数记下乘积的正负值，计算完毕之后再赋回去。

### 代码实现

```cpp
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
```

## 除法

​	因为除法有**商**和**余数**，所以用了一个结构体方便存储。

```cpp
/**
 * @brief 除法答案结构体
 * @param ans 商
 * @param mod 余数
 */
typedef struct __ans_division_t{
    u32 ans;
    u32 mod;
}ans_division_t;
```

### 算法思路

​	也是模拟手算流程，在理解了乘法的实现思路后便很容易理解。

- 将被除数不断**进位**，即乘上**进制数**，直到恰好比被除数小。
- 用被除数除以**进位后的除数**得到**商**（为了区别最终的商把它叫做*小商*吧），同时**商**加上==小商*进制数^(进位次数)^==
- 被除数不断**退位**（除以进制数），同时重复上述操作，直到回到最初大小，即**进位数为0**

### 代码实现

```cpp
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
```

​	因为负数的余数比较奇怪，平时也不用，这里没有考虑在内。

## DEBUG

​	vscode 程序文件不能用中文命名。

​	在程序运行的时候使用补码记录。即便是**usigned int**，倘若**scanf -9**，对应二进制编码也是**11111111111111111111111111110111**。**int**型同样如此。

​	所以在运算的时候不需要人为的去转换为补码了，在这方面耽误了不少时间。

​	而对于同样的一串二进制编码，**printf("%d")**和**printf("%u")**输出结果是不一样的：%d会自动转换输出原码对应的有符号数；那%u则是会将这个补码当作原码直接换算成十进制输出。

![image-20221028003819253](C:\Users\Semitia\AppData\Roaming\Typora\typora-user-images\image-20221028003819253.png)

![image-20221028122635111](C:\Users\Semitia\AppData\Roaming\Typora\typora-user-images\image-20221028122635111.png)

​	其余的就是一些逻辑漏洞了，不得不说VScode写代码还是比Keil舒服😁











