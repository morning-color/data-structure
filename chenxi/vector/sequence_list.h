//创建了一个类,实现sequence_list的基本功能
//
//在使用以.h,.cpp分离实现模板类时,不能像使用普通类一样只简单的包涵.h头文件,
//应该在使用模板类的cpp文件中引入模板类相应的cpp文件,否则会找不到其成员函数的实现代码.
//

#include <stdexcept>
#include <algorithm>

typedef bool Status;
static const bool OK = true, ERROR = false;  

template<typename T>
bool compare(const T a, const T b)
{
    return a==b;
}

enum 
{
    INIT_STORAGE = 40,
};

template<typename T>
struct sequence_list
{
    T* start;     /* 存储空间基址 */
    int length;   /* 当前长度 */
    int storage; /* 当前分配的存储容量(以sizeof(ElemType)为单位) */
};

/* 操作结果：构造一个空的顺序线性表 */
template<typename T>
Status init_list(sequence_list<T>& list) 
{
    if (list.start != nullptr)  destroy_list(list);
    list.start =  new T [INIT_STORAGE];
    if(list.start == nullptr) throw std::overflow_error("list 内存分配失败\n");
    list.length = 0;
    list.storage = INIT_STORAGE;
    return OK; 
}

/* 操作结果：销毁顺序线性表L. 初始条件：顺序线性表L已存在。 */
template<typename T>
Status destroy_list(sequence_list<T>& list) 
{
    if (list.start == nullptr) return;
    delete [] list.start;
    list.start = nullptr;
    list.length = 0;
    list.storage =0;
    return OK;
}

/* 初始条件：顺序线性表L已存在。操作结果：将L重置为空表 */
template<typename T>
sequence_list<T>& clear_list(sequence_list<T>& list) 
{
    if (list.start == nullptr) return;
    list.length = 0;
    return list;
}

/* 初始条件：顺序线性表L已存在。操作结果：若L为空表，则返回TRUE，否则返回FALSE */
template<typename T>
bool list_empty(sequence_list<T>& list) 
{
    if (list.start == nullptr) throw std::runtime_error("顺序链表不存在\n");
    return (list.length == 0);
}

/* 初始条件：顺序线性表L已存在。操作结果：返回L中数据元素个数 */
template<typename T>
size_t length(sequence_list<T>& list) 
{
    if (list.start == nullptr) throw std::runtime_error("顺序链表不存在\n");
    return list.length;
}


/* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/* 操作结果：用e返回L中第i个数据元素的值 */
template<typename T>
Status get_elem(sequence_list<T>& list, size_t i, T& e) 
{
    if (list.start == nullptr) throw std::runtime_error("顺序链表不存在\n");
    if (i>list.length || i<1) throw std::invalid_argument("i 超过了链表的长度\n");
    e = *(list.start+i-1);
    return OK;
}

/* 初始条件：顺序线性表L已存在，equal()是数据元素判定函数(满足为1,否则为0) */
/* 操作结果：返回L中第1个与arg满足关系equal()的数据元素的位序。 */
/*          若这样的数据元素不存在，则返回值为0。算法2.6 */
template<typename T>
size_t locate_elem(sequence_list<T>& list, T arg, bool (*equal)(const T, const T)) //学习函数指针
{
    if (list.start == nullptr) throw std::runtime_error("顺序链表不存在\n");
    if (list.length == 0) throw std::runtime_error("链表未初始化\n");
    
    // for (int i = 0; i < list.length -1; ++i)
    // {
    //     bool flag = equal(arg, *(list.start+i));
    //     if (flag) return i+1;
    // }
    
    // 替代上面for循环的简单办法:
    int i = 0;
    while(i<=list.length && !equal(arg,*list.start+i) ) //是否可用*start++来代替???????
        ++i;
    if (i <= list.length)
        return i;
    else
        return 0;
}

/* 初始条件：顺序线性表L已存在 */
/* 操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱， */
/*          否则操作失败，pre_e无定义 */
template<typename T>
T prior_elem(const sequence_list<T>& list, T cur_elem, T& pre_elem, bool (*equal)(const T, const T) ) 
{
    if(list.start == nullptr) throw std::runtime_error("该链表不存在\n");
    if(list.length == 0) throw std::runtime_error("该链表未初始化\n");
    for (int i = 1; i < list.length-1; ++i)
    {
        bool flag = equal(cur_elem, list.start+i);
        if (flag) 
            pre_elem = *(list.start+i-1);

    }
    return 0;
}

/* 初始条件：顺序线性表L已存在 */
/* 操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继， */
/*           否则操作失败，next_e无定义 */
template<typename T>
T next_elem(const sequence_list<T>& list, T cur_elem, bool (*equal)(const T, const T) ) 
{
    if(list.start == nullptr) throw std::runtime_error("该链表不存在\n");
    if(list.length == 0) throw std::runtime_error("该链表未初始化\n");
    for (int i = 0; i < list.length-2; ++i)
    {
        bool flag = equal(cur_elem, list.start+i);
        if (flag) return *(list.start+i+1);
    }
    return 0;
}
/* 初始条件：顺序线性表L已存在，1≤n≤ListLength(L)+1 */
/* 操作结果：在L中第n个位置之前插入新的数据元素e，L的长度加1 */
template<typename T>
Status insert_elem(sequence_list<T>& list, const T elem, const size_t n)
{
    if(n==0 || n>list.length+1) throw std::invalid_argument("插入位置错误!\n");
    if(list.start == nullptr) throw std::runtime_error("该链表不存在\n");
    if(list.length == 0) throw std::runtime_error("该链表未初始化\n");
    if((list.length+1) < list.storage)
    {
        for (int i = n-1; i < list.length; ++i)
            *(list.start+i+1) = *(list.start+i); 
        *(list.start+n-1) = elem;
    }
    else
    {
        T *p = list.start;
        list.start = new T[list.storage+10];
        if(list.start == nullptr) throw std::overflow_error("list 内存分配失败\n");
        std::copy(p+n-1, p+list.length-1, list.start+n);
        std::copy(p, p+n-2, list.start);
        *(list.start+n-1) = elem;  
        delete [] p;
    }
    ++list.length;    
    list.storage += 10;
    return OK;    
} 

/* 初始条件：顺序线性表L已存在，1≤i≤ListLength(L) */
/* 操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减1 */
template<typename T>
Status delete_elem(sequence_list<T>& list, const size_t n, T& elem) 
{
    if(n==0 || n>list.length) throw std::invalid_argument("该位置不存在!\n");
    if(list.start == nullptr) throw std::runtime_error("该链表不存在\n");
    if(list.length == 0) throw std::runtime_error("该链表未初始化\n");
    elem = *(list.start+n-1);
    std::copy(list.start+n, list.start+list.length-1, list.start+n-1);
    --list.length;
    --list.storage; 
    return OK;
}

/* 初始条件：顺序线性表L已存在 */
/* 操作结果：依次对L的每个数据元素调用函数vi()。一旦vi()失败，则操作失败 */
/*           vi()的形参加'&'，表明可通过调用vi()改变元素的值 */
template<typename T>
Status traverse_list(sequence_list<T>& list, void(*vi)(T&))
{
    if(list.start == nullptr) throw std::runtime_error("该链表不存在\n");
    for (int i = 0; i < list.length-1; ++i)
        vi( *(list.start++) );
    return OK;
}