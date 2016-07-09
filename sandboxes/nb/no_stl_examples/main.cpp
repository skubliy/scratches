#include <cstdlib>
#include <iostream>
//#include <unordered_map>
#include <vector>
#include <algorithm>

#include "heap.h"
#include "sorted_rotated_arr.h"
#include "tree_node_rec.h"
#include "btree.h"
#include "list_node.h"
#include "card_shifter.h"
#include "sum2num_in_array.h"
#include "binary_search.h"

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

void test_card_shifter() {
    cout << __func__ << endl;
    static const size_t msz = 36;
    int cards[36];
    for (size_t x = 0; x < msz; x++) {
        cards[x] = x;
    }
    card_shifter(cards, msz);
    print_array<int>(cards, msz);
}

void test_sum2num_in_array() {
    cout << __func__ << endl;
#if 1
    const int MSZ = 16;
    int m[MSZ];
    fill_int_arr(m, MSZ);
#else
    //int m[] = {8, 6, 6, 5, 6, 2, 5, 1, 5, 4, 3, 1, 1, 1, 8, 1};
    int m[] = {3, 3, 3, 1, 3, 1, 8, 5, 6, 3, 2, 3, 8, 8, 4, 7};
#endif
    size_t sz = sizeof (m) / sizeof (*m);
    vector<int> v(m, m + sz);
    print_vector(v);

    cout << sum2num(v, 11) << endl;

    sort(v.begin(), v.end());
    print_vector(v);
    cout << sum2num2(v, 11) << endl;
}

void test_binary_search() {
    cout << __func__ << endl;
#if 0
    const int MSZ = 16;
    int m[MSZ];
    fill_int_arr(m, MSZ);
#else
    //int m[] = {8, 6, 6, 5, 6, 2, 5, 1, 5, 4, 3, 1, 1, 1, 8, 1};
    //int m[] = {3, 3, 3, 1, 3, 1, 8, 5, 6, 3, 2, 3, 8, 8, 4, 7};
    int m[] = {4, 3, 1, 5, 2, 5, 7, 5, 1, 8, 2, 3, 7, 4, 5, 6};
#endif
    size_t sz = sizeof (m) / sizeof (*m);
    print_array<int>(m, sz);
    cout << binary_search(5, m, 0, sz - 1) << endl;
    cout << binary_search_recursive(5, m, 0, sz - 1) << endl;
}

void test_btn() {
    cout << __func__ << endl;
#if 0
    const int MSZ = 16;
    int m[MSZ];
    fill_int_arr(m, MSZ);
#else
    //int m[] = {8, 6, 6, 5, 6, 2, 5, 1, 5, 4, 3, 1, 1, 1, 8, 1};
    //int m[] = {3, 3, 3, 1, 3, 1, 8, 5, 6, 3, 2, 3, 8, 8, 4, 7};
    int m[] = {6, 3, 4, 4, 4, 4, 2, 1, 2, 4, 3, 3, 5, 7, 7, 8};
    //int m[] = {6, 3, 4, 2, 1, 9, 5, 7, 8};
#endif  
    int msz = sizeof (m) / sizeof (*m);
    print_array<int>(m, msz);
    cout << "-------\n";
    btn* bt = NULL;
    for (int i = 0; i < msz; i++) {
        btn::insert(bt, m[i]);
        //btn::insert2(&bt, m[i]);
    }

    cout << btn::height(bt) << endl;
    cout << endl;

    string s("");
    btn::print(bt, s, true);
    btn::in_order(bt);
    cout << endl;
    btn::pre_order(bt);
    cout << endl;
    btn::post_order(bt);
    cout << endl;

    cout << btn::getmax(bt) << endl;
    cout << endl;

    cout << btn::secondmax(bt) << endl;
    cout << endl;

    cout << btn::search(bt, 6) << endl;
    cout << endl;

    cout << btn::is_bst(bt, INT_MIN, INT_MAX) << endl;
    cout << endl;
    
    btn::print_by_level(bt);
    cout << endl;
    

    btn::destroy(bt);
    cout << bt << endl;
    btn::print(bt, s, true);
    cout << "------\n";

}

int main(int argc, char** argv) {
    //std::unordered_map<int,int> hm;
    test_btn();
    test_binary_search();
    test_sum2num_in_array();
    test_card_shifter();
    test_list();
    print_num_sings(8);
    test_heap<heap>();
    //theap< heap2<int> >();
    test_sorted_rotated_array();

    return 0;
}



