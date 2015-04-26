// 计算 1 - 1/x + 1/(x*x) - ... 共计算n次
// 注意每一项之间的规律,他们之间是除以(-x)的关系,所以拿一个变量来存储每一次的项,用另一个变量存储相加的结果

#include <iostream>
#include <stdio.h>
#include "common.h"


int main(int argc, char const *argv[])
{
    decltype(0) n = 0 ;
    double x = 0, elem = 1, sum =1;
    std::cout << "请输入 x 和 n : \n";

    scanf("%lf%d",&x,&n);
    print_it(x);
    print_it(n);
    for (int i = 0; i < n-1; ++i)
    {
        elem /= -x;
        sum  += elem;
        print_it(elem);
        print_it(sum);
        
    }
    print_it(sum);
    return 0;
}

