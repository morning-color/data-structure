#pragma once
#include <stdexcept>
#include "../include/public_lib.h"
#include "../stack/stack.h"

#define node_ptr bin_node<T>*
typedef enum {RED,BLACK} rb_color;


template<typename T>
struct bin_node
{

    bin_node()
    : d_parent(nullptr), 
      d_lc(nullptr), 
      d_rc(nullptr), 
      d_height(0), 
      d_npl(1), 
      d_color(RED) {}

    bin_node(const T& data, 
             node_ptr parent = nullptr, 
             node_ptr lc = nullptr, 
             node_ptr rc = nullptr, 
             int height = 0, 
             int npl = 1, 
             rb_color color = RED)
    : d_data(data), 
      d_parent(parent), 
      d_lc(lc), 
      d_rc(rc), 
      d_height(height), 
      d_npl(npl), 
      d_color(color) {}

    T data() const {return d_data;}
    int size() const {return d_size;}
    int height() const {return d_height;}
    int npl() const {return d_npl;}
    node_ptr parent() const {return d_parent;}
    node_ptr lc() const {return d_lc;}
    node_ptr rc() const {return d_rc;}
    node_ptr insert_as_lc(const T& data);
    node_ptr insert_as_rc(const T& data);
    node_ptr succ();//当前节点中序遍历的直接后继
    rb_color color() const {return d_color;}
    bool operator==(const bin_node& node) {return (d_data == node.d_data);} 
    bool operator<=(const bin_node& node) {return (d_data <= node.d_data);} 
    template<typename VST> void pre_order_trav(VST& visit); //子树先序遍历
    template<typename VST> void in_order_trav(VST& visit);  //子树中序遍历
    template<typename VST> void post_order_trav(VST& visit);//子树后序遍历
    template<typename VST> void level_trav(VST& visit);     //子树层次遍历

private:
    T d_data;
    node_ptr d_parent;
    node_ptr d_lc;   //left child
    node_ptr d_rc;   //left child
    int d_size;
    int d_height;
    int d_npl;
    rb_color d_color;
};

template<typename T> inline bool is_root(const node_ptr ptr)  {return !ptr->d_parent;}
template<typename T> inline bool is_lc(const node_ptr ptr)  {return (!is_root(ptr)) && (ptr == ptr->d_parent->d_lc);}
template<typename T> inline bool is_rc(const node_ptr ptr)  {return (!is_root(ptr)) && (ptr == ptr->d_parent->d_rc);}
template<typename T> inline bool has_lc(const node_ptr ptr)  {return ptr->d_lc;}
template<typename T> inline bool has_rc(const node_ptr ptr)  {return ptr->d_rc;}

template<typename T>
inline bool get_height(node_ptr ptr) {return (ptr)? ptr->height() : -1;}

template<typename T>
node_ptr bin_node<T>::insert_as_lc(const T& data)
{
  ASSERT_MSG(!d_lc,"原节点已存在左孩子！");
  return d_lc = new bin_node<T>(data, this);  //创建了一个以当前节点（this对象）为父亲的节点，并将其设为当前节点的左分支
}

template<typename T>
node_ptr bin_node<T>::insert_as_rc(const T& data)
{
  ASSERT_MSG(!d_rc,"原节点已存在右孩子！");
  return d_rc = new bin_node<T>(data, this);  //创建了一个以当前节点（this对象）为父亲的节点，并将其设为当前节点的左儿子
}

template<typename T>
node_ptr bin_node<T>::succ()
{
  node_ptr ptr = this;
  if(has_rc(ptr))//存在右子树，则直接后继为右子树的中序遍历起点 
    while(has_lc(ptr)) ptr = ptr->d_lc;
  else//若不存在右子树，则其直接后继为首个将该节点包含于其左子树的最低祖先
  {
    while(is_rc(ptr)) ptr = ptr->d_parent;//逆向地沿右分支不断上溯，直到该节点为为其父节点的左孩子
      ptr = ptr->d_parent;//再移动一步，抵达直接后继
  }
  return ptr;
}

#include "traversal.h"

