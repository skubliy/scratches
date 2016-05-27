#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "heap_sort.h"
#include "sorted_rotated_arr.h"
#include "tree_node_rec.h"
#include "btree.h"


using namespace std;

template <typename T>
class vh {
public:

    static int search(vector<T>& v, T target) { }

    static void print(vector<T>& v) {
        cout << __func__ << endl;
        //typename vector<T> vector_t;
        for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
            cout << *it << "  ";
        cout << endl;
    }

};

void test_heap() {
    cout << __func__ << endl;
    int arr[] = {8, 9, 9, 1, 3, 4, 4, 4, 6, 6, 7, 7};
    size_t sz = sizeof (arr) / sizeof (*arr);
    vector<int> v(arr, arr + sz);
    vh<int>::print(v);
    make_heap(v.begin(), v.end());
    cout << "initial max heap   : " << v.front() << '\n';
    vh<int>::print(v);

    int* arr2 = &v[0];
    vector<int> v2(arr2, arr2 + sizeof (arr) / sizeof (*arr));
    vh<int>::print(v2);

    sort_heap(v.begin(), v.end());
    vh<int>::print(v);
    heap<int>::sort(arr2);

    vector<int> v_sorted(arr2, arr2 + sizeof (arr) / sizeof (*arr));
    cout << "---------" << endl;
    vh<int>::print(v_sorted);
    cout << "---------" << endl;
    vh<int>::search(v, 8);
}

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

void test_sorted_rotated_array() {
    cout << __func__ << endl;
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);
    for (int i = 0; i < sz; i++) {
        ah<int>::rotate(a, sz);
        cout << "1min: " << ah<int>::find_min_iteration(a, sz) << endl;
        cout << "2min: " << ah<int>::find_min_recursion(a, 0, sz - 1) << endl;
        // int x = 2;
        // cout << "idx: " << ah<int>::search(v, x) << endl;
        cout << "split: " << ah<int>::find_split_index(a, 0, sz - 1) << endl;
    }
}

void test_tree() {
    cout << __func__ << endl;
    int arr[] = {8, 2, 9, 1, 5, 4, 6, 3, 7, 0};
    size_t sz = sizeof (arr) / sizeof (*arr);
    vector<int> v(arr, arr + sz);
    vh<int>::print(v);
    make_heap(v.begin(), v.end());
    vh<int>::print(v);
    cout << v.front() << endl;
    cout << "\n-----------------\n";
    for (int i = 0; i < 8; i++) {
        pop_heap(v.begin(), v.end());
        v.pop_back();
        vh<int>::print(v);
        cout << v.front() << endl;
    }
    cout << "\n-----------------\n";
}

void init_btree(btree<int>& bt) {
    cout << __func__ << endl;
    int a[] = {5, 2, 9, 1, 8, 3, 6, 7, 4, 0};
    size_t sz = sizeof (a) / sizeof (*a);
    vector<int> vv(a, a + sz);
    vh<int>::print(vv);
    make_heap(vv.begin(), vv.end());
    vh<int>::print(vv);
    cout << "\n-----------------\n";
    int arr[] = {5, 2, 9, 1, 8, 3, 6, 7, 4, 0};
    vector<int> v(arr, arr + sz);
    vh<int>::print(v);
    cout << "\n-----------------\n";
    for (vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        bt.insert(*it);
    }
}

void test_Btree() {
    btree<int> bt;
    init_btree(bt);
    cout << __func__ << endl;

    bt.print2();

   // bt.postorder_print();
    cout << "\n===\n";

    bt.in_order();
    cout << "\n===\n";

    bt.post_order();
    cout << "\n===\n";

    bt.pre_order();
    cout << "\n===\n";

    for (int i = 0; i < 8; i++) {

    }
    cout << "\n-----------------\n";
}

int main(int argc, char** argv) {
    test_Btree();
    /*
    test_tree();
    //print_num_sings(4);
    test_sorted_rotated_array();
    test_heap();
     */
    return 0;
}


