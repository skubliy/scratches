#include <boost/foreach.hpp>
//#include <conio.h>
#include <string>
#include <iostream>
#include <vector>
#define foreach BOOST_FOREACH

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

template <typename T>
struct C2 {
  owner<T*> buffer;
};




int main()
{
    std::vector<int> v = {0, 1, 2, 3, 4, 5};


template<typename T> owner = T;
/*
template<typename T>
void f_new(owner<T*> p){
    delete_IoBuffer(p);
}*/

int main()
{
    IoBuffer* ptr = create_IoBuffer();
    f1(ptr);

    C1 mc1;
    mc1.buffer = create_IoBuffer();
    f2(mc1.buffer);


    string str("Amazing syntax of \"foreach\"!");
    foreach(char c,str)
    {
       cout << c;
    }
    //getch();
    return 0;
}
