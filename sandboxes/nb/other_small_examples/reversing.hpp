/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   reversing.hpp
 * Author: someone
 *
 * Created on June 20, 2016, 6:49 PM
 */

#ifndef REVERSING_HPP
#define REVERSING_HPP

#include <string.h>

void reverse_iter(char* s) {
    std::cout << endl << __func__ << std::endl;
    size_t l = strlen(s) - 1;
    for (size_t i = 0; i < (l / 2); ++i) {
        char temp = s[i];
        s[i] = s[l - i];
        s[l - i] = temp;
    }
    cout << s << endl;
}

void reverse_rec(char* s, size_t l, size_t r) {
    if (l >= r)
        return;
    char t = s[l];
    s[l] = s[r];
    s[r] = t;
    reverse_rec(s, ++l, --r);
}

void reverse_recursive(char*s) {
    std::cout << endl << __func__ << std::endl;

    size_t sz = strlen(s) - 1;
    reverse_rec(s, 0, sz);
    cout << s << endl;
}

void reverse_by_xor(char* s) {
    std::cout << endl << __func__ << std::endl;

    size_t l = strlen(s);
    for (size_t i = 0; i < (l / 2); ++i) {
        s[i] ^= s[l - i];
        s[l - i] ^= s[i];
        s[i] ^= s[l - i];
    }
}

void reverse_by_stl(std::string& s) {
    std::cout << endl << __func__ << std::endl;

    std::reverse(s.begin(), s.end());
    std::cout << s << std::endl;
}

int revert_string_by_revert_iterator() {
    std::cout << endl << __func__ << std::endl;
    std::stringstream ss;
    std::string str("test string");
    std::copy(str.rbegin(), str.rend(), std::ostream_iterator<char> (ss));
    std::cout << ss.str() << std::endl;
}

#endif /* REVERSING_HPP */

