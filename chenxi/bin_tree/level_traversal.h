#pragma once
#include "bin_tree.h"
#include <deque>

template<typename T, typename VST>
void level_trave(node_ptr ptr, VST visit)
{
    std::deque<node_ptr> que;
    que.push_back(ptr);
    while(ptr = que.pop())
    {
        visit(ptr->d_data);
        if(ptr->d_lc) que.push_back(ptr->d_lc);
        if(ptr->d_rc) que.push_back(ptr->d_rc);
    }
}
