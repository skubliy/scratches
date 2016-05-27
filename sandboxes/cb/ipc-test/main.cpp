#include <stdio.h>
#include <stdlib.h>
#include <iostream>
//#include <list>

#include "ip_wrap.hpp"

using namespace std;


int main(int argc, char* argv[]) {
    std::string str;
    {
        ip_wrap ip("11.12.13.00");



    }

    cout  << "\nThe end!\n" << str.c_str() << endl;
    return 0;
}
