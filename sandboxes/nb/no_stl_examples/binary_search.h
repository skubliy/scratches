/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   binary_serch.h
 * Author: someone
 *
 * Created on July 2, 2016, 9:19 AM
 */

#ifndef BINARY_SERCH_H
#define BINARY_SERCH_H

int binary_search(int x, int* v, size_t l, size_t r) {
    while (r - l > 1) {
        size_t m = l + (r - l) / 2;
        if (x == (v[m])) {
            return m;
        }

        if (x < v[m])
            r = m;
        else
            l = m;
    }
    return -1;
}

size_t binary_search_recursive(int x, int v[], size_t l, size_t r) {
    size_t m = (l + r) / 2;
    if (v[m] == x)
        return m;

    if (x > v[m])
        return binary_search_recursive(x, v, m, r);

    return binary_search_recursive(x, v, l, m);
    
}

#endif /* BINARY_SERCH_H */

