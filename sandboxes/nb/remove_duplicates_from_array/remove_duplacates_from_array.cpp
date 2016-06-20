/* 
 * File:   remove_duplicates_from_array.cpp
 * Author: Serguei Koubli
 *
 * Created on June 11, 2016, 9:59 AM
 */

#include <cstdlib>
#include <iostream>

#include <algorithm>
#include <vector>
#include <iterator> 

#include "tutils/tarray.hpp" 

using namespace std;

template<typename T>
size_t remove_duplicates_c(T* m, size_t msz) {
    size_t i = 0;
    for (size_t x = 0; x < msz; x++) {
        bool flag = true;
        for (size_t j = 0; j < i; j++) {
            if (m[x] == m[j]) {
                flag = false;
                break;
            }
        }
        if (flag) {
            m[i++] = m[x];
        }
    }
    return i;
}

// This solution is shortest by design time and number of lines of code ;)
// The C++ standard does not specify which sorting algorithm was used
// But most likely:
// time complexity practically O(n log n) in worth case O(n^2)  
// memory O(n)

template<typename T>
void remove_duplicates_using_sort(vector<T>& v) {
    sort(v.begin(), v.end());
    v.resize(unique(v.begin(), v.end()) - v.begin());
}

// This solution generally the best 
// Guarantee time complexity O(n log n) 
// and no extra memory O(1)

template<typename T>
void remove_duplicates_using_heap_sort(vector<T>& v) {

    make_heap(v.begin(), v.end());

    sort_heap(v.begin(), v.end());
    v.resize(unique(v.begin(), v.end()) - v.begin());
}

// This is "costom-made" function 
// This solution can be most effective if many duplicates in the array
// In general case time complexity up to O(n^2)
// no extra memory O (1)

template<typename T>
void remove_duplicates(vector<T>& v) {
    typename vector<T>::iterator x = v.begin();
    for (typename vector<T>::iterator i = v.begin(); i != v.end(); i++) {
        bool flag = true;
        for (typename vector<T>::iterator j = v.begin(); j != i; j++) {
            if (*i == *j) {
                flag = false;
                break;
            }
        }
        if (flag) {
            *x = *i;
            x++;
        }
    }

    v.resize(x - v.begin());
}

// Next code may be useful for test-only

using namespace tarray;

template<typename T>
void rem_dup_tester(const T* m, size_t msz) {

    static void (*rdf[4])(vector<T>& v) = {
        remove_duplicates,
        remove_duplicates_using_sort,
        remove_duplicates_using_heap_sort,
        NULL
    };

    print_array<T>(m, msz);
    size_t i = 0;
    while (rdf[i]) {
        vector<T> v(m, m + msz);
        (*rdf[i++])(v);
        print_array<T>(&v[0], v.size());
    }
}

int main(int argc, char** argv) {
#if 1
    const int MSZ = 16;
    int m[MSZ];
    fill_int_arr(m, MSZ);
#else
    //int m[] = {8, 6, 6, 5, 6, 2, 5, 1, 5, 4, 3, 1, 1, 1, 8, 1};
    int m[] = {3, 3, 3, 1, 3, 1, 8, 5, 6, 3, 2, 3, 8, 8, 4, 7};
#endif    

    rem_dup_tester<int>(m, sizeof (m) / sizeof (*m));

    return 0;
}
