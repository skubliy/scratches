#include <iostream>
//#include <conio.h>
#include <string>
#include <vector>

using namespace std;

class IoBuffer{ int x; };
IoBuffer* create_IoBuffer(){ return new IoBuffer;}
void delete_IoBuffer(IoBuffer* p){ if (p) delete p;}
void f1(IoBuffer* p){ delete_IoBuffer(p); }

typedef IoBuffer OwnedIoBuffer;
struct C1 {
    OwnedIoBuffer* buffer;
};

void f2(OwnedIoBuffer* p){ delete_IoBuffer(p); }

//template<typename T> owner = IoBuffer*;


template<typename T> owner = T;

template <typename T>
struct C2 {
    owner<T*> buffer;
};

int main()
{
//template<typename T> owner = T;
/*
template<typename T>
void f_new(owner<T*> p){
    delete_IoBuffer(p);
}*/

    IoBuffer* ptr = create_IoBuffer();
    f1(ptr);

    C1 mc1;
    mc1.buffer = create_IoBuffer();
    f2(mc1.buffer);

    //getch();
    return 0;
}
