
/* 
 * File:   main.cpp
 * Author: someone
 *
 * Created on June 11, 2016, 4:05 PM
 */

#include <cstdlib>

void heap_sort(int m[], unsigned int msz) {
    if (msz == 0) // check if heap is empty
        return;

    int t; /* the temporary value */
    unsigned int n = msz, parent = msz / 2, index, child; /* heap indexes */
    /* loop until array is sorted */
    while (1) {
        if (parent > 0) {
            /* first stage - Sorting the heap */
            t = m[--parent]; /* save old value to t */
        } else {
            /* second stage - Extracting elements in-place */
            n--; /* make the heap smaller */
            if (n == 0) {
                return; /* When the heap is empty, we are done */
            }
            t = m[n]; /* save lost heap entry to temporary */
            m[n] = m[0]; /* save root entry beyond heap */
        }
        /* insert operation - pushing t down the heap to replace the parent */
        index = parent; /* start at the parent index */
        child = index * 2 + 1; /* get its left child index */
        while (child < n) {
            /* choose the largest child */
            if (child + 1 < n && m[child + 1] > m[child]) {
                child++; /* right child exists and is bigger */
            }
            /* is the largest child larger than the entry? */
            if (m[child] > t) {
                m[index] = m[child]; /* overwrite entry with child */
                index = child; /* move index to the child */
                child = index * 2 + 1; /* get the left child and go around again */
            } else {
                break; /* t's place is found */
            }
        }
        /* store the temporary value at its new location */
        m[index] = t;
    }
}
// Next code may be useful for test-only
#include<iostream>
#include<ctime>

#include "tutils/tarray.hpp" 

using namespace std;
using namespace tarray;

int main(int argc, char** argv) {
#if 1
    const int MSZ = 16;
    int m[MSZ];
    fill_int_arr(m, MSZ);
#else
    //int m[] = {8, 6, 6, 5, 6, 2, 5, 1, 5, 4, 3, 1, 1, 1, 8, 1};
    int m[] = {3, 3, 3, 1, 3, 1, 8, 5, 6, 3, 2, 3, 8, 8, 4, 7};
#endif 
    int msz = sizeof (m) / sizeof (*m);
    print_array<int>(m, msz);
    
    heap_sort(m, msz);
    
    print_array<int>(m, msz);
    
    return 0;
}

