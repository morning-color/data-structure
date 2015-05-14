#pragma once
#include "bin_node.h"
#include "../stack/stack.h"

//从当前节点出发，沿左分支不断深入，直至没有左分支的节点；沿途节点遇到后立即访问
template<typename T, typename VST> 
static void visit_along_left(node_ptr ptr, stack<node_ptr>& s, VST& visit) 
{
  while(ptr)
  {
    visit(ptr);
    s.push(ptr->d_rc);
    ptr = ptr->d_lc;
  }
}


//子树先序遍历递归版
template<typename T, typename VST> 
void rec_preorder_trav(node_ptr ptr,VST& visit) 
{
  if(ptr == nullptr) return;
  visit(ptr->d_data);
  rec_pre_order_trav(ptr->d_lc,visit);
  rec_pre_order_trav(ptr->d_rc,visit);
}

//子树先序遍历迭代版
template<typename T, typename VST> 
void ite_preorder_trav(node_ptr ptr,VST& visit) 
{
  stack<node_ptr> s;
  while(true)
  {
    visit_along_left(ptr, s, visit);
    if(s.empty()) break;
    ptr = s.pop();
  }
}



//子树先序遍历
template<typename T> template<typename VST> 
void bin_node<T>::pre_order_trav(VST& visit)
{
  rec_preorder_trav(this, visit);
} 

