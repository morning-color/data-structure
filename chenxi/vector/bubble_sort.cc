#include <iostream>
#include "../include/public_lib.h"

//通常情况下,函数是无法传递长度不确定的数组的.
//在这里,利用模板可构造出数组引用传递的函数,并且可通过sizeof(arr)/sizeof(arr[0])获取其长度,而不用再传递一个len进来
//若将函数定义为bubble_sort(int arr[]),则无法获取arr的长度,而且效率较低;
//这时sizeof(arr)为8,因为arr是个指针.
//以后要注意使用这种利用模板获取数据类型及其所占内存大小的技巧.
//对于模板函数来说,可以直接声明其形参为T&&,因为按照C++11引用折叠规则,调用该函数时的实际引用类型完全取决于实参的引用类型
//但非模板函数不能这样,因为其无法做引用折叠,声明为右值引用,则其实参必须是右值,不能是左值.
template<typename T>
void bubble_sort(T&& arr)
{
    int len = sizeof(arr) / sizeof(arr[0]);
    size_t last = len-1, hi = len-1;
    //last 记录了上一次循环存在交换操作的最大rank,那么rank之后的元素已经是有序的了
    //这一次循环只需要对[0,rank] 之间的元素进行排序就够了
    for (; 0<hi; hi=last) 
    {
        last = 0;
        //这里很重要,将last重置为0,若下面没有交换操作,本次外循环结束时,执行hi=last=0;下次循环就直接跳出了.
        //否则,当本次外循环不发生交换操作时,由于last没有重置,会使last一直保持当前值,程序无法正确跳出,一直循环下去.
        for (int j = 0; j < hi; ++j)//由于下面交换操作的rank是j和j+1,所以这里j<hi就可以了,j+1会保证rank为hi的元素得到检测.
        {
            if (arr[j+1] < arr[j])//这里注意内循环结束后,保证[j+1,len)之间的元素是有序的,下次循环只检测[0,j]就可以了
            {
                std::swap(arr[j],arr[j+1]);
                last = j;
            }
        }
    }        
}


int main(int argc, char const *argv[])
{
    int arr[10] = {2,5,9,7,3,8,3,4,5,6};
    PRINT_IT(sizeof(arr));
    PRINT_IT(sizeof(arr[0]));
    bubble_sort(arr);

    for(const auto& elem : arr)
        PRINT_IT(elem); 
    return 0;
}