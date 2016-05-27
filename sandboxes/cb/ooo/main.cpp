
#include <iostream>
#include <atomic>
#include <chrono>

using namespace std;
//private:
//HomeForSale(const HomeForSale&); // только объявления
//HomeForSale& oparetor=( const HomeForSale&)
#define nn 4
time_t x[nn];

void fx(time_t xx[]){
   for (int i=0; i<nn; i++){
   xx[i]=time(NULL);
   }
}

class aaa{
int x;
public:
friend void xxx(aaa& a);
int gx(){return x;}
}

void xxx(aaa& a){
    a.x=55;
}

int main() {
    aaa a;
    xxx(a);
    std:cout << a.gx() << endl;


   for (int i=0; i<nn; i++){
   x[i]=0;
   }
    fx(x);
   for (int i=0; i<nn; i++){
  cout << x[i] << endl;
   }
    return 0;
}
