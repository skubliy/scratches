#ifndef SORTED_ROTATED_ARRAY_H
#define SORTED_ROTATED_ARRAY_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;
// 3456012
// 5601234

template<typename T>
class vh {
public:

    static void print_all(vector<T>& v) {
        for (typename vector<T>::const_iterator it = v.begin(); it != v.end(); ++it)
            cout << *it << "  ";
        //cout << endl;
    }

    static void rotate(vector<T>& v) {
        size_t sz = v.size();
        T first = v[0];

        for (int i = 0; i < sz - 1; i++) {
            v[i] = v[i + 1];
        }
        v[sz - 1] = first;

        cout << "[ ";
        vh<T>::print_all(v);
        cout << " ]\n";
    }

    static T find_min_iteration(vector<T>& v) {
        if (v.size() == 1) // only one
            return v[0];

        size_t l = 0;
        size_t r = v.size() - 1;

        if (v[l] < v[r]) return v[0]; // not rotated

        while (l <= r) {
            if (r - l == 1)
                return v[r];
            size_t m = l + (r - l) / 2;
            if (v[m] > v[r])
                l = m;
            else
                r = m;
        }

        return v[l];
    }

    static T find_min_recursion(vector<T>& v, int l, int r) {
        // is not rotated
        if (r < l) return v[0];

        // only one element left
        if (r == l) return v[l];

        // mid
        int m = l + (r - l) / 2;

        // (m+1) is minimum element
        if (m < r && v[m + 1] < v[m])
            return v[m + 1];

        // mid itself is minimum element
        if (m > l && v[m] < v[m - 1])
            return v[m];

        // to left half or right half
        if (v[r] > v[m])
            return find_min_recursion(v, l, m - 1);
        return find_min_recursion(v, m + 1, r);
    }

    static int search(vector<T>& v, T& x) {
        int l = 0;
        int r = v.size() - 1;

        while (l <= r) {
            int m = l + ((r - l) / 2);
            if (v[m] == x) return m;

            // the bottom half is sorted
            if (v[l] <= v[m]) {
                if (v[l] <= x && x < v[m])
                    r = m - 1;
                else
                    l = m + 1;
            }// the upper half is sorted
            else {
                if (v[m] < x && x <= v[r])
                    l = m + 1;
                else
                    r = m - 1;
            }
        }
        return -1;
    }

};


#endif /* SORTED_ROTATED_ARRAY_H */

