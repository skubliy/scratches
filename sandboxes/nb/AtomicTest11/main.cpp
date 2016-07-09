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
#include <unistd.h>

#include <iostream>
#include <atomic>
#include <linux/sched.h>
#include <chrono>
#include <thread>
#include <mutex>

#include "RandomDelay.h"

using namespace std;

#define BILLION (1000*1000*1000)
typedef void (*fp_t)(const char**, size_t*, int*);
#define NUM_TO_REPEAT (1000*1000)
#define NUM_TO_INCREMENT (1000*1000)
#define NUM_THREADS 2

std::atomic<unsigned int> writer_done(0);
std::atomic<unsigned int> reader_done(0);

typedef union {
    uint64_t w64[2];
    uint32_t w32[4];
    uint16_t w16[8];
    uint8_t w8[16];
} data_t;

template <typename TV>
class cvers {
    TV vers;
public:

    cvers(TV v = 0) : vers(v) {
    }
    //cvers(cvers& cv) {}

    cvers& operator=(const TV& v) {
        vers = v;
        return *this;
    }

    // prefix increment operator.

    cvers& operator++() {
        cvers::fetchAndAdd(&vers, 1);
        return *this;
    }

    // postfix increment operator.

    cvers operator++(int) {
        cvers temp = *this;
        ++ * this;
        return temp;
    }

    operator TV() {
        return vers;
    }

    TV fetchAndAdd(volatile TV* dest, TV increment) {
        TV result = increment;
        asm volatile ("lock xadd %[src], %[dest]"
                    : [dest] "+m" (*dest),
                    [src] "+r" (result)
                    :
                    : "memory", "cc");
        return result;
    }
};

cvers<uint32_t> version = 0;
static data_t data;
static volatile data_t vdata;
static volatile uint64_t flag = false;

void zwait() {
    usleep(1000);
}

int stick_this_thread_to_core(int core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);
    pthread_t current_thread = pthread_self();
    //cerr << "\ncore_id=" << core_id << "\tthread="<<current_thread<<endl;
    return pthread_setaffinity_np(current_thread, sizeof (cpu_set_t), &cpuset);
}

int cpu_rotate() {
    static int num_cores = sysconf(_SC_NPROCESSORS_ONLN);
    static volatile uint64_t cur_cpu = 2;
    __sync_fetch_and_add(&cur_cpu, 1);

    cur_cpu = cur_cpu > num_cores ? 3 : cur_cpu;

    stick_this_thread_to_core(cur_cpu);
    return cur_cpu;
}

void* thread_func(void* pfunc) {
    struct timespec tstart = {0, 0}, tend = {0, 0};
    int cur_cpu = cpu_rotate();
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    //auto start_time = chrono::high_resolution_clock::now();
    size_t real_cntr = 0;
    int cntr = 0;
    const char* fname;
    ((fp_t) pfunc)(&fname, &real_cntr, &cntr);
    //auto end_time = chrono::high_resolution_clock::now();
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double te_ns = ((double) tend.tv_sec * BILLION + tend.tv_nsec);
    double ts_ns = ((double) tstart.tv_sec * BILLION + tstart.tv_nsec);
    cerr << ((te_ns - ts_ns) / NUM_TO_REPEAT) << " ns\t";
    //cerr << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count()/NUM_TO_REPEAT <<endl;
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
    writer_done = 0;
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
        //randomDelay1.doBusyWork();
        //usleep(1);
        std::this_thread::yield();
        //for (int xx = 0; ++xx < 32;) {
    };
    zwait();
    cout << " real_cnt = " << real_counter << "\tversion = " << (uint32_t) version << endl;
    writer_done++;
    return NULL;
}

static std::mutex stdmutex;

void* locking_writer_func(void*) {
    uint64_t real_counter = 0;
    int cur_cpu = cpu_rotate();
    cout << __func__ << "[" << cur_cpu << "]\n";
    writer_done = 0;
    reader_done = 0;
    while (reader_done < NUM_THREADS) {
        ++real_counter;
        {
            std::lock_guard<std::mutex> lock(stdmutex);
            static uint64_t x = 0xaaaaaaaaaaaaaaaa;
            x ^= 0xffffffffffffffff;
            vdata.w64[0] = x;
            vdata.w64[1] = x;
        }
        std::this_thread::yield();
        //for (int xx = 0; ++xx < 32) {}
    };
    zwait();
    cout << " real_cnt = " << real_counter << "\tversion = " << (uint32_t) version << endl;
    writer_done++;
    return NULL;
}

void simple_reader_func(const char** fn, size_t* real_cntr, int* cnt) {
    *fn = __func__;
    for (size_t x = 0; x < NUM_TO_REPEAT;) {
        ++(*real_cntr);
        //randomDelay2.doBusyWork();
        if (!flag) {
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[1];
            *cnt = (w0 != w1) ? (*cnt - 1) : (*cnt + 1);
            x++;
        }
    }
}

void locking_reader_func(const char** fn, size_t* real_cntr, int* cnt) {
    *fn = __func__;
    for (size_t x = 0; x < NUM_TO_REPEAT;) {
        ++(*real_cntr);
        {
            std::lock_guard<std::mutex> lock(stdmutex);
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[1];
            *cnt = (w0 != w1) ? (*cnt - 1) : (*cnt + 1);
            x++;
        }
        //std::this_thread::yield();
    }
}

void proper_reader_func(const char** fn, size_t* real_cntr, int* cnt) {
    *fn = __func__;
    for (unsigned int x = 0; x < NUM_TO_REPEAT;) {
        //randomDelay2.doBusyWork();
        ++(*real_cntr);
        if (!flag) {

            uint32_t version_before = version;
            if (version & 1)
                continue;
            uint64_t w0 = vdata.w64[0];
            uint64_t w1 = vdata.w64[1];
            uint32_t version_after = version;
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
        //randomDelay1.Initialize();
        //randomDelay2.Initialize();
        pthread_create(&writer_id, NULL, writer_func, NULL);
        run_threads(simple_reader_func);
        pthread_join(writer_id, NULL);
    }
    zwait();
    cerr << "\n lock-free algorithm demo\n";
    {
        cerr << "\n TEST 'PROPER' READER\n";
        //randomDelay1.Initialize();
        //randomDelay2.Initialize();
        pthread_t pr_writer_id;
        pthread_create(&pr_writer_id, NULL, writer_func, NULL);
        run_threads(proper_reader_func);
        pthread_join(pr_writer_id, NULL);
    }

    return 0;
}

