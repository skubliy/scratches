
#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

#include "sorted_rotated_array.h"

using namespace std;

void test_heap() {
    int a[] = {8, 9, 1, 3, 4, 9, 4, 6, 7, 4, 6, 7};
    size_t sz = sizeof (a) / sizeof (*a);
    vector<int> v(a, a + sz);
    vh<int>::print_all(v);
    reverse(v.begin(), v.end());
    vh<int>::print_all(v);
    make_heap(v.begin(), v.end());
    vh<int>::print_all(v);
    sort_heap(v.begin(), v.end());
    vh<int>::print_all(v);
    reverse(v.begin(), v.end());
    vh<int>::print_all(v);
}

void test_sorted_rotated_array() {
    int a[] = {0, 1, 2, 3, 4, 5, 6, 7};
    vector<int>v(a, a + sizeof (a) / sizeof (*a));
    for (int i = 0; i < v.size(); i++) {
        //std::rotate(v.begin(),v.begin()+1,v.end());
        vh<int>::rotate(v);
        vh<int>::print_all(v);
        cout << "1min: " << vh<int>::find_min_iteration(v) << endl;
        cout << "2min: " << vh<int>::find_min_recursion(v, 0, v.size() - 1) << endl;
        int x = 2;
        cout << "idx: " << vh<int>::search(v, x) << endl;
    }
}

int main(int argc, char** argv) {
    test_heap();
    test_sorted_rotated_array();
    return 0;
}
