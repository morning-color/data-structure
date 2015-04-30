#include "my_vector.h"



int main()
{
    my_vector<int> vec = {1,8,6,2,9,3,8};
    vec.traverse(print_elem);
    return 0;
}