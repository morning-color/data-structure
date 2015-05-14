#pragma once
#include "bin_node.h"
#include <stack>

template<typename T, typename VST>
void rec_postorder_trav(node_ptr ptr, VST visit)
{
    if (ptr == nullptr) return;
    rec_post_trav(ptr->d_lc);
    rec_post_trav(ptr->d_rc);
    visit(ptr->d_data);
}


template<typename T>
void find_deepest_leaf_in_left_branch(node_ptr ptr, std::stack<node_ptr>& s)//该叶节点本身可能是左孩子也可能是右孩子
{
    while(true)
    {
        if(ptr->d_lc)    //尽可能沿左分支下潜
        {
            if (ptr->d_rc) s.push(ptr->d_rc);   //若存在右孩子，则先入栈
            s.push(ptr = ptr->d_lc);    //转入该节点的左子树
        }          
        else if(ptr->d_rc) s.push(ptr->d_rc);//不存在左孩子了，再检测是否有右孩子，若有，则转移到当前节点的右子树 
        else return;//既无做孩子，又无又孩子，则到达左分支最深叶节点
    }
}


template<typename T, typename VST>
void ite_postorder_trav(node_ptr ptr, VST visit)
{
    stack<node_ptr> s;
    if(ptr) s.push(ptr);//根节点入栈
    while(!s.empty())
    {
        if(s.top() != ptr->d_parent)   //若栈顶非当前节点的父节点，则必为其右兄节点 
            find_deepest_leaf_in_left_branch(ptr,s);
        ptr = s.pop();
        visit(ptr->d_data);
    }


}

