/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: someone
 *
 * Created on June 21, 2016, 4:52 PM
 */

#include <cstdlib>

using namespace std;

class Base
{
public:
    Base()
    {
        base_func();
    }
    void base_func()
    {
        func(); // pure virtual function call exception
    }
    virtual void func() = 0;
};

class Derived : public Base
{
public:
    virtual void func()
    {
    }
};

int main(int argc, char** argv) {

    return 0;
}

