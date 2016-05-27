/*
 * File:   heap_sort.h
 */

#ifndef HEAP_SORT_H
#define HEAP_SORT_H

template <typename T>
class heap2 {
public:

    static void sort(T* a) {
        int i;
        //typename T::size_type size = a.size();
        size_t sz = sizeof (a) / sizeof (*a);
        for (i = sz / 2 - 1; i >= 0; i--)
            shift(a, i, sz);

        for (i = sz - 1; i >= 1; i--) {
            swap(a[0], a[i]);
            shift(a, 0, i);
        }
    }

private:

    static void swap(T& v1, T& v2) {
        T tmp = v1;
        v1 = v2;
        v2 = v1;
    }

    static void shift(T* a, int i, int j) {
        bool done = false;
        int max;

        while ((i * 2 + 1 < j) && (!done)) {
            if (i * 2 + 1 == j - 1)
                max = i * 2 + 1;
            else if (a[i * 2 + 1] > a[i * 2 + 2])
                max = i * 2 + 1;
            else
                max = i * 2 + 2;

            if (a[i] < a[max]) {
                swap(a[i], a[max]);
                i = max;
            } else
                done = true;
        }
    }
};

//*******************************

template <typename T>
class heap1 {
public:

    static void sort(T* a) {

        size_t sz = sizeof (a) / sizeof (*a);
        for (int i = sz / 2 - 1; i >= 0; i--)
            shift(a, i, sz);

        for (int i = sz - 1; i >= 1; i--) {
            swap(a[0], a[i]);
            shift(a, 0, i);
        }
    }

private:

    static void swap(T& v1, T& v2) {
        T tmp = v1;
        v1 = v2;
        v2 = v1;
    }

    static void shift(T* a, int i, int j) {
        bool more = true;
        int max;

        while ((i * 2 + 1 < j) && (more)) {
            max = i * 2 + (i * 2 + 1 == j - 1) ? 1 : (a[i * 2 + 1] > a[i * 2 + 2]) ? 1 : 2;

            if (a[i] < a[max]) {
                swap(a[i], a[max]);
                i = max;
                continue;
            }

            more = false;
        }
    }
};

template <typename T>
class heap {
public:

    static void sort(T* a) {

        size_t sz = sizeof (a) / sizeof (*a);
        for (int i = sz / 2 - 1; i >= 0; i--)
            shift(a, i, sz);

        for (int i = sz - 1; i >= 1; i--) {
            swap(a[0], a[i]);
            shift(a, 0, i);
        }
    }

private:

    static void shift(T* a, int i, int j) {
        bool more = true;
        int max;

        while ((i * 2 + 1 < j) && (more)) {
            int i2 = i * 2;
            max = i2 + (i2 + 1 == j - 1) ? 1 : (a[i2 + 1] > a[i2 + 2]) ? 1 : 2;

            if (a[i] < a[max]) {
                swap(a[i], a[max]);
                i = max;
                continue;
            }

            more = false;
        }
    }

    static void swap(T& v1, T& v2) {
        T tmp = v1;
        v1 = v2;
        v2 = v1;
    }
};

#endif /* HEAP_SORT_H */

