#pragma once
#include "stack.h"


//calculator
typedef enum : size_t {ADD=0,SUB,MUL,DIV,POW,FAC,LP,RP,END} d_oprator;//用于查表
static const char pri[9][9] =  
{
  /*           |----------------当前运算符-------------------|  */  
  /*               +    -    *    \    ^    !    (    )    \0  */  
  /* --  +  */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
  /* |   -  */    '>', '>', '<', '<', '<', '<', '<', '>', '>',
  /* 栈  *  */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
  /* 顶  /  */    '>', '>', '>', '>', '<', '<', '<', '>', '>',
  /* 运  ^  */    '>', '>', '>', '>', '>', '<', '<', '>', '>',
  /* 算  !  */    '>', '>', '>', '>', '>', '>', ' ', '>', '>',
  /* 符  (  */    '<', '<', '<', '<', '<', '<', '<', '=', ' ',
  /* |   )  */    ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
  /* --  \0 */    '<', '<', '<', '<', '<', '<', '<', ' ', '='
};

d_oprator convert_to_opr(char elem) 
{
    d_oprator opr;
    switch(elem)
    {
        case '+' : opr = ADD; break; 
        case '-' : opr = SUB; break;
        case '*' : opr = MUL; break;
        case '/' : opr = DIV; break;
        case '^' : opr = POW; break;
        case '!' : opr = FAC; break;
        case '(' : opr = LP;  break;
        case ')' : opr = RP;  break;
        case '\0': opr = END; break;
        default  : 
            PRINT_IT(elem); 
            throw std::runtime_error("输入的操作符不合法3!");
    }
    return opr;
}


float calculate(const std::string& exp)
{
    stack<float> s_num;   //数字栈
    stack<char>  s_opr;   //运算符栈

    auto pri_between = [](char top, char cur) 
    {
        auto l = convert_to_opr(top);
        auto r = convert_to_opr(cur);
        return pri[size_t(l)][size_t(r)];
    };
    auto is_num = [](char elem) ->bool {return (elem >= '0') && (elem <= '9');};
    auto culc_two = [](char elem) ->bool 
    {
        switch(elem)
        {
        case '+': 
        case '-': 
        case '*': 
        case '/': 
        case '^': return true;
        default : return false;
        }
    };
    auto read_num = [](char*& ptr) 
    {
        float number = 0;
        for(; (*ptr >= '0') && (*ptr <= '9'); ++ptr)
            number = number*10 + int(*ptr-'0');
        return number;
    };
    auto figure_out1 = [](float num, char opr) ->float
    {
        if (opr != '!') throw std::runtime_error("输入的操作符不合法4!");
        int tmp = num;
        while(0 < --num) tmp *= num;
        return tmp;
    };
    auto figure_out2 = [](float lnum, char opr, float rnum) ->float
    {   
        switch(opr)
        {
        case '+': return lnum + rnum;
        case '-': return lnum - rnum;
        case '*': return lnum * rnum;
        case '/': return lnum / rnum;
        case '^': return pow(lnum,rnum);
        default : throw std::runtime_error("输入的操作符不合法1!");
        }
    };
    if (exp.size() == 0) return 0;
    if (!recursive_bracket_matched(exp)) throw std::runtime_error("输入的表达式不合法!");
    s_opr.push('\0');    
    char* cur = const_cast<char*>(&exp[0]);
    while(!s_opr.empty())
    {
        if(is_num(*cur)) s_num.push(read_num(cur));
        else if(*cur == ' ') ++cur;
        {
            switch(pri_between(s_opr.top(),*cur ) )
            {
            case '<':
                s_opr.push(*cur);
                ++cur;
                break;
            case '=':
                s_opr.pop();
                ++cur;
                break;
            case '>':
            {
                char op = s_opr.pop();
                if(op == '!') s_num.push( figure_out1(s_num.pop(),'!') );
                else
                {
                    float r = s_num.pop(), l = s_num.pop();
                    s_num.push(figure_out2(l,op,r));
                }
                break; 
            }
            default :throw std::runtime_error("输入的操作符不合法2!");
            }
        }
    }
return s_num.pop();
}