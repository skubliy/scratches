
/* 
 * File:   tarray.hpp
 * Author: SergueiKoubli
 *
 * Created on June 12, 2016, 8:52 PM
 */

#ifndef TARRAY_HPP
#define TARRAY_HPP

#include <cstdlib>
#include<iostream>
#include <ctime>
#include <vector>

namespace tarray {

static void fill_int_arr(int* m, size_t msz) {
    std::srand(time(0));
    for (size_t x = 0; x < msz; x++) {
        m[x] = std::rand() % 8 + 1;
    }
}

template <typename T>
static void print_vector(typename std::vector<T> ::const_iterator vb, typename std::vector<T> ::const_iterator ie) {
    for (typename std::vector<T> ::const_iterator it = vb; it != ie; it++)
        std::cout << *it << (it == ie - 1 ? "\n" : ",");
}

template <typename T>
static void print_vector(const std::vector<T> & v, size_t vsz = 0) {
    if (!vsz)
        vsz = v.end() - v.begin();

    print_vector<T> (v.begin(), v.begin() + vsz);
}

template <typename T>
static void print_array(const T* m, size_t msz) {
    for (size_t x = 0; x < msz; x++)
        std::cout << m[x] << (x == msz - 1 ? "\n" : ",");
}

};

#endif /* TARRAY_HPP */

