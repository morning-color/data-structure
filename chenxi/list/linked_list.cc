#include <iostream>
#include "linked_list.h"
#include "../include/public_lib.h"

int main(int argc, char const *argv[])
{
    linked_list<int> list {0,1,2,3};
    list.traverse(print_elem);
    puts("");
    return 0;
}
