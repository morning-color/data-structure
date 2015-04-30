#include <iostream>

inline long sqr(long x) {return x*x;}

static long power(int n) //计算2^n 蛮力版 设r为n的二进制位数则有 n = 2^r 或 n = 2^r+1
{   
    long pwr = 1;
    for (int i = 0; i < n; ++i)
        pwr <<= 1;
    return pwr;
}//复杂度 O(n) = O(2^r)

static long iterator_power(int n)
{
    long pwr = 1;
    for (long i = 2; n > 0; n >>= 1, i *=i)
    {
        if(n&1) pwr *= i;
    }
    return pwr;
}//复杂度 O(r) = O(logn)

static long recursion_power(int n)
{
    if (n <= 0) return 1;
    return (n&1)? sqr(recursion_power(n>>1)) << 1 : sqr(recursion_power(n>>1));
}//复杂度 O(r) = O(logn)

int main()
{
    std::cout << iterator_power(4);

    return 0;
}