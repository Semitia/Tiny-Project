#include <stdio.h>
#include <stdlib.h>
#define u32 unsigned int

int bitset_contains(u32 s,int i)
{
    if(((s>>i) & 0x01) == 1) return 1;
    else return 0;
}

int bitset_size0(u32 s)
{
    int n;
    for(int i=0; i<32; i++)
    {
        n += bitset_contains(s,i);
    }
    return n;
}

int bitset_size(u32 s)
{
    s = (s & 0x55555555) + ((s>>1) & 0x55555555); //01010101010101010101010101010101
    s = (s & 0x33333333) + ((s>>2) & 0x33333333); //00110011001100110011001100110011
    s = (s & 0x0f0f0f0f) + ((s>>4) & 0x0f0f0f0f); //00001111000011110000111100001111
    s = (s & 0x00ff00ff) + ((s>>8) & 0x00ff00ff); //00000000111111110000000011111111
    s = (s & 0x0000ffff) + ((s>>16) & 0x0000ffff);//00000000000000001111111111111111
    return s;
}

int main()
{
    int a = 7;
    printf("%d",bitset_size(a));
    system("pause");
    return 0;
}