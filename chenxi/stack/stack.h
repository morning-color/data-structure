#pragma once
#include <string>
#include "../vector/my_vector.h"
#include <boost/assert.hpp>
#include <stdexcept>
#include <cmath>


//这里调用基类的成员前要加this->,否则编译器会报错,这是因为基类是个模板的缘故
//普通的非模板基类一般是不需要这么做的    
template<typename T>
class stack : public my_vector<T>
{
    using my_vector<T>::my_vector;
public:
    T pop(){return this->remove(this->size()-1);} 
    void push(T elem){this->push_back(elem);}
    T& top() const {return (*this)[this->size()-1]; }

};



void convert_scale(stack<char>& s, int num, int base) //十进制到base进制的转换
{
    static char symbol[] {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
    if (num > 0)
    {
        s.push(symbol[num%base]);
        convert_scale(s,num/base, base);
    }
}

template <typename T>
bool is_permutation(stack<T>& origin, stack<T>& per)//繁琐版,判定per栈是否是origin栈的栈混洗
{
    BOOST_ASSERT_MSG((!per.empty()) && (!origin.empty()),"输入的栈中有空栈!"); 
    int size = per.size(); 
    if(size != origin.size()) return false;
    //将要测试的栈反向存储,获取其元素进栈的顺序
    my_vector<T> vec;
    vec.resize(size);
    for (int i = size-1; i >= 0; --i) vec[i] = per.pop(); 
         
    stack<T> trans;//中转栈
    int i = 0;
    //模拟进栈过程
    trans.push(origin.pop());
    while(!origin.empty() || !trans.empty())
    {
        if (trans.top() != vec[i] && origin.empty()) return false;//中途退出
        if (trans.top()==vec[i]) 
        {
            trans.pop();
            ++i;
        }
        if(!origin.empty()) trans.push(origin.pop());
    }
    return (trans.empty())? true : false;
}

/*******************************************************
   |========>                             <========|
   permutation            ^                 origin
|================     pop |           ================|
|   C    B    A      /    | ___push___   A    B    C  |
|================  /      | |         ================|
    |            /      | | V |
    |      equal?       |     |
    |------------------>|  B  |
                        |  A  |
                        |=====|
                         trans
********************************************************/
template <typename T>
bool is_stack_permutation(stack<T>& origin, stack<T>& per)//优化版,判定per栈是否是origin栈的栈混洗
{
    BOOST_ASSERT_MSG((!per.empty()) && (!origin.empty()),"输入的栈中有空栈!"); 
    int size = per.size(); 
    if(size != origin.size()) return false;
    //将要测试的栈反向存储,获取其元素进栈的顺序
    my_vector<T> vec;
    vec.resize(size);
    for (int i = size-1; i >= 0; --i) vec[i] = per.pop(); 
         
    stack<T> trans;//中转栈
    int i = 0;
    //模拟进栈过程
    while(!origin.empty())
    {
        trans.push(origin.pop());
        while ( (!trans.empty()) && (trans.top() == vec[i]) ) 
        {
            trans.pop();
            ++i;
        }
    }
    return (trans.empty())? true : false;
}//复杂度 O(n),不要以为两个while就是O(n^2),其实这里最大为2n


bool if_bracket_matched(const std::string& str)//低效版,检测表达式是否是括号匹配的
{
    stack<char> s;
    for (auto& elem : str)
    {
        switch(elem)
        {
            case '(': 
            case '[': 
            case '{': s.push(elem); break;
            case ')': if(s.top() == '(') s.pop(); break;
            case ']': if(s.top() == '[') s.pop(); break;
            case '}': if(s.top() == '{') s.pop(); break;
            default : break;
        }
    }
    return s.empty();
}


bool is_bracket_matched(const std::string& str)//高效版,检测表达式是否是括号匹配的
{
    stack<char> s;
    for (auto& elem : str)
    {
        switch(elem)
        {
            case '(': 
            case '[': 
            case '{': s.push(elem); break;
            case ')': if( s.empty() || (s.pop()!='(') ) return false; break;
            case ']': if( s.empty() || (s.pop()!='[') ) return false; break;
            case '}': if( s.empty() || (s.pop()!='{') ) return false; break;
            default : break;
        }
    }
    return s.empty();
}

//递归版,比较困难,需仔细推敲每一步微妙的条件判定,特别是:带不带=, +1和-1
bool recursive_bracket_matched(const std::string& str, int lo, int hi)
{
    auto trim = [&]
    {
        //对于==的情况,有可能表达式中只有一个'(' 或 ')',也是非法的.
        while( (lo <= hi) && (str[lo] != '(') && (str[lo] != ')') ) ++lo; 
        while( (lo <= hi) && (str[hi] != '(') && (str[hi] != ')') ) --hi; 
    };

    auto divide = [&]() ->int //这里divide是一个函数指针,用的时候应该使用divide(),不要用divide 
    {
        int mi = lo;
        int crc = 1;
        while( (0 < crc) && (++mi < hi) )
        {
            if (str[mi] == '(') ++crc;
            if (str[mi] == ')') --crc;
        }
        return mi;
    };

    trim();
    if(lo > hi) return true;
    if(str[lo] != '(') return false;
    if(str[hi] != ')') return false;
    int mi = divide();
    if(mi > hi) return false;
    return recursive_bracket_matched(str,lo+1,mi-1) && recursive_bracket_matched(str,mi+1,hi); 
}
bool recursive_bracket_matched(const std::string& str)
{
    recursive_bracket_matched(str, 0, str.size()-1);
}

