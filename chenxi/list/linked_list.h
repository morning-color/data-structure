#pragma once
#include <initializer_list>
#include <boost/assert.hpp>


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
    node_ptr insert_pred(T t) noexcept;
    node_ptr insert_succ(T t) noexcept;
    node_ptr d_pred;
    node_ptr d_succ;
    T d_data;
};


template<typename T>
node_ptr node<T>::insert_pred(const T elem) noexcept
{
    node_ptr tmp_ptr = new node<T>(elem,d_pred,this);//创建新节点
    d_pred->succ() = tmp_ptr;//将本节点之前的前继的后继改为新的节点
    d_pred = tmp_ptr;      //将本节点的前继改为新的节点
    return d_pred;
}

template<typename T>
node_ptr node<T>::insert_succ(const T elem) noexcept
{
    
    node_ptr tmp_ptr = new node<T>(elem,this,d_succ);//创建新节点
    d_succ->pred = tmp_ptr;//将本节点之前的后继的前继改为新的节点
    d_succ->tmp_ptr;//将本节点的后继改为新的节点
    return d_succ;
}

template<typename T>
class linked_list
{
public:
    linked_list() {initialize();}
    linked_list(int size, const T elem);
    linked_list(const linked_list<T>& other);
    linked_list(const node_ptr ptr, int num);
    linked_list(const linked_list<T>& other, rank r, int num);
    linked_list(const std::initializer_list<T> init_list);
    ~linked_list();
    void clear() noexcept;
    void initialize();
    int size() const {return d_size;}
    node_ptr header() const {return d_header;} 
    node_ptr trailor() const {return d_trailor;} 
    template<typename VST> void traverse(VST& visit); 
private:
    int d_size;
    node_ptr d_header;
    node_ptr d_trailor;
};

template<typename T>
void linked_list<T>::initialize()
{
    d_header  = new node<T>(0, nullptr,nullptr);
    d_trailor = new node<T>(0,d_header,nullptr);
    d_header->succ() = d_trailor;
    d_size = 0;
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
linked_list<T>::linked_list(const linked_list<T>& other)
{
    initialize();
    if (other.size()==0) return;
    node_ptr ptr = other.header();
    while((ptr = ptr->succ) != other.trailor())
        d_trailor->insert_pred(ptr->data());
    d_size = other.size();
}

template<typename T>
linked_list<T>::linked_list(const node_ptr ptr, int num)
{
    initialize();
    if (ptr == nullptr || num == 0) return;
    for (int i = 0; i < num; ++i,ptr = ptr->succ())
    {
        if (ptr != nullptr) 
        {
            d_trailor->insert_pred(ptr->data());
            ++d_size;
        }
        else
            return;
    }
}

template<typename T>
linked_list<T>::linked_list(const linked_list<T>& other, rank r, int num)
{
    initialize();
    if (other.size() == 0 || num == 0) return;
    node_ptr ptr = other.header();
    for (int i = 0; i < r; ++i)   ptr = ptr->succ();
    for (int i = 0; i < num; ++i, ptr = ptr->succ())
    {
        if (ptr != nullptr) 
        {
            d_trailor->insert_pred(ptr->data());
            ++d_size;
        }
        else
            return;
    }

}

template<typename T>
linked_list<T>::linked_list(const std::initializer_list<T> init_list)
{
    initialize();
    if (init_list.size() == 0) return;
    for (auto elem : init_list)
    {
        d_trailor->insert_pred(elem);
        ++d_size;
    }
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
    node_ptr ptr = d_header;
    while(ptr != d_trailor)
    {
        node_ptr tmp = ptr->succ();
        delete ptr;
        ptr = tmp;
    }
    d_size = 0;
}

template<typename T> template<typename VST> 
void linked_list<T>::traverse(VST& visit)
{
    node_ptr ptr = d_header;
    while((ptr=ptr->succ()) != d_trailor) visit(ptr->data()); 
} 


