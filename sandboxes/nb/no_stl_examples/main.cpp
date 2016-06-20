#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "heap.h"
#include "sorted_rotated_arr.h"
#include "tree_node_rec.h"
#include "btree.h"


// Next code may be useful for test-only

#include "tutils/tarray.hpp" 

using namespace std;
using namespace tarray;

void print_num_sings(int n) {
    cout << __func__ << endl;
    char arr[n];
    for (int x = 0; x < n - 1; x++) {
        arr[x] = ' ';
    }
    arr[n] = '\0';
    for (int x = 1; x <= n; x++) {
        arr[n - x] = '#';
        cout << arr << endl;
    }
}

template <typename T>
void test_heap() {
    cout << __func__ << endl;
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
    T::build_max_heap(m, msz);
    print_array<int>(m, msz);
    T::sort_heap(m, msz);
    print_array<int>(m, msz);

}

void test_sorted_rotated_array() {
    cout << __func__ << endl;
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
    heap::build_max_heap(m, msz);
    heap::sort_heap(m, msz);
    print_array<int>(m, msz);
    cout << "------\n";
    for (int i = 0; i < msz; i++) {
        sorted_rotated_array<int>::rotate(m, msz);
        print_array<int>(m, msz);
        cout << "1min: " << sorted_rotated_array<int>::find_min_iteration(m, msz) << endl;
        cout << "2min: " << sorted_rotated_array<int>::find_min_recursion(m, 0, msz - 1) << endl;
        // int x = 2;
        // cout << "idx: " << sorted_rotated_array<int>::search(v, x) << endl;

        //TODO there is bug here
        //cout << "split: " << sorted_rotated_array<int>::find_split_index(m, 0, msz - 1) << endl;
    }
}

void test_tree() {
    cout << __func__ << endl;
    int arr[] = {8, 2, 9, 1, 5, 4, 6, 3, 7, 0};
    size_t sz = sizeof (arr) / sizeof (*arr);
    vector<int> v(arr, arr + sz);
    print_vector(v);
    make_heap(v.begin(), v.end());
    print_vector(v);
    cout << v.front() << endl;
    cout << "\n-----------------\n";
    for (int i = 0; i < 8; i++) {
        pop_heap(v.begin(), v.end());
        v.pop_back();
        print_vector(v);
        cout << v.front() << endl;
    }
    cout << "\n-----------------\n";
}

void test_btree() {
    cout << __func__ << endl;
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
    heap::build_max_heap(m, msz);
    print_array<int>(m, msz);
    cout << "-------\n";
    btree<int> bt;
    for (int i = 0; i < msz; i++) {
        bt.insert(m[i]);
    }

    bt.print2();
    // bt.postorder_print();

    cout << "\nin_order:\t";
    bt.in_order();


    cout << "\npost_order:\t";
    bt.post_order();

    cout << "\npre_order:\t";
    bt.pre_order();

    for (int i = 0; i < 8; i++) {

    }
    cout << "\n-----------------\n";
}

int main(int argc, char** argv) {
    print_num_sings(8);
    test_heap<heap>();
    //theap< heap2<int> >();
    test_sorted_rotated_array();
    test_btree();
    return 0;
}



