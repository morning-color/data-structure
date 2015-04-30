#ifndef MORNING_COLOR_LINKED_LIST_H
#define MORNING_COLOR_LINKED_LIST_H

//单链表线性表  linked_list
#include <iostream>
#include <stdexcept>
#include "public_lib.h"
//  前导声明，因为定义node类时不知道linked_list的含义，模板类需要模板声明
template<typename T>
class linked_list;

//链表的节点
template<typename T>
struct node 
{
friend linked_list<T>;
public:
    T get_data(){return data;}
    node<T> get_next(){return next;}
private:
    T data;
    node<T> *next;
};

//链表
template<typename T>
class linked_list
{
public:
    linked_list();
    linked_list(const linked_list<T>& source);
    ~linked_list();

    // access method
    // 若表空则返回 TRUE 否则返回 FALSE
    bool empty(void) const
    { return (head == tail); }
    
    // 返回表的长度
    size_t length(void) const
    { return size;}
    
    // 返回头指针
    node<T>* first(void) const
    { return head ;}
    
    // 返回尾指针
    node<T>* last(void) const
    { return tail; }
    
    // 改变当前结点为它的前驱,并返回新的当前结点的指针
    node<T>* prior(void);
    
    // 改变当前结点为它的后继,并返回新的当前结点的指针
    node<T>* behind(void);
    
    // 若 1≤pos≤表长,则返回指向第 pos 个元素所在的结点,并设它为
    // 当前位置
    node<T>* get_elem(size_t pos);
    
    // 若存在和 e 相等元素, 则返回它在表中的位序并移动当前指针指向该
    // 结点,前驱指针指向它的前驱; 否则返回 0 并指针不移动
    size_t locate(const T& elem);
    
    // modification method
    void reset(void);//恢复当前指针指向第一个结点
    
    void clear_list(void); // 将表清空
    
    
    // 在当前结点之后插入元素 e, 并改变当前指针指向新插入的结点
    void insert_after(const T& elem);

    // 在pos的位置插入元素 e, 改变当前指针为插入的节点
    void insert_elem(size_t pos, const T& elem);

    void modify_elem(size_t pos, const T& elem);
    // 在最后一个结点之后插入指针 s 所指一串结点,不改变当前指针
    void append(node<T> *s);

    // 删除当前指针所指的后继结点并返回指向它的指针, 若当前指针指向
    // 被删除的结点,则移动当前指针指向它的后继
    node<T>* del_next(void);

    // 删除元素值和 e 相等的结点, 若当前指针指向被删除的结点,
    //则移动当前指针指向它的后继
    void del_elem(const T& elem);

    void traverse(void (*vi)(T));
private:
    node<T> *head, *tail, *cur_ptr;
    size_t cur_pos, size;
};

template<typename T>
linked_list<T>::linked_list()
:cur_pos(1)
,size(1)
{
    head = new node<T>;
    if (head == nullptr)
        throw std::overflow_error("链表内存分配失败!\n");
    tail = head;
    cur_ptr = head;
}

template<typename T>
linked_list<T>::linked_list(const linked_list<T>& source)
{
    node<T> *scr_ptr = source.first();
    if (scr_ptr == nullptr )
        throw std::runtime_error("被拷贝链表不存在,拷贝构造失败!\n");
    linked_list();
    while (scr_ptr!=tail)
        insert_after(scr_ptr->behind());
}

template<typename T>
linked_list<T>::~linked_list()
{

    clear_list();
    delete head;
    head = nullptr;
}

template<typename T>
node<T>* linked_list<T>::prior(void)
{
    if(cur_ptr == head || cur_ptr == nullptr) 
        throw std::underflow_error("当前为链表的首个节点,不存在其前面节点\n");
    return get_elem(cur_pos-1);
}

template<typename T>
node<T>* linked_list<T>::behind(void)
{
    if(cur_ptr == nullptr) 
        throw std::underflow_error("当前为链表的首个节点,不存在其前面节点\n");
    cur_ptr = cur_ptr->next;
    ++cur_pos;
    return cur_ptr;
}

// 若 1≤pos≤表长,则返回指向第 pos 个元素所在的结点,并设它为
// 当前位置
template<typename T>
node<T>* linked_list<T>::get_elem(size_t pos)
{
    if(pos == 0 || pos > size) 
        throw std::domain_error("输入的位置不合法\n");
    if (cur_pos > pos) reset();
    while (cur_pos<pos) behind();
    return cur_ptr;
}


// 若存在和 e 相等元素, 则返回它在表中的位序并移动当前指针指向该
// 结点,前驱指针指向它的前驱; 否则返回 0 并指针不移动
template<typename T>
size_t  linked_list<T>::locate(const T& elem)
{
    node<T>* tmp_sptr = cur_ptr;
    size_t tmp_pos = cur_pos;
    reset();
    while(cur_ptr != nullptr && cur_ptr->data != elem)
        behind();
    if (cur_ptr == nullptr)
    {
        cur_pos = tmp_pos;
        cur_ptr = tmp_sptr;
        return 0;
    }
    else
        return cur_pos;
}

//恢复当前指针指向第一个结点
template<typename T>
void linked_list<T>::reset(void)
{
    cur_pos = 1;
    cur_ptr = head;
}

// 将表清空
template<typename T>
void linked_list<T>::clear_list(void)
{
    if(empty()) return;
    reset();
    while(cur_ptr != nullptr)
        del_next();
    reset();
}

// 在当前结点之后插入元素 e, 并改变当前指针指向新插入的结点
template<typename T>
void linked_list<T>::insert_after(const T& elem)
{
    node<T>* tmp_ptr = cur_ptr->next;
    cur_ptr->next = new node<T>;
    if(cur_ptr->next == nullptr)
        throw std::overflow_error("为新节点分配内存失败!\n");
    cur_ptr = cur_ptr->next;
    cur_ptr->data = elem;
    cur_ptr->next = tmp_ptr;
    ++cur_pos;
    if(cur_pos > size)
        tail = cur_ptr;
    ++size;
}


// 在pos的位置插入元素 e, 改变当前指针为插入的节点
template<typename T>
void linked_list<T>::insert_elem(size_t pos, const T& elem)
{
    if(pos<1 || pos > size) 
        throw std::domain_error("输入的位置不合法\n");

    if (pos == 1)//插入的位置为head,则需给head重新分配空间,并将改点连到原链表之前,
    {
        node<T>* origin_head = head;
        head = new node<T>;
        if (head == nullptr)
            throw std::overflow_error("为新节点分配内存失败!\n");
        head->next = origin_head;
        head->data = elem;
        reset();
    }
    else
    {
        get_elem(pos-1);
        node<T>* tmp_ptr = cur_ptr->next;
        cur_ptr->next = new node<T>;
        behind();
        cur_ptr->data = elem;
        cur_ptr->next = tmp_ptr;
    }
    ++size;
}

template<typename T>
void linked_list<T>::modify_elem(size_t pos, const T& data)
{
    get_elem(pos)->data = data;
}

// 在最后一个结点之后插入指针 s 所指一串结点,不改变当前指针
template<typename T>
void linked_list<T>::append(node<T> *s)
{
    if(s == nullptr) 
    {
        return;
    }
    tail->next = s;
    while(tail->next != nullptr)
    {
        tail = tail->next;
        ++size;
    }
}

// 删除当前指针所指的后继结点并返回指向它的指针, 若当前指针指向
// 被删除的结点,则移动当前指针指向它的后继
template<typename T>
node<T>* linked_list<T>::del_next(void)
{
    if(cur_ptr == tail) throw std::overflow_error("该节点为链表尾节点,无法删除后续节点!\n");
    node<T>* next_ptr = cur_ptr->next;
    node<T>* tmp_ptr = next_ptr->next;
    delete next_ptr;
    cur_ptr->next = tmp_ptr;
    behind();
    --size;
    return cur_ptr;
}


// 删除元素值和 e 相等的结点, 若当前指针指向被删除的结点,
//则移动当前指针指向它的后继
template<typename T>
void linked_list<T>::del_elem(const T& elem)
{

    node<T>* origin_ptr = cur_ptr;
    size_t   origin_pos = cur_pos;
    node<T>* tmp_ptr    = nullptr;
    node<T>* next_ptr   = nullptr;
    reset();
    while(cur_ptr != nullptr)
    {
        auto find_pos = locate(elem);
        if(find_pos == 0)
        {
            cur_pos = origin_pos;
            cur_ptr = origin_ptr;
            return;
        }
        tmp_ptr  = cur_ptr;    
        next_ptr = cur_ptr->next;
        if (find_pos == 1)
            head = next_ptr;
        else
        {
            prior();
            cur_ptr->next = next_ptr;
        } 
        delete tmp_ptr;
        behind();
        --size;
    }
}

template<typename T>
void linked_list<T>::traverse(void (*vi)(T))
{
    reset();
    while(cur_ptr != nullptr)
    {
        std::cout << "pos = " << cur_pos << "\t";
        vi(cur_ptr -> data);
        behind();
    }
    prior();//遍历完成之后,cur_ptr为nullptr,这样比较危险,需将当前指针转移至tail!!!

}

#endif
