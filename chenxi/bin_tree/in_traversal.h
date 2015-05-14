#pragma once
#include "bin_node.h"
#include <stack>

template<typename T, typename VST>
void rec_inorder_trav(node_ptr ptr, VST visit)
{
    if(!ptr) return;
    rec_inorder_trav(ptr->d_lc, visit);
    visit(ptr->d_data);
    rec_inorder_trav(ptr->d_rc, visit);
}

template<typename T>
static void go_along_left(node_ptr ptr, stack<node_ptr>& s)
{
    while(ptr)
    {
        s.push(ptr->d_rc);
        ptr = ptr->d_lc;
    }
}

template<typename T, typename VST>
void ite_inorder_trav(node_ptr ptr, VST visit)
{
    stack<node_ptr> s;
    while(true)
    {
        go_along_left(ptr, s, visit);
        if (s.empty()) break;
        ptr = s.pop();
        visit(ptr->d_data);
        ptr = ptr->d_rc;
    }
}


template<typename T, typename VST>
void ite_inorder_trav2(node_ptr ptr, VST visit)
{
    std::stack<node_ptr> s;
    s.push(ptr);
    while(!s.empty())
    {
        if(ptr) //检测该节点是否存在（除了第一次循环，该节点始终为上次外循环节点的右孩子） 
            while(ptr = ptr->d_lc) s.push(ptr); //一直沿左分支下潜，将沿途节点推入栈中，后续将遍历这些节点的右子树
        //每次循环
        ptr = s.pop();  //自下而上，取出栈中各节点
        visit(ptr->d_data); //访问该节点
        ptr = ptr->d_rc;    //进入该节点的右子树
    }
}




template<typename T, typename VST>
void ite_inorder_trav3(node_ptr ptr, VST visit)
{
    bool trace_back = false;
    while(true)
    {
        if(has_lc(ptr) && !trace_back) //有左子树，并且不是刚刚回溯
            ptr = ptr->d_lc;    //沿左分支下潜
        else //无左子树或刚刚回溯（也就是左子树已遍历完毕）
        {
            if(has_rc(ptr)) //有右子树
            {
                ptr = ptr->d_rc;    //转移至右子树
                trace_back = false;
            }
            else//左子树已遍历完毕，且无右子树，当回溯
            {
                if(!ptr=ptr->succ()) return;//无后续节点，遍历完成
                trace_back = true;//上一行刚刚做了回溯，将标识置true
            }
        }        
    }    

}


