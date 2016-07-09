/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   sum2num_in_array.h
 * Author: someone
 *
 * Created on June 30, 2016, 6:02 PM
 */

#ifndef SUM2NUM_IN_ARRAY_H
#define SUM2NUM_IN_ARRAY_H

int cmpfunc(const void * a, const void * b) {
    return ( *(int*) a - *(int*) b);
}

int bsearch(int x, vector<int>& v, size_t l, size_t r) {
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

bool sum2num2(vector<int>& v, int asum) {

    for (size_t i = 0; i < v.size(); ++i) {
        int x = bsearch(asum - v[i], v, 0, v.size());
        if (x >= 0)
            return true;
    }
    return false;
}

//#include <tr1/unordered_map>
//#include <ext/hash_map>

#include <unordered_map>

bool sum2num(vector<int>& v, int asum) {
    std::unordered_map<int, size_t> hm;
    for (size_t k = 0; k < v.size(); ++k) {
        if (hm.count(k) == 1) {
            ++hm[k];
        } else {
            hm.insert(std::make_pair(k, 1));
        }
    }
    
    for (size_t i = 0; i < hm.size(); ++i) {
        int x = asum - hm[i];
        if (hm.count(x)>0)
            return true;
    }
    
    return false;
}

#endif /* SUM2NUM_IN_ARRAY_H */

