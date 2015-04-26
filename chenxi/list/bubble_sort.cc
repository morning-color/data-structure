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
	//flag在初始条件下和排序未完成时,均为false,取反得true,使得程序继续循环.且每次循环初始时,flag=true.
	//若某一次,排序已完成,则不会进入if的内部,flag在本次循环未发生改变,保持为ture
	//下次进入循环时,flag取反后为false,则退出循环,排序完成.
	for (bool flag = false; flag = !flag; --len)
	{
		for (int j = 0; j < len -1; ++j)
		{
			if (arr[j]>arr[j+1])//只要进入过if内部,则说明排序未完成,flag = false;
			{
				std::swap(arr[j],arr[j+1]);
				flag = false;
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