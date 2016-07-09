/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   factory_method.hpp
 * Author: someone
 *
 * Created on June 20, 2016, 12:38 PM
 */

#ifndef FACTORY_METHOD_HPP
#define FACTORY_METHOD_HPP

#include <iostream>
#include <string>
using namespace std;

class Product {
public:
    virtual string getName() = 0;

    virtual ~Product() { }
};

class ProductA : public Product {
public:

    string getName() {
        return "ProductA";
    }
};

class ProductB : public Product {
public:

    string getName() {
        return "ProductB";
    }
};

class Factory {
public:
    virtual Product* factoryMethod() = 0;
};

class FactoryA : public Factory {
public:

    Product* factoryMethod() {
        return new ProductA();
    }
};

class FactoryB : public Factory {
public:

    Product* factoryMethod() {
        return new ProductB();
    }
};

void test_factory_method() {
    static const size_t count = 2;
    Factory * factories[count] = {new FactoryA(), new FactoryB()};

    for (size_t i = 0; i < count; i++) {
        Product* product = factories[i]->factoryMethod();
        cout << product->getName() << endl;
        delete product;
    }
    
    for (size_t i = 0; i < count; i++) {
        if (factories[i])
            delete factories[i];
    }
    return;
}

#endif /* FACTORY_METHOD_HPP */

