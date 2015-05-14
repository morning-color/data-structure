#include <iostream>
#include <vector>
#include "stack.h"
#include "calculator.h"
#include "../include/public_lib.h"
int main()
{
std::vector<std::vector<int> > a 
{
    {132, 1024, 124, 1280},
    {876, 1024, 660, 2560},
    {260, 2048, 252, 2560},
    {876, 2048, 916, 3840}
};
    #if 0
    // 正整数进制转换
    stack<char> s;
    convert_scale(s,64,2);
    for (int i = s.size(); i > 0; --i)
        PRINT_IT(s.pop());
    puts("");
    #endif
    
    #if 0
    //判断一个栈是否是另一个栈的栈混洗
    // 注意栈的方向  [******>
    stack<char> s {'a','b','c','d'};
    stack<char> p {'d','c','b','a'};
    is_stack_permutation(s, p)? puts("yes") : puts("no");
    #endif    

    #if 0
    //检测表达式是否是括号匹配的
    std::string str = "(8*9+(5+6)-4)+(8!-(8/9))";
    recursive_bracket_matched(str)? puts("yes") : puts("no"); 
    #endif

    auto result = calculate("(8*9+(5+6)-4) + (8!-(8/9))");
    PRINT_IT(result);
    for (auto vec : a)
    {
        puts("");
        for(auto elem : vec) std::cout << elem*4 << " ";
    }


    return 0;
}

