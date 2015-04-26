#include <iostream>
using namespace std;

class HasPtrMem {
public:
    HasPtrMem(): d(new int(3)) {
        cout << "Construct: " << ++n_cstr << endl; 
    }
#if 1
    HasPtrMem(const HasPtrMem & h): d(new int(*h.d)) {
        cout << "Copy construct: " << ++n_cptr << endl;
    }
#endif
    HasPtrMem(HasPtrMem && h): d(h.d) { // 移动构造函数
        h.d = nullptr;                  // 将临时值的指针成员置空
        cout << "Move construct: " << ++n_mvtr << endl;
    }
//在这里,下面的Const Move construct是不成立的,因为h为const, h.d=nullptr是非法的,
//但不执行这一句,将造成double free. 因为再退出main函数时,会执行a和h的析构函数,由于 h.d != nullptr
//所以会执行delete h.d  和 delete a.d, 出现double free的错误.
//所以对于包含指针的类的移动构造函数,不要把形参设为const,这将导致原对象的指针无法设为nullptr,
//造成hangling pointor 或 double free 
#if 0
    HasPtrMem(const HasPtrMem && h): d(h.d) { // 移动构造函数
       // h.d = nullptr;                  // 将临时值的指针成员置空
        cout << "Const Move construct: " << ++n_mvtr << endl;
    }
#endif
    ~HasPtrMem() { 
        delete d;
        cout << "Destruct: " << ++n_dstr << endl;
    }
    int * d;
    static int n_cstr;
    static int n_dstr;
    static int n_cptr;
    static int n_mvtr;
};

int HasPtrMem::n_cstr = 0;
int HasPtrMem::n_dstr = 0;
int HasPtrMem::n_cptr = 0;
int HasPtrMem::n_mvtr = 0;


#if 0
const HasPtrMem GetTemp() { 
    const HasPtrMem h;
    cout << "Resource from " <<  __func__ << ": " << hex << h.d << endl;
    return h;
}
#endif

//这里的返回值构造时,会调用move constructor:
HasPtrMem GetTemp() { 
    HasPtrMem h;
    cout << "Resource from " <<  __func__ << ": " << hex << h.d << endl;
    return h;
}


int main() {
    HasPtrMem a = GetTemp();
    cout << "Resource from " <<  __func__ << ": " << hex << a.d << endl;
}
