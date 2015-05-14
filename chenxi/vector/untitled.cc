#include <iostream>
#include <stdexcept>
#include <memory>
#include <vector>
#include <boost/progress.hpp>
// #include <boost/date_time/posix_time/posix_time.hpp>
#include "../include/public_lib.h"
#include <string>
#include <bitset>
#include <type_traits>
#include <complex>
// class empty_class 
// {
// public:
//     empty_class() {init(); PRINT_IT(elem);}
//     empty_class(int i):empty_class() {elem =i;}
//     void init() {elem += 1;}
// private:
//     int elem {1};
// };

constexpr int get_i() {return 1;}

int main(int argc, char const *argv[])
{
    switch(get_i())
    {
        case 1:
            PRINT_IT(get_i());
    }

    // SHOW_DURATION();
    auto sptr = std::make_shared<int>(10);
    PRINT_IT(sptr);
    sptr.reset();
    PRINT_IT(sptr);
    
    // auto sptr_vec = std::make_shared<std::vector<int>>(10,20);

    // std::shared_ptr<std::vector<int>> sptr_vec_1 (new std::vector<int>(10,30));

    // std::vector<char> *p_v = new std::vector<char>(10,'a');
    // std::shared_ptr<std::vector<char> > sptr_v(p_v); 

    // std::unique_ptr<int[]> p (new int [10]);
    
    // std::unique_ptr<int[]> uptr = std::move(p);

    // for( const auto& elem : *(sptr_vec))
    //     PRINT_IT(elem);

    // for( const auto& elem : *(sptr_vec_1))
    //     PRINT_IT(elem);
    // PRINT_HERE("here");

    // // empty_class ec(3);

    // int a = 0, b = 1;
    // int& c = a;
    // int& d = b;
    // // ref_swap(a,b);
    // decltype(c+d) e;
    // std::cout << std::is_lvalue_reference<decltype(c+d)>::value;
    // // std::cout << a << " "<< b;
    // // std::cout << typeid(e).name();
    // // PRINT_IT(b);

    // // char * str1="6789";
    // // char * str2="123456789";
    // // strcpy(str1,str2);
    // // printf("str1=%s\n",str1);
    // // printf("str2=%s\n",str2);
    // char  f = 0b10000111; 
    // // for (int i = 0; i < 8; ++i)
    // // {
    // //     int x = a & 1;
    // //     std::cout << x;
    // //     a>>=1;
    // // }
    // // std::cout << std::bitset<8>(a)<<"\n";
    // // a <<= 2;
    return 0; 
}