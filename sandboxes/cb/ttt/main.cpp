#include <iostream>
#include <stdio.h>
#include <list>

using namespace std;

typedef long tarrt1[32];
typedef tarrt1 tarrt2[8];

class iref {
public:
    tarrt1& x;
    iref(tarrt1& _x):x(_x) {}
};

typedef iref tarref[4];

tarrt1& _ref(tarrt1& r) {
    return r;
};

void marr() {
    //tarrt2 arrt2;
    long arr[32][8];
    //tarrt2* arrt2=new(reinterpret_cast<void*>(&(arr[0][0]))) tarrt2() ;
    tarrt2 arrt2;
    iref _r0(arrt2[0]);
    iref _r1(arrt2[1]);
    iref _r2(arrt2[2]);
    iref _r3(arrt2[3]);

    tarref arref = {_r0, _r1, _r2, _r3};

    fprintf(stdout,"\n%lx\n%lx\n%d\n%d\n%p\n%p\n%p\n%p\n%p\n",
            sizeof(arrt2),
            sizeof(arref),
            &(arr[2][1]) - &(arr[0][0]),
            &(arrt2[2][1]) - &(arrt2[0][0]),
            &arr,
            &(arr[2][1]),
            &(arrt2[2]),
            &(arrt2[2][1]),
            &(reinterpret_cast<tarrt1&>(arrt2[2])[1])
           );

}

template <typename TV, typename TD>
struct xxx {
    struct xx {
        TV xv;
        TD xd;
    };
    xx ixx;
    typedef struct xx& xx_ref;
};

typedef xxx<long, long> x_long_long_t;

template <typename TDD>
//struct xxc {
void fxx() {
    typedef xxx<long, TDD> xxx_long_t;
    static xxx_long_t xxx_long;
    typename xxx_long_t::xx& rxx=xxx_long.ixx;
    //typename xxx_long_t::xx refxx;//=&xxx_long.ixx;

    //static x_long_long_t xxx_long;
    //typename x_long_long_t::xx* ref2;
};
//};

void fxx_long() {
    fxx<long>();
}
template <typename TD>
void print_list(list<TD>& nvl) {
    typedef typename list<TD>::iterator __iterator_t;
    for (__iterator_t i = nvl.begin(); i != nvl.end(); i++) {
        fprintf(stdout,"%d\n",*i);
    }
    fprintf(stdout,"\n=======\n");
};

template <typename TD>
void test_list () {
    list<TD> nv_lst;
    for (int i=1; i<3; i++) {
        for (int j=1; j<5; j++) {
            TD x(i*0x100+j);
            nv_lst.push_back(x);
        }
        cout << nv_lst.size()<<"\n";
    }

    print_list<TD>(nv_lst);

    cout << "\n========\n";
    for ( typename list<TD>::iterator i = nv_lst.begin(); i != nv_lst.end(); ) {
        if ((*i)&1) {
            i = nv_lst.erase( i );
            cout << nv_lst.size()<<"\t";
        } else {
            ++i;
        }
    }
    cout << endl <<nv_lst.size()<<endl;
    print_list<TD>(nv_lst);
};


class aclass{
int x;
public:
friend void inita(aclass& a);
int gx(){return x;}
};

void inita(aclass& a){
    a.x=55;
}

int main() {
    aclass a;
    inita(a);
    std:cout << a.gx() << endl;

//int main(int argc, char* argv[]) {
    //xxc<long> xxc_long;
    test_list<long>();
    return 0;
}

