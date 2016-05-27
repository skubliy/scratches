#include <iostream>
#include <stdlib.h>
#include <sys/socket.h>
#include <atomic>
#include <string>
#include <list>

#include "ip_wrap.hpp"


//#include "dn_cache.h"

using namespace std;

typedef struct sockaddr sa;
typedef struct sockaddr_storage sas;


/*
class pseudo_atomic{

static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8)), "sizeof(T) is wrong");
}
*/

template <typename TFlag>
class bad_ip_flag {
    TFlag* px;
public:
    bad_ip_flag() noexcept = default;
    bad_ip_flag(TFlag* _px) : px(_px) {};

    TFlag&  operator[](uint8_t num) {
        //TODO check the range
        return px[num];
    }

    virtual ~bad_ip_flag() noexcept {};

    //TODO we will implement this methods in the future
    bad_ip_flag(const bad_ip_flag&) = delete;
    bad_ip_flag& operator=(const bad_ip_flag&) = delete;
    bad_ip_flag& operator=(const bad_ip_flag&) volatile = delete;
};

template <typename TIP>
class aIP {
    TIP* psa;
public:
    //aIP() noexcept = default;
    aIP(TIP* _px) : psa(_px) {};

    TIP&  operator[](uint8_t num) {
        //TODO check the range
        return psa[num];
    }

    virtual ~aIP() noexcept {};

    //TODO we will implement this methods in the future
    aIP(const aIP&) = delete;
    aIP& operator=(const aIP&) = delete;
    aIP& operator=(const aIP&) volatile = delete;
};

template<typename T>
class cxxx {
    T& x;
public:
    //cxxx() noexcept = default;
    //cxxx(T* _x){};
    cxxx(T& _x):x(_x) {};

    operator T() const noexcept {
        return x;
    }

    operator T() const volatile noexcept {
        return x;
    }

};

//std::ref()

class cyyy {};

void ff(string (&s)[3]) {
    for(int i=0; i<3; i++) {
        cout << s[i] << endl;
        //int x=0;
        s[i]=to_string(i+10);//itoa(i+10,(char*)&x,2);
    }
}
//template <typename T>

typedef list<int*> int_list;

void list_test() {
    int_list lst;

    for(int i=0; i<4; i++) lst.push_back(new int(i));
    cout<< lst.size()<<endl;

    for ( int_list::iterator i = lst.begin(); i != lst.end();) {
        if ((**i)%2 != 1) {
            delete *i;
            i = lst.erase( i );
        } else {
            ++i;
        }
    }
    cout<< lst.size()<<endl;
    for ( int_list::iterator i = lst.begin(); i != lst.end();) {
        delete *i;
        i = lst.erase( i );
    }
    cout<< lst.size()<<endl;

}

std::string gx(){
    static const std::string x = "123";
    cout << "\np to x = " <<&x << endl;
    return x;
}

void mmain() {

    std::string s0 = gx();    cout << s0 << "\t" << &s0 << endl;

    std::string const & s1 = gx();    cout << s1 << "\t" << &s1 << endl;

     std::string * s2 = gx();    cout << s2 << "\t" << &s2 << endl;

    s1 = "321";
    s2 = "456";

    //std::string & const s3 = gx();    cout << s3 << "\t" << &s3 << endl;

    list_test();

    ip_wrap ip1("1.1.1.1");
    cout << (string) ip1 << endl ;
    ip_wrap ip2("1.1.1.1");
    cout << (string) ip2 << endl ;
    cout << (ip1 == ip2 ) <<endl;

    string s[2][3];
    for(int i=0; i<3; i++) {
        s[0][i]=to_string(i);
    };

    ff(s[0]);
    cout << "-1------------\n";
    for(int i=0; i<3; i++) {
        cout<<s[0][i]<<endl;
    }
    cout << "-2------------\n";
    //bad_ip_flag<int>* pbip = new bad_ip_flag(88);
    int xxx[]= {1,2};
    int*pxxx=xxx;
    //bad_ip_flag<int> pp;
    bad_ip_flag<int> p(pxxx);
    bad_ip_flag<int> pp(pxxx);
    //pp=p;

    cout << p[0] <<"\t"<<p[1] << endl;

    int yy=88;
    cxxx<int> x(yy);
    cyyy y;
    cout << x << "\t"<< sizeof(x)<< "\t" << sizeof(y) <<endl;
}

void print_sizeof() {
    struct sockaddr_storage vsas;
    cout << sizeof(vsas) << endl;
    cout << sizeof(sas) << endl;
    struct sockaddr vsa;
    cout << sizeof(vsa) << endl;
    cout << sizeof(sa) << endl;

    static std::atomic<uint64_t> ax(0);

    cout << sizeof(ax) << endl;
    cout << sizeof(std::atomic<uint64_t>) << endl;
    cout << sizeof(std::atomic<bool>) << endl;
    cout << sizeof(bool) << endl;
    cout << "========================" << endl;

}

template <typename T>
static T fetchAndAdd2(volatile T* dest, T increment) {
    T result = increment;
    asm volatile ("lock xadd %[src], %[dest]"
                  : [dest] "+m" (*dest),
                  [src] "+r" (result)
                  :
                  : "memory", "cc");
    return result;
}

template <typename T>
static T fetchAndAdd(volatile T* dest, T increment) {
    return __atomic_fetch_add (dest, increment, __ATOMIC_SEQ_CST);
}

int main() {

    //size_t xxx=0;
    //fprintf(stdout,"%d\n",sizeof(xxx));

    uint32_t x=0;

    fetchAndAdd<uint32_t>(&x, 1);
    //__atomic_fetch_add(&x, 1);

    fprintf(stdout,"%d\n",x);

    print_sizeof();
    mmain();
    cout << "Hello world!" << endl;
    return 0;
}
