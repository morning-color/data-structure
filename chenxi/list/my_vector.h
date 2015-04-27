#pragma once
#include <iostream>
#include <boost/assert.hpp>
#include <algorithm>
#include "../include/public_lib.h"
#include <initializer_list>

template<typename T>
class my_vector
{
public:
    my_vector() noexcept {d_data = new T[INIT_CAPACITY];}
    my_vector(std::initializer_list<T> list) noexcept; 
    my_vector(const my_vector& vec) noexcept;
    my_vector(my_vector&& vec);
    ~my_vector() noexcept {if(d_data) delete [] d_data;}
    int size() const {return d_size;}
    void resize(int size) {d_size = size;expand();}
    void clear() {d_size = 0;shrink();}
    int capacity() const {return d_capacity;}
    T& operator[](size_t rank) const {return d_data[rank];}
    my_vector<T>& operator=(my_vector<T>& vec);
    int expand();
    int shrink();
    int remove(int lo, int hi);
    T remove(int rank);
    int find(const T& elem) const {find(elem,0,d_size);}
    int find(const T& elem, int lo, int hi) const;
    void insert(const T& elem, int rank);
    void push_back(const T& elem){insert(elem,size);}
    int search(const T& elem, int lo, int hi) const;
    int search(const T& elem) const {return (0 >= d_size)? -1 : search(elem, 0, d_size);}
    int uniquify();
    int uniquify_unsort();
    void traverse(void (*visit)(T&));
    template <typename TVS>
    void traverse(TVS& visit);
    void bubble_sort();
    void merge_sort(int lo, int hi);
    void merge(int lo, int mi, int hi);
    T*&  get_data_ptr() const {return d_data;}
private:
    T* d_data;
    static const int INIT_CAPACITY = 100;
    int d_size    = 10;
    int d_capacity = INIT_CAPACITY;
};

template<typename T>
my_vector<T>::my_vector(std::initializer_list<T> list) noexcept
{
    d_data = new T[INIT_CAPACITY];
    int rank = 0;

    for (auto i = list.begin(); i != list.end(); ++i)//第二项,只能用 != ,不能用 <
    {
        d_data[rank++] = *i;
    }
    d_size = rank;
}


template<typename T>
my_vector<T>::my_vector(const my_vector& vec) noexcept
{
    BOOST_ASSERT_MSG(vec,"输入序列无效!");
    d_size = vec.size();
    if (d_size <= 0)
    {
        my_vector();
        return;
    }
    d_data = new T[d_capacity = d_size<<1];
    std::copy(&vec[0], &vec[d_size-1], d_data); 
}

template<typename T>
my_vector<T>::my_vector(my_vector&& vec) 
{
    BOOST_ASSERT_MSG(vec,"输入序列无效!");
    d_size = vec.size();
    if (d_size <= 0)
    {
        my_vector();
        return;
    }
    d_data = &vec[0];
    d_capacity = vec.capacity();
    vec.get_data_ptr() = nullptr;
}

template<typename T>
my_vector<T>& my_vector<T>::operator=(my_vector<T>& vec)
{
    BOOST_ASSERT_MSG(vec,"输入序列无效!");
    if (d_data) delete [] d_data;
    (vec.size() <= 0)? my_vector() : my_vector(vec);
    return *this;        
}

template<typename T>
int my_vector<T>::expand()
{
    //这里必须是<,而不是<=;防止插入一个元素时,size从d_capacity涨到d_capacity+1,内存溢出
    if (d_size < d_capacity) return d_capacity;
    T* old_data = d_data;
    d_data = new T [d_capacity<<=1];
    std::copy_n(old_data, d_size, d_data);
    delete [] old_data;
    return d_capacity;
}

template<typename T>
int my_vector<T>::shrink()
{
    if (d_size > d_capacity/2) return d_capacity;
    while(d_size > (d_capacity>>=1))
        if (INIT_CAPACITY > d_capacity) return;
    T* old_data = d_data;
    d_data = new T [d_capacity];
    std::copy_n(old_data, d_size, d_data);
    delete [] old_data;
    return d_capacity;
}

//无序向量的顺序查找：返回最后一个元素e的位置；失败时，返回lo - 1
template<typename T>
int my_vector<T>::find(const T& elem, int lo, int hi) const
{
    BOOST_ASSERT_MSG( (lo>=0)&&(hi<=d_size),"输入了非法的查找范围!");
    //从后向前，顺序查找,这里的(lo<hi--)是先判断再自减,d_data[hi]取的是自减后的元素
    //这就保证了,开始时,判断时hi为size,而取值时hi就成了size-1,所以取值不会越界
    //当遍历到hi=1时,则取d_data[0]进行判断,若仍未找到,再次循环,此时hi=0,不符和判定条件,跳出循环
    //跳出循环前,会执行hi--的操作,故跳出循环后,hi成了-1.
    //等价于:
    //      --hi;
    //      for(;(lo<hi)&&(elem!=d_data[hi]); --hi)
    while((lo < hi--) && (elem != d_data[hi]));
    return hi;//若hi < lo，则意味着失败；否则hi即命中元素的秩
}

template<typename T>
int my_vector<T>::remove(int lo, int hi)
{
    BOOST_ASSERT_MSG( (0 <= lo) && (hi <= d_size), "输入了非法的删除范围!");
    if(lo = hi) return 0;
    while(hi<d_size) d_data[lo++] = d_data[hi++];
    d_size = lo;
    shrink();
    return hi - lo;
}

template<typename T>
T my_vector<T>::remove(int rank)
{
    BOOST_ASSERT_MSG( (0 <= rank) && (rank <= d_size), "输入了非法的删除范围!");
    T tmp = d_data[rank];
    remove(rank,++rank); 
    return tmp;
}


template<typename T>
void my_vector<T>::insert(const T& elem, int rank) 
{
    expand();
    int i = rank;
    while(i<d_size) d_data[i+1] = d_data[i];
    d_data[rank] = elem;
}


// 二分查找算法（版本C）：在有序向量的区间[lo, hi)内查找元素e，0 <= lo <= hi <= _size
//看源码Vector_search_binary_C.h,视频02-D4-4,5 二分改
//       _____________________________________________________
//       |___________|___________|_|_____________|___________|   
//          <=e      lo          mi              hi    >e
// 保持lo左边的元素(最大的是lo-1)均<=e, hi右边的元素均>e,总是大于e的最小者
//                                |                               |
//                                V
//       _____________________________________________________
//       |_______________________|_|__________________________|   
//                                 lo = mi = hi
//                          返回lo-1              
template<typename T>
int my_vector<T>::search(const T& elem, int lo, int hi) const
{
    BOOST_ASSERT_MSG( (0 <= lo) && (hi <= d_size), "输入了非法的删除范围!");
    while(lo < hi) //每步迭代仅需做一次比较判断，有两个分支
    {
        int mi = (lo + hi) >> 1;//以中点为轴点
        (elem < d_data[mi])? lo = mi + 1 : hi = mi;//经比较后确定深入[lo, mi)或(mi, hi) 注意此时忽略了mi
    }
    return --lo;//循环结束时，lo=mi=hi为大于e的元素的最小秩，故lo-1即不大于e的元素的最大秩
}//有多个命中元素时，总能保证返回秩最大者；查找失败时，能够返回失败的位置

//有序向量剔除算法,高效版
template<typename T>
int my_vector<T>::uniquify()
{
    if(d_size <= 0) return;
    bubble_sort();
    int lo = 0, hi = 0;
    while(++hi < d_size) //逐一扫描，直至末元素
        if (d_data[lo] != d_data[hi]) d_data[lo++] = d_data[hi];//跳过雷同者,发现不同元素时，向前移至紧邻于前者右侧
    d_size = lo;
    shrink();
    return d_size;
}

template<typename T>
void my_vector<T>::bubble_sort()
{
    if (d_size <= 0) return;
    int last = size-1;
    int hi = size-1;
    while((hi=last)>0)
    {
        last = 0;
        for (int i = 0; i < hi; ++i)
        {
            if (d_data[i+1]<d_data[i])
            {
                std::swap(d_data[i+1],d_data[i]);
                last = i;
            }
        }
    }
}


template<typename T>
int my_vector<T>::uniquify_unsort()
{
    if(d_size<=0) return;
    int old_size = d_size;
    for (int i = 1; i < size; ++i)
        (find(d_data[i], 0, i) < 0)? ++i : remove(i);
    return old_size - size;
}

template<typename T>
void my_vector<T>::merge(int lo, int mi, int hi)
{
    int len_B = mi-lo;
    int len_C = hi-mi;
    int B[len_B];
    std::copy_n(d_data, mi-lo, B);
    //两个子序列有一个还有未合并元素,就要执行合并,直至两序列都合并完毕
    for(int a=lo,b=0; (b<len_B)||(mi<hi); )
    {
        if ( (b<len_B) && ( (hi < mi)  || (B[b] <= d_data[mi]) ) ) d_data[a++] = B[b++];
        if ( (mi<hi)   && ( (len_B < b)|| (d_data[mi] < B[b])  ) ) d_data[a++] = d_data[mi++];
    }

}


template<typename T>
void my_vector<T>::merge_sort(int lo, int hi)
{
    BOOST_ASSERT_MSG( (0 <= lo) && (hi <= d_size), "输入了非法的删除范围!");
    if ( (hi - lo) < 2 ) return;
    int mi = (lo + hi) >> 1;
    merge_sort(lo, mi);
    merge_sort(mi,hi);
    merge(lo, mi, hi);
    
}


template<typename T>
void my_vector<T>::traverse(void (*visit) (T&))
{
    BOOST_ASSERT_MSG(visit,"非法的函数指针!");
    for (int i = 0; i < d_size; ++i)
        visit(d_data[i]);
}

template<typename T> template<typename TVS>
void my_vector<T>::traverse(TVS& visit)
{
    for (int i = 0; i < d_size; ++i)
        visit(d_data[i]);
}




