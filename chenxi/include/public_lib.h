_Pragma("once");
#include <iostream>
#include <boost/format.hpp>
#include <utility>
#include <ctime>

#define PRINT_IT(xxx) std::cout << boost::format("%1%  =  %2%\n") % #xxx % xxx 
#define PRINT_HERE(...) { print_fmt("%s",str_now()); \
                          print_fmt("File:%s/n, Func:%s, Line:%d-->",__FILE__,__func__,__LINE__); \
                          print_fmt("%s\n",__VA_ARGS__);}

#include <tuple>
#include <type_traits>
template<typename T>
int tuple_len(T t){ return std::tuple_size<decltype(t)>::value;} 
#define TUPLE_LEN(xxx) std::tuple_size<decltype(xxx)>::value

//for cout format strings 
template <typename T>
static void make_format(boost::format& fmt,  const T& variable)
{
    std::cout << fmt % variable << std::endl;
}

template <typename T, typename... others>
static void make_format(boost::format& fmt, const T& first, const others&... variables)
{
    fmt % first;
    make_format(fmt, variables...);
}
//对于这里的str:
//若声明为左值引用(const std::string& str),并且是个常量左值引用,它是个万能引用,用左值或右值对其进行初始化都是正确的
//因此不需要重载该函数的右值引用版本,即可将临时变量传给该函数
template <typename... T>
static void print_fmt(const std::string& str, const T&... variables)
{
    boost::format fmt(std::move(str)); 
    make_format(fmt,variables...);
}




//Boost date_time is not header only library. 
//Please build the library and then add it. Simple in gcc:
//g++ -std=c++11 untitle.cc -lboost_date_time
#include <boost/date_time/posix_time/posix_time.hpp>
namespace pt = boost::posix_time;
static std::string str_now()
{
  return pt::to_simple_string(pt::microsec_clock::local_time()).substr(0,24);
}

class boost_posix_timer
{
public:
  boost_posix_timer(const std::string& func)
  { 
      timer = pt::microsec_clock::local_time();
      ss<<"函数  "<< func <<"()\t运行时间为:";
  };
  ~boost_posix_timer()
  {
      pt::time_duration&& delay = pt::microsec_clock::local_time() - timer;
      auto duration  = delay.ticks()/1000.0;
      std::cout << ss.str() << duration <<"ms\n\n"; 
  };
private:
  std::stringstream ss;
  pt::ptime timer;
};
#define SHOW_DURATION() boost_posix_timer pd (__func__);



#include <sstream>
#include <boost/progress.hpp>
class process_timer
{
public:
    process_timer(const std::string& file, const std::string& func)
        {ss<<file<<"文件中, "<< func<<"函数 运行时间为:";};
    ~process_timer(){std::cout << ss.str();};
private:
    std::stringstream ss;
    boost::progress_timer t;
};
#define PROCESS_TIME() process_timer pt (__FILE__,__func__);

#include <utility>
template<typename T>
void ref_swap(T& a, T& b)
{
    T tmp(std::move(a) );
    a = std::move(b);
    b = std::move(tmp);
}

struct 
{
    template<typename T>
    void operator() (T t) {std::cout << t << "\t";}
} print_elem;

template<typename T>
void print_func(T& t) {std::cout << t << "\t";}
