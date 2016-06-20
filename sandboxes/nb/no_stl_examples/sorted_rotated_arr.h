#ifndef SORTED_ROTATED_ARR_H
#define SORTED_ROTATED_ARR_H

using namespace std;

template <typename T>
class sorted_rotated_array {
public:

    static void rotate(T* a, size_t sz) {
        T first = a[0];

        for (int i = 0; i < sz - 1; i++) {
            a[i] = a[i + 1];
        }
        a[sz - 1] = first;
    }

    static T find_min_iteration(T* v, size_t sz) {
        if (sz == 1) // only one
            return v[0];

        size_t l = 0;
        size_t r = sz - 1;

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

    static T find_min_recursion(T* a, size_t l, size_t r) {
        // is not rotated
        if (r < l) return a[0];

        // only one
        if (r == l) return a[l];

        // mid
        int m = l + (r - l) / 2;

        // (m+1) is min
        if (m < r && a[m + 1] < a[m])
            return a[m + 1];

        // mid itself is minimum element
        if (m > l && a[m] < a[m - 1])
            return a[m];

        // to left half or right half
        if (a[r] > a[m])
            return find_min_recursion(a, l, m - 1);
        return find_min_recursion(a, m + 1, r);
    }
    //TODO there is bug here
    static int find_split_index(T* a, int l, int r) {
        int m;
        if (l > r) return -1;
        while (true) {

            m = l + (r - l) / 2;
            if (a[m] > a[m + 1])
                break;

            if (a[m] > a[l])
                l = m;
            if (a[r] > a[m])
                r = m;
            cout << l << " " << r << " " << m << "\n";
        }
        return m;
    }

};


#endif /* SORTED_ROTATED_ARR_H */

