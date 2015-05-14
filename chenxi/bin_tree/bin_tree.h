#pragma once
#include "bin_node.h"
#include <memory>
#define tree_ptr bin_tree<T>*

template <typename T>
class bin_tree
{
  typedef std::shared_ptr<bin_tree<T>> sptr;
public:
  static sptr make() {return std::make_shared<bin_tree<T>>();}
  int size() const {return d_size;}
  bool empty() const {return (d_size == 0);}
  node_ptr root() const {return d_root;}
  node_ptr insert_as_root(T data);
  node_ptr insert_as_lc(node_ptr ptr, T data);
  node_ptr insert_as_rc(node_ptr ptr, T data);
  node_ptr attach_as_lc(node_ptr ptr, tree_ptr& tree);
  node_ptr attach_as_rc(node_ptr ptr, tree_ptr& tree);
  int remove(node_ptr ptr);
  tree_ptr secede(node_ptr ptr);
  template<typename VST> 
  void pre_trav(VST visit){d_root->pre_order_trav(visit);}
protected:
  bin_tree();
  ~bin_tree();
  int d_size {0};
  node_ptr d_root {nullptr};
  virtual int update_height(node_ptr ptr);
  void update_height_above(node_ptr ptr);

};


template<typename T>
node_ptr bin_tree<T>::insert_as_root(T data)
{
  node_ptr tmp = d_root;
  d_root = new bin_node<T>(data,nullptr,tmp);
  tmp->d_parent = d_root;
  ++d_size;
  update_height_above(tmp);
  return d_root;
}

template<typename T>
node_ptr bin_tree<T>::insert_as_lc(node_ptr ptr, T data)
{
  node_ptr tmp = ptr->insert_as_lc(data);
  ++size;
  return tmp;
}

template<typename T>
node_ptr bin_tree<T>::insert_as_rc(node_ptr ptr, T data)
{
  node_ptr tmp = ptr->insert_as_rc(data);
  ++size;
  return tmp;
}  


template<typename T>
node_ptr bin_tree<T>::attach_as_lc(node_ptr ptr, tree_ptr& tree)
{
  if ( ptr->d_lc = tree->d_root ) ptr->d_lc->d_parent = ptr; //接入
  size += tree->d_size;
  update_height_above(ptr);
  tree->d_root = nullptr;
  tree->d_size = 0;
  tree.reset();
  return ptr;
}

template<typename T>
node_ptr bin_tree<T>::attach_as_rc(node_ptr ptr, tree_ptr& tree)
{
  if ( ptr->d_rc = tree->d_root ) ptr->d_rc->d_parent = ptr; //接入
  size += tree->d_size;
  update_height_above(ptr);
  tree.d_root = nullptr;
  tree->d_size = 0;
  tree.reset();
  return ptr;
}


