/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: someone
 *
 * Created on June 25, 2016, 12:03 PM
 */

extern "C" {
#include <pthread.h>
}

#include <iostream>
#include <stdint.h>
#include <unistd.h>

#include <linux/sched.h>

using namespace std;

#define BILLION (1000*1000*1000)
typedef void (*fp_t)(const char**, size_t*, int*);
#define NUM_TO_REPEAT (1000*1000)
#define NUM_THREADS 2

typedef union {
    uint64_t w64[2];
    uint32_t w32[4];
    uint16_t w16[8];
    uint8_t w8[16];
} data_t;

template <typename T>
class catomic {
private:
    volatile T data;

public:

    catomic(T v = 0) : data(v) {
    }
    //cvers(cvers& cv) {}

    catomic& operator=(const T& v) {
        data = v;
        return *this;
    }

    // prefix increment operator.

    catomic& operator++() {
        catomic::fetchAndAdd(&data, 1);
        return *this;
    }

    // postfix increment operator.

    catomic operator++(int) {
        catomic temp = *this;
        ++ * this;
        return temp;
    }

    operator T() {
        return data;
    }

    T fetchAndAdd(volatile T* dest, T increment) {
        T result = increment;
        asm volatile ("lock xadd %[src], %[dest]"
                    : [dest] "+m" (*dest),
                    [src] "+r" (result)
                    :
                    : "memory", "cc");
        return result;
    }
};

static catomic<uint64_t> reader_done = 0;
static catomic<uint64_t> version = 0;
static volatile data_t vdata;
static volatile uint64_t flag = false;

void zwait() {
    usleep(1000);
}

int cpu_rotate() {
    static volatile unsigned int cur_cpu = 8;
    cur_cpu -= 2;
    cur_cpu = cur_cpu < 2 ? 8 : cur_cpu;

    cpu_set_t my_set; // Define your cpu_set bit mask. 
    CPU_ZERO(&my_set); // Initialize it all to 0, i.e. no CPUs selected.
    CPU_SET(cur_cpu, &my_set); // set the bit that represents core 7. 
    sched_setaffinity(0, sizeof (cpu_set_t), &my_set); // Set affinity of this process to 
    // the defined mask, i.e. only cur_cpu.
    return cur_cpu;
}

void* thread_func(void* pfunc) {
    struct timespec tstart = {0, 0}, tend = {0, 0};
    int cur_cpu = cpu_rotate();
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    size_t real_cntr = 0;
    int cntr = 0;
    const char* fname;
    ((fp_t) pfunc)(&fname, &real_cntr, &cntr);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double te_ns = ((double) tend.tv_sec * BILLION + tend.tv_nsec);
    double ts_ns = ((double) tstart.tv_sec * BILLION + tstart.tv_nsec);
    cerr << ((te_ns - ts_ns) / NUM_TO_REPEAT) << " ns\t";
    cerr << fname << "[" << cur_cpu << "]\treal_cntr=" << real_cntr << "\tcntr=" << cntr << endl;
    reader_done++;
    return NULL;
}

void run_threads(fp_t fp) {

    pthread_t tharr[NUM_THREADS];
    reader_done = 0;
    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&tharr[i], NULL, thread_func, (void*) fp) != 0)
            cerr << "error " << i << endl;
    }

    while (reader_done < NUM_THREADS)
        usleep(1);

    //for (unsigned int i = 0; i < NUM_THREADS; i++) {
    //pthread_join(tharr[i],NULL) ;
    //}
}

void* writer_func(void*) {
    uint64_t real_counter = 0;
    int cur_cpu = cpu_rotate();
    cout << __func__ << "[" << cur_cpu << "]\n";
    reader_done = 0;
    version = 0;

    flag = false;
    while (reader_done < NUM_THREADS) {
        static uint64_t x = 0xaaaaaaaaaaaaaaaa;
        flag = true;
        ++real_counter;
        ++version;
        x ^= 0xffffffffffffffff;
        vdata.w64[0] = x;
        vdata.w64[1] = x;
        ++version;
        flag = false;
        for (int xx = 0; ++xx < 32;) {
        };
    }
    zwait();
    cout << " real_cnt = " << real_counter << "\tversion = " << (uint32_t) version << endl;
    return NULL;
}

void simple_reader_func(const char** fn, size_t* real_cntr, int* cnt) {
    *fn = __func__;
    for (size_t x = 0; x < NUM_TO_REPEAT;) {
        ++(*real_cntr);
        if (!flag) {
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[1];
            *cnt = (w0 != w1) ? (*cnt - 1) : (*cnt + 1);
            x++;
        }
    }
}

void proper_reader_func(const char** fn, size_t* real_cntr, int* cnt) {
    *fn = __func__;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
        ++(*real_cntr);
        if (!flag) {

            uint32_t version_before = version;
            if (version & 1)
                continue;
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[1];
            uint64_t version_after = version;
            if (version_before != version_after)
                continue;
            *cnt = (w0 != w1) ? (*cnt - 1) : (*cnt + 1);
            x++;
        }
    }
}

//31536000 seconds in year

int main(int argc, char* argv[]) {
    {
        cerr << "\n TEST SIMPLE READER\n";
        pthread_t writer_id;
        pthread_create(&writer_id, NULL, writer_func, NULL);
        run_threads(simple_reader_func);
        pthread_join(writer_id, NULL);
    }
    zwait();
    cerr << "\n lock-free algorithm demo\n";
    {
        cerr << "\n TEST 'PROPER' READER\n";
        pthread_t pr_writer_id;
        pthread_create(&pr_writer_id, NULL, writer_func, NULL);
        run_threads(proper_reader_func);
        pthread_join(pr_writer_id, NULL);
    }

    return 0;
}


