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
#include <mutex>

using namespace std;

#define BILLION (1000*1000*1000)
typedef void (*fp_t)(const char**);
#define NUM_TO_INCREMENT (1000*1000)
#define NUM_THREADS 4

static volatile uint64_t gcntr = 0;

void zwait() {
    usleep(1000);
}

template <typename T>
T fetch_and_add(volatile T* dest, T increment) {
    T result = increment;
    asm volatile ("lock xadd %[src], %[dest]"
                : [dest] "+m" (*dest),
                [src] "+r" (result)
                :
                : "memory", "cc");
    return result;
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
/*
__declspec(naked) int __stdcall GetCoreNum()
{
   __asm
   {
      push ebx
      mov eax, 1
      cpuid
      mov eax, ebx
      pop ebx
      shr ebx, 0x18
      retn
   }
}
 */
static uint64_t reader_done = 0;

void* thread_func(void* pfunc) {
    struct timespec tstart = {0, 0}, tend = {0, 0};
    int cur_cpu = cpu_rotate();
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    const char* fname;
    ((fp_t) pfunc)(&fname);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double te_ns = ((double) tend.tv_sec * BILLION + tend.tv_nsec);
    double ts_ns = ((double) tstart.tv_sec * BILLION + tstart.tv_nsec);
    cerr << ((te_ns - ts_ns) / NUM_TO_INCREMENT) << " ns\t";
    //cerr << fname << "[" << GetCoreNum() << "]\tgcntr=" << gcntr << endl;
    //cerr << fname << "[" << cur_cpu << "]\t"<<pthread_self()<< "\tgcntr=" << gcntr << endl;
    cerr << fname << "[" << cur_cpu << "]\tgcntr=" << gcntr << endl;
    //cerr << fname << "\tgcntr=" << gcntr << endl;
    fetch_and_add<uint64_t>(&reader_done, 1);
    return NULL;
}

void run_threads(fp_t fp) {

    pthread_t tharr[NUM_THREADS];
    gcntr = 0;
    reader_done = 0;
    //static int nCPUs = sysconf(_SC_NPROCESSORS_ONLN);
    //cerr << "\n ncpus= " << nCPUs << "\n";
    //pthread_attr_t attr;
    //cpu_set_t cpus;
    //pthread_attr_init(&attr);

    for (unsigned int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&tharr[i], NULL, thread_func, (void*) fp) != 0)
            cerr << "error " << i << endl;

        //__sync_fetch_and_add(&cur_cpu, 1);
    }

    while (reader_done < NUM_THREADS)
        usleep(1);

    //for (unsigned int i = 0; i < NUM_THREADS; i++) {
    //pthread_join(tharr[i],NULL) ;
    //}
}

void simple_incrementor(const char** fn) {
    *fn = __func__;
    for (int x = 0; x < NUM_TO_INCREMENT; ++x)
        ++gcntr;
}

void fetch_and_add_incrementor(const char** fn) {
    *fn = __func__;
    for (int x = 0; x < NUM_TO_INCREMENT; ++x)
        fetch_and_add<uint64_t>(&gcntr, 1);
}

void sync_fetch_and_add_incrementor(const char** fn) {
    *fn = __func__;
    for (int x = 0; x < NUM_TO_INCREMENT; ++x)
        __sync_fetch_and_add(&gcntr, 1);
}

// static std::mutex amutex;
static pthread_once_t do_once = PTHREAD_ONCE_INIT;
static pthread_mutex_t ptmutex;

void mutex_init() {
    pthread_mutex_init(&ptmutex, NULL);
}

void pt_mtx_lock_incrementor(const char** fn) {
    *fn = __func__;

    pthread_once(&do_once, mutex_init);

    for (int x = 0; x < NUM_TO_INCREMENT; ++x) {
        pthread_mutex_lock(&ptmutex);
        ++gcntr;
        pthread_mutex_unlock(&ptmutex);
    }
}

static std::mutex stdmutex;

void std_mtx_lock_incrementor(const char** fn) {
    *fn = __func__;

    for (int x = 0; x < NUM_TO_INCREMENT; ++x) {
        std::lock_guard<std::mutex> alock(stdmutex);
        ++gcntr;
    }
}

//31536000 seconds in year

int main(int argc, char* argv[]) {
    cerr << "\n timing demo\n";
    run_threads(simple_incrementor);
    zwait();
    cerr << "simple_incr: " << gcntr << "\n\n";
    sleep(1);
    run_threads(sync_fetch_and_add_incrementor);
    zwait();
    cerr << "sync_fetch_and_add: " << gcntr << "\n\n";
    sleep(1);
    run_threads(fetch_and_add_incrementor);
    zwait();
    cerr << "fetch_and_add: " << gcntr << "\n\n";
    sleep(1);
    run_threads(pt_mtx_lock_incrementor);
    zwait();
    cerr << "pt_mtx_locked_inc: " << gcntr << "\n\n";
    sleep(1);
    run_threads(std_mtx_lock_incrementor);
    zwait();
    cerr << "std_mtx_locked_inc: " << gcntr << "\n\n";

    return 0;
}




