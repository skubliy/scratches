
#include <iostream>
#include <atomic>
#include <chrono>

using namespace std;



inline uint64_t stack_x_read(){
    uint64_t x=0;
    return x;
}

inline uint64_t static_x_read(){
    static uint64_t x=0;
    return x;
}

inline uint64_t static_mb_x_read(){
    static uint64_t x=0;
    asm volatile ("" : : : "memory");
    //asm volatile("mfence" : : : "memory");
    return x;
}

inline uint64_t static_syn_syn_x_read(){
    static uint64_t x=0;
    __sync_synchronize();
    //asm volatile ("mfence" ::: "memory")
    return x;
}

inline uint64_t static_lfence_x_read(){
    static uint64_t x=0;
    asm volatile("lfence" : : : "memory");
    return x;
}

inline uint64_t static_lfence_inc_x_read(){
    static uint64_t x=0;
    asm volatile("lfence" : : : "memory");
    return x++;
}


inline uint64_t volatile_static_x_read(){
    volatile static uint64_t x=0;
    return x;
}

inline uint64_t atomic_x_read(){
    static std::atomic<uint64_t> x(0);
    return x;
}

inline uint64_t atomic_inc_x_read(){
    static std::atomic<uint64_t> x(0);
    return x++;
}

#define BILLION 1000*1000*1000
#define NUM 1000000000

void time_test(uint64_t (*f)(void)){
    //struct timespec tstart = {0,0}, tend = {0,0};
    //clock_gettime(CLOCK_MONOTONIC, &tstart);
    auto start_time = chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < NUM; i++)
        f( );
    auto end_time = chrono::high_resolution_clock::now();
    cout << chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count()/NUM <<endl;
    /*
    clock_gettime(CLOCK_MONOTONIC, &tend);
    double te_ns = ((double)tend.tv_sec * BILLION + tend.tv_nsec);
    double ts_ns = ((double)tstart.tv_sec * BILLION + tstart.tv_nsec);
    cout << (( te_ns - ts_ns ) / NUM ) << endl;
    */
}


int main()
{
#if 0
    cout << "stack var:          "; time_test(stack_x_read);
    cout << "static var:         "; time_test(static_x_read);
    cout << "volatile var:       "; time_test(volatile_static_x_read);
    cout << "static mb var:      "; time_test(static_mb_x_read);
    cout << " syn syn var:       "; time_test(static_syn_syn_x_read);
    cout << " lfence var:        "; time_test(static_lfence_x_read);
    cout << " lfence inc var:    "; time_test(static_lfence_inc_x_read);
    cout << "std::atomic var:    "; time_test(atomic_x_read);
    cout << "std::atomic inc var:"; time_test(atomic_inc_x_read);
#endif
    cout << "std::atomic inc var:"; time_test(mins);



#if 0
    std::atomic<uint64_t> bad_ip_flags(0x301);
    uint64_t prev_bad_ip_flags=0x101;

    std::atomic<uint64_t>* p_bad_ip_flags=&bad_ip_flags;

    cout<< hex <<bad_ip_flags.fetch_xor(prev_bad_ip_flags) << endl;
    cout<< hex <<bad_ip_flags << endl;
    //std::atomic_fetch_xor()
#endif



    cout << "The end!" << endl;
    return 0;
}



