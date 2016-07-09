/*
 * File:   heap_sort.h
 */

#ifndef HEAP_SORT_H
#define HEAP_SORT_H

class heap {
public:

    static void build_max_heap(int* m, int msz) {
        // call maxHeapify on all internal nodes
        int last = msz - 1;
        int parent = (last - 1) / 2;
        for (int i = parent; i >= 0; i--) {
            max_heapify(m, i, msz);
        }
    }

    static void sort_heap(int* m, int msz) {
        int* mm = m;
        int cnt1 = 0;
        if (m == NULL || msz < 2)
            return;

        //buildMaxHeap(m, msz);
        int hsz = msz;
        for (int i = msz - 1; i > 0; i--) {
            swap(m[0], m[i]);
            max_heapify(m, 0, --hsz);
        }
    }
private:

    static void max_heapify(int* m, int cur, int sz) {
        // Left child in heap
        int left = 2 * cur + 1;
        // Right child in heap
        int right = 2 * cur + 2;
        int largest = cur;

        if (left < sz && m[left] > m[cur]) {
            largest = left;
        }

        if (right < sz && m[right] > m[largest]) {
            largest = right;
        }

        if (largest != cur) {
            swap(m[cur], m[largest]);
            max_heapify(m, largest, sz);
        }
    }

    static void swap(int& i, int& j) {
        int tmp = i;
        i = j;
        j = tmp;
    }
};


/*
 template <typename T>
void max_heapify(T *a, size_t i, size_t msz) {
    T tmp = a[i];
    size_t j = 2 * i;
    while (j < msz) {
        if (j < msz && a[j + 1] > a[j])
            j = j + 1;
        if (tmp > a[j])
            break;
        else if (tmp <= a[j]) {
            a[j / 2] = a[j];
            j = 2 * j;
        }
    }
    a[j / 2] = tmp;
    return;
}

template <typename T>
void make_max_heap(T *m, size_t msz) {
    for (size_t i = msz / 2; i >= 1; i--) {
        max_heapify<T>(m, i, msz);
    }
}
 */


#endif /* HEAP_SORT_H */

