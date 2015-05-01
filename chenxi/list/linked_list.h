#pragma once
#include <initializer_list>
#include <boost/assert.hpp>
#include "../include/public_lib.h"


using rank = int;
#define node_ptr node<T>*
#define boost_assert BOOST_ASSERT_MSG

template<typename T>
struct node 
{
    node() = default;
    node(T data, node_ptr pred = nullptr, node_ptr succ = nullptr)
        : d_data(data), d_pred(pred), d_succ(succ){}
    T& data() {return d_data;}
    node_ptr& pred()  {return d_pred;}
    node_ptr& succ()  {return d_succ;}
    node_ptr insert_pred(const T& t) noexcept;
    node_ptr insert_succ(const T& t) noexcept;
    node_ptr d_pred;
    node_ptr d_succ;
    T d_data;
};


template<typename T>
node_ptr node<T>::insert_pred(const T& elem) noexcept
{
    node_ptr tmp_ptr = new node<T>(elem,d_pred,this);//创建新节点
    d_pred->succ() = tmp_ptr;//将本节点之前的前继的后继改为新的节点
    d_pred = tmp_ptr;      //将本节点的前继改为新的节点
    return d_pred;
}

template<typename T>
node_ptr node<T>::insert_succ(const T& elem) noexcept
{
    
    node_ptr tmp_ptr = new node<T>(elem,this,d_succ);//创建新节点
    d_succ->pred() = tmp_ptr;//将本节点之前的后继的前继改为新的节点
    d_succ = tmp_ptr;//将本节点的后继改为新的节点
    return d_succ;
}

template<typename T>
class linked_list
{
public:
    linked_list() {initialize();}
    linked_list(int size, const T elem);
    //委派构造函数,必须把目标构造函数放在初始化列表里,编译器不允许在某个构造函数中调用其他构造函数
    //且初始化列表里不能再初始化成员变量了,成员变量的初始化应放在函数体内部.
    linked_list(const linked_list<T>& other):linked_list(other.first(),other.size()){}
    linked_list(node_ptr ptr, int num) {copy_list(ptr,num);}
    linked_list(const linked_list<T>& other, rank r, int num);
    linked_list(const std::initializer_list<T>& init_list);
    ~linked_list();
    void clear() noexcept;
    void initialize();
    node_ptr insert_before(node_ptr ptr, const T& elem);
    node_ptr insert_after(node_ptr ptr, const T& elem);
    node_ptr insert_first(const T& elem);
    node_ptr insert_last(const T& elem);
    int size() const {return d_size;}
    bool empty() const {return d_size <= 0;}
    bool valid_node(node_ptr p) const {return (p!=nullptr) && (p!=d_header) && (p!=d_trailor);}
    node_ptr first() const {return d_header->succ();} 
    node_ptr last() const {return d_trailor->pred();} 
    node_ptr get_ptr_by_rank(rank r) const; 
    T& operator[](rank r);
    template<typename VST> void traverse(VST& visit); 
    void traverse(void (*visit) (T&) ); 
    T remove(node_ptr ptr);
    T remove(rank r);
    node_ptr select_max(node_ptr ptr, int n) const;
    node_ptr select_max() const {return select_max(first(),d_size);}
    node_ptr find(T elem, int n, node_ptr ptr) const;
    node_ptr find(T elem) const {return find(elem, d_size, last());}
    node_ptr search(T elem, int n, node_ptr ptr) const;
    node_ptr search(T elem) const {return search(elem, d_size, last());}
    void select_sort();
    void insert_sort();
    int deduplicate();//无序去重
    int uniquify();//有序去重
    void reverse();//前后倒置
    int disordered() const;
    void merge(node_ptr& lhs_p, int lhs_num, const linked_list<T>& rhs_list, node_ptr rhs_p, int rhs_num);
    void merge(const linked_list<T>& rhs_list)
    { merge(first(),d_size,rhs_list, rhs_list.first(), rhs_list.size()); }
    void merge_sort(node_ptr p, int num);
    void merge_sort(node_ptr p, int num);
private:
    void copy_list(node_ptr ptr, int num);
    int d_size;
    node_ptr d_header;
    node_ptr d_trailor;
};

template<typename T>
void linked_list<T>::initialize()
{
    d_header  = new node<T>(0, nullptr,nullptr);
    d_trailor = new node<T>(0, nullptr,nullptr);
    d_header->succ() = d_trailor;
    d_trailor->pred() = d_header;
    d_size = 0;
}

template<typename T>
node_ptr linked_list<T>::insert_first(const T& elem)
{
    d_header->insert_succ(elem);
    ++d_size;
    return d_header->succ();
}

template<typename T>
node_ptr linked_list<T>::insert_last(const T& elem)
{
    d_trailor->insert_pred(elem);
    ++d_size;
    return d_trailor->pred();
}



template<typename T>
linked_list<T>::linked_list(int size, T elem)
{
    initialize();
    for (int i = 0; i < size; ++i)
        d_trailor->insert_pred(elem);
    d_size = size;
}


template<typename T>
void linked_list<T>::copy_list(node_ptr ptr, int num)
{
    initialize();
    if (ptr == nullptr || num == 0) return;
    for (int i = 0; i < num; ++i,ptr = ptr->succ())
    {
        if (ptr != nullptr) 
            insert_last(ptr->data());
        else 
            return;
    }
}

template<typename T>
linked_list<T>::linked_list(const linked_list<T>& other, rank r, int num)
{
    if (other.size() == 0 || num == 0) return;
    node_ptr ptr = other.first();
    for (int i = 0; i < r; ++i)   ptr = ptr->succ();
    copy_list(ptr, num);    

}

template<typename T>
linked_list<T>::linked_list(const std::initializer_list<T>& init_list)
{
    initialize();
    if (init_list.size() == 0) return;
    for (auto elem : init_list)
        insert_last(elem);
}

template<typename T>
linked_list<T>::~linked_list()
{
    clear();
    if (d_header != nullptr)  delete d_header;
    if (d_trailor != nullptr) delete d_trailor;
}

template<typename T>
void linked_list<T>::clear() noexcept
{
    if (d_header == nullptr) return;
    node_ptr ptr = first();
    while(ptr != d_trailor)
    {
        node_ptr tmp = ptr;
        ptr = ptr->succ();
        delete tmp;
    }
    d_size = 0;
}

template<typename T> template<typename VST> 
void linked_list<T>::traverse(VST& visit)
{
    node_ptr ptr = d_header;
    while((ptr=ptr->succ()) != d_trailor) visit(ptr->data()); 
} 

template<typename T>
void linked_list<T>::traverse(void (*visit) (T&))
{
    node_ptr ptr = d_header;
    int count = 0;
    while((ptr=ptr->succ()) != d_trailor) 
    {
        visit(ptr->data());
    } 
} 

template<typename T>
T& linked_list<T>::operator[](rank r)
{
    boost_assert(0<=r && r<=d_size,"输入的秩超出了有效范围!");
    node_ptr cur = d_header;
    while(0 <= r--) 
        cur = cur->succ();
    return cur->data();
}

template<typename T>
node_ptr linked_list<T>::get_ptr_by_rank(rank r) const
{
    boost_assert(0<=r && r<=d_size,"输入的秩超出了有效范围!");
    node_ptr cur = d_header;
    while(0 <= r--) cur = cur->succ();
    return cur;
}

template<typename T>
T linked_list<T>::remove(node_ptr ptr)
{
    boost_assert(valid_node(ptr),"输入的节点不存在!");
    T tmp = ptr->data();
    ptr->succ()->pred() = ptr->pred();
    ptr->pred()->succ() = ptr->succ();
    delete ptr;
    return tmp;
}

template<typename T>
T linked_list<T>::remove(rank r)
{
    node_ptr cur = d_header;
    while(0 <= r--) cur = cur->succ();
    return remove(cur);
}


template<typename T>
node_ptr linked_list<T>::select_max(node_ptr ptr, int n) const
{
    boost_assert(valid_node(ptr),"输入的节点不存在!");
    node_ptr max_ptr = ptr;
    for (; (ptr!=d_trailor)&&(n>0); ptr=ptr->succ(),--n)
        if (max_ptr->data() <= ptr->data()) 
            max_ptr = ptr;
    return max_ptr;
}

template<typename T>
node_ptr linked_list<T>::find(T elem, int n, node_ptr ptr) const
{
    boost_assert(valid_node(ptr),"输入的节点不存在!");
    for (; (ptr!=d_header)&&(n>0); ptr=ptr->pred(),--n)
        if (ptr->data() == elem) return ptr;
    return nullptr;
}

template<typename T>
node_ptr linked_list<T>::search(T elem, int n, node_ptr ptr) const
{
    boost_assert(valid_node(ptr),"输入的节点不存在!");
    for (; (ptr!=d_header)&&(n>0); ptr=ptr->pred(),--n)
        if (ptr->data() <= elem) return ptr;
    return ptr;
}


template<typename T>
void linked_list<T>::select_sort()
{
    node_ptr max_ptr = last();
    node_ptr cur = last();
    for (int i=d_size; i>1; cur=cur->pred(), --i)
    {
        max_ptr = select_max(first(),i);
        std::swap(max_ptr->data(),cur->data());
    }
}

template<typename T>
node_ptr linked_list<T>::insert_before(node_ptr ptr, const T& elem)
{
    return ptr->insert_pred(elem);
}

template<typename T>
node_ptr linked_list<T>::insert_after(node_ptr ptr, const T& elem)
{
    return ptr->insert_succ(elem);
}


template<typename T>
void linked_list<T>::insert_sort()
{
    node_ptr cur = first()->succ();//从第二个节点开始
    for (int i = 1; i < d_size; ++i)
    {
        search(cur->data(),i,cur->pred())->insert_succ(cur->data());
        node_ptr tmp = cur;
        cur = cur->succ();
        remove(tmp);
    }
}

template<typename T>
int linked_list<T>::deduplicate()
{
    int count = 0;
    node_ptr cur = first()->succ();
    for(int i =1; i<d_size; ++i,cur=cur->succ())
    {
        node_ptr found_pos = find(cur->data(),i,cur->pred());
        if (found_pos != nullptr)
        {
            remove(found_pos);
            ++count;
        }
    }
    return count;
}

template<typename T>
int linked_list<T>::uniquify()
{
    node_ptr cur = first();
    int count = 0;
    while((cur=cur->succ()) != d_trailor)
    {
        if(cur->data() == cur->pred()->data())
        {
            remove(cur->pred()); 
            ++count;
        }
    }
    return count;
}

template<typename T>
void linked_list<T>::reverse()
{
    node_ptr cur = first();
    node_ptr first_ptr = first();
    node_ptr last_ptr  = last();
    while((cur=cur->succ()) != nullptr)
        std::swap(cur->pred()->succ(),cur->pred()->pred());
    node_ptr tmp = d_header;
    d_header = d_trailor;
    d_header ->succ() = last_ptr;
    d_header ->pred() = nullptr;
    d_trailor = tmp;
    d_trailor->succ() = nullptr;
    d_trailor->pred() = first_ptr;
}


template<typename T>
int linked_list<T>::disordered() const
{
    node_ptr cur = first()->succ();
    int count = 0;
    while((cur=cur->succ()) != d_trailor)
        if(cur->pred()->data() > cur->data())  ++count;
    return count;
}


template<typename T>
void linked_list<T>::merge(node_ptr& lhs_p, int lhs_num, 
    const linked_list<T>& rhs_list, node_ptr rhs_p, int rhs_num)
{
    node_ptr tmp = lhs_p->pred();
    for(; (rhs_p!=rhs_list.last()->succ()) && (0<rhs_num--); rhs_p = rhs_p->succ() )
    {
        if (rhs_p->data() <= lhs_p->data())
            lhs_p->insert_pred(rhs_p->data());
        else
        {
            while( (lhs_p != d_trailor) && (0 < lhs_num) && (lhs_p->data() < rhs_p->data()) )
            {
                lhs_p=lhs_p->succ();
                --lhs_num;
            }
            lhs_p->insert_pred(rhs_p->data());
        }
    }
    lhs_p = tmp->succ();
}

template<typename T>
void linked_list<T>::merge_sort(node_ptr p, int num)
{
    
    boost_assert(valid_node(p),"输入的节点不存在!");
    if(num < 2) return;
    node_ptr tmp_ptr = p;
    node_ptr half_ptr = nullptr;
    int half = num/2;
    for (int i = 0; i < num; ++i)
    {
        tmp_ptr = tmp_ptr->succ();
        if(i == half) half_ptr = tmp_ptr;
        boost_assert(tmp_ptr != d_trailor,"该链表没有那么多节点!");
    }
    merge_sort(p, half);
    merge_sort(half_ptr, num-half);
    merge(p,half,*this,half_ptr,num-half);

}

