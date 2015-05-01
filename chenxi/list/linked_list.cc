#include <iostream>
#include "linked_list.h"

int main(int argc, char const *argv[])
{
    linked_list<int> list {10,8,9,3,5,4,3,10,5,8,7};
    linked_list<int> rlist {5,8,3,4,97,2,-1};
    // list.reverse();
    // PRINT_IT(list.disordered());
    // linked_list<int> list2(list.first(),4);
    // linked_list<int> list3(4,5);
    // linked_list<int> list4(list);
    // linked_list<int> list5(list,0,4);
    // puts("object traverse 5:");
    // list5.traverse(print_elem);
    // puts("");
    // puts("object traverse:");
    // puts("function traverse:");
    // list2.traverse(print_func);
    // puts("");
    // list3.traverse(print_elem);
    // puts("");
    // PRINT_IT(list[3]);
    // PRINT_IT(list[2]);
    // PRINT_IT(list[1]);
    // list.remove(3);
    // list.traverse(print_elem);
    // puts("");
    // list.remove(0);
    // list.traverse(print_elem);
    // puts("");
    // list.remove(1);
    // list.traverse(print_elem);
    // puts("");
    // PRINT_IT(list.select_max(list.first(),2)->data());
    // PRINT_IT(list.select_max()->data());
    // PRINT_IT(list.find(7)->data());
    // PRINT_IT(list.find(7,2,list.first()->succ())->data());
    // if(!list.find(9,2,list.last())) puts("未找到!");

    // rlist.select_sort();
    list.insert_sort();
    // PRINT_IT(list.search(7)->data());
    // list.deduplicate();
    // list.traverse(print_elem);
    // puts("\n+++++++++++++++++++");
    // rlist.traverse(print_elem);
    // puts("\n------------------");
    // list.uniquify();
    // PRINT_IT(list.disordered());
    auto ptr = list.first();
    // list.merge_sort(ptr,list.size());
    list.merge(ptr,11,rlist,rlist.first(),6);
    list.traverse(print_elem);
    puts("");
    return 0;
}
